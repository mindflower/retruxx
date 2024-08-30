#pragma once
#include "geoms/geom.h"
#include <ode/mass.h>
#include <server/objects/base/obj.h>
#include <server/objects/base/prototypeinfo.h>

struct dxSpace;

namespace m3d
{
    class DbgCounter;
    class AnimatedModel;
}

enum ActionType;

namespace ai
{
    class GeomTransform;
    class CollisionInfo;
    class PhysicObj;

    class PhysicBodyPrototypeInfo : public PrototypeInfo
    {
    public:
        virtual void RefreshFromXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);
        virtual bool LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);
        PhysicBodyPrototypeInfo();

    private:
        int m_engineModelId;
        CStr m_engineModelName;
        float m_massValue;
        std::vector<CollisionInfo> m_collisionInfos;
        bool m_bCollisionTrimeshAllowed;
    };

    class PhysicBody :  public Obj
    {
    public:
        virtual void RelinkToSpace(dxSpace*);
        virtual void DisableGeometry();
        virtual void EnableGeometry();
        virtual float GetMass() const;
        virtual void LinkGeomToCollisionCells();
        virtual void UnlinkGeomFromCollisionCells();
        virtual void RelinkGeomToCollisionCells();
        virtual Geom::CellAabb GetCollisionCellAabb() const;
        virtual void SetNodeAction(int, bool);
        virtual void SetNextForAnimation(int, int);
        virtual void SetNodeAnimAction(int, bool);

    public:
        PhysicBody(PhysicBody const&);
        void SetEffectActions(std::vector<ActionType> &);
        void SetModelName(CStr const &);
        void SetModelNameUnsafe(CStr const &);
        int GetNodeAnimAction() const ;
        void ChangePhysicBodyByCollisionInfo(std::vector<CollisionInfo> const &);
        void SetSgNode(m3d::SgNode *);
        CVector GetPosition() const ;
        void SetPosition(CVector const &);
        void UpdateGeomsByCollisionInfo(std::vector<CollisionInfo> const &);
        virtual void GetGeoms(std::vector<Geom *,std::allocator<Geom *> > &) const ;
        static m3d::SgNode * __fastcall CreateNode(CStr const &,int,CVector const &,PhysicBody *,bool);
        PhysicBody(PhysicBodyPrototypeInfo const &);
        static m3d::Class * GetBaseClass();
        virtual void TransferPhysicParamsToSceneGraphNode();
        void _ClearGeoms();
        CVector GetDirection() const ;
        virtual unsigned int GetNumGeoms() const ;
        virtual void LoadRuntimeValues(m3d::cmn::XmlFile *,m3d::cmn::XmlNode const *);
        static m3d::SgNode * __fastcall CreateEffectNode(CStr const &,CVector const &,Quaternion const &,bool,float);
        Quaternion GetRotation() const ;
        void SetRotation(Quaternion const *);
        void SetRotation(Quaternion const &);
        void UnlinkGeomsFromBody();
        void SetAnimationStopped(bool);
        static m3d::DbgCounter * __fastcall GetCountNodeRelinks();
        virtual Geom * GetGeom(unsigned int) const ;
        virtual void RelinkSceneGraphNode();
        virtual void SetNodeEffectAction(int);
        virtual void DumpPhysicInfo(m3d::cmn::XmlFile *,m3d::cmn::XmlNode *) const ;
        int GetNodeEffectAction() const ;
        std::vector<CollisionInfo,std::allocator<CollisionInfo> > const & GetCollisionInfo() const ;
        void SetNodeAbsolutePosition(CVector const &);
        CVector GetNodeAbsolutePosition() const ;
        virtual ~PhysicBody();
        virtual void SetBelong(int);
        virtual void SaveRuntimeValues(m3d::cmn::XmlFile *,m3d::cmn::XmlNode *) const ;
        int GetSkin() const ;
        void SetSkin(int);
        int GetNodeCfgNum() const ;
        virtual void RenderDebugInfo() const ;
        virtual void SetNodeCfgNum(int);
        virtual void SetPassedToAnotherMapStatus();
        virtual void ApplyCurrentModelCollision();
        CVector GetNodeAbsoluteDirection() const ;
        void SetNodeAbsoluteDirection(CVector const &);
        virtual m3d::Class * GetRtClass() const ;
        void SetOwnerBodyToGeoms();
        static void __fastcall Registration();
        int GetNodeRealAnimAction() const ;
        void SetCollisionTrimeshAllowed(bool);
        Quaternion GetNodeRelativeRotation() const ;
        void SetNodeRelativeRotation(Quaternion const &);
        int GetNodeRealAction() const ;
        bool bNeedToRelinkNode() const ;
        virtual void SetVisible();
        virtual void SetOwner(PhysicObj *);
        PhysicObj * GetOwner() const ;
        virtual void SetInvisible();
        virtual void ReceiveNodesToLink(std::list<m3d::SgNode *,std::allocator<m3d::SgNode *> > &) const ;
        void SetNodeAbsoluteRotation(Quaternion const &);
        Quaternion GetNodeAbsoluteRotation() const ;
        virtual bool CanChildBeAdded(m3d::Class *) const ;
        CVector GetNodeRelativeDirection() const ;
        virtual m3d::AnimatedModel * GetModel() const ;
        CVector GetNodeRelativePosition() const ;
        void SetNodeRelativePosition(CVector const &);
        int GetOwnerId() const ;

    protected:
        int _GetCurAnimationFrame() const ;
        void _SetScenegraphNode(CVector const &,Quaternion const &);
        PhysicBody();
        int _GetNodeRealAnimAction() const ;
        virtual void _InternalCreateVisualPart();

    private:
        void _ApplyCurrentModelName();
        void _DeleteNode();

    public:
        RT_CLASS_DECLARE(PhysicBody);

        static inline m3d::DbgCounter* m_countNodeRelinks = nullptr;

    private:
        CStr m_modelname;
        dMass m_mass;
        std::vector<GeomTransform *> m_pGeoms;
        float m_mU;
        m3d::SgNode *m_Node;
        int m_cfgNum;
        std::vector<CollisionInfo> m_collisionInfos;
        bool m_bCollisionTrimeshAllowed;
        PhysicObj *m_ownerPhysicObj;
        bool m_bNeedToRelinkNode;
        int m_animAction;
        int m_effectAction;
        bool m_bAnimationIsStopped;
        int m_loadedAnimTime;
    };
}
