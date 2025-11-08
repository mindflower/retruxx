#pragma once
#include <core/stringm3d.h>

#include "vehicle.h"
#include "base/prototypeinfo.h"
#include "retruxx/common.h"

namespace ai
{
    class VehiclesGeneratorInfoCache
    {
    public:
        struct VehiclePartInfo
        {
            /* 0x0000 */ unsigned int price;
            /* 0x0004 */ bool canBeUsedInAutoGenerating;
            /* 0x0005 */ char Padding_320[3];
            /* 0x0008 */ int protoId;
            bool operator<(const ai::VehiclesGeneratorInfoCache::VehiclePartInfo& rhs) const;
        }; /* size: 0x000c */

        using VehiclePartInfoVector = retruxx::vector<VehiclePartInfo, retruxx::allocator<VehiclePartInfo> >;
        
        struct WareInfo
        {
            /* 0x0000 */ unsigned int price;
            /* 0x0004 */ int protoId;
            bool operator<(const ai::VehiclesGeneratorInfoCache::WareInfo& rhs) const;
        }; /* size: 0x0008 */

        using WareInfoVector = retruxx::vector<WareInfo, retruxx::allocator<WareInfo> >;

        struct VehicleGroupInfo
        {
            retruxx::vector<ai::VehiclesGeneratorInfoCache::VehiclePartInfo, retruxx::allocator<ai::VehiclesGeneratorInfoCache::VehiclePartInfo> > cabins;
            retruxx::vector<ai::VehiclesGeneratorInfoCache::VehiclePartInfo, retruxx::allocator<ai::VehiclesGeneratorInfoCache::VehiclePartInfo> > chassises;
            retruxx::vector<ai::VehiclesGeneratorInfoCache::VehiclePartInfo, retruxx::allocator<ai::VehiclesGeneratorInfoCache::VehiclePartInfo> > baskets;
            retruxx::vector<CStr, retruxx::allocator<CStr> > gunPartNames;
        }; /* size: 0x0040 */

        using VehicleGroupInfoMap = retruxx::map<CStr, VehicleGroupInfo, retruxx::less<CStr>, retruxx::allocator<retruxx::pair<CStr const, VehicleGroupInfo> > >;
        using GunsGroupInfoMap = retruxx::map<CStr, retruxx::vector<VehiclePartInfo, retruxx::allocator<VehiclePartInfo> >, retruxx::less<CStr>, retruxx::allocator<retruxx::pair<CStr const, retruxx::vector<VehiclePartInfo, retruxx::allocator<VehiclePartInfo> > > > >;

    public:
        VehiclesGeneratorInfoCache(const VehiclesGeneratorInfoCache&);
        VehiclesGeneratorInfoCache();
        void EnsureInitialized();
        const retruxx::map<CStr, VehicleGroupInfo, retruxx::less<CStr>, retruxx::allocator<retruxx::pair<CStr const, VehicleGroupInfo> > >& GetVehicleGroupInfos() const;
        const retruxx::vector<WareInfo, retruxx::allocator<WareInfo> >& GetWareInfos() const;
        const retruxx::map<CStr, retruxx::vector<VehiclePartInfo, retruxx::allocator<VehiclePartInfo> >, retruxx::less<CStr>, retruxx::allocator<retruxx::pair<CStr const, retruxx::vector<VehiclePartInfo, retruxx::allocator<VehiclePartInfo> > > > >& GetGunInfos() const;

    private:
        /* 0x0000 */ bool m_bInited;
        /* 0x0001 */ char Padding_321[3];
        retruxx::map<CStr, VehicleGroupInfo, retruxx::less<CStr>, retruxx::allocator<retruxx::pair<CStr const, VehicleGroupInfo> > > m_vehicleGroupInfos;
        retruxx::vector<WareInfo, retruxx::allocator<WareInfo> > m_wareInfos;
        retruxx::map<CStr, retruxx::vector<VehiclePartInfo, retruxx::allocator<VehiclePartInfo> >, retruxx::less<CStr>, retruxx::allocator<retruxx::pair<CStr const, retruxx::vector<VehiclePartInfo, retruxx::allocator<VehiclePartInfo> > > > > m_gunInfos;
        VehiclePartInfo _GetVehiclePartInfo(int id) const;
        WareInfo _GetWareInfo(int id) const;
        void _GetVehiclePartInfos(const retruxx::vector<int, retruxx::allocator<int> >& prototypes, retruxx::vector<VehiclePartInfo, retruxx::allocator<VehiclePartInfo> >& vehiclePartInfos);
        void _GetWareInfos(const retruxx::vector<int, retruxx::allocator<int> >& prototypes, retruxx::vector<WareInfo, retruxx::allocator<WareInfo> >& wareInfos);
        void _GetVehiclePartInfosForVehicle(const VehiclePrototypeInfo* protoV, const CStr& partName, retruxx::vector<VehiclePartInfo, retruxx::allocator<VehiclePartInfo> >& vehiclePartInfos);
        void _GetAbstractVehiclesPrototypeIds(retruxx::vector<int, retruxx::allocator<int> >& abstractVehiclesPrototypeIds);
        void _InitializeVehicleParts();
        void _InitializeWares();
        void _InitializeGuns();
        void _Initialize();
    }; /* size: 0x002c */

