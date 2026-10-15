#pragma once
#include <server/objects/physicbodies/vehiclepart.h>

namespace ai
{
    class Boss04StationPartPrototypeInfo : public ai::VehiclePartPrototypeInfo
    {
    public:
        retruxx::vector<int, retruxx::allocator<int>> m_criticalMeshGroupIds;
        Boss04StationPartPrototypeInfo();
        virtual bool LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode) override;
        virtual void RefreshFromXml(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode) override;
        virtual ai::Obj* CreateTargetObject() const override;
        float m_maxHealth;
    };

    class Boss04StationPart : public ai::VehiclePart
    {
        friend class Boss04StationPartPrototypeInfo;

    protected:
        virtual ~Boss04StationPart() override;

    private:
        Boss04StationPart(ai::Boss04StationPartPrototypeInfo const& prototypeInfo);
        Boss04StationPart(ai::Boss04StationPart const&);
        virtual m3d::Object* Clone() override;
        static m3d::Object* CreateObject();

    public:
        static m3d::Class* GetBaseClass();
        virtual m3d::Class* GetClass() const override;
        static m3d::Class m_classBoss04StationPart;
        virtual ai::Boss04StationPartPrototypeInfo const* GetPrototypeInfo() const override;

    protected:
        static void RegisterProperty(char const*, int, ai::eGObjPropertySaveStatus);

    public:
        virtual ai::eGObjPropertySaveStatus GetPropertySaveStatus(int id) const override;
        virtual void GetPropertiesNames(
            retruxx::set<CStr, retruxx::less<CStr>, retruxx::allocator<CStr>>& Props) const override;
        virtual void GetPropertiesIDs(
            retruxx::set<int, retruxx::less<int>, retruxx::allocator<int>>& Props) const override;
        virtual CStr GetPropertyName(int id) const override;
        virtual bool SetPropertyById(int propertyId, m3d::AIParam const& newValue) override;
        virtual int GetPropertyId(char const* PropertyName) const override;

    protected:
        static inline retruxx::
            map<CStr, int, ai::Obj::LessNoCaseCStr, retruxx::allocator<retruxx::pair<CStr const, int>>>
                m_propertiesMap;
        static inline retruxx::map<
            int,
            enum ai::eGObjPropertySaveStatus,
            retruxx::less<int>,
            retruxx::allocator<retruxx::pair<int const, enum ai::eGObjPropertySaveStatus>>>
            m_propertiesSaveStatesMap;
        virtual bool _GetPropertyDefaultInternal(int propertyId, m3d::AIParam& retVal) const override;
        virtual bool _GetPropertyInternal(int propertyId, m3d::AIParam& retVal) const override;

        struct MeshGroupInfo
        {
            MeshGroupInfo(int groupId, float health);
            int m_groupId;
            float m_health;
        };

    public:
        virtual void Update(float elapsedTime, unsigned int workTime) override;
        static void Registration();
        float GetHealth() const;

    protected:
        virtual void _InternalPostLoad() override;

        using MeshGroupInfoVector = retruxx::
            vector<ai::Boss04StationPart::MeshGroupInfo, retruxx::allocator<ai::Boss04StationPart::MeshGroupInfo>>;

    private:
        retruxx::vector<ai::Boss04StationPart::MeshGroupInfo, retruxx::allocator<ai::Boss04StationPart::MeshGroupInfo>>
            m_meshGroupInfos;
        retruxx::vector<ai::Boss04StationPart::MeshGroupInfo, retruxx::allocator<ai::Boss04StationPart::MeshGroupInfo>>
            m_prevMeshGroupInfos;
        void _UpdateMeshGroupsHealth();
    };
}  // namespace ai
