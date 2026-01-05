#include "vehiclesgenerator.h"
#include "team.h"
#include "ware.h"
#include "core/log.h"
#include "server/resourcemanager.h"
#include "base/prototypemanager.h"

#include <algorithm>

namespace ai
{
    retruxx::vector<VehiclesGeneratorInfoCache::WareInfo> const& VehiclesGeneratorInfoCache::GetWareInfos() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool VehiclesGeneratorInfoCache::VehiclePartInfo::operator<(ai::VehiclesGeneratorInfoCache::VehiclePartInfo const& rhs) const
    {
        return this->price < rhs.price;
    }

    bool VehiclesGeneratorInfoCache::WareInfo::operator<(ai::VehiclesGeneratorInfoCache::WareInfo const& rhs) const
    {
        return this->price < rhs.price;
    }

    VehiclesGeneratorInfoCache::VehiclesGeneratorInfoCache(VehiclesGeneratorInfoCache const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    retruxx::map<
        CStr,
        retruxx::vector<VehiclesGeneratorInfoCache::VehiclePartInfo, retruxx::allocator<VehiclesGeneratorInfoCache::VehiclePartInfo>>,
        retruxx::less<CStr>,
        retruxx::allocator<retruxx::pair<
            CStr const,
            retruxx::vector<VehiclesGeneratorInfoCache::VehiclePartInfo, retruxx::allocator<VehiclesGeneratorInfoCache::VehiclePartInfo>>>>> const&
        VehiclesGeneratorInfoCache::GetGunInfos() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    VehiclesGeneratorPrototypeInfo::VehicleDescription::VehicleDescription(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        LoadFromXML(xmlFile, xmlNode);
    }

    void VehiclesGeneratorPrototypeInfo::VehicleDescription::PostLoad()
    {
        vehiclePrototypeIds.resize(vehiclePrototypeNames.size());
        for (size_t i = 0; i < vehiclePrototypeIds.size(); ++i)
        {
            vehiclePrototypeIds[i] = thePrototypeManager->GetPrototypeId(vehiclePrototypeNames[i]);
            if (vehiclePrototypeIds[i] == -1)
            {
                M3D_CRITICAL_ERROR("Unknown vehicle prototype name '" + vehiclePrototypeNames[i] + "' for vehicle generator");
            }

            auto* prototypeInfo = thePrototypeManager->GetPrototypeInfo(vehiclePrototypeIds[i]);
            M3D_ASSERT(prototypeInfo->IsPrototypeOf(RT_CLASS_LOCAL(Vehicle)));
        }

        for (size_t i = 0; i < waresPrototypesNames.size(); ++i)
        {
            int const prId = thePrototypeManager->GetPrototypeId(waresPrototypesNames[i]);
            M3D_ASSERT(prId != -1);
            waresPrototypesIds.push_back(prId);

            auto* prototypeInfo = thePrototypeManager->GetPrototypeInfo(waresPrototypesIds[i]);
            M3D_ASSERT(prototypeInfo->IsPrototypeOf(RT_CLASS_LOCAL(Ware)));
        }

        if (!gunAffixGeneratorPrototypeName.empty())
        {
            int const prId = thePrototypeManager->GetPrototypeId(gunAffixGeneratorPrototypeName);
            M3D_ASSERT(prId != -1);
            gunAffixGeneratorPrototypeId = prId;
        }
        else
        {
            gunAffixGeneratorPrototypeId = -1;
        }
    }

    void VehiclesGeneratorPrototypeInfo::VehicleDescription::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        partOfSchwartz = -1.0f;
        m3d::SafeFloatAttrib(partOfSchwartz, xmlNode, "PartOfSchwartz");

        bTuningBySchwartz = partOfSchwartz > 0.0;

        CStr strVehiclesPrototypes;
        m3d::SafeStrAttrib(strVehiclesPrototypes, xmlNode, "VehiclesPrototypes");
        m3d::Tokenize(strVehiclesPrototypes, vehiclePrototypeNames, "(), ;\t");

        CStr wares;
        m3d::SafeStrAttrib(wares, xmlNode, "WaresPrototypes");
        m3d::Tokenize(wares, waresPrototypesNames, "(), ;\t");

        m3d::SafeStrAttrib(gunAffixGeneratorPrototypeName, xmlNode, "GunAffixGeneratorPrototype");
    }

