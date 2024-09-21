#pragma once
#include <math/quaternion.h>
#include <math/vector.h>

namespace m3d
{
    namespace cmn
    {
        struct XmlNode;
        class XmlFile;
    }
}

namespace ai
{
    class Vehicle;
    class PhysicObj;

    class ChaseMotionData
    {
    public:
        ChaseMotionData(int, int);
        Vehicle* GetChaser() const;
        PhysicObj* GetTarget() const;

    private:
        int m_targetId;
        int m_chaserId;
        float m_chaseMotionTime;
        CVector m_initTargetPos;
        Quaternion m_initTargetRot;
        CVector m_initPos;
        Quaternion m_initRot;

    };
    class ChaseMotionTactics
    {
    public:
        ChaseMotionTactics(int,int);
        virtual void SaveToXML(m3d::cmn::XmlFile *,m3d::cmn::XmlNode *) const ;
        virtual ~ChaseMotionTactics();
        virtual void LoadFromXML(m3d::cmn::XmlFile *,m3d::cmn::XmlNode const *);
        ChaseMotionData const & GetChaseMotionData() const ;
        void SetTargetAndChaser(int,int);
        void Update(float);

    private:
        //ChaseMotionTactics_vtbl *__vftable /*VFT*/;
        ChaseMotionData m_chaseMotionData;
    };
}
