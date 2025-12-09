#pragma once
#include "aipassagecommand.h"
#include <vector>
#include <core/clazz.h>

namespace ai
{
    class AIState;
    class AISignal;

    class DecisionMatrixElement
    {
    public:
        /* 0x0000 */ std::vector<ai::AIPassageCommand, std::allocator<ai::AIPassageCommand> > m_PassageCommands;
        /* 0x0010 */ unsigned short m_flags;
        DecisionMatrixElement();
        void Dump();
    }; /* size: 0x0014 */

    class DecisionMatrix : public m3d::Object
    {
    protected:
        DecisionMatrix();
        DecisionMatrix(ai::DecisionMatrix const& rhs);

    public:
        virtual ~DecisionMatrix() override /* 0x00 */;
        virtual m3d::Object* Clone() override /* 0x04 */;
        static m3d::Object* __fastcall CreateObject();
        static m3d::Class* __fastcall GetBaseClass();
        virtual m3d::Class* GetClass() const override /* 0x34 */;
        static m3d::Class m_classDecisionMatrix;
        void Create(int numStates, int numSignals);
        void AddState(char const* stateName, char const* functionName);
        void AddState(ai::AIState const& state);
        void AddSignal(char const* signalName, char const* externSignalName, char const* functionName);
        void AddSignal(ai::AISignal const& Signal);
        void AddCommand(char const* stateName, char const* signalName, char const* newStateName);
        void AddCommand(
            unsigned int StateID,
            unsigned int SignalID,
            unsigned int PassStateID,
            std::vector<ai::AIParamRef, std::allocator<ai::AIParamRef>> const& ParamRefList);

        void AddSublevel(char const* stateName, m3d::Object* pSubDM);
        void AddSublevel(unsigned int StateID, ai::DecisionMatrix* pSubDM);
        void SetSaveStackFlag(char const* stateName, char const* signalName);
        void SetSaveStackFlag(int stateNum, int signalNum);
        ai::DecisionMatrixElement const* UnsafeGetDecision(int stateNum, int signalNum) const;
        unsigned int UnsafeFirstDecision(unsigned int stateNum, unsigned int signalNum) const;
        int GetStateNum(CStr const& stateName) const;
        unsigned int GetSignalNum(CStr const& signalName) const;
        void Dump() const;
        void LogDump() const;
        ai::DecisionMatrix const* GetSubmatrix(int stateNum) const;
        ai::AIState const& GetState(int stateNum) const;
        ai::AISignal const& GetSignal(int signalNum) const;
        int NumStates() const;
        int NumSignals() const;
        int GetExternSignalMapping(int numExternSignal) const;
        ai::AIPassageCommand const& GetDefault() const;
        int GetExitStateNum() const;
        void SetRetValueInterpretation(char const* stateName, char const* schemeName, char const* signalName);
        void SetDefaultState(char const* stateName);
        void AddDefaultStateParam(char const* functionName);
        void SetExitState(char const* stateName);
        void FitMatrix();
        void ClearTemporaryParams();
        void AddTemporaryParam(char const* functionName);

    private:
        /* 0x0034 */ std::vector<ai::AIParamRef, std::allocator<ai::AIParamRef>> m_tmpParamRefList;
        ai::DecisionMatrixElement& _GetElement(int signalNum, int stateNum);
        ai::DecisionMatrixElement const& _GetElement(int signalNum, int stateNum) const;
        static void __fastcall _LogUnexpectedToken(CStr const& token);
        /* 0x0044 */ std::vector<ai::AIState, std::allocator<ai::AIState>> m_States;
        /* 0x0054 */ std::vector<ai::AISignal, std::allocator<ai::AISignal>> m_Signals;
        /* 0x0064 */ std::vector<ai::DecisionMatrixElement, std::allocator<ai::DecisionMatrixElement>> m_Elements;
        /* 0x0074 */ int m_numStates;
        /* 0x0078 */ int m_numSignals;
        /* 0x007c */ int m_ExternSignalMappings[16];
        /* 0x00bc */ ai::AIPassageCommand m_Default;
        /* 0x00d0 */ int m_ExitStateNum;
    }; /* size: 0x00d4 */
}
