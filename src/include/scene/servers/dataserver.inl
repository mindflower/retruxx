#pragma once

namespace m3d
{
    class SgSpriteNode :  public SgNode
    {
    public:
        static Object * __fastcall CreateObject();
        virtual Class * GetClass() const ;
        virtual int GetPropertiesList(class std::set<unsigned int,struct std::less<unsigned int>,class std::allocator<unsigned int> > &) const ;
        virtual int Render(SgNodeRenderFlags,void *,int,int);
        virtual DataServer * GetServer() const ;
        virtual Object * Clone();
        virtual int ReadFromXmlNode(cmn::XmlFile *,cmn::XmlNode *);
        virtual int SetProperty(unsigned int,void *);
        virtual int WriteToXmlNode(cmn::XmlFile *,cmn::XmlNode *);
        static Class * __fastcall GetBaseClass();
        virtual int GetProperty(unsigned int,void *) const ;
    protected:
        virtual ~SgSpriteNode();
        virtual void UpdateOwnBoundingBox();
        SgSpriteNode();
        SgSpriteNode(class SgSpriteNode const &);
    private:
        unsigned int m_props[6];
    };
}

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

namespace m3d
{
    class SgSoundSourceNode :  public SgNode
    {
    public:
        virtual void CanBeFree();
        virtual int ReadFromXmlNode(cmn::XmlFile *,cmn::XmlNode *);
        virtual DataServer * GetServer() const ;
        virtual int GetPropertiesList(class std::set<unsigned int,struct std::less<unsigned int>,class std::allocator<unsigned int> > &) const ;
        virtual Class * GetClass() const ;
        virtual int WriteToXmlNode(cmn::XmlFile *,cmn::XmlNode *);
        virtual float IntersectRay(struct CVector const &,struct CVector const &,SgNode * &,Class *);
        virtual int Render(SgNodeRenderFlags,void *,int,int);
        virtual Object * Clone();
        virtual int GetProperty(unsigned int,void *) const ;
        virtual int SetProperty(unsigned int,void *);
        virtual void Restart();
        static Object * __fastcall CreateObject();
        virtual bool IsFree() const ;
        static Class * __fastcall GetBaseClass();
    protected:
        virtual ~SgSoundSourceNode();
        SgSoundSourceNode();
        SgSoundSourceNode(class SgSoundSourceNode const &);
        virtual void UpdateOwnBoundingBox();
    private:
        bool _OnSoundStopped();
        int _InternalRender();
        unsigned int m_props[6];
        int m_currentSoundNum;
        int m_framesPassed;
    };
}

namespace m3d
{
    class SgAnimatedModelNode :  public SgNode
    {
    public:
        virtual int UpdateXForm(bool,bool);
        virtual int ReadFromXmlNode(cmn::XmlFile *,cmn::XmlNode *);
        virtual int ReadFromXmlNodeAfterAdd(cmn::XmlFile *,cmn::XmlNode *);
        virtual Object * Clone();
        virtual int Think(int,int);
        static Class * __fastcall GetBaseClass();
        virtual int WriteToXmlNode(cmn::XmlFile *,cmn::XmlNode *);
        static Object * __fastcall CreateObject();
        virtual int Render(SgNodeRenderFlags,void *,int,int);
        virtual int SetProperty(unsigned int,void *);
        virtual DataServer * GetServer() const ;
        virtual int GetPropertiesList(class std::set<unsigned int,struct std::less<unsigned int>,class std::allocator<unsigned int> > &) const ;
        virtual int GetProperty(unsigned int,void *) const ;
        virtual Class * GetClass() const ;
    protected:
        virtual ~SgAnimatedModelNode();
        virtual void UpdateOwnBoundingBox();
        SgAnimatedModelNode();
        SgAnimatedModelNode(class SgAnimatedModelNode const &);
    private:
        ActionType m_action;
        std::vector<enum ActionType> m_effectActions;
        std::set<int> *m_suppressedLPs;
        unsigned int m_SkinNumber;
        Configuration m_cfg;
        bool m_manualAnimControl;
        bool m_castShadow;
        int m_frameUpdated;
        int m_lastTimeUpdated;
        int m_skipFrames;
        bool m_passable;
        ai::Obstacle *m_obstacle;
        bool m_imposted;
    };
}

