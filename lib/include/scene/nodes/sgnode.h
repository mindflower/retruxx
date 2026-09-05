#pragma once
#include <math/matrix.h>
#include <math/point2d.h>
#include <math/quaternion.h>
#include <math/vector.h>
#include <core/clazz.h>
#include <core/stringm3d.h>
#include <math/aabb.h>

class Obb;
class ComplexModelWnd;

namespace m3d
{
    class GraphItemsForSgNode;
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
        /* 0x0000 */ float value;
        /* 0x0004 */ float startDist;
        /* 0x0008 */ float objectWidth;
    }; /* size: 0x000c */

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

    protected:
        SgNode();
        SgNode(const m3d::SgNode& node);
        virtual  ~SgNode() override /* 0x00 */;

    public:
        virtual m3d::Object* Clone() override /* 0x04 */;
        static m3d::Object* CreateObject();
        static m3d::Class* GetBaseClass();
        virtual m3d::Class* GetClass() const override /* 0x34 */;
        static m3d::Class m_classSgNode;
        virtual int ReadFromXmlNode(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* node) override /* 0x08 */;
        virtual int ReadFromXmlNodeAfterAdd(m3d::cmn::XmlFile* file, m3d::cmn::XmlNode* node) override /* 0x0c */;
        virtual int WriteToXmlNode(m3d::cmn::XmlFile* file, m3d::cmn::XmlNode* writeTo) override /* 0x10 */;
        virtual int SetProperty(unsigned int propId, void* prop) override /* 0x14 */;
        virtual int GetProperty(unsigned int propId, void* prop) const override /* 0x18 */;
        virtual int GetPropertiesList(retruxx::set<unsigned int, retruxx::less<unsigned int>, retruxx::allocator<unsigned int> >& properties) const override /* 0x1c */;
        int SetServerItemProperty(unsigned int propId, void* property) const;
        int GetServerItemProperty(unsigned int propId, void* property) const;
        virtual int AddChild(m3d::Object* node) override /* 0x20 */;
        virtual int RemoveChild(m3d::Object* node) override /* 0x24 */;
        virtual m3d::DataServer* GetServer() const /* 0x3c */;
        int GetTtl() const;
        virtual bool IsFree() const /* 0x40 */;
        virtual void CanBeFree() /* 0x44 */;
        void RemoveImmediateAfterParent(bool YesOrNo);
        void SetBoundingBoxDirty();
        virtual void Restart() /* 0x48 */;
        void SetTransparencyType(m3d::TransparencyType tt);
        int GetNextThinkTime() const;
        int GetPrevThinkTime() const;
        void SetPrevThinkTime(int t);
        bool IsXFormUpdateNeeded() const;
        int SetOriginRel(const CVector&);
        int SetOriginAbs(const CVector& origin);
        int SetScale(const CVector& scale);
        const CVector& GetScale() const;
        int SetRotation(const Quaternion& quat);
        const Quaternion& GetRotation() const;
        const CVector& GetOriginWorldAbs() const;
        const Quaternion& GetRotationWorldAbs() const;
        const CVector& GetOriginWorldAbsForSphere() const;
        const CVector& GetOrigin() const;
        bool GetRelativeFlag() const;
        const CMatrix& GetCurrentMatrix() const;
        float GetBoundingRadius() const;
        Obb GetObb() const;
        Aabb GetAabb() const;
        Aabb GetOwnAabb() const;
        void GetVisCellBounds(PointBase<int>& p0, PointBase<int>& p1) const;
        bool VisCellBoundsChanged() const;
        m3d::TransparencyParams& GetTransparencyParams();
        virtual float IntersectRay(const CVector& v0, const CVector& dir, m3d::SgNode*& hitNode, m3d::Class* wantClass) /* 0x4c */;
        virtual int UpdateXForm(bool onlyVis, bool parentDirty) /* 0x50 */;
        virtual int Think(int dt, int curTime) /* 0x54 */;
        virtual int Render(m3d::SgNodeRenderFlags flags, void* data, int dt, int curTime) /* 0x58 */;
        m3d::SceneGraph* GetGraph();
        int GetServerHandle() const;
        unsigned int GetContourColor();
        float GetContourWidth();

    protected:
        virtual void UpdateOwnBoundingBox() /* 0x5c */;

        /* 0x0034 */ int m_nextThinkTime;
        /* 0x0038 */ int m_prevThinkTime;
        /* 0x003c */ int m_ttl;
        /* 0x0040 */ CMatrix m_ownXForm;
        /* 0x0080 */ CMatrix m_currentXForm;
        /* 0x00c0 */ CVector m_origin;
        /* 0x00cc */ CVector m_scaling;
        /* 0x00d8 */ Quaternion m_rotation;
        /* 0x00e8 */ CVector m_currentWorldOrigin;
        /* 0x00f4 */ Quaternion m_currentWorldRotation;
        /* 0x0104 */ CVector m_originWorldAbsForSphere;
        /* 0x0110 */ float m_boundingRadius;
        /* 0x0114 */ Aabb m_boundingBox;
        /* 0x012c */ Aabb m_ownBoundingBox;
        /* 0x0144 */ bool m_isOriginRelative;
        /* 0x0145 */ char Padding_110[3];
        /* 0x0148 */ unsigned int m_isXFormDirty;
        /* 0x014c */ bool m_isOwnBoundingBoxDirty;
        /* 0x014d */ bool m_removeImmediateAfterParent;
        /* 0x014e */ bool m_isRemoveIfFree;
        /* 0x014f */ bool m_isInRemoveIfFree;
        /* 0x0150 */ bool m_isContoured;
        /* 0x0151 */ char Padding_111[3];
        /* 0x0154 */ unsigned int m_contourColor;
        /* 0x0158 */ float m_contourWidth;
        /* 0x015c */ int m_frameTransparent;
        /* 0x0160 */ m3d::TransparencyType m_transparencyType;
        /* 0x0164 */ m3d::TransparencyParams m_transparencyParams;
        /* 0x0170 */ bool m_isWaitingForRender;
        /* 0x0171 */ char Padding_112[3];
        /* 0x0174 */ int m_srvId;

    public:
        /* 0x0178 */ int m_frameVisible;
        /* 0x017c */ int m_frameVisible2;
        /* 0x0180 */ float m_onScreenSize;
        /* 0x0184 */ bool m_isRootNode;
        /* 0x0185 */ char Padding_113[3];

    protected:
        /* 0x0188 */ int m_properties[3];
        /* 0x0194 */ CStr m_debugMsg;
        /* 0x01a0 */ unsigned int m_props[10];
        /* 0x01c8 */ m3d::GraphItemsForSgNode* m_forGraph;

    public:
        /* 0x01cc */ int m_predictIdx;

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
        /* 0x01d0 */ m3d::SgNode::Ritual m_initedWithRitual;

    protected:
        void InternalInit();
    }; /* size: 0x01d4 */

    static_assert(sizeof(SgNode) == 0x01d4);
}
