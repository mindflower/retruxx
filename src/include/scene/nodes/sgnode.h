#pragma once
#include <math/matrix.h>
#include <math/point2d.h>
#include <math/quaternion.h>
#include <math/vector.h>
#include <core/clazz.h>
#include <core/stringm3d.h>
#include <math/aabb.h>

class Obb;

namespace m3d
{
    class GraphItemsForSgNode;
    class DataServer;
    class SceneGraph;

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

    class SgNode : public Object
    {
    public:
        enum Ritual
        {
            RITUAL_NONE = 0x0,
            RITUAL_THINK_NODE = 0x1,
            RITUAL_REGISTERED_NODE = 0x2,
            RITUAL_THINK_AND_REGISTERED_NODE = 0x3,
        };

    public:
        static Object* CreateObject();
        static Class* GetBaseClass();

    public:
        static Class m_classSgNode;

    public:
        Obb GetObb() const;
        CVector const& GetOrigin() const;
        CMatrix const& GetCurrentMatrix() const;
        int GetPrevThinkTime() const;
        int SetScale(CVector const&);
        void SetTransparencyType(TransparencyType);
        int GetServerItemProperty(unsigned int, void*) const;
        bool IsXFormUpdateNeeded() const;
        virtual int WriteToXmlNode(cmn::XmlFile*, cmn::XmlNode*);
        CVector const& GetOriginWorldAbs() const;
        void SetPrevThinkTime(int);
        CVector const& GetScale() const;
        virtual Class* GetClass() const;
        int SetServerItemProperty(unsigned int, void*) const;
        virtual int Think(int, int);
        int GetTtl() const;
        virtual void CanBeFree();
        CVector const& GetOriginWorldAbsForSphere() const;
        virtual int RemoveChild(Object*);
        int GetNextThinkTime() const;
        virtual int Render(SgNodeRenderFlags, void*, int, int);
        Quaternion const& GetRotation() const;
        virtual int GetProperty(unsigned int, void*) const;
        SceneGraph* GetGraph();
        int GetServerHandle() const;
        virtual bool IsFree() const;
        virtual DataServer* GetServer() const;
        Aabb GetAabb() const;
        virtual int AddChild(Object*);
        virtual Object* Clone();
        virtual int ReadFromXmlNodeAfterAdd(cmn::XmlFile*, cmn::XmlNode*);
        TransparencyParams& GetTransparencyParams();
        void SetBoundingBoxDirty();
        void RemoveImmediateAfterParent(bool);
        unsigned int GetContourColor();
        Quaternion const& GetRotationWorldAbs() const;
        int SetRotation(Quaternion const&);
        virtual int SetProperty(unsigned int, void*);
        float GetBoundingRadius() const;
        void GetVisCellBounds(PointBase<int>&, PointBase<int>&) const;
        float GetContourWidth();
        int SetOriginAbs(CVector const&);
        virtual int UpdateXForm(bool, bool);
        bool VisCellBoundsChanged() const;
        virtual int ReadFromXmlNode(cmn::XmlFile*, cmn::XmlNode*);
        virtual int GetPropertiesList(std::set<unsigned int, std::less<unsigned int>, std::allocator<unsigned int> >&) const;
        Aabb GetOwnAabb() const;
        virtual float IntersectRay(CVector const&, CVector const&, SgNode*&, Class*);
        virtual void Restart();

    protected:
        CMatrix MatrixFromFlags(SgNodeRenderFlags, void*) const;
        virtual void UpdateOwnBoundingBox();
        void RitualInConstructor(Ritual);
        virtual ~SgNode();
        SgNode(SgNode const&);
        SgNode();
        void RitualInDestructor();
        void InternalInit();

    private:
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
        TransparencyType m_transparencyType;
        TransparencyParams m_transparencyParams;
        bool m_isWaitingForRender;
        int m_srvId;
        int m_frameVisible;
        int m_frameVisible2;
        float m_onScreenSize;
        bool m_isRootNode;
        int m_properties[3];
        CStr m_debugMsg;
        unsigned int m_props[10];
        GraphItemsForSgNode* m_forGraph;
        int m_predictIdx;
        Ritual m_initedWithRitual;
    };
}
