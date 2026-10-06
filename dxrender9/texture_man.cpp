// The texture manager of the Direct3D 9 renderer.
//
// Ported from the original dxrender9/texture_man.cpp; every function
// carries the RVA and start line of its original in the original binary.
#include "device.h"
#include "log.h"

#include <config.h>
#include <core/console/cvar.h>
#include <core/ini.h>
#include <core/kernel.h>
#include <core/ref_ptr.h>
#include <core/scoped_ptr.h>
#include <core/timer.h>
#include <file/filestream.h>
#include <file/fileserver.h>

#include <malloc.h>
#include <cstdio>
#include <cstring>

// The values of m3d::rend::TexFilters and m3d::rend::TexWrap of the original interface
// (engine/renderer/i_renderer.h); Hard Truck Apocalypse's i_renderer.h does not name them.
enum
{
    TM_TF_NEAREST = 1,
    TM_TF_LINEAR = 2,
    TM_TF_ANISOTROPIC = 3,
    TM_TF_BILINEAR = 4,
    TM_TF_TRILINEAR = 5,
    TM_TF_NEAREST_MIPMAP_NEAREST = 6,
    TM_TF_LINEAR_MIPMAP_NEAREST = 7,
    TM_TF_NEAREST_MIPMAP_LINEAR = 8,
    TM_TF_LINEAR_MIPMAP_LINEAR = 9,
    TM_TF_ANISOTROPIC_MIPMAP_NEAREST = 10,
    TM_TF_ANISOTROPIC_MIPMAP_LINEAR = 11,

    TM_WRAP_REPEAT = 1,
    TM_WRAP_CLAMP = 3,
    TM_WRAP_BORDER = 4,
};

// orig 0x63d610 texture_man.cpp:37
static int IsCompressed(D3DFORMAT fmt)
{
    return fmt == D3DFMT_DXT1 || fmt == D3DFMT_DXT2 || fmt == D3DFMT_DXT3 || fmt == D3DFMT_DXT4 || fmt == D3DFMT_DXT5;
}

// orig 0x63d8e0 texture_man.cpp:44
bool CDevice::IsDynamic(CTexture* tex)
{
    if (tex->m_maps.size() != 1)
        return false;

    CTexMap* map = tex->m_maps[0];

    return map->m_type == TT_2D_DYNAMIC || map->m_type == TT_2D_RENDER_TARGET || map->m_type == TT_CUBE_DYNAMIC ||
           map->m_type == TT_CUBE_RENDER_TARGET || map->m_type == TT_3D_DYNAMIC;
}

// orig 0x63fd20 texture_man.cpp:59
CDevice::CTexMap* CDevice::addTexMap(CStr const& filename, unsigned int flags)
{
    CTexMap* tm = 0;
    unsigned int i;
    for (i = 0; i < m_texMaps.size(); ++i)
    {
        tm = m_texMaps[i];
        if (tm->m_fileName == filename)
            break;
    }

    if (i == m_texMaps.size())
    {
        unsigned int mipq = flags & TM_MIPQ_MASK;
        bool noCompress = (flags & TM_NO_COMPRESS) != 0;

        scoped_ptr<m3d::fs::FileStream> stream(g_kernel->GetFileServer().CreateFileStream());
        if (!stream->Open(filename.c_str(), m3d::fs::IStream::OPEN_READ))
        {
            LogMsg(CStr("Cannot open ") + filename);
            return 0;
        }

        unsigned int sz = stream->GetSize();
        unsigned char* p = new unsigned char[sz];
        stream->ReadBytes(p, sz);

        // mipLevels is the D3DX MipLevels argument, filter its Filter and MipFilter arguments.
        unsigned int mipLevels = D3DX_DEFAULT;
        unsigned int filter;
        switch (mipq)
        {
        default:  // TM_MIPQ_HIGH
            filter = D3DX_FILTER_TRIANGLE;
            break;
        case TM_MIPQ_NOMIPS:
            filter = D3DX_FILTER_NONE;
            mipLevels = 0;
            break;
        case TM_MIPQ_LOW:
            filter = D3DX_FILTER_POINT;
            break;
        }

        D3DFORMAT fmt = GetTexFormat(true, !noCompress);

        D3DXIMAGE_INFO nfo;
        D3DXGetImageInfoFromFileInMemory(p, sz, &nfo);

        TexType type = TT_2D_FROM_FILE;
        switch (nfo.ResourceType)
        {
        case D3DRTYPE_TEXTURE:
            type = TT_2D_FROM_FILE;
            break;
        case D3DRTYPE_CUBETEXTURE:
            type = TT_CUBE_FROM_FILE;
            break;
        case D3DRTYPE_VOLUMETEXTURE:
            type = TT_3D_FROM_FILE;
            break;
        }

        // An already compressed file, or an uncompressed target format, is loaded in the file's own
        // format.
        if (IsCompressed(nfo.Format) || !IsCompressed(fmt))
            fmt = D3DFMT_UNKNOWN;

        tm = new CTexMap;

        HRESULT hr = 0;
        switch (type)
        {
        case TT_2D_FROM_FILE:
            hr = D3DXCreateTextureFromFileInMemoryEx(m_pd3dDevice, p, sz, D3DX_DEFAULT, D3DX_DEFAULT, mipLevels, 0, fmt,
                                                    D3DPOOL_MANAGED, filter, filter, 0, 0, 0, &tm->m_pTex2d);
            break;
        case TT_CUBE_FROM_FILE:
            hr = D3DXCreateCubeTextureFromFileInMemoryEx(m_pd3dDevice, p, sz, D3DX_DEFAULT, mipLevels, 0, fmt,
                                                        D3DPOOL_MANAGED, filter, filter, 0, 0, 0, &tm->m_pTexCube);
            break;
        case TT_3D_FROM_FILE:
            hr = D3DXCreateVolumeTextureFromFileInMemoryEx(m_pd3dDevice, p, sz, D3DX_DEFAULT, D3DX_DEFAULT,
                                                          D3DX_DEFAULT, mipLevels, 0, fmt, D3DPOOL_MANAGED, filter,
                                                          filter, 0, 0, 0, &tm->m_pTex3d);
            break;
        }

        delete[] p;

        if (FAILED(hr))
        {
            // The original passes the CStr filename itself to the "%s" (its first member is the char pointer).
            g_kernel->KernelLog("CreateTextureFromFileInMemoryEx error: %s file: %s", getD3dErrorStr(hr).c_str(),
                                filename.c_str());
            delete tm;
            return 0;
        }

        tm->m_fileName = filename;

        tm->m_fmt = fmt;
        tm->m_flags = flags;

        tm->m_pool = D3DPOOL_MANAGED;
        tm->m_usage = 0;
        tm->m_lastFileSize = sz;
        tm->m_lastFileDate = stream->GetDate();

        switch (type)
        {
        case TT_2D_FROM_FILE:
            tm->m_pTex2d->GetLevelDesc(0, &tm->m_desc2d);
            break;
        case TT_CUBE_FROM_FILE:
            tm->m_pTexCube->GetLevelDesc(0, &tm->m_desc2d);
            break;
        case TT_3D_FROM_FILE:
            tm->m_pTex3d->GetLevelDesc(0, &tm->m_desc3d);
            break;
        }

        tm->m_type = type;
        m_texMaps.push_back(tm);
    }

    tm->addRef();

    return tm;
}

