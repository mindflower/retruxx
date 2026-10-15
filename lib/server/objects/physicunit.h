#pragma once
#include "base/simplephysicobj.h"
#include "server/colliders/physicunitcolliders.h"
#include <game/uiwindows/mainwindows/durabilityindicatorwnd.h>
#include <server/components/numericinrangeregenerating.h>

namespace ai
{
    class PhysicUnitPrototypeInfo : public ai::SimplePhysicObjPrototypeInfo
    {
    public:
        float m_walkSpeed;
        float m_turnSpeed;
        float m_maxStandTime;
        PhysicUnitPrototypeInfo();
        virtual bool LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode) override;
        virtual ai::Obj* CreateTargetObject() const override;
    };

    class PhysicUnit : public ai::SimplePhysicObj
    {
        friend class PhysicUnitPrototypeInfo;
        // The colliders set the ragdoll cause and kill the unit directly.
        friend int CollidePhysicUnitAndVehicle(m3d::Object*, m3d::Object*, dContact*, unsigned int&, bool);
        friend int CollidePhysicUnitAndShell(m3d::Object*, m3d::Object*, dContact*, unsigned int&, bool);
        friend int CollidePhysicUnitAndBlastWave(m3d::Object*, m3d::Object*, dContact*, unsigned int&, bool);

    protected:
        virtual ~PhysicUnit() override;

    private:
        PhysicUnit(ai::PhysicUnitPrototypeInfo const& prototypeInfo);
        PhysicUnit(ai::PhysicUnit const&);
        virtual m3d::Object* Clone() override;
        static m3d::Object* CreateObject();

    public:
        static m3d::Class* GetBaseClass();
        virtual m3d::Class* GetClass() const override;
        static m3d::Class m_classPhysicUnit;
        virtual ai::PhysicUnitPrototypeInfo const* GetPrototypeInfo() const override;

    protected:
        static void RegisterProperty(char const* Name, int id, ai::eGObjPropertySaveStatus saveStatus);

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
        static void Registration();

        enum States
        {
            LIVE = 0,
            DEAD = 1,
        };

        enum WalkState
        {
            STAND = 0,
            TURN = 1,
            WALK = 2,
        };

    public:
        virtual void LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode) override;
        virtual void LoadRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode) override;
        virtual void SaveToXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const override;
        virtual void SaveRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const override;
        void SetState(ai::PhysicUnit::States newState);
        virtual void Update(float elapsedTime, unsigned int workTime) override;
        virtual void SetPositionSelf(CVector const& pos) override;
        virtual void SetDirection(CVector const& direction) override;
        virtual void RenderDebugInfo() const override;
        bool SetWalkPathByName(char const* pathName);
        bool AddWalkPathByName(char const* pathName);
        void SetCausePos(CVector const& pos);
        void SetCauseForce(float force);
        void SetInitVelocities(bool initVelocities);
        void OnCollideWithStandingVehicle();

    private:
        ai::PhysicUnit::States m_State;
        CVector m_causePos;
        float m_causeForce;
        bool m_initVelocities;
        float m_walkSpeed;
        float m_turnSpeed;
        float m_maxStandTime;
        retruxx::map<
            CStr,
            retruxx::vector<CVector, retruxx::allocator<CVector>>,
            retruxx::less<CStr>,
            retruxx::allocator<retruxx::pair<CStr const, retruxx::vector<CVector, retruxx::allocator<CVector>>>>>
            m_pathsMap;
        retruxx::vector<CVector, retruxx::allocator<CVector>> const* m_curPath;
        CStr m_curPathName;
        unsigned int m_curWayPointNum;
        CVector m_prevWayPoint;
        ai::PhysicUnit::WalkState m_walkState;
        ai::NumericInRangeRegenerating<float> m_standTtl;
        ai::NumericInRangeRegenerating<float> m_walkTtl;
        bool m_bMustChangePath;
        bool m_bMustWalk;
        retruxx::vector<CVector, retruxx::allocator<CVector>> m_dummyPath;
        void _SetWalkState(ai::PhysicUnit::WalkState newWalkState);
    };
}  // namespace ai
