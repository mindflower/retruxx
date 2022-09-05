#pragma once
#include "base/simplephysicobj.h"
#include <game/uiwindows/mainwindows/durabilityindicatorwnd.h>
#include <server/components/numericinrangeregenerating.h>

namespace ai
{
    class PhysicUnitPrototypeInfo : public SimplePhysicObjPrototypeInfo
    {
    public:
        virtual bool LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);
        virtual Obj* CreateTargetObject() const;
        PhysicUnitPrototypeInfo();

    private:
        float m_walkSpeed;
        float m_turnSpeed;
        float m_maxStandTime;

    };
    class PhysicUnit : public SimplePhysicObj
    {
    public:
        enum States
        {
            LIVE = 0,
            DEAD = 1,
        };

        enum WalkState
        {
            STAND = 0x0,
            TURN = 0x1,
            WALK = 0x2,
        };

    public:
        void SetCauseForce(float);
        void OnCollideWithStandingVehicle();
        virtual eGObjPropertySaveStatus GetPropertySaveStatus(int) const ;
        PhysicUnit(PhysicUnitPrototypeInfo const &);
        virtual CStr GetPropertyName(int) const ;
        void SetState(States);
        static void __fastcall Registration();
        virtual void GetPropertiesIDs(std::set<int,std::less<int>,std::allocator<int> > &) const ;
        virtual void RenderDebugInfo() const ;
        virtual m3d::Class * GetClass() const ;
        virtual void SaveToXML(m3d::cmn::XmlFile *,m3d::cmn::XmlNode *) const ;
        virtual void SaveRuntimeValues(m3d::cmn::XmlFile *,m3d::cmn::XmlNode *) const ;
        virtual int GetPropertyId(char const *) const ;
        void SetCausePos(CVector const &);
        bool SetWalkPathByName(char const *);
        virtual void SetPositionSelf(CVector const &);
        virtual void LoadFromXML(m3d::cmn::XmlFile *,m3d::cmn::XmlNode const *);
        virtual bool SetPropertyById(int,m3d::AIParam const &);
        virtual void LoadRuntimeValues(m3d::cmn::XmlFile *,m3d::cmn::XmlNode const *);
        virtual PhysicUnitPrototypeInfo const * GetPrototypeInfo() const ;
        bool AddWalkPathByName(char const *);
        static m3d::Class * GetBaseClass();
        void SetInitVelocities(bool);
        virtual void Update(float,unsigned int);
        virtual void SetDirection(CVector const &);
        virtual void GetPropertiesNames(std::set<CStr,std::less<CStr>,std::allocator<CStr> > &) const ;

    protected:
        static void __fastcall RegisterProperty(char const *,int,eGObjPropertySaveStatus);
        virtual bool _GetPropertyInternal(int,m3d::AIParam &) const ;
        virtual ~PhysicUnit();
        virtual bool _GetPropertyDefaultInternal(int,m3d::AIParam &) const ;

    private:
        void _SetWalkState(WalkState);
        virtual m3d::Object * Clone();
        static m3d::Object * CreateObject();

    public:
        RT_CLASS_DECLARE(PhysicUnit);

    private:
        States m_State;
        CVector m_causePos;
        float m_causeForce;
        bool m_initVelocities;
        float m_walkSpeed;
        float m_turnSpeed;
        float m_maxStandTime;
        std::map<CStr,std::vector<CVector>> m_pathsMap;
        const std::vector<CVector> *m_curPath;
        CStr m_curPathName;
        unsigned int m_curWayPointNum;
        CVector m_prevWayPoint;
        WalkState m_walkState;
        //NumericInRangeRegenerating<float> m_standTtl;
        //NumericInRangeRegenerating<float> m_walkTtl;
        bool m_bMustChangePath;
        bool m_bMustWalk;
        std::vector<CVector> m_dummyPath;
    };
}
