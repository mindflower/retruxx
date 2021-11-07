#pragma once
#include <math/vector.h>
#include <math/quaternion.h>
#include "wheel.h"
#include "base/complexphysicobj.h"
#include <deque>
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
    class GeomRepositoryItem;
    class Team;
    class Cabin;
    class Gadget;
    class Box;
    class Path;
    class IzvratRepository;
    class VehicleUpdater;

    class VehiclePrototypeInfo : public ComplexPhysicObjPrototypeInfo
    {
    public:
        class WheelInfo
        {
        public:
            WheelInfo(CStr, Wheel::WheelSteering);
            void PostLoad();

        private:
            int m_wheelPrototypeId;
            Wheel::WheelSteering m_steering;
            CStr m_wheelPrototypeName;
        };

    public:
        VehiclePrototypeInfo();
        virtual bool LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);
        virtual void PostLoad();
        virtual ~VehiclePrototypeInfo();
        virtual ai::Obj* CreateTargetObject() const;

    protected:
        virtual void _InternalCopyFrom(PrototypeInfo const&);

    private:
        std::vector<WheelInfo> m_wheelInfos;
        float m_diffRatio;
        float m_maxEngineRpm;
        float m_lowGearShiftLimit;
        float m_highGearShiftLimit;
        float m_selfBrakingCoeff;
        float m_steeringSpeed;
        int m_decisionMatrixNum;
        float m_takingRadius;
        unsigned __int8 m_priority;
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
        CStr m_blastWavePrototypeName;
    };

    class Vehicle :  public ComplexPhysicObj
    {
    public:
        enum EffectActionsPoses
        {
            DYNAMIC_ACTION = 0x0,
            LIGHT_ACTION = 0x1,
            EFFECT_ACTIONS_POSES_SIZE = 0x2,
        };

        enum TurningBackStatus
        {
            TURN_BACK_NONE = 0x0,
            TURN_BACK_ENABLED_ACCELERATING = 0x1,
            TURN_BACK_ENABLED_BRAKING = 0x2,
            TURN_BACK_DISABLED = 0x3,
        };

        enum CustomWeaponControlType
        {
            CUSTOM_WEAPON_CONTROL_NONE = 0x0,
            CUSTOM_WEAPON_CONTROL_POINT = 0x1,
            CUSTOM_WEAPON_CONTROL_OBJECT = 0x2,
        };

        enum VehicleMoveStatus
        {
            MOVE_IDLE = 0x0,
            MOVE_MOVING_ALONG_PATH = 0x1,
            MOVE_MOVING_BY_STEERING_FORCE = 0x2,
        };

        enum VehicleAttackStatus
        {
            ATTACK_IDLE = 0x0,
            ATTACK_ATTACKING = 0x1,
        };

        class WheelRuntimeInfo
        {
        public:
            Wheel const* GetWheel() const;
            Wheel* GetWheel();
            WheelRuntimeInfo(Wheel*);
            bool IsWheelPresent() const;
            void SetWheel(Wheel*);

        private:
            CVector m_initialPos;
            Quaternion m_initialRot;
            bool m_bWheelPresent;
            Wheel* m_wheel;
        };

    public:
        static inline m3d::Class m_classVehicle;

    public:
        VehicleMoveStatus GetMoveStatus() const ;
        float GetCruisingSpeed() const ;
        void UnlimitMaxSpeed();
        void IncStoppageMode();
        virtual void EnablePhysics();
        int GetInfoObjId() const ;
        bool bRocketLaunchersPresent() const ;
        void RecalcGadgets();
        void HoldFire(int);
        VehicleRecollection * GetRecollection() const ;
        virtual void SetLinearVelocity(CVector const &);
        CustomWeaponControlType GetCustomControlWeapons() const ;
        Quaternion GetWheelInitialRotation(unsigned int);
        void ActivateHeadLights(bool);
        virtual void Flow(Obj *,float);
        void DetachTrailer();
        virtual void AddChild(Obj *);
        virtual void SaveRuntimeValues(m3d::cmn::XmlFile *,m3d::cmn::XmlNode *) const ;
        bool GetHorn() const ;
        bool IsTrailer() const ;
        float GetCameraHeight() const ;
        int GetLockedObjId() const ;
        void setGodMode(bool);
        std::map<int,Gadget *,std::less<int>,std::allocator<std::pair<int const ,Gadget *> > > const & GetGadgets() const ;
        virtual void SetRandomSkin();
        float GetDefaultCruisingSpeed() const ;
        float GetMaxEngineRpm() const ;
        int SetExternalPath(std::vector<CVector2,std::allocator<CVector2> > const &);
        Chassis const * GetChassis() const ;
        Chassis * GetChassis();
        virtual void Remove();
        void FireFromWeaponCustom2(bool,int);
        float GetCameraMaxDist() const ;
        virtual bool SetPropertyById(int,m3d::AIParam const &);
        void DecOnOilMode();
        unsigned int GetNumWheels() const ;
        void SetHandBrake();
        CVector GetRecollectionPosition(float) const ;
        float GetSteer() const ;
        void IncNumWheelsTouchingGround();
        virtual void CreateChildren();
        void SetCustomControlEnabled(bool);
        float GetMaxFiringRangeAI() const ;
        static m3d::Class * __fastcall GetBaseClass();
        float GetMaxFuel() const ;
        bool getImmortalMode() const ;
        float GetHealth() const ;
        bool AddThing(GeomRepositoryItem const &,bool);
        void EnableSounds(bool);
        virtual void TransferToSpace(dxSpace *);
        static m3d::AIParam __fastcall VehicleAIOnAttack(Obj *);
        void SetCanBeDistractedFromMoving(bool);
        int GetCustomControlWeaponsTargetObj() const ;
        int GetToBeLockedObjId() const ;
        virtual void SetSkin(int);
        virtual void DisablePhysics();
        void FireFromWeaponCustom(bool,CVector const &,Obj *);
        virtual float GetMass() const ;
        virtual unsigned int GetPrice(IPriceCoeffProvider const *) const ;
        bool FireFromWeaponByGunId(int,bool);
        void SetMoveStatus(VehicleMoveStatus);
        virtual void SetPassedToAnotherMapStatus();
        void SetCruisingSpeed(float);
        void SetExternalDestination(CVector const &);
        static void __fastcall Registration();
        virtual eGObjPropertySaveStatus GetPropertySaveStatus(int) const ;
        virtual void SetPartByName(CStr const &,VehiclePart *,bool);
        float GetCurrentSteerAngle() const ;
        int GetCurrentGear() const ;
        virtual CVector GetLinearVelocity() const ;
        virtual void SaveToXML(m3d::cmn::XmlFile *,m3d::cmn::XmlNode *) const ;
        void SetCustomControlWeapons(int);
        void SetCustomControlWeapons(CustomWeaponControlType);
        virtual unsigned int GetSchwarz() const ;
        void UnsubscribeRadioManagerFromNearbyObjId(int) const ;
        virtual void Blow(Obj *);
        void SetHorn(bool);
        int SetExternalPathByName(char const *);
        void IncInSmokeScreenMode();
        unsigned char GetPriority() const ;
        bool getGodMode() const ;
        bool FireFromWeaponByGunPartName(CStr const &,bool);
        virtual void SetUpdatingByODE(bool);
        void GetOutOfDifficultPlace();
        float GetMaxHealth() const ;
        virtual int GetPropertyId(char const *) const ;
        float EstimateDamageAI(CVector const &,std::vector<int,std::allocator<int> >) const ;
        float EstimateDamageAI() const ;
        void PickUpNearbyObjects(bool,unsigned int &,std::vector<int,std::allocator<int> > &);
        float GetFullDurability() const ;
        NumericInRangeRegenerating<float> const & Health() const ;
        NumericInRangeRegenerating<float> & Health();
        bool GetOnOilMode() const ;
        bool bIsMovingAlongExternalPath() const ;
        virtual void DisableGeometry(bool);
        void SetSteer(float);
        float GetControl() const ;
        bool AddItemsToRepository(char const *,int);
        void ResetForcedMaxTorque();
        virtual void ClearSavedStatus();
        void HealWheels();
        void setImmortalMode(bool);
        bool bIsBraking() const ;
        static m3d::AIParam __fastcall VehicleAIOnMove(Obj *);
        bool IsHealthZero() const ;
        void FireFromWeaponAI(bool,float,Obj *);
        bool AddGadget(Gadget *);
        void SetCustomControlWeaponsTargetObj(int);
        bool AddObjectToRepository(Obj *);
        void ResetPositionAndRotation();
        virtual void SetPositionSelf(CVector const &);
        void WeaponLookAtPoint(CVector const &,float);
        bool CanPlaceItemsToRepository(char const *,int);
        void AttachTrailer(char const *);
        void SetTurboThrottleValue(float);
        void SetMaxPower(float);
        bool GetStoppageMode() const ;
        void GetEnemiesInNeighborhood(float,std::vector<int,std::allocator<int> > &) const ;
        VehicleRole* GetRole() const ;
        void SubscribeRadioManagerOnNearbyObjId(int) const ;
        int GetValidSlotIdForGadget(Gadget const *) const ;
        Wheel const * GetFirstExistingWheel() const ;
        bool TrailerExists() const ;
        float GetAverageEngineRpm() const ;
        CVector GetCustomControlWeaponsTarget() const ;
        float GetMaxTorque() const ;
        void SetAttackStatus(VehicleAttackStatus);
        Vehicle(VehiclePrototypeInfo const &);
        virtual void LoadRuntimeValues(m3d::cmn::XmlFile *,m3d::cmn::XmlNode const *);
        void DecInSmokeScreenMode();
        void SetTrailer();
        virtual void RenderDebugInfo() const ;
        virtual void SetVisible();
        int GetNpcMotionControllerId() const ;
        void SetIndexInTeam(int);
        NumericInRangeRegenerating<float> & Fuel();
        NumericInRangeRegenerating<float> const & Fuel() const ;
        float GetFuel() const ;
        int GetSeenObjId() const ;
        virtual void GetGeoms(std::vector<Geom *,std::allocator<Geom *> > &) const ;
        virtual void LoadFromXML(m3d::cmn::XmlFile *,m3d::cmn::XmlNode const *);
        bool bIsControlledByPlayer() const ;
        m3d::AIParam TakeOffAllGuns();
        void SetThrottle(float,bool);
        float GetBrake() const ;
        float GetMaxSpeed() const ;
        virtual bool CanChildBeAdded(m3d::Class *) const ;
        float GetTurboThrottleTime() const ;
        float GetFullDurabilityCoeffForDamageType(DamageType) const ;
        virtual Obj * CloneObj();
        int CheckSkin(int);
        static m3d::AIParam __fastcall VehicleAIOnDefend(Obj *);
        float GetEngineRpm() const ;
        virtual void SetBelong(int);
        Team * GetTeam() const ;
        void UnsubscribeRadioManagerFromAllNearbyObjIds() const ;
        void SetBasket(VehiclePart *);
        Cabin const * GetCabin() const ;
        Cabin * GetCabin();
        float GetTurboThrottleValue() const ;
        int GetMaxGadgets(CStr const &) const ;
        float GetMaxPower() const ;
        void DecStoppageMode();
        void ShowVehicle(bool);
        void SetRole(VehicleRole *);
        std::set<ref_ptr<Obstacle>,std::less<ref_ptr<Obstacle> >,std::allocator<ref_ptr<Obstacle> > > const & GetNearbyObstacles() const ;
        virtual CStr GetPropertyName(int) const ;
        bool RemoveItemsFromRepository(char const *,int);
        void SetCustomControlWeaponsTarget(CVector const &);
        float GetTimeToLockTarget() const ;
        void SetGamePositionOnGround(CVector const &,bool,bool);
        void SetMaxTorque(float);
        void LimitMaxSpeed(float);
        virtual m3d::Class * GetClass() const ;
        CVector GetSize() const ;
        void CollectNearbyObjectsToGroundRepository();
        VehicleAttackStatus GetAttackStatus() const ;
        void ReleaseAllPedals();
        bool GetInSmokeScreenMode() const ;
        void PlaySoundOnRechargeWeapon();
        Vehicle * GetTrailer() const ;
        void IncOnOilMode();
        virtual CVector GetGeometricCenter() const ;
        float EstimateDamageFromPositionAI(CVector const &,CVector const &,std::vector<int,std::allocator<int> >) const ;
        void SetNpcMotionControllerId(int);
        virtual VehiclePrototypeInfo const * GetPrototypeInfo() const ;
        void PlaceToEndOfPath();
        int GetIndexInTeam() const ;
        CVector GetBumperPoint() const ;
        GeomRepository * GetGroundRepository() const ;
        virtual bool ApplyModifier(Modifier const &);
        void SetTurningToGroundForceAndTorque(CVector const &,CVector const &,CVector const &);
        Wheel * GetWheel(unsigned int);
        Wheel const * GetWheel(unsigned int) const ;
        float GetThrottle() const ;
        IzvratRepository const * GetRepository() const ;
        IzvratRepository * GetRepository();
        virtual bool RemoveChild(Obj *);
        void SetBrake(float);
        static m3d::AIParam __fastcall VehicleAIOnDead(Obj *);
        void SetMaxSpeed(float);
        bool HasAmountOfItemsInRepository(char const *,int) const ;
        virtual void GetPropertiesNames(std::set<CStr,std::less<CStr>,std::allocator<CStr> > &) const ;
        void SetTurboThrottleTime(float);
        virtual void Update(float,unsigned int);
        float GetCollisionRadius() const ;
        float GetDriftCoeff() const ;
        virtual void SetRotationSelf(Quaternion const &);
        virtual void InflictDamage(DamageInfo const &);
        float GetMaxFullDurability() const ;
        virtual void SetInvisible();
        void SetCustomLinearVelocity(float);
        Basket * GetBasket();
        Basket const * GetBasket() const ;
        void SetCabin(VehiclePart *);
        virtual void GetPropertiesIDs(std::set<int,std::less<int>,std::allocator<int> > &) const ;
        void IntersectWithWorld() const ;
        void SetForcedMaxTorque(float);
        bool DriveToPoint(CVector const &,CVector const &,bool,float);
        virtual void EnableGeometry(bool);

    protected:
        virtual bool _GetPropertyInternal(int,m3d::AIParam &) const ;
        virtual void _PutContour();
        virtual void _InternalCreateVisualPart();
        virtual bool _GetPropertyDefaultInternal(int,m3d::AIParam &) const ;
        virtual void _InternalPostLoad();
        virtual float _CalcMassForBody() const ;
        virtual void _RemoveContour();
        virtual void _KeepSteer(float);
        virtual void _UpdateOwnPhysics(float);
        virtual AI * GetAIPtr();
        virtual ~Vehicle();
        static void __fastcall RegisterProperty(char const *,int,eGObjPropertySaveStatus);

    private:
        float _GetTimeOutForNextIntersectionWithWorld() const ;
        void _ApplyStabilizingForces();
        void _UpdateAlarmStatus();
        virtual m3d::Object * Clone();
        void _AttachExistingTrailer(Vehicle *,bool);
        void _InflictDamageToRepository(float);
        bool _SetIdleMoveStatus();
        CVector _CalcRepulsionForNearbyObjects(CVector const &,CVector const &,CVector const &,CVector const &,bool,CVector &) const ;
        void _DeadActions(float);
        void _OnChangeCabin();
        bool _bPassedPathPoint(CVector const &,CVector const &,bool) const ;
        void _CauseCustomGunPointedEvents();
        void _KeepSuspension();
        CVector _GetNextPathPoint() const ;
        void _KeepThrottle(bool);
        void _CreateBlastWave();
        CStr _GetTrailerName() const ;
        CVector _CalcRepulsionForObstacle(Obstacle const *,CVector const &,CVector const &,CVector const &,CVector const &,bool,CVector &) const ;
        void _DriveBySteeringForce(CVector const &);
        void _TurnWheelByAngle(Wheel *,float);
        CVector _GetCustomWeaponTargetPoint() const ;
        void _EnsureRecollection();
        void _UpdateLockedObj(float);
        bool _bPointIsBehind(CVector const &) const ;
        void _GetOutOfDifficlultPlaceInternal();
        float _GetCabinControlCoeff() const ;
        void _ValidateVehicleParts();
        void _AdjustLookBox(bool,CVector const &,CVector const &,CVector const &) const ;
        CVector _CalcSteeringForceToPathPoint(CVector const &,CVector const &) const ;
        void _TakeWaterIntoAccount(float);
        void _KeepGearBox(float);
        void _SetIdleMoveStatusAndCauseTargetReached();
        void _CalcRpms();
        int _UpdateRepositoryOnChangeBasket();
        void _UpdatePhysicsUpdater();
        void _AdjustSizeAndBumperPoint();
        void _OnChangeBasket();
        CVector _GetLastPathPoint() const ;
        float _GetAngleTo(CVector const &) const ;
        void _AdjustWheel(WheelRuntimeInfo &);
        static m3d::Object * __fastcall CreateObject();
        void _CheckForNearbyChests() const ;
        void _AdjustTrailer();
        void _UpdateSeenObjAndWeapons(float);
        CVector _CalcSteeringForce(float) const ;
        CVector _GetEtalonWheelAVel() const ;
        void _DropChests();
        void _EvaluateToDead();

    private:
        std::vector<WheelRuntimeInfo> m_wheels;
        AI m_AI;
        bool m_bHorn;
        bool m_bGodMode;
        bool m_bImmortalMode;
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
        std::map<int,Gadget *> m_gadgets;
        float m_antiMissileGadgetSavingRadius;
        float m_diffRatio;
        float m_maxEngineRpm;
        float m_lowGearShiftLimit;
        float m_highGearShiftLimit;
        float m_steeringSpeed;
        bool m_bIsTrailer;
        float m_cruisingSpeed;
        bool m_maxSpeedLimited;
        float m_maxSpeedLimit;
        bool m_maxTorqueForced;
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
        std::deque<float> m_recentEngineRpms;
        float m_steerRadians;
        TurningBackStatus m_turningBackStatus;
        int m_seenObjId;
        CVector m_curLookAt;
        int m_npcMotionControllerId;
        std::set<ref_ptr<Obstacle>> m_currentNearbyObstacles;
        std::set<ref_ptr<Obstacle>> m_pastNearbyObstacles;
        CVector m_pastTakingSpherePosition;
        bool m_bAllowPickUpMessage;
        int m_pastNumNearbyChests;
        int m_currentNumNearbyChests;
        scoped_ptr<Box> m_lookBox;
        scoped_ptr<Box> m_targetBox;
        std::set<m3d::Class *> m_targetClasses;
        CVector m_externalDestination;
        int m_numOfDrivenWheels;
        CVector m_bumperPoint;
        bool m_bIsControlledByPlayer;
        bool m_bIsMovingAlongExternalPath;
        int m_pathIndex;
        bool m_bCanBeDistractedFromMoving;
        CVector m_size;
        CVector m_currentDestination;
        Path *m_pPath;
        int m_pathNum;
        unsigned __int8 m_priority;
        bool m_bCustomControl;
        Vehicle::CustomWeaponControlType m_customControlWeapons;
        CVector m_customControlWeaponsTarget;
        int m_customControlWeaponsTargetObjId;
        std::map<int,bool> m_gunsPointed;
        bool m_bRocketLaunchersPresent;
        int m_indexInTeam;
        VehicleMoveStatus m_moveStatus;
        VehicleAttackStatus m_attackStatus;
        DamageType m_lastDamage;
        VehiclePart *m_lastDamagedPart;
        DamageType m_deathDamage;
        SphereForIntersection *m_takingSphere;
        IzvratRepository *m_repository;
        GeomRepository *m_groundRepository;
        std::vector<ActionType> m_effectActions;
        CStr m_blastEffectName;
        CStr m_destroyEffectNames[4];
        m3d::SgSoundSourceNode *m_engineHighSoundNode;
        m3d::SgSoundSourceNode *m_engineLowSoundNode;
        m3d::SgNode *m_hornSoundNode;
        float m_cameraHeight;
        float m_cameraMaxDist;
        int m_trailerObjId;
        dxJoint *m_trailerJoint;
        CVector m_relTrailerJointPosOnMe;
        CVector m_relTrailerJointPosOnTrailer;
        int m_recollectionId;
        VehicleUpdater *m_ownUpdater;
        int m_roleId;
        NumericInRangeRegenerating<float> m_timeOutForNextIntersectionWithWorld;
        int m_numWheelsTouchingGround;
        bool m_bHidden;
        int m_lockedObjId;
        int m_toBeLockedObjId;
        float m_timeToLockTarget;
        bool m_bMustGetOutOfDifficultPlace;
        bool m_bWasStuck;
        CVector m_prevPosToCheckStuck;
        float m_timeOutToCheckStuck;
        CVector m_curSteeringForce;
        bool m_bCurSteeringForceValid;
        int m_soundRechargeChannelId;
    };
}
