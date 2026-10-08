#include "wanderersgenerator.h"

#include <algorithm>
#include <cstdlib>

#include "core/log.h"
#include <core/ini.h>
#include <core/kernel.h>
#include "base/objcontainer.h"
#include "base/prototypemanager.h"
#include "physicbodies/physichelpers.h"
#include "physicbodies/vehiclepart.h"

namespace ai
{
    namespace
    {
        CStr const CABIN = "CABIN";
        CStr const BASKET = "BASKET";
        CStr const CABIN_SMALL_GUN = "CABIN_SMALL_GUN";
        CStr const CABIN_BIG_GUN = "CABIN_BIG_GUN";
        CStr const CABIN_SPECIAL_WEAPON = "CABIN_SPECIAL_WEAPON";
        CStr const BASKET_SMALL_GUN_0 = "BASKET_SMALL_GUN_0";
        CStr const BASKET_SMALL_GUN_1 = "BASKET_SMALL_GUN_1";
        CStr const BASKET_BIG_GUN_0 = "BASKET_BIG_GUN_0";
        CStr const BASKET_BIG_GUN_1 = "BASKET_BIG_GUN_1";
        CStr const BASKET_SIDE_GUN = "BASKET_SIDE_GUN";

        // Fills each index with a random value in [m_lowerBound, m_upperBound].
        struct RandomIndexPred
        {
            RandomIndexPred(int lowerBound, int upperBound) : m_lowerBound(lowerBound), m_upperBound(upperBound)
            {
            }

            void operator()(int& index)
            {
                index = m_lowerBound + rand() % (m_upperBound - m_lowerBound + 1);
            }

            int m_lowerBound;
            int m_upperBound;
        };
    }  // namespace

    WanderersGeneratorPrototypeInfo::VehiclePartDescription::VehiclePartDescription()
    {
        m_present = true;
    }

    void WanderersGeneratorPrototypeInfo::VehiclePartDescription::LoadFromXML(
        m3d::cmn::XmlFile* xmlfile,
        m3d::cmn::XmlNode const* xmlNode)
    {
        m3d::SafeBoolAttrib(m_present, xmlNode, "Present");

        CStr strPrototypes;
        m3d::SafeStrAttrib(strPrototypes, xmlNode, "Prototypes");
        m3d::Tokenize(strPrototypes, m_prototypeNames, "(), ;\t");
    }

    void WanderersGeneratorPrototypeInfo::VehiclePartDescription::PostLoad()
    {
        // RVA 0x8717C0
        for (auto const& prototypeName : m_prototypeNames)
        {
            m_prototypeIds.push_back(thePrototypeManager->GetPrototypeId(prototypeName));
            if (m_prototypeIds.back() == -1)
            {
                M3D_LOG_ERR(
                    CStr("Error: Unknown vehicle part prototype '") + prototypeName + CStr("' in wanderers generator"));
                SYS_ERROR("!\"Critical error, see log\"");
            }
        }
    }

    bool WanderersGeneratorPrototypeInfo::VehiclePartDescription::HasPrototypes() const
    {
        // RVA 0x870AD0
        return !m_prototypeNames.empty();
    }

    bool WanderersGeneratorPrototypeInfo::VehiclePartDescription::DoesPresent() const
    {
        // RVA 0x8705B0
        return m_present;
    }

    CStr const& WanderersGeneratorPrototypeInfo::VehiclePartDescription::GetRandomPrototypeName() const
    {
        // RVA 0x870C70
        if (m_prototypeNames.empty())
        {
            SYS_ERROR("!m_prototypeNames.empty()");
        }
        unsigned int const count = static_cast<unsigned int>(m_prototypeNames.size());
        return m_prototypeNames[(count * rand()) >> 15];
    }

    int WanderersGeneratorPrototypeInfo::VehiclePartDescription::GetRandomPrototypeId() const
    {
        // RVA 0x870E80
        if (m_prototypeIds.empty())
        {
            SYS_ERROR("!m_prototypeIds.empty()");
        }
        unsigned int const count = static_cast<unsigned int>(m_prototypeIds.size());
        return m_prototypeIds[(count * rand()) >> 15];
    }

