#pragma once
#include "base/simplephysicobj.h"

namespace ai
{
    class RopeObjPrototypeInfo : public ai::SimplePhysicObjPrototypeInfo
    {
    public:
        RopeObjPrototypeInfo();
        virtual bool LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override /* 0x04 */;
        virtual ai::Obj* CreateTargetObject() const override /* 0x00 */;
        CStr m_brokenModel;
    }; /* size: 0x008c */

    class RopeObj : public ai::SimplePhysicObj
    {
    protected:
        virtual  ~RopeObj() override /* 0x00 */;

    private:
        RopeObj(const ai::RopeObjPrototypeInfo& prototypeInfo);
        virtual m3d::Object* Clone() override /* 0x00 */;
        static m3d::Object* __fastcall CreateObject();

    public:
        static m3d::Class* __fastcall GetBaseClass();
        virtual m3d::Class* GetClass() const override /* 0x00 */;
        static m3d::Class m_classRopeObj;
        virtual const ai::RopeObjPrototypeInfo* GetPrototypeInfo() const override /* 0x00 */;
        virtual void LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override /* 0x00 */;
        virtual void SaveToXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const override /* 0x00 */;
        virtual CVector GetGeometricCenter() const override /* 0x00 */;
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
            /* 0x0000 */ CStr m_serverObjName;
            /* 0x000c */ retruxx::vector<CStr, retruxx::allocator<CStr> > m_nodesNamesHierarchy;
            /* 0x001c */ m3d::SgNode* m_postNode;
            /* 0x0020 */ ai::Obj* m_postObj;
            /* 0x0024 */ CStr m_lpName;
            /* 0x0030 */ CVector m_postTiePos;
            Post(const ai::RopeObj::Post& __that);
            Post();
        }; /* size: 0x003c */

    public:
        const ai::RopeObj::Post& GetPost(ai::RopeObj::HangPoints);
        void SetPost(ai::RopeObj::HangPoints hp, const ai::RopeObj::Post& post);

    protected:
        virtual void _InternalCreateVisualPart() override /* 0x00 */;

    private:
        ai::RopeObj::Post m_dummyPost;
        retruxx::vector<ai::RopeObj::Post, retruxx::allocator<ai::RopeObj::Post> > m_posts;
        /* 0x0190 */ float m_strech;
        retruxx::vector<CVector, retruxx::allocator<CVector> > m_tiePoses;
        retruxx::vector<CVector, retruxx::allocator<CVector> > m_tiedObjPoses;
        retruxx::vector<int, retruxx::allocator<int> > m_tieObjects;
    }; /* size: 0x01c4 */
}
