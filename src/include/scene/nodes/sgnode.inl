#pragma once

namespace m3d
{
    class SgNode :  public Object
    {
    public:
        class Obb GetObb() const ;
        struct CVector const & GetOrigin() const ;
        struct CMatrix const & GetCurrentMatrix() const ;
        int GetPrevThinkTime() const ;
        int SetScale(struct CVector const &);
        void SetTransparencyType(TransparencyType);
        int GetServerItemProperty(unsigned int,void *) const ;
        bool IsXFormUpdateNeeded() const ;
        virtual int WriteToXmlNode(cmn::XmlFile *,cmn::XmlNode *);
        struct CVector const & GetOriginWorldAbs() const ;
        void SetPrevThinkTime(int);
        struct CVector const & GetScale() const ;
        virtual Class * GetClass() const ;
        int SetServerItemProperty(unsigned int,void *) const ;
        virtual int Think(int,int);
        int GetTtl() const ;
        virtual void CanBeFree();
        struct CVector const & GetOriginWorldAbsForSphere() const ;
        static Object * __fastcall CreateObject();
        virtual int RemoveChild(Object *);
        int GetNextThinkTime() const ;
        virtual int Render(enum SgNodeRenderFlags,void *,int,int);
        struct Quaternion const & GetRotation() const ;
        virtual int GetProperty(unsigned int,void *) const ;
        SceneGraph * GetGraph();
        int GetServerHandle() const ;
        virtual bool IsFree() const ;
        virtual DataServer * GetServer() const ;
        class Aabb GetAabb() const ;
        virtual int AddChild(Object *);
        virtual Object * Clone();
        virtual int ReadFromXmlNodeAfterAdd(cmn::XmlFile *,cmn::XmlNode *);
        TransparencyParams & GetTransparencyParams();
        void SetBoundingBoxDirty();
        static Class * __fastcall GetBaseClass();
        void RemoveImmediateAfterParent(bool);
        unsigned int GetContourColor();
        struct Quaternion const & GetRotationWorldAbs() const ;
        int SetRotation(struct Quaternion const &);
        virtual int SetProperty(unsigned int,void *);
        float GetBoundingRadius() const ;
        void GetVisCellBounds(class PointBase<int> &,class PointBase<int> &) const ;
        float GetContourWidth();
        int SetOriginAbs(struct CVector const &);
        virtual int UpdateXForm(bool,bool);
        bool VisCellBoundsChanged() const ;
        virtual int ReadFromXmlNode(cmn::XmlFile *,cmn::XmlNode *);
        virtual int GetPropertiesList(class std::set<unsigned int,struct std::less<unsigned int>,class std::allocator<unsigned int> > &) const ;
        class Aabb GetOwnAabb() const ;
        virtual float IntersectRay(struct CVector const &,struct CVector const &,class SgNode * &,Class *);
        virtual void Restart();
    protected:
        struct CMatrix MatrixFromFlags(enum SgNodeRenderFlags,void *) const ;
        virtual void UpdateOwnBoundingBox();
        void RitualInConstructor(enum Ritual);
        virtual ~SgNode();
        SgNode(class SgNode const &);
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
        GraphItemsForSgNode *m_forGraph;
        int m_predictIdx;
        SgNode::Ritual m_initedWithRitual;
    };
}
