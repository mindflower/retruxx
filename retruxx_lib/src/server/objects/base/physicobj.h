#pragma once
#include "obj.h"
#include "prototypeinfo.h"
#include <core/ref_ptr.h>
#include <math/quaternion.h>
#include <math/vector.h>
#include <server/objects/physicbodies/geoms/geom.h>

namespace m3d
{
    class DbgCounter;
}

struct dxBody;
class dBody;
struct dxSpace;

namespace ai
{
    class Sphere;
    class SphereForIntersection;
    class Obstacle;

    CVector getPhysicObjOrPhysicBodyGeometricCenter(ai::Obj const*);

    class PhysicObjPrototypeInfo : public ai::PrototypeInfo
    {
    public:
        PhysicObjPrototypeInfo();
        virtual bool LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);

    private:
        float m_intersectionRadius;
        float m_lookRadius;
    };

    class PhysicObj :  public Obj
    {
    public:
        virtual float GetMass() const;
        virtual void SetPosition(CVector const&);
        virtual void SetRotation(Quaternion const&);
        virtual CVector GetDirection() const;
        virtual void SetDirection(CVector const&);
        virtual void SetDirections(CVector const&, CVector const&);
        virtual CVector GetGeometricCenter() const;
        virtual void SetPostEnablePhysics();

    public:
        virtual void LoadRuntimeValues(m3d::cmn::XmlFile *,m3d::cmn::XmlNode const *);
        bool GetBodyEnabledBit() const ;
        Quaternion GetRotation() const ;
        virtual void SetPostDisablePhysicsWithAutoEnable();
        CVector GetMassCenter() const ;
        bool bIsUpdatingByODE() const ;
        void RelinkToSpace(dxSpace *);
        virtual bool IsVisible();
        Quaternion GetPostRotation() const ;
        dxSpace * GetSpaceId() const ;
        virtual void SetSkin(int);
        static m3d::Class* GetBaseClass();
        virtual void UnlinkGeomsFromCollisionCells();
        bool CanPhysicsBeEnabled() const ;
        void DisablePhysicsAndGeometry();
        virtual bool SetPropertyById(int,m3d::AIParam const &);
        virtual void SetPostEnablePhysicsIfPossible();
        CVector GetMassCenterPosition() const ;
        virtual void TransferToSpace(dxSpace *);
        virtual void SetVisible();
        void AddTorque(CVector const &);
        virtual void CheckCollisionCells();
        static void __fastcall Registration();
        void AddImpulseAtRelPos(CVector const &,CVector const &);
        void TransferToNewSpace();
        bool CanCreateCollisionEffect() const ;
        virtual void GetPropertiesNames(std::set<CStr,std::less<CStr>,std::allocator<CStr> > &) const ;
        void AddImpulse(CVector const &);
        virtual void SetPositionSelf(CVector const &);
        virtual void SetPostRotation(Quaternion const &);
        virtual void DumpPhysicInfo(m3d::cmn::XmlFile *,m3d::cmn::XmlNode *) const ;
        void SetAutoDisabling(bool,float,float,int);
        void IncEnabledCellsCount();
        void SetTorque(CVector const &);
        unsigned int GetSkin() const ;
        void EnablePhysicsIfPossible();
        virtual void SetPassedToAnotherMapStatus();
        void SetCollisionEffectCreated();
        virtual void DisableGeometry(bool);
        virtual void EnableGeometry(bool);
        float GetIntersectionRadius() const ;
        dBody * GetBody();
        dBody const * GetBody() const ;
        virtual void Update(float,unsigned int);
        virtual void SetUpdatingByODE(bool);
        void SetMassCenterPosition(CVector const &);
        virtual void EnablePhysics();
        virtual void SaveRuntimeValues(m3d::cmn::XmlFile *,m3d::cmn::XmlNode *) const ;
        void AddForceAtPos(CVector const &,CVector const &);
        void DecEnabledCellsCount();
        CVector GetPosition() const ;
        virtual void PostCollide();
        virtual void DisablePhysics();
        void SetDisablePhysicsWhenBodyDisabled();
        void SetForce(CVector const &);
        CVector GetPostPosition() const ;
        void SetAngularVelocity(CVector const &);
        int GetPhysicState() const ;
        virtual void RenderDebugInfo() const ;
        void AddRelTorque(CVector const &);
        virtual void SetPostDisablePhysics();
        virtual void Remove();
        virtual Geom::CellAabb GetCollisionCellAabb() const ;
        virtual void RenderObstacleDebugInfo() const ;
        bool bIsStatic() const ;
        virtual void SetLinearVelocity(CVector const &);
        void AddForce(CVector const &);
        void AddForceAtRelPos(CVector const &,CVector const &);
        void AddImpulseAtPos(CVector const &,CVector const &);
        virtual void SetInvisible();
        static m3d::AIParam AIGetCurPos(Obj *);
        CVector GetPositionAtRelPoint(CVector) const ;
        PhysicObj(PhysicObjPrototypeInfo const &);
        bool bIsBodyDisabledGeomEnabled() const ;
        virtual void GetPropertiesIDs(std::set<int,std::less<int>,std::allocator<int> > &) const ;
        void DisablePhysicsWithAutoEnable();
        virtual void SetRotationSelf(Quaternion const &);
        virtual void SetPostPosition(CVector const &);
        virtual m3d::Class* GetClass() const ;
        CVector GetAngularVelocity() const ;
        virtual void RelinkGeomsToCollisionCells();
        void AddRelativeRotation(Quaternion const &);
        bool GetGeomEnabledBit() const ;
        virtual void LinkGeomsToCollisionCells();
        virtual CStr GetPropertyName(int) const ;
        virtual eGObjPropertySaveStatus GetPropertySaveStatus(int) const ;
        void ZeroEnabledCellsCount();
        void SetCorrectEnabledCellsCounter();
        virtual PhysicObjPrototypeInfo const * GetPrototypeInfo() const ;
        virtual CVector GetLinearVelocity() const ;
        static m3d::DbgCounter * __fastcall GetRelinksToCollisionCounter();
        virtual int GetPropertyId(char const *) const ;

    protected:
        void _SetSimpleCollision();
        void _AdjustMassCenter();
        SphereForIntersection * _GetLookSphere() const ;
        virtual void _SetRotationToGeoms(Quaternion const &);
        void _EnableIntersections(bool);
        void _SetBoundSphereRadius(float);
        void _SetBodyEnabledBit(bool);
        void _CreateSpace(bool);
        virtual bool _GetPropertyInternal(int,m3d::AIParam &) const ;
        virtual bool _GetPropertyDefaultInternal(int,m3d::AIParam &) const ;
        virtual void _UpdateOwnPhysics(float);
        virtual void _SetPositionToGeoms(CVector const &);
        virtual void _UnlinkBodyFromGeoms();
        void _SetStaticCollision();
        void _SetStatic();
        void _SetMassCenter(CVector const &);
        virtual ~PhysicObj();
        virtual void _InternalPostLoad();
        virtual void _InternalCreateVisualPart();
        void _SetGeomEnabledBit(bool);
        static void __fastcall RegisterProperty(char const *,int,eGObjPropertySaveStatus);
        bool _UpdateMustBeRelinked();
        virtual void _LinkBodyToGeoms();

    private:
        virtual m3d::Object * Clone();
        static m3d::Object * CreateObject();
        static void __fastcall _CommonBodyChangeEnabledStateCallback(dxBody *);

        static inline m3d::DbgCounter* m_countRelinksToCollisionCells = nullptr;

    public:
        RT_CLASS_DECLARE(PhysicObj);

    private:
        int m_postActionFlags;
        Quaternion m_postRotation;
        CVector m_postPosition;
        int m_physicBehaviorFlags;
        CVector m_massCenter;
        dxSpace *m_spaceId;
        bool m_bIsSpaceOwner;
        dBody *m_body;
        ref_ptr<Obstacle> m_intersectionObstacle;
        SphereForIntersection *m_lookSphere;
        Sphere *m_boundSphere;
        int m_physicState;
        int m_bIsUpdatingByODE;
        int m_enabledCellsCount;
        bool m_bBodyEnabledLastFrame;
        int m_skinNumber;
        float m_timeFromLastCollisionEffect;
    };
}