// orig 0x6404d0 texture_man.cpp:308
int CDevice::newTexture()
{
    unsigned int i;
    for (i = 0; i < m_textures.size(); ++i)
    {
        if (m_textures[i].m_maps.empty())
            break;
    }

    if (i == m_textures.size())
        m_textures.push_back(CTexture());

    return i;
}

// orig 0x6418e0 texture_man.cpp:328
TexHandle CDevice::AddTexture(CStr const& filename, unsigned int flags)
{
    TexHandle result;

    if (filename.empty())
    {
        LogMsg(CStr("invalid filename for tex: ") + filename);
        result.SetInvalid();
        return result;
    }

    result = readShader(filename, flags);
    if (result.IsValid())
        return result;

    CTexMap* tm = addTexMap(filename, flags);
    if (!tm)
    {
        LogMsg(CStr("texmap failed to load: ") + filename);
        result.SetInvalid();
        return result;
    }

    int i = newTexture();

    CTexture& tex = m_textures[i];

    tex.m_refs = 0;
    tex.m_maps.push_back(tm);

    tex.m_address[0] = D3DTADDRESS_WRAP;
    tex.m_address[1] = D3DTADDRESS_WRAP;
    tex.m_address[2] = D3DTADDRESS_WRAP;
    tex.m_maxAnisotropy = 1;
    tex.m_magFilter = D3DTEXF_LINEAR;
    tex.m_minFilter = D3DTEXF_LINEAR;
    tex.m_borderColor = 0;
    tex.m_lodBias = 0.0f;
    tex.m_lodMax = 0;
    tex.m_fps = 0;
    tex.m_fileName = filename;

    if (flags & TM_MIPQ_MASK)
    {
        tex.m_mipFilter = D3DTEXF_POINT;
    }
    else
    {
        tex.m_mipFilter = D3DTEXF_NONE;
        tex.m_lodBias = -16.0f;
    }

    tex.addRef();

    m_isDevMemStatsValid = false;
    ((InternalHandle<TexHandle>&)result).SetId(i);
    return result;
}

// orig 0x6405e0 texture_man.cpp:402
TexHandle CDevice::AddDynamicTexture(const char* texName, int sx, int sy, unsigned int format)
{
    TexHandle result;

    unsigned int i;
    for (i = 0; i < m_textures.size(); ++i)
    {
        if (m_textures[i].m_maps.empty())
            break;
    }

    if (i == m_textures.size())
        m_textures.push_back(CTexture());

    int allMips = 0;
    if (format & TM_DTF_WANT_MIPS)
    {
        allMips = 1;
        format &= ~TM_DTF_WANT_MIPS;
    }

    D3DFORMAT fmt;
    unsigned int usage;
    D3DPOOL pool;
    switch (format)
    {
    case TM_DTF_RENDER_TARGET:
        fmt = GetTexFormatRt();
        usage = D3DUSAGE_RENDERTARGET;
        pool = D3DPOOL_DEFAULT;
        break;
    case TM_DTF_SHADOW_TARGET:
        fmt = GetTexFormatShadow();
        usage = D3DUSAGE_RENDERTARGET;
        pool = D3DPOOL_DEFAULT;
        break;
    case TM_DTF_DEPTH_TARGET:
        fmt = GetTexFormatDepth();
        usage = D3DUSAGE_DEPTHSTENCIL;
        pool = D3DPOOL_DEFAULT;
        break;
    case TM_DTF_SAME_AS_RENDER_TARGET:
        fmt = GetTexFormatRt();
        usage = 0;
        pool = D3DPOOL_MANAGED;
        break;
    case TM_DTF_COMPRESSED_RGBA_FORMAT:
        fmt = GetTexFormat(true, true);
        usage = 0;
        pool = D3DPOOL_MANAGED;
        break;
    case TM_DTF_COMPRESSED_RGB_FORMAT:
        fmt = GetTexFormat(false, true);
        usage = 0;
        pool = D3DPOOL_MANAGED;
        break;
    case TM_DTF_LUMINANCE8:
    case TM_DTF_ALPHA8:
    case TM_DTF_RGBA8888:
    case TM_DTF_RGBA8888_VIDEOFRAME:
        fmt = GetTexFormat(true, false);
        usage = 0;
        pool = D3DPOOL_MANAGED;
        break;
    default:
        result.SetInvalid();
        return result;
    }

    IDirect3DTexture9* d3dtex;
    HRESULT hr = D3DXCreateTexture(m_pd3dDevice, sx, sy, allMips ? 0 : 1, usage, fmt, pool, &d3dtex);
    if (FAILED(hr))
    {
        LogMsg(CStr("failed to create dynamic texture:sx = ") + CStr(sx) + " sy = " + CStr(sy) + "usage = " +
               (usage ? "rt" : "generic") + "fmt = " + getD3dFmtStr(fmt) + "pool = " +
               (pool == D3DPOOL_MANAGED ? "managed" : "default") + "error = " + getD3dErrorStr(hr));
        result.SetInvalid();
        return result;
    }

    CTexMap* tm = new CTexMap;

    tm->m_pTex2d = d3dtex;
    d3dtex->GetLevelDesc(0, &tm->m_desc2d);

    tm->m_pool = pool;
    tm->m_usage = usage;
    tm->m_type = TT_2D_DYNAMIC;
    tm->addRef();
    tm->m_fileName = texName;
    if (allMips)
        allMips = TM_MIPQ_LOW;
    tm->m_flags = allMips | TM_NO_COMPRESS;
    m_texMaps.push_back(tm);

    CTexture& tex = m_textures[i];

    tex.m_refs = 0;
    tex.m_maps.push_back(tm);

    tex.m_magFilter = D3DTEXF_LINEAR;
    tex.m_minFilter = D3DTEXF_LINEAR;
    tex.m_address[0] = D3DTADDRESS_WRAP;
    tex.m_address[1] = D3DTADDRESS_WRAP;
    tex.m_address[2] = D3DTADDRESS_WRAP;
    tex.m_maxAnisotropy = 1;
    tex.m_mipFilter = allMips ? D3DTEXF_POINT : D3DTEXF_NONE;
    tex.m_borderColor = 0;
    tex.m_lodBias = 0.0f;
    tex.m_lodMax = 0;
    tex.m_fps = 0;
    tex.m_fileName = texName;

    tex.addRef();

    m_isDevMemStatsValid = false;
    ((InternalHandle<TexHandle>&)result).SetId(i);
    return result;
}

// orig 0x63d930 texture_man.cpp:546
int CDevice::ReferenceTexture(TexHandle const& id)
{
    if (!IsTexValid(id))
        return 0;

    return m_textures[TexId(id)].addRef();
}