    void WanderersGeneratorPrototypeInfo::VehicleDescription::LoadFromXML(
        m3d::cmn::XmlFile* xmlFile,
        m3d::cmn::XmlNode const* xmlNode)
    {
        m3d::SafeStrAttrib(m_prototype, xmlNode, "Prototype");
        LoadPartFromXML(m_cabin, "Cabin", xmlFile, xmlNode);
        LoadPartFromXML(m_basket, "Basket", xmlFile, xmlNode);
        LoadPartFromXML(m_cabinSmallGun, "CabinSmallGun", xmlFile, xmlNode);
        LoadPartFromXML(m_cabinBigGun, "CabinBigGun", xmlFile, xmlNode);
        LoadPartFromXML(m_cabinSpecialWeapon, "CabinSpecialWeapon", xmlFile, xmlNode);
        LoadPartFromXML(m_basketSmallGun0, "BasketSmallGun0", xmlFile, xmlNode);
        LoadPartFromXML(m_basketSmallGun1, "BasketSmallGun1", xmlFile, xmlNode);
        LoadPartFromXML(m_basketBigGun0, "BasketBigGun0", xmlFile, xmlNode);
        LoadPartFromXML(m_basketBigGun1, "BasketBigGun1", xmlFile, xmlNode);
        LoadPartFromXML(m_basketSideGun, "BasketSideGun", xmlFile, xmlNode);
    }

    void WanderersGeneratorPrototypeInfo::VehicleDescription::LoadPartFromXML(
        ai::WanderersGeneratorPrototypeInfo::VehiclePartDescription& part,
        char const* partName,
        m3d::cmn::XmlFile* xmlFile,
        m3d::cmn::XmlNode const* xmlNode)
    {
        ref_ptr partNode = xmlFile->CreateNode();
        xmlNode->GetFirstChild(partNode, partName);
        if (!partNode->IsEmpty())
        {
            part.LoadFromXML(xmlFile, partNode);
        }
    }

    void WanderersGeneratorPrototypeInfo::VehicleDescription::PostLoad()
    {
        // RVA 0x871BF0
        m_prototypeId = thePrototypeManager->GetPrototypeId(m_prototype);
        if (m_prototypeId == -1)
        {
            M3D_LOG_ERR(CStr("Error: Unknown vehicle prototype '") + m_prototype + CStr("' in wanderers generator"));
            SYS_ERROR("!\"Critical error, see log\"");
        }

        m_cabin.PostLoad();
        m_basket.PostLoad();
        m_cabinSmallGun.PostLoad();
        m_cabinBigGun.PostLoad();
        m_cabinSpecialWeapon.PostLoad();
        m_basketSmallGun0.PostLoad();
        m_basketSmallGun1.PostLoad();
        m_basketBigGun0.PostLoad();
        m_basketBigGun1.PostLoad();
        m_basketSideGun.PostLoad();
    }

    ai::WanderersGeneratorPrototypeInfo::VehiclePartDescription const&
        WanderersGeneratorPrototypeInfo::VehicleDescription::GetCabinDescription() const
    {
        // RVA 0x870480
        return m_cabin;
    }

    ai::WanderersGeneratorPrototypeInfo::VehiclePartDescription const&
        WanderersGeneratorPrototypeInfo::VehicleDescription::GetBasketDescription() const
    {
        // RVA 0x870490
        return m_basket;
    }

    ai::WanderersGeneratorPrototypeInfo::VehiclePartDescription const&
        WanderersGeneratorPrototypeInfo::VehicleDescription::GetCabinSmallGunDescription() const
    {
        // RVA 0x8704A0
        return m_cabinSmallGun;
    }

    ai::WanderersGeneratorPrototypeInfo::VehiclePartDescription const&
        WanderersGeneratorPrototypeInfo::VehicleDescription::GetCabinBigGunDescription() const
    {
        // RVA 0x8704B0
        return m_cabinBigGun;
    }

    ai::WanderersGeneratorPrototypeInfo::VehiclePartDescription const&
        WanderersGeneratorPrototypeInfo::VehicleDescription::GetCabinSpecialWeaponDescription() const
    {
        // RVA 0x8704C0
        return m_cabinSpecialWeapon;
    }

    ai::WanderersGeneratorPrototypeInfo::VehiclePartDescription const&
        WanderersGeneratorPrototypeInfo::VehicleDescription::GetBasketSmallGun0Description() const
    {
        // RVA 0x8704D0
        return m_basketSmallGun0;
    }

    ai::WanderersGeneratorPrototypeInfo::VehiclePartDescription const&
        WanderersGeneratorPrototypeInfo::VehicleDescription::GetBasketSmallGun1Description() const
    {
        // RVA 0x8704E0
        return m_basketSmallGun1;
    }

    ai::WanderersGeneratorPrototypeInfo::VehiclePartDescription const&
        WanderersGeneratorPrototypeInfo::VehicleDescription::GetBasketBigGun0Description() const
    {
        // RVA 0x8704F0
        return m_basketBigGun0;
    }

    ai::WanderersGeneratorPrototypeInfo::VehiclePartDescription const&
        WanderersGeneratorPrototypeInfo::VehicleDescription::GetBasketBigGun1Description() const
    {
        // RVA 0x870500
        return m_basketBigGun1;
    }

