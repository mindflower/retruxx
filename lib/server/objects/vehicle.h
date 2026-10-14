#pragma once
#include <math/vector.h>
#include <math/quaternion.h>
#include "wheel.h"
#include "base/complexphysicobj.h"
#include <skelmodel.h>
#include <core/scoped_ptr.h>
#include <server/damageinfo.h>
#include <server/ai/ai.h>
#include <server/components/numericinrangeregenerating.h>

namespace m3d
{
    class SgSoundSourceNode;
}

struct dxJoint;

namespace ai
{
    class Basket;
    class VehicleRole;
    class VehicleRecollection;
    class Chassis;
    struct GeomRepositoryItem;
    class Team;
    class Cabin;
    class Gadget;
    class Box;
    class Path;
    class IzvratRepository;
    class VehicleUpdater;

    class VehiclePrototypeInfo : public ai::ComplexPhysicObjPrototypeInfo
    {
    protected:
        virtual void _InternalCopyFrom(const ai::PrototypeInfo& rhs) override;

        struct WheelInfo
        {
            WheelInfo(const CStr wheelPrototypeName, ai::Wheel::WheelSteering steering);
            void PostLoad();
            int m_wheelPrototypeId;
            ai::Wheel::WheelSteering m_steering;
            CStr m_wheelPrototypeName;
        };

        using WheelInfoVector = retruxx::vector<ai::VehiclePrototypeInfo::WheelInfo, retruxx::allocator<ai::VehiclePrototypeInfo::WheelInfo> >;

    public:
        retruxx::vector<ai::VehiclePrototypeInfo::WheelInfo, retruxx::allocator<ai::VehiclePrototypeInfo::WheelInfo> > m_wheelInfos;
        float m_diffRatio;
        float m_maxEngineRpm;
        float m_lowGearShiftLimit;
        float m_highGearShiftLimit;
        float m_selfBrakingCoeff;
        float m_steeringSpeed;
        int m_decisionMatrixNum;
        float m_takingRadius;
        unsigned char m_priority;
        char Padding_208[3];
        CStr m_hornSoundName;
        float m_cameraHeight;
        float m_cameraMaxDist;
        CStr m_destroyEffectNames[4];
        int m_blastWavePrototypeId;
        float m_additionalWheelsHover;
        float m_driftCoeff;
        float m_pressingForce;
        float m_healthRegeneration;
        float m_durabilityRegeneration;
        VehiclePrototypeInfo();
        virtual  ~VehiclePrototypeInfo() override;
        virtual bool LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override;
        virtual void PostLoad() override;
        virtual ai::Obj* CreateTargetObject() const override;

    protected:
        CStr m_blastWavePrototypeName;
    };

    class Vehicle : public ComplexPhysicObj
    {
        friend class VehicleUpdater;
        friend class Player;
        // Turns off its trailer's wheel steering.
        friend class ArticulatedVehicle;

        using AfterChangeFloatCallback = ai::MemberFunctionOneArg<ai::Vehicle, float, void>;
        using BeforeApplyModifierFloatCallback = ai::MemberFunctionTwoArgsRef<ai::Vehicle, ai::Modifier, float, bool>;

    protected:
        virtual ~Vehicle() override;

    public:
        Vehicle(const ai::VehiclePrototypeInfo& prototypeInfo);
        Vehicle(const ai::Vehicle&);
        virtual m3d::Object* Clone() override;
        static m3d::Object* CreateObject();

    public:
        static m3d::Class* GetBaseClass();
        virtual m3d::Class* GetClass() const override;
        static m3d::Class m_classVehicle;
        virtual const ai::VehiclePrototypeInfo* GetPrototypeInfo() const override;

    protected:
        static void __fastcall RegisterProperty(const char* Name, int id, ai::eGObjPropertySaveStatus saveStatus);

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

        enum EffectActionsPoses
        {
            DYNAMIC_ACTION = 0,
            LIGHT_ACTION = 1,
            EFFECT_ACTIONS_POSES_SIZE = 2,
        };