// orig 0x63fbd0 texture_man.cpp:558
int CDevice::ReleaseTexture(TexHandle& id)
{
    if (!IsTexValid(id))
        return 0;

    CTexture& tex = m_textures[TexId(id)];
    // release() already dropped one reference of every map; the loop drops a second one.
    if (tex.release() == 0)
    {
        for (unsigned int i = 0; i < tex.m_maps.size(); ++i)
        {
            CTexMap* map = tex.m_maps[i];

            if (map->release() == 0)
            {
                // The original erases the find() result unconditionally; a map that is not in the table would
                // make that erase(end()) (retruxx adaptation: guarded, same result otherwise).
                std::vector<CTexMap*>::iterator found = std::find(m_texMaps.begin(), m_texMaps.end(), map);
                if (found != m_texMaps.end())
                    m_texMaps.erase(found);

                map->freeTex();

                delete map;
            }
        }

        std::vector<CTexMap*>().swap(tex.m_maps);

        id.SetInvalid();

        m_isDevMemStatsValid = false;
    }

    return tex.m_refs;
}

// orig 0x63d980 texture_man.cpp:608 (original: float tsc, passed as an 8-byte double)
int CDevice::SetTexture(int stage, TexHandle const& id, long double tsc)
{
    if (!id.IsValid())
    {
        SetWhiteTexture(stage);
        return 1;
    }

    if (!IsTexValid(id))
        return 0;

    CTexture* tex = &m_textures[TexId(id)];

    int frame = GetTextureCurrentFrame(tex, (float)tsc);
    setTexture(stage, tex->m_maps[frame]->m_pTex);

    float kk = g_kernel->GetEngineCfg().m_r_texLodBias.GetF();
    setTextureSamplerState(stage, D3DSAMP_ADDRESSU, tex->m_address[0]);
    setTextureSamplerState(stage, D3DSAMP_ADDRESSV, tex->m_address[1]);
    setTextureSamplerState(stage, D3DSAMP_ADDRESSW, tex->m_address[2]);
    setTextureSamplerState(stage, D3DSAMP_MAXANISOTROPY, tex->m_maxAnisotropy);
    setTextureSamplerState(stage, D3DSAMP_MINFILTER, tex->m_minFilter);
    setTextureSamplerState(stage, D3DSAMP_MAGFILTER, tex->m_magFilter);
    setTextureSamplerState(stage, D3DSAMP_MIPFILTER, tex->m_mipFilter);
    setTextureSamplerState(stage, D3DSAMP_BORDERCOLOR, tex->m_borderColor);
    if (tex->m_lodBias != 0.0f)
        setTextureSamplerState(stage, D3DSAMP_MIPMAPLODBIAS, *((unsigned int*)&tex->m_lodBias));
    else
        setTextureSamplerState(stage, D3DSAMP_MIPMAPLODBIAS, *((unsigned int*)&kk));
    setTextureSamplerState(stage, D3DSAMP_MAXMIPLEVEL, tex->m_lodMax);

    return 1;
}

// orig 0x63d650 texture_man.cpp:664
void CDevice::SetWhiteTexture(int stage)
{
    setTexture(stage, 0);
}

// orig 0x63d660 texture_man.cpp:671
void CDevice::SetBlackTexture(int stage)
{
    setTexture(stage, 0);
}

// orig 0x63d670 texture_man.cpp:678
void CDevice::SetErrorTexture(int stage)
{
    setTexture(stage, 0);
}

// orig 0x63dad0 texture_man.cpp:685
void CDevice::SetTexAnimStart(TexHandle const& texId)
{
    if (!texId.IsValid())
        return;

    m_textures[TexId(texId)].m_timeStamp = 0.0;
}

// orig 0x63d770 texture_man.cpp:696 (original: float tsc, passed as an 8-byte double; the arithmetic is
// done in double)
int CDevice::GetTextureCurrentFrame(CTexture* tex, float tsc)
{
    double t = tsc;

    if (t < 0.0 && tex->m_fps != 0)
    {
        int curTime = g_kernel->GetTimer().GetFrameStartTimeUnscaled();

        if (tex->m_looped)
        {
            int period = tex->m_maps.size() * 1000 / tex->m_fps;

            t = (double)(curTime % period) / (double)period;
        }
        else
        {
            if (tex->m_timeStamp == 0.0)
                tex->m_timeStamp = curTime;

            double period = (double)tex->m_maps.size() / tex->m_fps;
            double cur = curTime - tex->m_timeStamp;
            if (cur > period)
                cur = period;

            t = cur / period;
        }
    }

    if (t == -1.0)
        return 0;

    int frame = (int)(tex->m_maps.size() * t);
    if (frame < 0)
        frame = 0;
    if (frame > (int)tex->m_maps.size() - 1)
        frame = (int)tex->m_maps.size() - 1;

    return frame;
}

// orig 0x63db00 texture_man.cpp:743 (original: float tsc, passed as an 8-byte double)
int CDevice::GetTextureCurrentFrame(TexHandle const& texId, float tsc)
{
    if (TexId(texId) < 0)
        return -1;

    return GetTextureCurrentFrame(&m_textures[TexId(texId)], tsc);
}

