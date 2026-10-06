// texture_man_stats.cpp - texture memory statistics and the "Loaded textures info" report.
// Ported from the original dxrender9/texture_man_stats.cpp.
#include "device.h"
#include "log.h"

#include <core/kernel.h>
#include <core/scoped_ptr.h>
#include <file/fileserver.h>
#include <file/filestream.h>

namespace
{
    struct Sort2DTextureByFormatAndSize
    {
        // orig 0x628ab0 texture_man_stats.cpp:37
        bool operator()(CDevice::CTexMap* tex1, CDevice::CTexMap* tex2)
        {
            if (tex1->m_desc2d.Format < tex2->m_desc2d.Format)
                return true;
            if (tex1->m_desc2d.Format > tex2->m_desc2d.Format)
                return false;
            return tex1->m_desc2d.Height * tex1->m_desc2d.Width < tex2->m_desc2d.Height * tex2->m_desc2d.Width;
        }
    };

    struct Sort3DTextureByFormatAndSize
    {
        // orig 0x628af0 texture_man_stats.cpp:61
        bool operator()(CDevice::CTexMap* tex1, CDevice::CTexMap* tex2)
        {
            if (tex1->m_desc3d.Format < tex2->m_desc3d.Format)
                return true;
            if (tex1->m_desc3d.Format > tex2->m_desc3d.Format)
                return false;
            return tex1->m_desc3d.Depth * tex1->m_desc3d.Height * tex1->m_desc3d.Width <
                   tex2->m_desc3d.Depth * tex2->m_desc3d.Height * tex2->m_desc3d.Width;
        }
    };

    // The byte size of a 2D/cube map as the original estimates it. The format table is the one in the binary
    // (jump table at 0x628bf8/0x628c08): A1R5G5B5 and A8R3G3B2 count 3 bytes per texel, X1R5G5B5,
    // R3G3B2 and A8 count 0.
    // orig 0x628b40 texture_man_stats.cpp:84
    unsigned int getTextureApproxSize(CDevice::CTexMap* tex)
    {
        unsigned int size = tex->m_desc2d.Height * tex->m_desc2d.Width;

        switch (tex->m_desc2d.Format)
        {
            case D3DFMT_R8G8B8:
            case D3DFMT_A1R5G5B5:
            case D3DFMT_A8R3G3B2:
                size *= 3;
                break;
            case D3DFMT_A8R8G8B8:
            case D3DFMT_X8R8G8B8:
                size *= 4;
                break;
            case D3DFMT_R5G6B5:
            case D3DFMT_A4R4G4B4:
                size *= 2;
                break;
            case D3DFMT_DXT1:
                size /= 2;
                break;
            case D3DFMT_DXT3:
            case D3DFMT_DXT5:
                break;
            default:
                size = 0;
                break;
        }

        if (tex->IsCube())
            size *= 6;

        unsigned int mipq = tex->m_flags & TM_MIPQ_MASK;
        if (mipq == TM_MIPQ_HIGH || mipq == TM_MIPQ_LOW)
            size = (int)(size * 1.3f);

        return size;
    }

    // orig 0x628c20 texture_man_stats.cpp:136
    unsigned int getVolumeTextureApproxSize(CDevice::CTexMap* tex)
    {
        unsigned int size = tex->m_desc3d.Depth * tex->m_desc3d.Height * tex->m_desc3d.Width;

        switch (tex->m_desc3d.Format)
        {
            case D3DFMT_R8G8B8:
            case D3DFMT_A1R5G5B5:
            case D3DFMT_A8R3G3B2:
                size *= 3;
                break;
            case D3DFMT_A8R8G8B8:
            case D3DFMT_X8R8G8B8:
                size *= 4;
                break;
            case D3DFMT_R5G6B5:
            case D3DFMT_A4R4G4B4:
                size *= 2;
                break;
            case D3DFMT_DXT1:
                size /= 2;
                break;
            case D3DFMT_DXT3:
            case D3DFMT_DXT5:
                break;
            default:
                size = 0;
                break;
        }

        unsigned int mipq = tex->m_flags & TM_MIPQ_MASK;
        if (mipq == TM_MIPQ_HIGH || mipq == TM_MIPQ_LOW)
            size = (int)(size * 1.3f);

        return size;
    }

