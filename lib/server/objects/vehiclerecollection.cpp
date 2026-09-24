#include "vehiclerecollection.h"
#include "base/objcontainer.h"
#include <stdexcept>
#include <algorithm>
#include <core/ini.h>
#include <core/kernel.h>
#include <m3dapp.h>
#include <math/geom2d.h>
#include "base/globalproperties.h"
#include "base/prototypemanager.h"
#include "vehicle.h"

namespace ai
{
    namespace
    {
        // How far back, in game time units, the recollection reaches.
        float const RECOLLECTION_HORIZONT = 3.0f;

        // The position on the line through two recollections at the given time. Used both to
        // interpolate between them and to extrapolate beyond them.
        CVector LerpRecollections(
            VehicleRecollection::ReollectionItem const& a,
            VehicleRecollection::ReollectionItem const& b,
            float time)
        {
            float const t = time - a.time;
            float const invDuration = 1.0f / (b.time - a.time);
            CVector result;
            result.x = a.pos.x + (b.pos.x - a.pos.x) * invDuration * t;
            result.y = a.pos.y + (b.pos.y - a.pos.y) * invDuration * t;
            result.z = a.pos.z + (b.pos.z - a.pos.z) * invDuration * t;
            return result;
        }
    }  // namespace

    RT_CLASS_EXPORTS_BEGIN(VehicleRecollection)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(VehicleRecollection);

    ai::Obj* VehicleRecollectionPrototypeInfo::CreateTargetObject() const
    {
        return new VehicleRecollection(*this);
    }

    VehicleRecollectionPrototypeInfo::VehicleRecollectionPrototypeInfo() = default;