// orig 0x63db30 texture_man.cpp:754
void CDevice::SetTextureParameter(TexHandle const& id, TexParam param, unsigned int value)
{
    if (!IsTexValid(id))
        return;

    CTexture& tex = m_textures[TexId(id)];

    switch (param)
    {
    case TM_MIP_LOD_BIAS:
        tex.m_lodBias = *((float*)&value);
        break;

    case TM_MIP_LOD_MAX:
        tex.m_lodMax = value;
        break;

    case TM_WRAP_BORDER_COLOR:
        tex.m_borderColor = value;
        break;

    case TM_MAX_ANISOTROPY:
        tex.m_maxAnisotropy = value;
        break;

    case TM_WRAP_S:
    case TM_WRAP_T:
    case TM_WRAP_R:
    {
        int i;
        if (param == TM_WRAP_S)
            i = 0;
        else if (param == TM_WRAP_T)
            i = 1;
        else
            i = 2;

        D3DTEXTUREADDRESS addr;
        switch (value)
        {
        case TM_WRAP_REPEAT:
            addr = D3DTADDRESS_WRAP;
            break;
        case TM_WRAP_CLAMP:
            addr = D3DTADDRESS_CLAMP;
            break;
        case TM_WRAP_BORDER:
            addr = D3DTADDRESS_BORDER;
            break;
        default:
            addr = D3DTADDRESS_WRAP;
            break;
        }

        tex.m_address[i] = addr;
        break;
    }

    case TM_TEX_FILTER:
    {
        D3DTEXTUREFILTERTYPE mag;
        D3DTEXTUREFILTERTYPE min;
        D3DTEXTUREFILTERTYPE mip;
        switch (value)
        {
        default:
            mag = D3DTEXF_POINT;
            min = D3DTEXF_POINT;
            mip = D3DTEXF_NONE;
            break;
        case TM_TF_LINEAR:
            mag = D3DTEXF_LINEAR;
            min = D3DTEXF_LINEAR;
            mip = D3DTEXF_NONE;
            break;
        case TM_TF_BILINEAR:
            mag = D3DTEXF_LINEAR;
            min = D3DTEXF_LINEAR;
            mip = D3DTEXF_POINT;
            break;
        case TM_TF_TRILINEAR:
            mag = D3DTEXF_LINEAR;
            min = D3DTEXF_LINEAR;
            mip = D3DTEXF_LINEAR;
            break;
        case TM_TF_ANISOTROPIC:
            mag = D3DTEXF_ANISOTROPIC;
            min = D3DTEXF_ANISOTROPIC;
            mip = D3DTEXF_LINEAR;
            break;
        }

        tex.m_magFilter = mag;
        tex.m_minFilter = min;
        tex.m_mipFilter = mip;
        break;
    }

    case TM_TEX_MAG_FILTER:
    {
        D3DTEXTUREFILTERTYPE mag;
        switch (value)
        {
        case TM_TF_NEAREST:
            mag = D3DTEXF_POINT;
            break;
        case TM_TF_LINEAR:
            mag = D3DTEXF_LINEAR;
            break;
        case TM_TF_ANISOTROPIC:
            mag = D3DTEXF_ANISOTROPIC;
            break;
        default:
            mag = D3DTEXF_POINT;
            break;
        }

        tex.m_magFilter = mag;
        break;
    }

    case TM_TEX_MIN_FILTER:
    {
        D3DTEXTUREFILTERTYPE min;
        D3DTEXTUREFILTERTYPE mip;
        switch (value)
        {
        default:
            min = D3DTEXF_POINT;
            mip = D3DTEXF_NONE;
            break;
        case TM_TF_NEAREST_MIPMAP_NEAREST:
            min = D3DTEXF_POINT;
            mip = D3DTEXF_POINT;
            break;
        case TM_TF_LINEAR_MIPMAP_NEAREST:
            min = D3DTEXF_LINEAR;
            mip = D3DTEXF_POINT;
            break;
        case TM_TF_NEAREST_MIPMAP_LINEAR:
            min = D3DTEXF_POINT;
            mip = D3DTEXF_LINEAR;
            break;
        case TM_TF_LINEAR_MIPMAP_LINEAR:
            min = D3DTEXF_LINEAR;
            mip = D3DTEXF_LINEAR;
            break;
        case TM_TF_ANISOTROPIC_MIPMAP_NEAREST:
            min = D3DTEXF_ANISOTROPIC;
            mip = D3DTEXF_POINT;
            break;
        case TM_TF_ANISOTROPIC_MIPMAP_LINEAR:
            min = D3DTEXF_ANISOTROPIC;
            mip = D3DTEXF_LINEAR;
            break;
        }

        tex.m_minFilter = min;
        tex.m_mipFilter = mip;
        break;
    }
    }
}

// orig 0x63dd60 texture_man.cpp:933
int CDevice::UploadTexImage(TexHandle const& id, unsigned int sx, unsigned int sy, unsigned char* bits,
                            TexDynFormat incomingFormat, int mipLevel)
{
    int Pitch = 0;

    void* dst = LockTexture(id, incomingFormat, Pitch, mipLevel);
    if (!dst)
        return 0;

    CTexMap* map = m_textures[TexId(id)].m_maps[0];

    unsigned int w = std::min(map->m_desc2d.Width, sx);
    unsigned int h = std::min(map->m_desc2d.Height, sy);

    int result = 1;
    switch (map->m_desc2d.Format)
    {
    case D3DFMT_X1R5G5B5:
    case D3DFMT_A1R5G5B5:
        switch (incomingFormat)
        {
        case TM_DTF_LUMINANCE8:
        {
            unsigned char* src = bits;
            unsigned short* d = (unsigned short*)dst;
            int skip = Pitch / 2 - w;
            for (int y = 0; y < (int)h; ++y)
            {
                for (int x = 0; x < (int)w; ++x)
                {
                    unsigned int l = *src++ >> 3;
                    *d++ = (unsigned short)(((((l | 0x20) << 5) | l) << 5) | l);
                }
                d += skip;
            }
            break;
        }

        case TM_DTF_ALPHA8:
        {
            unsigned char* src = bits;
            unsigned short* d = (unsigned short*)dst;
            int skip = Pitch / 2 - w;
            for (int y = 0; y < (int)h; ++y)
            {
                for (int x = 0; x < (int)w; ++x)
                {
                    unsigned int a = *src++ >> 7;
                    *d++ = (unsigned short)(a << 15);
                }
                d += skip;
            }
            break;
        }

        case TM_DTF_RGBA8888:
        {
            unsigned int* src = (unsigned int*)bits;
            unsigned short* d = (unsigned short*)dst;
            int skip = Pitch / 2 - w;
            for (unsigned int y = 0; y < h; ++y)
            {
                for (unsigned int x = 0; x < w; ++x)
                {
                    unsigned int c = *src++;
                    // A1 R5 G5 B5
                    unsigned int v = (((c >> 24) & 0xff80) << 1) | ((c >> 16) & 0xf8);
                    v = (v << 5) | ((c >> 8) & 0xf8);
                    v = (v << 2) | ((c >> 3) & 0x1f);
                    *d++ = (unsigned short)v;
                }
                d += skip;
            }
            break;
        }

        default:
            result = 0;
            break;
        }
        break;

    case D3DFMT_R5G6B5:
        switch (incomingFormat)
        {
        case TM_DTF_ALPHA8:
        {
            unsigned short* d = (unsigned short*)dst;
            for (int y = 0; y < (int)h; ++y)
            {
                for (int x = 0; x < (int)w; ++x)
                    d[x] = 0;
                d += Pitch / 2;
            }
            break;
        }

        case TM_DTF_LUMINANCE8:
        {
            unsigned char* src = bits;
            unsigned short* d = (unsigned short*)dst;
            for (int y = 0; y < (int)h; ++y)
            {
                for (int x = 0; x < (int)w; ++x)
                {
                    unsigned int l = src[x] >> 3;
                    d[x] = (unsigned short)((((l << 5) | l) << 6) | l);
                }
                d += Pitch / 2;
                src += w;
            }
            break;
        }

        case TM_DTF_RGBA8888:
        {
            unsigned int* src = (unsigned int*)bits;
            unsigned short* d = (unsigned short*)dst;
            int skip = Pitch / 2 - w;
            for (unsigned int y = 0; y < h; ++y)
            {
                for (unsigned int x = 0; x < w; ++x)
                {
                    unsigned int c = *src++;
                    // As the original does it: the red's top bit is forced to one and green keeps 5 bits.
                    unsigned int v = ((c >> 16) & 0xfff8) | 0xff80;
                    v = (v << 6) | ((c >> 8) & 0xf8);
                    v = (v << 2) | ((c >> 3) & 0x1f);
                    *d++ = (unsigned short)v;
                }
                d += skip;
            }
            break;
        }

        default:
            result = 0;
            break;
        }
        break;

    case D3DFMT_A4R4G4B4:
    case D3DFMT_X4R4G4B4:
        switch (incomingFormat)
        {
        case TM_DTF_ALPHA8:
        {
            unsigned char* src = bits;
            unsigned short* d = (unsigned short*)dst;
            int skip = Pitch / 2 - w;
            for (int y = 0; y < (int)h; ++y)
            {
                for (int x = 0; x < (int)w; ++x)
                {
                    unsigned int a = *src++ >> 4;
                    *d++ = (unsigned short)((a << 12) | 0xfff);
                }
                d += skip;
            }
            break;
        }

        case TM_DTF_LUMINANCE8:
        {
            unsigned char* src = bits;
            unsigned short* d = (unsigned short*)dst;
            int skip = Pitch / 2 - w;
            for (int y = 0; y < (int)h; ++y)
            {
                for (int x = 0; x < (int)w; ++x)
                {
                    unsigned int l = *src++ >> 4;
                    *d++ = (unsigned short)(((((l | 0xf0) << 4) | l) << 4) | l);
                }
                d += skip;
            }
            break;
        }

        case TM_DTF_RGBA8888:
        {
            unsigned char* src = bits;
            unsigned short* d = (unsigned short*)dst;
            for (int y = 0; y < (int)h; ++y)
            {
                for (int x = 0; x < (int)w; ++x)
                {
                    unsigned char* c = src + x * 4;
                    unsigned int v = c[3] >> 4;
                    v = (v << 4) | (c[2] >> 4);
                    v = (v << 4) | (c[1] >> 4);
                    v = (v << 4) | (c[0] >> 4);
                    d[x] = (unsigned short)v;
                }
                d += Pitch / 2;
                src += w * 4;
            }
            break;
        }

        default:
            result = 0;
            break;
        }
        break;

    case D3DFMT_A8R8G8B8:
    case D3DFMT_X8R8G8B8:
        switch (incomingFormat)
        {
        case TM_DTF_ALPHA8:
        {
            unsigned char* src = bits;
            unsigned int* d = (unsigned int*)dst;
            for (int y = 0; y < (int)h; ++y)
            {
                for (int x = 0; x < (int)w; ++x)
                    d[x] = (unsigned int)src[x] << 24;
                d += Pitch / 4;
                src += w;
            }
            break;
        }

        case TM_DTF_LUMINANCE8:
        {
            unsigned char* src = bits;
            unsigned int* d = (unsigned int*)dst;
            for (int y = 0; y < (int)h; ++y)
            {
                for (int x = 0; x < (int)w; ++x)
                {
                    unsigned int c = src[x];
                    d[x] = 0xff000000 | (c << 16) | (c << 8) | c;
                }
                d += Pitch / 4;
                src += w;
            }
            break;
        }

        case TM_DTF_RGBA8888:
        {
            unsigned int* src = (unsigned int*)bits;
            unsigned int* d = (unsigned int*)dst;
            int skip = Pitch / 4 - w;
            for (unsigned int y = 0; y < h; ++y)
            {
                for (unsigned int x = 0; x < w; ++x)
                    *d++ = *src++;
                d += skip;
            }
            break;
        }

        case TM_DTF_RGBA8888_VIDEOFRAME:
            memcpy(dst, bits, h * w * 4);
            break;

        default:
            result = 0;
            break;
        }
        break;

    default:
        result = 0;
        break;
    }

    UnlockTexture(id);

    return result;
}

