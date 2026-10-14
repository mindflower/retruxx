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
        virtual bool LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override;
        virtual ai::Obj* CreateTargetObject() const override;
    };

    class Boss04Drone : public ai::ComplexPhysicObj
    {
        friend class Boss04DronePrototypeInfo;

    protected:
        virtual  ~Boss04Drone() override;

    private:
        Boss04Drone(const ai::Boss04DronePrototypeInfo& prototypeInfo);
        Boss04Drone(const ai::Boss04Drone&);
        virtual m3d::Object* Clone() override;
        static m3d::Object* __fastcall CreateObject();

    public:
        static m3d::Class* __fastcall GetBaseClass();
        virtual m3d::Class* GetClass() const override;
        static m3d::Class m_classBoss04Drone;
        virtual const ai::Boss04DronePrototypeInfo* GetPrototypeInfo() const override;

    protected:
        static void __fastcall RegisterProperty(const char*, int, ai::eGObjPropertySaveStatus);

    public:
        virtual ai::eGObjPropertySaveStatus GetPropertySaveStatus(int id) const override;
        virtual void GetPropertiesNames(retruxx::set<CStr, retruxx::less<CStr>, retruxx::allocator<CStr> >& Props) const override;
        virtual void GetPropertiesIDs(retruxx::set<int, retruxx::less<int>, retruxx::allocator<int> >& Props) const override;
        virtual CStr GetPropertyName(int id) const override;
        virtual bool SetPropertyById(int propertyId, const m3d::AIParam& newValue) override;
        virtual int GetPropertyId(const char* PropertyName) const override;

    protected:
        static inline retruxx::map<CStr, int, ai::Obj::LessNoCaseCStr, retruxx::allocator<retruxx::pair<CStr const, int> > > m_propertiesMap;
        static inline retruxx::map<int, enum ai::eGObjPropertySaveStatus, retruxx::less<int>, retruxx::allocator<retruxx::pair<int const, enum ai::eGObjPropertySaveStatus> > > m_propertiesSaveStatesMap;
        virtual bool _GetPropertyDefaultInternal(int propertyId, m3d::AIParam& retVal) const override;
        virtual bool _GetPropertyInternal(int propertyId, m3d::AIParam& retVal) const override;

    public:
        virtual bool CanChildBeAdded(m3d::Class* pClass) const override;
        virtual void LoadRuntimeValues(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override;
        virtual void SaveRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const override;
        virtual void Update(float elapsedTime, unsigned int workTime) override;
        void SetFlyPathName(const CStr& flyPathName);
        virtual void InflictDamage(const ai::DamageInfo& damageInfo) override;
        static void __fastcall Registration();
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
}