    VehiclesGeneratorPrototypeInfo::VehiclesGeneratorPrototypeInfo() = default;

    Obj* VehiclesGeneratorPrototypeInfo::CreateTargetObject() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool VehiclesGeneratorPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
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
                M3D_LOG_WARN("Warning: high desired count is less than low desired count in VehiclesGenerator '" + m_prototypeName + "'");
                m_desiredCountHigh = m_desiredCountLow;
            }

            M3D_ASSERT(m_desiredCountHigh <= MAX_VEHICLES_IN_TEAM);
            ref_ptr descriptionNode = xmlFile->CreateNode();
            for (xmlNode->GetFirstChild(descriptionNode, "Description"); !descriptionNode->IsEmpty();
                 descriptionNode->GetNextSibling(descriptionNode, "Description"))
            {
                VehicleDescription desc(xmlFile, descriptionNode);
                m_vehicleDescriptions.push_back(std::move(desc));
            }

            m_partOfSchwartzForCabin = 0.25f;
            m_partOfSchwartzForBasket = 0.25f;
            m_partOfSchwartzForGuns = 0.5f;
            m_partOfSchwartzForWares = 0.0f;

            m3d::SafeFloatAttrib(m_partOfSchwartzForCabin, xmlNode, "partOfSchwartzForCabin");
            m3d::SafeFloatAttrib(m_partOfSchwartzForBasket, xmlNode, "partOfSchwartzForBasket");
            m3d::SafeFloatAttrib(m_partOfSchwartzForGuns, xmlNode, "partOfSchwartzForGuns");
            m3d::SafeFloatAttrib(m_partOfSchwartzForWares, xmlNode, "partOfSchwartzForWares");
        }
        return result;
    }

    void VehiclesGeneratorPrototypeInfo::PostLoad()
    {
        for (auto& descr : m_vehicleDescriptions)
        {
            descr.PostLoad();
        }
    }

    void VehiclesGeneratorPrototypeInfo::Generate(unsigned int restSchwarz, retruxx::vector<int, retruxx::allocator<int>>& generatedVehicleIds) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void VehiclesGeneratorPrototypeInfo::GenerateAndPlace(
        retruxx::vector<CVector, retruxx::allocator<CVector>> const& points,
        unsigned int restSchwarz,
        retruxx::vector<int, retruxx::allocator<int>>& generatedVehicleIds) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void VehiclesGeneratorPrototypeInfo::TuneVehicleBySchwarz(Vehicle* v, float desiredSchwarz, VehicleDescription const* vd) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int VehiclesGeneratorPrototypeInfo::_GetBestPrototypeByMoney(
        retruxx::vector<VehiclesGeneratorInfoCache::VehiclePartInfo, retruxx::allocator<VehiclesGeneratorInfoCache ::VehiclePartInfo>> const& partInfos,
        float money)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int VehiclesGeneratorPrototypeInfo::_GetBestPrototypeByMoneyEconomy(
        retruxx::vector<VehiclesGeneratorInfoCache::VehiclePartInfo, retruxx::allocator<VehiclesGeneratorInfoCache ::VehiclePartInfo>> const& partInfos,
        float money)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int VehiclesGeneratorPrototypeInfo::_GetRandomPrototypeByMoney(
        retruxx::vector<VehiclesGeneratorInfoCache::WareInfo, retruxx::allocator<VehiclesGeneratorInfoCache::WareInfo>> const& wareInfos,
        float money)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int VehiclesGeneratorPrototypeInfo::_GetRandomPrototypeByMoney(
        retruxx::vector<VehiclesGeneratorInfoCache::VehiclePartInfo, retruxx::allocator<VehiclesGeneratorInfoCache ::VehiclePartInfo>> const& partInfos,
        float money)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    unsigned int VehiclesGeneratorPrototypeInfo::_ChangeVehiclePart(Vehicle* v, CStr const& partName, int newPartProtoId, int affixGenetatorProtoId)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    VehiclesGeneratorInfoCache::VehiclesGeneratorInfoCache()
    {
        this->m_bInited = 0;
    }

    retruxx::map<CStr, VehiclesGeneratorInfoCache::VehicleGroupInfo> const& VehiclesGeneratorInfoCache::GetVehicleGroupInfos() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void VehiclesGeneratorInfoCache::EnsureInitialized()
    {
        if (!m_bInited)
        {
            _InitializeVehicleParts();
            _InitializeWares();
            _InitializeGuns();
            m_bInited = true;
        }
    }

    void VehiclesGeneratorInfoCache::_GetVehiclePartInfos(retruxx::vector<int> const& prototypes, retruxx::vector<VehiclePartInfo>& vehiclePartInfos)
    {
        vehiclePartInfos.clear();
        for (auto& protoId : prototypes)
        {
            auto proto = (VehiclePrototypeInfo*)thePrototypeManager->GetPrototypeInfo(protoId);

            VehiclePartInfo partInfo;
            partInfo.price = proto->GetBasePrice();
            partInfo.protoId = protoId;
            // TODO: check this
            partInfo.canBeUsedInAutoGenerating = false;
            vehiclePartInfos.push_back(std::move(partInfo));
        }
        std::sort(vehiclePartInfos.begin(), vehiclePartInfos.end());
    }

    void VehiclesGeneratorInfoCache::_GetVehiclePartInfosForVehicle(
        VehiclePrototypeInfo const* protoV,
        CStr const& partName,
        retruxx::vector<VehiclePartInfo>& vehiclePartInfos)
    {
        if (auto partInfo = protoV->GetPartDescriptionByName(partName))
        {
            retruxx::vector<int> goodParts;
            auto partResId = partInfo->GetPartResourceId();
            thePrototypeManager->GetPrototypeIdsByResourceId(partResId, goodParts);
            _GetVehiclePartInfos(goodParts, vehiclePartInfos);
        }
        else
        {
            vehiclePartInfos.clear();
        }
    }

    void VehiclesGeneratorInfoCache::_Initialize()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void VehiclesGeneratorInfoCache::_InitializeGuns()
    {
        retruxx::vector<int> gunsTypes;
        theResourceManager->GetResourceDescendants(theResourceManager->GetResourceId("GUN"), gunsTypes);

        for (auto& gun : gunsTypes)
        {
            retruxx::vector<int> gunTypePrototypes;
            thePrototypeManager->GetPrototypeIdsByResourceId(gun, gunTypePrototypes);

            retruxx::vector<ai::VehiclesGeneratorInfoCache::VehiclePartInfo> gunPartInfos;
            _GetVehiclePartInfos(gunTypePrototypes, gunPartInfos);

            m_gunInfos[theResourceManager->GetResourceName(gun)] = std::move(gunPartInfos);
        }
    }

    void VehiclesGeneratorInfoCache::_GetWareInfos(retruxx::vector<int> const& prototypes, retruxx::vector<WareInfo>& wareInfos)
    {
        wareInfos.clear();
        for (auto& id : prototypes)
        {
            auto proto = (VehiclePrototypeInfo*)thePrototypeManager->GetPrototypeInfo(id);
            WareInfo val;
            val.price = proto->GetBasePrice();
            val.protoId = id;
            m_wareInfos.push_back(std::move(val));
        }

        std::sort(wareInfos.begin(), wareInfos.end());
    }

    VehiclesGeneratorInfoCache::WareInfo VehiclesGeneratorInfoCache::_GetWareInfo(int) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    VehiclesGeneratorInfoCache::VehiclePartInfo VehiclesGeneratorInfoCache::_GetVehiclePartInfo(int) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void VehiclesGeneratorInfoCache::_GetAbstractVehiclesPrototypeIds(retruxx::vector<int, retruxx::allocator<int>>& abstractVehiclesPrototypeIds)
    {
        auto id = theResourceManager->GetResourceId("VEHICLE");

        retruxx::vector<int> vehiclesPrototypesIds;
        thePrototypeManager->GetPrototypeIdsByResourceId(id, vehiclesPrototypesIds);
        abstractVehiclesPrototypeIds.clear();

        for (auto& protoId : vehiclesPrototypesIds)
        {
            auto prototype = thePrototypeManager->GetPrototypeInfo(protoId);
            if (prototype->bIsAbstract())
            {
                abstractVehiclesPrototypeIds.push_back(protoId);
            }
        }
    }

    void VehiclesGeneratorInfoCache::_InitializeWares()
    {
        retruxx::vector<int> goodsTypes;
        theResourceManager->GetResourceDescendants(theResourceManager->GetResourceId("GOODS"), goodsTypes);

        retruxx::vector<int> allGoods;
        allGoods.reserve(goodsTypes.size());

        for (auto& good : goodsTypes)
        {
            if (!theResourceManager->ResourceHasChildren(good))
            {
                retruxx::vector<int> goodPrototypes;
                thePrototypeManager->GetPrototypeIdsByResourceId(good, goodPrototypes);
                if (goodPrototypes.size() == 1)
                {
                    allGoods.push_back(goodPrototypes.front());
                }
            }
        }

        _GetWareInfos(allGoods, m_wareInfos);
    }

    void VehiclesGeneratorInfoCache::_InitializeVehicleParts()
    {
        retruxx::vector<int> vehiclePrototypeIds;
        _GetAbstractVehiclesPrototypeIds(vehiclePrototypeIds);

        auto gunsId = theResourceManager->GetResourceId("GUN");

        for (auto toVehicleId = vehiclePrototypeIds.begin(); toVehicleId != vehiclePrototypeIds.end(); ++toVehicleId)
        {
            unsigned int prototypeId = *toVehicleId;
            ai::VehiclePrototypeInfo const* vehicleProto = nullptr;

            // Get the vehicle prototype info
            vehicleProto = dynamic_cast<ai::VehiclePrototypeInfo const*>(ai::thePrototypeManager->GetPrototypeInfo(prototypeId));

            if (!vehicleProto)
            {
                continue;
            }

            // Initialize new group info
            VehicleGroupInfo newGroupInfo;

            // Get basket parts
            _GetVehiclePartInfosForVehicle(vehicleProto, "BASKET", newGroupInfo.baskets);

            // Get chassis parts
            _GetVehiclePartInfosForVehicle(vehicleProto, "CHASSIS", newGroupInfo.chassises);

            // Get cabin parts
            _GetVehiclePartInfosForVehicle(vehicleProto, "CABIN", newGroupInfo.cabins);

            // Find gun parts
            for (auto partNameIter = vehicleProto->GetAllPartNames().begin(); partNameIter != vehicleProto->GetAllPartNames().end(); ++partNameIter)
            {
                // Get the part description
                auto* partDesc = vehicleProto->GetPartDescriptionByName((*partNameIter));

                if (!partDesc)
                    continue;

                // Get resource ID and check if it's a gun
                int partResourceId = partDesc->GetPartResourceId();
                ai::Resource* resource = ai::theResourceManager->GetResource(partResourceId);

                if (resource && resource->bIsKindOf(gunsId))
                {
                    newGroupInfo.gunPartNames.push_back(*partNameIter);
                }
            }
            // Insert into the map (assuming m_vehicleGroupInfos is a std::map)
            auto result = this->m_vehicleGroupInfos.emplace(vehicleProto->m_prototypeName, std::move(newGroupInfo));
        }
    }
}  // namespace ai
