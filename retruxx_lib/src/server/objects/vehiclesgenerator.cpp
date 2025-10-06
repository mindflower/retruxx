#include "vehiclesgenerator.h"

#include <stdexcept>
#include <server/resourcemanager.h>
#include "base/prototypemanager.h"
#include <algorithm>

namespace ai
{
	retruxx::vector<VehiclesGeneratorInfoCache::WareInfo> const& VehiclesGeneratorInfoCache::GetWareInfos() const
	{
		throw retruxx::logic_error("Not implemented");
	}

    bool VehiclesGeneratorInfoCache::VehiclePartInfo::operator<(
        const ai::VehiclesGeneratorInfoCache::VehiclePartInfo& rhs) const
    {
        return this->price < rhs.price;
    }

    bool VehiclesGeneratorInfoCache::WareInfo::operator<(const ai::VehiclesGeneratorInfoCache::WareInfo& rhs) const
    {
        throw std::logic_error("Not implemented");
    }

    VehiclesGeneratorInfoCache::VehiclesGeneratorInfoCache(const VehiclesGeneratorInfoCache&)
    {
        throw std::logic_error("Not implemented");
    }

    const retruxx::map<CStr, retruxx::vector<VehiclesGeneratorInfoCache::VehiclePartInfo, retruxx::allocator<
    VehiclesGeneratorInfoCache::VehiclePartInfo>>, retruxx::less<CStr>, retruxx::allocator<retruxx::pair<CStr const,
    retruxx::vector<VehiclesGeneratorInfoCache::VehiclePartInfo, retruxx::allocator<VehiclesGeneratorInfoCache::
    VehiclePartInfo>>>>>& VehiclesGeneratorInfoCache::GetGunInfos() const
    {
        throw std::logic_error("Not implemented");
    }

    VehiclesGeneratorPrototypeInfo::VehiclesGeneratorPrototypeInfo()
    {
    }

    Obj* VehiclesGeneratorPrototypeInfo::CreateTargetObject() const
    {
        throw std::logic_error("Not implemented");
    }

    bool VehiclesGeneratorPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode)
    {
        throw std::logic_error("Not implemented");
    }

    void VehiclesGeneratorPrototypeInfo::PostLoad()
    {
        throw std::logic_error("Not implemented");
    }

    void VehiclesGeneratorPrototypeInfo::Generate(unsigned int restSchwarz,
        retruxx::vector<int, retruxx::allocator<int>>& generatedVehicleIds) const
    {
        throw std::logic_error("Not implemented");
    }

    void VehiclesGeneratorPrototypeInfo::GenerateAndPlace(
        const retruxx::vector<CVector, retruxx::allocator<CVector>>& points, unsigned int restSchwarz,
        retruxx::vector<int, retruxx::allocator<int>>& generatedVehicleIds) const
    {
        throw std::logic_error("Not implemented");
    }

    void VehiclesGeneratorPrototypeInfo::TuneVehicleBySchwarz(Vehicle* v, float desiredSchwarz,
        const VehicleDescription* vd) const
    {
        throw std::logic_error("Not implemented");
    }

    int VehiclesGeneratorPrototypeInfo::_GetBestPrototypeByMoney(
        const retruxx::vector<VehiclesGeneratorInfoCache::VehiclePartInfo, retruxx::allocator<VehiclesGeneratorInfoCache
        ::VehiclePartInfo>>& partInfos, float money)
    {
        throw std::logic_error("Not implemented");
    }

    int VehiclesGeneratorPrototypeInfo::_GetBestPrototypeByMoneyEconomy(
        const retruxx::vector<VehiclesGeneratorInfoCache::VehiclePartInfo, retruxx::allocator<VehiclesGeneratorInfoCache
        ::VehiclePartInfo>>& partInfos, float money)
    {
        throw std::logic_error("Not implemented");
    }

    int VehiclesGeneratorPrototypeInfo::_GetRandomPrototypeByMoney(
        const retruxx::vector<VehiclesGeneratorInfoCache::WareInfo, retruxx::allocator<VehiclesGeneratorInfoCache::
        WareInfo>>& wareInfos, float money)
    {
        throw std::logic_error("Not implemented");
    }

    int VehiclesGeneratorPrototypeInfo::_GetRandomPrototypeByMoney(
        const retruxx::vector<VehiclesGeneratorInfoCache::VehiclePartInfo, retruxx::allocator<VehiclesGeneratorInfoCache
        ::VehiclePartInfo>>& partInfos, float money)
    {
        throw std::logic_error("Not implemented");
    }

    unsigned int VehiclesGeneratorPrototypeInfo::_ChangeVehiclePart(Vehicle* v, const CStr& partName,
        int newPartProtoId, int affixGenetatorProtoId)
    {
        throw std::logic_error("Not implemented");
    }

	VehiclesGeneratorInfoCache::VehiclesGeneratorInfoCache()
	{
        this->m_bInited = 0;
	}

	retruxx::map<CStr, VehiclesGeneratorInfoCache::VehicleGroupInfo> const& VehiclesGeneratorInfoCache::
	GetVehicleGroupInfos() const
	{
		throw retruxx::logic_error("Not implemented");
	}

	void VehiclesGeneratorInfoCache::EnsureInitialized()
	{
        if (!this->m_bInited)
        {
            _InitializeVehicleParts();
            _InitializeWares();
            _InitializeGuns();
            this->m_bInited = true;
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

	void VehiclesGeneratorInfoCache::_GetVehiclePartInfosForVehicle(VehiclePrototypeInfo const* protoV, CStr const& partName,
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
		throw retruxx::logic_error("Not implemented");
	}

	void VehiclesGeneratorInfoCache::_InitializeGuns()
	{
		throw retruxx::logic_error("Not implemented");
	}

	void VehiclesGeneratorInfoCache::_GetWareInfos(retruxx::vector<int> const&, retruxx::vector<WareInfo>&)
	{
		throw retruxx::logic_error("Not implemented");
	}

	VehiclesGeneratorInfoCache::WareInfo VehiclesGeneratorInfoCache::_GetWareInfo(int) const
	{
		throw retruxx::logic_error("Not implemented");
	}

	VehiclesGeneratorInfoCache::VehiclePartInfo VehiclesGeneratorInfoCache::_GetVehiclePartInfo(int) const
	{
		throw retruxx::logic_error("Not implemented");
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
		throw retruxx::logic_error("Not implemented");
	}

	void VehiclesGeneratorInfoCache::_InitializeVehicleParts()
	{
        retruxx::vector<int> vehiclePrototypeIds;
        _GetAbstractVehiclesPrototypeIds(vehiclePrototypeIds);
        
        auto gunsId = theResourceManager->GetResourceId("GUN");
        
        for (auto toVehicleId = vehiclePrototypeIds.begin(); toVehicleId != vehiclePrototypeIds.end(); ++toVehicleId)
        {
            unsigned int prototypeId = *toVehicleId;
            const ai::VehiclePrototypeInfo* vehicleProto = nullptr;
        
            // Get the vehicle prototype info
             vehicleProto = dynamic_cast<const ai::VehiclePrototypeInfo*>(ai::thePrototypeManager->GetPrototypeInfo(prototypeId));
        
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
            for (auto partNameIter = vehicleProto->GetAllPartNames().begin();
                partNameIter != vehicleProto->GetAllPartNames().end();
                ++partNameIter)
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
}
