#pragma once
#include <server/objects/dummyobject.h>

namespace m3d
{
    class CameraPathState;
}

namespace ai
{
    enum eSubmarineState
    {
        SUBMARINE_IDLE = 0x0,
        SUBMARINE_MOVES = 0x1,
        SUBMARINE_OPENS = 0x2,
        SUBMARINE_WAITS = 0x3,
    };

    class SubmarinePrototypeInfo : public DummyObjectPrototypeInfo
    {
    public:
        virtual void RefreshFromXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);
        SubmarinePrototypeInfo();
        virtual Obj* CreateTargetObject() const;
        virtual bool LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);

    private:
        float m_maxLinearVelocity;
        float m_linearAcceleration;
        int m_platformOpenFps;
        float m_vehicleMaxSpeed;
        CVector m_vehicleRelativePosition;
    };

    class Submarine : public DummyObject
    {
    public:
        virtual void GetPropertiesIDs(std::set<int,std::less<int>,std::allocator<int> > &) const ;
        virtual void _InternalPostLoad();
        virtual int GetPropertyId(char const *) const ;
        virtual eGObjPropertySaveStatus GetPropertySaveStatus(int) const ;
        virtual CStr GetPropertyName(int) const ;
        virtual SubmarinePrototypeInfo const * GetPrototypeInfo() const ;
        virtual void SaveToXML(m3d::cmn::XmlFile *,m3d::cmn::XmlNode *) const ;
        virtual m3d::Class * GetClass() const ;
        virtual int OnEvent(Event const &);
        virtual void LoadFromXML(m3d::cmn::XmlFile *,m3d::cmn::XmlNode const *);
        virtual void RenderDebugInfo() const ;
        virtual void GetPropertiesNames(std::set<CStr,std::less<CStr>,std::allocator<CStr> > &) const ;
        static void __fastcall Registration();
        virtual void Update(float,unsigned int);
        Submarine(SubmarinePrototypeInfo const &);
        void InitPosition();
        void StartMotionToPort();
        virtual bool SetPropertyById(int,m3d::AIParam const &);
        static m3d::Class * GetBaseClass();

    protected:
        static void __fastcall RegisterProperty(char const *,int,eGObjPropertySaveStatus);
        virtual bool _GetPropertyDefaultInternal(int,m3d::AIParam &) const ;
        virtual ~Submarine();
        virtual bool _GetPropertyInternal(int,m3d::AIParam &) const ;

    private:
        void _OnSkipCinematic(Event const &);
        void _UpdatePositionOnMove(float);
        void _SetSubmarineState(eSubmarineState);
        static m3d::Object * CreateObject();
        virtual m3d::Object * Clone();
        void _WatchPlatformRotation();
        void _FillCameraStates(std::vector<m3d::CameraPathState,std::allocator<m3d::CameraPathState> > &,CVector const &) const ;
        void _OnTargetReached(Event const &);

    public:
        RT_CLASS_INLINE_DECLARE(Submarine);

    private:
        eSubmarineState m_state;
        CVector m_placePosition;
        CVector m_portPosition;
        CVector m_moveDirection;
        CVector m_placeToMove;
        CVector m_linearVelocity;
        CinematicPath m_entryPath;
        CStr m_nextMap;
        CStr m_nextMapLocation;
        int m_nextMapAngle;
    };
}
