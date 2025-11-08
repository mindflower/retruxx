#pragma once
#include "obj.h"
#include "prototypeinfo.h"
#include <math/vector.h>

struct dxJoint;
struct dxSpace;
struct CMatrix;

namespace m3d
{
    class AnimatedModel;
    class AnimInfo;
}

namespace ai
{
    class Geom;
    class GeomObj;
    class GoDataForLoad;

    struct ExternalJointInfo
    {
        /* 0x0000 */ CVector jointPos;
        /* 0x000c */ CStr lpName;
        /* 0x0018 */ bool connectedToStatics;
        ExternalJointInfo(const ai::ExternalJointInfo& __that);
        ExternalJointInfo();
    }; /* size: 0x001c */

    static_assert(sizeof(ExternalJointInfo) == 0x001c);

    class JointedObjPrototypeInfo : public ai::PrototypeInfo
    {
    public:
        JointedObjPrototypeInfo();
        virtual bool LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override /* 0x04 */;
        virtual ai::Obj* CreateTargetObject() const override /* 0x10 */;
    }; /* size: 0x0040 */

    static_assert(sizeof(JointedObjPrototypeInfo) == 0x0040);

    class JointedObj : public ai::Obj
    {
    protected:
        virtual  ~JointedObj() override /* 0x00 */;

    private:
        JointedObj(const ai::JointedObjPrototypeInfo& prototypeInfo);
        JointedObj(const ai::JointedObj&);
        virtual m3d::Object* Clone() override /* 0x00 */;
        static m3d::Object* __fastcall CreateObject();

    public:
        static m3d::Class* __fastcall GetBaseClass();
        virtual m3d::Class* GetClass() const override /* 0x00 */;
        static m3d::Class m_classJointedObj;
        virtual const ai::JointedObjPrototypeInfo* GetPrototypeInfo() const override /* 0x4c */;
        virtual void Remove() override /* 0x54 */;
        virtual void LoadRuntimeValues(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override /* 0xb0 */;
        virtual void SaveRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const override /* 0xb8 */;
        void Init(const CStr& modelName, const CVector& pos, const Quaternion& rot, float mass, m3d::SgNode* toAccept, float strechZ);
        virtual void Update(float elapsedTime, unsigned int workTime) override /* 0x80 */;
        virtual void RenderDebugInfo() const override /* 0xe4 */;
        void Disable(bool disable);
        void InitImpulses(CVector causePos, float force);
        void SetDeadTimer(int resttime, bool testVisibility);
        void SetDisableTimer(int restTime);
        virtual void RelinkSceneGraphNode() override /* 0xe0 */;
        void AddExternalJoint(int IdAttachTo, CVector attachPos, CStr lpName);
        void ReattachExternalJoint(int IdAttachTo);
        virtual void PostCollide() override /* 0x84 */;
        void SetAsRope(bool b);
        void CalcSplineNeighbours();

    protected:
        void PutToNewSpace(dxSpace* Parent);
        virtual void _InternalPostLoad() override /* 0xfc */;
        void RecalcBoundBox();
        bool IsParent(int i, int j);
        void CheckDisablePhysics();

        using GeomVector = retruxx::vector<ai::Geom*, retruxx::allocator<ai::Geom*> >;
        using GeomObjVector = retruxx::vector<ai::GeomObj*, retruxx::allocator<ai::GeomObj*> >;
        using UintGeomObjMap = retruxx::map<unsigned int, ai::GeomObj*, retruxx::less<unsigned int>, retruxx::allocator<retruxx::pair<unsigned int const, ai::GeomObj*> > >;
        using BonesVector = retruxx::vector<unsigned int, retruxx::allocator<unsigned int> >;

    protected:
        /* 0x00c0 */ dxSpace* m_MembersSpace;
        /* 0x00c4 */ retruxx::vector<ai::GeomObj*, retruxx::allocator<ai::GeomObj*> > m_Members;
        /* 0x00d4 */ retruxx::map<unsigned int, ai::GeomObj*, retruxx::less<unsigned int>, retruxx::allocator<retruxx::pair<unsigned int const, ai::GeomObj*> > > m_ExtraMembers;
        /* 0x00e0 */ retruxx::vector<dxJoint*, retruxx::allocator<dxJoint*> > m_Joints;
        /* 0x00f0 */ retruxx::vector<unsigned int, retruxx::allocator<unsigned int> > m_jointsIndices;
        /* 0x0100 */ retruxx::vector<dxJoint*, retruxx::allocator<dxJoint*> > m_externalJoints;
        /* 0x0110 */ retruxx::vector<ai::ExternalJointInfo, retruxx::allocator<ai::ExternalJointInfo> > m_externalJointsInfo;
        /* 0x0120 */ bool m_onLoad;
        /* 0x0121 */ char Padding_102[3];
        /* 0x0124 */ retruxx::vector<CMatrix, retruxx::allocator<CMatrix> > m_JointToGeom;
        /* 0x0134 */ CStr m_modelName;
        /* 0x0140 */ m3d::SgNode* m_node;
        /* 0x0144 */ m3d::AnimatedModel* m_Model;
        /* 0x0148 */ m3d::AnimInfo* m_Anim;
        /* 0x014c */ float m_mass;
        /* 0x0150 */ float m_strech;
        /* 0x0154 */ CVector m_Position;
        /* 0x0160 */ bool m_deadTimerActive;
        /* 0x0161 */ char Padding_103[3];
        /* 0x0164 */ float m_deadTimer;
        /* 0x0168 */ bool m_testVisibility;
        /* 0x0169 */ bool m_enabled;
        /* 0x016a */ bool m_disableTimerActive;
        /* 0x016b */ char Padding_104;
        /* 0x016c */ float m_disableTimer;
        /* 0x0170 */ retruxx::vector<ai::GoDataForLoad, retruxx::allocator<ai::GoDataForLoad> > m_dataForLoad;
        /* 0x0180 */ retruxx::map<unsigned int, ai::GoDataForLoad, retruxx::less<unsigned int>, retruxx::allocator<retruxx::pair<unsigned int const, ai::GoDataForLoad> > > m_edataForLoad;

        struct SplineBones
        {
            /* 0x0000 */ retruxx::vector<unsigned int, retruxx::allocator<unsigned int> > bones;
            /* 0x0010 */ float t;
        }; /* size: 0x0014 */

    protected:
        /* 0x018c */ bool m_asRope;
        /* 0x018d */ char Padding_105[3];
        /* 0x0190 */ retruxx::map<int, ai::JointedObj::SplineBones, retruxx::less<int>, retruxx::allocator<retruxx::pair<int const, ai::JointedObj::SplineBones> > > m_splineNeighbours;
    }; /* size: 0x019c */

    static_assert(sizeof(JointedObj) == 0x019c);
}
