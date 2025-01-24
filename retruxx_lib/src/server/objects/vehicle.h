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
    class GeomRepositoryItem;
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
        virtual void _InternalCopyFrom(const ai::PrototypeInfo& rhs) override /* 0x00 */;

        struct WheelInfo
        {
            WheelInfo(const ai::VehiclePrototypeInfo::WheelInfo& __that);
            WheelInfo(const CStr wheelPrototypeName, ai::Wheel::WheelSteering steering);
            void PostLoad();
            /* 0x0000 */ int m_wheelPrototypeId;
            /* 0x0004 */ ai::Wheel::WheelSteering m_steering;
            /* 0x0008 */ CStr m_wheelPrototypeName;
        }; /* size: 0x0014 */

        using WheelInfoVector = retruxx::vector<ai::VehiclePrototypeInfo::WheelInfo, retruxx::allocator<ai::VehiclePrototypeInfo::WheelInfo> >;

    public:
        /* 0x0090 */ retruxx::vector<ai::VehiclePrototypeInfo::WheelInfo, retruxx::allocator<ai::VehiclePrototypeInfo::WheelInfo> > m_wheelInfos;
        /* 0x00a0 */ float m_diffRatio;
        /* 0x00a4 */ float m_maxEngineRpm;
        /* 0x00a8 */ float m_lowGearShiftLimit;
        /* 0x00ac */ float m_highGearShiftLimit;
        /* 0x00b0 */ float m_selfBrakingCoeff;
        /* 0x00b4 */ float m_steeringSpeed;
        /* 0x00b8 */ int m_decisionMatrixNum;
        /* 0x00bc */ float m_takingRadius;
        /* 0x00c0 */ unsigned char m_priority;
        /* 0x00c1 */ char Padding_208[3];
        /* 0x00c4 */ CStr m_hornSoundName;
        /* 0x00d0 */ float m_cameraHeight;
        /* 0x00d4 */ float m_cameraMaxDist;
        /* 0x00d8 */ CStr m_destroyEffectNames[4];
        /* 0x0108 */ int m_blastWavePrototypeId;
        /* 0x010c */ float m_additionalWheelsHover;
        /* 0x0110 */ float m_driftCoeff;
        /* 0x0114 */ float m_pressingForce;
        /* 0x0118 */ float m_healthRegeneration;
        /* 0x011c */ float m_durabilityRegeneration;
        VehiclePrototypeInfo();
        virtual  ~VehiclePrototypeInfo() override /* 0x00 */;
        virtual bool LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override /* 0x04 */;
        virtual void PostLoad() override /* 0x00 */;
        virtual ai::Obj* CreateTargetObject() const override /* 0x00 */;

    protected:
        /* 0x0120 */ CStr m_blastWavePrototypeName;
    }; /* size: 0x012c */

    static_assert(sizeof(VehiclePrototypeInfo) == 0x012c);

    class Vehicle : public ComplexPhysicObj
    {
        RETRUXX_DLL_FRIEND_CLASS(Vehicle);

        using AfterChangeFloatCallback = ai::MemberFunctionOneArg<ai::Vehicle, float, void>;
        using BeforeApplyModifierFloatCallback = ai::MemberFunctionTwoArgsRef<ai::Vehicle, ai::Modifier, float, bool>;

    protected:
        virtual ~Vehicle() override /* 0x00 */;

    public:
        Vehicle(const ai::VehiclePrototypeInfo& prototypeInfo);
        Vehicle(const ai::Vehicle&);
        virtual m3d::Object* Clone() override /* 0x00 */;
        static m3d::Object* CreateObject();

    public:
        static m3d::Class* GetBaseClass();
        virtual m3d::Class* GetClass() const override /* 0x00 */;
        static m3d::Class m_classVehicle;
        virtual const ai::VehiclePrototypeInfo* GetPrototypeInfo() const override /* 0x4c */;

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
        static const int NUM_GEARS;
        static const float GEAR_RATIOS[5];
        static const float TRANSFERBOX_RATIO;

        using IntGadgetMap = retruxx::map<int, ai::Gadget*, retruxx::less<int>, retruxx::allocator<retruxx::pair<int const, ai::Gadget*> > >;

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
        virtual void Remove() override /* 0x54 */;
        virtual void LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override /* 0x00 */;
        virtual void LoadRuntimeValues(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override /* 0xb0 */;
        virtual void SaveToXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const override /* 0x00 */;
        virtual void SaveRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const override /* 0xb8 */;
        virtual void CreateChildren() override /* 0x00 */;
        virtual void SetBelong(int newBelong) override /* 0x00 */;
        virtual bool CanChildBeAdded(m3d::Class* pClass) const override /* 0x00 */;
        virtual void AddChild(ai::Obj* pObj) override /* 0x00 */;
        virtual bool RemoveChild(ai::Obj* pObj) override /* 0x00 */;
        virtual void Update(float elapsedTime, unsigned int workTime) override /* 0x00 */;
        virtual void TransferToSpace(dxSpace* newSpace) override /* 0x178 */;
        virtual void SetPassedToAnotherMapStatus() override /* 0x50 */;
        virtual bool ApplyModifier(const ai::Modifier& modifier) override /* 0x00 */;
        virtual void SetPartByName(const CStr& partName, ai::VehiclePart* vehiclePart, bool bUnsafe) override /* 0x1a4 */;
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
        virtual void InflictDamage(const ai::DamageInfo& damageInfo) override /* 0x00 */;
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
        virtual float GetMass() const override /* 0x00 */;
        CVector GetSize() const;
        virtual void SetPositionSelf(const CVector& pos) override /* 0x00 */;
        virtual void SetRotationSelf(const Quaternion& rot) override /* 0x00 */;
        void SetGamePositionOnGround(const CVector& pos, bool bWithCollisions, bool bWithWater);
        virtual CVector GetGeometricCenter() const override /* 0x00 */;
        virtual CVector GetLinearVelocity() const override /* 0x00 */;
        virtual void SetLinearVelocity(const CVector& linearVel) override /* 0x00 */;
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
        virtual void RenderDebugInfo() const override /* 0xe4 */;
        virtual void Flow(ai::Obj* partToFlow, float averageSpeed) override /* 0x1a8 */;
        virtual void Blow(ai::Obj* partToBlow) override /* 0x1ac */;
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
        virtual void SetUpdatingByODE(bool byODE) override /* 0x00 */;
        ai::VehicleRecollection* GetRecollection() const;
        CVector GetRecollectionPosition(float recollectionRange) const;
        float GetCollisionRadius() const;
        void IncNumWheelsTouchingGround();
        virtual void SetSkin(int skin) override /* 0x184 */;
        virtual void SetRandomSkin() override /* 0x1b0 */;
        virtual unsigned int GetPrice(const ai::IPriceCoeffProvider* priceCoeffProvider) const override /* 0x00 */;
        virtual unsigned int GetSchwarz() const override /* 0x00 */;
        bool GetHorn() const;
        void SetHorn(bool bHorn);
        void HealWheels();
        virtual void SetVisible() override /* 0xc8 */;
        virtual void SetInvisible() override /* 0xcc */;
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
        virtual void GetGeoms(retruxx::vector<ai::Geom*, retruxx::allocator<ai::Geom*> >& geoms) const override /* 0x1b4 */;
        virtual ai::Obj* CloneObj() override /* 0x00 */;
        virtual void ClearSavedStatus() override /* 0x00 */;
        float GetDriftCoeff() const;
        void ShowVehicle(bool bShow);
        virtual void DisablePhysics() override /* 0x154 */;
        virtual void EnablePhysics() override /* 0x158 */;
        virtual void DisableGeometry(bool changePhysicState) override /* 0x15c */;
        virtual void EnableGeometry(bool changePhysicState) override /* 0x160 */;
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
            /* 0x0000 */ CVector m_initialPos;
            /* 0x000c */ Quaternion m_initialRot;
            WheelRuntimeInfo(const ai::Vehicle::WheelRuntimeInfo& __that);
            WheelRuntimeInfo(ai::Wheel* wheel);
            bool IsWheelPresent() const;
            const ai::Wheel* GetWheel() const;
            ai::Wheel* GetWheel();
            void SetWheel(ai::Wheel* wheel);

        private:
            /* 0x001c */ bool m_bWheelPresent;
            /* 0x001d */ char Padding_175[3];
            /* 0x0020 */ ai::Wheel* m_wheel;
        }; /* size: 0x0024 */

        using WheelRuntimeInfoVector = retruxx::vector<ai::Vehicle::WheelRuntimeInfo, retruxx::allocator<ai::Vehicle::WheelRuntimeInfo> >;

    protected:
        /* 0x014c */ retruxx::vector<ai::Vehicle::WheelRuntimeInfo, retruxx::allocator<ai::Vehicle::WheelRuntimeInfo> > m_wheels;
        virtual void _InternalPostLoad() override /* 0x00 */;
        virtual void _InternalCreateVisualPart() override /* 0x100 */;
        virtual ai::AI* GetAIPtr() override /* 0x00 */;
        virtual void _KeepSteer(float elapsedTime) /* 0x1cc */;
        virtual void _UpdateOwnPhysics(float elapsedTime) override /* 0x00 */;
        virtual void _PutContour() override /* 0x1c4 */;
        virtual void _RemoveContour() override /* 0x1c8 */;
        virtual float _CalcMassForBody() const override /* 0x1c0 */;

    private:
        /* 0x015c */ ai::AI m_AI;
        /* 0x01bc */ bool m_bHorn;
        /* 0x01bd */ bool m_bGodMode;
        /* 0x01be */ bool m_bImmortalMode;
        /* 0x01bf */ char Padding_178;
        /* 0x01c0 */ int m_stoppageMode;
        /* 0x01c4 */ int m_onOilMode;
        /* 0x01c8 */ int m_inSmokeScreenMode;
        /* 0x01cc */ float m_turboThrottleTime;
        /* 0x01d0 */ float m_turboThrottleValue;
        /* 0x01d4 */ float m_timeAfterDeath;
        /* 0x01d8 */ unsigned int m_numBlownParts;
        /* 0x01dc */ float m_timeAfterLastBlow;
        /* 0x01e0 */ unsigned int m_shootTypeChangeTime;
        /* 0x01e4 */ unsigned int m_shootTimeToWait;
        /* 0x01e8 */ bool m_bIsShooting;
        /* 0x01e9 */ char Padding_179[3];
        /* 0x01ec */ retruxx::map<int, ai::Gadget*, retruxx::less<int>, retruxx::allocator<retruxx::pair<int const, ai::Gadget*> > > m_gadgets;
        /* 0x01f8 */ float m_antiMissileGadgetSavingRadius;
        /* 0x01fc */ float m_diffRatio;
        /* 0x0200 */ float m_maxEngineRpm;
        /* 0x0204 */ float m_lowGearShiftLimit;
        /* 0x0208 */ float m_highGearShiftLimit;
        /* 0x020c */ float m_steeringSpeed;
        /* 0x0210 */ bool m_bIsTrailer;
        /* 0x0211 */ char Padding_180[3];
        /* 0x0214 */ float m_cruisingSpeed;
        /* 0x0218 */ bool m_maxSpeedLimited;
        /* 0x0219 */ char Padding_181[3];
        /* 0x021c */ float m_maxSpeedLimit;
        /* 0x0220 */ bool m_maxTorqueForced;
        /* 0x0221 */ char Padding_182[3];
        /* 0x0224 */ float m_maxTorqueForcedValue;
        /* 0x0228 */ int m_currentGear;
        /* 0x022c */ float m_throttle;
        /* 0x0230 */ float m_brake;
        /* 0x0234 */ float m_realThrottle;
        /* 0x0238 */ float m_engineRpm;
        /* 0x023c */ float m_averageEngineRpm;
        /* 0x0240 */ float m_driftCoeff;
        /* 0x0244 */ float m_averageWheelAVel;
        /* 0x0248 */ bool m_bAutoBrake;
        /* 0x0249 */ bool m_bHandBrake;

        using FloatDeque = retruxx::deque<float, retruxx::allocator<float> >;

    private:
        /* 0x024c */ retruxx::deque<float, retruxx::allocator<float> > m_recentEngineRpms;
        /* 0x0260 */ float m_steerRadians;
        /* 0x0264 */ ai::Vehicle::TurningBackStatus m_turningBackStatus;
        /* 0x0268 */ int m_seenObjId;
        /* 0x026c */ CVector m_curLookAt;
        /* 0x0278 */ int m_npcMotionControllerId;
        /* 0x027c */ retruxx::set<ref_ptr<ai::Obstacle>, retruxx::less<ref_ptr<ai::Obstacle> >, retruxx::allocator<ref_ptr<ai::Obstacle> > > m_currentNearbyObstacles;
        /* 0x0288 */ retruxx::set<ref_ptr<ai::Obstacle>, retruxx::less<ref_ptr<ai::Obstacle> >, retruxx::allocator<ref_ptr<ai::Obstacle> > > m_pastNearbyObstacles;
        /* 0x0294 */ CVector m_pastTakingSpherePosition;
        /* 0x02a0 */ bool m_bAllowPickUpMessage;
        /* 0x02a1 */ char Padding_183[3];
        /* 0x02a4 */ int m_pastNumNearbyChests;
        /* 0x02a8 */ int m_currentNumNearbyChests;
        /* 0x02ac */ scoped_ptr<ai::Box> m_lookBox;
        /* 0x02b0 */ scoped_ptr<ai::Box> m_targetBox;
        /* 0x02b4 */ retruxx::set<m3d::Class*, retruxx::less<m3d::Class*>, retruxx::allocator<m3d::Class*> > m_targetClasses;
        /* 0x02c0 */ CVector m_externalDestination;
        /* 0x02cc */ int m_numOfDrivenWheels;
        /* 0x02d0 */ CVector m_bumperPoint;
        /* 0x02dc */ bool m_bIsControlledByPlayer;
        /* 0x02dd */ bool m_bIsMovingAlongExternalPath;
        /* 0x02de */ char Padding_184[2];
        /* 0x02e0 */ int m_pathIndex;
        /* 0x02e4 */ bool m_bCanBeDistractedFromMoving;
        /* 0x02e5 */ char Padding_185[3];
        /* 0x02e8 */ CVector m_size;
        /* 0x02f4 */ CVector m_currentDestination;
        /* 0x0300 */ ai::Path* m_pPath;
        /* 0x0304 */ int m_pathNum;
        /* 0x0308 */ unsigned char m_priority;
        /* 0x0309 */ bool m_bCustomControl;
        /* 0x030a */ char Padding_186[2];
        /* 0x030c */ ai::Vehicle::CustomWeaponControlType m_customControlWeapons;
        /* 0x0310 */ CVector m_customControlWeaponsTarget;
        /* 0x031c */ int m_customControlWeaponsTargetObjId;

        using GunPointedMap = retruxx::map<int, bool, retruxx::less<int>, retruxx::allocator<retruxx::pair<int const, bool> > >;
        using GunPointedMapPair = retruxx::pair<int const, bool>;

    private:
        /* 0x0320 */ retruxx::map<int, bool, retruxx::less<int>, retruxx::allocator<retruxx::pair<int const, bool> > > m_gunsPointed;
        /* 0x032c */ bool m_bRocketLaunchersPresent;
        /* 0x032d */ char Padding_187[3];
        /* 0x0330 */ int m_indexInTeam;
        /* 0x0334 */ ai::Vehicle::VehicleMoveStatus m_moveStatus;
        /* 0x0338 */ ai::Vehicle::VehicleAttackStatus m_attackStatus;
        /* 0x033c */ ai::DamageType m_lastDamage;
        /* 0x0340 */ ai::VehiclePart* m_lastDamagedPart;
        /* 0x0344 */ ai::DamageType m_deathDamage;
        /* 0x0348 */ ai::SphereForIntersection* m_takingSphere;
        /* 0x034c */ ai::IzvratRepository* m_repository;
        /* 0x0350 */ ai::GeomRepository* m_groundRepository;
        /* 0x0354 */ retruxx::vector<enum ActionType, retruxx::allocator<enum ActionType> > m_effectActions;
        /* 0x0364 */ CStr m_blastEffectName;
        /* 0x0370 */ CStr m_destroyEffectNames[4];
        /* 0x03a0 */ m3d::SgSoundSourceNode* m_engineHighSoundNode;
        /* 0x03a4 */ m3d::SgSoundSourceNode* m_engineLowSoundNode;
        /* 0x03a8 */ m3d::SgNode* m_hornSoundNode;
        /* 0x03ac */ float m_cameraHeight;
        /* 0x03b0 */ float m_cameraMaxDist;
        /* 0x03b4 */ int m_trailerObjId;
        /* 0x03b8 */ dxJoint* m_trailerJoint;
        /* 0x03bc */ CVector m_relTrailerJointPosOnMe;
        /* 0x03c8 */ CVector m_relTrailerJointPosOnTrailer;
        /* 0x03d4 */ int m_recollectionId;
        /* 0x03d8 */ ai::VehicleUpdater* m_ownUpdater;
        /* 0x03dc */ int m_roleId;
        /* 0x03e0 */ ai::NumericInRangeRegenerating<float> m_timeOutForNextIntersectionWithWorld{0,0,10,-1};
        /* 0x04b8 */ int m_numWheelsTouchingGround;
        /* 0x04bc */ bool m_bHidden;
        /* 0x04bd */ char Padding_188[3];
        /* 0x04c0 */ int m_lockedObjId;
        /* 0x04c4 */ int m_toBeLockedObjId;
        /* 0x04c8 */ float m_timeToLockTarget;
        /* 0x04cc */ bool m_bMustGetOutOfDifficultPlace;
        /* 0x04cd */ bool m_bWasStuck;
        /* 0x04ce */ char Padding_189[2];
        /* 0x04d0 */ CVector m_prevPosToCheckStuck;
        /* 0x04dc */ float m_timeOutToCheckStuck;
        /* 0x04e0 */ CVector m_curSteeringForce;
        /* 0x04ec */ bool m_bCurSteeringForceValid;

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

        /* 0x04ed */ char Padding_190[3];
        /* 0x04f0 */ int m_soundRechargeChannelId;

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
    }; /* size: 0x04f4 */

    static_assert(sizeof(Vehicle) == 0x04f4);
}
