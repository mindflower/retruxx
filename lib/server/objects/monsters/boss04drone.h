#pragma once
#include <server/objects/base/complexphysicobj.h>

namespace m3d
{
    class CameraPath;
}

namespace ai
{
    class Boss04DronePrototypeInfo : public ai::ComplexPhysicObjPrototypeInfo
    {
    public:
        float m_maxLinearVelocity;
        Boss04DronePrototypeInfo();
        virtual bool LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode) override;
        virtual ai::Obj* CreateTargetObject() const override;
    };

    class Boss04Drone : public ai::ComplexPhysicObj
    {
        friend class Boss04DronePrototypeInfo;

    protected:
        virtual ~Boss04Drone() override;

    private:
        Boss04Drone(ai::Boss04DronePrototypeInfo const& prototypeInfo);
        Boss04Drone(ai::Boss04Drone const&);
        virtual m3d::Object* Clone() override;
        static m3d::Object* CreateObject();

    public:
        static m3d::Class* GetBaseClass();
        virtual m3d::Class* GetClass() const override;
        static m3d::Class m_classBoss04Drone;
        virtual ai::Boss04DronePrototypeInfo const* GetPrototypeInfo() const override;

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

    public:
        virtual bool CanChildBeAdded(m3d::Class* pClass) const override;
        virtual void LoadRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode) override;
        virtual void SaveRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const override;
        virtual void Update(float elapsedTime, unsigned int workTime) override;
        void SetFlyPathName(CStr const& flyPathName);
        virtual void InflictDamage(ai::DamageInfo const& damageInfo) override;
        static void Registration();
        float GetMaxHealth() const;
        float GetHealth() const;
        void SetCustomControl(bool bCustomControl);

    private:
        CStr m_flyPathName;
        m3d::CameraPath* m_currentFlyPath;
        float m_currentFlyTime;
        bool m_bCustomControl;
        void _RecalcFlyPath();
        void _UpdateLookAtPlayer();
    };
}  // namespace ai
