#include "vehiclesgenerator.h"

#include <stdexcept>

namespace ai
{
	std::vector<VehiclesGeneratorInfoCache::WareInfo> const& VehiclesGeneratorInfoCache::GetWareInfos() const
	{
		throw std::logic_error("Not implemented");
	}

	VehiclesGeneratorInfoCache::VehiclesGeneratorInfoCache()
	{
		throw std::logic_error("Not implemented");
	}

	std::map<CStr, VehiclesGeneratorInfoCache::VehicleGroupInfo> const& VehiclesGeneratorInfoCache::
	GetVehicleGroupInfos() const
	{
		throw std::logic_error("Not implemented");
	}

	void VehiclesGeneratorInfoCache::EnsureInitialized()
	{
		throw std::logic_error("Not implemented");
	}

	void VehiclesGeneratorInfoCache::_GetVehiclePartInfos(std::vector<int> const&, std::vector<VehiclePartInfo>&)
	{
		throw std::logic_error("Not implemented");
	}

	void VehiclesGeneratorInfoCache::_GetVehiclePartInfosForVehicle(VehiclePrototypeInfo const*, CStr const&,
		std::vector<VehiclePartInfo>&)
	{
		throw std::logic_error("Not implemented");
	}

	void VehiclesGeneratorInfoCache::_Initialize()
	{
		throw std::logic_error("Not implemented");
	}

	void VehiclesGeneratorInfoCache::_InitializeGuns()
	{
		throw std::logic_error("Not implemented");
	}

	void VehiclesGeneratorInfoCache::_GetWareInfos(std::vector<int> const&, std::vector<WareInfo>&)
	{
		throw std::logic_error("Not implemented");
	}

	VehiclesGeneratorInfoCache::WareInfo VehiclesGeneratorInfoCache::_GetWareInfo(int) const
	{
		throw std::logic_error("Not implemented");
	}

	VehiclesGeneratorInfoCache::VehiclePartInfo VehiclesGeneratorInfoCache::_GetVehiclePartInfo(int) const
	{
		throw std::logic_error("Not implemented");
	}

	void VehiclesGeneratorInfoCache::_GetAbstractVehiclesPrototypeIds(std::vector<int, std::allocator<int>>&)
	{
		throw std::logic_error("Not implemented");
	}

	void VehiclesGeneratorInfoCache::_InitializeWares()
	{
		throw std::logic_error("Not implemented");
	}

	void VehiclesGeneratorInfoCache::_InitializeVehicleParts()
	{
		throw std::logic_error("Not implemented");
	}
}
