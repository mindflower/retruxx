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
        /* 0x0090 */ retruxx::vector<int, retruxx::allocator<int> > m_dronePrototypeIds;
        /* 0x00a0 */ int m_maxDrones;
        /* 0x00a4 */ CVector m_droneRelPosition;
        /* 0x00b0 */ Quaternion m_droneRelRotation;
        /* 0x00c0 */ float m_maxHealth;
        /* 0x00c4 */ float m_maxHorizAngularVelocity;
        /* 0x00c8 */ float m_horizAngularAcceleration;
        /* 0x00cc */ float m_maxVertAngularVelocity;
        /* 0x00d0 */ float m_vertAngularAcceleration;
        /* 0x00d4 */ float m_maxLinearVelocity;
        /* 0x00d8 */ float m_linearAcceleration;
        /* 0x00dc */ float m_pathTrackTiltAngle;
        /* 0x00e0 */ float m_maxShootingTime;
        /* 0x00e4 */ float m_defaultHover;
        /* 0x00e8 */ float m_hoverForPlacingDrones;
        Boss03PrototypeInfo();
        virtual bool LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override /* 0x00 */;
        virtual void PostLoad() override /* 0x00 */;
        virtual void RefreshFromXml(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override /* 0x00 */;
        virtual ai::Obj* CreateTargetObject() const override /* 0x00 */;

    private:
        /* 0x00ec */ retruxx::vector<CStr, retruxx::allocator<CStr> > m_dronePrototypeNames;
    }; /* size: 0x00fc */

    static_assert(sizeof(Boss03PrototypeInfo) == 0x00fc);

    class Boss03 : public ai::AnimatedComplexPhysicObj
    {
    public:
        using AfterChangeFloatCallback = ai::MemberFunctionOneArg<ai::Boss03, float, void>;
        using BeforeApplyModifierFloatCallback = ai::MemberFunctionTwoArgsRef<ai::Boss03, ai::Modifier, float, bool>;

    protected:
        virtual  ~Boss03() override /* 0x00 */;

    private:
        Boss03(const ai::Boss03PrototypeInfo& prototypeInfo);
        Boss03(const ai::Boss03&);
        virtual m3d::Object* Clone() override /* 0x00 */;
        static m3d::Object* __fastcall CreateObject();

    public:
        static m3d::Class* __fastcall GetBaseClass();
        virtual m3d::Class* GetClass() const override /* 0x00 */;
        static m3d::Class m_classBoss03;
        virtual const ai::Boss03PrototypeInfo* GetPrototypeInfo() const override /* 0x4c */;

    protected:
        static void __fastcall RegisterProperty(const char*, int, ai::eGObjPropertySaveStatus);

    public:
        virtual ai::eGObjPropertySaveStatus GetPropertySaveStatus(int id) const override /* 0x00 */;
        virtual void GetPropertiesNames(retruxx::set<CStr, retruxx::less<CStr>, retruxx::allocator<CStr> >& Props) const override /* 0x00 */;
        virtual void GetPropertiesIDs(retruxx::set<int, retruxx::less<int>, retruxx::allocator<int> >& Props) const override /* 0x00 */;
        virtual CStr GetPropertyName(int id) const override /* 0x00 */;
        virtual bool SetPropertyById(int propertyId, const m3d::AIParam& newValue) override /* 0x00 */;
        virtual int GetPropertyId(const char* PropertyName) const override /* 0x00 */;

    protected:
        static retruxx::map<CStr, int, ai::Obj::LessNoCaseCStr, retruxx::allocator<retruxx::pair<CStr const, int> > > m_propertiesMap;
        static retruxx::map<int, enum ai::eGObjPropertySaveStatus, retruxx::less<int>, retruxx::allocator<retruxx::pair<int const, enum ai::eGObjPropertySaveStatus> > > m_propertiesSaveStatesMap;
        virtual bool _GetPropertyDefaultInternal(int propertyId, m3d::AIParam& retVal) const override /* 0x00 */;
        virtual bool _GetPropertyInternal(int propertyId, m3d::AIParam& retVal) const override /* 0x00 */;

        enum LiveStatus;
        enum DroneSpawningStatus;
        enum PathTrackingStatus;

    public:
        virtual bool CanChildBeAdded(m3d::Class* pClass) const override /* 0x00 */;
        virtual void AddChild(ai::Obj* pObj) override /* 0x00 */;
        virtual bool RemoveChild(ai::Obj* pChild) override /* 0x00 */;
        virtual void CreateChildren() override /* 0x00 */;
        virtual void LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override /* 0x00 */;
        virtual void LoadRuntimeValues(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override /* 0x00 */;
        virtual void SaveToXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const override /* 0x00 */;
        virtual void SaveRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const override /* 0x00 */;
        virtual void Update(float elapsedTime, unsigned int workTime) override /* 0x00 */;
        virtual bool ApplyModifier(const ai::Modifier& modifier) override /* 0x00 */;
        virtual void InflictDamage(const ai::DamageInfo& damageInfo) override /* 0x00 */;
        bool StartPlacingDrone();
        void StartMoving(const CVector& destination, const CVector& finalDirection);
        void NoticePlayer();
        float GetMaxHealth() const;
        float GetHealth() const;

    protected:
        virtual void _InternalPostLoad() override /* 0x00 */;
        virtual void _InternalCreateVisualPart() override /* 0x00 */;
        virtual void _Construct(bool bForAnimation) override /* 0x00 */;

    private:
        /* 0x014c */ ai::NumericInRange<float> m_health;
        /* 0x01f8 */ CVector m_linearVelocity;
        /* 0x0204 */ CVector m_relAngularVelocity;
        /* 0x0210 */ retruxx::vector<CVector, retruxx::allocator<CVector> > m_pointsForDrones;
        /* 0x0220 */ retruxx::vector<CVector, retruxx::allocator<CVector> > m_pointsForShooting;
        /* 0x0230 */ ai::Boss03::LiveStatus m_liveStatus;
        /* 0x0234 */ ai::Boss03::DroneSpawningStatus m_droneSpawningStatus;
        /* 0x0238 */ float m_dronePlacingTimeout;
        /* 0x023c */ float m_shootingTimeout;
        /* 0x0240 */ ai::Boss03::PathTrackingStatus m_pathTrackingStatus;
        /* 0x0244 */ CVector m_desiredDestination;
        /* 0x0250 */ CVector m_desiredDirection;
        /* 0x025c */ ai::Team* m_droneTeam;
        /* 0x0260 */ CStr m_pathNameForFlyingWithWings;
        /* 0x026c */ m3d::CameraPath* m_currentFlyPath;
        /* 0x0270 */ float m_currentFlyTime;
        /* 0x0274 */ float m_keyPartsMaxDurability;
        void _UpdateLivePlacingDrones(float elapsedTime);
        void _UpdateLiveFightingWithWings(float elapsedTime);
        void _UpdatePathTracking(float elapsedTime);
        void _UpdateSwinging(float elapsedTime);
        void _SetLiveStatus(ai::Boss03::LiveStatus newLiveStatus);
        void _SetDroneSpawningStatus(ai::Boss03::DroneSpawningStatus newSpawinigStatus);
        void _SpawnDroneNow();
        void _SetPathTrackingStatus(ai::Boss03::PathTrackingStatus newPathTrackingStatus);
        bool _ProceedSettingVerticalAngle(float elapsedTime, float desiredAngle);
        bool _ProceedSettingHorizontalDirection(float elapsedTime, const CVector& desiredDir);
        void _UpdateLookAtPlayer(float elapsedTime);
        ai::Boss03Part* _GetBossPartByName(const CStr& partName);
        ai::Boss03Part* _GetDroneSpawner();
        ai::Boss03Part* _GetMainPart();
        void _SetAllPartsNotDamageable();
        void _RecalcFlyPath();
        bool _OnHealthValueBeforeApplyModifier(const ai::Modifier& modifier, float& newHealth);
        static const CStr& __fastcall _LiveStatusToStr(ai::Boss03::LiveStatus status);
        static const CStr& __fastcall _DroneSpawningStatusToStr(ai::Boss03::DroneSpawningStatus status);
        float CalcKeyPartsMaxDurability() const;

    public:
        static void __fastcall Registration();
    }; /* size: 0x0278 */

    static_assert(sizeof(Boss03) == 0x0278);
}