    class VehiclesGeneratorPrototypeInfo : public PrototypeInfo
    {
    public:
        struct VehicleDescription
        {
            /* 0x0000 */ bool bTuningBySchwartz;
            /* 0x0001 */ char Padding_177[3];
            /* 0x0004 */ float partOfSchwartz;
            retruxx::vector<int, retruxx::allocator<int> > vehiclePrototypeIds;
            retruxx::vector<int, retruxx::allocator<int> > waresPrototypesIds;
            /* 0x0028 */ int gunAffixGeneratorPrototypeId;
            VehicleDescription(const ai::VehiclesGeneratorPrototypeInfo::VehicleDescription& __that);
            VehicleDescription(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode);
            void LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode);
            void PostLoad();
            retruxx::vector<CStr, retruxx::allocator<CStr> > vehiclePrototypeNames;
            retruxx::vector<CStr, retruxx::allocator<CStr> > waresPrototypesNames;
            CStr gunAffixGeneratorPrototypeName;
        }; /* size: 0x0058 */

        using VehicleDescriptionVector = retruxx::vector<VehicleDescription, retruxx::allocator<VehicleDescription> >;

    public:
        /* 0x0040 */ int m_desiredCountLow;
        /* 0x0044 */ int m_desiredCountHigh;
        /* 0x0048 */ float m_partOfSchwartzForCabin;
        /* 0x004c */ float m_partOfSchwartzForBasket;
        /* 0x0050 */ float m_partOfSchwartzForGuns;
        /* 0x0054 */ float m_partOfSchwartzForWares;

        retruxx::vector<VehicleDescription, retruxx::allocator<VehicleDescription> > m_vehicleDescriptions;
        VehiclesGeneratorPrototypeInfo();
        virtual Obj* CreateTargetObject() const override /* 0x10 */;
        virtual bool LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override /* 0x04 */;
        virtual void PostLoad() override /* 0x08 */;
        void Generate(unsigned int restSchwarz, retruxx::vector<int, retruxx::allocator<int> >& generatedVehicleIds) const;
        void GenerateAndPlace(const retruxx::vector<CVector, retruxx::allocator<CVector> >& points, unsigned int restSchwarz, retruxx::vector<int, retruxx::allocator<int> >& generatedVehicleIds) const;
        void TuneVehicleBySchwarz(Vehicle* v, float desiredSchwarz, const VehicleDescription* vd) const;

    private:
        static int __fastcall _GetBestPrototypeByMoney(const retruxx::vector<VehiclesGeneratorInfoCache::VehiclePartInfo, retruxx::allocator<VehiclesGeneratorInfoCache::VehiclePartInfo> >& partInfos, float money);
        static int __fastcall _GetBestPrototypeByMoneyEconomy(const retruxx::vector<VehiclesGeneratorInfoCache::VehiclePartInfo, retruxx::allocator<VehiclesGeneratorInfoCache::VehiclePartInfo> >& partInfos, float money);
        static int __fastcall _GetRandomPrototypeByMoney(const retruxx::vector<VehiclesGeneratorInfoCache::WareInfo, retruxx::allocator<VehiclesGeneratorInfoCache::WareInfo> >& wareInfos, float money);
        static int __fastcall _GetRandomPrototypeByMoney(const retruxx::vector<VehiclesGeneratorInfoCache::VehiclePartInfo, retruxx::allocator<VehiclesGeneratorInfoCache::VehiclePartInfo> >& partInfos, float money);
        static unsigned int __fastcall _ChangeVehiclePart(Vehicle* v, const CStr& partName, int newPartProtoId, int affixGenetatorProtoId);
    }; /* size: 0x0068 */

    inline VehiclesGeneratorInfoCache* theVehiclesGeneratorInfoCache = nullptr;
}
