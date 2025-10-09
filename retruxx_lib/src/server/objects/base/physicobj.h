#pragma once
#include "obj.h"
#include "prototypeinfo.h"
#include <core/ref_ptr.h>
#include <math/quaternion.h>
#include <math/vector.h>
#include <server/objects/physicbodies/geoms/geom.h>
#include <server/objects/physicbodies/geoms/sphereforintersection.h>

namespace m3d
{
    class DbgCounter;
}

struct dxBody;
class dBody;
struct dxSpace;

namespace ai
{
    CVector getPhysicObjOrPhysicBodyGeometricCenter(ai::Obj const*);

    class PhysicObjPrototypeInfo : public ai::PrototypeInfo
    {
    public:
        PhysicObjPrototypeInfo();
        virtual bool LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override /* 0x04 */;
        /* 0x0040 */ float m_intersectionRadius;
        /* 0x0044 */ float m_lookRadius;
    }; /* size: 0x0048 */

    static_assert(sizeof(PhysicObjPrototypeInfo) == 0x0048);

    class PhysicObj : public Obj
    {
        friend class IntersectionManager;
    protected:
        virtual ~PhysicObj() override /* 0x00 */;

    //private:
    public:
        PhysicObj(const ai::PhysicObjPrototypeInfo& prototypeInfo);
        virtual m3d::Object* Clone() override /* 0x00 */;
        static m3d::Object* __fastcall CreateObject();

    public:
        static m3d::Class* __fastcall GetBaseClass();
        virtual m3d::Class* GetClass() const override /* 0x00 */;
        static m3d::Class m_classPhysicObj;
        virtual const ai::PhysicObjPrototypeInfo* GetPrototypeInfo() const override /* 0x4c */;

    protected:
        static void __fastcall RegisterProperty(const char* Name, int id, ai::eGObjPropertySaveStatus saveStatus);

    public:
        virtual ai::eGObjPropertySaveStatus GetPropertySaveStatus(int id) const override /* 0x58 */;
        virtual void GetPropertiesNames(retruxx::set<CStr>& Props) const override /* 0x5c */;
        virtual void GetPropertiesIDs(retruxx::set<int>& Props) const override /* 0x60 */;
        virtual CStr GetPropertyName(int id) const override /* 0x78 */;
        virtual bool SetPropertyById(int propertyId, const m3d::AIParam& newValue) override /* 0x7c */;
        virtual int GetPropertyId(const char* PropertyName) const override /* 0x74 */;

    protected:
        static inline retruxx::map<CStr, int, ai::Obj::LessNoCaseCStr> m_propertiesMap;
        static inline retruxx::map<int, enum ai::eGObjPropertySaveStatus> m_propertiesSaveStatesMap;
        virtual bool _GetPropertyDefaultInternal(int propertyId, m3d::AIParam& retVal) const override /* 0x10c */;
        virtual bool _GetPropertyInternal(int propertyId, m3d::AIParam& retVal) const override /* 0x108 */;

        enum PhysicState
        {
            BODY_ENABLED_BIT = 1,
            GEOM_ENABLED_BIT = 2,
            STATIC_BIT = 4,
            BODY_DISABLED_GEOM_DISABLED = 0,
            BODY_ENABLED_GEOM_DISABLED = 1,
            BODY_DISABLED_GEOM_ENABLED = 2,
            BODY_ENABLED_GEOM_ENABLED = 3,
        };

