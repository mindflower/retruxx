#pragma once
#include "retruxx/common.h"
#include <core/stringm3d.h>

namespace ai
{
    class PlayerPassMap
    {
    private:
        retruxx::vector<unsigned int> m_container;
        /* 0x0010 */ unsigned int m_sideSize = 0;

    public:
        PlayerPassMap(const ai::PlayerPassMap&);
        PlayerPassMap();
        void Create(unsigned int sideLength, bool bDefaultValue);
        void Fill(bool bValue);
        void Clear();
        bool LoadFromBinaryFile(const CStr& fileName);
        bool SaveToBinaryFile(const CStr& fileName) const;
        bool GetValue(unsigned int x, unsigned int y) const;
        void SetValue(unsigned int x, unsigned int y, bool bValue);
        bool IsEmpty() const;
    }; /* size: 0x0014 */
}
