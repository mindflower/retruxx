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
        AIManager(const ai::AIManager& rhs);

    public:
        virtual  ~AIManager() override /* 0x00 */;
        virtual m3d::Object* Clone() override /* 0x04 */;
        static m3d::Object* __fastcall CreateObject();
        static m3d::Class* __fastcall GetBaseClass();
        virtual m3d::Class* GetClass() const override /* 0x34 */;
        static m3d::Class m_classAIManager;

    private:
        std::vector<ai::AIMatrix, std::allocator<ai::AIMatrix> > m_Matrix;
        std::vector<ai::AIFunc, std::allocator<ai::AIFunc> > m_Actions;
        std::vector<CStr, std::allocator<CStr> > m_Schemes;

    public:
        /* 0x0064 */ unsigned int m_workTime;
        /* 0x0068 */ float m_elapsedTime;

    private:
        ai::DecisionMatrix* ReadNewMatrix(const char* FileName);
        int GetParamNum(ai::GPARSER_VARS*, const CStr&) const;
        int GetSymbolNum(const CStr&) const;

    public:
        void RegisterMatrix(const CStr& Name, ai::DecisionMatrix* pDM);
        int RegisterFunc(const CStr& Name, m3d::AIParam(*funcAction)(ai::Obj*));
        int GetSchemeNum(const CStr& Word) const;
        int GetFuncNum(const CStr& Name) const;
        int GetMatrixNum(const CStr& Name) const;
        CStr GetMatrixName(int MatrixNum) const;
        ai::DecisionMatrix* GetDecisionMatrixPtr(int MatrixNum) const;
        m3d::AIParam AIAction(int FuncNum, ai::Obj* pObj);
        void Dump();
        void LogDump();
        ai::DecisionMatrix* CreateNewDecisionMatrix();
        ai::DecisionMatrix* LoadMatrix(const char* fileName);
    }; /* size: 0x006c */

    void SetAIManager(AIManager*);


    inline AIManager* theAIManager = nullptr;
}
