#pragma once
#include <math/quaternion.h>
#include <math/vector.h>

namespace m3d
{
    namespace cmn
    {
        struct XmlNode;
        class XmlFile;
    }  // namespace cmn
}  // namespace m3d

namespace ai
{
    class Vehicle;
    class PhysicObj;

    struct ChaseMotionData
    {
        ChaseMotionData(ai::ChaseMotionData const&);
        ChaseMotionData(int targetId, int chaserId);
        ai::PhysicObj* GetTarget() const;
        ai::Vehicle* GetChaser() const;
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
        ChaseMotionTactics(ai::ChaseMotionTactics const&);
        ChaseMotionTactics(int targetId, int chaserId);
        virtual ~ChaseMotionTactics();
        void SetTargetAndChaser(int targetId, int chaserId);
        void Update(float elapsedTime);
        virtual CVector EvaluateCurrentChasePoint(float) = 0;
        virtual void LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode);
        virtual void SaveToXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const;
        ai::ChaseMotionData const& GetChaseMotionData() const;

    private:
        ai::ChaseMotionData m_chaseMotionData;
    };

    class ChaseMotionTacticsCheater : public ai::ChaseMotionTactics
    {
    public:
        ChaseMotionTacticsCheater(ai::ChaseMotionTacticsCheater const&);
        ChaseMotionTacticsCheater(int targetId, int ChaseMotionrId);
        virtual CVector EvaluateCurrentChasePoint(float elapsedTime) override;

        enum CheaterState
        {
            csNormal = 0,
            csOut = 1,
            csIn = 2,
        };

    private:
        ai::ChaseMotionTacticsCheater::CheaterState m_CheaterState;
        float m_elapsedTime;
        float m_Period;

    public:
        ai::ChaseMotionTacticsCheater::CheaterState getCheaterState() const;
    };

    class ChaseMotionTacticsCircle : public ai::ChaseMotionTactics
    {
    public:
        ChaseMotionTacticsCircle(ai::ChaseMotionTacticsCircle const&);
        ChaseMotionTacticsCircle(int targetId, int chaserId, float radius);
        virtual CVector EvaluateCurrentChasePoint(float elapsedTime) override;

    private:
        float m_radius;
        float m_clockwiseSign;
    };

    class ChaseMotionTacticsFigureOfEight : public ai::ChaseMotionTactics
    {
    public:
        ChaseMotionTacticsFigureOfEight(ai::ChaseMotionTacticsFigureOfEight const&);
        ChaseMotionTacticsFigureOfEight(int targetId, int chaserId, float radius);
        virtual CVector EvaluateCurrentChasePoint(float elapsedTime) override;

    private:
        float m_radius;
        float m_clockwiseSign;
        float m_angle;
    };
}  // namespace ai
