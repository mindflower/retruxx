#pragma once
#include <vector>
#include <core/clazz.h>

namespace m3d
{
    class AIParam;
}

namespace ai
{
    class AIFunc;
    class AIMatrix;
    class Obj;
    class DecisionMatrix;
    struct GPARSER_VARS;

    class AIManager : public m3d::Object
    {
    protected:
        AIManager();
        AIManager(ai::AIManager const& rhs);

    public:
        virtual ~AIManager() override;
        virtual m3d::Object* Clone() override;
        static m3d::Object* __fastcall CreateObject();
        static m3d::Class* __fastcall GetBaseClass();
        virtual m3d::Class* GetClass() const override;
        static m3d::Class m_classAIManager;

    private:
        std::vector<ai::AIMatrix, std::allocator<ai::AIMatrix>> m_Matrix;
        std::vector<ai::AIFunc, std::allocator<ai::AIFunc>> m_Actions;
        std::vector<CStr, std::allocator<CStr>> m_Schemes;

    public:
        unsigned int m_workTime;
        float m_elapsedTime;

    private:
        ai::DecisionMatrix* ReadNewMatrix(char const* FileName);
        int GetParamNum(ai::GPARSER_VARS*, CStr const&) const;
        int GetSymbolNum(CStr const&) const;

    public:
        void RegisterMatrix(CStr const& Name, ai::DecisionMatrix* pDM);
        int RegisterFunc(CStr const& Name, m3d::AIParam (*funcAction)(ai::Obj*));
        int GetSchemeNum(CStr const& Word) const;
        int GetFuncNum(CStr const& Name) const;
        int GetMatrixNum(CStr const& Name) const;
        CStr GetMatrixName(int MatrixNum) const;
        ai::DecisionMatrix* GetDecisionMatrixPtr(int MatrixNum) const;
        m3d::AIParam AIAction(int FuncNum, ai::Obj* pObj);
        void Dump();
        void LogDump();
        ai::DecisionMatrix* CreateNewDecisionMatrix();
        ai::DecisionMatrix* LoadMatrix(char const* fileName);
    };

    void SetAIManager(AIManager*);

    inline AIManager* theAIManager = nullptr;
}  // namespace ai
