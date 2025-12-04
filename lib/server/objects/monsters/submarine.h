#pragma once
#include <server/objects/dummyobject.h>

#include "server/cinematicpath.h"

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

    class SubmarinePrototypeInfo : public ai::DummyObjectPrototypeInfo
    {
    public:
        SubmarinePrototypeInfo();
        virtual  ~SubmarinePrototypeInfo() override /* 0x00 */;
        /* 0x0084 */ float m_maxLinearVelocity;
        /* 0x0088 */ float m_linearAcceleration;
        /* 0x008c */ int m_platformOpenFps;
        /* 0x0090 */ float m_vehicleMaxSpeed;
        /* 0x0094 */ CVector m_vehicleRelativePosition;
        virtual bool LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override /* 0x04 */;
        virtual void RefreshFromXml(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override /* 0x00 */;
        virtual ai::Obj* CreateTargetObject() const override /* 0x00 */;
    }; /* size: 0x00a0 */

    static_assert(sizeof(SubmarinePrototypeInfo) == 0x00a0);

    class Submarine : public ai::DummyObject
    {
    protected:
        virtual  ~Submarine() override /* 0x00 */;

    private:
        Submarine(const ai::SubmarinePrototypeInfo& prototypeInfo);
        Submarine(const ai::Submarine&);
        virtual m3d::Object* Clone() override /* 0x00 */;
        static m3d::Object* __fastcall CreateObject();

    public:
        static m3d::Class* __fastcall GetBaseClass();
        virtual m3d::Class* GetClass() const override /* 0x00 */;
        static m3d::Class m_classSubmarine;
        virtual const ai::SubmarinePrototypeInfo* GetPrototypeInfo() const override /* 0x00 */;

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
        static inline retruxx::map<CStr, int, ai::Obj::LessNoCaseCStr, retruxx::allocator<retruxx::pair<CStr const, int> > > m_propertiesMap;
        static inline retruxx::map<int, enum ai::eGObjPropertySaveStatus, retruxx::less<int>, retruxx::allocator<retruxx::pair<int const, enum ai::eGObjPropertySaveStatus> > > m_propertiesSaveStatesMap;
        virtual bool _GetPropertyDefaultInternal(int propertyId, m3d::AIParam& retVal) const override /* 0x00 */;
        virtual bool _GetPropertyInternal(int propertyId, m3d::AIParam& retVal) const override /* 0x00 */;

    public:
        virtual int OnEvent(const ai::Event& evn) override /* 0x00 */;
        virtual void LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override /* 0x00 */;
        virtual void SaveToXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const override /* 0x00 */;
        virtual void _InternalPostLoad() override /* 0x00 */;
        virtual void Update(float elapsedTime, unsigned int workTime) override /* 0x00 */;
        void InitPosition();
        void StartMotionToPort();
        virtual void RenderDebugInfo() const override /* 0x00 */;

    private:
        /* 0x0150 */ ai::eSubmarineState m_state;
        /* 0x0154 */ CVector m_placePosition;
        /* 0x0160 */ CVector m_portPosition;
        /* 0x016c */ CVector m_moveDirection;
        /* 0x0178 */ CVector m_placeToMove;
        /* 0x0184 */ CVector m_linearVelocity;
        /* 0x0190 */ ai::CinematicPath m_entryPath;
        /* 0x01b0 */ CStr m_nextMap;
        /* 0x01bc */ CStr m_nextMapLocation;
        /* 0x01c8 */ int m_nextMapAngle;
        void _UpdatePositionOnMove(float elapsedTime);
        void _WatchPlatformRotation();
        void _SetSubmarineState(ai::eSubmarineState newState);
        void _FillCameraStates(retruxx::vector<m3d::CameraPathState, retruxx::allocator<m3d::CameraPathState> >& cameraStates, const CVector& offset) const;
        void _OnTargetReached(const ai::Event& evn);
        void _OnSkipCinematic(const ai::Event& evn);

    public:
        static void __fastcall Registration();
    }; /* size: 0x01cc */

    static_assert(sizeof(Submarine) == 0x01cc);
}
