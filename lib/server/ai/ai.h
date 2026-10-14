#pragma once
#include "core/stringm3d.h"
#include "retruxx/common.h"

namespace m3d
{
    namespace cmn
    {
        struct XmlNode;
        class XmlFile;
    }

    class AIParam;
}

namespace ai
{
    class Obj;
    class AIMessage;
    class AIPassageState;
    class DecisionMatrix;

    class AI
    {
    public:
        AI(const ai::AI&);
        AI();
        void AIInit();
        const CStr& GetCurState2Name();
        const CStr& GetCurState1Name();
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
        void SetState2Param(int ParamNum, const m3d::AIParam& Param);
        void SetState1Param(int ParamNum, const m3d::AIParam& Param);
        void PutCommand(int Num, const m3d::AIParam& Param1, const m3d::AIParam& Param2, const m3d::AIParam& Param3);
        void PutCommand(const ai::AIMessage& command);
        void InsCommand(int Num, const m3d::AIParam& Param1, const m3d::AIParam& Param2, const m3d::AIParam& Param3);
        void SetCommand(int Num, const m3d::AIParam& Param1, const m3d::AIParam& Param2, const m3d::AIParam& Param3);
        void CommandStackOpen();
        void CommandStackClose();
        void PutMessage2(const ai::AIMessage& Message);
        void PutMessage1(const ai::AIMessage& Message);
        void LoadAIFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* OwnNode);
        void SaveAIToXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* OwnNode) const;
        void Dump();
        CStr ToStr();

    private:
        int _CurrentState2Num();
        int _CurrentState1Num();
        int _CurrentMessage2CommandNum();
        int _CurrentMessage1CommandNum();
        retruxx::vector<ai::AIPassageState, retruxx::allocator<ai::AIPassageState> > m_StateStack2;
        retruxx::vector<ai::AIPassageState, retruxx::allocator<ai::AIPassageState> > m_StateStack1;
        ai::DecisionMatrix* m_pDM;
        bool m_fStateStack2Changed;
        char Padding_177[3];
        retruxx::vector<ai::AIMessage, retruxx::allocator<ai::AIMessage> > m_Messages2;
        retruxx::vector<ai::AIMessage, retruxx::allocator<ai::AIMessage> > m_Messages1;
        retruxx::vector<ai::AIMessage, retruxx::allocator<ai::AIMessage> > m_Commands;
        int m_numCurCommand;
        bool m_CommandProcessed;
        bool m_CommandStackOpen;
    };
}
