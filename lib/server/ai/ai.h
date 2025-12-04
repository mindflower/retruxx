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
        /* 0x0000 */ retruxx::vector<ai::AIPassageState, retruxx::allocator<ai::AIPassageState> > m_StateStack2;
        /* 0x0010 */ retruxx::vector<ai::AIPassageState, retruxx::allocator<ai::AIPassageState> > m_StateStack1;
        /* 0x0020 */ ai::DecisionMatrix* m_pDM;
        /* 0x0024 */ bool m_fStateStack2Changed;
        /* 0x0025 */ char Padding_177[3];
        /* 0x0028 */ retruxx::vector<ai::AIMessage, retruxx::allocator<ai::AIMessage> > m_Messages2;
        /* 0x0038 */ retruxx::vector<ai::AIMessage, retruxx::allocator<ai::AIMessage> > m_Messages1;
        /* 0x0048 */ retruxx::vector<ai::AIMessage, retruxx::allocator<ai::AIMessage> > m_Commands;
        /* 0x0058 */ int m_numCurCommand;
        /* 0x005c */ bool m_CommandProcessed;
        /* 0x005d */ bool m_CommandStackOpen;
    }; /* size: 0x0060 */

    static_assert(sizeof(AI) == 0x0060);
}
