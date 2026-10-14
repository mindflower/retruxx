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
        CVector jointPos;
        CStr lpName;
        bool connectedToStatics;
        ExternalJointInfo(const ai::ExternalJointInfo& __that);
        ExternalJointInfo();
    };

    class JointedObjPrototypeInfo : public ai::PrototypeInfo
    {
    public:
        JointedObjPrototypeInfo();
        virtual bool LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override;
        virtual ai::Obj* CreateTargetObject() const override;
    };

    class JointedObj : public ai::Obj
    {
        // RopeObj::BreakOff marks the broken rope it creates as a rope.
        friend class RopeObj;
        // CreateTargetObject builds one of these.
        friend class JointedObjPrototypeInfo;

    protected:
        virtual  ~JointedObj() override;

    private:
        JointedObj(const ai::JointedObjPrototypeInfo& prototypeInfo);
        JointedObj(const ai::JointedObj&);
        virtual m3d::Object* Clone() override;
        static m3d::Object* __fastcall CreateObject();

    public:
        static m3d::Class* __fastcall GetBaseClass();
        virtual m3d::Class* GetClass() const override;
        static m3d::Class m_classJointedObj;
        virtual const ai::JointedObjPrototypeInfo* GetPrototypeInfo() const override;
        virtual void Remove() override;
        virtual void LoadRuntimeValues(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override;
        virtual void SaveRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const override;
        void Init(const CStr& modelName, const CVector& pos, const Quaternion& rot, float mass, m3d::SgNode* toAccept, float strechZ);
        virtual void Update(float elapsedTime, unsigned int workTime) override;
        virtual void RenderDebugInfo() const override;
        void Disable(bool disable);
        void InitImpulses(CVector causePos, float force);
        void SetDeadTimer(int resttime, bool testVisibility);
        void SetDisableTimer(int restTime);
        virtual void RelinkSceneGraphNode() override;
        void AddExternalJoint(int IdAttachTo, CVector attachPos, CStr lpName);
        void ReattachExternalJoint(int IdAttachTo);
        virtual void PostCollide() override;
        void SetAsRope(bool b);
        void CalcSplineNeighbours();

    protected:
        void PutToNewSpace(dxSpace* Parent);
        virtual void _InternalPostLoad() override;
        void RecalcBoundBox();
        bool IsParent(int i, int j);
        void CheckDisablePhysics();

        using GeomVector = retruxx::vector<ai::Geom*, retruxx::allocator<ai::Geom*> >;
        using GeomObjVector = retruxx::vector<ai::GeomObj*, retruxx::allocator<ai::GeomObj*> >;
        using UintGeomObjMap = retruxx::map<unsigned int, ai::GeomObj*, retruxx::less<unsigned int>, retruxx::allocator<retruxx::pair<unsigned int const, ai::GeomObj*> > >;
        using BonesVector = retruxx::vector<unsigned int, retruxx::allocator<unsigned int> >;

    protected:
        dxSpace* m_MembersSpace;
        retruxx::vector<ai::GeomObj*, retruxx::allocator<ai::GeomObj*> > m_Members;
        retruxx::map<unsigned int, ai::GeomObj*, retruxx::less<unsigned int>, retruxx::allocator<retruxx::pair<unsigned int const, ai::GeomObj*> > > m_ExtraMembers;
        retruxx::vector<dxJoint*, retruxx::allocator<dxJoint*> > m_Joints;
        retruxx::vector<unsigned int, retruxx::allocator<unsigned int> > m_jointsIndices;
        retruxx::vector<dxJoint*, retruxx::allocator<dxJoint*> > m_externalJoints;
        retruxx::vector<ai::ExternalJointInfo, retruxx::allocator<ai::ExternalJointInfo> > m_externalJointsInfo;
        bool m_onLoad;
        char Padding_102[3];
        retruxx::vector<CMatrix, retruxx::allocator<CMatrix> > m_JointToGeom;
        CStr m_modelName;
        m3d::SgNode* m_node;
        m3d::AnimatedModel* m_Model;
        m3d::AnimInfo* m_Anim;
        float m_mass;
        float m_strech;
        CVector m_Position;
        bool m_deadTimerActive;
        char Padding_103[3];
        float m_deadTimer;
        bool m_testVisibility;
        bool m_enabled;
        bool m_disableTimerActive;
        char Padding_104;
        float m_disableTimer;
        retruxx::vector<ai::GoDataForLoad, retruxx::allocator<ai::GoDataForLoad> > m_dataForLoad;
        retruxx::map<unsigned int, ai::GoDataForLoad, retruxx::less<unsigned int>, retruxx::allocator<retruxx::pair<unsigned int const, ai::GoDataForLoad> > > m_edataForLoad;

        struct SplineBones
        {
            retruxx::vector<unsigned int, retruxx::allocator<unsigned int> > bones;
            float t;
        };

    protected:
        bool m_asRope;
        char Padding_105[3];
        retruxx::map<int, ai::JointedObj::SplineBones, retruxx::less<int>, retruxx::allocator<retruxx::pair<int const, ai::JointedObj::SplineBones> > > m_splineNeighbours;
    };
}