// orig 0x63e330 texture_man.cpp:1223
void* CDevice::LockTexture(TexHandle const& id, TexDynFormat incomingFormat, int& pitch, int mipLevel)
{
    // The thread-safety guard of original: in the release build only the GetCurrentThreadId() call is
    // left of it. Its second condition, m_mt_render_in_separate_thread.GetB(), has no cvar in
    // Hard Truck Apocalypse's EngineConfig.
    if (m_bThreadSafeGuardEnabled)
        GetCurrentThreadId();

    if (!IsTexValid(id))
        return 0;

    CTexture& tex = m_textures[TexId(id)];

    D3DLOCKED_RECT lr;
    if (FAILED(tex.m_maps[0]->m_pTex2d->LockRect(mipLevel, &lr, 0, D3DLOCK_NOSYSLOCK)))
    {
        LogMsg("cannot lock texture surface");
        return 0;
    }

    pitch = lr.Pitch;
    return lr.pBits;
}

// orig 0x63e430 texture_man.cpp:1254
void CDevice::UnlockTexture(TexHandle const& id)
{
    CTexture& tex = m_textures[TexId(id)];
    tex.m_maps[0]->m_pTex2d->UnlockRect(0);
}

// orig 0x63e460 texture_man.cpp:1265
void CDevice::rstTexPrepareFor()
{
    for (unsigned int i = 0; i < m_texMaps.size(); ++i)
    {
        CTexMap* tm = m_texMaps[i];
        if (tm->m_pool == D3DPOOL_DEFAULT)
            tm->m_pTex->Release();
    }
}

// orig 0x63e4a0 texture_man.cpp:1283
bool CDevice::rstTexRestoreAfter()
{
    int kf = 0;

    for (unsigned int i = 0; i < m_texMaps.size(); ++i)
    {
        if (m_texMaps[i]->m_pool != D3DPOOL_DEFAULT)
            continue;

        CTexMap* tm = m_texMaps[i];

        // A map of none of the three types takes the failure path; the value it starts from is not
        // observable.
        HRESULT hr = E_FAIL;
        if (tm->Is2D())
        {
            hr = m_pd3dDevice->CreateTexture(tm->m_desc2d.Width, tm->m_desc2d.Height, 1, tm->m_usage,
                                             tm->m_desc2d.Format, tm->m_pool, &tm->m_pTex2d, 0);
        }
        else if (tm->IsCube())
        {
            hr = m_pd3dDevice->CreateCubeTexture(tm->m_desc2d.Width, 1, tm->m_usage, tm->m_desc2d.Format, tm->m_pool,
                                                 &tm->m_pTexCube, 0);
        }
        else if (tm->Is3D())
        {
            hr = m_pd3dDevice->CreateVolumeTexture(tm->m_desc3d.Width, tm->m_desc3d.Height, tm->m_desc3d.Depth, 1,
                                                   tm->m_usage, tm->m_desc3d.Format, tm->m_pool, &tm->m_pTex3d, 0);
        }

        if (FAILED(hr))
        {
            LogMsg(CStr("cannot restore texmap ") + CStr(i));
            m_texMaps[i]->m_pTex = 0;
            kf++;
        }
    }

    if (kf)
    {
        LogMsg(CStr("TexMan failed to restore ") + CStr(kf) + " textures");

        return false;
    }

    return true;
}

// orig 0x63e880 texture_man.cpp:1368
void CDevice::GetDims(TexHandle const& tex, int& sx, int& sy)
{
    sx = 0;
    sy = 0;

    if (!IsTexValid(tex))
        return;

    int id = TexId(tex);
    CTexMap* tm = m_textures[id].m_maps[0];

    if (tm->Is2D() || tm->IsCube())
    {
        sx = tm->m_desc2d.Width;
        sy = tm->m_desc2d.Height;
    }
    else if (tm->Is3D())
    {
        sx = tm->m_desc3d.Width;
        sy = tm->m_desc3d.Height;
    }
}

