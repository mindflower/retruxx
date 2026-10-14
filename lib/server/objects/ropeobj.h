#pragma once
#include "base/simplephysicobj.h"

namespace ai
{
    class RopeObjPrototypeInfo : public ai::SimplePhysicObjPrototypeInfo
    {
    public:
        RopeObjPrototypeInfo();
        virtual bool LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override;
        virtual ai::Obj* CreateTargetObject() const override;
        CStr m_brokenModel;
    };

    class RopeObj : public ai::SimplePhysicObj
    {
        friend class RopeObjPrototypeInfo;
    protected:
        virtual  ~RopeObj() override;

    private:
        RopeObj(const ai::RopeObjPrototypeInfo& prototypeInfo);
        virtual m3d::Object* Clone() override;
        static m3d::Object* __fastcall CreateObject();

    public:
        static m3d::Class* __fastcall GetBaseClass();
        virtual m3d::Class* GetClass() const override;
        static m3d::Class m_classRopeObj;
        virtual const ai::RopeObjPrototypeInfo* GetPrototypeInfo() const override;
        virtual void LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override;
        virtual void SaveToXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const override;
        virtual CVector GetGeometricCenter() const override;
        void SetPosFromTies();
        m3d::SgNode* GetTiedSgNodeNearPos(const CVector& tiePos, bool simpleSearch) const;
        void HangToPosts();
        void BreakOff(int breakObj, bool createJointForBreakObj);

        enum HangPoints
        {
            HP_BEGIN = 0,
            HP_END = 1,
            HP_NUM_POINTS = 2,
        };

        struct Post
        {
            CStr m_serverObjName;
            retruxx::vector<CStr, retruxx::allocator<CStr> > m_nodesNamesHierarchy;
            m3d::SgNode* m_postNode;
            ai::Obj* m_postObj;
            CStr m_lpName;
            CVector m_postTiePos;
            Post(const ai::RopeObj::Post& __that);
            Post();
        };

    public:
        const ai::RopeObj::Post& GetPost(ai::RopeObj::HangPoints);
        void SetPost(ai::RopeObj::HangPoints hp, const ai::RopeObj::Post& post);

    protected:
        virtual void _InternalCreateVisualPart() override;

    private:
        ai::RopeObj::Post m_dummyPost;
        retruxx::vector<ai::RopeObj::Post, retruxx::allocator<ai::RopeObj::Post> > m_posts;
        float m_strech;
        retruxx::vector<CVector, retruxx::allocator<CVector> > m_tiePoses;
        retruxx::vector<CVector, retruxx::allocator<CVector> > m_tiedObjPoses;
        retruxx::vector<int, retruxx::allocator<int> > m_tieObjects;
    };
}