        enum TurningBackStatus
        {
            TURN_BACK_NONE = 0,
            TURN_BACK_ENABLED_ACCELERATING = 1,
            TURN_BACK_ENABLED_BRAKING = 2,
            TURN_BACK_DISABLED = 3,
        };

        enum CustomWeaponControlType
        {
            CUSTOM_WEAPON_CONTROL_NONE = 0,
            CUSTOM_WEAPON_CONTROL_POINT = 1,
            CUSTOM_WEAPON_CONTROL_OBJECT = 2,
        };

    private:
        static inline const int NUM_GEARS = 5;
        static inline const float GEAR_RATIOS[5] = {4.0f, 2.5f, 1.5f, 1.0f, 0.7f};
        static inline const float TRANSFERBOX_RATIO = 1.8f;

        using IntGadgetMap = retruxx::map<int, ai::Gadget*, retruxx::less<int>, retruxx::allocator<retruxx::pair<int const, ai::Gadget*> > >;

    public:
        enum VehicleMoveStatus
        {
            MOVE_IDLE = 0,
            MOVE_MOVING_ALONG_PATH = 1,
            MOVE_MOVING_BY_STEERING_FORCE = 2,
        };

        enum VehicleAttackStatus
        {
            ATTACK_IDLE = 0,
            ATTACK_ATTACKING = 1,
        };

