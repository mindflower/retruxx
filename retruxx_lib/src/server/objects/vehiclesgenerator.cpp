#include "vehiclesgenerator.h"

#include <stdexcept>

namespace ai
{
	retruxx::vector<VehiclesGeneratorInfoCache::WareInfo> const& VehiclesGeneratorInfoCache::GetWareInfos() const
	{
		throw retruxx::logic_error("Not implemented");
	}

    bool VehiclesGeneratorInfoCache::VehiclePartInfo::operator<(
        const ai::VehiclesGeneratorInfoCache::VehiclePartInfo& rhs) const
    {
        throw std::logic_error("Not implemented");
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
        // TODO: implement VehiclesGeneratorPrototypeInfo::LoadFromXML
        auto result = ai::PrototypeInfo::LoadFromXML(xmlFile, xmlNode);
        return result;
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
        // TODO: implement VehiclesGeneratorInfoCache::EnsureInitialized
        // throw retruxx::logic_error("Not implemented");
	}

	void VehiclesGeneratorInfoCache::_GetVehiclePartInfos(retruxx::vector<int> const&, retruxx::vector<VehiclePartInfo>&)
	{
		throw retruxx::logic_error("Not implemented");
	}

	void VehiclesGeneratorInfoCache::_GetVehiclePartInfosForVehicle(VehiclePrototypeInfo const*, CStr const&,
		retruxx::vector<VehiclePartInfo>&)
	{
		throw retruxx::logic_error("Not implemented");
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

	void VehiclesGeneratorInfoCache::_GetAbstractVehiclesPrototypeIds(retruxx::vector<int, retruxx::allocator<int>>&)
	{
		throw retruxx::logic_error("Not implemented");
	}

	void VehiclesGeneratorInfoCache::_InitializeWares()
	{
		throw retruxx::logic_error("Not implemented");
	}

	void VehiclesGeneratorInfoCache::_InitializeVehicleParts()
	{
		throw retruxx::logic_error("Not implemented");
	}
}