// orig 0x63e910 texture_man.cpp:1392
void CDevice::TexCopy(TexHandle const& dest, TexHandle const& src, bool withMips)
{
    if (!IsTexValid(dest) || !IsTexValid(src))
        return;

    IDirect3DSurface9* srcSurf = 0;
    IDirect3DSurface9* dstSurf = 0;

    CTexMap* dstMap = m_textures[TexId(dest)].m_maps[0];
    CTexMap* srcMap = m_textures[TexId(src)].m_maps[0];

    int mipsNum;
    if (withMips)
        mipsNum = std::min(srcMap->m_pTex->GetLevelCount(), dstMap->m_pTex->GetLevelCount());
    else
        mipsNum = 1;

    for (int i = 0; i < mipsNum; ++i)
    {
        if (FAILED(dstMap->m_pTex2d->GetSurfaceLevel(i, &dstSurf)) ||
            FAILED(srcMap->m_pTex2d->GetSurfaceLevel(i, &srcSurf)))
        {
            LogMsg("cannot acquire surfaces to copy");
            return;
        }

        m_lastResult = D3DXLoadSurfaceFromSurface(dstSurf, 0, 0, srcSurf, 0, 0, D3DX_FILTER_POINT, 0);

        if (FAILED(m_lastResult))
            LogMsg(CStr("texcopy failed, result = ") + CStr((int)m_lastResult));

        srcSurf->Release();
        dstSurf->Release();
    }
}

// HTA-only wrapper, no original body
void CDevice::TexCopy(TexHandle const& dest, TexHandle const& src)
{
    TexCopy(dest, src, false);
}

// orig 0x63d680 texture_man.cpp:1449
int CDevice::DownloadTexImageRgba8888(unsigned int* dstBits, TexHandle const& tex)
{
    int sx, sy;
    GetDims(tex, sx, sy);
    return DownloadTexImageRgba8888(dstBits, &tex, sx, sy);
}

// orig 0x63eb30 texture_man.cpp:1458
int CDevice::DownloadTexImageRgba8888(unsigned int* dstBits, TexHandle const* tex, int sxx, int syy)
{
    if (!IsTexValid(*tex))
        return 0;

    IDirect3DSurface9* surf = 0;
    if (FAILED(m_textures[TexId(*tex)].m_maps[0]->m_pTex2d->GetSurfaceLevel(0, &surf)))
    {
        LogMsg("cannot get surface for tex");
        return 0;
    }

    int sx;
    int sy;
    GetDims(*tex, sx, sy);

    sx = sxx;
    sy = syy;

    IDirect3DTexture9* destTex = 0;
    if (FAILED(D3DXCreateTexture(m_pd3dDevice, sx, sy, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_SYSTEMMEM, &destTex)))
    {
        LogMsg("Cannot create temp texture");
        return 0;
    }

    IDirect3DSurface9* dest = 0;
    if (FAILED(destTex->GetSurfaceLevel(0, &dest)))
    {
        LogMsg("Cannot get surface2");
        return 0;
    }

    RECT dstrect;
    dstrect.left = 0;
    dstrect.top = 0;
    dstrect.right = sx;
    dstrect.bottom = sy;

    if (FAILED(D3DXLoadSurfaceFromSurface(dest, 0, &dstrect, surf, 0, 0, D3DX_FILTER_BOX, 0)))
    {
        LogMsg("Cannot get out texture");
        return 0;
    }

    surf->Release();

    D3DLOCKED_RECT r;
    if (FAILED(dest->LockRect(&r, 0, D3DLOCK_READONLY)))
    {
        LogMsg("cannot lock surf");
        return 0;
    }

    D3DSURFACE_DESC fmt;
    destTex->GetLevelDesc(0, &fmt);

    int result = 1;
    switch (fmt.Format)
    {
    case D3DFMT_X1R5G5B5:
    {
        unsigned int* d = dstBits;
        unsigned short* src = (unsigned short*)r.pBits;
        for (int y = 0; y < sy; ++y)
        {
            for (int x = 0; x < sx; ++x)
            {
                unsigned short rgba = src[x] & 0x7fff;
                unsigned char b = (unsigned char)rgba;
                rgba >>= 5;
                unsigned char g = (unsigned char)rgba;
                unsigned char rr = (unsigned char)(rgba >> 5);
                rr <<= 3;
                g <<= 3;
                b <<= 3;
                *d++ = 0xff000000 | (rr << 16) | (g << 8) | b;
            }
            src += r.Pitch / 2;
        }
        break;
    }

    case D3DFMT_A1R5G5B5:
    {
        unsigned int* d = dstBits;
        unsigned short* src = (unsigned short*)r.pBits;
        for (int y = 0; y < sy; ++y)
        {
            for (int x = 0; x < sx; ++x)
            {
                unsigned short c = src[x];
                unsigned short rgba = c & 0x7fff;
                unsigned char b = (unsigned char)rgba;
                rgba >>= 5;
                unsigned char g = (unsigned char)rgba;
                unsigned char rr = (unsigned char)(rgba >> 5);
                rr <<= 3;
                unsigned char a = (unsigned char)((c >> 8) & 0x80);
                g <<= 3;
                b <<= 3;
                *d++ = (a << 24) | (rr << 16) | (g << 8) | b;
            }
            src += r.Pitch / 2;
        }
        break;
    }

    case D3DFMT_A4R4G4B4:
    {
        unsigned int* d = dstBits;
        unsigned short* src = (unsigned short*)r.pBits;
        for (int y = 0; y < sy; ++y)
        {
            for (int x = 0; x < sx; ++x)
            {
                unsigned short c = src[x];
                unsigned char b = (unsigned char)(c << 4);
                unsigned short rgba = c;
                rgba >>= 4;
                unsigned char g = (unsigned char)rgba;
                rgba >>= 4;
                unsigned char a = (unsigned char)(rgba >> 4);
                a <<= 4;
                unsigned char rr = (unsigned char)rgba;
                rr <<= 4;
                g <<= 4;
                // The channel order of original: blue lands in the top byte, alpha, red, green follow.
                *d++ = (b << 24) | (a << 16) | (rr << 8) | g;
            }
            src += r.Pitch / 2;
        }
        break;
    }

    case D3DFMT_X8R8G8B8:
    {
        unsigned int* d = dstBits;
        unsigned int* src = (unsigned int*)r.pBits;
        for (int y = 0; y < sy; ++y)
        {
            for (int x = 0; x < sx; ++x)
                *d++ = src[x] | 0xff000000;
            src += r.Pitch / 4;
        }
        break;
    }

    case D3DFMT_A8R8G8B8:
    {
        unsigned int* d = dstBits;
        unsigned int* src = (unsigned int*)r.pBits;
        for (int i = 0; i < sy; ++i)
        {
            memcpy(d, src, sx * 4);
            d += sx;
            src += r.Pitch / 4;
        }
        break;
    }

    default:
        result = 0;
        break;
    }

    dest->UnlockRect();

    dest->Release();
    destTex->Release();

    return result;
}

