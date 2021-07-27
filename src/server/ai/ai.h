#pragma once
#include "decisionmatrix.h"
#include <vector>

namespace m3d
{
    namespace cmn
    {
        class XmlNode;
        class XmlFile;
    }

    class AIParam;
}

namespace ai
{
    class Obj;
    class AIMessage;
    class AIPassageState;

    class AI
    {
    public:
        void LoadAIFromXML(m3d::cmn::XmlFile *,m3d::cmn::XmlNode *);
        int GetCurState2Num();
        void CommandStackOpen();
        void CommandStackClose();
        m3d::AIParam GetMessage2Param(unsigned int);
        m3d::AIParam GetMessage1Param(unsigned int);
        void AIInit();
        AI();
        int GetCurState1Num();
        void PutMessage2(AIMessage const &);
        void PutMessage1(AIMessage const &);
        void SaveAIToXML(m3d::cmn::XmlFile *,m3d::cmn::XmlNode *) const ;
        void SetDecisionMatrix(int);
        void PutCommand(int,m3d::AIParam const &,m3d::AIParam const &,m3d::AIParam const &);
        void PutCommand(AIMessage const &);
        void SetCommand(int,m3d::AIParam const &,m3d::AIParam const &,m3d::AIParam const &);
        void AIUpdate(Obj *);
        void Dump();
        CStr const & GetCurState2Name();
        m3d::AIParam GetCmdParam(unsigned int);
        CStr ToStr();
        CStr const & GetCurState1Name();
        void InsCommand(int,m3d::AIParam const &,m3d::AIParam const &,m3d::AIParam const &);
        void SetState2Param(int,m3d::AIParam const &);
        void SetState1Param(int,m3d::AIParam const &);
        m3d::AIParam GetState2Param(unsigned int);
        m3d::AIParam GetState1Param(unsigned int);

    private:
        int _CurrentMessage1CommandNum();
        int _CurrentMessage2CommandNum();
        int _CurrentState1Num();
        int _CurrentState2Num();

    private:
        std::vector<AIPassageState> m_StateStack2;
        std::vector<AIPassageState> m_StateStack1;
        DecisionMatrix *m_pDM;
        bool m_fStateStack2Changed;
        std::vector<AIMessage> m_Messages2;
        std::vector<AIMessage> m_Messages1;
        std::vector<AIMessage> m_Commands;
        int m_numCurCommand;
        bool m_CommandProcessed;
        bool m_CommandStackOpen;
    };
}