namespace m3d
{
    class SgPointLightSourceNode :  public SgNode
    {
    public:
        virtual Object * Clone();
        virtual DataServer * GetServer() const ;
        virtual int GetProperty(unsigned int,void *) const ;
        virtual int Render(SgNodeRenderFlags,void *,int,int);
        static Class * __fastcall GetBaseClass();
        virtual int SetProperty(unsigned int,void *);
        virtual int GetPropertiesList(class std::set<unsigned int,struct std::less<unsigned int>,class std::allocator<unsigned int> > &) const ;
        virtual Class * GetClass() const ;
        static Object * __fastcall CreateObject();
    protected:
        virtual ~SgPointLightSourceNode();
        SgPointLightSourceNode();
        SgPointLightSourceNode(class SgPointLightSourceNode const &);
        virtual void UpdateOwnBoundingBox();
    private:
        unsigned int m_props[3];
    };
}

namespace m3d
{
    class SgDecalsNode :  public SgNode
    {
    public:
        static Class * __fastcall GetBaseClass();
        virtual int GetProperty(unsigned int,void *) const ;
        unsigned int GetNumDecals();
        virtual int SetProperty(unsigned int,void *);
        DecalInfo const & GetDecal(int);
        virtual DataServer * GetServer() const ;
        virtual Object * Clone();
        virtual int Render(SgNodeRenderFlags,void *,int,int);
        static Object * __fastcall CreateObject();
        virtual int GetPropertiesList(class std::set<unsigned int,struct std::less<unsigned int>,class std::allocator<unsigned int> > &) const ;
        virtual Class * GetClass() const ;
    protected:
        virtual ~SgDecalsNode();
        SgDecalsNode(class SgDecalsNode const &);
        SgDecalsNode();
        virtual void UpdateOwnBoundingBox();
    private:
        DecalsList m_decals;
    };
}

namespace m3d
{
    class SgProjectorNode :  public SgNode
    {
    public:
        virtual Class * GetClass() const ;
        `public: virtual int Render(SgNodeRenderFlags,void *,int,int)'::`4'::__unnamed::__unnamed();
        virtual int SetProperty(unsigned int,void *);
        virtual DataServer * GetServer() const ;
        virtual int GetProperty(unsigned int,void *) const ;
        virtual int GetPropertiesList(class std::set<unsigned int,struct std::less<unsigned int>,class std::allocator<unsigned int> > &) const ;
        static Class * __fastcall GetBaseClass();
        static Object * __fastcall CreateObject();
        virtual Object * Clone();
    protected:
        virtual ~SgProjectorNode();
        virtual void UpdateOwnBoundingBox();
        SgProjectorNode(class SgProjectorNode const &);
        SgProjectorNode();
    private:
        unsigned int m_props[2];
    };
}

namespace m3d
{
    class DataServer
    {
    public:
        int GetItemByName(char const *,bool) const ;
        virtual int RenderShadowVolumesSet(SgNode * *,unsigned int);
        virtual int RenderNodeSet(SgNode * *,unsigned int,RenderNodeInfo);
        virtual void RenderTransparents(SgNode * *,unsigned int);
        virtual void PostLoad();
        class CStr const & GetNameByItem(int) const ;
        virtual bool ReportServerInfo(char const *);
        virtual int WriteItemToXmlNode(int,cmn::XmlFile *,cmn::XmlNode *);
        int WriteToXmlNode(cmn::XmlFile *,cmn::XmlNode *);
        virtual void UpdateItem(int,void *);
        virtual int Release();
        virtual int GenerateImpostorsIfNeeded();
        virtual int Init();
        virtual void Restore();
        virtual void UnregisterNode(SgNode *);
        virtual int ReadFromXmlNode(cmn::XmlFile *,cmn::XmlNode *);
        DataServer();
        virtual int SetItemProperty(int,int,void *);
        void GenerateItemsRemap();
        class CStr GetOriginalFileName(int);
        virtual ~DataServer();
        virtual void RegisterNode(SgNode *);
        virtual void Invalidate();
        virtual int GetItemProperty(int,int,void *);
        int GetNumItems() const ;
    protected:
        void SetError(class CStr const &);
        bool IsValid() const ;
        int ParseProto(char const *,enum Proto *,int *);
    private:
        std::copy_backward<Model *,Model *>(Model *,Model *,Model *);
        std::_Allocate<Model>(uint,Model *);
        std::fill<ServerItem *,ServerItem>(ServerItem *,ServerItem *,ServerItem const &);
        std::_Destroy<Model>(Model *);
        std::copy<Model *,Model *>(Model *,Model *,Model *);
        std::allocator<ServerItem>::allocator<ServerItem>();
        std::allocator<ServerItem>::allocator<ServerItem>();
        std::_Allocate<ServerItem>(uint,ServerItem *);
        std::_Ptr_cat<ServerItem *,ServerItem *>(ServerItem * &,ServerItem * &);
        std::copy_backward<ServerItem *,ServerItem *>(ServerItem *,ServerItem *,ServerItem *);
        std::_Construct<Model,Model>(Model *,Model const &);
        std::_Destroy<ServerItem>(ServerItem *);
        std::_Ptr_cat<Model *,Model *>(Model * &,Model * &);
        std::_Construct<ServerItem,ServerItem>(ServerItem *,ServerItem const &);
        std::allocator<Model>::allocator<Model>();
        std::allocator<Model>::allocator<Model>();
        std::_Copy_backward_opt<ServerItem *,ServerItem *>(ServerItem *,ServerItem *,ServerItem *,std::_Nonscalar_ptr_iterator_tag);
        std::_Copy_opt<Model *,Model *>(Model *,Model *,Model *,std::_Nonscalar_ptr_iterator_tag);
        std::_Copy_backward_opt<Model *,Model *>(Model *,Model *,Model *,std::_Nonscalar_ptr_iterator_tag);
        std::fill<Model *,Model>(Model *,Model *,Model const &);
        DataServer_vtbl *__vftable /*VFT*/;
        void *m_fnLoadCallbackData;
        void (__fastcall *m_fnLoadCallback)(int, void *);
        std::vector<DataServer::Model> m_models;
        std::map<CStr,int> m_shRemap;
        std::vector<DataServer::ServerItem> m_itemslist;
        CStr m_lastError;
        bool m_valid;
    };
}

