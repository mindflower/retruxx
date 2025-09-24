#pragma once
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

enum ActionType;

namespace ai
{
    class PhysicBodyPrototypeInfo : public ai::PrototypeInfo
    {
    public:
        /* 0x0040 */ int m_engineModelId;
        /* 0x0044 */ CStr m_engineModelName;
        /* 0x0050 */ float m_massValue;
        /* 0x0054 */ retruxx::vector<ai::CollisionInfo, retruxx::allocator<ai::CollisionInfo> > m_collisionInfos;
        /* 0x0064 */ bool m_bCollisionTrimeshAllowed;
        PhysicBodyPrototypeInfo();
        virtual bool LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override /* 0x04 */;
        virtual void RefreshFromXml(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override /* 0x0c */;
    }; /* size: 0x0068 */

    static_assert(sizeof(PhysicBodyPrototypeInfo) == 0x0068);

    class PhysicBody : public Obj
    {
    protected:
        PhysicBody(const ai::PhysicBody&);
        PhysicBody(const ai::PhysicBodyPrototypeInfo& prototypeInfo);
        PhysicBody();

    public:
        virtual ~PhysicBody() override /* 0x00 */;
        static m3d::Class* __fastcall GetBaseClass();
        virtual m3d::Class* GetRtClass() const override /* 0x00 */;
        static m3d::Class m_classPhysicBody;

        using GeomTransformPtrVector = retruxx::vector<ai::GeomTransform*, retruxx::allocator<ai::GeomTransform*> >;

    public:
        /* 0x00c0 */ CStr m_modelname;
        /* 0x00cc */ dMass m_mass;
        /* 0x0110 */ retruxx::vector<ai::GeomTransform*, retruxx::allocator<ai::GeomTransform*> > m_pGeoms;
        /* 0x0120 */ float m_mU;
        /* 0x0124 */ m3d::SgNode* m_Node;

        void _ClearGeoms();
        virtual void SetBelong(int newBelong) override /* 0x00 */;
        void SetEffectActions(retruxx::vector<enum ActionType, retruxx::allocator<enum ActionType> >& Actions);
        virtual bool CanChildBeAdded(m3d::Class* pClass) const override /* 0x00 */;
        virtual void LoadRuntimeValues(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override /* 0x00 */;
        virtual void SaveRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const override /* 0x00 */;
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
        virtual void RelinkToSpace(dxSpace* newSpace) /* 0x110 */;
        void SetOwnerBodyToGeoms();
        void UnlinkGeomsFromBody();
        virtual void DisableGeometry() /* 0x114 */;
        virtual void EnableGeometry() /* 0x118 */;
        virtual float GetMass() const /* 0x11c */;
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
        virtual void TransferPhysicParamsToSceneGraphNode() override /* 0x00 */;
        virtual void RelinkSceneGraphNode() override /* 0x00 */;
        virtual void LinkGeomToCollisionCells() /* 0x120 */;
        virtual void UnlinkGeomFromCollisionCells() /* 0x124 */;
        virtual void RelinkGeomToCollisionCells() /* 0x128 */;
        virtual void ReceiveNodesToLink(retruxx::list<m3d::SgNode*, retruxx::allocator<m3d::SgNode*> >& nodes) const override /* 0x00 */;
        virtual ai::Geom::CellAabb GetCollisionCellAabb() const /* 0x12c */;
        virtual void RenderDebugInfo() const override /* 0x00 */;
        bool bNeedToRelinkNode() const;
        void SetSkin(int skin);
        int GetSkin() const;
        virtual void SetNodeAction(int action, bool forceRestartAction) /* 0x130 */;
        virtual void SetNextForAnimation(int action, int nextAction) /* 0x134 */;
        int GetNodeAnimAction() const;
        virtual void SetNodeAnimAction(int action, bool forceRestartAction) /* 0x138 */;
        int GetNodeEffectAction() const;
        virtual void SetNodeEffectAction(int action) /* 0x13c */;
        int GetNodeRealAnimAction() const;
        int GetNodeCfgNum() const;
        virtual void SetNodeCfgNum(int cfgNum) /* 0x140 */;
        void SetAnimationStopped(bool bStopped);
        virtual void SetPassedToAnotherMapStatus() override /* 0x00 */;
        static void __fastcall Registration();

        static m3d::SgNode* __fastcall CreateNode(const CStr& modelname, int action, const CVector& scale, ai::PhysicBody* owner, bool addToRoot);
        static m3d::SgNode* __fastcall CreateAnimatedNode(const CStr&, int, const CVector&, ai::PhysicBody*, bool);
        static m3d::SgNode* __fastcall CreateEffectNode(const CStr& modelname, const CVector& pos, const Quaternion& rot, bool bInsertInRemoveIfFree, float scale);
        static m3d::DbgCounter* __fastcall GetCountNodeRelinks();

        int GetOwnerId() const;
        virtual void SetOwner(ai::PhysicObj* owner) /* 0x144 */;
        virtual void DumpPhysicInfo(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const /* 0x148 */;
        virtual m3d::AnimatedModel* GetModel() const /* 0x14c */;
        virtual void SetVisible() override /* 0x00 */;
        virtual void SetInvisible() override /* 0x00 */;
        virtual void GetGeoms(retruxx::vector<ai::Geom*, retruxx::allocator<ai::Geom*> >& geoms) const /* 0x150 */;
        virtual unsigned int GetNumGeoms() const /* 0x154 */;
        virtual ai::Geom* GetGeom(unsigned int n) const /* 0x158 */;
        const retruxx::vector<ai::CollisionInfo, retruxx::allocator<ai::CollisionInfo> >& GetCollisionInfo() const;
        void ChangePhysicBodyByCollisionInfo(const retruxx::vector<ai::CollisionInfo, retruxx::allocator<ai::CollisionInfo> >& collisionInfos);
        virtual void ApplyCurrentModelCollision() /* 0x15c */;
        int GetNodeRealAction() const;
        void UpdateGeomsByCollisionInfo(const retruxx::vector<ai::CollisionInfo, retruxx::allocator<ai::CollisionInfo> >& collisionInfos);
        void SetCollisionTrimeshAllowed(bool bCollisionTrimeshAllowed);

    protected:
        /* 0x0128 */ int m_cfgNum;
        /* 0x012c */ retruxx::vector<ai::CollisionInfo, retruxx::allocator<ai::CollisionInfo> > m_collisionInfos;
        /* 0x013c */ bool m_bCollisionTrimeshAllowed;

        virtual void _InternalCreateVisualPart() override /* 0x00 */;
        void _SetScenegraphNode(const CVector& pos, const Quaternion& rot);
        int _GetCurAnimationFrame() const;
        int _GetNodeRealAnimAction() const;
        /* 0x013d */ char Padding_31[3];

    private:
        /* 0x0140 */ ai::PhysicObj* m_ownerPhysicObj;
        /* 0x0144 */ bool m_bNeedToRelinkNode;

        static inline m3d::DbgCounter* m_countNodeRelinks = nullptr;

        /* 0x0145 */ char Padding_32[3];
        /* 0x0148 */ int m_animAction;
        /* 0x014c */ int m_effectAction;
        /* 0x0150 */ bool m_bAnimationIsStopped;
        /* 0x0151 */ char Padding_33[3];
        /* 0x0154 */ int m_loadedAnimTime;

        void _DeleteNode();
        void _ApplyCurrentModelName();
    }; /* size: 0x0158 */

    static_assert(sizeof(PhysicBody) == 0x0158);
}
