#pragma once
#include "retruxx/common.h"
#include <core/stringm3d.h>

namespace ai
{
    class PlayerPassMap
    {
    private:
        retruxx::vector<unsigned int> m_container;
        unsigned int m_sideSize = 0;

    public:
        PlayerPassMap(ai::PlayerPassMap const&);
        PlayerPassMap();
        void Create(unsigned int sideLength, bool bDefaultValue);
        void Fill(bool bValue);
        void Clear();
        bool LoadFromBinaryFile(CStr const& fileName);
        bool SaveToBinaryFile(CStr const& fileName) const;
        bool GetValue(unsigned int x, unsigned int y) const;
        void SetValue(unsigned int x, unsigned int y, bool bValue);
        bool IsEmpty() const;
    };
}  // namespace ai
