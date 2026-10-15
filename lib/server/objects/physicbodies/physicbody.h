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
}  // namespace m3d

enum ActionType : int32_t;

namespace ai
{
    class PhysicBodyPrototypeInfo : public ai::PrototypeInfo
    {
    public:
        int m_engineModelId;
        CStr m_engineModelName;
        float m_massValue;
        retruxx::vector<ai::CollisionInfo, retruxx::allocator<ai::CollisionInfo>> m_collisionInfos;
        bool m_bCollisionTrimeshAllowed;
        PhysicBodyPrototypeInfo();
        virtual bool LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode) override;
        virtual void RefreshFromXml(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode) override;
    };

    class PhysicBody : public Obj
    {
        // ComplexPhysicObj::_TearOffPart hands the part's collision infos to its splinter.
        friend class ComplexPhysicObj;

    protected:
        PhysicBody(ai::PhysicBodyPrototypeInfo const& prototypeInfo);
        PhysicBody();

    public:
        virtual ~PhysicBody() override;
        static m3d::Class* GetBaseClass();
        virtual m3d::Class* GetRtClass() const override;
        static m3d::Class m_classPhysicBody;

        using GeomTransformPtrVector = retruxx::vector<ai::GeomTransform*, retruxx::allocator<ai::GeomTransform*>>;

    public:
        CStr m_modelname;
        dMass m_mass;
        retruxx::vector<ai::GeomTransform*, retruxx::allocator<ai::GeomTransform*>> m_pGeoms;
        float m_mU;
        m3d::SgNode* m_Node;

        void _ClearGeoms();
        virtual void SetBelong(int newBelong) override;
        void SetEffectActions(retruxx::vector<enum ActionType, retruxx::allocator<enum ActionType>>& Actions);
        virtual bool CanChildBeAdded(m3d::Class* pClass) const override;
        virtual void LoadRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode) override;
        virtual void SaveRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const override;
        ai::PhysicObj* GetOwner() const;
        void SetModelName(CStr const& newModelName);
        void SetModelNameUnsafe(CStr const& newModelName);
        void SetSgNode(m3d::SgNode* node);
        CVector GetPosition() const;
        Quaternion GetRotation() const;
        CVector GetDirection() const;
        void SetPosition(CVector const& vec);
        void SetRotation(Quaternion const* q);
        void SetRotation(Quaternion const& q);
        virtual void RelinkToSpace(dxSpace* newSpace);
        void SetOwnerBodyToGeoms();
        void UnlinkGeomsFromBody();
        virtual void DisableGeometry();
        virtual void EnableGeometry();
        virtual float GetMass() const;
        CVector GetNodeRelativePosition() const;
        void SetNodeRelativePosition(CVector const& pos);
        CVector GetNodeAbsolutePosition() const;
        void SetNodeAbsolutePosition(CVector const& pos);
        Quaternion GetNodeRelativeRotation() const;
        void SetNodeRelativeRotation(Quaternion const& q);
        Quaternion GetNodeAbsoluteRotation() const;
        void SetNodeAbsoluteRotation(Quaternion const& q);
        CVector GetNodeRelativeDirection() const;
        CVector GetNodeAbsoluteDirection() const;
        void SetNodeAbsoluteDirection(CVector const& direction);
        virtual void TransferPhysicParamsToSceneGraphNode() override;
        virtual void RelinkSceneGraphNode() override;
        virtual void LinkGeomToCollisionCells();
        virtual void UnlinkGeomFromCollisionCells();
        virtual void RelinkGeomToCollisionCells();
        virtual void ReceiveNodesToLink(
            retruxx::list<m3d::SgNode*, retruxx::allocator<m3d::SgNode*>>& nodes) const override;
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
        static void Registration();

        static m3d::SgNode*
            CreateNode(CStr const& modelname, int action, CVector const& scale, ai::PhysicBody* owner, bool addToRoot);
        static m3d::SgNode* CreateAnimatedNode(CStr const&, int, CVector const&, ai::PhysicBody*, bool);
        static m3d::SgNode* CreateEffectNode(
            CStr const& modelname,
            CVector const& pos,
            Quaternion const& rot,
            bool bInsertInRemoveIfFree,
            float scale);
        static m3d::DbgCounter* GetCountNodeRelinks();

        int GetOwnerId() const;
        virtual void SetOwner(ai::PhysicObj* owner);
        virtual void DumpPhysicInfo(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const;
        virtual m3d::AnimatedModel* GetModel() const;
        virtual void SetVisible() override;
        virtual void SetInvisible() override;
        virtual void GetGeoms(retruxx::vector<ai::Geom*, retruxx::allocator<ai::Geom*>>& geoms) const;
        virtual unsigned int GetNumGeoms() const;
        virtual ai::Geom* GetGeom(unsigned int n) const;
        retruxx::vector<ai::CollisionInfo, retruxx::allocator<ai::CollisionInfo>> const& GetCollisionInfo() const;
        void ChangePhysicBodyByCollisionInfo(
            retruxx::vector<ai::CollisionInfo, retruxx::allocator<ai::CollisionInfo>> const& collisionInfos);
        virtual void ApplyCurrentModelCollision();
        int GetNodeRealAction() const;
        void UpdateGeomsByCollisionInfo(
            retruxx::vector<ai::CollisionInfo, retruxx::allocator<ai::CollisionInfo>> const& collisionInfos);
        void SetCollisionTrimeshAllowed(bool bCollisionTrimeshAllowed);

    protected:
        int m_cfgNum;
        retruxx::vector<ai::CollisionInfo, retruxx::allocator<ai::CollisionInfo>> m_collisionInfos;
        bool m_bCollisionTrimeshAllowed;

        virtual void _InternalCreateVisualPart() override;
        void _SetScenegraphNode(CVector const& pos, Quaternion const& rot);
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
}  // namespace ai
