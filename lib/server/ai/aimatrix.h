#pragma once
#include <core/stringm3d.h>

namespace ai
{
    class DecisionMatrix;

    class AIMatrix
    {
    private:
        ai::DecisionMatrix* m_pDM;
        CStr m_name;

    public:
        AIMatrix(CStr const& Name, ai::DecisionMatrix* pDM);
        void Set(CStr const& Name, ai::DecisionMatrix* pDM);
        ai::DecisionMatrix* GetPtr() const;
        CStr const& GetName() const;
        void Delete();
    };
}  // namespace ai
