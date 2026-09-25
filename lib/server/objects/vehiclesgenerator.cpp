#include "vehiclesgenerator.h"
#include "team.h"
#include "ware.h"
#include "core/log.h"
#include "server/izvratrepository.h"
#include "server/objects/physicbodies/vehiclepart.h"
#include "server/obstacle.h"
#include "server/resourcemanager.h"
#include "server/objects/affixgenerator.h"
#include "server/objects/physicbodies/physichelpers.h"
#include "server/objects/physicbodies/geoms/sphereforintersection.h"
#include "base/objcontainer.h"
#include "base/prototypemanager.h"

#include <algorithm>
#include <cmath>
#include <set>

namespace ai
{
    namespace
    {
        CStr const CABIN("CABIN");
        CStr const BASKET("BASKET");
    }  // namespace

    retruxx::vector<VehiclesGeneratorInfoCache::WareInfo> const& VehiclesGeneratorInfoCache::GetWareInfos() const
    {
        // RVA 0x85B2A0
        return m_wareInfos;
    }

    bool VehiclesGeneratorInfoCache::VehiclePartInfo::operator<(ai::VehiclesGeneratorInfoCache::VehiclePartInfo const& rhs) const
    {
        // RVA 0x85B270
        return this->price < rhs.price;
    }

    bool VehiclesGeneratorInfoCache::WareInfo::operator<(ai::VehiclesGeneratorInfoCache::WareInfo const& rhs) const
    {
        // RVA 0x85B280
        return this->price < rhs.price;
    }

    VehiclesGeneratorInfoCache::VehiclesGeneratorInfoCache(VehiclesGeneratorInfoCache const& other) :
        m_bInited(other.m_bInited),
        m_vehicleGroupInfos(other.m_vehicleGroupInfos),
        m_wareInfos(other.m_wareInfos),
        m_gunInfos(other.m_gunInfos)
    {
        // Declared in the PDB but never emitted in the shipped build; a member-wise copy.
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
        // RVA 0x85B2B0
        return m_gunInfos;
    }

    VehiclesGeneratorPrototypeInfo::VehicleDescription::VehicleDescription(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        // RVA 0x860A80
        LoadFromXML(xmlFile, xmlNode);
    }

    void VehiclesGeneratorPrototypeInfo::VehicleDescription::PostLoad()
    {
        // RVA 0x85EB20
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
        // RVA 0x8606C0
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
        // RVA 0x85C880 - a generator only describes vehicles; it has no object of its own.
        SYS_ERROR("0");
        return nullptr;
    }

