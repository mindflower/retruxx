#pragma once
#include "base/simplephysicobj.h"

namespace ai
{
    class RopeObjPrototypeInfo : public SimplePhysicObjPrototypeInfo
    {
    public:
        RopeObjPrototypeInfo();
        virtual Obj* CreateTargetObject() const;
        virtual bool LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);

    private:
        CStr m_brokenModel;

    };

    class RopeObj :  public SimplePhysicObj
    {
    public:
        enum HangPoints
        {
            HP_BEGIN = 0x0,
            HP_END = 0x1,
            HP_NUM_POINTS = 0x2,
        };

        class Post
        {
        public:
            Post(Post const&);

        private:
            CStr m_serverObjName;
            std::vector<CStr> m_nodesNamesHierarchy;
            m3d::SgNode* m_postNode;
            Obj* m_postObj;
            CStr m_lpName;
            CVector m_postTiePos;
        };

    public:
        virtual void LoadFromXML(m3d::cmn::XmlFile *,m3d::cmn::XmlNode const *);
        void SetPosFromTies();
        void BreakOff(int,bool);
        void HangToPosts();
        void SetPost(HangPoints,Post const &);
        virtual m3d::Class * GetClass() const ;
        virtual RopeObjPrototypeInfo const * GetPrototypeInfo() const ;
        virtual CVector GetGeometricCenter() const ;
        static m3d::Class * __fastcall GetBaseClass();
        m3d::SgNode * GetTiedSgNodeNearPos(CVector const &,bool) const ;
        RopeObj(RopeObjPrototypeInfo const &);
        virtual void SaveToXML(m3d::cmn::XmlFile *,m3d::cmn::XmlNode *) const ;

    protected:
        virtual void _InternalCreateVisualPart();
        virtual ~RopeObj();

    private:
        static m3d::Object * __fastcall CreateObject();
        virtual m3d::Object * Clone();

    private:
        Post m_dummyPost;
        std::vector<Post> m_posts;
        float m_strech;
        std::vector<CVector> m_tiePoses;
        std::vector<CVector> m_tiedObjPoses;
        std::vector<int> m_tieObjects;
    };
}
