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
        /* 0x0000 */ int m_targetId;
        /* 0x0004 */ int m_chaserId;
        /* 0x0008 */ float m_chaseMotionTime;
        /* 0x000c */ CVector m_initTargetPos;
        /* 0x0018 */ Quaternion m_initTargetRot;
        /* 0x0028 */ CVector m_initPos;
        /* 0x0034 */ Quaternion m_initRot;
    }; /* size: 0x0044 */

    class ChaseMotionTactics
    {
    public:
        ChaseMotionTactics(ai::ChaseMotionTactics const&);
        ChaseMotionTactics(int targetId, int chaserId);
        virtual ~ChaseMotionTactics() /* 0x00 */;
        void SetTargetAndChaser(int targetId, int chaserId);
        void Update(float elapsedTime);
        virtual CVector EvaluateCurrentChasePoint(float) = 0 /* 0x04 */;
        virtual void LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode) /* 0x08 */;
        virtual void SaveToXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const /* 0x0c */;
        ai::ChaseMotionData const& GetChaseMotionData() const;

    private:
        /* 0x0004 */ ai::ChaseMotionData m_chaseMotionData;
    }; /* size: 0x0048 */

    class ChaseMotionTacticsCheater : public ai::ChaseMotionTactics
    {
    public:
        ChaseMotionTacticsCheater(ai::ChaseMotionTacticsCheater const&);
        ChaseMotionTacticsCheater(int targetId, int ChaseMotionrId);
        virtual CVector EvaluateCurrentChasePoint(float elapsedTime) override /* 0x04 */;

        enum CheaterState
        {
            csNormal = 0,
            csOut = 1,
            csIn = 2,
        };

    private:
        /* 0x0048 */ ai::ChaseMotionTacticsCheater::CheaterState m_CheaterState;
        /* 0x004c */ float m_elapsedTime;
        /* 0x0050 */ float m_Period;

    public:
        ai::ChaseMotionTacticsCheater::CheaterState getCheaterState() const;
    }; /* size: 0x0054 */

    class ChaseMotionTacticsCircle : public ai::ChaseMotionTactics
    {
    public:
        ChaseMotionTacticsCircle(ai::ChaseMotionTacticsCircle const&);
        ChaseMotionTacticsCircle(int targetId, int chaserId, float radius);
        virtual CVector EvaluateCurrentChasePoint(float elapsedTime) override /* 0x04 */;

    private:
        /* 0x0048 */ float m_radius;
        /* 0x004c */ float m_clockwiseSign;
    }; /* size: 0x0050 */

    class ChaseMotionTacticsFigureOfEight : public ai::ChaseMotionTactics
    {
    public:
        ChaseMotionTacticsFigureOfEight(ai::ChaseMotionTacticsFigureOfEight const&);
        ChaseMotionTacticsFigureOfEight(int targetId, int chaserId, float radius);
        virtual CVector EvaluateCurrentChasePoint(float elapsedTime) override /* 0x04 */;

    private:
        /* 0x0048 */ float m_radius;
        /* 0x004c */ float m_clockwiseSign;
        /* 0x0050 */ float m_angle;
    }; /* size: 0x0054 */

    static_assert(sizeof(ChaseMotionData) == 0x44);
    static_assert(sizeof(ChaseMotionTactics) == 0x48);
    static_assert(sizeof(ChaseMotionTacticsCheater) == 0x54);
    static_assert(sizeof(ChaseMotionTacticsCircle) == 0x50);
    static_assert(sizeof(ChaseMotionTacticsFigureOfEight) == 0x54);
}  // namespace ai
