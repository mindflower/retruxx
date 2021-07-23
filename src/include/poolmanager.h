#pragma once
#include <stack>
#include <vector>

namespace m3d
{
    template<class T>
    class PoolManager
    {
    public:
        class Block
        {
            T Data;
            unsigned int BlockNumber;
        };

    public:
        PoolManager(unsigned int);
        New();
        Delete(T*&);
        GetFree();
        SetFree(T*&);
        ~PoolManager();

    private:
        std::vector<Block *> Pool;
        std::stack<unsigned int> Free;
    };
}
