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
        DecisionMatrixElement();
        void Dump();
    protected:
    private:
        std::vector<AIPassageCommand> m_PassageCommands;
        unsigned __int16 m_flags;
    };

class DecisionMatrix :  public m3d::Object
    {
    public:
        void ClearTemporaryParams();
        void FitMatrix();
        int GetExternSignalMapping(int) const ;
        void AddTemporaryParam(char const *);
        virtual m3d::Object * Clone();
        unsigned int GetSignalNum(CStr const &) const ;
        unsigned int UnsafeFirstDecision(unsigned int,unsigned int) const ;
        static m3d::Class* __fastcall GetBaseClass();
        void Dump() const ;
        static m3d::Object * __fastcall CreateObject();
        void Create(int,int);
        void LogDump() const ;
        void SetDefaultState(char const *);
        void AddCommand(char const *,char const *,char const *);
        void AddCommand(unsigned int,unsigned int,unsigned int,std::vector<AIParamRef,std::allocator<AIParamRef> > const &);
        int GetExitStateNum() const ;
        int NumStates() const ;
        void AddState(char const *,char const *);
        void AddState(AIState const &);
        void SetSaveStackFlag(char const *,char const *);
        void SetSaveStackFlag(int,int);
        void AddSignal(char const *,char const *,char const *);
        void AddSignal(AISignal const &);
        int NumSignals() const ;
        AISignal const & GetSignal(int) const ;
        virtual ~DecisionMatrix();
        void AddDefaultStateParam(char const *);
        AIState const & GetState(int) const ;
        DecisionMatrix const * GetSubmatrix(int) const ;
        AIPassageCommand const & GetDefault() const ;
        virtual m3d::Class* GetClass() const ;
        void SetRetValueInterpretation(char const *,char const *,char const *);
        void SetExitState(char const *);
        void AddSublevel(unsigned int,DecisionMatrix *);
        void AddSublevel(char const *,m3d::Object *);
        DecisionMatrixElement const * UnsafeGetDecision(int,int) const ;
        int GetStateNum(CStr const &) const ;

    protected:
        DecisionMatrix(DecisionMatrix const &);
        DecisionMatrix();

    private:
        DecisionMatrixElement const & _GetElement(int,int) const ;
        DecisionMatrixElement & _GetElement(int,int);
        static void __fastcall _LogUnexpectedToken(CStr const &);

private:
        std::vector<AIParamRef> m_tmpParamRefList;
        std::vector<AIState> m_States;
        std::vector<AISignal> m_Signals;
        std::vector<DecisionMatrixElement> m_Elements;
        int m_numStates;
        int m_numSignals;
        int m_ExternSignalMappings[16];
        AIPassageCommand m_Default;
        int m_ExitStateNum;
    };
}