    // orig 0x6292a0 texture_man_stats.cpp:183
    CStr getTextureInfoStr(CDevice::CTexMap* texMap, unsigned long* currTexsSize)
    {
        CStr resStr;
        CStr mipsStr;
        CStr sizeStr;
        CStr fmtStr;

        unsigned int mipq = texMap->m_flags & TM_MIPQ_MASK;

        switch (mipq)
        {
            default:
            case TM_MIPQ_HIGH:
                mipsStr = "Trilinear";
                break;
            case TM_MIPQ_NOMIPS:
                mipsStr = "None";
                break;
            case TM_MIPQ_LOW:
                mipsStr = "Point";
                break;
        }

        resStr.format("%dx%d", texMap->m_desc2d.Width, texMap->m_desc2d.Height);

        unsigned long size = getTextureApproxSize(texMap);
        *currTexsSize += size;
        sizeStr.format("%8d (%5.2f Kb)", size, size / 1024.0f);

        fmtStr.format("%10s    %9s    %22s    %9s    %s", getD3dFmtStr(texMap->m_desc2d.Format).c_str(),
                      resStr.c_str(), sizeStr.c_str(), mipsStr.c_str(), texMap->m_fileName.c_str());

        return fmtStr;
    }

    // orig 0x6294c0 texture_man_stats.cpp:229
    CStr getVolumeTextureInfoStr(CDevice::CTexMap* texMap, unsigned long* currTexsSize)
    {
        CStr resStr;
        CStr mipsStr;  // declared but never used in the original
        CStr sizeStr;
        CStr fmtStr;

        resStr.format("%dx%dx%d", texMap->m_desc3d.Width, texMap->m_desc3d.Height, texMap->m_desc3d.Depth);

        unsigned long size = getVolumeTextureApproxSize(texMap);
        *currTexsSize += size;
        sizeStr.format("%8d (%5.2f Kb)", size, size / 1024.0f);

        fmtStr.format("%10s    %12s    %22s    %s", getD3dFmtStr(texMap->m_desc3d.Format).c_str(), resStr.c_str(),
                      sizeStr.c_str(), texMap->m_fileName.c_str());

        return fmtStr;
    }

    // orig 0x629650 texture_man_stats.cpp:256
    CStr getApproxSizeStr(unsigned long size)
    {
        CStr sizeStr;
        sizeStr.format("%d (%.2f Kb / %.2f Mb)", size, size / 1024.0f, size / (1024.0f * 1024.0f));
        return sizeStr;
    }
}  // namespace

