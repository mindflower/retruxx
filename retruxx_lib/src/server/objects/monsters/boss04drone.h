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
        /* 0x0090 */ float m_maxLinearVelocity;
        Boss04DronePrototypeInfo();
        virtual bool LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override /* 0x04 */;
        virtual ai::Obj* CreateTargetObject() const override /* 0x00 */;
    }; /* size: 0x0094 */

    static_assert(sizeof(Boss04DronePrototypeInfo) == 0x0094);

    class Boss04Drone : public ai::ComplexPhysicObj
    {
    protected:
        virtual  ~Boss04Drone() override /* 0x00 */;

    private:
        Boss04Drone(const ai::Boss04DronePrototypeInfo& prototypeInfo);
        Boss04Drone(const ai::Boss04Drone&);
        virtual m3d::Object* Clone() override /* 0x00 */;
        static m3d::Object* __fastcall CreateObject();

    public:
        static m3d::Class* __fastcall GetBaseClass();
        virtual m3d::Class* GetClass() const override /* 0x00 */;
        static m3d::Class m_classBoss04Drone;
        virtual const ai::Boss04DronePrototypeInfo* GetPrototypeInfo() const override /* 0x4c */;

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

    public:
        virtual bool CanChildBeAdded(m3d::Class* pClass) const override /* 0x00 */;
        virtual void LoadRuntimeValues(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override /* 0xb0 */;
        virtual void SaveRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const override /* 0xb8 */;
        virtual void Update(float elapsedTime, unsigned int workTime) override /* 0x00 */;
        void SetFlyPathName(const CStr& flyPathName);
        virtual void InflictDamage(const ai::DamageInfo& damageInfo) override /* 0x00 */;
        static void __fastcall Registration();
        float GetMaxHealth() const;
        float GetHealth() const;
        void SetCustomControl(bool bCustomControl);

    private:
        /* 0x014c */ CStr m_flyPathName;
        /* 0x0158 */ m3d::CameraPath* m_currentFlyPath;
        /* 0x015c */ float m_currentFlyTime;
        /* 0x0160 */ bool m_bCustomControl;
        void _RecalcFlyPath();
        void _UpdateLookAtPlayer();
    }; /* size: 0x0164 */

    static_assert(sizeof(Boss04Drone) == 0x0164);
}
