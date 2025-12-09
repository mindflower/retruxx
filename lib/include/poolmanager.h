#pragma once
#include "retruxx/common.h"

namespace m3d
{
    template<class T>
    class PoolManager
    {
        struct Block
        {
            T Data;
            unsigned int BlockNumber;
        };

    protected:
        retruxx::vector<Block*> Pool;
        retruxx::stack<unsigned int> Free;

    public:
        T* New()
        {
            return GetFree();
        }

        T* GetFree()
        {
            if (Free.empty())
            {
                auto block = new Block;
                block->BlockNumber = Pool.size();
                Pool.push_back(block);
                return &block->Data;
            }
            else
            {
                auto top = Free.top();
                Free.pop();

                auto* block = Pool[top];
                auto* res = new (&block->Data) T(); 
                return res;
            }
        }

        void Delete(T*& pData)
        {
            if (pData)
            {
                auto block = (Block*)(pData);
                Free.push(block->BlockNumber);
                block->Data.~T();
                pData = nullptr;
            }
        }

        void SetFree(T*& pData);
        PoolManager(const PoolManager<T>&);

        PoolManager(unsigned int StartSize) :
            Pool(StartSize)
        {
            for (size_t i = 0; i < Pool.size(); ++i)
            {
                Pool[i] = new Block;
                Pool[i]->BlockNumber = i;
                Free.push(i);
            }
        }

        ~PoolManager()
        {
            for (auto& elem : Pool)
            {
                delete elem;
            }
        }
    };
}