    public:
        virtual void LoadRuntimeValues(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override /* 0xb0 */;
        virtual void SaveRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const override /* 0xb8 */;
        virtual void Remove() override /* 0x54 */;
        virtual void SetPassedToAnotherMapStatus() override /* 0x50 */;
        dBody* GetBody();
        const dBody* GetBody() const;
        virtual float GetMass() const /* 0x110 */;
        CVector GetPosition() const;
        virtual void SetPosition(const CVector& pos) /* 0x114 */;
        Quaternion GetRotation() const;
        virtual void SetRotation(const Quaternion& rot) /* 0x118 */;
        void AddRelativeRotation(const Quaternion& relDeltaRot);
        virtual CVector GetDirection() const /* 0x11c */;
        virtual void SetDirection(const CVector& direction) /* 0x120 */;
        virtual void SetDirections(const CVector& forward, const CVector& up) /* 0x124 */;
        CVector GetMassCenterPosition() const;
        void SetMassCenterPosition(const CVector& pos);
        virtual CVector GetGeometricCenter() const /* 0x128 */;
        CVector GetMassCenter() const;
        virtual void SetPostEnablePhysics() /* 0x12c */;
        virtual void SetPostEnablePhysicsIfPossible() /* 0x130 */;
        virtual void SetPostDisablePhysics() /* 0x134 */;
        virtual void SetPostDisablePhysicsWithAutoEnable() /* 0x138 */;
        virtual void SetPostRotation(const Quaternion& rot) /* 0x13c */;
        virtual void SetPostPosition(const CVector& pos) /* 0x140 */;
        Quaternion GetPostRotation() const;
        CVector GetPostPosition() const;
        virtual void SetPositionSelf(const CVector& pos) /* 0x144 */;
        virtual void SetRotationSelf(const Quaternion& rot) /* 0x148 */;
        virtual CVector GetLinearVelocity() const /* 0x14c */;
        virtual void SetLinearVelocity(const CVector& linearVel) /* 0x150 */;
        CVector GetAngularVelocity() const;
        void SetAngularVelocity(const CVector& angularVel);
        void SetForce(const CVector& force);
        void AddForce(const CVector& force);
        void AddForceAtPos(const CVector& force, const CVector& pos);
        void AddForceAtRelPos(const CVector& force, const CVector& relPos);
        void AddImpulse(const CVector& impulse);
        void AddImpulseAtPos(const CVector& impulse, const CVector& pos);
        void AddImpulseAtRelPos(const CVector& impulse, const CVector& relPos);
        void SetTorque(const CVector& torque);
        void AddTorque(const CVector& torque);
        void AddRelTorque(const CVector& relTorque);
        CVector GetPositionAtRelPoint(CVector point) const;
        virtual void Update(float elapsedTime, unsigned int workTime) override /* 0x80 */;
        virtual void PostCollide() override /* 0x84 */;
        void DisablePhysicsWithAutoEnable();
        virtual void DisablePhysics() /* 0x154 */;
        virtual void EnablePhysics() /* 0x158 */;
        virtual void DisableGeometry(bool changePhysicState) /* 0x15c */;
        virtual void EnableGeometry(bool changePhysicState) /* 0x160 */;
        void DisablePhysicsAndGeometry();
        void EnablePhysicsAndGeometry();
        void SetAutoDisabling(bool bIsAutoDisabling, float linearThreshold, float angularThreshold, int steps);
        void SetDisablePhysicsWhenBodyDisabled();
        bool CanPhysicsBeEnabled() const;
        void EnablePhysicsIfPossible();
        virtual void LinkGeomsToCollisionCells() /* 0x164 */;
        virtual void UnlinkGeomsFromCollisionCells() /* 0x168 */;
        virtual void RelinkGeomsToCollisionCells() /* 0x16c */;
        virtual void CheckCollisionCells() /* 0x170 */;
        virtual ai::Geom::CellAabb GetCollisionCellAabb() const /* 0x174 */;
        void SetCorrectEnabledCellsCounter();
        dxSpace* GetSpaceId() const;
        void RelinkToSpace(dxSpace* newSpace);
        virtual void TransferToSpace(dxSpace* newSpace) /* 0x178 */;
        void TransferToNewSpace();
        virtual void RenderDebugInfo() const override /* 0xe4 */;
        virtual void RenderObstacleDebugInfo() const /* 0x17c */;
        static void __fastcall Registration();
        int GetPhysicState() const;
        bool bIsStatic() const;
        bool bIsBodyDisabledGeomEnabled() const;
        bool GetGeomEnabledBit() const;
        bool GetBodyEnabledBit() const;
        void IncEnabledCellsCount();
        void DecEnabledCellsCount();
        void ZeroEnabledCellsCount();
        static m3d::DbgCounter* __fastcall GetRelinksToCollisionCounter();
        bool bIsUpdatingByODE() const;
        virtual void SetUpdatingByODE(bool byODE) /* 0x180 */;
        float GetIntersectionRadius() const;
        const ai::SphereForIntersection* GetIntersectionSphere() const;
        unsigned int GetSkin() const;
        virtual void SetSkin(int skin) /* 0x184 */;
        virtual void SetVisible() override /* 0xc8 */;
        virtual void SetInvisible() override /* 0xcc */;
        virtual bool IsVisible() /* 0x188 */;
        bool CanCreateCollisionEffect() const;
        void SetCollisionEffectCreated();

    protected:
        /* 0x00c0 */ int m_postActionFlags;
        /* 0x00c4 */ Quaternion m_postRotation;
        /* 0x00d4 */ CVector m_postPosition;
        /* 0x00e0 */ int m_physicBehaviorFlags;
        /* 0x00e4 */ CVector m_massCenter;
        /* 0x00f0 */ dxSpace* m_spaceId;
        /* 0x00f4 */ bool m_bIsSpaceOwner;
        /* 0x00f5 */ char Padding_10[3];
        /* 0x00f8 */ dBody* m_body;

        void _CreateSpace(bool bForUntransfer);
        virtual void _InternalPostLoad() override /* 0xfc */;
        virtual void _InternalCreateVisualPart() override /* 0x100 */;
        void _EnableIntersections(bool enable);
        ai::SphereForIntersection* _GetLookSphere() const;
        virtual void _UnlinkBodyFromGeoms() /* 0x18c */;
        virtual void _LinkBodyToGeoms() /* 0x190 */;
        virtual void _SetPositionToGeoms(const CVector& pos) /* 0x194 */;
        virtual void _SetRotationToGeoms(const Quaternion& rot) /* 0x198 */;
        void _SetStatic();
        void _SetStaticCollision();
        void _SetSimpleCollision();
        void _SetBodyEnabledBit(bool enabled);
        void _SetGeomEnabledBit(bool enabled);
        void _SetBoundSphereRadius(float radius);
        bool _UpdateMustBeRelinked();
        virtual void _UpdateOwnPhysics(float elapsedTime) /* 0x19c */;
        void _SetMassCenter(const CVector& massCenter);
        void _AdjustMassCenter();

    private:
        /* 0x00fc */ ref_ptr<ai::Obstacle> m_intersectionObstacle;
        /* 0x0100 */ ai::SphereForIntersection* m_lookSphere;
        /* 0x0104 */ ai::Sphere* m_boundSphere;
        /* 0x0108 */ int m_physicState;
        /* 0x010c */ int m_bIsUpdatingByODE;
        /* 0x0110 */ int m_enabledCellsCount;
        /* 0x0114 */ bool m_bBodyEnabledLastFrame;
        /* 0x0115 */ char Padding_11[3];
        /* 0x0118 */ int m_skinNumber;
        /* 0x011c */ float m_timeFromLastCollisionEffect;
        static inline m3d::DbgCounter* m_countRelinksToCollisionCells = nullptr;
        static void __fastcall _CommonBodyChangeEnabledStateCallback(dxBody* bodyId);

    public:
        static m3d::AIParam __fastcall AIGetCurPos(ai::Obj* pObj);
        virtual void DumpPhysicInfo(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const /* 0x1a0 */;
    }; /* size: 0x0120 */

    static_assert(sizeof(PhysicObj) == 0x0120);
}
