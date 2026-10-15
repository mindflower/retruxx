#pragma once
#include <cstdint>
#include <vector>
#include <math/matrix.h>
#include <math/point2d.h>
#include <math/quaternion.h>
#include <math/vector.h>
#include <core/clazz.h>
#include <core/stringm3d.h>
#include <math/aabb.h>

class Obb;
class ComplexModelWnd;

namespace ai
{
    class DummyObject;
    class RopeObj;
    class BossArm;
    class BossMetalArm;
    class CompositeObj;
    class JointedObj;
}  // namespace ai

namespace m3d
{
    struct GraphItemsForSgNode;
    class DataServer;
    class SceneGraph;

    namespace ui
    {
        class SgNodeArrayWnd;
    }

    enum TransparencyType
    {
        TT_NONE = 0x0,
        TT_VISIBILITY = 0x1,
        TT_PERMANENT = 0x2,
    };

    struct TransparencyParams
    {
        float value;
        float startDist;
        float objectWidth;
    };

    enum SgNodeRenderFlags
    {
        NRF_DEFAULT = 0x0,
        NRF_RMUL_BY_MAT = 0x1,
        NRF_LMUL_BY_MAT = 0x2,
        NRF_NO_LIGHTING = 0x4,
    };

    class SgNode : public m3d::Object
    {
        friend class SceneGraph;
        // SgNodeArrayWnd renders a set of nodes (and their children) into a
        // texture, and needs each node's current world transform to place the
        // children relative to the node it was handed. ComplexModelWnd, which
        // builds that set out of a vehicle or gun, reads the same transform plus
        // each node's own bounding box to frame the model.
        friend class m3d::ui::SgNodeArrayWnd;
        friend class ::ComplexModelWnd;
        // DummyObject::SetSgNodeAndCollision sizes a box collision from the node's own bounds.
        friend class ::ai::DummyObject;
        // ai::CompositeObj drives its node's box from the pieces the wreck broke into.
        friend class ::ai::CompositeObj;
        friend class ::ai::JointedObj;
        // RopeObj matches its tie positions against nodes' world origins and load points.
        friend class ::ai::RopeObj;
        friend class ::ai::BossArm;
        friend class ::ai::BossMetalArm;
        // A loadpoint node thinks again when its parent does.
        friend class SgLoadpointNode;
        // The shadow volumes are placed with each caster's world transform.
        friend class AnimatedModelsServer;

    protected:
        SgNode();
        SgNode(m3d::SgNode const& node);
        virtual ~SgNode() override;

    public:
        virtual m3d::Object* Clone() override;
        static m3d::Object* CreateObject();
        static m3d::Class* GetBaseClass();
        virtual m3d::Class* GetClass() const override;
        static m3d::Class m_classSgNode;
        virtual int ReadFromXmlNode(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* node) override;
        virtual int ReadFromXmlNodeAfterAdd(m3d::cmn::XmlFile* file, m3d::cmn::XmlNode* node) override;
        virtual int WriteToXmlNode(m3d::cmn::XmlFile* file, m3d::cmn::XmlNode* writeTo) override;
        virtual int SetProperty(unsigned int propId, void* prop) override;
        virtual int GetProperty(unsigned int propId, void* prop) const override;
        virtual int GetPropertiesList(
            retruxx::set<unsigned int, retruxx::less<unsigned int>, retruxx::allocator<unsigned int>>& properties)
            const override;
        int SetServerItemProperty(unsigned int propId, void* property) const;
        int GetServerItemProperty(unsigned int propId, void* property) const;
        virtual int AddChild(m3d::Object* node) override;
        virtual int RemoveChild(m3d::Object* node) override;
        virtual m3d::DataServer* GetServer() const;
        int GetTtl() const;
        virtual bool IsFree() const;
        virtual void CanBeFree();
        void RemoveImmediateAfterParent(bool YesOrNo);
        void SetBoundingBoxDirty();
        virtual void Restart();
        void SetTransparencyType(m3d::TransparencyType tt);
        int GetNextThinkTime() const;
        int GetPrevThinkTime() const;
        void SetPrevThinkTime(int t);
        bool IsXFormUpdateNeeded() const;
        int SetOriginRel(CVector const&);
        int SetOriginAbs(CVector const& origin);
        int SetScale(CVector const& scale);
        CVector const& GetScale() const;
        int SetRotation(Quaternion const& quat);
        Quaternion const& GetRotation() const;
        CVector const& GetOriginWorldAbs() const;
        Quaternion const& GetRotationWorldAbs() const;
        CVector const& GetOriginWorldAbsForSphere() const;
        CVector const& GetOrigin() const;
        bool GetRelativeFlag() const;
        CMatrix const& GetCurrentMatrix() const;
        float GetBoundingRadius() const;
        Obb GetObb() const;
        Aabb GetAabb() const;
        Aabb GetOwnAabb() const;
        void GetVisCellBounds(PointBase<int>& p0, PointBase<int>& p1) const;
        bool VisCellBoundsChanged() const;
        m3d::TransparencyParams& GetTransparencyParams();
        virtual float IntersectRay(CVector const& v0, CVector const& dir, m3d::SgNode*& hitNode, m3d::Class* wantClass);
        virtual int UpdateXForm(bool onlyVis, bool parentDirty);
        virtual int Think(int dt, int curTime);
        virtual int Render(m3d::SgNodeRenderFlags flags, void* data, int dt, int curTime);
        m3d::SceneGraph* GetGraph();
        int GetServerHandle() const;
        unsigned int GetContourColor();
        float GetContourWidth();

