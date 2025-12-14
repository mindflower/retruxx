#pragma once
#include "vehicle.h"
#include "retruxx/common.h"

namespace ai
{
    class VehiclesGeneratorInfoCache
    {
    public:
        struct VehiclePartInfo
        {
            unsigned int price;
            bool canBeUsedInAutoGenerating;
            int protoId;

            bool operator<(ai::VehiclesGeneratorInfoCache::VehiclePartInfo const& rhs) const;
        };

        using VehiclePartInfoVector = retruxx::vector<VehiclePartInfo>;

        struct WareInfo
        {
            unsigned int price;
            int protoId;

            bool operator<(ai::VehiclesGeneratorInfoCache::WareInfo const& rhs) const;
        };

        using WareInfoVector = retruxx::vector<WareInfo>;

        struct VehicleGroupInfo
        {
            VehiclePartInfoVector cabins;
            VehiclePartInfoVector chassises;
            VehiclePartInfoVector baskets;
            retruxx::vector<CStr> gunPartNames;
        };

        using VehicleGroupInfoMap = retruxx::map<CStr, VehicleGroupInfo>;
        using GunsGroupInfoMap = retruxx::map<CStr, VehiclePartInfoVector>;

    public:
        VehiclesGeneratorInfoCache(VehiclesGeneratorInfoCache const&);
        VehiclesGeneratorInfoCache();
        void EnsureInitialized();
        retruxx::map<CStr, VehicleGroupInfo, retruxx::less<CStr>, retruxx::allocator<retruxx::pair<CStr const, VehicleGroupInfo>>> const& GetVehicleGroupInfos()
            const;
        retruxx::vector<WareInfo, retruxx::allocator<WareInfo>> const& GetWareInfos() const;
        retruxx::map<
            CStr,
            retruxx::vector<VehiclePartInfo, retruxx::allocator<VehiclePartInfo>>,
            retruxx::less<CStr>,
            retruxx::allocator<retruxx::pair<CStr const, retruxx::vector<VehiclePartInfo, retruxx::allocator<VehiclePartInfo>>>>> const&
            GetGunInfos() const;

    private:
        bool m_bInited;
        retruxx::map<CStr, VehicleGroupInfo, retruxx::less<CStr>, retruxx::allocator<retruxx::pair<CStr const, VehicleGroupInfo>>> m_vehicleGroupInfos;
        retruxx::vector<WareInfo, retruxx::allocator<WareInfo>> m_wareInfos;
        retruxx::map<
            CStr,
            retruxx::vector<VehiclePartInfo, retruxx::allocator<VehiclePartInfo>>,
            retruxx::less<CStr>,
            retruxx::allocator<retruxx::pair<CStr const, retruxx::vector<VehiclePartInfo, retruxx::allocator<VehiclePartInfo>>>>>
            m_gunInfos;

        VehiclePartInfo _GetVehiclePartInfo(int id) const;
        WareInfo _GetWareInfo(int id) const;
        void _GetVehiclePartInfos(
            retruxx::vector<int, retruxx::allocator<int>> const& prototypes,
            retruxx::vector<VehiclePartInfo, retruxx::allocator<VehiclePartInfo>>& vehiclePartInfos);
        void _GetWareInfos(retruxx::vector<int, retruxx::allocator<int>> const& prototypes, retruxx::vector<WareInfo, retruxx::allocator<WareInfo>>& wareInfos);
        void _GetVehiclePartInfosForVehicle(
            VehiclePrototypeInfo const* protoV,
            CStr const& partName,
            retruxx::vector<VehiclePartInfo, retruxx::allocator<VehiclePartInfo>>& vehiclePartInfos);
        void _GetAbstractVehiclesPrototypeIds(retruxx::vector<int, retruxx::allocator<int>>& abstractVehiclesPrototypeIds);
        void _InitializeVehicleParts();
        void _InitializeWares();
        void _InitializeGuns();
        void _Initialize();
    };

    class VehiclesGeneratorPrototypeInfo : public PrototypeInfo
    {
    public:
        struct VehicleDescription
        {
            bool bTuningBySchwartz;
            float partOfSchwartz;
            retruxx::vector<int> vehiclePrototypeIds;
            retruxx::vector<int> waresPrototypesIds;
            int gunAffixGeneratorPrototypeId;

            VehicleDescription(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode);
            void LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode);
            void PostLoad();

            retruxx::vector<CStr> vehiclePrototypeNames;
            retruxx::vector<CStr> waresPrototypesNames;
            CStr gunAffixGeneratorPrototypeName;
        };

        using VehicleDescriptionVector = retruxx::vector<VehicleDescription, retruxx::allocator<VehicleDescription>>;

    public:
        int m_desiredCountLow;
        int m_desiredCountHigh;
        float m_partOfSchwartzForCabin;
        float m_partOfSchwartzForBasket;
        float m_partOfSchwartzForGuns;
        float m_partOfSchwartzForWares;
        retruxx::vector<VehicleDescription, retruxx::allocator<VehicleDescription>> m_vehicleDescriptions;

        VehiclesGeneratorPrototypeInfo();
        virtual Obj* CreateTargetObject() const override;
        virtual bool LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode) override;
        virtual void PostLoad() override;
        void Generate(unsigned int restSchwarz, retruxx::vector<int, retruxx::allocator<int>>& generatedVehicleIds) const;
        void GenerateAndPlace(
            retruxx::vector<CVector, retruxx::allocator<CVector>> const& points,
            unsigned int restSchwarz,
            retruxx::vector<int, retruxx::allocator<int>>& generatedVehicleIds) const;
        void TuneVehicleBySchwarz(Vehicle* v, float desiredSchwarz, VehicleDescription const* vd) const;

    private:
        static int _GetBestPrototypeByMoney(
            retruxx::vector<VehiclesGeneratorInfoCache::VehiclePartInfo, retruxx::allocator<VehiclesGeneratorInfoCache::VehiclePartInfo>> const& partInfos,
            float money);
        static int _GetBestPrototypeByMoneyEconomy(
            retruxx::vector<VehiclesGeneratorInfoCache::VehiclePartInfo, retruxx::allocator<VehiclesGeneratorInfoCache::VehiclePartInfo>> const& partInfos,
            float money);
        static int _GetRandomPrototypeByMoney(
            retruxx::vector<VehiclesGeneratorInfoCache::WareInfo, retruxx::allocator<VehiclesGeneratorInfoCache::WareInfo>> const& wareInfos,
            float money);
        static int _GetRandomPrototypeByMoney(
            retruxx::vector<VehiclesGeneratorInfoCache::VehiclePartInfo, retruxx::allocator<VehiclesGeneratorInfoCache::VehiclePartInfo>> const& partInfos,
            float money);
        static unsigned int _ChangeVehiclePart(Vehicle* v, CStr const& partName, int newPartProtoId, int affixGenetatorProtoId);
    };

    inline VehiclesGeneratorInfoCache* theVehiclesGeneratorInfoCache = nullptr;
}  // namespace ai
