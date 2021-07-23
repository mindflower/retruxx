#pragma once
#include "obj.h"
#include "prototypeinfo.h"
#include <math/vector.h>

struct dxJoint;
struct dxSpace;
class CMatrix;

namespace m3d
{
    class AnimatedModel;
    class AnimInfo;
}

namespace ai
{
    class GeomObj;
    class GoDataForLoad;

    class ExternalJointInfo
    {
    public:
        ExternalJointInfo();

    private:
        CVector jointPos;
        CStr lpName;
        bool connectedToStatics;
    };

    class JointedObjPrototypeInfo : public PrototypeInfo
    {
    public:
        JointedObjPrototypeInfo();
        virtual bool LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);
        virtual ai::Obj* CreateTargetObject() const;
    };

    class JointedObj : public Obj
    {
    public:
        class SplineBones
        {
        private:
            std::vector<unsigned int> bones;
            float t;
        };

    public:
        void CalcSplineNeighbours();
        void Init(CStr const &,CVector const &,Quaternion const &,float,m3d::SgNode *,float);
        JointedObj(JointedObjPrototypeInfo const &);
        void AddExternalJoint(int,CVector,CStr);
        void SetDisableTimer(int);
        void SetDeadTimer(int,bool);
        virtual void SaveRuntimeValues(m3d::cmn::XmlFile *,m3d::cmn::XmlNode *) const ;
        virtual void Remove();
        virtual void RelinkSceneGraphNode();
        virtual m3d::Class * GetClass() const ;
        void SetAsRope(bool);
        virtual void RenderDebugInfo() const ;
        virtual JointedObjPrototypeInfo const * GetPrototypeInfo() const ;
        virtual void LoadRuntimeValues(m3d::cmn::XmlFile *,m3d::cmn::XmlNode const *);
        void InitImpulses(CVector,float);
        void Disable(bool);
        virtual void PostCollide();
        virtual void Update(float,unsigned int);
        static m3d::Class * __fastcall GetBaseClass();
        void ReattachExternalJoint(int);

    protected:
        virtual void _InternalPostLoad();
        virtual ~JointedObj();
        void CheckDisablePhysics();
        bool IsParent(int,int);
        void PutToNewSpace(dxSpace *);
        void RecalcBoundBox();

    private:
        virtual m3d::Object * Clone();
        static m3d::Object * __fastcall CreateObject();

    private:
        dxSpace *m_MembersSpace;
        std::vector<GeomObj *> m_Members;
        std::map<unsigned int,GeomObj *> m_ExtraMembers;
        std::vector<dxJoint *> m_Joints;
        std::vector<unsigned int> m_jointsIndices;
        std::vector<dxJoint *> m_externalJoints;
        std::vector<ExternalJointInfo> m_externalJointsInfo;
        bool m_onLoad;
        std::vector<CMatrix> m_JointToGeom;
        CStr m_modelName;
        m3d::SgNode *m_node;
        m3d::AnimatedModel *m_Model;
        m3d::AnimInfo *m_Anim;
        float m_mass;
        float m_strech;
        CVector m_Position;
        bool m_deadTimerActive;
        float m_deadTimer;
        bool m_testVisibility;
        bool m_enabled;
        bool m_disableTimerActive;
        float m_disableTimer;
        std::vector<GoDataForLoad> m_dataForLoad;
        std::map<unsigned int,GoDataForLoad> m_edataForLoad;
        bool m_asRope;
        std::map<int,SplineBones> m_splineNeighbours;
    };
}

namespace ai
{
}
