#include "playerpassmap.h"

#include <stdexcept>
#include <file/tagged.h>
#include <core/log.h>

namespace ai
{
    bool PlayerPassMap::GetValue(unsigned x, unsigned y) const
    {
        M3D_ASSERT(x < m_sideSize && y < m_sideSize);
        return ((1 << ((y + x * LOBYTE(this->m_sideSize)) & 0x1F)) & this->m_container[(y + x * this->m_sideSize) >> 5]) != 0;
    }

    void PlayerPassMap::Clear()
    {
        this->m_sideSize = 0;
        m_container.clear();
    }

    bool PlayerPassMap::IsEmpty() const
    {
        return this->m_sideSize == 0;
    }

    // RVA 0x7BBAD0
    bool PlayerPassMap::SaveToBinaryFile(CStr const& fileName) const
    {
        if (!m_sideSize)
        {
            return false;
        }

        m3d::fs::auxTaggedFile file;
        if (file.Open(fileName.c_str(), m3d::fs::auxTaggedFile::CREATE_IGNORE_CRC))
        {
            M3D_LOG_ERR(CStr("Couldn't create player pass map file ") + CStr(fileName));
            return false;
        }

        file.setFormatTitle("PLAYERPASSMAP");
        file.setFormatVersion(2);
        file.addChunk(0xBADF00D);
        file.addChunkDataCopy(0xBADF00D, 4, &m_sideSize);
        file.addChunkDataCopy(0xBADF00D, 4 * m_container.size(), m_container.data());
        file.Close();
        return true;
    }

    // RVA 0x7BBEC0
    void PlayerPassMap::SetValue(unsigned x, unsigned y, bool bValue)
    {
        M3D_ASSERT(x < m_sideSize && y < m_sideSize);
        unsigned int const bit = y + x * m_sideSize;
        if (bValue)
        {
            m_container[bit >> 5] |= 1 << (bit & 0x1F);
        }
        else
        {
            m_container[bit >> 5] &= ~(1 << (bit & 0x1F));
        }
    }

    void PlayerPassMap::Create(unsigned sideLength, bool bDefaultValue)
    {
        M3D_ASSERT(sideLength > 0 && IsEmpty());
        m_container.resize(((sideLength * sideLength) >> 5) + 1, -bDefaultValue);
        m_sideSize = sideLength;
    }

    // RVA 0x7BBE60
    void PlayerPassMap::Fill(bool bValue)
    {
        for (unsigned int i = 0; i < m_container.size(); ++i)
        {
            m_container[i] = -static_cast<int>(bValue);
        }
    }

    PlayerPassMap::PlayerPassMap()
    {
    }

    bool PlayerPassMap::LoadFromBinaryFile(CStr const& fileName)
    {
        // RVA 0x7BC2C0
        m3d::fs::auxTaggedFile file;
        if (file.Open(fileName.c_str(), m3d::fs::auxTaggedFile::eOpenFlag::PROCESS_NORMAL_IGNORE_CRC))
        {
            M3D_LOG_ERR("Couldn't load player pass map from file " + fileName);
            return false;
        }

        char* title = nullptr;
        file.getFormatTitle(&title);
        if (strcmp(title, "PLAYERPASSMAP"))
        {
            M3D_LOG_ERR("Error: Bad player passmap format title: '" + CStr(title) + CStr("'"));
            file.Close();
            return false;
        }

        unsigned formatVersion = 0;
        file.getFormatVersion(formatVersion);
        if (formatVersion == 1)
        {
            // Version 1 stores a map of half the resolution, one bit per cell, row by row: each bit becomes a
            // 2x2 block of cells.
            unsigned* data = nullptr;
            file.getChunkData(0xBADF00D, reinterpret_cast<void**>(&data));
            unsigned const* const pSideSize = data;
            unsigned const* const bits = data + 1;
            if (m_sideSize)
            {
                Clear();
            }
            Create(2 * *pSideSize, false);
            for (unsigned i = 0; i < *pSideSize; ++i)
            {
                for (unsigned j = 0; j < *pSideSize; ++j)
                {
                    unsigned const bit = j + i * *pSideSize;
                    bool const value = (bits[bit >> 5] & (1u << (bit & 0x1F))) != 0;
                    SetValue(2 * i, 2 * j, value);
                    SetValue(2 * i, 2 * j + 1, value);
                    SetValue(2 * i + 1, 2 * j, value);
                    SetValue(2 * i + 1, 2 * j + 1, value);
                }
            }
        }
        else if (formatVersion == 2)
        {
            // Version 2 is the side size followed by the raw container.
            unsigned* data = nullptr;
            file.getChunkData(0xBADF00D, reinterpret_cast<void**>(&data));
            if (m_sideSize)
            {
                Clear();
            }
            Create(data[0], false);
            memcpy(m_container.data(), data + 1, sizeof(m_container[0]) * m_container.size());
        }
        else
        {
            M3D_LOG_ERR("Error: Wrong player passmap format version: " + CStr(formatVersion));
            file.Close();
            return false;
        }

        file.Close();
        return true;
    }
}
