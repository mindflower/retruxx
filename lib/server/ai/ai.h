#pragma once
#include "core/stringm3d.h"
#include "retruxx/common.h"

namespace m3d
{
    namespace cmn
    {
        struct XmlNode;
        class XmlFile;
    }  // namespace cmn

    class AIParam;
}  // namespace m3d

namespace ai
{
    class Obj;
    class AIMessage;
    class AIPassageState;
    class DecisionMatrix;

    class AI
    {
    public:
        AI(ai::AI const&);
        AI();
        void AIInit();
        CStr const& GetCurState2Name();
        CStr const& GetCurState1Name();
        int GetCurState2Num();
        int GetCurState1Num();
        m3d::AIParam GetCmdParam(unsigned int paramNum);
        m3d::AIParam GetState1Param(unsigned int paramNum);
        m3d::AIParam GetState2Param(unsigned int paramNum);
        m3d::AIParam GetMessage1Param(unsigned int paramNum);
        m3d::AIParam GetMessage2Param(unsigned int paramNum);
        void SetDecisionMatrix(int MatrixNum);
        ai::DecisionMatrix* GetDecisionMatrixPtr();
        void AIUpdate(ai::Obj* pObj);
        void SetState2Param(int ParamNum, m3d::AIParam const& Param);
        void SetState1Param(int ParamNum, m3d::AIParam const& Param);
        void PutCommand(int Num, m3d::AIParam const& Param1, m3d::AIParam const& Param2, m3d::AIParam const& Param3);
        void PutCommand(ai::AIMessage const& command);
        void InsCommand(int Num, m3d::AIParam const& Param1, m3d::AIParam const& Param2, m3d::AIParam const& Param3);
        void SetCommand(int Num, m3d::AIParam const& Param1, m3d::AIParam const& Param2, m3d::AIParam const& Param3);
        void CommandStackOpen();
        void CommandStackClose();
        void PutMessage2(ai::AIMessage const& Message);
        void PutMessage1(ai::AIMessage const& Message);
        void LoadAIFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* OwnNode);
        void SaveAIToXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* OwnNode) const;
        void Dump();
        CStr ToStr();

    private:
        int _CurrentState2Num();
        int _CurrentState1Num();
        int _CurrentMessage2CommandNum();
        int _CurrentMessage1CommandNum();
        retruxx::vector<ai::AIPassageState, retruxx::allocator<ai::AIPassageState>> m_StateStack2;
        retruxx::vector<ai::AIPassageState, retruxx::allocator<ai::AIPassageState>> m_StateStack1;
        ai::DecisionMatrix* m_pDM;
        bool m_fStateStack2Changed;
        retruxx::vector<ai::AIMessage, retruxx::allocator<ai::AIMessage>> m_Messages2;
        retruxx::vector<ai::AIMessage, retruxx::allocator<ai::AIMessage>> m_Messages1;
        retruxx::vector<ai::AIMessage, retruxx::allocator<ai::AIMessage>> m_Commands;
        int m_numCurCommand;
        bool m_CommandProcessed;
        bool m_CommandStackOpen;
    };
}  // namespace ai