// orig 0x640220 texture_man.cpp:1673
int CDevice::loadTexMaps(CTexture* tex, CStr const& str, unsigned int flags)
{
    CStr sstr(str);
    int i = sstr.find('%');

    if (i < 0)
    {
        CTexMap* tm = addTexMap(sstr, flags);
        if (!tm)
            return 0;

        tex->m_maps.push_back(tm);
    }
    else
    {
        int n = 0;
        while (sstr[i + n] == '%')
            n++;

        if (n > 3)
            n = 3;

        int k = 1;
        for (int kk = 0; kk < n; ++kk)
            k *= 10;

        for (int j = 0; j < k; ++j)
        {
            const char* fmt = 0;

            if (n == 1)
                fmt = "%.1d";
            else if (n == 2)
                fmt = "%.2d";
            else if (n == 3)
                fmt = "%.3d";

            char ss[4];
            sprintf(ss, fmt, j);

            for (int kk = 0; kk < n; ++kk)
                sstr[i + kk] = ss[kk];

            scoped_ptr<m3d::fs::FileStream> stream(g_kernel->GetFileServer().CreateFileStream());
            if (stream->Open(sstr.c_str(), m3d::fs::IStream::OPEN_READ))
            {
                stream->Close();

                CTexMap* tm = addTexMap(sstr, flags);
                if (!tm)
                    break;

                tex->m_maps.push_back(tm);
            }
            else
                break;
        }
    }

    return tex->m_maps.size() > 0;
}

// orig 0x641370 texture_man.cpp:1756
TexHandle CDevice::readShader(CStr const& name, unsigned int flags)
{
    TexHandle result;

    {
        scoped_ptr<m3d::fs::FileStream> stream(g_kernel->GetFileServer().CreateFileStream());
        if (!stream->Open(name.c_str(), m3d::fs::IStream::OPEN_READ))
        {
            result.SetInvalid();
            return result;
        }

        static char sign[] = "<!-- M3D_SHADER -->";
        char* buf = (char*)_alloca(sizeof(sign));
        stream->ReadBytes(buf, sizeof(sign) - 1);
        stream->Close();

        buf[sizeof(sign) - 1] = 0;

        if (strcmp(buf, sign) != 0)
        {
            result.SetInvalid();
            return result;
        }
    }

    ref_ptr<m3d::cmn::IniFile> parser(g_kernel->CreateIniFile());
    scoped_ptr<m3d::fs::FileStream> stream(g_kernel->GetFileServer().CreateFileStream());
    if (!stream->Open(name.c_str(), m3d::fs::IStream::OPEN_READ))
    {
        result.SetInvalid();
        return result;
    }

    parser->Read(*stream);
    stream->Close();

    const char* err = parser->GetError();
    if (err)
    {
        LogMsg(CStr("parse error: file: ") + name + " err: " + err);
        result.SetInvalid();
        return result;
    }

    int fps = parser->GetInteger("MAPS", "FPS");

    CStr str(parser->GetString("MAPS", "FRAME"));

    int looped = parser->GetInteger("MAPS", "LOOPED");

    if (str.empty())
    {
        LogMsg(CStr("shader: ") + name + " tex frames are not specified");
        result.SetInvalid();
        return result;
    }

    int i = newTexture();
    CTexture& tex = m_textures[i];

    if (!loadTexMaps(&tex, str.c_str(), flags))
    {
        result.SetInvalid();
        return result;
    }

    if (looped == 2)
    {
        for (int j = (int)tex.m_maps.size() - 1; j >= 0; --j)
        {
            tex.m_maps.push_back(tex.m_maps[j]);
            tex.m_maps[j]->addRef();
        }
    }

    tex.m_looped = looped;
    tex.m_fps = fps;

    tex.m_address[0] = D3DTADDRESS_WRAP;
    tex.m_address[1] = D3DTADDRESS_WRAP;
    tex.m_address[2] = D3DTADDRESS_WRAP;
    tex.m_maxAnisotropy = 1;
    tex.m_mipFilter = D3DTEXF_POINT;
    tex.m_magFilter = D3DTEXF_LINEAR;
    tex.m_minFilter = D3DTEXF_LINEAR;
    tex.m_borderColor = 0;
    tex.m_lodBias = 0.0f;
    tex.m_lodMax = 0;
    tex.m_refs = 0;
    tex.m_fileName = name;

    tex.addRef();

    ((InternalHandle<TexHandle>&)result).SetId(i);
    return result;
}

// orig 0x63f110 texture_man.cpp:1892
int CDevice::GetTextureName(TexHandle const& id, CStr& name)
{
    if (!IsTexValid(id))
    {
        name.erase();
        return 0;
    }

    name = m_textures[TexId(id)].m_fileName;

    return 1;
}

// orig 0x63d6c0 texture_man.cpp:1907
bool CDevice::CTexMap::Is2D()
{
    return m_type == TT_2D_FROM_FILE || m_type == TT_2D_DYNAMIC || m_type == TT_2D_RENDER_TARGET;
}

// orig 0x63d6e0 texture_man.cpp:1916
bool CDevice::CTexMap::IsCube()
{
    return m_type == TT_CUBE_FROM_FILE || m_type == TT_CUBE_DYNAMIC || m_type == TT_CUBE_RENDER_TARGET;
}

// orig 0x63d700 texture_man.cpp:1925
bool CDevice::CTexMap::Is3D()
{
    return m_type == TT_3D_FROM_FILE || m_type == TT_3D_DYNAMIC;
}

// orig 0x63d720 texture_man.cpp:1932 - m_texFormat[0][*] are the formats with alpha, [*][0] the
// compressed ones.
D3DFORMAT CDevice::GetTexFormat(bool useAlpha, bool compressed)
{
    if (useAlpha)
        return compressed ? m_texFormat[0][0] : m_texFormat[0][1];
    else
        return compressed ? m_texFormat[1][0] : m_texFormat[1][1];
}