    public:
        virtual void Remove() override;
        virtual void LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override;
        virtual void LoadRuntimeValues(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override;
        virtual void SaveToXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const override;
        virtual void SaveRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const override;
        virtual void CreateChildren() override;
        virtual void SetBelong(int newBelong) override;
        virtual bool CanChildBeAdded(m3d::Class* pClass) const override;
        virtual void AddChild(ai::Obj* pObj) override;
        virtual bool RemoveChild(ai::Obj* pObj) override;
        virtual void Update(float elapsedTime, unsigned int workTime) override;
        virtual void TransferToSpace(dxSpace* newSpace) override;
        virtual void SetPassedToAnotherMapStatus() override;
        virtual bool ApplyModifier(const ai::Modifier& modifier) override;
        virtual void SetPartByName(const CStr& partName, ai::VehiclePart* vehiclePart, bool bUnsafe) override;
        const ai::Chassis* GetChassis() const;
        ai::Chassis* GetChassis();
        const ai::Cabin* GetCabin() const;
        ai::Cabin* GetCabin();
        const ai::Basket* GetBasket() const;
        ai::Basket* GetBasket();
        void SetCabin(ai::VehiclePart* newCabin);
        void SetBasket(ai::VehiclePart* newBasket);
        unsigned int GetNumWheels() const;
        ai::Wheel* GetWheel(unsigned int num);
        const ai::Wheel* GetWheel(unsigned int num) const;
        Quaternion GetWheelInitialRotation(unsigned int num);
        float GetHealth() const;
        float GetMaxHealth() const;
        float GetFuel() const;
        float GetMaxFuel() const;
        float GetControl() const;
        void ActivateHeadLights(bool bActivate);
        void GetOutOfDifficultPlace();
        virtual void InflictDamage(const ai::DamageInfo& damageInfo) override;
        ai::Vehicle::VehicleMoveStatus GetMoveStatus() const;
        void SetMoveStatus(ai::Vehicle::VehicleMoveStatus moveStatus);
        ai::Vehicle::VehicleAttackStatus GetAttackStatus() const;
        void SetAttackStatus(ai::Vehicle::VehicleAttackStatus attackStatus);
        const ai::IzvratRepository* GetRepository() const;
        ai::IzvratRepository* GetRepository();
        void CollectNearbyObjectsToGroundRepository();
        ai::GeomRepository* GetGroundRepository() const;
        void PickUpNearbyObjects(bool bNeedCollectFromGround, unsigned int& originalNumItems, retruxx::vector<int, retruxx::allocator<int> >& addedObjIds);
        void IntersectWithWorld() const;
        const retruxx::set<ref_ptr<ai::Obstacle>, retruxx::less<ref_ptr<ai::Obstacle> >, retruxx::allocator<ref_ptr<ai::Obstacle> > >& GetNearbyObstacles() const;
        void GetEnemiesInNeighborhood(float radius, retruxx::vector<int, retruxx::allocator<int> >& enemiesIds) const;
        void SubscribeRadioManagerOnNearbyObjId(const int objId) const;
        void UnsubscribeRadioManagerFromNearbyObjId(const int objId) const;
        void UnsubscribeRadioManagerFromAllNearbyObjIds() const;
        bool bIsControlledByPlayer() const;
        bool bIsMovingAlongExternalPath() const;
        void SetCustomControlEnabled(bool Value);
        void SetCustomControlWeapons(int Custom);
        void SetCustomControlWeapons(ai::Vehicle::CustomWeaponControlType Custom);
        ai::Vehicle::CustomWeaponControlType GetCustomControlWeapons() const;
        void SetCustomControlWeaponsTarget(const CVector& lookAt);
        CVector GetCustomControlWeaponsTarget() const;
        void SetCustomControlWeaponsTargetObj(int lookAt);
        int GetCustomControlWeaponsTargetObj() const;
        void SetCustomLinearVelocity(float velocityValue);
        int GetCurrentGear() const;
        float GetEngineRpm() const;
        float GetMaxEngineRpm() const;
        float GetAverageEngineRpm() const;
        float GetMaxPower() const;
        void SetMaxPower(float newMaxPower);
        float GetMaxTorque() const;
        void SetMaxTorque(float newMaxTorque);
        int GetMaxGadgets(const CStr& gadgetResourceName) const;
        const retruxx::map<int, ai::Gadget*, retruxx::less<int>, retruxx::allocator<retruxx::pair<int const, ai::Gadget*> > >& GetGadgets() const;
        int GetValidSlotIdForGadget(const ai::Gadget* gadget) const;
        bool AddGadget(ai::Gadget* g);
        void RecalcGadgets();
        const ai::NumericInRangeRegenerating<float>& Health() const;
        ai::NumericInRangeRegenerating<float>& Health();
        const ai::NumericInRangeRegenerating<float>& Fuel() const;
        ai::NumericInRangeRegenerating<float>& Fuel();
        float GetFullDurability() const;
        float GetMaxFullDurability() const;
        float GetFullDurabilityCoeffForDamageType(ai::DamageType damageType) const;
        void WeaponLookAtPoint(const CVector& lookAt, float elapsedTime);
        void FireFromWeaponCustom(bool enable, const CVector& targetPoint, ai::Obj* target);
        void FireFromWeaponCustom2(bool enable, int targetId);
        void FireFromWeaponAI(bool enable, float elapsedTime, ai::Obj* target);
        void HoldFire(int msc);
        float EstimateDamageAI(const CVector& point, retruxx::vector<int, retruxx::allocator<int> > exceptions) const;
        float EstimateDamageAI() const;
        float EstimateDamageFromPositionAI(const CVector& position, const CVector& point, retruxx::vector<int, retruxx::allocator<int> > exceptions) const;
        float GetMaxFiringRangeAI() const;
        bool FireFromWeaponByGunId(int gunId, bool enable);
        bool FireFromWeaponByGunPartName(const CStr& gunPartName, bool enable);
        bool DriveToPoint(const CVector& point, const CVector& nextPoint, bool bPrecisely, float elapsedTime);
        virtual float GetMass() const override;
        CVector GetSize() const;
        virtual void SetPositionSelf(const CVector& pos) override;
        virtual void SetRotationSelf(const Quaternion& rot) override;
        void SetGamePositionOnGround(const CVector& pos, bool bWithCollisions, bool bWithWater);
        virtual CVector GetGeometricCenter() const override;
        virtual CVector GetLinearVelocity() const override;
        virtual void SetLinearVelocity(const CVector& linearVel) override;
        void ResetPositionAndRotation();
        float GetCameraHeight() const;
        float GetCameraMaxDist() const;
        int GetIndexInTeam() const;
        void SetIndexInTeam(int indexInTeam);
        ai::Team* GetTeam() const;
        float GetThrottle() const;
        void SetThrottle(float throttle, bool autoBrake);
        bool bIsBraking() const;
        float GetBrake() const;
        void SetBrake(float brake);
        void SetHandBrake();
        void ReleaseAllPedals();
        float GetSteer() const;
        void SetSteer(float radians);
        float GetCurrentSteerAngle() const;
        unsigned char GetPriority() const;
        float GetMaxSpeed() const;
        void SetMaxSpeed(float speed);
        float GetCruisingSpeed() const;
        void SetCruisingSpeed(float cruisingSpeed);
        float GetDefaultCruisingSpeed() const;
        void LimitMaxSpeed(float maxSpeedLimit);
        void UnlimitMaxSpeed();
        void SetForcedMaxTorque(float forcedMaxTorque);
        void ResetForcedMaxTorque();
        void SetTurningToGroundForceAndTorque(const CVector& pos, const CVector& force, const CVector& torque);
        CVector GetBumperPoint() const;
        const ai::Wheel* GetFirstExistingWheel() const;
        virtual void RenderDebugInfo() const override;
        virtual void Flow(ai::Obj* partToFlow, float averageSpeed) override;
        virtual void Blow(ai::Obj* partToBlow) override;
        int SetExternalPath(const retruxx::vector<CVector2, retruxx::allocator<CVector2> >& path);
        int SetExternalPathByName(const char* pathName);
        void SetCanBeDistractedFromMoving(bool bCanBeDistracted);
        void SetExternalDestination(const CVector& destination);
        void PlaceToEndOfPath();
        void SetTrailer();
        bool IsTrailer() const;
        bool AddThing(const ai::GeomRepositoryItem& item, bool bFlushInReferenceChests);
        bool AddItemsToRepository(const char* prototypeName, int amount);
        bool RemoveItemsFromRepository(const char* prototypeName, int amount);
        bool HasAmountOfItemsInRepository(const char* prototypeName, int amount) const;
        bool CanPlaceItemsToRepository(const char* prototypeName, int amount);
        bool AddObjectToRepository(ai::Obj* pObj);
        m3d::AIParam TakeOffAllGuns();
        void AttachTrailer(const char* trailerPrototypeName);
        void DetachTrailer();
        bool TrailerExists() const;
        ai::Vehicle* GetTrailer() const;
        virtual void SetUpdatingByODE(bool byODE) override;
        ai::VehicleRecollection* GetRecollection() const;
        CVector GetRecollectionPosition(float recollectionRange) const;
        float GetCollisionRadius() const;
        void IncNumWheelsTouchingGround();
        virtual void SetSkin(int skin) override;
        virtual void SetRandomSkin() override;
        virtual unsigned int GetPrice(const ai::IPriceCoeffProvider* priceCoeffProvider) const override;
        virtual unsigned int GetSchwarz() const override;
        bool GetHorn() const;
        void SetHorn(bool bHorn);
        void HealWheels();
        virtual void SetVisible() override;
        virtual void SetInvisible() override;
        bool getGodMode() const;
        void setGodMode(bool bGod);
        bool getImmortalMode() const;
        void setImmortalMode(bool bImmortal);
        bool GetStoppageMode() const;
        void IncStoppageMode();
        void DecStoppageMode();
        bool GetOnOilMode() const;
        void IncOnOilMode();
        void DecOnOilMode();
        bool GetInSmokeScreenMode() const;
        void IncInSmokeScreenMode();
        void DecInSmokeScreenMode();
        float GetTurboThrottleTime() const;
        void SetTurboThrottleTime(float time);
        float GetTurboThrottleValue() const;
        void SetTurboThrottleValue(float Value);
        virtual void GetGeoms(retruxx::vector<ai::Geom*, retruxx::allocator<ai::Geom*> >& geoms) const override;
        virtual ai::Obj* CloneObj() override;
        virtual void ClearSavedStatus() override;
        float GetDriftCoeff() const;
        void ShowVehicle(bool bShow);
        virtual void DisablePhysics() override;
        virtual void EnablePhysics() override;
        virtual void DisableGeometry(bool changePhysicState) override;
        virtual void EnableGeometry(bool changePhysicState) override;
        ai::VehicleRole* GetRole() const;
        void SetRole(ai::VehicleRole* vr);
        int GetNpcMotionControllerId() const;
        void SetNpcMotionControllerId(int npcMotionControllerId);
        int GetSeenObjId() const;
        int GetInfoObjId() const;
        int GetLockedObjId() const;
        int GetToBeLockedObjId() const;
        float GetTimeToLockTarget() const;
        bool bRocketLaunchersPresent() const;
        void EnableSounds(bool bEnable);
        bool IsHealthZero() const;
        void PlaySoundOnRechargeWeapon();
        int CheckSkin(int skinNum);

        class WheelRuntimeInfo
        {
        public:
            CVector m_initialPos;
            Quaternion m_initialRot;
            WheelRuntimeInfo(ai::Wheel* wheel);
            bool IsWheelPresent() const;
            const ai::Wheel* GetWheel() const;
            ai::Wheel* GetWheel();
            void SetWheel(ai::Wheel* wheel);

        private:
            bool m_bWheelPresent;
            char Padding_175[3];
            ai::Wheel* m_wheel;
        };

        using WheelRuntimeInfoVector = retruxx::vector<ai::Vehicle::WheelRuntimeInfo, retruxx::allocator<ai::Vehicle::WheelRuntimeInfo> >;

    protected:
        retruxx::vector<ai::Vehicle::WheelRuntimeInfo, retruxx::allocator<ai::Vehicle::WheelRuntimeInfo> > m_wheels;
        virtual void _InternalPostLoad() override;
        virtual void _InternalCreateVisualPart() override;
        virtual ai::AI* GetAIPtr() override;
        virtual void _KeepSteer(float elapsedTime);
        virtual void _UpdateOwnPhysics(float elapsedTime) override;
        virtual void _PutContour() override;
        virtual void _RemoveContour() override;
        virtual float _CalcMassForBody() const override;

    private:
        ai::AI m_AI;
        bool m_bHorn;
        bool m_bGodMode;
        bool m_bImmortalMode;
        char Padding_178;
        int m_stoppageMode;
        int m_onOilMode;
        int m_inSmokeScreenMode;
        float m_turboThrottleTime;
        float m_turboThrottleValue;
        float m_timeAfterDeath;
        unsigned int m_numBlownParts;
        float m_timeAfterLastBlow;
        unsigned int m_shootTypeChangeTime;
        unsigned int m_shootTimeToWait;
        bool m_bIsShooting;
        char Padding_179[3];
        retruxx::map<int, ai::Gadget*, retruxx::less<int>, retruxx::allocator<retruxx::pair<int const, ai::Gadget*> > > m_gadgets;
        float m_antiMissileGadgetSavingRadius;
        float m_diffRatio;
        float m_maxEngineRpm;
        float m_lowGearShiftLimit;
        float m_highGearShiftLimit;
        float m_steeringSpeed;
        bool m_bIsTrailer;
        char Padding_180[3];
        float m_cruisingSpeed;
        bool m_maxSpeedLimited;
        char Padding_181[3];
        float m_maxSpeedLimit;
        bool m_maxTorqueForced;
        char Padding_182[3];
        float m_maxTorqueForcedValue;
        int m_currentGear;
        float m_throttle;
        float m_brake;
        float m_realThrottle;
        float m_engineRpm;
        float m_averageEngineRpm;
        float m_driftCoeff;
        float m_averageWheelAVel;
        bool m_bAutoBrake;
        bool m_bHandBrake;

        using FloatDeque = retruxx::deque<float, retruxx::allocator<float> >;

    private:
        retruxx::deque<float, retruxx::allocator<float> > m_recentEngineRpms;
        float m_steerRadians;
        ai::Vehicle::TurningBackStatus m_turningBackStatus;
        int m_seenObjId;
        CVector m_curLookAt;
        int m_npcMotionControllerId;
        mutable retruxx::set<ref_ptr<ai::Obstacle>> m_currentNearbyObstacles;
        mutable retruxx::set<ref_ptr<ai::Obstacle>> m_pastNearbyObstacles;
        mutable CVector m_pastTakingSpherePosition;
        mutable bool m_bAllowPickUpMessage;
        char Padding_183[3];
        mutable int m_pastNumNearbyChests;
        mutable int m_currentNumNearbyChests;
        scoped_ptr<ai::Box> m_lookBox;
        scoped_ptr<ai::Box> m_targetBox;
        retruxx::set<m3d::Class*, retruxx::less<m3d::Class*>, retruxx::allocator<m3d::Class*> > m_targetClasses;
        CVector m_externalDestination;
        int m_numOfDrivenWheels;
        CVector m_bumperPoint;
        bool m_bIsControlledByPlayer;
        bool m_bIsMovingAlongExternalPath;
        char Padding_184[2];
        int m_pathIndex;
        bool m_bCanBeDistractedFromMoving;
        char Padding_185[3];
        CVector m_size;
        CVector m_currentDestination;
        ai::Path* m_pPath;
        int m_pathNum;
        unsigned char m_priority;
        bool m_bCustomControl;
        char Padding_186[2];
        ai::Vehicle::CustomWeaponControlType m_customControlWeapons;
        CVector m_customControlWeaponsTarget;
        int m_customControlWeaponsTargetObjId;

        using GunPointedMap = retruxx::map<int, bool, retruxx::less<int>, retruxx::allocator<retruxx::pair<int const, bool> > >;
        using GunPointedMapPair = retruxx::pair<int const, bool>;

    private:
        retruxx::map<int, bool, retruxx::less<int>, retruxx::allocator<retruxx::pair<int const, bool> > > m_gunsPointed;
        bool m_bRocketLaunchersPresent;
        char Padding_187[3];
        int m_indexInTeam;
        ai::Vehicle::VehicleMoveStatus m_moveStatus;
        ai::Vehicle::VehicleAttackStatus m_attackStatus;
        ai::DamageType m_lastDamage;
        ai::VehiclePart* m_lastDamagedPart;
        ai::DamageType m_deathDamage;
        ai::SphereForIntersection* m_takingSphere;
        ai::IzvratRepository* m_repository;
        ai::GeomRepository* m_groundRepository;
        retruxx::vector<enum ActionType, retruxx::allocator<enum ActionType> > m_effectActions;
        CStr m_blastEffectName;
        CStr m_destroyEffectNames[4];
        m3d::SgSoundSourceNode* m_engineHighSoundNode;
        m3d::SgSoundSourceNode* m_engineLowSoundNode;
        m3d::SgNode* m_hornSoundNode;
        float m_cameraHeight;
        float m_cameraMaxDist;
        int m_trailerObjId;
        dxJoint* m_trailerJoint;
        CVector m_relTrailerJointPosOnMe;
        CVector m_relTrailerJointPosOnTrailer;
        int m_recollectionId;
        ai::VehicleUpdater* m_ownUpdater;
        int m_roleId;
        ai::NumericInRangeRegenerating<float> m_timeOutForNextIntersectionWithWorld;
        int m_numWheelsTouchingGround;
        bool m_bHidden;
        char Padding_188[3];
        int m_lockedObjId;
        int m_toBeLockedObjId;
        float m_timeToLockTarget;
        bool m_bMustGetOutOfDifficultPlace;
        mutable bool m_bWasStuck;
        char Padding_189[2];
        mutable CVector m_prevPosToCheckStuck;
        mutable float m_timeOutToCheckStuck;
        mutable CVector m_curSteeringForce;
        mutable bool m_bCurSteeringForceValid;

        float _GetTimeOutForNextIntersectionWithWorld() const;
        void _DropChests();
        void _EvaluateToDead();
        void _DeadActions(float elapsedTime);
        int _UpdateRepositoryOnChangeBasket();
        void _ValidateVehicleParts();
        void _OnChangeCabin();
        void _OnChangeBasket();
        void _CheckForNearbyChests() const;
        void _CauseCustomGunPointedEvents();
        CVector _GetCustomWeaponTargetPoint() const;
        float _GetAngleTo(const CVector& point) const;
        CVector _GetNextPathPoint() const;
        CVector _GetLastPathPoint() const;
        void _AdjustSizeAndBumperPoint();
        void _AdjustWheel(ai::Vehicle::WheelRuntimeInfo& wheelInfo);
        void _TurnWheelByAngle(ai::Wheel* pWheel, float angle);
        void _DriveBySteeringForce(const CVector& steeringForce);
        bool _bPassedPathPoint(const CVector& point, const CVector& nextPoint, bool bPrecisely) const;
        bool _bPointIsBehind(const CVector& point) const;
        void _KeepThrottle(bool applyActions);
        void _KeepGearBox(float elapsedTime);
        void _KeepSuspension();
        void _ApplyStabilizingForces();
        CVector _CalcSteeringForce(float elapsedTime) const;
        CVector _CalcSteeringForceToPathPoint(const CVector& point, const CVector& nextPoint) const;
        CVector _CalcRepulsionForNearbyObjects(const CVector& myPos, const CVector& myPredictedPos, const CVector& myVel, const CVector& guide, bool bIsLookObstacle, CVector& attraction) const;
        CVector _CalcRepulsionForObstacle(const ai::Obstacle* ob, const CVector& myPos, const CVector& myPredictedPos, const CVector& myVel, const CVector& guide, bool bIsLookObstacle, CVector& attraction) const;
        void _AdjustLookBox(bool bForLooking, const CVector& myPos, const CVector& pathPoint, const CVector& guide) const;
        void _UpdateAlarmStatus();
        CVector _GetEtalonWheelAVel() const;
        void _CalcRpms();
        CStr _GetTrailerName() const;
        void _AttachExistingTrailer(ai::Vehicle* trailer, bool bTrailerIsNew);
        void _AdjustTrailer();
        void _TakeWaterIntoAccount(float elapsedTime);
        void _UpdateSeenObjAndWeapons(float elapsedTime);
        void _UpdateInfoObj();
        void _UpdateLockedObj(float elapsedTime);
        void _UpdatePhysicsUpdater();
        void _InflictDamageToRepository(float damage);
        void _CreateBlastWave();
        void _EnsureRecollection();
        bool _SetIdleMoveStatus();
        void _SetIdleMoveStatusAndCauseTargetReached();
        void _GetOutOfDifficlultPlaceInternal();
        float _GetCabinControlCoeff() const;

        char Padding_190[3];
        int m_soundRechargeChannelId;

    public:
        static void __fastcall Registration();
        static m3d::AIParam __fastcall VehicleAIOnDefend(ai::Obj* pObj);
        static m3d::AIParam __fastcall VehicleAIOnMove(ai::Obj* pObj);
        static m3d::AIParam __fastcall VehicleAIOnAttack(ai::Obj* pObj);
        static m3d::AIParam __fastcall VehicleAIOnDead(ai::Obj* pObj);
        static const char* __fastcall MoveStatusName(ai::Vehicle::VehicleMoveStatus);
        static const char* __fastcall AttackStatusName(ai::Vehicle::VehicleAttackStatus);
        static ai::Vehicle::VehicleMoveStatus __fastcall MoveStatusID(const CStr&);
        static ai::Vehicle::VehicleAttackStatus __fastcall AttackStatusID(const CStr&);
    };
}
