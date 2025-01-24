#pragma once
#include <server/objects/physicbodies/vehiclepart.h>

namespace ai
{
    class Boss04StationPartPrototypeInfo : public ai::VehiclePartPrototypeInfo
    {
    public:
        /* 0x0110 */ retruxx::vector<int, retruxx::allocator<int> > m_criticalMeshGroupIds;
        Boss04StationPartPrototypeInfo();
        virtual bool LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override /* 0x04 */;
        virtual void RefreshFromXml(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override /* 0x0c */;
        virtual ai::Obj* CreateTargetObject() const override /* 0x00 */;
        /* 0x0120 */ float m_maxHealth;
    }; /* size: 0x0124 */

    static_assert(sizeof(Boss04StationPartPrototypeInfo) == 0x0124);

    class Boss04StationPart : public ai::VehiclePart
    {
    protected:
        virtual  ~Boss04StationPart() override /* 0x00 */;

    private:
        Boss04StationPart(const ai::Boss04StationPartPrototypeInfo& prototypeInfo);
        Boss04StationPart(const ai::Boss04StationPart&);
        virtual m3d::Object* Clone() override /* 0x00 */;
        static m3d::Object* __fastcall CreateObject();

    public:
        static m3d::Class* __fastcall GetBaseClass();
        virtual m3d::Class* GetClass() const override /* 0x00 */;
        static m3d::Class m_classBoss04StationPart;
        virtual const ai::Boss04StationPartPrototypeInfo* GetPrototypeInfo() const override /* 0x00 */;

    protected:
        static void __fastcall RegisterProperty(const char*, int, ai::eGObjPropertySaveStatus);

    public:
        virtual ai::eGObjPropertySaveStatus GetPropertySaveStatus(int id) const override /* 0x00 */;
        virtual void GetPropertiesNames(retruxx::set<CStr, retruxx::less<CStr>, retruxx::allocator<CStr> >& Props) const override /* 0x00 */;
        virtual void GetPropertiesIDs(retruxx::set<int, retruxx::less<int>, retruxx::allocator<int> >& Props) const override /* 0x00 */;
        virtual CStr GetPropertyName(int id) const override /* 0x00 */;
        virtual bool SetPropertyById(int propertyId, const m3d::AIParam& newValue) override /* 0x00 */;
        virtual int GetPropertyId(const char* PropertyName) const override /* 0x00 */;

    protected:
        static retruxx::map<CStr, int, ai::Obj::LessNoCaseCStr, retruxx::allocator<retruxx::pair<CStr const, int> > > m_propertiesMap;
        static retruxx::map<int, enum ai::eGObjPropertySaveStatus, retruxx::less<int>, retruxx::allocator<retruxx::pair<int const, enum ai::eGObjPropertySaveStatus> > > m_propertiesSaveStatesMap;
        virtual bool _GetPropertyDefaultInternal(int propertyId, m3d::AIParam& retVal) const override /* 0x00 */;
        virtual bool _GetPropertyInternal(int propertyId, m3d::AIParam& retVal) const override /* 0x00 */;

        struct MeshGroupInfo;

    public:
        virtual void Update(float elapsedTime, unsigned int workTime) override /* 0x00 */;
        static void __fastcall Registration();
        float GetHealth() const;

    protected:
        virtual void _InternalPostLoad() override /* 0x00 */;

        using MeshGroupInfoVector = retruxx::vector<ai::Boss04StationPart::MeshGroupInfo, retruxx::allocator<ai::Boss04StationPart::MeshGroupInfo> >;

    private:
        /* 0x02c8 */ retruxx::vector<ai::Boss04StationPart::MeshGroupInfo, retruxx::allocator<ai::Boss04StationPart::MeshGroupInfo> > m_meshGroupInfos;
        /* 0x02d8 */ retruxx::vector<ai::Boss04StationPart::MeshGroupInfo, retruxx::allocator<ai::Boss04StationPart::MeshGroupInfo> > m_prevMeshGroupInfos;
        void _UpdateMeshGroupsHealth();
    }; /* size: 0x02e8 */

    static_assert(sizeof(Boss04StationPart) == 0x02e8);
}