// orig 0x63f160 texture_man.cpp:1942
int CDevice::ReloadTextures()
{
    int relCount = 0;

    for (unsigned int i = 0; i < m_texMaps.size(); ++i)
    {
        CTexMap* tm = m_texMaps[i];

        if (!tm)
            continue;

        switch (tm->m_type)
        {
        case TT_2D_DYNAMIC:
        case TT_2D_RENDER_TARGET:
        case TT_CUBE_DYNAMIC:
        case TT_CUBE_RENDER_TARGET:
        case TT_3D_DYNAMIC:
            continue;
        }

        unsigned int mipq = tm->m_flags & TM_MIPQ_MASK;
        scoped_ptr<m3d::fs::FileStream> fs(g_kernel->GetFileServer().CreateFileStream());

        if (!fs->Open(tm->m_fileName.c_str(), m3d::fs::IStream::OPEN_READ))
        {
            LogMsg(CStr("error reloading texture: cannot open ") + tm->m_fileName);
            continue;
        }

        unsigned int sz = fs->GetSize();

        FILETIME ftime = fs->GetDate();

        if (!m_reloadAllTextures && sz == tm->m_lastFileSize && ftime.dwHighDateTime == tm->m_lastFileDate.dwHighDateTime &&
            ftime.dwLowDateTime == tm->m_lastFileDate.dwLowDateTime)
            continue;

        unsigned char* p = new unsigned char[sz];
        fs->ReadBytes(p, sz);

        unsigned int mipLevels = D3DX_DEFAULT;
        unsigned int filter;
        switch (mipq)
        {
        default:  // TM_MIPQ_HIGH
            filter = D3DX_FILTER_TRIANGLE;
            break;
        case TM_MIPQ_NOMIPS:
            filter = D3DX_FILTER_NONE;
            mipLevels = 0;
            break;
        case TM_MIPQ_LOW:
            filter = D3DX_FILTER_POINT;
            break;
        }

        D3DXIMAGE_INFO nfo;
        D3DXGetImageInfoFromFileInMemory(p, sz, &nfo);

        TexType type = TT_2D_FROM_FILE;
        switch (nfo.ResourceType)
        {
        case D3DRTYPE_TEXTURE:
            type = TT_2D_FROM_FILE;
            break;
        case D3DRTYPE_CUBETEXTURE:
            type = TT_CUBE_FROM_FILE;
            break;
        case D3DRTYPE_VOLUMETEXTURE:
            type = TT_3D_FROM_FILE;
            break;
        }

        if (type != tm->m_type)
        {
            LogMsg(CStr("error reloading texture: types do not match for ") + tm->m_fileName);
            delete[] p;
            continue;
        }

        CTexMap* ntm = new CTexMap;

        HRESULT hr = 0;
        switch (type)
        {
        case TT_2D_FROM_FILE:
            hr = D3DXCreateTextureFromFileInMemoryEx(m_pd3dDevice, p, sz, D3DX_DEFAULT, D3DX_DEFAULT, mipLevels, 0,
                                                    tm->m_fmt, tm->m_pool, filter, filter, 0, 0, 0, &ntm->m_pTex2d);
            break;
        case TT_CUBE_FROM_FILE:
            hr = D3DXCreateCubeTextureFromFileInMemoryEx(m_pd3dDevice, p, sz, D3DX_DEFAULT, mipLevels, 0, tm->m_fmt,
                                                        tm->m_pool, filter, filter, 0, 0, 0, &ntm->m_pTexCube);
            break;
        case TT_3D_FROM_FILE:
            hr = D3DXCreateVolumeTextureFromFileInMemoryEx(m_pd3dDevice, p, sz, D3DX_DEFAULT, D3DX_DEFAULT,
                                                          D3DX_DEFAULT, mipLevels, 0, tm->m_fmt, tm->m_pool, filter,
                                                          filter, 0, 0, 0, &ntm->m_pTex3d);
            break;
        }

        delete[] p;

        if (FAILED(hr))
        {
            // The original passes the CStr file name itself to the "%s" (its first member is the char pointer).
            g_kernel->KernelLog("error reloading texture: CreateTextureFromFileInMemoryEx error: %s file: %s",
                                getD3dErrorStr(hr).c_str(), tm->m_fileName.c_str());
            delete ntm;
            continue;
        }

        ntm->m_fileName = tm->m_fileName;
        ntm->m_pool = D3DPOOL_MANAGED;
        ntm->m_usage = 0;
        ntm->m_flags = tm->m_flags;
        ntm->m_lastFileSize = sz;
        ntm->m_lastFileDate = ftime;

        switch (type)
        {
        case TT_2D_FROM_FILE:
            ntm->m_pTex2d->GetLevelDesc(0, &ntm->m_desc2d);
            break;
        case TT_CUBE_FROM_FILE:
            ntm->m_pTexCube->GetLevelDesc(0, &ntm->m_desc2d);
            break;
        case TT_3D_FROM_FILE:
            ntm->m_pTex3d->GetLevelDesc(0, &ntm->m_desc3d);
            break;
        }

        ntm->m_type = type;
        tm->freeTex();
        ntm->m_refs = tm->m_refs;

        // Repoint every texture that uses the old map.
        for (unsigned int j = 0; j < m_textures.size(); ++j)
        {
            CTexture& tex = m_textures[j];

            for (unsigned int k = 0; k < tex.m_maps.size(); ++k)
            {
                if (tex.m_maps[k] == tm)
                    tex.m_maps[k] = ntm;
            }
        }

        delete m_texMaps[i];
        m_texMaps[i] = 0;

        relCount++;
        m_texMaps[i] = ntm;
    }

    m_reloadAllTextures = false;
    return relCount;
}

// orig 0x63d760 texture_man.cpp:2194
int CDevice::GetMaxAnisotropy()
{
    return m_d3dCaps.MaxAnisotropy;
}

// orig 0x63f6b0 texture_man.cpp:2200
void CDevice::CreateMips(TexHandle const& tex)
{
    if (!IsTexValid(tex))
        return;

    CTexMap* map = m_textures[TexId(tex)].m_maps[0];

    IDirect3DSurface9* baseSurf = 0;
    if (FAILED(map->m_pTex2d->GetSurfaceLevel(0, &baseSurf)))
    {
        LogMsg("cannot acquire surfaces to copy");
        return;
    }

    for (unsigned int i = 1; i < map->m_pTex->GetLevelCount(); ++i)
    {
        IDirect3DSurface9* mipSurf = 0;
        if (FAILED(map->m_pTex2d->GetSurfaceLevel(i, &mipSurf)))
        {
            LogMsg("cannot acquire surfaces to copy");
            return;
        }

        m_lastResult = D3DXLoadSurfaceFromSurface(mipSurf, 0, 0, baseSurf, 0, 0, D3DX_FILTER_TRIANGLE, 0);

        if (FAILED(m_lastResult))
            LogMsg(CStr("texcopy failed, result = ") + CStr((int)m_lastResult));

        baseSurf->Release();
        baseSurf = mipSurf;
    }

    baseSurf->Release();
}

// orig 0x63f920 texture_man.cpp:2250
void CDevice::RepaintAllTexturesMips()
{
    IDirect3DSurface9* srcSurf = 0;
    IDirect3DSurface9* dstSurf = 0;

    TexHandle texMips = AddTexture(CStr("data/mips.dds"), TM_MIPQ_HIGH);

    for (unsigned int i = 0; i < m_textures.size(); ++i)
    {
        CTexture& tex = m_textures[i];

        if (tex.m_maps.empty())
            continue;

        CTexMap* map = tex.m_maps[0];

        if (map->m_type != TT_2D_FROM_FILE)
            continue;

        for (unsigned int kk = 0; kk < map->m_pTex->GetLevelCount(); ++kk)
        {
            if (map->Is2D())
            {
                if (FAILED(map->m_pTex2d->GetSurfaceLevel(kk, &dstSurf)) ||
                    FAILED(m_textures[TexId(texMips)].m_maps[0]->m_pTex2d->GetSurfaceLevel(kk > 9 ? 9 : kk, &srcSurf)))
                {
                    LogMsg(CStr("cannot acquire surfaces to copy with mip ") + CStr(kk));
                    return;
                }

                m_lastResult = D3DXLoadSurfaceFromSurface(dstSurf, 0, 0, srcSurf, 0, 0, D3DX_FILTER_POINT, 0);
                dstSurf->Release();
                srcSurf->Release();
            }
        }
    }

    if (texMips.IsValid())
        ReleaseTexture(texMips);

    m_reloadAllTextures = true;
}
