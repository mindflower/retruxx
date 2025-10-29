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

    bool PlayerPassMap::SaveToBinaryFile(CStr const&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void PlayerPassMap::SetValue(unsigned, unsigned, bool)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void PlayerPassMap::Create(unsigned, bool)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void PlayerPassMap::Fill(bool)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    PlayerPassMap::PlayerPassMap()
    {
    }

    bool PlayerPassMap::LoadFromBinaryFile(CStr const& fileName)
    {
        // TODO: check this
        m3d::fs::auxTaggedFile file;
        if (file.Open(fileName.c_str(), m3d::fs::auxTaggedFile::eOpenFlag::PROCESS_NORMAL_IGNORE_CRC))
        {
            M3D_LOG_ERR("Couldn't load player pass map from file " + fileName);
            return 0;
        }


        char* title = nullptr;
        file.getFormatTitle(&title);
        if (strcmp(title, "PLAYERPASSMAP"))
        {
            M3D_LOG_ERR("Error: Bad player passmap format title: '" + CStr(title) + "'");
            return 0;
        }

        unsigned uFormatVersion = 0;
        file.getFormatVersion(uFormatVersion);
        if (uFormatVersion == 1)
        {
            uint8_t* Data = nullptr;
            file.getChunkData(0xBADF00Du, (void**)&Data);
            auto v7 = (unsigned int*)Data;
            auto v8 = this->m_sideSize == 0;
            auto pSideSize = (unsigned int*)Data;
            Data += 4;
            auto pContainer = (unsigned int*)Data;
            if (!v8)
                Clear();
            Create(2 * *v7, 0);
            auto v9 = *v7;
            auto v10 = 0;
            v8 = *v7 == 0;
            auto i = 0;
            if (!v8)
            {
                auto v14 = 0;
                do
                {
                    auto v11 = 0;
                    auto j = 0;
                    if (v9)
                    {
                        auto x = 2 * v10 + 1;
                        do
                        {
                            auto v12 = j + v10 * v9;
                            auto v13 = 1 << (v12 & 0x1F);
                            v12 >>= 5;

                            SetValue(2 * v10, v11, (v13 & pContainer[v12]) != 0);
                            SetValue(2 * i, v11 + 1, (v13 & pContainer[v12]) != 0);
                            SetValue(x, v11, (v13 & pContainer[v12]) != 0);
                            SetValue(x, v11 + 1, (v13 & pContainer[v12]) != 0);

                            v9 = *pSideSize;
                            v10 = i;
                            v11 += 2;
                            v14 = ++j < *pSideSize;
                        } while (v14);
                        v7 = pSideSize;
                    }
                    v9 = *v7;
                    v14 = ++v10 < *v7;
                    i = v10;
                } while (v14);
            }
        }
        else
        {
            if (uFormatVersion != 2)
            {
                M3D_LOG_ERR("Error: Wrong player passmap format version: " + CStr(uFormatVersion));
                return 0;
            }
            uint8_t* Data = nullptr;
            file.getChunkData(0xBADF00Du, (void**)&Data);

            auto v15 = Data;
            auto v8 = this->m_sideSize == 0;
            Data += 4;
            if (!v8)
                Clear(); 
            Create(*(int*)v15, 0);
            memcpy(m_container.data(), Data, 4 * m_container.size());
        }
    }
}
