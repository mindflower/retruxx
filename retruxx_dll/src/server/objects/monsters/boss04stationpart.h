#pragma once
#include <server/objects/physicbodies/vehiclepart.h>

namespace ai
{
    class Boss04StationPartPrototypeInfo : public VehiclePartPrototypeInfo
    {
    public:
        Boss04StationPartPrototypeInfo();
        virtual void RefreshFromXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);
        virtual Obj* CreateTargetObject() const;
        virtual bool LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);

    private:
        std::vector<int> m_criticalMeshGroupIds;
        float m_maxHealth;
    };

    class Boss04StationPart :  public VehiclePart
    {
    public:
        class MeshGroupInfo
        {
        public:
            MeshGroupInfo(int, float);

        private:
            int m_groupId;
            float m_health;
        };

    public:
        virtual void GetPropertiesIDs(std::set<int,std::less<int>,std::allocator<int> > &) const ;
        virtual m3d::Class * GetClass() const ;
        virtual int GetPropertyId(char const *) const ;
        virtual CStr GetPropertyName(int) const ;
        virtual eGObjPropertySaveStatus GetPropertySaveStatus(int) const ;
        virtual Boss04StationPartPrototypeInfo const * GetPrototypeInfo() const ;
        Boss04StationPart(Boss04StationPartPrototypeInfo const &);
        float GetHealth() const ;
        virtual void GetPropertiesNames(std::set<CStr,std::less<CStr>,std::allocator<CStr> > &) const ;
        static void __fastcall Registration();
        virtual void Update(float,unsigned int);
        static m3d::Class * GetBaseClass();
        virtual bool SetPropertyById(int,m3d::AIParam const &);

    protected:
        virtual void _InternalPostLoad();
        virtual bool _GetPropertyDefaultInternal(int,m3d::AIParam &) const ;
        virtual ~Boss04StationPart();
        virtual bool _GetPropertyInternal(int,m3d::AIParam &) const ;

    private:
        static m3d::Object * CreateObject();
        virtual m3d::Object * Clone();
        void _UpdateMeshGroupsHealth();

    public:
        RT_CLASS_DECLARE(Boss04StationPart);

    private:
        std::vector<MeshGroupInfo> m_meshGroupInfos;
        std::vector<MeshGroupInfo> m_prevMeshGroupInfos;
    };
}
