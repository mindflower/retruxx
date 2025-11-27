#pragma once
#include "retruxx/common.h"

namespace Graph
{
    template<class T>
    class CMatrix
    {
        using TLine = std::vector<T>;
        using TMatrix = std::vector<std::vector<T>>;

    protected:
        /* 0x0000 */ TMatrix m_Data;
        void Resize(unsigned int n, unsigned int m)
        {
            std::vector<float> tLine(m, 0.0);
            m_Data.resize(n, tLine);
        }

    public:
        CMatrix(unsigned int n, unsigned int m)
        {
            Resize(n, m);
        }

        T const& operator()(unsigned int i, unsigned int j) const
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        T& operator()(unsigned int i, unsigned int j)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }
    }; /* size: 0x0010 */

    template<class T>
    class CSquareMatrix : public CMatrix<T>
    {
    public:
        CSquareMatrix(int n) : CMatrix(n, n)
        {
        }

        unsigned int getSize() const
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        void Input(_iobuf*)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        void Output(_iobuf*)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }
    }; /* size: 0x0010 */

    std::vector<int> getMaxValuedMatching(Graph::CSquareMatrix<float> const&);
    std::vector<int> getMinValuedMatching(Graph::CSquareMatrix<float>);
    float getMatchingValue(Graph::CSquareMatrix<float> const&, std::vector<int> const&);
}
