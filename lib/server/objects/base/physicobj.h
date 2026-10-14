#pragma once
#include "obj.h"
#include "prototypeinfo.h"
#include <core/ref_ptr.h>
#include <math/quaternion.h>
#include <math/vector.h>
#include <server/objects/physicbodies/geoms/geom.h>
#include <server/objects/physicbodies/geoms/sphereforintersection.h>

class ContextModelWnd;

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
        virtual bool LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override;
        float m_intersectionRadius;
        float m_lookRadius;
    };

    class PhysicObj : public Obj
    {
        friend class IntersectionManager;
        // ContextModelWnd reads the skin number of the vehicle a cabin or basket
        // is mounted on, so the preview matches the vehicle's paint job.
        friend class ::ContextModelWnd;
        // Vehicle reads other objects' intersection obstacles to size its pick-up and rescue checks.
        friend class Vehicle;
        friend class CombatMastermind;
        // PlasmaBunch's and Rocket's constructors mark themselves as physically enabled while their
        // body stays off.
        friend class PlasmaBunch;
        friend class Rocket;
        friend class MortarShell;
        friend class Submarine;
        // Keeps the boss out of the physics by hand.
        friend class Boss03;
        // Hitches its trailer's body.
        friend class ArticulatedVehicle;
        // Reads the obstacle radius when placing generated vehicles.
        friend class VehiclesGeneratorPrototypeInfo;
        // Reads the obstacle radius when placing vehicles passed to a new map.
        friend class PassageData;

    protected:
        virtual ~PhysicObj() override;

    //private:
    public:
        PhysicObj(const ai::PhysicObjPrototypeInfo& prototypeInfo);
        virtual m3d::Object* Clone() override;
        static m3d::Object* __fastcall CreateObject();

    public:
        static m3d::Class* __fastcall GetBaseClass();
        virtual m3d::Class* GetClass() const override;
        static m3d::Class m_classPhysicObj;
        virtual const ai::PhysicObjPrototypeInfo* GetPrototypeInfo() const override;

    protected:
        static void __fastcall RegisterProperty(const char* Name, int id, ai::eGObjPropertySaveStatus saveStatus);

    public:
        virtual ai::eGObjPropertySaveStatus GetPropertySaveStatus(int id) const override;
        virtual void GetPropertiesNames(retruxx::set<CStr>& Props) const override;
        virtual void GetPropertiesIDs(retruxx::set<int>& Props) const override;
        virtual CStr GetPropertyName(int id) const override;
        virtual bool SetPropertyById(int propertyId, const m3d::AIParam& newValue) override;
        virtual int GetPropertyId(const char* PropertyName) const override;

    protected:
        static inline retruxx::map<CStr, int, ai::Obj::LessNoCaseCStr> m_propertiesMap;
        static inline retruxx::map<int, enum ai::eGObjPropertySaveStatus> m_propertiesSaveStatesMap;
        virtual bool _GetPropertyDefaultInternal(int propertyId, m3d::AIParam& retVal) const override;
        virtual bool _GetPropertyInternal(int propertyId, m3d::AIParam& retVal) const override;

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
        virtual void LoadRuntimeValues(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override;
        virtual void SaveRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const override;
        virtual void Remove() override;
        virtual void SetPassedToAnotherMapStatus() override;
        dBody* GetBody();
        const dBody* GetBody() const;
        virtual float GetMass() const;
        CVector GetPosition() const;
        virtual void SetPosition(const CVector& pos);
        Quaternion GetRotation() const;
        virtual void SetRotation(const Quaternion& rot);
        void AddRelativeRotation(const Quaternion& relDeltaRot);
        virtual CVector GetDirection() const;
        virtual void SetDirection(const CVector& direction);
        virtual void SetDirections(const CVector& forward, const CVector& up);
        CVector GetMassCenterPosition() const;
        void SetMassCenterPosition(const CVector& pos);
        virtual CVector GetGeometricCenter() const;
        CVector GetMassCenter() const;
        virtual void SetPostEnablePhysics();
        virtual void SetPostEnablePhysicsIfPossible();
        virtual void SetPostDisablePhysics();
        virtual void SetPostDisablePhysicsWithAutoEnable();
        virtual void SetPostRotation(const Quaternion& rot);
        virtual void SetPostPosition(const CVector& pos);
        Quaternion GetPostRotation() const;
        CVector GetPostPosition() const;
        virtual void SetPositionSelf(const CVector& pos);
        virtual void SetRotationSelf(const Quaternion& rot);
        virtual CVector GetLinearVelocity() const;
        virtual void SetLinearVelocity(const CVector& linearVel);
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
        virtual void Update(float elapsedTime, unsigned int workTime) override;
        virtual void PostCollide() override;
        void DisablePhysicsWithAutoEnable();
        virtual void DisablePhysics();
        virtual void EnablePhysics();
        virtual void DisableGeometry(bool changePhysicState);
        virtual void EnableGeometry(bool changePhysicState);
        void DisablePhysicsAndGeometry();
        void EnablePhysicsAndGeometry();
        void SetAutoDisabling(bool bIsAutoDisabling, float linearThreshold, float angularThreshold, int steps);
        void SetDisablePhysicsWhenBodyDisabled();
        bool CanPhysicsBeEnabled() const;
        void EnablePhysicsIfPossible();
        virtual void LinkGeomsToCollisionCells();
        virtual void UnlinkGeomsFromCollisionCells();
        virtual void RelinkGeomsToCollisionCells();
        virtual void CheckCollisionCells();
        virtual ai::Geom::CellAabb GetCollisionCellAabb() const;
        void SetCorrectEnabledCellsCounter();
        dxSpace* GetSpaceId() const;
        void RelinkToSpace(dxSpace* newSpace);
        virtual void TransferToSpace(dxSpace* newSpace);
        void TransferToNewSpace();
        virtual void RenderDebugInfo() const override;
        virtual void RenderObstacleDebugInfo() const;
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
        virtual void SetUpdatingByODE(bool byODE);
        float GetIntersectionRadius() const;
        const ai::SphereForIntersection* GetIntersectionSphere() const;
        unsigned int GetSkin() const;
        virtual void SetSkin(int skin);
        virtual void SetVisible() override;
        virtual void SetInvisible() override;
        virtual bool IsVisible();
        bool CanCreateCollisionEffect() const;
        void SetCollisionEffectCreated();

    protected:
        int m_postActionFlags;
        Quaternion m_postRotation;
        CVector m_postPosition;
        int m_physicBehaviorFlags;
        CVector m_massCenter;
        dxSpace* m_spaceId;
        bool m_bIsSpaceOwner;
        dBody* m_body;

        void _CreateSpace(bool bForUntransfer);
        virtual void _InternalPostLoad() override;
        virtual void _InternalCreateVisualPart() override;
        void _EnableIntersections(bool enable);
        ai::SphereForIntersection* _GetLookSphere() const;
        virtual void _UnlinkBodyFromGeoms();
        virtual void _LinkBodyToGeoms();
        virtual void _SetPositionToGeoms(const CVector& pos);
        virtual void _SetRotationToGeoms(const Quaternion& rot);
        void _SetStatic();
        void _SetStaticCollision();
        void _SetSimpleCollision();
        void _SetBodyEnabledBit(bool enabled);
        void _SetGeomEnabledBit(bool enabled);
        void _SetBoundSphereRadius(float radius);
        bool _UpdateMustBeRelinked();
        virtual void _UpdateOwnPhysics(float elapsedTime);
        void _SetMassCenter(const CVector& massCenter);
        void _AdjustMassCenter();

    private:
        ref_ptr<ai::Obstacle> m_intersectionObstacle;
        ai::SphereForIntersection* m_lookSphere;
        ai::Sphere* m_boundSphere;
        int m_physicState;
        int m_bIsUpdatingByODE;
        int m_enabledCellsCount;
        bool m_bBodyEnabledLastFrame;
        int m_skinNumber;
        float m_timeFromLastCollisionEffect;
        static inline m3d::DbgCounter* m_countRelinksToCollisionCells = nullptr;
        static void __fastcall _CommonBodyChangeEnabledStateCallback(dxBody* bodyId);

    public:
        static m3d::AIParam __fastcall AIGetCurPos(ai::Obj* pObj);
        virtual void DumpPhysicInfo(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const;
    };

    
    CVector getPhysicObjOrPhysicBodyPosition(ai::Obj const*);
    CVector getPhysicObjOrPhysicBodyLinearVelocity(ai::Obj const*);
    CVector getPhysicObjOrPhysicBodyDirection(ai::Obj const*);
}
