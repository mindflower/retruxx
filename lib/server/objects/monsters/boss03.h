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

    class Boss03PrototypeInfo : public ai::AnimatedComplexPhysicObjPrototypeInfo
    {
    public:
        retruxx::vector<int, retruxx::allocator<int>> m_dronePrototypeIds;
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
        Boss03PrototypeInfo();
        virtual bool LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode) override;
        virtual void PostLoad() override;
        virtual void RefreshFromXml(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode) override;
        virtual ai::Obj* CreateTargetObject() const override;

    private:
        retruxx::vector<CStr, retruxx::allocator<CStr>> m_dronePrototypeNames;
    };

    class Boss03 : public ai::AnimatedComplexPhysicObj
    {
        friend class Boss03PrototypeInfo;

    public:
        using AfterChangeFloatCallback = ai::MemberFunctionOneArg<ai::Boss03, float, void>;
        using BeforeApplyModifierFloatCallback = ai::MemberFunctionTwoArgsRef<ai::Boss03, ai::Modifier, float, bool>;

    protected:
        virtual ~Boss03() override;

    private:
        Boss03(ai::Boss03PrototypeInfo const& prototypeInfo);
        Boss03(ai::Boss03 const&);
        virtual m3d::Object* Clone() override;
        static m3d::Object* CreateObject();

    public:
        static m3d::Class* GetBaseClass();
        virtual m3d::Class* GetClass() const override;
        static m3d::Class m_classBoss03;
        virtual ai::Boss03PrototypeInfo const* GetPrototypeInfo() const override;

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

        enum LiveStatus
        {
            LIVE_PLACING_DRONES = 0,
            LIVE_FIGHTING_WITH_WINGS = 1,
            LIVE_SMALL_HELICOPTER = 2,
            LIVE_DEAD = 3,
            LIVE_IDLE = 4,
            LIVE_LAST = 5,
        };
        enum DroneSpawningStatus
        {
            DRONE_DECIDING = 0,
            DRONE_FLYING_TO_SPAWN = 1,
            DRONE_START_PLACING = 2,
            DRONE_PLACING_NOW = 3,
            DRONE_END_PLACING = 4,
            DRONE_FLYING_TO_SHOOT = 5,
            DRONE_SHOOTING = 6,
            DRONE_LAST = 7,
        };
        enum PathTrackingStatus
        {
            PATH_IDLE = 0,
            PATH_SETTING_HORIZONTAL_BEFORE = 1,
            PATH_SETTING_DIRECTION_TO_MOVE = 2,
            PATH_TILTING = 3,
            PATH_MOVING = 4,
            PATH_SETTING_HORIZONTAL_AFTER = 5,
            PATH_SETTING_FINAL_DIRECTION = 6,
        };

    public:
        virtual bool CanChildBeAdded(m3d::Class* pClass) const override;
        virtual void AddChild(ai::Obj* pObj) override;
        virtual bool RemoveChild(ai::Obj* pChild) override;
        virtual void CreateChildren() override;
        virtual void LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode) override;
        virtual void LoadRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode) override;
        virtual void SaveToXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const override;
        virtual void SaveRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const override;
        virtual void Update(float elapsedTime, unsigned int workTime) override;
        virtual bool ApplyModifier(ai::Modifier const& modifier) override;
        virtual void InflictDamage(ai::DamageInfo const& damageInfo) override;
        bool StartPlacingDrone();
        void StartMoving(CVector const& destination, CVector const& finalDirection);
        void NoticePlayer();
        float GetMaxHealth() const;
        float GetHealth() const;

    protected:
        virtual void _InternalPostLoad() override;
        virtual void _InternalCreateVisualPart() override;
        virtual void _Construct(bool bForAnimation) override;

    private:
        ai::NumericInRange<float> m_health;
        CVector m_linearVelocity;
        CVector m_relAngularVelocity;
        retruxx::vector<CVector, retruxx::allocator<CVector>> m_pointsForDrones;
        retruxx::vector<CVector, retruxx::allocator<CVector>> m_pointsForShooting;
        ai::Boss03::LiveStatus m_liveStatus;
        ai::Boss03::DroneSpawningStatus m_droneSpawningStatus;
        float m_dronePlacingTimeout;
        float m_shootingTimeout;
        ai::Boss03::PathTrackingStatus m_pathTrackingStatus;
        CVector m_desiredDestination;
        CVector m_desiredDirection;
        ai::Team* m_droneTeam;
        CStr m_pathNameForFlyingWithWings;
        m3d::CameraPath* m_currentFlyPath;
        float m_currentFlyTime;
        float m_keyPartsMaxDurability;
        void _UpdateLivePlacingDrones(float elapsedTime);
        void _UpdateLiveFightingWithWings(float elapsedTime);
        void _UpdatePathTracking(float elapsedTime);
        void _UpdateSwinging(float elapsedTime);
        void _SetLiveStatus(ai::Boss03::LiveStatus newLiveStatus);
        void _SetDroneSpawningStatus(ai::Boss03::DroneSpawningStatus newSpawinigStatus);
        void _SpawnDroneNow();
        void _SetPathTrackingStatus(ai::Boss03::PathTrackingStatus newPathTrackingStatus);
        bool _ProceedSettingVerticalAngle(float elapsedTime, float desiredAngle);
        bool _ProceedSettingHorizontalDirection(float elapsedTime, CVector const& desiredDir);
        void _UpdateLookAtPlayer(float elapsedTime);
        ai::Boss03Part* _GetBossPartByName(CStr const& partName);
        ai::Boss03Part* _GetDroneSpawner();
        ai::Boss03Part* _GetMainPart();
        void _SetAllPartsNotDamageable();
        void _RecalcFlyPath();
        bool _OnHealthValueBeforeApplyModifier(ai::Modifier const& modifier, float& newHealth);
        static CStr const& _LiveStatusToStr(ai::Boss03::LiveStatus status);
        static CStr const& _DroneSpawningStatusToStr(ai::Boss03::DroneSpawningStatus status);
        float CalcKeyPartsMaxDurability() const;

    public:
        static void Registration();
    };
}  // namespace ai
