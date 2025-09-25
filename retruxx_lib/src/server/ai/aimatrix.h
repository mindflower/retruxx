#pragma once
#include <core/stringm3d.h>

namespace ai
{
    class DecisionMatrix;

    class AIMatrix
    {
    private:
        /* 0x0000 */ ai::DecisionMatrix* m_pDM;
        CStr m_name;

    public:
        AIMatrix(const ai::AIMatrix& __that);
        AIMatrix(const CStr& Name, ai::DecisionMatrix* pDM);
        void Set(const CStr& Name, ai::DecisionMatrix* pDM);
        ai::DecisionMatrix* GetPtr() const;
        const CStr& GetName() const;
        void Delete();
    }; /* size: 0x0010 */
}
