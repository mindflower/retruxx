#pragma once
#include <server/components/numericinrange.h>
#include <server/objects/base/animatedcomplexphysicobj.h>

namespace m3d
{
    class CameraPath;
}

namespace ai
{
    class Team;
    class Boss03Part;

    class Boss03PrototypeInfo : public AnimatedComplexPhysicObjPrototypeInfo
    {
    public:
        virtual bool LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);
        Boss03PrototypeInfo();
        virtual void PostLoad();
        virtual void RefreshFromXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);
        virtual Obj* CreateTargetObject() const;

    private:
        std::vector<int> m_dronePrototypeIds;
        int m_maxDrones;
        CVector m_droneRelPosition;
        Quaternion m_droneRelRotation;
        float m_maxHealth;
        float m_maxHorizAngularVelocity;
        float m_horizAngularAcceleration;
        float m_maxVertAngularVelocity;
        float m_vertAngularAcceleration;
        float m_maxLinearVelocity;
        float m_linearAcceleration;
        float m_pathTrackTiltAngle;
        float m_maxShootingTime;
        float m_defaultHover;
        float m_hoverForPlacingDrones;
        std::vector<CStr> m_dronePrototypeNames;
    };

    class Boss03 :  public AnimatedComplexPhysicObj
    {
    public:
        enum LiveStatus
        {
            LIVE_PLACING_DRONES = 0x0,
            LIVE_FIGHTING_WITH_WINGS = 0x1,
            LIVE_SMALL_HELICOPTER = 0x2,
            LIVE_DEAD = 0x3,
            LIVE_IDLE = 0x4,
            LIVE_LAST = 0x5,
        };

        enum DroneSpawningStatus
        {
            DRONE_DECIDING = 0x0,
            DRONE_FLYING_TO_SPAWN = 0x1,
            DRONE_START_PLACING = 0x2,
            DRONE_PLACING_NOW = 0x3,
            DRONE_END_PLACING = 0x4,
            DRONE_FLYING_TO_SHOOT = 0x5,
            DRONE_SHOOTING = 0x6,
            DRONE_LAST = 0x7,
        };

        enum PathTrackingStatus
        {
            PATH_IDLE = 0x0,
            PATH_SETTING_HORIZONTAL_BEFORE = 0x1,
            PATH_SETTING_DIRECTION_TO_MOVE = 0x2,
            PATH_TILTING = 0x3,
            PATH_MOVING = 0x4,
            PATH_SETTING_HORIZONTAL_AFTER = 0x5,
            PATH_SETTING_FINAL_DIRECTION = 0x6,
        };

    public:
        virtual void SaveToXML(m3d::cmn::XmlFile *,m3d::cmn::XmlNode *) const ;
        virtual void SaveRuntimeValues(m3d::cmn::XmlFile *,m3d::cmn::XmlNode *) const ;
        virtual void CreateChildren();
        static void __fastcall Registration();
        Boss03(Boss03PrototypeInfo const &);
        virtual void InflictDamage(DamageInfo const &);
        virtual void GetPropertiesIDs(std::set<int,std::less<int>,std::allocator<int> > &) const ;
        bool StartPlacingDrone();
        virtual m3d::Class * GetClass() const ;
        virtual CStr GetPropertyName(int) const ;
        virtual bool CanChildBeAdded(m3d::Class *) const ;
        void StartMoving(CVector const &,CVector const &);
        virtual eGObjPropertySaveStatus GetPropertySaveStatus(int) const ;
        virtual void GetPropertiesNames(std::set<CStr,std::less<CStr>,std::allocator<CStr> > &) const ;
        static m3d::Class * GetBaseClass();
        virtual void Update(float,unsigned int);
        virtual bool ApplyModifier(Modifier const &);
        void NoticePlayer();
        virtual bool SetPropertyById(int,m3d::AIParam const &);
        virtual void LoadRuntimeValues(m3d::cmn::XmlFile *,m3d::cmn::XmlNode const *);
        float GetHealth() const ;
        virtual void AddChild(Obj *);
        virtual Boss03PrototypeInfo const * GetPrototypeInfo() const ;
        virtual bool RemoveChild(Obj *);
        virtual int GetPropertyId(char const *) const ;
        virtual void LoadFromXML(m3d::cmn::XmlFile *,m3d::cmn::XmlNode const *);
        float GetMaxHealth() const ;

    protected:
        virtual void _InternalPostLoad();
        virtual bool _GetPropertyDefaultInternal(int,m3d::AIParam &) const ;
        virtual void _Construct(bool);
        virtual bool _GetPropertyInternal(int,m3d::AIParam &) const ;
        virtual void _InternalCreateVisualPart();
        virtual ~Boss03();

    private:
        void _SetAllPartsNotDamageable();
        void _UpdateLiveFightingWithWings(float);
        void _SetDroneSpawningStatus(DroneSpawningStatus);
        Boss03Part * _GetBossPartByName(CStr const &);
        bool _OnHealthValueBeforeApplyModifier(Modifier const &,float &);
        //MemberFunctionTwoArgsRef<Boss03,Modifier,float,bool>::MemberFunctionTwoArgsRef<Boss03,Modifier,float,bool>(Boss03 &,bool (*const)(Modifier const &,float &));
        bool _ProceedSettingVerticalAngle(float,float);
        void _SpawnDroneNow();
        void _UpdatePathTracking(float);
        Boss03Part * _GetMainPart();
        Boss03Part * _GetDroneSpawner();
        void _SetLiveStatus(LiveStatus);
        void _RecalcFlyPath();
        void _UpdateLookAtPlayer(float);
        void _UpdateLivePlacingDrones(float);
        void _UpdateSwinging(float);
        bool _ProceedSettingHorizontalDirection(float,CVector const &);
        static CStr const & __fastcall _DroneSpawningStatusToStr(DroneSpawningStatus);
        static m3d::Object * CreateObject();
        virtual m3d::Object * Clone();
        static CStr const & __fastcall _LiveStatusToStr(LiveStatus);
        void _SetPathTrackingStatus(PathTrackingStatus);
        float CalcKeyPartsMaxDurability() const ;

    public:
        RT_CLASS_INLINE_DECLARE(Boss03);

    private:
        NumericInRange<float> m_health;
        CVector m_linearVelocity;
        CVector m_relAngularVelocity;
        std::vector<CVector> m_pointsForDrones;
        std::vector<CVector> m_pointsForShooting;
        Boss03::LiveStatus m_liveStatus;
        Boss03::DroneSpawningStatus m_droneSpawningStatus;
        float m_dronePlacingTimeout;
        float m_shootingTimeout;
        Boss03::PathTrackingStatus m_pathTrackingStatus;
        CVector m_desiredDestination;
        CVector m_desiredDirection;
        Team *m_droneTeam;
        CStr m_pathNameForFlyingWithWings;
        m3d::CameraPath *m_currentFlyPath;
        float m_currentFlyTime;
        float m_keyPartsMaxDurability;
    };
}
