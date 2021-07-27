#pragma once
#include <vector>
#include <core/stringm3d.h>

namespace ai
{
    class PlayerPassMap
    {
    public:
        bool GetValue(unsigned int,unsigned int) const ;
        void Clear();
        bool IsEmpty() const ;
        bool SaveToBinaryFile(CStr const &) const ;
        void SetValue(unsigned int,unsigned int,bool);
        void Create(unsigned int,bool);
        void Fill(bool);
        PlayerPassMap();
        bool LoadFromBinaryFile(CStr const &);

    private:
        std::vector<unsigned int> m_container;
        unsigned int m_sideSize;
    };
}
