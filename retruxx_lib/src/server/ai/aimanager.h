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

    class AIManager :  public m3d::Object
    {
    public:
        DecisionMatrix * LoadMatrix(char const *);
        int GetMatrixNum(CStr const &) const ;
        void RegisterMatrix(CStr const &,DecisionMatrix *);
        void Dump();
        static m3d::Class* GetBaseClass();
        int RegisterFunc(CStr const &, m3d::AIParam(__fastcall*)(Obj*));
        void LogDump();
        int GetSchemeNum(CStr const &) const ;
        CStr GetMatrixName(int) const ;
        DecisionMatrix * GetDecisionMatrixPtr(int) const ;
        static Object * CreateObject();
        virtual m3d::Class* GetClass() const ;
        virtual ~AIManager();
        m3d::AIParam AIAction(int,Obj *);
        int GetFuncNum(CStr const &) const ;
        virtual m3d::Object * Clone();
        DecisionMatrix * CreateNewDecisionMatrix();

    protected:
        AIManager();
        AIManager(AIManager const &);

    private:
        DecisionMatrix * ReadNewMatrix(char const *);

    public:
        RT_CLASS_DECLARE(AIManager);

    public:
        float m_elapsedTime;

    private:
        std::vector<AIMatrix> m_Matrix;
        std::vector<AIFunc> m_Actions;
        std::vector<CStr> m_Schemes;
        unsigned int m_workTime;
    };

    void SetAIManager(AIManager*);
}
