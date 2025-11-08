#pragma once
#include <vector>

namespace Graph
{
    template<class T>
    struct CMatrix
    {
    public:
        CMatrix(CMatrix<T> const&);
        T& operator()(unsigned int, unsigned int);
        T const& operator()(unsigned int, unsigned int) const;

    protected:
        void Resize(unsigned int, unsigned int);

    private:
        std::vector<std::vector<T>> m_Data;
    };

    template<class T>
    class CSquareMatrix : public CMatrix<T>
    {
    public:
        CSquareMatrix(int);
        unsigned int getSize() const ;
    };
}