    protected:
        virtual void UpdateOwnBoundingBox();

        int m_nextThinkTime;
        int m_prevThinkTime;
        int m_ttl;
        CMatrix m_ownXForm;
        CMatrix m_currentXForm;
        CVector m_origin;
        CVector m_scaling;
        Quaternion m_rotation;
        CVector m_currentWorldOrigin;
        Quaternion m_currentWorldRotation;
        CVector m_originWorldAbsForSphere;
        float m_boundingRadius;
        Aabb m_boundingBox;
        Aabb m_ownBoundingBox;
        bool m_isOriginRelative;
        unsigned int m_isXFormDirty;
        bool m_isOwnBoundingBoxDirty;
        bool m_removeImmediateAfterParent;
        bool m_isRemoveIfFree;
        bool m_isInRemoveIfFree;
        bool m_isContoured;
        unsigned int m_contourColor;
        float m_contourWidth;
        int m_frameTransparent;
        m3d::TransparencyType m_transparencyType;
        m3d::TransparencyParams m_transparencyParams;
        bool m_isWaitingForRender;
        int m_srvId;

    public:
        int m_frameVisible;
        int m_frameVisible2;
        float m_onScreenSize;
        bool m_isRootNode;

    protected:
        // retruxx adaptation: the server slots PROP_SERVER_SLOT / PROP_SERVER_SLOT_1 hold the
        // per-node data pointers of the servers (particle info, animated model, effect list); the
        // shipped build kept them as ints, here each is a machine word moved as a pointer.
        std::uintptr_t m_properties[3];
        CStr m_debugMsg;
        // retruxx adaptation: the generic node properties PROP_NODE_FIRST..PROP_NODE_GAMEOBJECT. The
        // shipped build kept them as unsigned ints and stored pointers in four of them (physic body,
        // geom objects list, last parent, game object), so each slot is a machine word here;
        // GetProperty and SetProperty move the pointer-valued ones as pointers.
        std::uintptr_t m_props[10];
        m3d::GraphItemsForSgNode* m_forGraph;

    public:
        int m_predictIdx;

    protected:
        CMatrix MatrixFromFlags(m3d::SgNodeRenderFlags nrf, void* data) const;

        enum Ritual
        {
            RITUAL_NONE = 0,
            RITUAL_THINK_NODE = 1,
            RITUAL_REGISTERED_NODE = 2,
            RITUAL_THINK_AND_REGISTERED_NODE = 3,
        };

    protected:
        void RitualInConstructor(m3d::SgNode::Ritual rt);
        void RitualInDestructor();

    private:
        m3d::SgNode::Ritual m_initedWithRitual;

    protected:
        void InternalInit();
    };

    // Calls f(node) for every node below root (not root itself), as the engine's inlined subtree walks do: a stack
    // of nodes whose children are still to be visited, each popped node's children visited in sibling order.
    template<class F>
    void ForEachDescendant(SgNode* root, F&& f)
    {
        std::vector<SgNode*> stack{root};
        while (!stack.empty())
        {
            SgNode* const node = stack.back();
            stack.pop_back();
            for (auto* child = static_cast<SgNode*>(node->GetFirstChild()); child;
                 child = static_cast<SgNode*>(child->GetNextSibling()))
            {
                f(child);
                if (child->GetFirstChild())
                {
                    stack.push_back(child);
                }
            }
        }
    }
}  // namespace m3d
