#pragma once
#include "base/simplephysicobj.h"
#include <game/uiwindows/mainwindows/durabilityindicatorwnd.h>
#include <server/components/numericinrangeregenerating.h>

namespace ai
{
    class PhysicUnitPrototypeInfo : public ai::SimplePhysicObjPrototypeInfo
    {
    public:
        /* 0x0080 */ float m_walkSpeed;
        /* 0x0084 */ float m_turnSpeed;
        /* 0x0088 */ float m_maxStandTime;
        PhysicUnitPrototypeInfo();
        virtual bool LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override /* 0x04 */;
        virtual ai::Obj* CreateTargetObject() const override /* 0x00 */;
    }; /* size: 0x008c */

    static_assert(sizeof(PhysicUnitPrototypeInfo) == 0x008c);

    class PhysicUnit : public ai::SimplePhysicObj
    {
    protected:
        virtual  ~PhysicUnit() override /* 0x00 */;

    private:
        PhysicUnit(const ai::PhysicUnitPrototypeInfo& prototypeInfo);
        PhysicUnit(const ai::PhysicUnit&);
        virtual m3d::Object* Clone() override /* 0x00 */;
        static m3d::Object* __fastcall CreateObject();

    public:
        static m3d::Class* __fastcall GetBaseClass();
        virtual m3d::Class* GetClass() const override /* 0x00 */;
        static m3d::Class m_classPhysicUnit;
        virtual const ai::PhysicUnitPrototypeInfo* GetPrototypeInfo() const override /* 0x00 */;

    protected:
        static void __fastcall RegisterProperty(const char* Name, int id, ai::eGObjPropertySaveStatus saveStatus);

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
        static void __fastcall Registration();

        enum States;
        enum WalkState;

    public:
        virtual void LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override /* 0x00 */;
        virtual void LoadRuntimeValues(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override /* 0x00 */;
        virtual void SaveToXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const override /* 0x00 */;
        virtual void SaveRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const override /* 0x00 */;
        void SetState(ai::PhysicUnit::States newState);
        virtual void Update(float elapsedTime, unsigned int workTime) override /* 0x00 */;
        virtual void SetPositionSelf(const CVector& pos) override /* 0x00 */;
        virtual void SetDirection(const CVector& direction) override /* 0x00 */;
        virtual void RenderDebugInfo() const override /* 0x00 */;
        bool SetWalkPathByName(const char* pathName);
        bool AddWalkPathByName(const char* pathName);
        void SetCausePos(const CVector& pos);
        void SetCauseForce(float force);
        void SetInitVelocities(bool initVelocities);
        void OnCollideWithStandingVehicle();

    private:
        /* 0x0144 */ ai::PhysicUnit::States m_State;
        /* 0x0148 */ CVector m_causePos;
        /* 0x0154 */ float m_causeForce;
        /* 0x0158 */ bool m_initVelocities;
        /* 0x0159 */ char Padding_373[3];
        /* 0x015c */ float m_walkSpeed;
        /* 0x0160 */ float m_turnSpeed;
        /* 0x0164 */ float m_maxStandTime;
        /* 0x0168 */ retruxx::map<CStr, retruxx::vector<CVector, retruxx::allocator<CVector> >, retruxx::less<CStr>, retruxx::allocator<retruxx::pair<CStr const, retruxx::vector<CVector, retruxx::allocator<CVector> > > > > m_pathsMap;
        /* 0x0174 */ const retruxx::vector<CVector, retruxx::allocator<CVector> >* m_curPath;
        /* 0x0178 */ CStr m_curPathName;
        /* 0x0184 */ unsigned int m_curWayPointNum;
        /* 0x0188 */ CVector m_prevWayPoint;
        /* 0x0194 */ ai::PhysicUnit::WalkState m_walkState;
        /* 0x0198 */ ai::NumericInRangeRegenerating<float> m_standTtl;
        /* 0x0270 */ ai::NumericInRangeRegenerating<float> m_walkTtl;
        /* 0x0348 */ bool m_bMustChangePath;
        /* 0x0349 */ bool m_bMustWalk;
        /* 0x034a */ char Padding_374[2];
        /* 0x034c */ retruxx::vector<CVector, retruxx::allocator<CVector> > m_dummyPath;
        void _SetWalkState(ai::PhysicUnit::WalkState newWalkState);
    }; /* size: 0x035c */

    static_assert(sizeof(PhysicUnit) == 0x035c);
}