    ai::WanderersGeneratorPrototypeInfo::VehiclePartDescription const&
        WanderersGeneratorPrototypeInfo::VehicleDescription::GetBasketSideGunDescription() const
    {
        // RVA 0x870510
        return m_basketSideGun;
    }

    CStr const& WanderersGeneratorPrototypeInfo::VehicleDescription::GetPrototypeName() const
    {
        // NOTE: never instantiated in the shipped binary, so this body does not come from it.
        return m_prototype;
    }

    int WanderersGeneratorPrototypeInfo::VehicleDescription::GetPrototypeId() const
    {
        // RVA 0x870520
        return m_prototypeId;
    }

    WanderersGeneratorPrototypeInfo::WanderersGeneratorPrototypeInfo()
    {
        m_desiredCountLow = -1;
        m_desiredCountHigh = -1;
    }

    bool WanderersGeneratorPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        bool const result = PrototypeInfo::LoadFromXML(xmlFile, xmlNode);
        if (result)
        {
            m_desiredCountLow = -1;
            m_desiredCountHigh = -1;

            CStr desiredCount;
            m3d::SafeStrAttrib(desiredCount, xmlNode, "DesiredCount");

            retruxx::vector<CStr> tknsDesiredCount;
            m3d::Tokenize(desiredCount, tknsDesiredCount, "-");

            if (!tknsDesiredCount.empty())
            {
                m_desiredCountLow = std::atoi(tknsDesiredCount.front().c_str());
            }
            m_desiredCountHigh = m_desiredCountLow;

            if (tknsDesiredCount.size() > 1)
            {
                m_desiredCountHigh = std::atoi(tknsDesiredCount.back().c_str());
            }

            if (m_desiredCountHigh < m_desiredCountLow)
            {
                m_desiredCountHigh = m_desiredCountLow;
            }

            ref_ptr vehiclesNode = xmlFile->CreateNode();
            xmlNode->GetFirstChild(vehiclesNode, "Vehicles");
            if (!vehiclesNode->IsEmpty())
            {
                ref_ptr vehicleNode = xmlFile->CreateNode();
                for (vehiclesNode->GetFirstChild(vehicleNode, "Vehicle"); !vehicleNode->IsEmpty();
                     vehicleNode->GetNextSibling(vehicleNode, "Vehicle"))
                {
                    VehicleDescription desc;
                    desc.LoadFromXML(xmlFile, vehicleNode);
                    m_vehicleDescriptions.push_back(std::move(desc));
                }
            }
        }
        return result;
    }

    retruxx::vector<int, retruxx::allocator<int>> WanderersGeneratorPrototypeInfo::Generate(int count) const
    {
        // RVA 0x872060 - creates one vehicle per generated index and refits its parts from the
        // matching description. A count of -1 means a random count within DesiredCount.
        retruxx::vector<int> indexes;
        _GenerateIndexes(indexes, count);

        retruxx::vector<int> objIds;
        objIds.reserve(indexes.size());
        for (int index : indexes)
        {
            VehicleDescription const& vehicleDescription = m_vehicleDescriptions[index];
            objIds.push_back(theObjects->CreateNewObject(vehicleDescription.GetPrototypeId(), "Wanderer", -1, -1));
            // NOTE: the new object is cast to a vehicle without a type or null check.
            _TuneVehicleParts(static_cast<Vehicle*>(theObjects->GetEntityByObjId(objIds.back())), vehicleDescription);
        }
        return objIds;
    }

    retruxx::vector<int, retruxx::allocator<int>> WanderersGeneratorPrototypeInfo::GenerateAndPlace(
        CVector const& pos,
        int count) const
    {
        // RVA 0x8721D0 - every vehicle is placed at pos and then moved to the nearest free spot,
        // if one is found.
        retruxx::vector<int> objIds = Generate(count);
        for (int objId : objIds)
        {
            // NOTE: the object is used without a type or null check.
            auto* vehicle = static_cast<Vehicle*>(theObjects->GetEntityByObjId(objId));
            vehicle->SetPosition(pos);

            CVector newPos;
            if (GetValidPosition(
                    pos, vehicle->GetIntersectionRadius(), 0xFF, newPos, false, false, std::set<m3d::Class*>()))
            {
                vehicle->SetPosition(newPos);
            }
        }
        return objIds;
    }

    ai::Obj* WanderersGeneratorPrototypeInfo::CreateTargetObject() const
    {
        // RVA 0x870910 - a generator is only a prototype and never becomes an object.
        SYS_ERROR("0");
        return nullptr;
    }

    void WanderersGeneratorPrototypeInfo::PostLoad()
    {
        // RVA 0x871EE0 - NOTE: PrototypeInfo::PostLoad is not called.
        for (auto& vehicleDescription : m_vehicleDescriptions)
        {
            vehicleDescription.PostLoad();
        }
    }

    void WanderersGeneratorPrototypeInfo::_GenerateIndexes(
        retruxx::vector<int, retruxx::allocator<int>>& indexes,
        int count) const
    {
        // RVA 0x871670 - without a DesiredCount every description is used once, in order;
        // otherwise count (or a random number within DesiredCount) random descriptions are picked.
        if (m_desiredCountLow == -1)
        {
            indexes.resize(m_vehicleDescriptions.size(), 0);
            // NOTE: the indices start at 1, so the first description is never used and the last
            // index is one past the end of m_vehicleDescriptions.
            int i = 0;
            for (int& index : indexes)
            {
                index = ++i;
            }
            return;
        }

        if (count == -1)
        {
            count = m_desiredCountLow + rand() % (m_desiredCountHigh - m_desiredCountLow + 1);
        }
        indexes.resize(count, 0);
        std::for_each(
            indexes.begin(), indexes.end(), RandomIndexPred(0, static_cast<int>(m_vehicleDescriptions.size()) - 1));
    }

    void WanderersGeneratorPrototypeInfo::_TuneVehicleParts(
        ai::Vehicle* vehicle,
        ai::WanderersGeneratorPrototypeInfo::VehicleDescription const& vehicleDescription) const
    {
        // RVA 0x871F10
        _TunePart(vehicle, CABIN, vehicleDescription.GetCabinDescription());
        _TunePart(vehicle, BASKET, vehicleDescription.GetBasketDescription());
        _TunePart(vehicle, CABIN_SMALL_GUN, vehicleDescription.GetCabinSmallGunDescription());
        _TunePart(vehicle, CABIN_BIG_GUN, vehicleDescription.GetCabinBigGunDescription());
        _TunePart(vehicle, CABIN_SPECIAL_WEAPON, vehicleDescription.GetCabinSpecialWeaponDescription());
        _TunePart(vehicle, BASKET_SMALL_GUN_0, vehicleDescription.GetBasketSmallGun0Description());
        _TunePart(vehicle, BASKET_SMALL_GUN_1, vehicleDescription.GetBasketSmallGun1Description());
        _TunePart(vehicle, BASKET_BIG_GUN_0, vehicleDescription.GetBasketBigGun0Description());
        _TunePart(vehicle, BASKET_BIG_GUN_1, vehicleDescription.GetBasketBigGun1Description());
        _TunePart(vehicle, BASKET_SIDE_GUN, vehicleDescription.GetBasketSideGunDescription());
    }

    void WanderersGeneratorPrototypeInfo::_TunePart(
        ai::Vehicle* vehicle,
        CStr const& partName,
        ai::WanderersGeneratorPrototypeInfo::VehiclePartDescription const& partDescription) const
    {
        // RVA 0x871740 - a part marked absent is removed; a present part without a prototype
        // list keeps the one the vehicle came with.
        if (partDescription.DoesPresent())
        {
            if (partDescription.HasPrototypes())
            {
                _ChangePartByName(vehicle, partName, partDescription.GetRandomPrototypeId());
            }
        }
        else
        {
            _RemovePartByName(vehicle, partName);
        }
    }

    void WanderersGeneratorPrototypeInfo::_ChangePartByName(
        ai::Vehicle* vehicle,
        CStr const& partName,
        int partPrototypeId) const
    {
        // RVA 0x8710A0
        if (partPrototypeId == -1)
        {
            SYS_ERROR("partPrototypeId != INVALID_ID");
        }

        VehiclePart* oldPart = vehicle->GetPartByName(partName);
        int const newPartId = theObjects->CreateNewObject(partPrototypeId, "", -1, -1);
        Obj* newPart = theObjects->GetEntityByObjId(newPartId);
        if (newPart)
        {
            // NOTE: the new object is cast to a part without a type check.
            vehicle->SetPartByName(partName, static_cast<VehiclePart*>(newPart), false);
            if (oldPart)
            {
                oldPart->Remove();
            }
            return;
        }

        M3D_LOG_ERR(CStr("Error: created a NULL part by prototype id ") + CStr(partPrototypeId));
        PrototypeInfo const* prototype = thePrototypeManager->GetPrototypeInfo(partPrototypeId);
        if (prototype)
        {
            M3D_LOG_INFO(CStr("Prototype name = '") + prototype->m_prototypeName + CStr("'"));
        }
        else
        {
            M3D_LOG_INFO(CStr("Prototype is NULL"));
        }
        SYS_ERROR("0");
    }

    void WanderersGeneratorPrototypeInfo::_RemovePartByName(ai::Vehicle* vehicle, CStr const& partName) const
    {
        // RVA 0x870570
        VehiclePart* part = vehicle->GetPartByName(partName);
        if (part)
        {
            vehicle->SetPartByName(partName, nullptr, false);
            part->Remove();
        }
    }
}  // namespace ai