    bool VehiclesGeneratorPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        // RVA 0x863850
        bool const result = PrototypeInfo::LoadFromXML(xmlFile, xmlNode);
        if (result)
        {
            m_desiredCountLow = -1;
            m_desiredCountHigh = -1;

            CStr desiredCount;
            m3d::SafeStrAttrib(desiredCount, xmlNode, "DesiredCount");

            retruxx::vector<CStr> tknsDesiredCount;
            m3d::Tokenize(desiredCount, tknsDesiredCount, "-");

            if (!tknsDesiredCount.empty() && !tknsDesiredCount.front().empty())
            {
                m_desiredCountLow = std::atoi(tknsDesiredCount.front().c_str());
            }
            m_desiredCountHigh = m_desiredCountLow;

            // NOTE: the second token is taken, not the last.
            if (tknsDesiredCount.size() > 1 && !tknsDesiredCount[1].empty())
            {
                m_desiredCountHigh = std::atoi(tknsDesiredCount[1].c_str());
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
        // RVA 0x85F910 - NOTE: does not chain to PrototypeInfo::PostLoad.
        for (auto& descr : m_vehicleDescriptions)
        {
            descr.PostLoad();
        }
    }

    void VehiclesGeneratorPrototypeInfo::Generate(unsigned int restSchwarz, retruxx::vector<int, retruxx::allocator<int>>& generatedVehicleIds) const
    {
        // RVA 0x862B50
        // Builds one candidate set of vehicles per description, each vehicle a random pick from
        // the description's prototypes and tuned to its share of restSchwarz, keeps the set whose
        // total Schwarz is closest to restSchwarz and removes the others.
        size_t const numDescriptions = m_vehicleDescriptions.size();
        retruxx::vector<retruxx::vector<int>> possibleSetsOfVehicles(numDescriptions);
        retruxx::vector<retruxx::vector<unsigned>> possibleIndexesOfVehicles(numDescriptions);

        unsigned sizeOfSet;
        if (m_desiredCountLow == -1)
        {
            // NOTE: the first description's prototype count, whichever set is built.
            sizeOfSet = static_cast<unsigned>(m_vehicleDescriptions.front().vehiclePrototypeIds.size());
        }
        else
        {
            sizeOfSet = m_desiredCountLow + rand() % (m_desiredCountHigh - m_desiredCountLow + 1);
        }

        for (size_t i = 0; i < possibleIndexesOfVehicles.size(); ++i)
        {
            possibleIndexesOfVehicles[i].resize(sizeOfSet, 0);
            for (unsigned j = 0; j < sizeOfSet; ++j)
            {
                unsigned const numPrototypes = static_cast<unsigned>(m_vehicleDescriptions[i].vehiclePrototypeIds.size());
                possibleIndexesOfVehicles[i][j] = rand() % numPrototypes;
            }
        }

        unsigned closestSchwarz = 0xFFFFFFFF;
        size_t closetSetIdx = 0;
        for (size_t i = 0; i < possibleSetsOfVehicles.size(); ++i)
        {
            possibleSetsOfVehicles[i].resize(sizeOfSet, 0);
            unsigned curSetsSchwarz = 0;
            for (unsigned j = 0; j < sizeOfSet; ++j)
            {
                VehicleDescription const& description = m_vehicleDescriptions[i];
                possibleSetsOfVehicles[i][j] =
                    theObjects->CreateNewObject(description.vehiclePrototypeIds[possibleIndexesOfVehicles[i][j]], "", -1, -1);
                Obj* const obj = possibleSetsOfVehicles[i][j] >= 0 ? theObjects->GetEntityByObjId(possibleSetsOfVehicles[i][j]) : nullptr;
                if (!obj || !obj->IsKindOf(RT_CLASS_LOCAL(Vehicle)))
                {
                    SYS_ERROR("vehicle && IS_KIND_OF( vehicle, Vehicle )");
                }
                // NOTE: carries on with the object after the assertion.
                auto* const vehicle = static_cast<Vehicle*>(obj);
                if (description.bTuningBySchwartz && restSchwarz)
                {
                    TuneVehicleBySchwarz(vehicle, static_cast<float>(static_cast<double>(restSchwarz) * description.partOfSchwartz), &description);
                }
                curSetsSchwarz += vehicle->GetSchwarz();
            }
            if (fabs(static_cast<double>(closestSchwarz) - static_cast<double>(restSchwarz)) >
                fabs(static_cast<double>(curSetsSchwarz) - static_cast<double>(restSchwarz)))
            {
                closestSchwarz = curSetsSchwarz;
                closetSetIdx = i;
            }
        }

        for (size_t i = 0; i < possibleSetsOfVehicles.size(); ++i)
        {
            if (i == closetSetIdx)
            {
                generatedVehicleIds = possibleSetsOfVehicles[i];
                continue;
            }
            for (unsigned j = 0; j < sizeOfSet; ++j)
            {
                Obj* const obj = possibleSetsOfVehicles[i][j] >= 0 ? theObjects->GetEntityByObjId(possibleSetsOfVehicles[i][j]) : nullptr;
                if (!obj || !obj->IsKindOf(RT_CLASS_LOCAL(Vehicle)))
                {
                    SYS_ERROR("vehicle && IS_KIND_OF( vehicle, Vehicle )");
                }
                obj->Remove();
            }
        }
    }

    void VehiclesGeneratorPrototypeInfo::GenerateAndPlace(
        retruxx::vector<CVector, retruxx::allocator<CVector>> const& points,
        unsigned int restSchwarz,
        retruxx::vector<int, retruxx::allocator<int>>& generatedVehicleIds) const
    {
        // RVA 0x8635A0
        // Generates the vehicles and puts them at the first point, or, for larger groups and with
        // a chance growing with their size, the back half at the last point.
        Generate(restSchwarz, generatedVehicleIds);
        bool bSplit = false;
        if (generatedVehicleIds.size() > 2)
        {
            bSplit = static_cast<double>(generatedVehicleIds.size()) * 0.25 > static_cast<double>(rand()) * 0.000030518509;
        }
        size_t const halfSize = generatedVehicleIds.size() >> 1;
        for (size_t k = 0; k < generatedVehicleIds.size(); ++k)
        {
            int const id = generatedVehicleIds[k];
            auto* const vehicle = id >= 0 ? static_cast<Vehicle*>(theObjects->GetEntityByObjId(id)) : nullptr;
            CVector const pos = bSplit && k > halfSize ? points.back() : points.front();
            CVector newPos;
            std::set<m3d::Class*> const noClasses;
            // NOTE: the vehicle is used without a null check.
            float const radius =
                vehicle->m_intersectionObstacle ? vehicle->m_intersectionObstacle->m_intersectionSphere->GetRadius() : 0.0f;
            bool const bValid = GetValidPosition(pos, radius, 0xFF, newPos, false, false, noClasses);
            vehicle->SetGamePositionOnGround(bValid ? newPos : pos, true, false);
        }
    }

    void VehiclesGeneratorPrototypeInfo::TuneVehicleBySchwarz(Vehicle* v, float desiredSchwarz, VehicleDescription const* vd) const
    {
        // RVA 0x861CD0
        // Spends desiredSchwarz on the vehicle: shares of it (each varied by +-20%) buy the best
        // cabin and basket its group offers, random guns for random free gun slots and random
        // wares for its repository.
        theVehiclesGeneratorInfoCache->EnsureInitialized();
        VehiclePrototypeInfo const* const vProto = static_cast<VehiclePrototypeInfo const*>(v->GetPrototypeInfo());

        // The group is the vehicle's root prototype.
        CStr protoName(v->GetPrototypeInfo()->m_prototypeName);
        CStr groupName;
        while (!protoName.empty())
        {
            PrototypeInfo const* const proto = thePrototypeManager->GetPrototypeInfo(thePrototypeManager->GetPrototypeId(protoName));
            groupName = proto->m_prototypeName;
            protoName = proto->GetParentPrototypeName();
        }

        float const moneyForCabin = ((static_cast<float>(rand()) * 0.000030518509f) * 0.40000004f + 0.80000001f) * m_partOfSchwartzForCabin;
        float const moneyForBasket = ((static_cast<float>(rand()) * 0.000030518509f) * 0.40000004f + 0.80000001f) * m_partOfSchwartzForBasket;
        float const partForGuns = ((static_cast<float>(rand()) * 0.000030518509f) * 0.40000004f + 0.80000001f) * m_partOfSchwartzForGuns;
        float const partForWares = ((static_cast<float>(rand()) * 0.000030518509f) * 0.40000004f + 0.80000001f) * m_partOfSchwartzForWares;

        auto const& vehicleGroupInfos = theVehiclesGeneratorInfoCache->GetVehicleGroupInfos();
        auto const& gunInfos = theVehiclesGeneratorInfoCache->GetGunInfos();

        retruxx::vector<VehiclesGeneratorInfoCache::WareInfo> wareInfos;
        if (vd)
        {
            for (auto const& wareInfo : theVehiclesGeneratorInfoCache->GetWareInfos())
            {
                if (std::find(vd->waresPrototypesIds.begin(), vd->waresPrototypesIds.end(), wareInfo.protoId) != vd->waresPrototypesIds.end())
                {
                    wareInfos.push_back(wareInfo);
                }
            }
        }

        auto const groupIt = vehicleGroupInfos.find(groupName);
        if (groupIt == vehicleGroupInfos.end())
        {
            M3D_LOG_ERR("Error: invalid group name: '" + groupName + CStr("' for ") + v->GetDebugDescription());
            return;
        }
        VehiclesGeneratorInfoCache::VehicleGroupInfo const& groupInfo = groupIt->second;
        int const bestBasket = _GetBestPrototypeByMoney(groupInfo.baskets, moneyForBasket * desiredSchwarz);
        int const bestCabin = _GetBestPrototypeByMoney(groupInfo.cabins, moneyForCabin * desiredSchwarz);
        _ChangeVehiclePart(v, CABIN, bestCabin, -1);
        _ChangeVehiclePart(v, BASKET, bestBasket, -1);

        retruxx::vector<CStr> gunPositionsForFill(groupInfo.gunPartNames);
        float moneyForGuns = partForGuns * desiredSchwarz;
        while (moneyForGuns > 0.0 && !gunPositionsForFill.empty())
        {
            // A random free gun slot, taken off the list.
            size_t const idx = rand() % gunPositionsForFill.size();
            CStr const gunPartName(gunPositionsForFill[idx]);
            gunPositionsForFill.erase(gunPositionsForFill.begin() + idx);
            if (!v->CanPartBeAttached(gunPartName))
            {
                continue;
            }
            CStr const resourceName =
                theResourceManager->GetResourceName(vProto->GetPartDescriptionByName(gunPartName)->GetPartResourceId());
            auto const gunIt = gunInfos.find(resourceName);
            if (gunIt == gunInfos.end())
            {
                M3D_LOG_ERR(
                    "Error: invalid resource name for gun: '" + gunPartName + CStr("' for ") + v->GetDebugDescription() + CStr(": '") +
                    resourceName + CStr("'"));
                return;
            }
            if (!gunIt->second.empty())
            {
                int const gunId = _GetRandomPrototypeByMoney(gunIt->second, moneyForGuns);
                // NOTE: vd is dereferenced here without a null check.
                unsigned const spent = _ChangeVehiclePart(v, gunPartName, gunId, vd->gunAffixGeneratorPrototypeId);
                moneyForGuns = static_cast<float>(moneyForGuns - static_cast<double>(spent));
            }
        }

        IzvratRepository* const repository = v->GetRepository();
        if (!repository)
        {
            return;
        }
        float moneyForWares = partForWares * desiredSchwarz;
        while (moneyForWares > 0.1)
        {
            int const wareId = _GetRandomPrototypeByMoney(wareInfos, moneyForWares);
            PrototypeInfo const* const wareProto = thePrototypeManager->GetPrototypeInfo(wareId);
            if (!wareProto || !repository->CanPlaceItems(wareId, 1))
            {
                break;
            }
            if (moneyForWares <= static_cast<double>(wareProto->GetBasePrice()))
            {
                break;
            }
            repository->AddItems(wareId, 1);
            moneyForWares = static_cast<float>(moneyForWares - static_cast<double>(wareProto->GetBasePrice()));
        }
    }

    int VehiclesGeneratorPrototypeInfo::_GetBestPrototypeByMoney(
        retruxx::vector<VehiclesGeneratorInfoCache::VehiclePartInfo, retruxx::allocator<VehiclesGeneratorInfoCache ::VehiclePartInfo>> const& partInfos,
        float money)
    {
        // RVA 0x85D270
        // The dearest usable part cheaper than money, or failing that the cheapest usable one.
        int const size = static_cast<int>(partInfos.size());
        if (size == 0)
        {
            return -1;
        }
        int affordable = 0;
        while (affordable < size && money > static_cast<double>(partInfos[affordable].price))
        {
            ++affordable;
        }
        for (int i = affordable - 1; i >= 0; --i)
        {
            if (partInfos[i].canBeUsedInAutoGenerating)
            {
                return partInfos[i].protoId;
            }
        }
        for (int i = 0; i < size; ++i)
        {
            if (partInfos[i].canBeUsedInAutoGenerating)
            {
                return partInfos[i].protoId;
            }
        }
        return -1;
    }

    int VehiclesGeneratorPrototypeInfo::_GetBestPrototypeByMoneyEconomy(
        retruxx::vector<VehiclesGeneratorInfoCache::VehiclePartInfo, retruxx::allocator<VehiclesGeneratorInfoCache ::VehiclePartInfo>> const& partInfos,
        float money)
    {
        // RVA 0x85D330 - the dearest usable part cheaper than money, or none.
        int const size = static_cast<int>(partInfos.size());
        if (size == 0)
        {
            return -1;
        }
        int affordable = 0;
        while (affordable < size && money > static_cast<double>(partInfos[affordable].price))
        {
            ++affordable;
        }
        for (int i = affordable - 1; i >= 0; --i)
        {
            if (partInfos[i].canBeUsedInAutoGenerating)
            {
                return partInfos[i].protoId;
            }
        }
        return -1;
    }

    int VehiclesGeneratorPrototypeInfo::_GetRandomPrototypeByMoney(
        retruxx::vector<VehiclesGeneratorInfoCache::WareInfo, retruxx::allocator<VehiclesGeneratorInfoCache::WareInfo>> const& wareInfos,
        float money)
    {
        // RVA 0x85D4A0
        // A random ware among the ones cheaper than money; the cheapest always counts, whatever
        // its price.
        if (wareInfos.empty())
        {
            return -1;
        }
        size_t count = 1;
        while (count < wareInfos.size() && money > static_cast<double>(wareInfos[count].price))
        {
            ++count;
        }
        return wareInfos[rand() % count].protoId;
    }

    int VehiclesGeneratorPrototypeInfo::_GetRandomPrototypeByMoney(
        retruxx::vector<VehiclesGeneratorInfoCache::VehiclePartInfo, retruxx::allocator<VehiclesGeneratorInfoCache ::VehiclePartInfo>> const& partInfos,
        float money)
    {
        // RVA 0x85D3C0
        // A random usable part among the ones cheaper than money (the cheapest always counts):
        // the pick, or the nearest usable one below it, or else the cheapest usable one.
        int const size = static_cast<int>(partInfos.size());
        if (size == 0)
        {
            return -1;
        }
        int count = 1;
        while (count < size && money > static_cast<double>(partInfos[count].price))
        {
            ++count;
        }
        for (int i = rand() % count; i >= 0; --i)
        {
            if (partInfos[i].canBeUsedInAutoGenerating)
            {
                return partInfos[i].protoId;
            }
        }
        for (int i = 0; i < count; ++i)
        {
            if (partInfos[i].canBeUsedInAutoGenerating)
            {
                return partInfos[i].protoId;
            }
        }
        return -1;
    }

    unsigned int VehiclesGeneratorPrototypeInfo::_ChangeVehiclePart(Vehicle* v, CStr const& partName, int newPartProtoId, int affixGenetatorProtoId)
    {
        // RVA 0x85DE10 - swaps in a new part and returns its price.
        if (newPartProtoId == -1)
        {
            return 0;
        }
        VehiclePart* const oldPart = v->GetPartByName(partName);
        int const newPartId = theObjects->CreateNewObject(newPartProtoId, "", -1, -1);
        Obj* const newPart = newPartId >= 0 ? theObjects->GetEntityByObjId(newPartId) : nullptr;
        if (!newPart)
        {
            M3D_LOG_ERR("Error: created a NULL part by prototype id " + CStr(newPartProtoId));
            PrototypeInfo const* const prototype = thePrototypeManager->GetPrototypeInfo(newPartProtoId);
            if (!prototype)
            {
                M3D_LOG_INFO(CStr("Prototype is NULL"));
            }
            else
            {
                M3D_LOG_INFO("Prototype name = '" + prototype->m_prototypeName + CStr("'"));
            }
            SYS_ERROR("!\"Critical error, see log\"");
            return 0;
        }
        if (affixGenetatorProtoId != -1)
        {
            // NOTE: an unknown affix generator is used without a null check.
            static_cast<AffixGeneratorPrototypeInfo const*>(thePrototypeManager->GetPrototypeInfo(affixGenetatorProtoId))
                ->GenerateAffixesForObj(newPart, 2);
        }
        // NOTE: the new object is assumed to be a VehiclePart without a type check.
        v->SetPartByName(partName, static_cast<VehiclePart*>(newPart), false);
        if (oldPart)
        {
            oldPart->Remove();
        }
        return newPart->GetPrice(nullptr);
    }

    VehiclesGeneratorInfoCache::VehiclesGeneratorInfoCache()
    {
        // RVA 0x863E90
        this->m_bInited = 0;
    }

    retruxx::map<CStr, VehiclesGeneratorInfoCache::VehicleGroupInfo> const& VehiclesGeneratorInfoCache::GetVehicleGroupInfos() const
    {
        // RVA 0x85B290
        return m_vehicleGroupInfos;
    }

    void VehiclesGeneratorInfoCache::EnsureInitialized()
    {
        // RVA 0x861CB0
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
        // RVA 0x8602B0
        vehiclePartInfos.clear();
        for (auto& protoId : prototypes)
        {
            vehiclePartInfos.push_back(_GetVehiclePartInfo(protoId));
        }
        std::sort(vehiclePartInfos.begin(), vehiclePartInfos.end());
    }

    void VehiclesGeneratorInfoCache::_GetVehiclePartInfosForVehicle(
        VehiclePrototypeInfo const* protoV,
        CStr const& partName,
        retruxx::vector<VehiclePartInfo>& vehiclePartInfos)
    {
        // RVA 0x860470
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
        // RVA 0x8618D0 - NOTE: unlike EnsureInitialized, does not set m_bInited.
        _InitializeVehicleParts();
        _InitializeWares();
        _InitializeGuns();
    }

    void VehiclesGeneratorInfoCache::_InitializeGuns()
    {
        // RVA 0x861240
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
        // RVA 0x860370
        wareInfos.clear();
        for (auto& id : prototypes)
        {
            wareInfos.push_back(_GetWareInfo(id));
        }
        std::sort(wareInfos.begin(), wareInfos.end());
    }

    VehiclesGeneratorInfoCache::WareInfo VehiclesGeneratorInfoCache::_GetWareInfo(int id) const
    {
        // RVA 0x85DDB0 - NOTE: an unknown prototype is used without a null check.
        WareInfo info;
        info.price = thePrototypeManager->GetPrototypeInfo(id)->GetBasePrice();
        info.protoId = id;
        return info;
    }

    VehiclesGeneratorInfoCache::VehiclePartInfo VehiclesGeneratorInfoCache::_GetVehiclePartInfo(int id) const
    {
        // RVA 0x85DD50 - NOTE: the prototype is assumed to be a vehicle part without a check.
        auto const* const proto = static_cast<VehiclePartPrototypeInfo const*>(thePrototypeManager->GetPrototypeInfo(id));
        VehiclePartInfo info;
        info.price = proto->GetBasePrice();
        info.canBeUsedInAutoGenerating = proto->m_canBeUsedInAutogenerating;
        info.protoId = id;
        return info;
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
        // RVA 0x860530
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
        // RVA 0x8615D0
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