    bool VehicleRecollectionPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        return ai::PrototypeInfo::LoadFromXML(xmlFile, xmlNode) != 0;
    }

    VehicleRecollection::ReollectionItem::ReollectionItem(CVector const& _pos, float _time)
    {
        pos = _pos;
        time = _time;
    }

    void VehicleRecollection::ReollectionItem::SaveToXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode* xmlNode) const
    {
        // RVA 0x7D57F0
        xmlNode->SetAttribute("Position", CStr(pos).c_str());
        xmlNode->SetAttribute("Time", CStr(time).c_str());
    }

    void VehicleRecollection::ReollectionItem::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const* xmlNode)
    {
        // RVA 0x7D5170
        m3d::SafeVectorAttrib(pos, xmlNode, "Position");
        m3d::SafeFloatAttrib(time, xmlNode, "Time");
    }

    void VehicleRecollection::LoadRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        // RVA 0x7D6AF0 - NOTE: the loaded items are appended to whatever is already recorded.
        Obj::LoadRuntimeValues(xmlFile, xmlNode);
        m3d::SafeIntAttrib(m_vehicleId, xmlNode, "VehicleId");

        ref_ptr itemsNode = xmlFile->CreateNode();
        xmlNode->GetFirstChild(itemsNode, "Items");
        if (itemsNode->IsEmpty())
        {
            return;
        }

        ref_ptr itemNode = xmlFile->CreateNode();
        for (itemsNode->GetFirstChild(itemNode, "Item"); !itemNode->IsEmpty();
             itemNode->GetNextSibling(itemNode, "Item"))
        {
            ReollectionItem newRecollection(ZeroVector, 0.0f);
            newRecollection.LoadFromXML(xmlFile, itemNode);
            m_recollectionItems.push_back(newRecollection);
        }
    }

    void VehicleRecollection::RenderDebugInfo() const
    {
        // RVA 0x7D61D0 - the recorded track in red and green, and in yellow the position enemies
        // currently aim at.
        for (auto const& item : m_recollectionItems)
        {
            M3D_APP->DrawBoundingRadius(item.pos, 1.5f, 0xFFFF0000);
        }
        for (size_t i = 1; i < m_recollectionItems.size(); ++i)
        {
            M3D_APP->DrawLine(m_recollectionItems[i - 1].pos, m_recollectionItems[i].pos, 0xFF00FF00);
        }

        float const time = theObjects->GetGameTimeDiff() -
            theGlobProp.m_gameTimeMult * theGlobProp.GetCoeffsForCurrentDifficultyLevel().m_enemiesShootingDelay;
        M3D_APP->DrawBoundingRadius(GetRecollectionPosition(time), 2.5f, 0xFFFFFF00);
    }

    void VehicleRecollection::Update(float, unsigned)
    {
        // RVA 0x7D6950 - drops recollections older than the horizon, records the vehicle's centre
        // about every 0.3 game time units and removes itself once the vehicle is gone.
        float const time = theObjects->GetGameTimeDiff();

        while (!m_recollectionItems.empty() &&
               time - theGlobProp.m_gameTimeMult * RECOLLECTION_HORIZONT > m_recollectionItems.front().time)
        {
            m_recollectionItems.erase(m_recollectionItems.begin());
        }

        if (m_recollectionItems.empty() ||
            time > theGlobProp.m_gameTimeMult * 0.30000001f + m_recollectionItems.back().time)
        {
            // NOTE: the object is not type-checked; GetGeometricCenter is reached through the
            // PhysicObj vtable.
            auto* vehicle = static_cast<PhysicObj*>(theObjects->GetEntityByObjId(m_vehicleId));
            if (vehicle)
            {
                m_recollectionItems.push_back(ReollectionItem(vehicle->GetGeometricCenter(), time));
            }
            else
            {
                Remove();
            }
        }
    }

    void VehicleRecollection::Clear()
    {
        // RVA 0x7D6710 - releases the storage as well.
        std::vector<ReollectionItem>().swap(m_recollectionItems);
    }

    ai::VehicleRecollectionPrototypeInfo const* VehicleRecollection::GetPrototypeInfo() const
    {
        // RVA 0x7D5CA0 - NOTE: the prototype is cast without a type check.
        return static_cast<VehicleRecollectionPrototypeInfo const*>(
            thePrototypeManager->GetPrototypeInfo(GetPrototypeId()));
    }

    m3d::Class* VehicleRecollection::GetClass() const
    {
        return RT_CLASS_LOCAL(VehicleRecollection);
    }

    VehicleRecollection::VehicleRecollection(ai::VehicleRecollectionPrototypeInfo const& prototypeInfo) :
        Obj(prototypeInfo)
    {
        m_vehicleId = -1;
    }

    CVector VehicleRecollection::GetRecollectionPosition(float time) const
    {
        // RVA 0x7D5D10 - where the vehicle was at the given game time: interpolated between the two
        // recollections around it, or extrapolated along the first or last pair.
        if (m_recollectionItems.empty())
        {
            // NOTE: the object is used as a PhysicObj without a type check.
            auto* physicObj = static_cast<PhysicObj*>(theObjects->GetEntityByObjId(m_vehicleId));
            if (physicObj)
            {
                return physicObj->GetPosition();
            }
            return ZeroVector;
        }

        size_t const count = m_recollectionItems.size();
        if (count == 1)
        {
            return m_recollectionItems.front().pos;
        }

        size_t found = 1;
        for (; found < count; ++found)
        {
            geom1d::Segment1<float> const interval(
                m_recollectionItems[found - 1].time, m_recollectionItems[found].time);
            if (interval.isPointOn(time))
            {
                break;
            }
        }

        if (found != count)
        {
            return LerpRecollections(m_recollectionItems[found - 1], m_recollectionItems[found], time);
        }
        if (m_recollectionItems.front().time > time)
        {
            return LerpRecollections(m_recollectionItems[0], m_recollectionItems[1], time);
        }
        if (time > m_recollectionItems.back().time)
        {
            return LerpRecollections(m_recollectionItems[count - 2], m_recollectionItems[count - 1], time);
        }
        return ZeroVector;
    }

    m3d::Class* VehicleRecollection::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Obj);
    }

    Vehicle* VehicleRecollection::GetVehicle() const
    {
        // RVA 0x7D5CD0 - NOTE: the object is cast without a type check.
        return static_cast<Vehicle*>(theObjects->GetEntityByObjId(m_vehicleId));
    }

    void VehicleRecollection::SaveRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const
    {
        // RVA 0x7D5A10
        Obj::SaveRuntimeValues(xmlFile, xmlNode);
        xmlNode->SetAttribute("VehicleId", CStr(m_vehicleId).c_str());
        if (m_recollectionItems.empty())
        {
            return;
        }

        ref_ptr itemsNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_ELEMENT, "Items");
        xmlNode->AddChild(itemsNode);
        for (auto const& item : m_recollectionItems)
        {
            ref_ptr itemNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_ELEMENT, "Item");
            itemsNode->AddChild(itemNode);
            item.SaveToXML(xmlFile, itemNode);
        }
    }

    void VehicleRecollection::SetVehicle(Vehicle const* vehicle)
    {
        m_vehicleId = vehicle->GetId();
    }

    VehicleRecollection::~VehicleRecollection() = default;

    void VehicleRecollection::_InternalCreateVisualPart()
    {
        // RVA 0x7D6880
        Clear();
    }

    m3d::Object* VehicleRecollection::CreateObject()
    {
        // RVA 0x7D5630
        SYS_ERROR("!\"Object cannot be created directly\"");
        return nullptr;
    }

    m3d::Object* VehicleRecollection::Clone()
    {
        // RVA 0x7D5470
        SYS_ERROR("!\"Object cannot be cloned\"");
        return nullptr;
    }
}  // namespace ai
