#include "complexphysicobj.h"
#include <stdexcept>

#include "core/ini.h"
#include "core/kernel.h"
#include "core/log.h"
#include "thirdparty/injecttools.h"
#include <server/resourcemanager.h>
#include "prototypemanager.h"

RT_CLASS_EXPORT_METHOD_DEFINE(ComplexPhysicObj, CanPartBeAttached)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(ComplexPhysicObj, SetPartByName)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(ComplexPhysicObj, SetNewPart)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(ComplexPhysicObj, TakeOffPart)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(ComplexPhysicObj, GetPartByName)
{
    throw std::logic_error("Not implemented");
}

const CStr NO_LP("NO_LP");

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(ComplexPhysicObj)
        RT_CLASS_EXPORT(ComplexPhysicObj, m3d::METHOD, CanPartBeAttached, "", "", "")
        RT_CLASS_EXPORT(ComplexPhysicObj, m3d::METHOD, SetPartByName, "", "", "")
        RT_CLASS_EXPORT(ComplexPhysicObj, m3d::METHOD, SetNewPart, "", "", "")
        RT_CLASS_EXPORT(ComplexPhysicObj, m3d::METHOD, TakeOffPart, "", "", "")
        RT_CLASS_EXPORT(ComplexPhysicObj, m3d::METHOD, GetPartByName, "", "", "")
	RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(ComplexPhysicObj);

    RT_CLASS_EXPORTS_BEGIN(ComplexPhysicObjPartDescription)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(ComplexPhysicObjPartDescription);

	m3d::Class* ComplexPhysicObjPartDescription::GetClass() const
	{
		throw std::logic_error("Not implemented");
	}

	m3d::Object* ComplexPhysicObjPartDescription::CreateObject()
	{
        return new ComplexPhysicObjPartDescription;
	}

	int ComplexPhysicObjPartDescription::GetPartResourceId() const
	{
        return m_partResourceId;
	}

	unsigned ComplexPhysicObjPartDescription::GetNumLps() const
	{
		throw std::logic_error("Not implemented");
	}

	void ComplexPhysicObjPartDescription::GetPartNames(retruxx::vector<CStr, retruxx::allocator<CStr>>& partNames) const
	{
        partNames.push_back(m_name);
        for (auto child = dynamic_cast<ComplexPhysicObjPartDescription*>(GetFirstChild()); child; child = dynamic_cast<ComplexPhysicObjPartDescription*>(child->GetNextSibling()))
        {
            child->GetPartNames(partNames);
        }
	}

	void ComplexPhysicObjPartDescription::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
	{
        m3d::SafeStrAttrib(m_name, xmlNode, "id");
        if (GetParent())
        {
            auto child = GetParent()->GetChildByName(m_name);
            if (child && this != child)
            {
                M3D_LOG_INFO("Warning: when loading PartDescription: name = " + m_name + " conflicts with another child");
            }
        }

        CStr strPartResourceId;
        m3d::SafeStrAttrib(strPartResourceId, xmlNode, "partResourceType");
        m_partResourceId = theResourceManager->GetResourceId(strPartResourceId);

        CStr lpName;
        m3d::SafeStrAttrib(lpName, xmlNode, "lpName");

        retruxx::vector<CStr> names;
        m3d::Tokenize(lpName, names, "(), ;\t");
        if (names.empty())
        {
            m_lpNames.push_back(NO_LP);
        }

        ref_ptr descNode = xmlFile->CreateNode();
        for (xmlNode->GetFirstChild(descNode, "PartDescription"); !descNode->IsEmpty(); descNode->GetNextSibling(descNode, "PartDescription"))
        {
            auto desc = (ComplexPhysicObjPartDescription*)M3D_KERNEL->New("ComplexPhysicObjPartDescription");
            AddChild(desc);
            desc->LoadFromXML(xmlFile, descNode);
        }
	}

	ComplexPhysicObjPartDescription const* ComplexPhysicObjPartDescription::GetChildByNameDeep(CStr const& childName) const
	{
        if (m_name == childName)
        {
            return this;
        }

	    auto child = dynamic_cast<ai::ComplexPhysicObjPartDescription*>(GetFirstChild());
        if (!child)
            return nullptr;

        while (true)
        {
            auto result = child->GetChildByNameDeep(childName);
            if (result)
                return result;

            child = dynamic_cast<ai::ComplexPhysicObjPartDescription*>(child->GetNextSibling());
            if (!child)
                return nullptr;
        }
        return nullptr;
	}

	CStr const& ComplexPhysicObjPartDescription::GetLpName(unsigned) const
	{
		throw std::logic_error("Not implemented");
	}

	m3d::Object* ComplexPhysicObjPartDescription::Clone()
	{
		throw std::logic_error("Not implemented");
	}

	ComplexPhysicObjPartDescription* ComplexPhysicObjPartDescription::GetParent() const
	{
        return dynamic_cast<ComplexPhysicObjPartDescription*>(Object::GetParent());
	}

	ComplexPhysicObjPartDescription::~ComplexPhysicObjPartDescription()
	{
        RemoveAllChildren();
	}

	m3d::Class* ComplexPhysicObjPartDescription::GetBaseClass()
	{
        return RT_CLASS_LOCAL(Object);
	}

	ComplexPhysicObjPartDescription::ComplexPhysicObjPartDescription()
	{
        this->m_partResourceId = -1;
	}

	ComplexPhysicObjPartDescription::ComplexPhysicObjPartDescription(ComplexPhysicObjPartDescription const&)
	{
		throw std::logic_error("Not implemented");
	}

	ComplexPhysicObjPrototypeInfo::MassShapes ComplexPhysicObjPrototypeInfo::GetMassShape() const
	{
		throw std::logic_error("Not implemented");
	}

	ComplexPhysicObjPartDescription const* ComplexPhysicObjPrototypeInfo::GetPartDescriptionByName(CStr const& partName) const
	{
        return m_partDescription->GetChildByNameDeep(partName);
	}

	Obj* ComplexPhysicObjPrototypeInfo::CreateRandomTargetObject() const
	{
		throw std::logic_error("Not implemented");
	}

	bool ComplexPhysicObjPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
	{
        auto result = ai::PhysicObjPrototypeInfo::LoadFromXML(xmlFile, xmlNode);
        if (result)
        {
            ref_ptr partsRootNode = xmlFile->CreateNode();
            xmlNode->GetFirstChild(partsRootNode, "MainPartDescription");
            if (partsRootNode->IsEmpty())
            {
                if (m_parentPrototypeName.empty())
                {
                    M3D_LOG_INFO("Warning: parts description missing for prototype '" + m_prototypeName + "'");
                }
            }
            else
            {
                m_partDescription->LoadFromXML(xmlFile, partsRootNode);
            }

            xmlNode->GetFirstChild(partsRootNode, "Parts");
            if (partsRootNode->IsEmpty())
            {
                M3D_LOG_INFO("Warning: Parts description is missing for prototype '" + m_prototypeName + "'");
            }
            else
            {
                ref_ptr partNode = xmlFile->CreateNode();
                for (partsRootNode->GetFirstChild(partNode, "Part"); !partNode->IsEmpty(); partNode->GetNextSibling(partNode, "Part"))
                {
                    CStr id;
                    m3d::SafeStrAttrib(id, partNode, "id");

                    CStr proto;
                    m3d::SafeStrAttrib(proto, partNode, "Prototype");

                    // TODO: check this
                    m_partPrototypeNames[id] = proto;
                }
            }

            m3d::SafeVectorAttrib(m_massSize, xmlNode, "MassSize");
            m3d::SafeVectorAttrib(m_massTranslation, xmlNode, "MassTranslation");
            m3d::SafeEnumAttrib(m_massShape, xmlNode, "MassShape");
        }
        return result;
	}

	void ComplexPhysicObjPrototypeInfo::GetPartNames(retruxx::vector<CStr, retruxx::allocator<CStr>>&) const
	{
		throw std::logic_error("Not implemented");
	}

	ComplexPhysicObjPrototypeInfo::~ComplexPhysicObjPrototypeInfo()
	{
        throw std::logic_error("Not implemented");
	}

	retruxx::vector<CStr, retruxx::allocator<CStr>> const& ComplexPhysicObjPrototypeInfo::GetAllPartNames() const
	{
		throw std::logic_error("Not implemented");
	}

	ComplexPhysicObjPrototypeInfo::ComplexPhysicObjPrototypeInfo()
	{
        this->m_massSize.x = 1.0;
        this->m_massSize.y = 1.0;
        this->m_massSize.z = 1.0;
        this->m_massTranslation = {0.0, 0.0, 0.0};
        m_partDescription = (ComplexPhysicObjPartDescription*)M3D_KERNEL->New("ComplexPhysicObjPartDescription");
        this->m_massShape = MS_BOX;
	}

	unsigned ComplexPhysicObjPrototypeInfo::GetBasePrice() const
	{
		throw std::logic_error("Not implemented");
	}

	void ComplexPhysicObjPrototypeInfo::PostLoad()
	{
        for (auto& name : m_partPrototypeNames)
        {
            m_partPrototypeIds[name.first] = thePrototypeManager->GetPrototypeId(name.second);
        }

        m_allPartNames.clear();
        m_partDescription->GetPartNames(m_allPartNames);
	}

    void ComplexPhysicObj::UnlinkGeomsFromCollisionCells()
    {
        throw std::logic_error("Not implemented");
    }

    void ComplexPhysicObj::GetGeoms(retruxx::vector<Geom*, retruxx::allocator<Geom*>>&) const
    {
        throw std::logic_error("Not implemented");
    }

    void ComplexPhysicObj::SetPassedToAnotherMapStatus()
    {
        throw std::logic_error("Not implemented");
    }

    void ComplexPhysicObj::EnableGeometry(bool)
    {
        throw std::logic_error("Not implemented");
    }

    void ComplexPhysicObj::RenderDebugInfo() const
    {
        throw std::logic_error("Not implemented");
    }

    retruxx::vector<CStr, retruxx::allocator<CStr>> ComplexPhysicObj::GetAttachedPartNames() const
    {
        throw std::logic_error("Not implemented");
    }

    void ComplexPhysicObj::DumpPhysicInfo(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        throw std::logic_error("Not implemented");
    }

    void ComplexPhysicObj::SetPartByName(CStr const&, VehiclePart*, bool)
    {
        throw std::logic_error("Not implemented");
    }

    void ComplexPhysicObj::Remove()
    {
        throw std::logic_error("Not implemented");
    }

    ComplexPhysicObj::ComplexPhysicObj(ComplexPhysicObjPrototypeInfo const& prototypeInfo) : PhysicObj(prototypeInfo)
    {
        //throw std::logic_error("Not implemented");
    }

    m3d::Class* ComplexPhysicObj::GetBaseClass()
    {
        return RT_CLASS_LOCAL(PhysicObj);
    }

    void ComplexPhysicObj::PutContour()
    {
        throw std::logic_error("Not implemented");
    }

    bool ComplexPhysicObj::IsVisible()
    {
        throw std::logic_error("Not implemented");
    }

    void ComplexPhysicObj::SetSkin(int)
    {
        throw std::logic_error("Not implemented");
    }

    void ComplexPhysicObj::CreateChildren()
    {
        throw std::logic_error("Not implemented");
    }

    void ComplexPhysicObj::TransferPhysicParamsToSceneGraphNode()
    {
        throw std::logic_error("Not implemented");
    }

    void ComplexPhysicObj::ClearSavedStatus()
    {
        throw std::logic_error("Not implemented");
    }

    VehiclePart const* ComplexPhysicObj::GetPartByName(CStr const&) const
    {
        throw std::logic_error("Not implemented");
    }

    VehiclePart* ComplexPhysicObj::GetPartByName(CStr const&)
    {
        throw std::logic_error("Not implemented");
    }

    void ComplexPhysicObj::SetVisible()
    {
        throw std::logic_error("Not implemented");
    }

    void ComplexPhysicObj::LoadRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw std::logic_error("Not implemented");
    }

    void ComplexPhysicObj::EnablePhysics()
    {
        throw std::logic_error("Not implemented");
    }

    VehiclePart* ComplexPhysicObj::TakeOffPart(CStr const&)
    {
        throw std::logic_error("Not implemented");
    }

    void ComplexPhysicObj::AddChild(Obj*)
    {
        throw std::logic_error("Not implemented");
    }

    unsigned ComplexPhysicObj::GetPrice(IPriceCoeffProvider const*) const
    {
        throw std::logic_error("Not implemented");
    }

    void ComplexPhysicObj::ReceiveNodesToLink(retruxx::list<m3d::SgNode*, retruxx::allocator<m3d::SgNode*>>&) const
    {
        throw std::logic_error("Not implemented");
    }

    void ComplexPhysicObj::RemoveContour()
    {
        throw std::logic_error("Not implemented");
    }

    void ComplexPhysicObj::RelinkGeomsToCollisionCells()
    {
        throw std::logic_error("Not implemented");
    }

    ComplexPhysicObjPrototypeInfo const* ComplexPhysicObj::GetPrototypeInfo() const
    {
        throw std::logic_error("Not implemented");
    }

    void ComplexPhysicObj::DisableGeometry(bool)
    {
        throw std::logic_error("Not implemented");
    }

    void ComplexPhysicObj::SetContourWidth(float)
    {
        throw std::logic_error("Not implemented");
    }

    void ComplexPhysicObj::SaveRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        throw std::logic_error("Not implemented");
    }

    unsigned ComplexPhysicObj::size() const
    {
        throw std::logic_error("Not implemented");
    }

    void ComplexPhysicObj::TransferToSpace(dxSpace*)
    {
        throw std::logic_error("Not implemented");
    }

    bool ComplexPhysicObj::bIsContoured() const
    {
        throw std::logic_error("Not implemented");
    }

    void ComplexPhysicObj::DisablePhysics()
    {
        throw std::logic_error("Not implemented");
    }

    void ComplexPhysicObj::LinkGeomsToCollisionCells()
    {
        throw std::logic_error("Not implemented");
    }

    void ComplexPhysicObj::SaveToXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        throw std::logic_error("Not implemented");
    }

    Obj* ComplexPhysicObj::CloneObj()
    {
        throw std::logic_error("Not implemented");
    }

    void ComplexPhysicObj::SetRandomSkin()
    {
        throw std::logic_error("Not implemented");
    }

    void ComplexPhysicObj::SetBelong(int)
    {
        throw std::logic_error("Not implemented");
    }

    void ComplexPhysicObj::SetInvisible()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* ComplexPhysicObj::GetClass() const
    {
        throw std::logic_error("Not implemented");
    }

    Geom::CellAabb ComplexPhysicObj::GetCollisionCellAabb() const
    {
        throw std::logic_error("Not implemented");
    }

    void ComplexPhysicObj::SetContourColor(unsigned)
    {
        throw std::logic_error("Not implemented");
    }

    int ComplexPhysicObj::GetNumPhysicBodies() const
    {
        throw std::logic_error("Not implemented");
    }

    bool ComplexPhysicObj::RemoveChild(Obj*)
    {
        throw std::logic_error("Not implemented");
    }

    bool ComplexPhysicObj::CanPartBeAttached(CStr const&) const
    {
        throw std::logic_error("Not implemented");
    }

    int ComplexPhysicObj::GetGunHorizontalStopAngles(CStr const&, int, float&, float&) const
    {
        throw std::logic_error("Not implemented");
    }

    void ComplexPhysicObj::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw std::logic_error("Not implemented");
    }

    void ComplexPhysicObj::Blow(Obj*)
    {
        throw std::logic_error("Not implemented");
    }

    void ComplexPhysicObj::Flow(Obj*, float)
    {
        throw std::logic_error("Not implemented");
    }

    unsigned ComplexPhysicObj::GetRepairPrice() const
    {
        throw std::logic_error("Not implemented");
    }

    void ComplexPhysicObj::RefreshMass()
    {
        throw std::logic_error("Not implemented");
    }

    RETRUXX_DLL_OVERWRITE_BY_ORIGINAL_FUNCTION(0x006BCC10, ComplexPhysicObj::GetSmoothTargetPointForObj)
    CVector ComplexPhysicObj::GetSmoothTargetPointForObj(Obj const*, float)
    {
        throw std::logic_error("Not implemented");
    }

    void ComplexPhysicObj::FlowUnattachableParts(float)
    {
        throw std::logic_error("Not implemented");
    }

    bool ComplexPhysicObj::SetNewPart(CStr const&, CStr const&)
    {
        throw std::logic_error("Not implemented");
    }

    void ComplexPhysicObj::RemoveComponent(Obj*)
    {
        throw std::logic_error("Not implemented");
    }

    void ComplexPhysicObj::_DestroyHierarchy()
    {
        throw std::logic_error("Not implemented");
    }

    void ComplexPhysicObj::_LinkBodyToGeoms()
    {
        throw std::logic_error("Not implemented");
    }

    void ComplexPhysicObj::_UnlinkBodyFromGeoms()
    {
        throw std::logic_error("Not implemented");
    }

    void ComplexPhysicObj::_SetCorrectBoundSphereRadius()
    {
        throw std::logic_error("Not implemented");
    }

    void ComplexPhysicObj::_RemoveContour()
    {
        throw std::logic_error("Not implemented");
    }

    void ComplexPhysicObj::_InternalCreateVisualPart()
    {
        throw std::logic_error("Not implemented");
    }

    void ComplexPhysicObj::_SetPositionToGeoms(CVector const&)
    {
        throw std::logic_error("Not implemented");
    }

    void ComplexPhysicObj::_ConstructVehiclePart(CStr const&, VehiclePart*, int, bool)
    {
        throw std::logic_error("Not implemented");
    }

    float ComplexPhysicObj::_CalcMassForBody() const
    {
        throw std::logic_error("Not implemented");
    }

    ComplexPhysicObj::~ComplexPhysicObj()
    {
    }

    void ComplexPhysicObj::_PutContour()
    {
        throw std::logic_error("Not implemented");
    }

    void ComplexPhysicObj::_SetRotationToGeoms(Quaternion const&)
    {
        throw std::logic_error("Not implemented");
    }

    void ComplexPhysicObj::_Construct(bool)
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* ComplexPhysicObj::CreateObject()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* ComplexPhysicObj::Clone()
    {
        throw std::logic_error("Not implemented");
    }

    void ComplexPhysicObj::_CreateSplinterFromSgNode(VehiclePart*, int, CVector const&, float, m3d::SgNode*, CollisionInfo const*)
    {
        throw std::logic_error("Not implemented");
    }

    void ComplexPhysicObj::_TearOffPart(VehiclePart*, float)
    {
        throw std::logic_error("Not implemented");
    }
}
