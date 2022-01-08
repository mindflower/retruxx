#pragma once
#include "obj.h"
#include "prototypeinfo.h"
#include <math/matrix.h>
#include <math/quaternion.h>
#include <math/vector.h>

namespace m3d
{
    class AnimInfo;
    class AnimatedModel;
}

struct dxJoint;

namespace ai
{
    class GeomObj;

    class GoDataForLoad
    {
    private:
        CVector pos;
        Quaternion rot;
        CVector linVel;
        CVector angVel;
    };

    class CompositeObjPrototypeInfo : public ai::PrototypeInfo
    {
    public:
        virtual bool LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);
        CompositeObjPrototypeInfo();
        virtual class ai::Obj* CreateTargetObject() const;
    };

    class CompositeObj :  public Obj
    {
    public:
        CompositeObj(CompositeObjPrototypeInfo const &);
        virtual void Update(float,unsigned int);
        void Disable(bool);
        virtual CVector GetPosition();
        virtual m3d::Class * GetClass() const ;
        virtual void SaveRuntimeValues(m3d::cmn::XmlFile *,m3d::cmn::XmlNode *) const ;
        virtual void RelinkSceneGraphNode();
        virtual void PostCollide();
        virtual Quaternion GetRotation();
        void SetDeadTimer(int,bool);
        void Init(CStr const &,CVector const &,Quaternion const &,float,m3d::SgNode *);
        static m3d::Class * GetBaseClass();
        virtual void RenderDebugInfo() const ;
        virtual void Remove();
        virtual CompositeObjPrototypeInfo const * GetPrototypeInfo() const ;
        virtual void LoadRuntimeValues(m3d::cmn::XmlFile *,m3d::cmn::XmlNode const *);
        void InitImpulses(CVector,float);

    protected:
        virtual ~CompositeObj();
        void CreateBreakableJoints();
        virtual void _InternalPostLoad();
        void RecalcBoundBox();
        void PutToGlobalSpace();
        void PutToTempSpace();
        void CheckDisablePhysics();

    private:
        static m3d::Object * CreateObject();
        virtual m3d::Object * Clone();

    private:
        std::vector<GeomObj *> m_Members;
        std::vector<CMatrix> m_boneToGeom;
        std::vector<unsigned int> m_bonesIndices;
        std::vector<dxJoint *> m_joints;
        std::vector<std::pair<int,int>> m_connections;
        CStr m_modelName;
        m3d::SgNode *m_node;
        m3d::AnimatedModel *m_Model;
        m3d::AnimInfo *m_Anim;
        float m_mass;
        CVector m_Position;
        CMatrix m_initialNodeTransform;
        bool m_enabled;
        bool m_deadTimerActive;
        float m_deadTimer;
        bool m_testVisibility;
        std::vector<GoDataForLoad> m_dataForLoad;
    };
}