namespace m3d
{
    class SgStaticModelNode :  public SgNode
    {
    public:
        virtual Class * GetClass() const ;
        virtual DataServer * GetServer() const ;
        virtual int GetPropertiesList(class std::set<unsigned int,struct std::less<unsigned int>,class std::allocator<unsigned int> > &) const ;
        static Object * __fastcall CreateObject();
        static Class * __fastcall GetBaseClass();
        virtual int Render(SgNodeRenderFlags,void *,int,int);
        virtual Object * Clone();
        virtual int GetProperty(unsigned int,void *) const ;
        virtual int SetProperty(unsigned int,void *);
    protected:
        SgStaticModelNode();
        SgStaticModelNode(class SgStaticModelNode const &);
        virtual ~SgStaticModelNode();
        virtual void UpdateOwnBoundingBox();
    private:
    };
}

namespace m3d
{
    class SgParticlesNode :  public SgNode
    {
    public:
        virtual int WriteToXmlNode(cmn::XmlFile *,cmn::XmlNode *);
        static Object * __fastcall CreateObject();
        static Class * __fastcall GetBaseClass();
        virtual int Think(int,int);
        virtual Object * Clone();
        virtual int ReadFromXmlNode(cmn::XmlFile *,cmn::XmlNode *);
        virtual Class * GetClass() const ;
        virtual bool IsFree() const ;
        virtual int GetProperty(unsigned int,void *) const ;
        virtual int GetPropertiesList(class std::set<unsigned int,struct std::less<unsigned int>,class std::allocator<unsigned int> > &) const ;
        virtual int SetProperty(unsigned int,void *);
        virtual void Restart();
        virtual DataServer * GetServer() const ;
        virtual int Render(SgNodeRenderFlags,void *,int,int);
        virtual void CanBeFree();
    protected:
        virtual ~SgParticlesNode();
        SgParticlesNode(class SgParticlesNode const &);
        SgParticlesNode();
        virtual void UpdateOwnBoundingBox();
    private:
        int m_lastTimeUpdated;
        int m_numMesh;
        unsigned int m_Parts0Times;
    };
}

namespace m3d
{
    class SgLinesNode :  public SgNode
    {
    public:
        `public: virtual int Render(SgNodeRenderFlags,void *,int,int)'::`4'::RenderInfo::RenderInfo();
        virtual int SetProperty(unsigned int,void *);
        virtual DataServer * GetServer() const ;
        virtual int GetProperty(unsigned int,void *) const ;
        virtual int GetPropertiesList(class std::set<unsigned int,struct std::less<unsigned int>,class std::allocator<unsigned int> > &) const ;
        virtual Class * GetClass() const ;
        virtual Object * Clone();
        static Class * __fastcall GetBaseClass();
        static Object * __fastcall CreateObject();
    protected:
        virtual ~SgLinesNode();
        SgLinesNode(class SgLinesNode const &);
        SgLinesNode();
        virtual void UpdateOwnBoundingBox();
    private:
        unsigned int m_props[2];
    };
}
