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
        std::vector<ai::AIPassageCommand, std::allocator<ai::AIPassageCommand>> m_PassageCommands;
        unsigned short m_flags;
        DecisionMatrixElement();
        void Dump();
    };

    class DecisionMatrix : public m3d::Object
    {
    protected:
        DecisionMatrix();
        DecisionMatrix(ai::DecisionMatrix const& rhs);

    public:
        virtual ~DecisionMatrix() override;
        virtual m3d::Object* Clone() override;
        static m3d::Object* CreateObject();
        static m3d::Class* GetBaseClass();
        virtual m3d::Class* GetClass() const override;
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

        // AI reads the state and signal tables directly when naming the state it is in.
        friend class AI;

    private:
        std::vector<ai::AIParamRef, std::allocator<ai::AIParamRef>> m_tmpParamRefList;
        ai::DecisionMatrixElement& _GetElement(int signalNum, int stateNum);
        ai::DecisionMatrixElement const& _GetElement(int signalNum, int stateNum) const;
        static void _LogUnexpectedToken(CStr const& token);
        std::vector<ai::AIState, std::allocator<ai::AIState>> m_States;
        std::vector<ai::AISignal, std::allocator<ai::AISignal>> m_Signals;
        std::vector<ai::DecisionMatrixElement, std::allocator<ai::DecisionMatrixElement>> m_Elements;
        int m_numStates;
        int m_numSignals;
        int m_ExternSignalMappings[16];
        ai::AIPassageCommand m_Default;
        int m_ExitStateNum;
    };
}  // namespace ai
