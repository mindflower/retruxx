#pragma once
#include <map>
#include <vector>

class CStr;

namespace ai
{
    class VehiclePrototypeInfo;

    class VehiclesGeneratorInfoCache
    {
    public:
        struct WareInfo
        {
            unsigned int price;
            int protoId;

        public:
            bool operator<(WareInfo const&) const;

        };
        struct VehiclePartInfo
        {
            unsigned int price;
            bool canBeUsedInAutoGenerating;
            int protoId;

        public:
            bool operator<(VehiclePartInfo const&) const;

        };

        class VehicleGroupInfo
        {
        private:
            std::vector<VehiclePartInfo> cabins;
            std::vector<VehiclePartInfo> chassises;
            std::vector<VehiclePartInfo> baskets;
            std::vector<CStr> gunPartNames;
        };

    public:
        std::vector<WareInfo> const & GetWareInfos() const ;
        VehiclesGeneratorInfoCache();
        std::map<CStr,VehicleGroupInfo> const & GetVehicleGroupInfos() const ;
        void EnsureInitialized();

    private:
        void _GetVehiclePartInfos(std::vector<int> const &,std::vector<VehiclePartInfo> &);
        void _GetVehiclePartInfosForVehicle(VehiclePrototypeInfo const *,CStr const &,std::vector<VehiclePartInfo> &);
        void _Initialize();
        //GetGunInfos();
        void _InitializeGuns();
        void _GetWareInfos(std::vector<int> const &,std::vector<WareInfo> &);
        WareInfo _GetWareInfo(int) const ;
        VehiclePartInfo _GetVehiclePartInfo(int) const ;
        void _GetAbstractVehiclesPrototypeIds(std::vector<int,std::allocator<int> > &);
        void _InitializeWares();
        void _InitializeVehicleParts();

    private:
        bool m_bInited;
        std::map<CStr,VehicleGroupInfo> m_vehicleGroupInfos;
        std::vector<WareInfo> m_wareInfos;
        std::map<CStr,std::vector<VehiclePartInfo>> m_gunInfos;
    };
}