// orig 0x62abb0 texture_man_stats.cpp:272
bool CDevice::ReportTexturesInfo(const char* fileName)
{
    scoped_ptr<m3d::fs::FileStream> stream(g_kernel->GetFileServer().CreateFileStream());
    if (!stream->Open(fileName, m3d::fs::IStream::OPEN_WRITE))
        return false;

    *stream << "-==- -==- -==- -==- -==- -==- -==- -==- -==- -==- -==- -==- -==- -==- -==-\n";
    *stream << "-==\n";
    *stream << "-== Log category  : Loaded textures info\n";
    *stream << "-== Build         : " << DXRENDER9_BUILD_STRING << "\n";
    *stream << "-==\n";
    *stream << "-==- -==- -==- -==- -==- -==- -==- -==- -==- -==- -==- -==- -==- -==- -==-\n\n";

    std::vector<CTexMap*> textures[TT_NUM_TYPES];
    unsigned int numTexs[TT_NUM_TYPES] = { 0 };

    unsigned int numTextures = m_texMaps.size();
    for (unsigned int i = 0; i < numTextures; i++)
    {
        CTexMap* texMap = m_texMaps[i];

        if (texMap)
        {
            numTexs[texMap->m_type]++;
            textures[texMap->m_type].push_back(texMap);
        }
    }

    *stream << "** General stats **\n";
    *stream << "2D textures: " << numTexs[TT_2D_FROM_FILE] << "\n";
    *stream << "Dynamic 2D textures: " << numTexs[TT_2D_DYNAMIC] << "\n";
    *stream << "2D render targets: " << numTexs[TT_2D_RENDER_TARGET] << "\n";
    *stream << "Cube maps: " << numTexs[TT_CUBE_FROM_FILE] << "\n";
    *stream << "Dynamic cube maps: " << numTexs[TT_CUBE_DYNAMIC] << "\n";
    *stream << "Cube map render targets: " << numTexs[TT_CUBE_RENDER_TARGET] << "\n";
    *stream << "Volume textures: " << numTexs[TT_3D_FROM_FILE] << "\n";
    *stream << "Dynamic volume textures: " << numTexs[TT_3D_DYNAMIC] << "\n";
    *stream << "\n\n";

    CStr typeStr;
    CStr resStr;   // the next four are declared but never used in the original
    CStr mipsStr;
    CStr sizeStr;
    CStr fmtStr;

    unsigned long totalSize = 0;
    for (unsigned int nType = 0; nType < TT_NUM_TYPES; nType++)
    {
        unsigned long currSize = 0;

        switch (nType)
        {
            default:
                typeStr = "X3 what this";
                break;
            case TT_2D_FROM_FILE:
                typeStr = "2D textures";
                break;
            case TT_2D_DYNAMIC:
                typeStr = "2D dynamic textures";
                break;
            case TT_2D_RENDER_TARGET:
                typeStr = "2D render target textures";
                break;
            case TT_CUBE_FROM_FILE:
                typeStr = "Cube maps";
                break;
            case TT_CUBE_DYNAMIC:
                typeStr = "Dynamic cube maps";
                break;
            case TT_CUBE_RENDER_TARGET:
                typeStr = "Cube map render targets";
                break;
            case TT_3D_FROM_FILE:
                typeStr = "Volume textures";
                break;
            case TT_3D_DYNAMIC:
                typeStr = "Dynamic volume textures";
                break;
        }

        if (nType == TT_3D_FROM_FILE || nType == TT_3D_DYNAMIC)
        {
            std::sort(textures[nType].begin(), textures[nType].end(), Sort3DTextureByFormatAndSize());

            *stream << "** " << typeStr.c_str() << " (" << numTexs[nType] << " entries) **\n";
            *stream << "   Format       Resolution              ApproxSize          File\n";
            *stream << "--------------------------------------------------------------------------------------------------\n";

            if (numTexs[nType] != 0)
            {
                for (unsigned int i = 0; i < numTexs[nType]; i++)
                {
                    *stream << getVolumeTextureInfoStr(textures[nType][i], &currSize).c_str() << "\n";
                }
            }
            else
                *stream << "nothing\n";
        }
        else
        {
            std::sort(textures[nType].begin(), textures[nType].end(), Sort2DTextureByFormatAndSize());

            *stream << "** " << typeStr.c_str() << " (" << numTexs[nType] << " entries) **\n";
            *stream << "   Format       Resolution           ApproxSize          Mips     File\n";
            *stream << "--------------------------------------------------------------------------------------------------\n";

            if (numTexs[nType] != 0)
            {
                for (unsigned int i = 0; i < numTexs[nType]; i++)
                {
                    *stream << getTextureInfoStr(textures[nType][i], &currSize).c_str() << "\n";
                }
            }
            else
                *stream << "nothing\n";
        }

        *stream << "--------------------------------------------------------------------------------------------------\n";

        *stream << "Total ApproxSize for " << typeStr.c_str() << ": ";
        *stream << getApproxSizeStr(currSize).c_str() << "\n";

        totalSize += currSize;
        *stream << "\n\n";
    }

    *stream << "-----------------------------------------------------------------------\n";

    *stream << "\n\n";

    *stream << "Total ApproxSize for all textures: ";
    *stream << getApproxSizeStr(totalSize).c_str() << "\n";

    *stream << "\n\n";

    return true;
}

// orig 0x62a700 texture_man_stats.cpp:426
void CDevice::UpdateTexMemStats()
{
    std::vector<CTexMap*> textures[TT_NUM_TYPES];
    unsigned int numTexs[TT_NUM_TYPES] = { 0 };

    unsigned int numTextures = m_texMaps.size();
    for (unsigned int i = 0; i < numTextures; i++)
    {
        CTexMap* texMap = m_texMaps[i];

        if (texMap)
        {
            numTexs[texMap->m_type]++;
            textures[texMap->m_type].push_back(texMap);
        }
    }

    unsigned int texSize[TT_NUM_TYPES] = { 0 };
    for (unsigned int i = 0; i < TT_NUM_TYPES; i++)
    {
        for (unsigned int j = 0; j < numTexs[i]; j++)
            texSize[i] += getTextureApproxSize(textures[i][j]);
    }

    m_devMemStats.RtTexCount = numTexs[TT_2D_RENDER_TARGET] + numTexs[TT_CUBE_RENDER_TARGET];
    m_devMemStats.RtTexSize = texSize[TT_2D_RENDER_TARGET] + texSize[TT_CUBE_RENDER_TARGET];
    m_devMemStats.DynamicTexCount = numTexs[TT_2D_DYNAMIC] + numTexs[TT_CUBE_DYNAMIC] + numTexs[TT_3D_DYNAMIC];
    m_devMemStats.DynamicTexSize = texSize[TT_2D_DYNAMIC] + texSize[TT_CUBE_DYNAMIC] + texSize[TT_3D_DYNAMIC];
    m_devMemStats.StaticTexCount = numTexs[TT_2D_FROM_FILE] + numTexs[TT_CUBE_FROM_FILE] + numTexs[TT_3D_FROM_FILE];
    m_devMemStats.StaticTexSize = texSize[TT_2D_FROM_FILE] + texSize[TT_CUBE_FROM_FILE] + texSize[TT_3D_FROM_FILE];
}
