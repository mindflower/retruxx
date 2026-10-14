#pragma once
#include <cstdint>
#include "geoms/geom.h"
#include <ode/mass.h>
#include <server/objects/base/obj.h>
#include <server/objects/base/prototypeinfo.h>

#include "physichelpers.h"
#include "geoms/geomtransform.h"
#include "server/objects/base/physicobj.h"

struct dxSpace;

namespace m3d
{
    class DbgCounter;
    class AnimatedModel;
}

enum ActionType : int32_t;

namespace ai
{
    class PhysicBodyPrototypeInfo : public ai::PrototypeInfo
    {
    public:
        int m_engineModelId;
        CStr m_engineModelName;
        float m_massValue;
        retruxx::vector<ai::CollisionInfo, retruxx::allocator<ai::CollisionInfo> > m_collisionInfos;
        bool m_bCollisionTrimeshAllowed;
        PhysicBodyPrototypeInfo();
        virtual bool LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override;
        virtual void RefreshFromXml(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override;
    };

    class PhysicBody : public Obj
    {
        // ComplexPhysicObj::_TearOffPart hands the part's collision infos to its splinter.
        friend class ComplexPhysicObj;

    protected:
        PhysicBody(const ai::PhysicBodyPrototypeInfo& prototypeInfo);
        PhysicBody();

    public:
        virtual ~PhysicBody() override;
        static m3d::Class* __fastcall GetBaseClass();
        virtual m3d::Class* GetRtClass() const override;
        static m3d::Class m_classPhysicBody;

        using GeomTransformPtrVector = retruxx::vector<ai::GeomTransform*, retruxx::allocator<ai::GeomTransform*> >;

    public:
        CStr m_modelname;
        dMass m_mass;
        retruxx::vector<ai::GeomTransform*, retruxx::allocator<ai::GeomTransform*> > m_pGeoms;
        float m_mU;
        m3d::SgNode* m_Node;

        void _ClearGeoms();
        virtual void SetBelong(int newBelong) override;
        void SetEffectActions(retruxx::vector<enum ActionType, retruxx::allocator<enum ActionType> >& Actions);
        virtual bool CanChildBeAdded(m3d::Class* pClass) const override;
        virtual void LoadRuntimeValues(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override;
        virtual void SaveRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const override;
        ai::PhysicObj* GetOwner() const;
        void SetModelName(const CStr& newModelName);
        void SetModelNameUnsafe(const CStr& newModelName);
        void SetSgNode(m3d::SgNode* node);
        CVector GetPosition() const;
        Quaternion GetRotation() const;
        CVector GetDirection() const;
        void SetPosition(const CVector& vec);
        void SetRotation(const Quaternion* q);
        void SetRotation(const Quaternion& q);
        virtual void RelinkToSpace(dxSpace* newSpace);
        void SetOwnerBodyToGeoms();
        void UnlinkGeomsFromBody();
        virtual void DisableGeometry();
        virtual void EnableGeometry();
        virtual float GetMass() const;
        CVector GetNodeRelativePosition() const;
        void SetNodeRelativePosition(const CVector& pos);
        CVector GetNodeAbsolutePosition() const;
        void SetNodeAbsolutePosition(const CVector& pos);
        Quaternion GetNodeRelativeRotation() const;
        void SetNodeRelativeRotation(const Quaternion& q);
        Quaternion GetNodeAbsoluteRotation() const;
        void SetNodeAbsoluteRotation(const Quaternion& q);
        CVector GetNodeRelativeDirection() const;
        CVector GetNodeAbsoluteDirection() const;
        void SetNodeAbsoluteDirection(const CVector& direction);
        virtual void TransferPhysicParamsToSceneGraphNode() override;
        virtual void RelinkSceneGraphNode() override;
        virtual void LinkGeomToCollisionCells();
        virtual void UnlinkGeomFromCollisionCells();
        virtual void RelinkGeomToCollisionCells();
        virtual void ReceiveNodesToLink(retruxx::list<m3d::SgNode*, retruxx::allocator<m3d::SgNode*> >& nodes) const override;
        virtual ai::Geom::CellAabb GetCollisionCellAabb() const;
        virtual void RenderDebugInfo() const override;
        bool bNeedToRelinkNode() const;
        void SetSkin(int skin);
        int GetSkin() const;
        virtual void SetNodeAction(int action, bool forceRestartAction);
        virtual void SetNextForAnimation(int action, int nextAction);
        int GetNodeAnimAction() const;
        virtual void SetNodeAnimAction(int action, bool forceRestartAction);
        int GetNodeEffectAction() const;
        virtual void SetNodeEffectAction(int action);
        int GetNodeRealAnimAction() const;
        int GetNodeCfgNum() const;
        virtual void SetNodeCfgNum(int cfgNum);
        void SetAnimationStopped(bool bStopped);
        virtual void SetPassedToAnotherMapStatus() override;
        static void __fastcall Registration();

        static m3d::SgNode* __fastcall CreateNode(const CStr& modelname, int action, const CVector& scale, ai::PhysicBody* owner, bool addToRoot);
        static m3d::SgNode* __fastcall CreateAnimatedNode(const CStr&, int, const CVector&, ai::PhysicBody*, bool);
        static m3d::SgNode* __fastcall CreateEffectNode(const CStr& modelname, const CVector& pos, const Quaternion& rot, bool bInsertInRemoveIfFree, float scale);
        static m3d::DbgCounter* __fastcall GetCountNodeRelinks();

        int GetOwnerId() const;
        virtual void SetOwner(ai::PhysicObj* owner);
        virtual void DumpPhysicInfo(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const;
        virtual m3d::AnimatedModel* GetModel() const;
        virtual void SetVisible() override;
        virtual void SetInvisible() override;
        virtual void GetGeoms(retruxx::vector<ai::Geom*, retruxx::allocator<ai::Geom*> >& geoms) const;
        virtual unsigned int GetNumGeoms() const;
        virtual ai::Geom* GetGeom(unsigned int n) const;
        const retruxx::vector<ai::CollisionInfo, retruxx::allocator<ai::CollisionInfo> >& GetCollisionInfo() const;
        void ChangePhysicBodyByCollisionInfo(const retruxx::vector<ai::CollisionInfo, retruxx::allocator<ai::CollisionInfo> >& collisionInfos);
        virtual void ApplyCurrentModelCollision();
        int GetNodeRealAction() const;
        void UpdateGeomsByCollisionInfo(const retruxx::vector<ai::CollisionInfo, retruxx::allocator<ai::CollisionInfo> >& collisionInfos);
        void SetCollisionTrimeshAllowed(bool bCollisionTrimeshAllowed);

    protected:
        int m_cfgNum;
        retruxx::vector<ai::CollisionInfo, retruxx::allocator<ai::CollisionInfo> > m_collisionInfos;
        bool m_bCollisionTrimeshAllowed;

        virtual void _InternalCreateVisualPart() override;
        void _SetScenegraphNode(const CVector& pos, const Quaternion& rot);
        int _GetCurAnimationFrame() const;
        int _GetNodeRealAnimAction() const;

    private:
        ai::PhysicObj* m_ownerPhysicObj;
        bool m_bNeedToRelinkNode;

        static inline m3d::DbgCounter* m_countNodeRelinks = nullptr;

        int m_animAction;
        int m_effectAction;
        bool m_bAnimationIsStopped;
        int m_loadedAnimTime;

        void _DeleteNode();
        void _ApplyCurrentModelName();
    };
}
