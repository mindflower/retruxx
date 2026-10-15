#pragma once
#include <server/objects/dummyobject.h>

#include "server/cinematicpath.h"

namespace m3d
{
    struct CameraPathState;
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
        virtual ~SubmarinePrototypeInfo() override;
        float m_maxLinearVelocity;
        float m_linearAcceleration;
        int m_platformOpenFps;
        float m_vehicleMaxSpeed;
        CVector m_vehicleRelativePosition;
        virtual bool LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode) override;
        virtual void RefreshFromXml(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode) override;
        virtual ai::Obj* CreateTargetObject() const override;
    };

    class Submarine : public ai::DummyObject
    {
        friend class SubmarinePrototypeInfo;

    protected:
        virtual ~Submarine() override;

    private:
        Submarine(ai::SubmarinePrototypeInfo const& prototypeInfo);
        Submarine(ai::Submarine const&);
        virtual m3d::Object* Clone() override;
        static m3d::Object* CreateObject();

    public:
        static m3d::Class* GetBaseClass();
        virtual m3d::Class* GetClass() const override;
        static m3d::Class m_classSubmarine;
        virtual ai::SubmarinePrototypeInfo const* GetPrototypeInfo() const override;

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
        virtual int OnEvent(ai::Event const& evn) override;
        virtual void LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode) override;
        virtual void SaveToXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const override;
        virtual void _InternalPostLoad() override;
        virtual void Update(float elapsedTime, unsigned int workTime) override;
        void InitPosition();
        void StartMotionToPort();
        virtual void RenderDebugInfo() const override;

    private:
        ai::eSubmarineState m_state;
        CVector m_placePosition;
        CVector m_portPosition;
        CVector m_moveDirection;
        CVector m_placeToMove;
        CVector m_linearVelocity;
        ai::CinematicPath m_entryPath;
        CStr m_nextMap;
        CStr m_nextMapLocation;
        int m_nextMapAngle;
        void _UpdatePositionOnMove(float elapsedTime);
        void _WatchPlatformRotation();
        void _SetSubmarineState(ai::eSubmarineState newState);
        void _FillCameraStates(
            retruxx::vector<m3d::CameraPathState, retruxx::allocator<m3d::CameraPathState>>& cameraStates,
            CVector const& offset) const;
        void _OnTargetReached(ai::Event const& evn);
        void _OnSkipCinematic(ai::Event const& evn);

    public:
        static void Registration();
    };
}  // namespace ai
