#include <cstring>
#include <config.h>
#include <core/kernel.h>
#include "device.h"
#include "log.h"
#include "query.h"
#include "shaders/assembly/asm_shader.h"
#include "shaders/effects/effect.h"
#include "shaders/hlsl/hlsl_shader.h"
#include <core/ini.h>
#include <core/ref_ptr.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <core/console/cvar.h>

// The Direct3D 9 device: construction, Create/CreateDevice, device reset and display mode
// switching, the format finders and capability log, the render-state stacks and the Direct3D
// state cache, texture stages and texture coordinate generation, viewport, stats, back buffer
// textures, hardware cursor and the single layer stencil.
// Ported from the original dxrender9/device.cpp; the functions are
// in the order of their original source lines.

namespace
{
    // The failure path of the original M3D_ASSERT: Kernel::SysError(assertion, file, line). HTA's
    // kernel has SysError(whence, descr); the message is assembled the way HTA's SYS_ERROR does.
    void deviceAssertFailed(char const* assertion, char const* file, int line)
    {
        g_kernel->SysError(CStr(file) + CStr(":") + CStr(line), CStr(assertion));
    }
}


// The m3d -> Direct3D state tables of the original device.cpp (file statics, emitted in this
// order in the original binary; they precede the stack functions that index them).

// orig 0x8522a0 device.cpp (file static)
static D3DCULL m3dCullToD3dCull[3] = { D3DCULL_NONE, D3DCULL_CW, D3DCULL_CCW };

// orig 0x8522ac device.cpp (file static)
static D3DCMPFUNC m3dCmpToD3dCmp[8] = {
    D3DCMP_NEVER,        // M3DCMP_NEVER
    D3DCMP_LESS,         // M3DCMP_LESS
    D3DCMP_EQUAL,        // M3DCMP_EQUAL
    D3DCMP_LESSEQUAL,    // M3DCMP_LESSEQUAL
    D3DCMP_GREATER,      // M3DCMP_GREATER
    D3DCMP_NOTEQUAL,     // M3DCMP_NOTEQUAL
    D3DCMP_GREATEREQUAL, // M3DCMP_GREATEREQUAL
    D3DCMP_ALWAYS,       // M3DCMP_ALWAYS
};

// orig 0x8522cc device.cpp (file static)
static D3DFILLMODE m3dFmToD3dFm[3] = { D3DFILL_POINT, D3DFILL_WIREFRAME, D3DFILL_SOLID };

// orig 0x8522d8 device.cpp (file static)
static D3DFOGMODE m3dFogMToD3dFogM[4] = { D3DFOG_NONE, D3DFOG_EXP, D3DFOG_EXP2, D3DFOG_LINEAR };

// orig 0x8522e8 device.cpp (file static)
static D3DSHADEMODE m3dSmToD3dSm[2] = { D3DSHADE_FLAT, D3DSHADE_GOURAUD };

// orig 0x8522f0 device.cpp (file static) -- {src, dst} per BlendMode (the PDB prints the dims
// inner-first as _D3DBLEND[2][124]; SetBlend reads srcDst[c][0] / srcDst[c][1])
static D3DBLEND srcDst[124][2] = {
    { D3DBLEND_ZERO, D3DBLEND_ZERO },  // BM_NONE = 0
    { D3DBLEND_ONE, D3DBLEND_ONE },  // BM_COLOR = 1
    { D3DBLEND_SRCALPHA, D3DBLEND_INVSRCALPHA },  // BM_ALPHA = 2
    { D3DBLEND_ZERO, D3DBLEND_ZERO },  // BM_0_0 = 3
    { D3DBLEND_ZERO, D3DBLEND_ONE },  // BM_0_1 = 4
    { D3DBLEND_ZERO, D3DBLEND_SRCCOLOR },  // BM_0_SCOLOR = 5
    { D3DBLEND_ZERO, D3DBLEND_INVSRCCOLOR },  // BM_0_ISCOLOR = 6
    { D3DBLEND_ZERO, D3DBLEND_SRCALPHA },  // BM_0_SALPHA = 7
    { D3DBLEND_ZERO, D3DBLEND_INVSRCALPHA },  // BM_0_ISALPHA = 8
    { D3DBLEND_ZERO, D3DBLEND_DESTALPHA },  // BM_0_DALPHA = 9
    { D3DBLEND_ZERO, D3DBLEND_INVDESTALPHA },  // BM_0_IDALPHA = 10
    { D3DBLEND_ZERO, D3DBLEND_DESTCOLOR },  // BM_0_DCOLOR = 11
    { D3DBLEND_ZERO, D3DBLEND_INVDESTCOLOR },  // BM_0_IDCOLOR = 12
    { D3DBLEND_ZERO, D3DBLEND_SRCALPHASAT },  // BM_0_SALPHASAT = 13
    { D3DBLEND_ONE, D3DBLEND_ZERO },  // BM_1_0 = 14
    { D3DBLEND_ONE, D3DBLEND_ONE },  // BM_1_1 = 15
    { D3DBLEND_ONE, D3DBLEND_SRCCOLOR },  // BM_1_SCOLOR = 16
    { D3DBLEND_ONE, D3DBLEND_INVSRCCOLOR },  // BM_1_ISCOLOR = 17
    { D3DBLEND_ONE, D3DBLEND_SRCALPHA },  // BM_1_SALPHA = 18
    { D3DBLEND_ONE, D3DBLEND_INVSRCALPHA },  // BM_1_ISALPHA = 19
    { D3DBLEND_ONE, D3DBLEND_DESTALPHA },  // BM_1_DALPHA = 20
    { D3DBLEND_ONE, D3DBLEND_INVDESTALPHA },  // BM_1_IDALPHA = 21
    { D3DBLEND_ONE, D3DBLEND_DESTCOLOR },  // BM_1_DCOLOR = 22
    { D3DBLEND_ONE, D3DBLEND_INVDESTCOLOR },  // BM_1_IDCOLOR = 23
    { D3DBLEND_ONE, D3DBLEND_SRCALPHASAT },  // BM_1_SALPHASAT = 24
    { D3DBLEND_SRCCOLOR, D3DBLEND_ZERO },  // BM_SCOLOR_0 = 25
    { D3DBLEND_SRCCOLOR, D3DBLEND_ONE },  // BM_SCOLOR_1 = 26
    { D3DBLEND_SRCCOLOR, D3DBLEND_SRCCOLOR },  // BM_SCOLOR_SCOLOR = 27
    { D3DBLEND_SRCCOLOR, D3DBLEND_INVSRCCOLOR },  // BM_SCOLOR_ISCOLOR = 28
    { D3DBLEND_SRCCOLOR, D3DBLEND_SRCALPHA },  // BM_SCOLOR_SALPHA = 29
    { D3DBLEND_SRCCOLOR, D3DBLEND_INVSRCALPHA },  // BM_SCOLOR_ISALPHA = 30
    { D3DBLEND_SRCCOLOR, D3DBLEND_DESTALPHA },  // BM_SCOLOR_DALPHA = 31
    { D3DBLEND_SRCCOLOR, D3DBLEND_INVDESTALPHA },  // BM_SCOLOR_IDALPHA = 32
    { D3DBLEND_SRCCOLOR, D3DBLEND_DESTCOLOR },  // BM_SCOLOR_DCOLOR = 33
    { D3DBLEND_SRCCOLOR, D3DBLEND_INVDESTCOLOR },  // BM_SCOLOR_IDCOLOR = 34
    { D3DBLEND_SRCCOLOR, D3DBLEND_SRCALPHASAT },  // BM_SCOLOR_SALPHASAT = 35
    { D3DBLEND_INVSRCCOLOR, D3DBLEND_ZERO },  // BM_ISCOLOR_0 = 36
    { D3DBLEND_INVSRCCOLOR, D3DBLEND_ONE },  // BM_ISCOLOR_1 = 37
    { D3DBLEND_INVSRCCOLOR, D3DBLEND_SRCCOLOR },  // BM_ISCOLOR_SCOLOR = 38
    { D3DBLEND_INVSRCCOLOR, D3DBLEND_INVSRCCOLOR },  // BM_ISCOLOR_ISCOLOR = 39
    { D3DBLEND_INVSRCCOLOR, D3DBLEND_SRCALPHA },  // BM_ISCOLOR_SALPHA = 40
    { D3DBLEND_INVSRCCOLOR, D3DBLEND_INVSRCALPHA },  // BM_ISCOLOR_ISALPHA = 41
    { D3DBLEND_INVSRCCOLOR, D3DBLEND_DESTALPHA },  // BM_ISCOLOR_DALPHA = 42
    { D3DBLEND_INVSRCCOLOR, D3DBLEND_INVDESTALPHA },  // BM_ISCOLOR_IDALPHA = 43
    { D3DBLEND_INVSRCCOLOR, D3DBLEND_DESTCOLOR },  // BM_ISCOLOR_DCOLOR = 44
    { D3DBLEND_INVSRCCOLOR, D3DBLEND_INVDESTCOLOR },  // BM_ISCOLOR_IDCOLOR = 45
    { D3DBLEND_INVSRCCOLOR, D3DBLEND_SRCALPHASAT },  // BM_ISCOLOR_SALPHASAT = 46
    { D3DBLEND_SRCALPHA, D3DBLEND_ZERO },  // BM_SALPHA_0 = 47
    { D3DBLEND_SRCALPHA, D3DBLEND_ONE },  // BM_SALPHA_1 = 48
    { D3DBLEND_SRCALPHA, D3DBLEND_SRCCOLOR },  // BM_SALPHA_SCOLOR = 49
    { D3DBLEND_SRCALPHA, D3DBLEND_INVSRCCOLOR },  // BM_SALPHA_ISCOLOR = 50
    { D3DBLEND_SRCALPHA, D3DBLEND_SRCALPHA },  // BM_SALPHA_SALPHA = 51
    { D3DBLEND_SRCALPHA, D3DBLEND_INVSRCALPHA },  // BM_SALPHA_ISALPHA = 52
    { D3DBLEND_SRCALPHA, D3DBLEND_DESTALPHA },  // BM_SALPHA_DALPHA = 53
    { D3DBLEND_SRCALPHA, D3DBLEND_INVDESTALPHA },  // BM_SALPHA_IDALPHA = 54
    { D3DBLEND_SRCALPHA, D3DBLEND_DESTCOLOR },  // BM_SALPHA_DCOLOR = 55
    { D3DBLEND_SRCALPHA, D3DBLEND_INVDESTCOLOR },  // BM_SALPHA_IDCOLOR = 56
    { D3DBLEND_SRCALPHA, D3DBLEND_SRCALPHASAT },  // BM_SALPHA_SALPHASAT = 57
    { D3DBLEND_INVSRCALPHA, D3DBLEND_ZERO },  // BM_ISALPHA_0 = 58
    { D3DBLEND_INVSRCALPHA, D3DBLEND_ONE },  // BM_ISALPHA_1 = 59
    { D3DBLEND_INVSRCALPHA, D3DBLEND_SRCCOLOR },  // BM_ISALPHA_SCOLOR = 60
    { D3DBLEND_INVSRCALPHA, D3DBLEND_INVSRCCOLOR },  // BM_ISALPHA_ISCOLOR = 61
    { D3DBLEND_INVSRCALPHA, D3DBLEND_SRCALPHA },  // BM_ISALPHA_SALPHA = 62
    { D3DBLEND_INVSRCALPHA, D3DBLEND_INVSRCALPHA },  // BM_ISALPHA_ISALPHA = 63
    { D3DBLEND_INVSRCALPHA, D3DBLEND_DESTALPHA },  // BM_ISALPHA_DALPHA = 64
    { D3DBLEND_INVSRCALPHA, D3DBLEND_INVDESTALPHA },  // BM_ISALPHA_IDALPHA = 65
    { D3DBLEND_INVSRCALPHA, D3DBLEND_DESTCOLOR },  // BM_ISALPHA_DCOLOR = 66
    { D3DBLEND_INVSRCALPHA, D3DBLEND_INVDESTCOLOR },  // BM_ISALPHA_IDCOLOR = 67
    { D3DBLEND_INVSRCALPHA, D3DBLEND_SRCALPHASAT },  // BM_ISALPHA_SALPHASAT = 68
    { D3DBLEND_DESTALPHA, D3DBLEND_ZERO },  // BM_DALPHA_0 = 69
    { D3DBLEND_DESTALPHA, D3DBLEND_ONE },  // BM_DALPHA_1 = 70
    { D3DBLEND_DESTALPHA, D3DBLEND_SRCCOLOR },  // BM_DALPHA_SCOLOR = 71
    { D3DBLEND_DESTALPHA, D3DBLEND_INVSRCCOLOR },  // BM_DALPHA_ISCOLOR = 72
    { D3DBLEND_DESTALPHA, D3DBLEND_SRCALPHA },  // BM_DALPHA_SALPHA = 73
    { D3DBLEND_DESTALPHA, D3DBLEND_INVSRCALPHA },  // BM_DALPHA_ISALPHA = 74
    { D3DBLEND_DESTALPHA, D3DBLEND_DESTALPHA },  // BM_DALPHA_DALPHA = 75
    { D3DBLEND_DESTALPHA, D3DBLEND_INVDESTALPHA },  // BM_DALPHA_IDALPHA = 76
    { D3DBLEND_DESTALPHA, D3DBLEND_DESTCOLOR },  // BM_DALPHA_DCOLOR = 77
    { D3DBLEND_DESTALPHA, D3DBLEND_INVDESTCOLOR },  // BM_DALPHA_IDCOLOR = 78
    { D3DBLEND_DESTALPHA, D3DBLEND_SRCALPHASAT },  // BM_DALPHA_SALPHASAT = 79
    { D3DBLEND_INVDESTALPHA, D3DBLEND_ZERO },  // BM_IDALPHA_0 = 80
    { D3DBLEND_INVDESTALPHA, D3DBLEND_ONE },  // BM_IDALPHA_1 = 81
    { D3DBLEND_INVDESTALPHA, D3DBLEND_SRCCOLOR },  // BM_IDALPHA_SCOLOR = 82
    { D3DBLEND_INVDESTALPHA, D3DBLEND_INVSRCCOLOR },  // BM_IDALPHA_ISCOLOR = 83
    { D3DBLEND_INVDESTALPHA, D3DBLEND_SRCALPHA },  // BM_IDALPHA_SALPHA = 84
    { D3DBLEND_INVDESTALPHA, D3DBLEND_INVSRCALPHA },  // BM_IDALPHA_ISALPHA = 85
    { D3DBLEND_INVDESTALPHA, D3DBLEND_DESTALPHA },  // BM_IDALPHA_DALPHA = 86
    { D3DBLEND_INVDESTALPHA, D3DBLEND_INVDESTALPHA },  // BM_IDALPHA_IDALPHA = 87
    { D3DBLEND_INVDESTALPHA, D3DBLEND_DESTCOLOR },  // BM_IDALPHA_DCOLOR = 88
    { D3DBLEND_INVDESTALPHA, D3DBLEND_INVDESTCOLOR },  // BM_IDALPHA_IDCOLOR = 89
    { D3DBLEND_INVDESTALPHA, D3DBLEND_SRCALPHASAT },  // BM_IDALPHA_SALPHASAT = 90
    { D3DBLEND_DESTCOLOR, D3DBLEND_ZERO },  // BM_DCOLOR_0 = 91
    { D3DBLEND_DESTCOLOR, D3DBLEND_ONE },  // BM_DCOLOR_1 = 92
    { D3DBLEND_DESTCOLOR, D3DBLEND_SRCCOLOR },  // BM_DCOLOR_SCOLOR = 93
    { D3DBLEND_DESTCOLOR, D3DBLEND_INVSRCCOLOR },  // BM_DCOLOR_ISCOLOR = 94
    { D3DBLEND_DESTCOLOR, D3DBLEND_SRCALPHA },  // BM_DCOLOR_SALPHA = 95
    { D3DBLEND_DESTCOLOR, D3DBLEND_INVSRCALPHA },  // BM_DCOLOR_ISALPHA = 96
    { D3DBLEND_DESTCOLOR, D3DBLEND_DESTALPHA },  // BM_DCOLOR_DALPHA = 97
    { D3DBLEND_DESTCOLOR, D3DBLEND_INVDESTALPHA },  // BM_DCOLOR_IDALPHA = 98
    { D3DBLEND_DESTCOLOR, D3DBLEND_DESTCOLOR },  // BM_DCOLOR_DCOLOR = 99
    { D3DBLEND_DESTCOLOR, D3DBLEND_INVDESTCOLOR },  // BM_DCOLOR_IDCOLOR = 100
    { D3DBLEND_DESTCOLOR, D3DBLEND_SRCALPHASAT },  // BM_DCOLOR_SALPHASAT = 101
    { D3DBLEND_INVDESTCOLOR, D3DBLEND_ZERO },  // BM_IDCOLOR_0 = 102
    { D3DBLEND_INVDESTCOLOR, D3DBLEND_ONE },  // BM_IDCOLOR_1 = 103
    { D3DBLEND_INVDESTCOLOR, D3DBLEND_SRCCOLOR },  // BM_IDCOLOR_SCOLOR = 104
    { D3DBLEND_INVDESTCOLOR, D3DBLEND_INVSRCCOLOR },  // BM_IDCOLOR_ISCOLOR = 105
    { D3DBLEND_INVDESTCOLOR, D3DBLEND_SRCALPHA },  // BM_IDCOLOR_SALPHA = 106
    { D3DBLEND_INVDESTCOLOR, D3DBLEND_INVSRCALPHA },  // BM_IDCOLOR_ISALPHA = 107
    { D3DBLEND_INVDESTCOLOR, D3DBLEND_DESTALPHA },  // BM_IDCOLOR_DALPHA = 108
    { D3DBLEND_INVDESTCOLOR, D3DBLEND_INVDESTALPHA },  // BM_IDCOLOR_IDALPHA = 109
    { D3DBLEND_INVDESTCOLOR, D3DBLEND_DESTCOLOR },  // BM_IDCOLOR_DCOLOR = 110
    { D3DBLEND_INVDESTCOLOR, D3DBLEND_INVDESTCOLOR },  // BM_IDCOLOR_IDCOLOR = 111
    { D3DBLEND_INVDESTCOLOR, D3DBLEND_SRCALPHASAT },  // BM_IDCOLOR_SALPHASAT = 112
    { D3DBLEND_SRCALPHASAT, D3DBLEND_ZERO },  // BM_SALPHASAT_0 = 113
    { D3DBLEND_SRCALPHASAT, D3DBLEND_ONE },  // BM_SALPHASAT_1 = 114
    { D3DBLEND_SRCALPHASAT, D3DBLEND_SRCCOLOR },  // BM_SALPHASAT_SCOLOR = 115
    { D3DBLEND_SRCALPHASAT, D3DBLEND_INVSRCCOLOR },  // BM_SALPHASAT_ISCOLOR = 116
    { D3DBLEND_SRCALPHASAT, D3DBLEND_SRCALPHA },  // BM_SALPHASAT_SALPHA = 117
    { D3DBLEND_SRCALPHASAT, D3DBLEND_INVSRCALPHA },  // BM_SALPHASAT_ISALPHA = 118
    { D3DBLEND_SRCALPHASAT, D3DBLEND_DESTALPHA },  // BM_SALPHASAT_DALPHA = 119
    { D3DBLEND_SRCALPHASAT, D3DBLEND_INVDESTALPHA },  // BM_SALPHASAT_IDALPHA = 120
    { D3DBLEND_SRCALPHASAT, D3DBLEND_DESTCOLOR },  // BM_SALPHASAT_DCOLOR = 121
    { D3DBLEND_SRCALPHASAT, D3DBLEND_INVDESTCOLOR },  // BM_SALPHASAT_IDCOLOR = 122
    { D3DBLEND_SRCALPHASAT, D3DBLEND_SRCALPHASAT },  // BM_SALPHASAT_SALPHASAT = 123
};

// orig 0x8526d0 device.cpp (file static)
static D3DSTENCILOP m3dStencilOpD3dStencilOp[8] = {
    D3DSTENCILOP_KEEP,    // OP_KEEP
    D3DSTENCILOP_ZERO,    // OP_ZERO
    D3DSTENCILOP_REPLACE, // OP_REPLACE
    D3DSTENCILOP_INCRSAT, // OP_INCRSAT
    D3DSTENCILOP_DECRSAT, // OP_DECRSAT
    D3DSTENCILOP_INVERT,  // OP_INVERT
    D3DSTENCILOP_INCR,    // OP_INCR
    D3DSTENCILOP_DECR,    // OP_DECR
};

// ------------------------------------------------------------------------------------------------
// The render state stacks. Each state X has PushX() (duplicate the top), PopX(), SetX(c, force)
// and PushX(c); all four of a state come from one macro line in the original source. The
// SetX bodies call setRenderState (inlined in the binary): nothing is sent to Direct3D when the
// cached value is unchanged, and `force` is not read by any of them.



// orig 0x60cc10 device.cpp:287
int CDevice::IncRef()
{
    if (m_parent)
    {
        m_parent->IncRef();
    }
    return ++m_refCount;
}

// orig 0x60cc30 device.cpp:287
int CDevice::DecRef()
{
    int refs = --m_refCount;
    if (m_parent)
    {
        m_parent->DecRef();
    }
    if (m_refCount <= 0)
    {
        delete this;
    }
    return refs;
}

// orig 0x60cc60 device.cpp:287
void* CDevice::QueryIface(const char* ifaceName)
{
    if (m_parent)
    {
        return m_parent->QueryIface(ifaceName);
    }
    return 0;
}

// orig 0x61dbc0 device.cpp:294
CDevice::CDevice() :
    m_VbPoolSize(0x10000),
    m_IbPoolSize(0x10000)
{
    // 304
    m_refCount = 0;
    m_parent = 0;
    m_pD3D = 0;
    m_pd3dDevice = 0;
    m_inScene = 0;
    m_lastResult = 0;
    m_activeStencilTarget = 0;
    InitMatrices();

    // 307
    memset(&m_stats, 0, sizeof(m_stats));

    // 309
    m_rtsPtr = 0;
    m_rtsSaveZs = 0;
    m_rtsSaveColor = 0;
    m_rtsNewZs = 0;

    // 314
    m_isActive = 0;

    // 316
    m_NormalizingCubemap = 0;
    m_SpecularPowerLookup = 0;

    // 319
    memset(m_lightsEnabled, 0, sizeof(m_lightsEnabled));

    // 321
    m_curVertexShader = (IDirect3DVertexShader9*)-1;
    m_curPixelShader = (IDirect3DPixelShader9*)-1;
    m_curFVF = 0xffffffff;

    // 325
    m_latchedIb = 0;

    // 327
    m_latchedCheck = true;

    // 329
    m_presents = 0;

    // 338
    EffectImpl::m_dev = this;
    HlslShaderImpl::m_dev = this;
    AsmShaderImpl::m_dev = this;
    Query::m_dev = this;
    m_stateManager.SetDevice(this);

    // 341
    m_fsRt = 0;
    m_fsRtZBuffer = 0;
    m_fsRtSurf = 0;

    // 345
    m_userClipPlaneEnabled = 0;

    // 348
    for (int i = 0; i < 6; ++i)
    {
        m_userClipPlanesUpdated[i] = false;
    }

    // 352
    m_globalFxPool = 0;

    // 359
    m_pSysFont = 0;

    // 361
    m_reloadAllTextures = false;

    // 363
    m_stencilLevel[0] = -1;
    m_stencilLevel[1] = -1;

    // 366
    m_renderThreadId = 0;
    m_bThreadSafeGuardEnabled = true;
}

// orig 0x61d360 device.cpp:374
CDevice::~CDevice()
{
    // 376
    if (m_pSysFont)
    {
        m_pSysFont->Release();
        m_pSysFont = 0;
    }

    // 378
    if (m_texFrameBufer.IsValid())
    {
        ReleaseTexture(m_texFrameBufer);
    }

    // 381
    for (std::map<int, TexHandle>::iterator it = m_texBufferedRT.begin(); it != m_texBufferedRT.end(); ++it)
    {
        ReleaseTexture(it->second);
    }

    // 386
    ReleaseTexture(m_currentDXCursorInfo.m_texId);

    // 389: the map arrays of the textures are freed (the maps themselves are deleted below)
    for (unsigned int i = 0; i < m_textures.size(); ++i)
    {
        // 400
        std::vector<CTexMap*>().swap(m_textures[i].m_maps);
    }

    // 404
    for (unsigned int i = 0; i < m_texMaps.size(); ++i)
    {
        // 406
        CTexMap* map = m_texMaps[i];
        // 407
        map->freeTex();
        // 408
        delete map;
    }
    // 411
    std::vector<CTexMap*>().swap(m_texMaps);

    // 414
    DoneVertexDeclarations();

    // 417
    doneFullScreenQuadStuff();

    // 429: the original bounds this loop by m_VbPoolBuffers.size() (its unchecked operator[] then read past
    // the end of m_IbPoolBuffers and ReleaseIb rejected the garbage handles); with a bounds-checked
    // vector that is an abort at shutdown, so the loop is bounded by the index pool count
    // (retruxx adaptation, no behavioural difference).
    for (unsigned int i = 0; i < m_IbPoolBuffers.size(); ++i)
    {
        // 430
        ReleaseIb(m_IbPoolBuffers[i]);
    }

    // 433
    for (unsigned int i = 0; i < m_ibs.size(); ++i)
    {
        // 440
        if (m_ibs[i].m_ib)
        {
            m_ibs[i].m_ib->Release();
            m_ibs[i].m_ib = 0;
        }
    }

    // 444
    for (unsigned int i = 0; i < m_VbPoolBuffers.size(); ++i)
    {
        // 454
        for (unsigned int j = 0; j < m_VbPoolBuffers[i].size(); ++j)
        {
            // 455
            ReleaseVb(m_VbPoolBuffers[i][j]);
        }
    }

    // 459
    for (unsigned int i = 0; i < m_vbs.size(); ++i)
    {
        // 466
        if (m_vbs[i].m_vb)
        {
            m_vbs[i].m_vb->Release();
            m_vbs[i].m_vb = 0;
        }
    }

    // 478
    for (unsigned int i = 0; i < m_meshes.size(); ++i)
    {
        // 480
        if (m_meshes[i].m_mesh)
        {
            m_meshes[i].m_mesh->Release();
            m_meshes[i].m_mesh = 0;
        }
    }

    // 484
    SetGamma(0.5f, 0.5f, 0.5f);

    // 487
    for (std::map<unsigned int, IDirect3DSurface9*>::iterator it = m_rtsZSurfaces.begin(); it != m_rtsZSurfaces.end();
         ++it)
    {
        // 488
        if (it->second)
        {
            it->second->Release();
            it->second = 0;
        }
    }
    // 489
    m_rtsZSurfaces.clear();

    // 492
    if (m_globalFxPool)
    {
        m_globalFxPool->Release();
        m_globalFxPool = 0;
    }

    // 495
    if (m_NormalizingCubemap)
    {
        m_NormalizingCubemap->Release();
        m_NormalizingCubemap = 0;
    }
    // 496
    if (m_SpecularPowerLookup)
    {
        m_SpecularPowerLookup->Release();
        m_SpecularPowerLookup = 0;
    }

    // 499
    if (m_pd3dDevice)
    {
        m_pd3dDevice->Release();
        m_pd3dDevice = 0;
    }
    // 500
    if (m_pD3D)
    {
        m_pD3D->Release();
        m_pD3D = 0;
    }

    // 503
    ReleaseFsRt();
}

// orig 0x61a790 device.cpp:509
int CDevice::Create(void(__fastcall* logFunc)(CStr const&), m3d::Kernel* kernel)
{
    // HTA hands over the log callback and the kernel; the original's Create() took nothing and logged
    // through the statically linked kernel. createIRenderer has already stored the kernel.
    SetLogFunc(logFunc);
    (void)kernel;

    // 527
    m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

    // 530
    m_lastResult = m_pD3D->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &m_d3dCaps);
    // 531
    if (FAILED(m_lastResult))
    {
        // 533
        LogMsg(CStr("Could not get device caps for the default device, hr = ") + CStr::format_("%x", m_lastResult));
        // 534
        return 0;
    }

    // 537
    if ((m_d3dCaps.PixelShaderVersion & 0xffff) < 0x0101)
    {
        // 540
        g_kernel->MessageBoxA(0, "Cannot initialize 3d: this application requires Pixel Shader 1.1 or greater", "error",
                              0);
        // 541
        LogMsg("Could not initialize 3d: no PS11 support found");
        // 542
        return 0;
    }

    // 546
    if ((m_d3dCaps.TextureCaps & D3DPTEXTURECAPS_POW2) && !(m_d3dCaps.TextureCaps & D3DPTEXTURECAPS_NONPOW2CONDITIONAL))
    {
        // 549
        g_kernel->MessageBoxA(0, "Cannot initialize 3d: requires some non pow2 textures ", "error", 0);
        // 550
        LogMsg("Could not initialize 3d: no D3DPTEXTURECAPS_NONPOW2CONDITIONAL support found");
        // 551
        return 0;
    }

    // 554
    if (FAILED(D3DXCreateEffectPool(&m_globalFxPool)))
    {
        // 556
        m_globalFxPool = 0;
        // 557
        LogMsg("Failed to create global fx pool!");
    }

    // 563
    return m_pD3D != 0;
}

// orig 0x60cc80 device.cpp:569
int CDevice::FindDepthFormat(D3DFORMAT& depthFormat, D3DFORMAT TargetFormat, D3DFORMAT const* fmtDepthArray,
                             int const* fmtDepthArrayBits, int numDepthFmts, int bpp)
{
    for (int iFmt = 0; iFmt < numDepthFmts; iFmt++)
    {
        m_lastResult = m_pD3D->CheckDeviceFormat(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, TargetFormat,
                                                 D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, fmtDepthArray[iFmt]);
        if (SUCCEEDED(m_lastResult) && fmtDepthArrayBits[iFmt] == bpp)
        {
            m_lastResult = m_pD3D->CheckDepthStencilMatch(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, TargetFormat,
                                                          TargetFormat, fmtDepthArray[iFmt]);
            if (SUCCEEDED(m_lastResult))
            {
                depthFormat = fmtDepthArray[iFmt];
                return 1;
            }
        }
    }

    for (int iFmt = 0; iFmt < numDepthFmts; iFmt++)
    {
        m_lastResult = m_pD3D->CheckDeviceFormat(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, TargetFormat,
                                                 D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, fmtDepthArray[iFmt]);
        if (SUCCEEDED(m_lastResult))
        {
            m_lastResult = m_pD3D->CheckDepthStencilMatch(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, TargetFormat,
                                                          TargetFormat, fmtDepthArray[iFmt]);
            if (SUCCEEDED(m_lastResult))
            {
                depthFormat = fmtDepthArray[iFmt];
                return 1;
            }
        }
    }

    return 0;
}

// orig 0x60cdb0 device.cpp:608
int CDevice::FindDepthFormat(D3DFORMAT TargetFormat, D3DFORMAT* pDepthStencilFormat, int bpp)
{
    static D3DFORMAT const fmtDepthArray[6] = {
        D3DFMT_D24S8, D3DFMT_D15S1, D3DFMT_D24X4S4, D3DFMT_D24X8, D3DFMT_D16, D3DFMT_D32,
    };
    static int const fmtDepthArrayBits[6] = {32, 16, 32, 32, 16, 32};

    // The stencil formats first, then all of them.
    if (FindDepthFormat(*pDepthStencilFormat, TargetFormat, fmtDepthArray, fmtDepthArrayBits, 3, bpp))
    {
        return 1;
    }
    if (FindDepthFormat(*pDepthStencilFormat, TargetFormat, fmtDepthArray, fmtDepthArrayBits, 6, bpp))
    {
        return 1;
    }
    return 0;
}

// orig 0x60ce10 device.cpp:637
int CDevice::FindSurfaceFormat(D3DFORMAT* pSurfaceFormat, int bpp)
{
    static D3DFORMAT const fmtFullscreenArray[8] = {
        D3DFMT_R5G6B5, D3DFMT_X1R5G5B5, D3DFMT_A1R5G5B5, D3DFMT_X4R4G4B4,
        D3DFMT_A4R4G4B4, D3DFMT_R8G8B8, D3DFMT_A8R8G8B8, D3DFMT_X8R8G8B8,
    };
    static int const bits[8] = {16, 16, 16, 16, 16, 32, 32, 32};

    for (int iFmt = 0; iFmt < 8; iFmt++)
    {
        m_lastResult = m_pD3D->CheckDeviceType(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, fmtFullscreenArray[iFmt],
                                               fmtFullscreenArray[iFmt], FALSE);
        if (SUCCEEDED(m_lastResult) && bits[iFmt] == bpp)
        {
            *pSurfaceFormat = fmtFullscreenArray[iFmt];
            return 1;
        }
    }

    for (int iFmt = 0; iFmt < 8; iFmt++)
    {
        m_lastResult = m_pD3D->CheckDeviceType(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, fmtFullscreenArray[iFmt],
                                               fmtFullscreenArray[iFmt], FALSE);
        if (SUCCEEDED(m_lastResult))
        {
            *pSurfaceFormat = fmtFullscreenArray[iFmt];
            return 1;
        }
    }

    return 0;
}

// orig 0x60ceb0 device.cpp:688
int CDevice::FindTexFormat(D3DFORMAT& texFormat, D3DFORMAT TargetFormat, D3DFORMAT const* fmtTextureArray,
                           int const* bits, int numTextureFmts, int bpp, int usage)
{
    for (int iFmt = 0; iFmt < numTextureFmts; iFmt++)
    {
        m_lastResult = m_pD3D->CheckDeviceFormat(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, TargetFormat, usage,
                                                 D3DRTYPE_TEXTURE, fmtTextureArray[iFmt]);
        if (SUCCEEDED(m_lastResult) && bits[iFmt] == bpp)
        {
            texFormat = fmtTextureArray[iFmt];
            return 1;
        }
    }

    return 0;
}

// orig 0x60cf40 device.cpp:708
int CDevice::FindTextureFormat(D3DFORMAT TargetFormat, int bpp)
{
    static D3DFORMAT const tfRgbaFormats[6] = {
        D3DFMT_A1R5G5B5, D3DFMT_A4R4G4B4, D3DFMT_A8R8G8B8, D3DFMT_DXT5, D3DFMT_DXT3, D3DFMT_DXT1,
    };
    static int const tfRgbaBits[6] = {16, 16, 32, 0, 0, 0};

    static D3DFORMAT const tfRgbFormats[3] = {D3DFMT_R8G8B8, D3DFMT_R5G6B5, D3DFMT_DXT1};
    static int const tfRgbBits[3] = {24, 16, 0};

    static D3DFORMAT const tfRgbaFormatsRts[7] = {
        D3DFMT_R5G6B5, D3DFMT_A1R5G5B5, D3DFMT_A4R4G4B4, D3DFMT_A8R8G8B8,
        D3DFMT_X1R5G5B5, D3DFMT_X4R4G4B4, D3DFMT_X8R8G8B8,
    };
    static int const tfRgbaBitsRts[7] = {16, 16, 16, 32, 16, 16, 32};

    static D3DFORMAT const tfDepthFormats[6] = {
        D3DFMT_D24S8, D3DFMT_D24X4S4, D3DFMT_D24X8, D3DFMT_D32, D3DFMT_D16, D3DFMT_D15S1,
    };
    static int const tfDepthBits[6] = {32, 32, 32, 32, 16, 16};

    if (!FindTexFormat(m_texFormat[0][0], TargetFormat, tfRgbaFormats, tfRgbaBits, 6, bpp, 0))
    {
        if (!FindTexFormat(m_texFormat[0][0], TargetFormat, tfRgbaFormats, tfRgbaBits, 6, 0, 0))
        {
            if (!FindTexFormat(m_texFormat[0][0], TargetFormat, tfRgbaFormats, tfRgbaBits, 6, 16, 0))
            {
                if (!FindTexFormat(m_texFormat[0][0], TargetFormat, tfRgbaFormats, tfRgbaBits, 6, 32, 0))
                {
                    return 0;
                }
            }
        }
    }

    if (!FindTexFormat(m_texFormat[0][1], TargetFormat, tfRgbaFormats, tfRgbaBits, 6, 32, 0))
    {
        if (!FindTexFormat(m_texFormat[0][1], TargetFormat, tfRgbaFormats, tfRgbaBits, 6, 16, 0))
        {
            return 0;
        }
    }

    if (!FindTexFormat(m_texFormat[1][0], TargetFormat, tfRgbFormats, tfRgbBits, 3, 0, 0))
    {
        if (!FindTexFormat(m_texFormat[1][0], TargetFormat, tfRgbFormats, tfRgbBits, 3, 16, 0))
        {
            if (!FindTexFormat(m_texFormat[1][0], TargetFormat, tfRgbFormats, tfRgbBits, 3, 24, 0))
            {
                return 0;
            }
        }
    }

    if (!FindTexFormat(m_texFormat[1][1], TargetFormat, tfRgbFormats, tfRgbBits, 3, 24, 0))
    {
        if (!FindTexFormat(m_texFormat[1][1], TargetFormat, tfRgbFormats, tfRgbBits, 3, 16, 0))
        {
            return 0;
        }
    }

    if (!FindTexFormat(m_texFormatRt, TargetFormat, tfRgbaFormatsRts, tfRgbaBitsRts, 7, 32, D3DUSAGE_RENDERTARGET))
    {
        if (!FindTexFormat(m_texFormatRt, TargetFormat, tfRgbaFormatsRts, tfRgbaBitsRts, 7, 16, D3DUSAGE_RENDERTARGET))
        {
            return 0;
        }
    }

    if (!FindTexFormat(m_texFormatShadow, TargetFormat, tfRgbaFormatsRts, tfRgbaBitsRts, 7, 16, D3DUSAGE_RENDERTARGET))
    {
        return 0;
    }

    if (!FindTexFormat(m_texFormatDepth, TargetFormat, tfDepthFormats, tfDepthBits, 6, 32, D3DUSAGE_DEPTHSTENCIL))
    {
        if (!FindTexFormat(m_texFormatDepth, TargetFormat, tfDepthFormats, tfDepthBits, 6, 16, D3DUSAGE_DEPTHSTENCIL))
        {
            m_texFormatDepth = D3DFMT_UNKNOWN;
        }
    }

    return 1;
}

// orig 0x60d160 device.cpp:886
bool CDevice::IsMultiSamplingSupported(D3DFORMAT format, D3DMULTISAMPLE_TYPE multiSampleType, DWORD* qualityLevels)
{
    return SUCCEEDED(m_pD3D->CheckDeviceMultiSampleType(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, format, FALSE,
                                                        multiSampleType, qualityLevels));
}

// orig 0x60d190 device.cpp:893
int CDevice::FindMultisampleType(D3DFORMAT TargetFormat, D3DFORMAT depthStencilFormat, int multiSamplesNum,
                                 D3DPRESENT_PARAMETERS& d3dpp)
{
    DWORD qualityLevels = 0;

    if (FAILED(m_pD3D->CheckDeviceMultiSampleType(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, TargetFormat, FALSE,
                                                  (D3DMULTISAMPLE_TYPE)multiSamplesNum, &qualityLevels)))
    {
        return 0;
    }
    if (FAILED(m_pD3D->CheckDeviceMultiSampleType(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, depthStencilFormat, FALSE,
                                                  (D3DMULTISAMPLE_TYPE)multiSamplesNum, &qualityLevels)))
    {
        return 0;
    }

    d3dpp.MultiSampleType = (D3DMULTISAMPLE_TYPE)multiSamplesNum;
    d3dpp.MultiSampleQuality = qualityLevels - 1;
    return 1;
}

// orig 0x610820 device.cpp:912
CStr getD3dFmtStr(D3DFORMAT fmt)
{
    CStr str;
    switch (fmt)
    {
    default:
        str = "unknown";
        break;
    case D3DFMT_R8G8B8:
        str = "R8G8B8";
        break;
    case D3DFMT_A8R8G8B8:
        str = "A8R8G8B8";
        break;
    case D3DFMT_X8R8G8B8:
        str = "X8R8G8B8";
        break;
    case D3DFMT_R5G6B5:
        str = "R5G6B5";
        break;
    case D3DFMT_X1R5G5B5:
        str = "X1R5G5B5";
        break;
    case D3DFMT_A1R5G5B5:
        str = "A1R5G5B5";
        break;
    case D3DFMT_A4R4G4B4:
        str = "A4R4G4B4";
        break;
    case D3DFMT_R3G3B2:
        str = "R3G3B2";
        break;
    case D3DFMT_A8:
        str = "A8";
        break;
    case D3DFMT_A8R3G3B2:
        str = "A8R3G3B2";
        break;
    case D3DFMT_X4R4G4B4:
        str = "X4R4G4B4";
        break;

    case D3DFMT_A8P8:
        str = "A8P8";
        break;
    case D3DFMT_P8:
        str = "P8";
        break;

    case D3DFMT_L8:
        str = "L8";
        break;
    case D3DFMT_A8L8:
        str = "A8L8";
        break;
    case D3DFMT_A4L4:
        str = "A4L4";
        break;

    case D3DFMT_V8U8:
        str = "V8U8";
        break;
    case D3DFMT_L6V5U5:
        str = "L6V5U5";
        break;
    case D3DFMT_X8L8V8U8:
        str = "X8L8V8U8";
        break;
    case D3DFMT_Q8W8V8U8:
        str = "Q8W8V8U8";
        break;
    case D3DFMT_V16U16:
        str = "V16U16";
        break;

    case D3DFMT_UYVY:
        str = "UYVY";
        break;
    case D3DFMT_YUY2:
        str = "YUY2";
        break;
    case D3DFMT_DXT1:
        str = "DXT1";
        break;
    case D3DFMT_DXT2:
        str = "DXT2";
        break;
    case D3DFMT_DXT3:
        str = "DXT3";
        break;
    case D3DFMT_DXT4:
        str = "DXT4";
        break;
    case D3DFMT_DXT5:
        str = "DXT5";
        break;

    case D3DFMT_D16_LOCKABLE:
        str = "D16_LOCKABLE";
        break;
    case D3DFMT_D32:
        str = "D32";
        break;
    case D3DFMT_D15S1:
        str = "D15S1";
        break;
    case D3DFMT_D24S8:
        str = "D24S8";
        break;
    case D3DFMT_D16:
        str = "D16";
        break;
    case D3DFMT_D24X8:
        str = "D24X8";
        break;
    case D3DFMT_D24X4S4:
        str = "D24X4S4";
        break;

    case D3DFMT_VERTEXDATA:
        str = "VERTEXDATA";
        break;
    case D3DFMT_INDEX16:
        str = "INDEX16";
        break;
    case D3DFMT_INDEX32:
        str = "INDEX32";
        break;
    }

    return str;
}

namespace
{
    // The D3DX fill callbacks of the normalizing cube map and the specular power lookup texture
    // (created by CreateDevice / internalReset in device.cpp).

    // orig 0x60d210 device.cpp:1053
    void WINAPI FillNormalizingCubemap(D3DXVECTOR4* pOut, D3DXVECTOR3 const* pTexCoord, D3DXVECTOR3 const* pTexelSize,
                                       void* pData)
    {
        D3DXVECTOR3 texCoord = *pTexCoord;

        D3DXVec3Normalize(&texCoord, &texCoord);

        *pOut = D3DXVECTOR4(texCoord.x * 0.5f + 0.5f, texCoord.y * 0.5f + 0.5f, texCoord.z * 0.5f + 0.5f, 1.0f);
    }

    // orig 0x60d2b0 device.cpp:1067
    void WINAPI FillSpecularPowerTexture(D3DXVECTOR4* pOut, D3DXVECTOR2 const* pTexCoord,
                                         D3DXVECTOR2 const* pTexelSize, void* pData)
    {
        D3DXVECTOR2 texCoord = *pTexCoord;

        // The original computes the power in double precision (pow of the promoted floats).
        *pOut = D3DXVECTOR4((float)pow((double)texCoord.x, (double)(texCoord.y * 100.0f)), 0.0f, 0.0f, 0.0f);
    }
}

// orig 0x611120 device.cpp:1079
int CDevice::CreateDevice()
{
    m3d::EngineConfig& cfg = g_kernel->GetEngineCfg();

    // 1081
    m_lastResult = m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &m_desktopMode);
    if (FAILED(m_lastResult))
    {
        // 1083
        LogMsg("Could not get current desktop format!");
        // 1084
        return 0;
    }

    // 1088
    if (!FindSurfaceFormat(&m_surfaceFormat, cfg.m_r_bpp.GetI()))
    {
        // 1090
        LogMsg(CStr("FindSurfaceFormat failed for ") + CStr(cfg.m_r_bpp.GetI()));
        // 1091
        return 0;
    }

    // 1094
    if (!FindTextureFormat(m_surfaceFormat, cfg.m_r_compressedTextures.GetI()))
    {
        // 1096
        LogMsg(CStr("FindTextureFormat failed for ") + CStr(cfg.m_r_compressedTextures.GetI()));
        // 1097
        return 0;
    }

    // 1100
    if (!FindDepthFormat(m_surfaceFormat, &m_depthStencilFormat, cfg.m_r_depthBpp.GetI()))
    {
        // 1102
        LogMsg(CStr("FindDepthFormat failed for ") + CStr(cfg.m_r_depthBpp.GetI()));
        return 0;
    }

    // 1106
    if (!FindDepthFormat(m_texFormatRt, &m_depthStencilFormatRt, 32))
    {
        // 1108
        m_depthStencilFormatRt = D3DFMT_D16;
    }

    // 1112
    m_pd3dDevice = 0;
    // 1113
    memset(&m_d3dpp, 0, sizeof(m_d3dpp));
    // 1114
    m_d3dpp.BackBufferCount = 2;
    // 1115
    m_d3dpp.Windowed = !cfg.m_r_fullScreen.GetB();
    // 1116
    m_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
    // 1117
    m_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    // 1118
    m_d3dpp.EnableAutoDepthStencil = TRUE;

    // 1124
    if (!SwitchDisplayModes(cfg.m_mainWnd, cfg.m_r_width.GetI(), cfg.m_r_height.GetI(), cfg.m_r_fullScreen.GetB()))
    {
        // 1125
        return 0;
    }

    // 1165
    loadShadersMacros();

    // 1168
    InitVertexDeclarations();

    // 1171
    CreateFsRt();

    // 1174
    doneFullScreenQuadStuff();
    // 1175
    initFullScreenQuad();

    // 1178
    if (m_pSysFont)
    {
        m_pSysFont->Release();
        m_pSysFont = 0;
    }

    // 1199
    if (FAILED(D3DXCreateFont(m_pd3dDevice, 15, 0, FW_BOLD, 1, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
                              DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "System", &m_pSysFont)))
    {
        // 1202
        m_pSysFont = 0;
    }

    // 1207
    m_stencilLevel[0] = -1;
    m_stencilLevel[1] = -1;
    m_activeStencilTarget = 0;

    // 1209
    return 1;
}

// orig 0x61cd00 device.cpp:1215
int CDevice::Reset()
{
    static int wasReset = 0;

    // 1220
    if (m_pSysFont)
    {
        // 1221
        m_pSysFont->OnLostDevice();
    }

    // 1224
    if (!wasReset)
    {
        // 1227
        for (unsigned int i = 0; i < m_resetCallbacks.size(); ++i)
        {
            // 1228
            m_resetCallbacks[i]->OnBeforeDeviceReset();
        }

        // 1231
        for (std::map<unsigned int, IDirect3DSurface9*>::iterator it = m_rtsZSurfaces.begin();
             it != m_rtsZSurfaces.end(); ++it)
        {
            // 1232
            if (it->second)
            {
                it->second->Release();
                it->second = 0;
            }
        }
        // 1233
        m_rtsZSurfaces.clear();

        // 1236
        rstShadersPrepareFor();
        rstTexPrepareFor();
        rstIbPrepareFor();
        rstVbPrepareFor();
        rstQueryPrepareFor();

        // 1243
        wasReset = 1;
    }

    // 1247
    if (!FindMultisampleType(m_d3dpp.BackBufferFormat, m_d3dpp.AutoDepthStencilFormat,
                             g_kernel->GetEngineCfg().m_r_multiSamplesNum.GetI(), m_d3dpp))
    {
        // 1249
        m_d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
        // 1250
        m_d3dpp.MultiSampleQuality = 0;
    }

    // 1254
    HRESULT hr = m_pd3dDevice->Reset(&m_d3dpp);
    // 1255
    m_lastResult = hr;
    // 1257
    if (FAILED(hr))
    {
        LogMsg(CStr("d3d device reset failed: ") + getD3dErrorStr(hr));
        // 1258
        return 0;
    }
    // 1261
    LogMsg("Device reset ok");

    // 1264
    if (wasReset == 1)
    {
        // 1267
        if (m_pSysFont)
        {
            // 1268
            m_pSysFont->OnResetDevice();
        }

        // 1271
        rstShadersRestoreAfter();
        rstVbRestoreAfter();
        rstIbRestoreAfter();
        rstTexRestoreAfter();
        rstQueryRestoreAfter();

        // 1278
        ReleaseTexture(m_texFrameBufer);

        // 1281
        for (unsigned int i = 0; i < m_resetCallbacks.size(); ++i)
        {
            // 1282
            m_resetCallbacks[i]->OnAfterDeviceReset();
        }

        // 1285
        internalReset();

        // 1291
        wasReset = 0;
        InitMatrices();
    }

    // 1296
    if (g_kernel->GetEngineCfg().m_r_dxcursor.GetB())
    {
        // 1298
        SetupDXCursorForce(m_currentDXCursorInfo.m_texId, m_currentDXCursorInfo.m_xHotSpot,
                           m_currentDXCursorInfo.m_yHotSpot, 0);
    }

    // 1302
    m_stencilLevel[0] = -1;
    m_stencilLevel[1] = -1;

    // 1305: the ATI instancing hack
    if (m_d3dCaps.PixelShaderVersion < D3DPS_VERSION(3, 0))
    {
        if (m_pD3D->CheckDeviceFormat(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, D3DFMT_X8R8G8B8, 0, D3DRTYPE_SURFACE,
                                      (D3DFORMAT)MAKEFOURCC('I', 'N', 'S', 'T')) == S_OK)
        {
            m_pd3dDevice->SetRenderState(D3DRS_POINTSIZE, MAKEFOURCC('I', 'N', 'S', 'T'));
        }
    }

    // 1307
    return 1;
}

// orig 0x6115e0 device.cpp:1315
void CDevice::internalReset()
{
    // 1317
    rstCaches();

    for (int i = 0; i < 8; ++i)
    {
        // 1321
        memset(m_curTexStagesStates[i], -1, sizeof(m_curTexStagesStates[i]));
        // 1322
        memset(m_curTexSamplerStates[i], -1, sizeof(m_curTexSamplerStates[i]));
        // 1323
        m_curStreamFreq[i] = 1;
    }

    // 1331
    memset(m_curRenderState, -1, sizeof(m_curRenderState));
    float one = 1.0f;

    // 1332
    setRenderState(D3DRS_ZENABLE, TRUE);
    setRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
    setRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);
    // 1336
    setRenderState(D3DRS_ZWRITEENABLE, TRUE);
    setRenderState(D3DRS_ALPHATESTENABLE, FALSE);
    setRenderState(D3DRS_LASTPIXEL, TRUE);
    setRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
    setRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);
    setRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
    setRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
    setRenderState(D3DRS_ALPHAREF, 0);
    setRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
    setRenderState(D3DRS_DITHERENABLE, FALSE);
    setRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
    setRenderState(D3DRS_FOGENABLE, FALSE);
    setRenderState(D3DRS_SPECULARENABLE, FALSE);
    // 1350
    setRenderState(D3DRS_FOGCOLOR, 0);
    setRenderState(D3DRS_FOGTABLEMODE, D3DFOG_NONE);
    setRenderState(D3DRS_FOGSTART, 0);
    setRenderState(D3DRS_FOGEND, 0);
    setRenderState(D3DRS_FOGDENSITY, *reinterpret_cast<DWORD*>(&one));
    // 1356
    setRenderState(D3DRS_ANTIALIASEDLINEENABLE, FALSE);
    setRenderState(D3DRS_DEPTHBIAS, 0);
    setRenderState(D3DRS_RANGEFOGENABLE, FALSE);
    setRenderState(D3DRS_STENCILENABLE, FALSE);
    setRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);
    setRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);
    setRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP);
    setRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS);
    setRenderState(D3DRS_STENCILREF, 0);
    setRenderState(D3DRS_STENCILMASK, 0xffffffff);
    setRenderState(D3DRS_STENCILWRITEMASK, 0xffffffff);
    setRenderState(D3DRS_TEXTUREFACTOR, 0);
    setRenderState(D3DRS_WRAP0, 0);
    setRenderState(D3DRS_WRAP1, 0);
    setRenderState(D3DRS_WRAP2, 0);
    setRenderState(D3DRS_WRAP3, 0);
    setRenderState(D3DRS_WRAP4, 0);
    setRenderState(D3DRS_WRAP5, 0);
    setRenderState(D3DRS_WRAP6, 0);
    setRenderState(D3DRS_WRAP7, 0);
    setRenderState(D3DRS_CLIPPING, TRUE);
    setRenderState(D3DRS_LIGHTING, TRUE);
    setRenderState(D3DRS_AMBIENT, 0);
    setRenderState(D3DRS_FOGVERTEXMODE, D3DFOG_NONE);
    setRenderState(D3DRS_COLORVERTEX, TRUE);
    setRenderState(D3DRS_LOCALVIEWER, TRUE);
    setRenderState(D3DRS_NORMALIZENORMALS, TRUE);
    setRenderState(D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_COLOR1);
    // 1386
    setRenderState(D3DRS_SPECULARMATERIALSOURCE, D3DMCS_MATERIAL);
    // 1388
    setRenderState(D3DRS_AMBIENTMATERIALSOURCE, D3DMCS_MATERIAL);
    // 1390
    setRenderState(D3DRS_EMISSIVEMATERIALSOURCE, D3DMCS_MATERIAL);
    setRenderState(D3DRS_VERTEXBLEND, D3DVBF_DISABLE);
    setRenderState(D3DRS_CLIPPLANEENABLE, 0);
    // 1397
    setRenderState(D3DRS_POINTSIZE, *reinterpret_cast<DWORD*>(&one));
    setRenderState(D3DRS_POINTSIZE_MIN, *reinterpret_cast<DWORD*>(&one));
    setRenderState(D3DRS_POINTSPRITEENABLE, FALSE);
    setRenderState(D3DRS_POINTSCALEENABLE, FALSE);
    setRenderState(D3DRS_POINTSCALE_A, *reinterpret_cast<DWORD*>(&one));
    setRenderState(D3DRS_POINTSCALE_B, 0);
    setRenderState(D3DRS_POINTSCALE_C, 0);
    setRenderState(D3DRS_MULTISAMPLEANTIALIAS, FALSE);
    setRenderState(D3DRS_MULTISAMPLEMASK, 0xffffffff);
    setRenderState(D3DRS_PATCHEDGESTYLE, D3DPATCHEDGE_DISCRETE);
    // 1408
    setRenderState(D3DRS_DEBUGMONITORTOKEN, D3DDMT_ENABLE);
    setRenderState(D3DRS_POINTSIZE_MAX, *reinterpret_cast<DWORD*>(&one));
    setRenderState(D3DRS_INDEXEDVERTEXBLENDENABLE, FALSE);
    setRenderState(D3DRS_COLORWRITEENABLE, 0x0000000f);
    setRenderState(D3DRS_TWEENFACTOR, 0);
    setRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
    setRenderState(D3DRS_POSITIONDEGREE, D3DDEGREE_CUBIC);
    setRenderState(D3DRS_NORMALDEGREE, D3DDEGREE_LINEAR);
    setRenderState(D3DRS_SCISSORTESTENABLE, FALSE);
    setRenderState(D3DRS_SLOPESCALEDEPTHBIAS, 0);
    setRenderState(D3DRS_ANTIALIASEDLINEENABLE, FALSE);
    setRenderState(D3DRS_MINTESSELLATIONLEVEL, *reinterpret_cast<DWORD*>(&one));
    setRenderState(D3DRS_MAXTESSELLATIONLEVEL, *reinterpret_cast<DWORD*>(&one));
    setRenderState(D3DRS_ADAPTIVETESS_X, 0);
    setRenderState(D3DRS_ADAPTIVETESS_Y, 0);
    setRenderState(D3DRS_ADAPTIVETESS_Z, *reinterpret_cast<DWORD*>(&one));
    setRenderState(D3DRS_ADAPTIVETESS_W, 0);
    // 1426
    setRenderState(D3DRS_TWOSIDEDSTENCILMODE, FALSE);
    setRenderState(D3DRS_CCW_STENCILFAIL, D3DSTENCILOP_KEEP);
    setRenderState(D3DRS_CCW_STENCILZFAIL, D3DSTENCILOP_KEEP);
    setRenderState(D3DRS_CCW_STENCILPASS, D3DSTENCILOP_KEEP);
    setRenderState(D3DRS_CCW_STENCILFUNC, D3DCMP_ALWAYS);
    setRenderState(D3DRS_COLORWRITEENABLE1, 0x0000000f);
    setRenderState(D3DRS_COLORWRITEENABLE2, 0x0000000f);
    setRenderState(D3DRS_COLORWRITEENABLE3, 0x0000000f);
    setRenderState(D3DRS_BLENDFACTOR, 0xffffffff);
    setRenderState(D3DRS_SRGBWRITEENABLE, 0);
    setRenderState(D3DRS_DEPTHBIAS, 0);
    setRenderState(D3DRS_WRAP8, 0);
    setRenderState(D3DRS_WRAP9, 0);
    setRenderState(D3DRS_WRAP10, 0);
    setRenderState(D3DRS_WRAP11, 0);
    setRenderState(D3DRS_WRAP12, 0);
    setRenderState(D3DRS_WRAP13, 0);
    setRenderState(D3DRS_WRAP14, 0);
    setRenderState(D3DRS_WRAP15, 0);
    setRenderState(D3DRS_SEPARATEALPHABLENDENABLE, FALSE);
    setRenderState(D3DRS_SRCBLENDALPHA, D3DBLEND_ONE);
    setRenderState(D3DRS_DESTBLENDALPHA, D3DBLEND_ZERO);
    setRenderState(D3DRS_BLENDOPALPHA, D3DBLENDOP_ADD);

    // 1451
    for (int i = 0; i < 8; ++i)
    {
        // 1453
        setTextureStageState(i, D3DTSS_COLOROP, i == 0 ? D3DTOP_MODULATE : D3DTOP_DISABLE);
        setTextureStageState(i, D3DTSS_COLORARG1, D3DTA_TEXTURE);
        setTextureStageState(i, D3DTSS_COLORARG2, D3DTA_CURRENT);
        setTextureStageState(i, D3DTSS_ALPHAOP, i == 0 ? D3DTOP_SELECTARG1 : D3DTOP_DISABLE);
        setTextureStageState(i, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
        setTextureStageState(i, D3DTSS_ALPHAARG2, D3DTA_CURRENT);
        // 1460
        setTextureStageState(i, D3DTSS_BUMPENVMAT00, 0);
        setTextureStageState(i, D3DTSS_BUMPENVMAT01, 0);
        setTextureStageState(i, D3DTSS_BUMPENVMAT10, 0);
        setTextureStageState(i, D3DTSS_BUMPENVMAT11, 0);
        // 1465
        setTextureStageState(i, D3DTSS_TEXCOORDINDEX, i);
        setTextureStageState(i, D3DTSS_BUMPENVLSCALE, 0);
        setTextureStageState(i, D3DTSS_BUMPENVLOFFSET, 0);
        setTextureStageState(i, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
        setTextureStageState(i, D3DTSS_COLORARG0, D3DTA_DIFFUSE);
        setTextureStageState(i, D3DTSS_ALPHAARG0, D3DTA_DIFFUSE);
        setTextureStageState(i, D3DTSS_RESULTARG, D3DTA_CURRENT);
        setTextureStageState(i, D3DTSS_CONSTANT, 0);

        // 1474
        setTextureSamplerState(i, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
        setTextureSamplerState(i, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
        setTextureSamplerState(i, D3DSAMP_ADDRESSW, D3DTADDRESS_WRAP);
        setTextureSamplerState(i, D3DSAMP_BORDERCOLOR, 0);
        setTextureSamplerState(i, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
        setTextureSamplerState(i, D3DSAMP_MINFILTER, D3DTEXF_POINT);
        setTextureSamplerState(i, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
        setTextureSamplerState(i, D3DSAMP_MIPMAPLODBIAS, 0);
        setTextureSamplerState(i, D3DSAMP_MAXMIPLEVEL, 0);
        setTextureSamplerState(i, D3DSAMP_MAXANISOTROPY, 1);
        setTextureSamplerState(i, D3DSAMP_SRGBTEXTURE, 0);
        setTextureSamplerState(i, D3DSAMP_ELEMENTINDEX, 0);
        setTextureSamplerState(i, D3DSAMP_DMAPOFFSET, 256);
    }

    // 1490
    rstStacks();

    // 1495
    m3d::EngineConfig& cfg = g_kernel->GetEngineCfg();
    SetGamma(cfg.m_gammaGamma.GetF(), cfg.m_gammaBrightness.GetF(), cfg.m_gammaContrast.GetF());

    // 1498
    m_maxLights = std::min<int>(m_d3dCaps.MaxActiveLights, cfg.m_r_maxLights.GetI());
}

// orig 0x612d00 device.cpp:1504
void CDevice::ResetTextureStates()
{
    // 1505
    for (int i = 0; i < 8; ++i)
    {
        // 1507
        setTextureSamplerState(i, D3DSAMP_BORDERCOLOR, 0);
        setTextureSamplerState(i, D3DSAMP_MIPMAPLODBIAS, 0);
        setTextureSamplerState(i, D3DSAMP_MAXMIPLEVEL, 0);
        setTextureSamplerState(i, D3DSAMP_MAXANISOTROPY, 1);
        setTextureSamplerState(i, D3DSAMP_SRGBTEXTURE, 0);
        setTextureSamplerState(i, D3DSAMP_ELEMENTINDEX, 0);
        setTextureSamplerState(i, D3DSAMP_DMAPOFFSET, 256);
    }
}

// orig 0x61d2b0 device.cpp:1520
void CDevice::RegisterResetCallback(m3d::IDeviceResetCallback* callback)
{
    // 1525
    if (!(m_resetCallbacks.empty() ||
          m_resetCallbacks.end() == std::find(m_resetCallbacks.begin(), m_resetCallbacks.end(), callback)))
    {
        deviceAssertFailed("m_resetCallbacks.empty() || m_resetCallbacks.end() == std::find( m_resetCallbacks.begin(), "
                           "m_resetCallbacks.end(), callback )",
                           ".\\device.cpp", 1525);
    }

    // 1527
    m_resetCallbacks.push_back(callback);
}

// orig 0x61d100 device.cpp:1531
void CDevice::UnregisterResetCallback(m3d::IDeviceResetCallback* callback)
{
    // 1535
    std::vector<m3d::IDeviceResetCallback*>::iterator it =
        std::find(m_resetCallbacks.begin(), m_resetCallbacks.end(), callback);

    // 1537
    if (!(it != m_resetCallbacks.end()))
    {
        deviceAssertFailed("it != m_resetCallbacks.end()", ".\\device.cpp", 1537);
    }

    // 1539: the original erases unconditionally, end() included (its unchecked vector then dropped the last
    // element); with a bounds-checked vector that is an abort, so the erase is guarded (retruxx
    // adaptation, reached only after the assert above).
    if (it != m_resetCallbacks.end())
    {
        m_resetCallbacks.erase(it);
    }
}

// orig 0x61d170 device.cpp:1544
int CDevice::SwitchDisplayModes(HWND wnd, int w, int h, int fullscreen)
{
    // 1546
    for (std::map<unsigned int, IDirect3DSurface9*>::iterator it = m_rtsZSurfaces.begin(); it != m_rtsZSurfaces.end();
         ++it)
    {
        // 1547
        if (it->second)
        {
            it->second->Release();
            it->second = 0;
        }
    }
    // 1548
    m_rtsZSurfaces.clear();

    // 1551
    return SwitchDisplayModes0(wnd, w, h, fullscreen);
}

// orig 0x61aa70 device.cpp:1557
int CDevice::SwitchDisplayModes0(HWND wnd, int w, int h, int fullscreen)
{
    int result;
    m3d::EngineConfig& cfg = g_kernel->GetEngineCfg();

    // 1567
    IDirect3DSurface9* pBackBuffer = 0;
    Viewport port;
    port.m_x0 = 0;
    port.m_y0 = 0;
    port.m_zMin = 0.0f;
    port.m_zMax = 1.0f;

    // 1571
    if (fullscreen)
    {
        // 1573
        m_d3dpp.hDeviceWindow = wnd;
        m_d3dpp.BackBufferWidth = port.m_width = w;
        m_d3dpp.BackBufferHeight = port.m_height = h;
        m_d3dpp.BackBufferFormat = m_surfaceFormat;
        // 1574
        m_d3dpp.AutoDepthStencilFormat = m_depthStencilFormat;
        m_d3dpp.Windowed = FALSE;
    }
    else
    {
        // 1579
        m_d3dpp.hDeviceWindow = wnd;
        // 1585
        m_d3dpp.Windowed = TRUE;
        m_d3dpp.BackBufferWidth = port.m_width = w;
        m_d3dpp.BackBufferHeight = port.m_height = h;
        m_d3dpp.BackBufferFormat = m_desktopMode.Format;

        // 1585: the original matches the desktop format against depth formats, so the depth bpp is 16 here
        int bpp;
        switch (m_desktopMode.Format)
        {
        case D3DFMT_D32:
        case D3DFMT_D24S8:
        case D3DFMT_D24X4S4:
            // 1597
            bpp = 32;
            break;
        default:
            // 1591
            bpp = 16;
            break;
        }

        // 1602
        if (!FindDepthFormat(m_desktopMode.Format, &m_d3dpp.AutoDepthStencilFormat, bpp))
        {
            // 1604
            LogMsg(CStr("CDevice::FindDepthFormat( ") + CStr((int)m_desktopMode.Format) + CStr(", FORMAT, ") + CStr(bpp) +
                   CStr(") failed"));
            // 1605
            return 0;
        }
    }

    // 1611
    if (m_pd3dDevice)
    {
        // 1614
        result = Reset();
    }
    else
    {
        // 1619
        if (!FindMultisampleType(m_d3dpp.BackBufferFormat, m_d3dpp.AutoDepthStencilFormat,
                                 cfg.m_r_multiSamplesNum.GetI(), m_d3dpp))
        {
            // 1621
            m_d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
            // 1622
            m_d3dpp.MultiSampleQuality = 0;
        }

        // 1626
        m_lastResult = m_pD3D->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &m_d3dCaps);
        // 1627
        if (FAILED(m_lastResult))
        {
            // 1629
            LogMsg(CStr("Could not get device caps for the default device, hr = ") +
                   CStr::format_("%x", m_lastResult));
            // 1630
            return 0;
        }

        // 1634
        bool bHwTnL = (m_d3dCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT) != 0;
        // 1635
        bool bPureDevice = bHwTnL && (m_d3dCaps.DevCaps & D3DDEVCAPS_PUREDEVICE) != 0;

        // 1637
        unsigned long actualBehavior = cfg.m_r_behavior.GetI();
        // 1639
        switch (actualBehavior)
        {
        case D3DCREATE_HARDWARE_VERTEXPROCESSING:
        case D3DCREATE_MIXED_VERTEXPROCESSING:
            // 1647
            if (!bHwTnL)
            {
                actualBehavior = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
            }
            break;

        case D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_PUREDEVICE:
        case D3DCREATE_MIXED_VERTEXPROCESSING | D3DCREATE_PUREDEVICE:
            // 1654
            if (!bPureDevice)
            {
                // 1655
                actualBehavior = bHwTnL ? D3DCREATE_HARDWARE_VERTEXPROCESSING : D3DCREATE_SOFTWARE_VERTEXPROCESSING;
            }
            // 1660
            else if (!bHwTnL)
            {
                // 1661
                actualBehavior = D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_PUREDEVICE;
            }
            break;

        default:
            // 1642
            actualBehavior = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
            break;
        }

        // 1666
        LogMsg(CStr("Given device behavior: ") + CStr(cfg.m_r_behavior.GetI()));
        // 1667
        LogMsg(CStr("Confirmed device behavior: ") + CStr(actualBehavior));

        // 1675
        UINT AdapterToUse = D3DADAPTER_DEFAULT;
        D3DDEVTYPE DeviceType = D3DDEVTYPE_HAL;
        for (UINT Adapter = 0; Adapter < m_pD3D->GetAdapterCount(); Adapter++)
        {
            D3DADAPTER_IDENTIFIER9 Identifier;
            // 1678
            m_pD3D->GetAdapterIdentifier(Adapter, 0, &Identifier);
            // 1679
            if (strcmp(Identifier.Description, "NVIDIA NVPerfHUD") == 0)
            {
                // 1683
                AdapterToUse = Adapter;
                DeviceType = D3DDEVTYPE_REF;
                LogMsg("NVIDIA NVPerfHUD detected!!!");
                break;
            }
        }

        // 1699
        HRESULT hr = m_pD3D->CreateDevice(AdapterToUse, DeviceType, cfg.m_mainWnd, actualBehavior, &m_d3dpp,
                                          &m_pd3dDevice);
        // 1701
        m_lastResult = hr;
        if (FAILED(hr))
        {
            // 1703
            LogMsg(CStr("error creating device: ") + CStr(GetLastErrorStr()));
            // 1704
            return 0;
        }

        // 1709
        D3DADAPTER_IDENTIFIER9 id;
        m_pD3D->GetAdapterIdentifier(0, 0, &id);

        // 1711
        LogMsg(CStr("Description: ") + CStr(id.Description));
        LogMsg(CStr("Driver:      ") + CStr(id.Driver));
        LogMsg(CStr("VendorId:    0x") + CStr::format_("%x", id.VendorId));
        LogMsg(CStr("DeviceId:    0x") + CStr::format_("%x", id.DeviceId));
        LogMsg(CStr("SubSysId:    0x") + CStr::format_("%x", id.SubSysId));
        LogMsg(CStr("Revision:    ") + CStr(id.Revision));
        // 1718
        LogMsg("Driver version info:");
        LogMsg(CStr("  Product:    ") + CStr((int)HIWORD(id.DriverVersion.HighPart)));
        LogMsg(CStr("  Version:    ") + CStr((int)LOWORD(id.DriverVersion.HighPart)));
        LogMsg(CStr("  SubVersion: ") + CStr((int)HIWORD(id.DriverVersion.LowPart)));
        LogMsg(CStr("  Build:      ") + CStr((int)LOWORD(id.DriverVersion.LowPart)));
        // 1724
        LogMsg(CStr("WHQLLevel:    ") + CStr(id.WHQLLevel));

        // 1727
        m_lastResult = m_pd3dDevice->GetDeviceCaps(&m_d3dCaps);

        // 1729
        result = SetupAllFeaturesSupport(CStr("data\\DeviceCompatible.xml"));

        // 1732
        logCaps();

        // 1735
        m_isNV30 = id.VendorId == 0x10de &&
                   ((id.DeviceId >= 0x301 && id.DeviceId < 0x330) || (id.DeviceId > 0x334 && id.DeviceId < 0x400));

        // 1738
        internalReset();
    }

    // 1742
    if (result)
    {
        // 1749
        m_lastResult = m_pd3dDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer);
        // 1750
        pBackBuffer->GetDesc(&m_d3dsdBackBuffer);
        // 1751
        pBackBuffer->Release();

        // 1756
        IDirect3DSurface9* ps;
        m_lastResult = m_pd3dDevice->GetDepthStencilSurface(&ps);
        // 1757
        D3DSURFACE_DESC dd;
        ps->GetDesc(&dd);
        // 1758
        ps->Release();

        // 1776
        SetViewport(port);

        // 1779
        LogMsg("Creating or resetting the device");
        LogMsg(CStr("Back buffer format: ") + getD3dFmtStr(m_d3dsdBackBuffer.Format));
        LogMsg(CStr("Depth/stencil buffer format: ") + getD3dFmtStr(dd.Format));
        LogMsg(CStr("Stenciling: ") + CStr(m_haveStencil ? "on" : "off"));
        LogMsg(CStr("Rgba textures format: ") + getD3dFmtStr(m_texFormat[0][0]));
        LogMsg(CStr("Rgba no c. textures format: ") + getD3dFmtStr(m_texFormat[0][1]));
        LogMsg(CStr("Rgb textures format: ") + getD3dFmtStr(m_texFormat[1][0]));
        LogMsg(CStr("Rgb no c. textures format: ") + getD3dFmtStr(m_texFormat[1][1]));
        LogMsg(CStr("Rt textures format: ") + getD3dFmtStr(m_texFormatRt));
        LogMsg(CStr("Shadow textures format: ") + getD3dFmtStr(m_texFormatShadow));
        LogMsg(CStr("Depth textures format: ") + getD3dFmtStr(m_texFormatDepth));
        // 1790
        LogMsg(CStr("Rt depth format: ") + getD3dFmtStr(m_depthStencilFormatRt));
    }
    else
    {
        // 1794
        LogMsg(CStr("Reset or CreateDevice error: ") + CStr(GetLastErrorStr()));
    }

    // 1797
    return result;
}

// orig 0x612e30 device.cpp:1802
void CDevice::SetStreamFrequency(int stage, StreamDataType streamType, unsigned int frequency)
{
    switch (streamType)
    {
    case M3DSDT_DEFAULT_DATA:
        setStreamSourceFreq(stage, frequency);
        break;

    case M3DSDT_INDEXED_DATA:
        setStreamSourceFreq(stage, frequency | D3DSTREAMSOURCE_INDEXEDDATA);
        break;

    case M3DSDT_INSTANCED_DATA:
        setStreamSourceFreq(stage, frequency | D3DSTREAMSOURCE_INSTANCEDATA);
        break;
    }
}

// orig 0x612eb0 device.cpp:1821
// The original TextureState has TS_ITEX = 14 between TS_IPREV_ADD_DIFF and TS_TEX_ADD_PREV, so every
// value from TS_TEX_ADD_PREV on is one higher than in HTA's i_renderer.h. The cases are written by
// name (the executable passes HTA's values); the original TS_ITEX case was
//     setTextureStageState(stage, arg1, D3DTA_TEXTURE | D3DTA_COMPLEMENT);
//     setTextureStageState(stage, op, D3DTOP_SELECTARG1);
// and has no HTA value.
void CDevice::SetStageState(int stage, BlendMode mode, TextureState state)
{
    D3DTEXTURESTAGESTATETYPE arg1, arg2, op;

    if (mode == BM_COLOR)
    {
        arg1 = D3DTSS_COLORARG1;
        arg2 = D3DTSS_COLORARG2;
        op = D3DTSS_COLOROP;
    }
    else
    {
        arg1 = D3DTSS_ALPHAARG1;
        arg2 = D3DTSS_ALPHAARG2;
        op = D3DTSS_ALPHAOP;
    }

    switch (state)
    {
    case TS_NONE:
        setTextureStageState(stage, op, D3DTOP_DISABLE);
        break;

    case TS_TEXTURE:
        setTextureStageState(stage, arg1, D3DTA_TEXTURE);
        setTextureStageState(stage, op, D3DTOP_SELECTARG1);
        break;

    case TS_DIFFUSE:
        setTextureStageState(stage, arg1, D3DTA_DIFFUSE);
        setTextureStageState(stage, op, D3DTOP_SELECTARG1);
        break;

    case TS_TFACTOR:
        setTextureStageState(stage, arg1, D3DTA_TFACTOR);
        setTextureStageState(stage, op, D3DTOP_SELECTARG1);
        break;

    case TS_MODULATE:
        setTextureStageState(stage, arg1, D3DTA_TEXTURE);
        setTextureStageState(stage, op, D3DTOP_MODULATE);
        setTextureStageState(stage, arg2, D3DTA_DIFFUSE);
        break;

    case TS_MODULATE2X:
        setTextureStageState(stage, arg1, D3DTA_TEXTURE);
        setTextureStageState(stage, op, D3DTOP_MODULATE2X);
        setTextureStageState(stage, arg2, D3DTA_DIFFUSE);
        break;

    case TS_TEX_ADDSIGNED_DIFF:
        setTextureStageState(stage, arg1, D3DTA_TEXTURE);
        setTextureStageState(stage, op, D3DTOP_ADDSIGNED);
        setTextureStageState(stage, arg2, D3DTA_DIFFUSE);
        break;

    case TS_TEX_ADD_DIFF:
        setTextureStageState(stage, arg1, D3DTA_TEXTURE);
        setTextureStageState(stage, op, D3DTOP_ADD);
        setTextureStageState(stage, arg2, D3DTA_DIFFUSE);
        break;

    case TS_ITEX_ADDSIGNED_DIFF:
        setTextureStageState(stage, arg1, D3DTA_TEXTURE | D3DTA_COMPLEMENT);
        setTextureStageState(stage, op, D3DTOP_ADDSIGNED);
        setTextureStageState(stage, arg2, D3DTA_DIFFUSE);
        break;

    case TS_TEX_DP3_TFAC:
        setTextureStageState(stage, arg1, D3DTA_TEXTURE);
        setTextureStageState(stage, op, D3DTOP_DOTPRODUCT3);
        setTextureStageState(stage, arg2, D3DTA_TFACTOR);
        break;

    case TS_PREV:
        setTextureStageState(stage, arg1, D3DTA_CURRENT);
        setTextureStageState(stage, op, D3DTOP_SELECTARG1);
        break;

    case TS_IPREV:
        setTextureStageState(stage, arg1, D3DTA_CURRENT | D3DTA_COMPLEMENT);
        setTextureStageState(stage, op, D3DTOP_SELECTARG1);
        break;

    // original: case TS_ITEX (14) here, see above.

    case TS_IPREV_ADDSIGNED_DIFF:
        setTextureStageState(stage, arg1, D3DTA_DIFFUSE);
        setTextureStageState(stage, op, D3DTOP_ADDSIGNED);
        setTextureStageState(stage, arg2, D3DTA_CURRENT | D3DTA_COMPLEMENT);
        break;

    case TS_IPREV_ADD_DIFF:
        setTextureStageState(stage, arg1, D3DTA_DIFFUSE);
        setTextureStageState(stage, op, D3DTOP_ADD);
        setTextureStageState(stage, arg2, D3DTA_CURRENT | D3DTA_COMPLEMENT);
        break;

    case TS_TEX_MODULATE_PREV:
        setTextureStageState(stage, arg1, D3DTA_TEXTURE);
        setTextureStageState(stage, op, D3DTOP_MODULATE);
        setTextureStageState(stage, arg2, D3DTA_CURRENT);
        break;

    case TS_TEX_MODULATE2X_PREV:
        setTextureStageState(stage, arg1, D3DTA_TEXTURE);
        setTextureStageState(stage, op, D3DTOP_MODULATE2X);
        setTextureStageState(stage, arg2, D3DTA_CURRENT);
        break;

    case TS_TEX_ADDSIGNED_PREV:
        setTextureStageState(stage, arg1, D3DTA_TEXTURE);
        setTextureStageState(stage, op, D3DTOP_ADDSIGNED);
        setTextureStageState(stage, arg2, D3DTA_CURRENT);
        break;

    case TS_TEX_ADD_PREV:
        setTextureStageState(stage, arg1, D3DTA_TEXTURE);
        setTextureStageState(stage, op, D3DTOP_ADD);
        setTextureStageState(stage, arg2, D3DTA_CURRENT);
        break;

    case TS_TEX_ADDSMOOTH_PREV:
        setTextureStageState(stage, arg1, D3DTA_TEXTURE);
        setTextureStageState(stage, op, D3DTOP_ADDSMOOTH);
        setTextureStageState(stage, arg2, D3DTA_CURRENT);
        break;

    case TS_DIFF_MODULATE_PREV:
        setTextureStageState(stage, arg1, D3DTA_DIFFUSE);
        setTextureStageState(stage, op, D3DTOP_MODULATE);
        setTextureStageState(stage, arg2, D3DTA_CURRENT);
        break;

    case TS_DIFF_ADD_PREV:
        setTextureStageState(stage, arg1, D3DTA_DIFFUSE);
        setTextureStageState(stage, op, D3DTOP_ADD);
        setTextureStageState(stage, arg2, D3DTA_CURRENT);
        break;

    case TS_DIFF_ADDSMOOTH_PREV:
        setTextureStageState(stage, arg1, D3DTA_DIFFUSE);
        setTextureStageState(stage, op, D3DTOP_ADDSMOOTH);
        setTextureStageState(stage, arg2, D3DTA_CURRENT);
        break;

    case TS_DIFF_MODULATE_TFAC:
        setTextureStageState(stage, arg1, D3DTA_DIFFUSE);
        setTextureStageState(stage, op, D3DTOP_MODULATE);
        setTextureStageState(stage, arg2, D3DTA_TFACTOR);
        break;

    case TS_TEX_TFAC_LERP_PREV:
        setTextureStageState(stage, op, D3DTOP_BLENDFACTORALPHA);
        setTextureStageState(stage, arg1, D3DTA_CURRENT);
        setTextureStageState(stage, arg2, D3DTA_TEXTURE);
        break;

    case TS_PREV_MINUS_TEX:
        setTextureStageState(stage, arg1, D3DTA_CURRENT);
        setTextureStageState(stage, op, D3DTOP_SUBTRACT);
        setTextureStageState(stage, arg2, D3DTA_TEXTURE);
        break;

    case TS_PREV_ADDSMOOTH_TFAC:
        setTextureStageState(stage, arg1, D3DTA_TFACTOR);
        setTextureStageState(stage, op, D3DTOP_ADDSMOOTH);
        setTextureStageState(stage, arg2, D3DTA_CURRENT);
        break;

    case TS_TEX_MODULATE_TFAC:
        setTextureStageState(stage, arg1, D3DTA_TEXTURE);
        setTextureStageState(stage, op, D3DTOP_MODULATE);
        setTextureStageState(stage, arg2, D3DTA_TFACTOR);
        break;

    case TS_TEX_MODULATE2X_TFAC:
        setTextureStageState(stage, arg1, D3DTA_TEXTURE);
        setTextureStageState(stage, op, D3DTOP_MODULATE2X);
        setTextureStageState(stage, arg2, D3DTA_TFACTOR);
        break;
    }
}

// orig 0x613350 device.cpp:2030
void CDevice::DisableTextureStages(int stageToStartFrom)
{
    setTextureStageState(stageToStartFrom, D3DTSS_COLOROP, D3DTOP_DISABLE);
    setTextureStageState(stageToStartFrom, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
}

// orig 0x60d320 device.cpp:2061
void CDevice::PushAlphaTest()
{
    ++m_stackTopAlphaTest;
    m_stackAlphaTest[m_stackTopAlphaTest] = m_stackAlphaTest[m_stackTopAlphaTest - 1];
}

// orig 0x60d340 device.cpp:2061
void CDevice::PopAlphaTest()
{
    --m_stackTopAlphaTest;
    SetAlphaTest(m_stackAlphaTest[m_stackTopAlphaTest], false);
}

// orig 0x6133c0 device.cpp:2061
void CDevice::SetAlphaTest(int c, bool force)
{
    m_stackAlphaTest[m_stackTopAlphaTest] = c;
    if (c > 0)
    {
        if (m_d3dCaps.AlphaCmpCaps & D3DPCMPCAPS_GREATEREQUAL)
        {
            setRenderState(D3DRS_ALPHATESTENABLE, TRUE);
            setRenderState(D3DRS_ALPHAREF, c);
            setRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
        }
        else
        {
            setRenderState(D3DRS_ALPHATESTENABLE, FALSE);
        }
    }
    else
    {
        setRenderState(D3DRS_ALPHATESTENABLE, FALSE);
    }
}

// orig 0x60d360 device.cpp:2061
void CDevice::PushAlphaTest(int c)
{
    bool force = m_stackAlphaTest[m_stackTopAlphaTest] != c;
    PushAlphaTest();
    SetAlphaTest(c, force);
}

// HTA-only wrapper, no original body
void CDevice::SetAlphaTest(int c)
{
    SetAlphaTest(c, false);
}

// orig 0x60d3a0 device.cpp:2076
void CDevice::PushBlend()
{
    ++m_stackTopBlend;
    m_stackBlend[m_stackTopBlend] = m_stackBlend[m_stackTopBlend - 1];
}

// orig 0x60d3c0 device.cpp:2076
void CDevice::PopBlend()
{
    --m_stackTopBlend;
    SetBlend(m_stackBlend[m_stackTopBlend], false);
}

// orig 0x6134d0 device.cpp:2076
void CDevice::SetBlend(BlendMode c, bool force)
{
    m_stackBlend[m_stackTopBlend] = c;
    if (c == BM_NONE)
    {
        setRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
    }
    else
    {
        setRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
        setRenderState(D3DRS_SRCBLEND, srcDst[c][0]);
        setRenderState(D3DRS_DESTBLEND, srcDst[c][1]);
    }
}

// orig 0x60d3e0 device.cpp:2076
void CDevice::PushBlend(BlendMode c)
{
    bool force = m_stackBlend[m_stackTopBlend] != c;
    PushBlend();
    SetBlend(c, force);
}

// orig 0x60d420 device.cpp:2106
void CDevice::PushZbState()
{
    ++m_stackTopZbState;
    m_stackZbState[m_stackTopZbState] = m_stackZbState[m_stackTopZbState - 1];
}

// orig 0x60d440 device.cpp:2106
void CDevice::PopZbState()
{
    --m_stackTopZbState;
    SetZbState(m_stackZbState[m_stackTopZbState], false);
}

// orig 0x6135b0 device.cpp:2106
void CDevice::SetZbState(ZbState c, bool force)
{
    m_stackZbState[m_stackTopZbState] = c;
    switch (c)
    {
    case ZB_DISABLE:
        setRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
        break;
    case ZB_NOWRITE:
        setRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
        setRenderState(D3DRS_ZWRITEENABLE, FALSE);
        setRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
        break;
    case ZB_ENABLE:
        setRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
        setRenderState(D3DRS_ZWRITEENABLE, TRUE);
        setRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
        break;
    case ZB_WRITE_NOTEST:
        setRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
        setRenderState(D3DRS_ZWRITEENABLE, TRUE);
        setRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
        break;
    }
}

// orig 0x60d460 device.cpp:2106
void CDevice::PushZbState(ZbState c)
{
    bool force = m_stackZbState[m_stackTopZbState] != c;
    PushZbState();
    SetZbState(c, force);
}

// orig 0x60d4a0 device.cpp:2112
void CDevice::PushCull()
{
    ++m_stackTopCull;
    m_stackCull[m_stackTopCull] = m_stackCull[m_stackTopCull - 1];
}

// orig 0x60d4c0 device.cpp:2112
void CDevice::PopCull()
{
    --m_stackTopCull;
    SetCull(m_stackCull[m_stackTopCull], false);
}

// orig 0x613670 device.cpp:2112
void CDevice::SetCull(Cull c, bool force)
{
    m_stackCull[m_stackTopCull] = c;
    setRenderState(D3DRS_CULLMODE, m3dCullToD3dCull[c]);
}

// orig 0x60d4e0 device.cpp:2112
void CDevice::PushCull(Cull c)
{
    bool force = m_stackCull[m_stackTopCull] != c;
    PushCull();
    SetCull(c, force);
}

// orig 0x60d520 device.cpp:2118
void CDevice::PushZFunc()
{
    ++m_stackTopZFunc;
    m_stackZFunc[m_stackTopZFunc] = m_stackZFunc[m_stackTopZFunc - 1];
}

// orig 0x60d540 device.cpp:2118
void CDevice::PopZFunc()
{
    --m_stackTopZFunc;
    SetZFunc(m_stackZFunc[m_stackTopZFunc], false);
}

// orig 0x6136c0 device.cpp:2118
void CDevice::SetZFunc(CmpFunc c, bool force)
{
    m_stackZFunc[m_stackTopZFunc] = c;
    setRenderState(D3DRS_ZFUNC, m3dCmpToD3dCmp[c]);
}

// orig 0x60d560 device.cpp:2118
void CDevice::PushZFunc(CmpFunc c)
{
    bool force = m_stackZFunc[m_stackTopZFunc] != c;
    PushZFunc();
    SetZFunc(c, force);
}

// orig 0x60d5a0 device.cpp:2124
void CDevice::PushLighting()
{
    ++m_stackTopLighting;
    m_stackLighting[m_stackTopLighting] = m_stackLighting[m_stackTopLighting - 1];
}

// orig 0x60d5c0 device.cpp:2124
void CDevice::PopLighting()
{
    --m_stackTopLighting;
    SetLighting(m_stackLighting[m_stackTopLighting], false);
}

// orig 0x613710 device.cpp:2124
void CDevice::SetLighting(bool c, bool force)
{
    m_stackLighting[m_stackTopLighting] = c;
    setRenderState(D3DRS_LIGHTING, c);
}

// orig 0x60d5e0 device.cpp:2124
void CDevice::PushLighting(bool c)
{
    bool force = m_stackLighting[m_stackTopLighting] != c;
    PushLighting();
    SetLighting(c, force);
}

// orig 0x60d620 device.cpp:2130
void CDevice::PushAmbient()
{
    ++m_stackTopAmbient;
    m_stackAmbient[m_stackTopAmbient] = m_stackAmbient[m_stackTopAmbient - 1];
}

// orig 0x60d640 device.cpp:2130
void CDevice::PopAmbient()
{
    --m_stackTopAmbient;
    SetAmbient(m_stackAmbient[m_stackTopAmbient], false);
}

// orig 0x613760 device.cpp:2130
void CDevice::SetAmbient(unsigned int c, bool force)
{
    m_stackAmbient[m_stackTopAmbient] = c;
    setRenderState(D3DRS_AMBIENT, c);
}

// orig 0x60d660 device.cpp:2130
void CDevice::PushAmbient(unsigned int c)
{
    bool force = m_stackAmbient[m_stackTopAmbient] != c;
    PushAmbient();
    SetAmbient(c, force);
}

// orig 0x60d6a0 device.cpp:2136
void CDevice::PushFog()
{
    ++m_stackTopFog;
    m_stackFog[m_stackTopFog] = m_stackFog[m_stackTopFog - 1];
}

// orig 0x60d6c0 device.cpp:2136
void CDevice::PopFog()
{
    --m_stackTopFog;
    SetFog(m_stackFog[m_stackTopFog], false);
}

// orig 0x6137a0 device.cpp:2136
void CDevice::SetFog(bool c, bool force)
{
    m_stackFog[m_stackTopFog] = c;
    setRenderState(D3DRS_FOGENABLE, c);
}

// orig 0x60d6e0 device.cpp:2136
void CDevice::PushFog(bool c)
{
    bool force = m_stackFog[m_stackTopFog] != c;
    PushFog();
    SetFog(c, force);
}

// orig 0x60d720 device.cpp:2143
void CDevice::PushFogMode()
{
    ++m_stackTopFogMode;
    m_stackFogMode[m_stackTopFogMode] = m_stackFogMode[m_stackTopFogMode - 1];
}

// orig 0x60d740 device.cpp:2143
void CDevice::PopFogMode()
{
    --m_stackTopFogMode;
    SetFogMode(m_stackFogMode[m_stackTopFogMode], false);
}

// orig 0x6137e0 device.cpp:2143
void CDevice::SetFogMode(FogMode c, bool force)
{
    m_stackFogMode[m_stackTopFogMode] = c;
    setRenderState(D3DRS_FOGVERTEXMODE, m3dFogMToD3dFogM[c]);
}

// orig 0x60d760 device.cpp:2143
void CDevice::PushFogMode(FogMode c)
{
    bool force = m_stackFogMode[m_stackTopFogMode] != c;
    PushFogMode();
    SetFogMode(c, force);
}

// orig 0x60d7a0 device.cpp:2150
void CDevice::PushFogColor()
{
    ++m_stackTopFogColor;
    m_stackFogColor[m_stackTopFogColor] = m_stackFogColor[m_stackTopFogColor - 1];
}

// orig 0x60d7c0 device.cpp:2150
void CDevice::PopFogColor()
{
    --m_stackTopFogColor;
    SetFogColor(m_stackFogColor[m_stackTopFogColor], false);
}

// orig 0x613830 device.cpp:2150
void CDevice::SetFogColor(unsigned int c, bool force)
{
    m_stackFogColor[m_stackTopFogColor] = c;
    setRenderState(D3DRS_FOGCOLOR, c);
}

// orig 0x60d7e0 device.cpp:2150
void CDevice::PushFogColor(unsigned int c)
{
    bool force = m_stackFogColor[m_stackTopFogColor] != c;
    PushFogColor();
    SetFogColor(c, force);
}

// orig 0x60d820 device.cpp:2157
void CDevice::PushFogStart()
{
    ++m_stackTopFogStart;
    m_stackFogStart[m_stackTopFogStart] = m_stackFogStart[m_stackTopFogStart - 1];
}

// orig 0x60d840 device.cpp:2157
void CDevice::PopFogStart()
{
    --m_stackTopFogStart;
    SetFogStart(m_stackFogStart[m_stackTopFogStart], false);
}

// orig 0x613870 device.cpp:2157
void CDevice::SetFogStart(float c, bool force)
{
    m_stackFogStart[m_stackTopFogStart] = c;
    setRenderState(D3DRS_FOGSTART, *(DWORD*)&c);
}

// orig 0x60d860 device.cpp:2157
void CDevice::PushFogStart(float c)
{
    bool force = m_stackFogStart[m_stackTopFogStart] != c;
    PushFogStart();
    SetFogStart(c, force);
}

// orig 0x60d8b0 device.cpp:2163
void CDevice::PushFogEnd()
{
    ++m_stackTopFogEnd;
    m_stackFogEnd[m_stackTopFogEnd] = m_stackFogEnd[m_stackTopFogEnd - 1];
}

// orig 0x60d8d0 device.cpp:2163
void CDevice::PopFogEnd()
{
    --m_stackTopFogEnd;
    SetFogEnd(m_stackFogEnd[m_stackTopFogEnd], false);
}

// orig 0x6138c0 device.cpp:2163
void CDevice::SetFogEnd(float c, bool force)
{
    m_stackFogEnd[m_stackTopFogEnd] = c;
    setRenderState(D3DRS_FOGEND, *(DWORD*)&c);
}

// orig 0x60d8f0 device.cpp:2163
void CDevice::PushFogEnd(float c)
{
    bool force = m_stackFogEnd[m_stackTopFogEnd] != c;
    PushFogEnd();
    SetFogEnd(c, force);
}

// orig 0x60d940 device.cpp:2170
void CDevice::PushFillMode()
{
    ++m_stackTopFillMode;
    m_stackFillMode[m_stackTopFillMode] = m_stackFillMode[m_stackTopFillMode - 1];
}

// orig 0x60d960 device.cpp:2170
void CDevice::PopFillMode()
{
    --m_stackTopFillMode;
    SetFillMode(m_stackFillMode[m_stackTopFillMode], false);
}

// orig 0x613910 device.cpp:2170
void CDevice::SetFillMode(FillMode c, bool force)
{
    m_stackFillMode[m_stackTopFillMode] = c;
    setRenderState(D3DRS_FILLMODE, m3dFmToD3dFm[c]);
}

// orig 0x60d980 device.cpp:2170
void CDevice::PushFillMode(FillMode c)
{
    bool force = m_stackFillMode[m_stackTopFillMode] != c;
    PushFillMode();
    SetFillMode(c, force);
}

// orig 0x60d9c0 device.cpp:2177
void CDevice::PushZBias()
{
    ++m_stackTopZBias;
    m_stackZBias[m_stackTopZBias] = m_stackZBias[m_stackTopZBias - 1];
}

// orig 0x60d9e0 device.cpp:2177
void CDevice::PopZBias()
{
    --m_stackTopZBias;
    SetZBias(m_stackZBias[m_stackTopZBias], false);
}

// orig 0x613960 device.cpp:2177
void CDevice::SetZBias(float c, bool force)
{
    m_stackZBias[m_stackTopZBias] = c;
    setRenderState(D3DRS_DEPTHBIAS, *(DWORD*)&c);
}

// orig 0x60da00 device.cpp:2177
void CDevice::PushZBias(float c)
{
    bool force = m_stackZBias[m_stackTopZBias] != c;
    PushZBias();
    SetZBias(c, force);
}

// orig 0x60da50 device.cpp:2183
void CDevice::PushZBiasSlopeScale()
{
    ++m_stackTopZBiasSlopeScale;
    m_stackZBiasSlopeScale[m_stackTopZBiasSlopeScale] = m_stackZBiasSlopeScale[m_stackTopZBiasSlopeScale - 1];
}

// orig 0x60da70 device.cpp:2183
void CDevice::PopZBiasSlopeScale()
{
    --m_stackTopZBiasSlopeScale;
    SetZBiasSlopeScale(m_stackZBiasSlopeScale[m_stackTopZBiasSlopeScale], false);
}

// orig 0x6139b0 device.cpp:2183
void CDevice::SetZBiasSlopeScale(float c, bool force)
{
    m_stackZBiasSlopeScale[m_stackTopZBiasSlopeScale] = c;
    setRenderState(D3DRS_SLOPESCALEDEPTHBIAS, *(DWORD*)&c);
}

// orig 0x60da90 device.cpp:2183
void CDevice::PushZBiasSlopeScale(float c)
{
    bool force = m_stackZBiasSlopeScale[m_stackTopZBiasSlopeScale] != c;
    PushZBiasSlopeScale();
    SetZBiasSlopeScale(c, force);
}

// orig 0x60dae0 device.cpp:2189
void CDevice::PushShadeMode()
{
    ++m_stackTopShadeMode;
    m_stackShadeMode[m_stackTopShadeMode] = m_stackShadeMode[m_stackTopShadeMode - 1];
}

// orig 0x60db00 device.cpp:2189
void CDevice::PopShadeMode()
{
    --m_stackTopShadeMode;
    SetShadeMode(m_stackShadeMode[m_stackTopShadeMode], false);
}

// orig 0x613a00 device.cpp:2189
void CDevice::SetShadeMode(ShadeMode c, bool force)
{
    m_stackShadeMode[m_stackTopShadeMode] = c;
    setRenderState(D3DRS_SHADEMODE, m3dSmToD3dSm[c]);
}

// orig 0x60db20 device.cpp:2189
void CDevice::PushShadeMode(ShadeMode c)
{
    bool force = m_stackShadeMode[m_stackTopShadeMode] != c;
    PushShadeMode();
    SetShadeMode(c, force);
}

// orig 0x60db60 device.cpp:2196
void CDevice::PushPointSpriteEnable()
{
    ++m_stackTopPointSpriteEnable;
    m_stackPointSpriteEnable[m_stackTopPointSpriteEnable] = m_stackPointSpriteEnable[m_stackTopPointSpriteEnable - 1];
}

// orig 0x60db80 device.cpp:2196
void CDevice::PopPointSpriteEnable()
{
    --m_stackTopPointSpriteEnable;
    SetPointSpriteEnable(m_stackPointSpriteEnable[m_stackTopPointSpriteEnable], false);
}

// orig 0x613a50 device.cpp:2196
void CDevice::SetPointSpriteEnable(int c, bool force)
{
    m_stackPointSpriteEnable[m_stackTopPointSpriteEnable] = c;
    setRenderState(D3DRS_POINTSPRITEENABLE, c);
}

// orig 0x60dba0 device.cpp:2196
void CDevice::PushPointSpriteEnable(int c)
{
    bool force = m_stackPointSpriteEnable[m_stackTopPointSpriteEnable] != c;
    PushPointSpriteEnable();
    SetPointSpriteEnable(c, force);
}

// orig 0x60dbe0 device.cpp:2202
void CDevice::PushPointScaleEnable()
{
    ++m_stackTopPointScaleEnable;
    m_stackPointScaleEnable[m_stackTopPointScaleEnable] = m_stackPointScaleEnable[m_stackTopPointScaleEnable - 1];
}

// orig 0x60dc00 device.cpp:2202
void CDevice::PopPointScaleEnable()
{
    --m_stackTopPointScaleEnable;
    SetPointScaleEnable(m_stackPointScaleEnable[m_stackTopPointScaleEnable], false);
}

// orig 0x613a90 device.cpp:2202
void CDevice::SetPointScaleEnable(int c, bool force)
{
    m_stackPointScaleEnable[m_stackTopPointScaleEnable] = c;
    setRenderState(D3DRS_POINTSCALEENABLE, c);
}

// orig 0x60dc20 device.cpp:2202
void CDevice::PushPointScaleEnable(int c)
{
    bool force = m_stackPointScaleEnable[m_stackTopPointScaleEnable] != c;
    PushPointScaleEnable();
    SetPointScaleEnable(c, force);
}

// orig 0x60dc60 device.cpp:2208
void CDevice::PushPointSizeMin()
{
    ++m_stackTopPointSizeMin;
    m_stackPointSizeMin[m_stackTopPointSizeMin] = m_stackPointSizeMin[m_stackTopPointSizeMin - 1];
}

// orig 0x60dc80 device.cpp:2208
void CDevice::PopPointSizeMin()
{
    --m_stackTopPointSizeMin;
    SetPointSizeMin(m_stackPointSizeMin[m_stackTopPointSizeMin], false);
}

// orig 0x613ad0 device.cpp:2208
void CDevice::SetPointSizeMin(float c, bool force)
{
    m_stackPointSizeMin[m_stackTopPointSizeMin] = c;
    setRenderState(D3DRS_POINTSIZE_MIN, *(DWORD*)&c);
}

// orig 0x60dca0 device.cpp:2208
void CDevice::PushPointSizeMin(float c)
{
    bool force = m_stackPointSizeMin[m_stackTopPointSizeMin] != c;
    PushPointSizeMin();
    SetPointSizeMin(c, force);
}

// orig 0x60dcf0 device.cpp:2215
void CDevice::PushPointSizeMax()
{
    ++m_stackTopPointSizeMax;
    m_stackPointSizeMax[m_stackTopPointSizeMax] = m_stackPointSizeMax[m_stackTopPointSizeMax - 1];
}

// orig 0x60dd10 device.cpp:2215
void CDevice::PopPointSizeMax()
{
    --m_stackTopPointSizeMax;
    SetPointSizeMax(m_stackPointSizeMax[m_stackTopPointSizeMax], false);
}

// orig 0x613b20 device.cpp:2215
void CDevice::SetPointSizeMax(float c, bool force)
{
    m_stackPointSizeMax[m_stackTopPointSizeMax] = c;
    setRenderState(D3DRS_POINTSIZE_MAX, *(DWORD*)&c);
}

// orig 0x60dd30 device.cpp:2215
void CDevice::PushPointSizeMax(float c)
{
    bool force = m_stackPointSizeMax[m_stackTopPointSizeMax] != c;
    PushPointSizeMax();
    SetPointSizeMax(c, force);
}

// orig 0x60dd80 device.cpp:2221
void CDevice::PushPointSize()
{
    ++m_stackTopPointSize;
    m_stackPointSize[m_stackTopPointSize] = m_stackPointSize[m_stackTopPointSize - 1];
}

// orig 0x60dda0 device.cpp:2221
void CDevice::PopPointSize()
{
    --m_stackTopPointSize;
    SetPointSize(m_stackPointSize[m_stackTopPointSize], false);
}

// orig 0x613b70 device.cpp:2221
void CDevice::SetPointSize(float c, bool force)
{
    m_stackPointSize[m_stackTopPointSize] = c;
    setRenderState(D3DRS_POINTSIZE, *(DWORD*)&c);
}

// orig 0x60ddc0 device.cpp:2221
void CDevice::PushPointSize(float c)
{
    bool force = m_stackPointSize[m_stackTopPointSize] != c;
    PushPointSize();
    SetPointSize(c, force);
}

// orig 0x60de10 device.cpp:2227
void CDevice::PushPointScaleA()
{
    ++m_stackTopPointScaleA;
    m_stackPointScaleA[m_stackTopPointScaleA] = m_stackPointScaleA[m_stackTopPointScaleA - 1];
}

// orig 0x60de30 device.cpp:2227
void CDevice::PopPointScaleA()
{
    --m_stackTopPointScaleA;
    SetPointScaleA(m_stackPointScaleA[m_stackTopPointScaleA], false);
}

// orig 0x613bc0 device.cpp:2227
void CDevice::SetPointScaleA(float c, bool force)
{
    m_stackPointScaleA[m_stackTopPointScaleA] = c;
    setRenderState(D3DRS_POINTSCALE_A, *(DWORD*)&c);
}

// orig 0x60de50 device.cpp:2227
void CDevice::PushPointScaleA(float c)
{
    bool force = m_stackPointScaleA[m_stackTopPointScaleA] != c;
    PushPointScaleA();
    SetPointScaleA(c, force);
}

// orig 0x60dea0 device.cpp:2233
void CDevice::PushPointScaleB()
{
    ++m_stackTopPointScaleB;
    m_stackPointScaleB[m_stackTopPointScaleB] = m_stackPointScaleB[m_stackTopPointScaleB - 1];
}

// orig 0x60dec0 device.cpp:2233
void CDevice::PopPointScaleB()
{
    --m_stackTopPointScaleB;
    SetPointScaleB(m_stackPointScaleB[m_stackTopPointScaleB], false);
}

// orig 0x613c10 device.cpp:2233
void CDevice::SetPointScaleB(float c, bool force)
{
    m_stackPointScaleB[m_stackTopPointScaleB] = c;
    setRenderState(D3DRS_POINTSCALE_B, *(DWORD*)&c);
}

// orig 0x60dee0 device.cpp:2233
void CDevice::PushPointScaleB(float c)
{
    bool force = m_stackPointScaleB[m_stackTopPointScaleB] != c;
    PushPointScaleB();
    SetPointScaleB(c, force);
}

// orig 0x60df30 device.cpp:2239
void CDevice::PushPointScaleC()
{
    ++m_stackTopPointScaleC;
    m_stackPointScaleC[m_stackTopPointScaleC] = m_stackPointScaleC[m_stackTopPointScaleC - 1];
}

// orig 0x60df50 device.cpp:2239
void CDevice::PopPointScaleC()
{
    --m_stackTopPointScaleC;
    SetPointScaleC(m_stackPointScaleC[m_stackTopPointScaleC], false);
}

// orig 0x613c60 device.cpp:2239
void CDevice::SetPointScaleC(float c, bool force)
{
    m_stackPointScaleC[m_stackTopPointScaleC] = c;
    setRenderState(D3DRS_POINTSCALE_C, *(DWORD*)&c);
}

// orig 0x60df70 device.cpp:2239
void CDevice::PushPointScaleC(float c)
{
    bool force = m_stackPointScaleC[m_stackTopPointScaleC] != c;
    PushPointScaleC();
    SetPointScaleC(c, force);
}

// orig 0x60dfc0 device.cpp:2245
void CDevice::PushTFactor()
{
    ++m_stackTopTFactor;
    m_stackTFactor[m_stackTopTFactor] = m_stackTFactor[m_stackTopTFactor - 1];
}

// orig 0x60dfe0 device.cpp:2245
void CDevice::PopTFactor()
{
    --m_stackTopTFactor;
    SetTFactor(m_stackTFactor[m_stackTopTFactor], false);
}

// orig 0x613cb0 device.cpp:2245
void CDevice::SetTFactor(unsigned int c, bool force)
{
    m_stackTFactor[m_stackTopTFactor] = c;
    setRenderState(D3DRS_TEXTUREFACTOR, c);
}

// orig 0x60e000 device.cpp:2245
void CDevice::PushTFactor(unsigned int c)
{
    bool force = m_stackTFactor[m_stackTopTFactor] != c;
    PushTFactor();
    SetTFactor(c, force);
}

// orig 0x60e040 device.cpp:2251
void CDevice::PushLocalViewer()
{
    ++m_stackTopLocalViewer;
    m_stackLocalViewer[m_stackTopLocalViewer] = m_stackLocalViewer[m_stackTopLocalViewer - 1];
}

// orig 0x60e060 device.cpp:2251
void CDevice::PopLocalViewer()
{
    --m_stackTopLocalViewer;
    SetLocalViewer(m_stackLocalViewer[m_stackTopLocalViewer], false);
}

// orig 0x613cf0 device.cpp:2251
void CDevice::SetLocalViewer(bool c, bool force)
{
    m_stackLocalViewer[m_stackTopLocalViewer] = c;
    setRenderState(D3DRS_LOCALVIEWER, c);
}

// orig 0x60e080 device.cpp:2251
void CDevice::PushLocalViewer(bool c)
{
    bool force = m_stackLocalViewer[m_stackTopLocalViewer] != c;
    PushLocalViewer();
    SetLocalViewer(c, force);
}

// orig 0x60e0c0 device.cpp:2257
void CDevice::PushSpecularLighting()
{
    ++m_stackTopSpecularLighting;
    m_stackSpecularLighting[m_stackTopSpecularLighting] = m_stackSpecularLighting[m_stackTopSpecularLighting - 1];
}

// orig 0x60e0e0 device.cpp:2257
void CDevice::PopSpecularLighting()
{
    --m_stackTopSpecularLighting;
    SetSpecularLighting(m_stackSpecularLighting[m_stackTopSpecularLighting], false);
}

// orig 0x613d40 device.cpp:2257
void CDevice::SetSpecularLighting(bool c, bool force)
{
    m_stackSpecularLighting[m_stackTopSpecularLighting] = c;
    setRenderState(D3DRS_SPECULARENABLE, c);
}

// orig 0x60e100 device.cpp:2257
void CDevice::PushSpecularLighting(bool c)
{
    bool force = m_stackSpecularLighting[m_stackTopSpecularLighting] != c;
    PushSpecularLighting();
    SetSpecularLighting(c, force);
}

// orig 0x60e140 device.cpp:2263
void CDevice::PushColorWriteMask()
{
    ++m_stackTopColorWriteMask;
    m_stackColorWriteMask[m_stackTopColorWriteMask] = m_stackColorWriteMask[m_stackTopColorWriteMask - 1];
}

// orig 0x60e160 device.cpp:2263
void CDevice::PopColorWriteMask()
{
    --m_stackTopColorWriteMask;
    SetColorWriteMask(m_stackColorWriteMask[m_stackTopColorWriteMask], false);
}

// orig 0x613d80 device.cpp:2263
void CDevice::SetColorWriteMask(unsigned int c, bool force)
{
    m_stackColorWriteMask[m_stackTopColorWriteMask] = c;
    setRenderState(D3DRS_COLORWRITEENABLE, c);
}

// orig 0x60e180 device.cpp:2263
void CDevice::PushColorWriteMask(unsigned int c)
{
    bool force = m_stackColorWriteMask[m_stackTopColorWriteMask] != c;
    PushColorWriteMask();
    SetColorWriteMask(c, force);
}

// orig 0x60e1c0 device.cpp:2269
void CDevice::PushDithering()
{
    ++m_stackTopDithering;
    m_stackDithering[m_stackTopDithering] = m_stackDithering[m_stackTopDithering - 1];
}

// orig 0x60e1e0 device.cpp:2269
void CDevice::PopDithering()
{
    --m_stackTopDithering;
    SetDithering(m_stackDithering[m_stackTopDithering], false);
}

// orig 0x613dc0 device.cpp:2269
void CDevice::SetDithering(bool c, bool force)
{
    m_stackDithering[m_stackTopDithering] = c;
    setRenderState(D3DRS_DITHERENABLE, c);
}

// orig 0x60e200 device.cpp:2269
void CDevice::PushDithering(bool c)
{
    bool force = m_stackDithering[m_stackTopDithering] != c;
    PushDithering();
    SetDithering(c, force);
}

// orig 0x60e240 device.cpp:2275
void CDevice::PushNPatchLevel()
{
    ++m_stackTopNPatchLevel;
    m_stackNPatchLevel[m_stackTopNPatchLevel] = m_stackNPatchLevel[m_stackTopNPatchLevel - 1];
}

// orig 0x60e260 device.cpp:2275
void CDevice::PopNPatchLevel()
{
    --m_stackTopNPatchLevel;
    SetNPatchLevel(m_stackNPatchLevel[m_stackTopNPatchLevel], false);
}

// orig 0x60e280 device.cpp:2275
void CDevice::SetNPatchLevel(float c, bool force)
{
    m_stackNPatchLevel[m_stackTopNPatchLevel] = c;
    m_pd3dDevice->SetNPatchMode(c);
}

// orig 0x60e2b0 device.cpp:2275
void CDevice::PushNPatchLevel(float c)
{
    bool force = m_stackNPatchLevel[m_stackTopNPatchLevel] != c;
    PushNPatchLevel();
    SetNPatchLevel(c, force);
}

// orig 0x60e300 device.cpp:2285
void CDevice::PushStencilState()
{
    ++m_stackTopStencilState;
    m_stackStencilState[m_stackTopStencilState] = m_stackStencilState[m_stackTopStencilState - 1];
}

// orig 0x60e320 device.cpp:2285
void CDevice::PopStencilState()
{
    --m_stackTopStencilState;
    SetStencilState(m_stackStencilState[m_stackTopStencilState], false);
}

// orig 0x613e00 device.cpp:2285
void CDevice::SetStencilState(bool c, bool force)
{
    m_stackStencilState[m_stackTopStencilState] = c;
    setRenderState(D3DRS_STENCILENABLE, c);
    m_stencilLevel[m_activeStencilTarget] = -1;
}

// orig 0x60e340 device.cpp:2285
void CDevice::PushStencilState(bool c)
{
    bool force = m_stackStencilState[m_stackTopStencilState] != c;
    PushStencilState();
    SetStencilState(c, force);
}

// orig 0x60e380 device.cpp:2291
void CDevice::PushStencilMask()
{
    ++m_stackTopStencilMask;
    m_stackStencilMask[m_stackTopStencilMask] = m_stackStencilMask[m_stackTopStencilMask - 1];
}

// orig 0x60e3a0 device.cpp:2291
void CDevice::PopStencilMask()
{
    --m_stackTopStencilMask;
    SetStencilMask(m_stackStencilMask[m_stackTopStencilMask], false);
}

// orig 0x613e70 device.cpp:2291
void CDevice::SetStencilMask(unsigned int c, bool force)
{
    m_stackStencilMask[m_stackTopStencilMask] = c;
    setRenderState(D3DRS_STENCILMASK, c);
}

// orig 0x60e3c0 device.cpp:2291
void CDevice::PushStencilMask(unsigned int c)
{
    bool force = m_stackStencilMask[m_stackTopStencilMask] != c;
    PushStencilMask();
    SetStencilMask(c, force);
}

// orig 0x60e400 device.cpp:2297
void CDevice::PushStencilRef()
{
    ++m_stackTopStencilRef;
    m_stackStencilRef[m_stackTopStencilRef] = m_stackStencilRef[m_stackTopStencilRef - 1];
}

// orig 0x60e420 device.cpp:2297
void CDevice::PopStencilRef()
{
    --m_stackTopStencilRef;
    SetStencilRef(m_stackStencilRef[m_stackTopStencilRef], false);
}

// orig 0x613eb0 device.cpp:2297
void CDevice::SetStencilRef(unsigned int c, bool force)
{
    m_stackStencilRef[m_stackTopStencilRef] = c;
    setRenderState(D3DRS_STENCILREF, c);
}

// orig 0x60e440 device.cpp:2297
void CDevice::PushStencilRef(unsigned int c)
{
    bool force = m_stackStencilRef[m_stackTopStencilRef] != c;
    PushStencilRef();
    SetStencilRef(c, force);
}

// orig 0x60e480 device.cpp:2303
void CDevice::PushStencilWriteMask()
{
    ++m_stackTopStencilWriteMask;
    m_stackStencilWriteMask[m_stackTopStencilWriteMask] = m_stackStencilWriteMask[m_stackTopStencilWriteMask - 1];
}

// orig 0x60e4a0 device.cpp:2303
void CDevice::PopStencilWriteMask()
{
    --m_stackTopStencilWriteMask;
    SetStencilWriteMask(m_stackStencilWriteMask[m_stackTopStencilWriteMask], false);
}

// orig 0x613ef0 device.cpp:2303
void CDevice::SetStencilWriteMask(unsigned int c, bool force)
{
    m_stackStencilWriteMask[m_stackTopStencilWriteMask] = c;
    setRenderState(D3DRS_STENCILWRITEMASK, c);
}

// orig 0x60e4c0 device.cpp:2303
void CDevice::PushStencilWriteMask(unsigned int c)
{
    bool force = m_stackStencilWriteMask[m_stackTopStencilWriteMask] != c;
    PushStencilWriteMask();
    SetStencilWriteMask(c, force);
}

// orig 0x60e500 device.cpp:2309
void CDevice::PushStencilFunc()
{
    ++m_stackTopStencilFunc;
    m_stackStencilFunc[m_stackTopStencilFunc] = m_stackStencilFunc[m_stackTopStencilFunc - 1];
}

// orig 0x60e520 device.cpp:2309
void CDevice::PopStencilFunc()
{
    --m_stackTopStencilFunc;
    SetStencilFunc(m_stackStencilFunc[m_stackTopStencilFunc], false);
}

// orig 0x613f30 device.cpp:2309
void CDevice::SetStencilFunc(CmpFunc c, bool force)
{
    m_stackStencilFunc[m_stackTopStencilFunc] = c;
    setRenderState(D3DRS_STENCILFUNC, m3dCmpToD3dCmp[c]);
}

// orig 0x60e540 device.cpp:2309
void CDevice::PushStencilFunc(CmpFunc c)
{
    bool force = m_stackStencilFunc[m_stackTopStencilFunc] != c;
    PushStencilFunc();
    SetStencilFunc(c, force);
}

// orig 0x60e580 device.cpp:2315
void CDevice::PushStencilFail()
{
    ++m_stackTopStencilFail;
    m_stackStencilFail[m_stackTopStencilFail] = m_stackStencilFail[m_stackTopStencilFail - 1];
}

// orig 0x60e5a0 device.cpp:2315
void CDevice::PopStencilFail()
{
    --m_stackTopStencilFail;
    SetStencilFail(m_stackStencilFail[m_stackTopStencilFail], false);
}

// orig 0x613f80 device.cpp:2315
void CDevice::SetStencilFail(StencilOp c, bool force)
{
    m_stackStencilFail[m_stackTopStencilFail] = c;
    setRenderState(D3DRS_STENCILFAIL, m3dStencilOpD3dStencilOp[c]);
}

// orig 0x60e5c0 device.cpp:2315
void CDevice::PushStencilFail(StencilOp c)
{
    bool force = m_stackStencilFail[m_stackTopStencilFail] != c;
    PushStencilFail();
    SetStencilFail(c, force);
}

// orig 0x60e600 device.cpp:2321
void CDevice::PushStencilZFail()
{
    ++m_stackTopStencilZFail;
    m_stackStencilZFail[m_stackTopStencilZFail] = m_stackStencilZFail[m_stackTopStencilZFail - 1];
}

// orig 0x60e620 device.cpp:2321
void CDevice::PopStencilZFail()
{
    --m_stackTopStencilZFail;
    SetStencilZFail(m_stackStencilZFail[m_stackTopStencilZFail], false);
}

// orig 0x613fd0 device.cpp:2321
void CDevice::SetStencilZFail(StencilOp c, bool force)
{
    m_stackStencilZFail[m_stackTopStencilZFail] = c;
    setRenderState(D3DRS_STENCILZFAIL, m3dStencilOpD3dStencilOp[c]);
}

// orig 0x60e640 device.cpp:2321
void CDevice::PushStencilZFail(StencilOp c)
{
    bool force = m_stackStencilZFail[m_stackTopStencilZFail] != c;
    PushStencilZFail();
    SetStencilZFail(c, force);
}

// orig 0x60e680 device.cpp:2327
void CDevice::PushStencilPass()
{
    ++m_stackTopStencilPass;
    m_stackStencilPass[m_stackTopStencilPass] = m_stackStencilPass[m_stackTopStencilPass - 1];
}

// orig 0x60e6a0 device.cpp:2327
void CDevice::PopStencilPass()
{
    --m_stackTopStencilPass;
    SetStencilPass(m_stackStencilPass[m_stackTopStencilPass], false);
}

// orig 0x614020 device.cpp:2327
void CDevice::SetStencilPass(StencilOp c, bool force)
{
    m_stackStencilPass[m_stackTopStencilPass] = c;
    setRenderState(D3DRS_STENCILPASS, m3dStencilOpD3dStencilOp[c]);
}

// orig 0x60e6c0 device.cpp:2327
void CDevice::PushStencilPass(StencilOp c)
{
    bool force = m_stackStencilPass[m_stackTopStencilPass] != c;
    PushStencilPass();
    SetStencilPass(c, force);
}

// orig 0x60e700 device.cpp:2339
void CDevice::PushStencil2SidedEnable()
{
    ++m_stackTopStencil2SidedEnable;
    m_stackStencil2SidedEnable[m_stackTopStencil2SidedEnable] = m_stackStencil2SidedEnable[m_stackTopStencil2SidedEnable - 1];
}

// orig 0x60e720 device.cpp:2339
void CDevice::PopStencil2SidedEnable()
{
    --m_stackTopStencil2SidedEnable;
    SetStencil2SidedEnable(m_stackStencil2SidedEnable[m_stackTopStencil2SidedEnable], false);
}

// orig 0x614070 device.cpp:2339
void CDevice::SetStencil2SidedEnable(bool c, bool force)
{
    m_stackStencil2SidedEnable[m_stackTopStencil2SidedEnable] = c;
    setRenderState(D3DRS_TWOSIDEDSTENCILMODE, c);
    m_stencilLevel[m_activeStencilTarget] = -1;
}

// orig 0x60e740 device.cpp:2339
void CDevice::PushStencil2SidedEnable(bool c)
{
    bool force = m_stackStencil2SidedEnable[m_stackTopStencil2SidedEnable] != c;
    PushStencil2SidedEnable();
    SetStencil2SidedEnable(c, force);
}

// orig 0x60e780 device.cpp:2345
void CDevice::PushStencilCcwFunc()
{
    ++m_stackTopStencilCcwFunc;
    m_stackStencilCcwFunc[m_stackTopStencilCcwFunc] = m_stackStencilCcwFunc[m_stackTopStencilCcwFunc - 1];
}

// orig 0x60e7a0 device.cpp:2345
void CDevice::PopStencilCcwFunc()
{
    --m_stackTopStencilCcwFunc;
    SetStencilCcwFunc(m_stackStencilCcwFunc[m_stackTopStencilCcwFunc], false);
}

// orig 0x6140e0 device.cpp:2345
void CDevice::SetStencilCcwFunc(CmpFunc c, bool force)
{
    m_stackStencilCcwFunc[m_stackTopStencilCcwFunc] = c;
    setRenderState(D3DRS_CCW_STENCILFUNC, m3dCmpToD3dCmp[c]);
}

// orig 0x60e7c0 device.cpp:2345
void CDevice::PushStencilCcwFunc(CmpFunc c)
{
    bool force = m_stackStencilCcwFunc[m_stackTopStencilCcwFunc] != c;
    PushStencilCcwFunc();
    SetStencilCcwFunc(c, force);
}

// orig 0x60e800 device.cpp:2351
void CDevice::PushStencilCcwFail()
{
    ++m_stackTopStencilCcwFail;
    m_stackStencilCcwFail[m_stackTopStencilCcwFail] = m_stackStencilCcwFail[m_stackTopStencilCcwFail - 1];
}

// orig 0x60e820 device.cpp:2351
void CDevice::PopStencilCcwFail()
{
    --m_stackTopStencilCcwFail;
    SetStencilCcwFail(m_stackStencilCcwFail[m_stackTopStencilCcwFail], false);
}

// orig 0x614130 device.cpp:2351
void CDevice::SetStencilCcwFail(StencilOp c, bool force)
{
    m_stackStencilCcwFail[m_stackTopStencilCcwFail] = c;
    setRenderState(D3DRS_CCW_STENCILFAIL, m3dStencilOpD3dStencilOp[c]);
}

// orig 0x60e840 device.cpp:2351
void CDevice::PushStencilCcwFail(StencilOp c)
{
    bool force = m_stackStencilCcwFail[m_stackTopStencilCcwFail] != c;
    PushStencilCcwFail();
    SetStencilCcwFail(c, force);
}

// orig 0x60e880 device.cpp:2357
void CDevice::PushStencilCcwZFail()
{
    ++m_stackTopStencilCcwZFail;
    m_stackStencilCcwZFail[m_stackTopStencilCcwZFail] = m_stackStencilCcwZFail[m_stackTopStencilCcwZFail - 1];
}

// orig 0x60e8a0 device.cpp:2357
void CDevice::PopStencilCcwZFail()
{
    --m_stackTopStencilCcwZFail;
    SetStencilCcwZFail(m_stackStencilCcwZFail[m_stackTopStencilCcwZFail], false);
}

// orig 0x614180 device.cpp:2357
void CDevice::SetStencilCcwZFail(StencilOp c, bool force)
{
    m_stackStencilCcwZFail[m_stackTopStencilCcwZFail] = c;
    setRenderState(D3DRS_CCW_STENCILZFAIL, m3dStencilOpD3dStencilOp[c]);
}

// orig 0x60e8c0 device.cpp:2357
void CDevice::PushStencilCcwZFail(StencilOp c)
{
    bool force = m_stackStencilCcwZFail[m_stackTopStencilCcwZFail] != c;
    PushStencilCcwZFail();
    SetStencilCcwZFail(c, force);
}

// orig 0x60e900 device.cpp:2363
void CDevice::PushStencilCcwPass()
{
    ++m_stackTopStencilCcwPass;
    m_stackStencilCcwPass[m_stackTopStencilCcwPass] = m_stackStencilCcwPass[m_stackTopStencilCcwPass - 1];
}

// orig 0x60e920 device.cpp:2363
void CDevice::PopStencilCcwPass()
{
    --m_stackTopStencilCcwPass;
    SetStencilCcwPass(m_stackStencilCcwPass[m_stackTopStencilCcwPass], false);
}

// orig 0x6141d0 device.cpp:2363
void CDevice::SetStencilCcwPass(StencilOp c, bool force)
{
    m_stackStencilCcwPass[m_stackTopStencilCcwPass] = c;
    setRenderState(D3DRS_CCW_STENCILPASS, m3dStencilOpD3dStencilOp[c]);
}

// orig 0x60e940 device.cpp:2363
void CDevice::PushStencilCcwPass(StencilOp c)
{
    bool force = m_stackStencilCcwPass[m_stackTopStencilCcwPass] != c;
    PushStencilCcwPass();
    SetStencilCcwPass(c, force);
}

// orig 0x60e980 device.cpp:2370
void CDevice::PushMultiSample()
{
    ++m_stackTopMultiSample;
    m_stackMultiSample[m_stackTopMultiSample] = m_stackMultiSample[m_stackTopMultiSample - 1];
}

// orig 0x60e9a0 device.cpp:2370
void CDevice::PopMultiSample()
{
    --m_stackTopMultiSample;
    SetMultiSample(m_stackMultiSample[m_stackTopMultiSample], false);
}

// orig 0x614220 device.cpp:2370
void CDevice::SetMultiSample(bool c, bool force)
{
    m_stackMultiSample[m_stackTopMultiSample] = c;
    if (m_d3dpp.MultiSampleType != D3DMULTISAMPLE_NONE)
    {
        setRenderState(D3DRS_MULTISAMPLEANTIALIAS, c);
    }
}

// orig 0x60e9c0 device.cpp:2370
void CDevice::PushMultiSample(bool c)
{
    bool force = m_stackMultiSample[m_stackTopMultiSample] != c;
    PushMultiSample();
    SetMultiSample(c, force);
}

// orig 0x60ea00 device.cpp:2376
void CDevice::PushMultiSampleMask()
{
    ++m_stackTopMultiSampleMask;
    m_stackMultiSampleMask[m_stackTopMultiSampleMask] = m_stackMultiSampleMask[m_stackTopMultiSampleMask - 1];
}

// orig 0x60ea20 device.cpp:2376
void CDevice::PopMultiSampleMask()
{
    --m_stackTopMultiSampleMask;
    SetMultiSampleMask(m_stackMultiSampleMask[m_stackTopMultiSampleMask], false);
}

// orig 0x614270 device.cpp:2376
void CDevice::SetMultiSampleMask(unsigned int c, bool force)
{
    m_stackMultiSampleMask[m_stackTopMultiSampleMask] = c;
    setRenderState(D3DRS_MULTISAMPLEMASK, c);
}

// orig 0x60ea40 device.cpp:2376
void CDevice::PushMultiSampleMask(unsigned int c)
{
    bool force = m_stackMultiSampleMask[m_stackTopMultiSampleMask] != c;
    PushMultiSampleMask();
    SetMultiSampleMask(c, force);
}

// orig 0x60ea80 device.cpp:2383
void CDevice::rstStacks()
{
    m_stackTopAlphaTest = 0;
    m_stackAlphaTest[0] = 0;
    SetAlphaTest(0, false);
    m_stackTopBlend = 0;
    m_stackBlend[0] = BM_NONE;
    SetBlend(BM_NONE, false);
    m_stackTopZbState = 0;
    m_stackZbState[0] = ZB_ENABLE;
    SetZbState(ZB_ENABLE, false);
    m_stackTopCull = 0;
    m_stackCull[0] = M3DCULL_NONE;
    SetCull(M3DCULL_NONE, false);
    m_stackTopZFunc = 0;
    m_stackZFunc[0] = M3DCMP_LESSEQUAL;
    SetZFunc(M3DCMP_LESSEQUAL, false);
    m_stackTopLighting = 0;
    m_stackLighting[0] = false;
    SetLighting(false, false);
    m_stackTopAmbient = 0;
    m_stackAmbient[0] = 0;
    SetAmbient(0, false);
    m_stackTopFog = 0;
    m_stackFog[0] = false;
    SetFog(false, false);
    m_stackTopFogColor = 0;
    m_stackFogColor[0] = 0;
    SetFogColor(0, false);
    m_stackTopFogMode = 0;
    m_stackFogMode[0] = M3DFOG_NONE;
    SetFogMode(M3DFOG_NONE, false);
    m_stackTopFogStart = 0;
    m_stackFogStart[0] = 0.0f;
    SetFogStart(0.0f, false);
    m_stackTopFogEnd = 0;
    m_stackFogEnd[0] = 0.0f;
    SetFogEnd(0.0f, false);
    m_stackTopFillMode = 0;
    m_stackFillMode[0] = M3DFILL_SOLID;
    SetFillMode(M3DFILL_SOLID, false);
    m_stackTopZBias = 0;
    m_stackZBias[0] = 0.0f;
    SetZBias(0.0f, false);
    m_stackTopZBiasSlopeScale = 0;
    m_stackZBiasSlopeScale[0] = 0.0f;
    SetZBiasSlopeScale(0.0f, false);
    m_stackTopShadeMode = 0;
    m_stackShadeMode[0] = M3DSHADE_GOURAUD;
    SetShadeMode(M3DSHADE_GOURAUD, false);
    m_stackTopPointSpriteEnable = 0;
    m_stackPointSpriteEnable[0] = 0;
    SetPointSpriteEnable(0, false);
    m_stackTopPointScaleEnable = 0;
    m_stackPointScaleEnable[0] = 0;
    SetPointScaleEnable(0, false);
    m_stackTopPointSizeMin = 0;
    m_stackPointSizeMin[0] = 1.0f;
    SetPointSizeMin(1.0f, false);
    m_stackTopPointSizeMax = 0;
    m_stackPointSizeMax[0] = 1.0f;
    SetPointSizeMax(1.0f, false);
    m_stackTopPointSize = 0;
    m_stackPointSize[0] = 1.0f;
    SetPointSize(1.0f, false);
    m_stackTopPointScaleA = 0;
    m_stackPointScaleA[0] = 1.0f;
    SetPointScaleA(1.0f, false);
    m_stackTopPointScaleB = 0;
    m_stackPointScaleB[0] = 0.0f;
    SetPointScaleB(0.0f, false);
    m_stackTopPointScaleC = 0;
    m_stackPointScaleC[0] = 0.0f;
    SetPointScaleC(0.0f, false);
    m_stackTopTFactor = 0;
    m_stackTFactor[0] = 0;
    SetTFactor(0, false);
    m_stackTopLocalViewer = 0;
    m_stackLocalViewer[0] = false;
    SetLocalViewer(false, false);
    m_stackTopSpecularLighting = 0;
    m_stackSpecularLighting[0] = false;
    SetSpecularLighting(false, false);
    m_stackTopColorWriteMask = 0;
    m_stackColorWriteMask[0] = 0xffffffff;
    SetColorWriteMask(0xffffffff, false);
    m_stackTopNPatchLevel = 0;
    m_stackNPatchLevel[0] = 0.0f;
    SetNPatchLevel(0.0f, false);
    m_stackTopStencilState = 0;
    m_stackStencilState[0] = false;
    SetStencilState(false, false);
    m_stackTopStencilMask = 0;
    m_stackStencilMask[0] = 0xffffffff;
    SetStencilMask(0xffffffff, false);
    m_stackTopDithering = 0;
    m_stackDithering[0] = false;
    SetDithering(false, false);
    m_stackTopStencilRef = 0;
    m_stackStencilRef[0] = 0;
    SetStencilRef(0, false);
    m_stackTopStencilWriteMask = 0;
    m_stackStencilWriteMask[0] = 0xffffffff;
    SetStencilWriteMask(0xffffffff, false);
    m_stackTopStencilFunc = 0;
    m_stackStencilFunc[0] = M3DCMP_ALWAYS;
    SetStencilFunc(M3DCMP_ALWAYS, false);
    m_stackTopStencilFail = 0;
    m_stackStencilFail[0] = OP_KEEP;
    SetStencilFail(OP_KEEP, false);
    m_stackTopStencilZFail = 0;
    m_stackStencilZFail[0] = OP_KEEP;
    SetStencilZFail(OP_KEEP, false);
    m_stackTopStencilPass = 0;
    m_stackStencilPass[0] = OP_KEEP;
    SetStencilPass(OP_KEEP, false);
    m_stackTopStencil2SidedEnable = 0;
    m_stackStencil2SidedEnable[0] = false;
    SetStencil2SidedEnable(false, false);
    m_stackTopStencilCcwFunc = 0;
    m_stackStencilCcwFunc[0] = M3DCMP_ALWAYS;
    SetStencilCcwFunc(M3DCMP_ALWAYS, false);
    m_stackTopStencilCcwFail = 0;
    m_stackStencilCcwFail[0] = OP_KEEP;
    SetStencilCcwFail(OP_KEEP, false);
    m_stackTopStencilCcwZFail = 0;
    m_stackStencilCcwZFail[0] = OP_KEEP;
    SetStencilCcwZFail(OP_KEEP, false);
    m_stackTopStencilCcwPass = 0;
    m_stackStencilCcwPass[0] = OP_KEEP;
    SetStencilCcwPass(OP_KEEP, false);
    m_stackTopMultiSample = 0;
    m_stackMultiSample[0] = false;
    SetMultiSample(false, false);
    m_stackTopMultiSampleMask = 0;
    m_stackMultiSampleMask[0] = 0xffffffff;
    SetMultiSampleMask(0xffffffff, false);
}

// orig 0x60ef30 device.cpp:2434
// u0, v0, u1, v1 are never read. The matrices are built with the engine's CMatrix (the inlined
// code zeroes each one and stores the diagonal / translation / rotation elements, then multiplies
// them with the inlined CMatrix operator*).
void CDevice::TgEnableSetLinearSt(int stage, float sx, float sz, float tx, float tz, float roty, bool camSpace, float u0,
                                  float v0, float u1, float v1)
{
    CMatrix shift;
    shift.translation(-tx, 0.0f, -tz);

    CMatrix scale;
    scale.scaling(sx, 0.0f, sz);

    CMatrix rt;
    rt.rotY(roty);

    CMatrix shiftHalf1;
    shiftHalf1.translation(0.5f, 0.0f, 0.5f);

    CMatrix Change;
    Change.zero();
    Change._11 = 1.0f;
    Change._23 = 1.0f;
    Change._32 = 1.0f;
    Change._44 = 1.0f;

    CMatrix m = shift * scale * rt * shiftHalf1 * Change;
    TgEnableSetMatrixSt(stage, &m, camSpace);
}

// orig 0x6142b0 device.cpp:2460
void CDevice::TgEnableSetMatrixSt(int stage, CMatrix const* m, bool camSpace)
{
    setTextureStageState(stage, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);
    TgSetTcSource(stage, TC_FROM_POSITION_IN_CAMERA_SPACE, stage);

    if (camSpace)
        SetXFormMatrix(D3DTS_TEXTURE0 + stage, *m);
    else
        SetXFormMatrix(D3DTS_TEXTURE0 + stage, MatGetInv() * *m);
}

// orig 0x614710 device.cpp:2474
void CDevice::TgEnableSetMatrixStr(int stage, CMatrix const* m, bool camSpace)
{
    setTextureStageState(stage, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT3);
    TgSetTcSource(stage, TC_FROM_POSITION_IN_CAMERA_SPACE, stage);

    if (camSpace)
        SetXFormMatrix(D3DTS_TEXTURE0 + stage, *m);
    else
        SetXFormMatrix(D3DTS_TEXTURE0 + stage, MatGetInv() * *m);
}

// orig 0x614b70 device.cpp:2487
void CDevice::TgEnableSetMatrixStrReflection(int stage, CMatrix const& m, bool camSpace)
{
    setTextureStageState(stage, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT3);
    TgSetTcSource(stage, TC_FROM_REFLECTION_IN_CAMERA_SPACE, stage);

    if (camSpace)
        SetXFormMatrix(D3DTS_TEXTURE0 + stage, m);
    else
        SetXFormMatrix(D3DTS_TEXTURE0 + stage, MatGetInv() * m);
}

// orig 0x614fd0 device.cpp:2500
void CDevice::TgSetTransformMode(int stage, TgMode mode)
{
    unsigned int flags = D3DTTFF_DISABLE;

    switch (mode)
    {
    case TG_DISABLE:
        flags = D3DTTFF_DISABLE;
        break;

    case TG_THRU_1:
        flags = D3DTTFF_COUNT1;
        break;

    case TG_THRU_2:
        flags = D3DTTFF_COUNT2;
        break;

    case TG_THRU_3:
        flags = D3DTTFF_COUNT3;
        break;

    case TG_THRU_4:
        flags = D3DTTFF_COUNT4;
        break;

    case TG_PROJ_1:
        flags = D3DTTFF_COUNT1 | D3DTTFF_PROJECTED;
        break;

    case TG_PROJ_2:
        flags = D3DTTFF_COUNT2 | D3DTTFF_PROJECTED;
        break;

    case TG_PROJ_3:
        flags = D3DTTFF_COUNT3 | D3DTTFF_PROJECTED;
        break;

    case TG_PROJ_4:
        flags = D3DTTFF_COUNT4 | D3DTTFF_PROJECTED;
        break;

    case TG_PROJ_PS11:
        flags = D3DTTFF_PROJECTED;
        break;
    }

    setTextureStageState(stage, D3DTSS_TEXTURETRANSFORMFLAGS, flags);
}

// orig 0x615090 device.cpp:2552
// The camera space sources replace the index (the index is not or'ed in).
void CDevice::TgSetTcSource(int stage, TcSource mode, int index)
{
    unsigned int val;

    switch (mode)
    {
    case TC_FROM_VERTEX:
        val = index;
        break;

    case TC_FROM_NORMAL_IN_CAMERA_SPACE:
        val = D3DTSS_TCI_CAMERASPACENORMAL;
        break;

    case TC_FROM_POSITION_IN_CAMERA_SPACE:
        val = D3DTSS_TCI_CAMERASPACEPOSITION;
        break;

    case TC_FROM_REFLECTION_IN_CAMERA_SPACE:
        val = D3DTSS_TCI_CAMERASPACEREFLECTIONVECTOR;
        break;

    default:
        val = index;
        break;
    }

    setTextureStageState(stage, D3DTSS_TEXCOORDINDEX, val);
}

// orig 0x60fef0 device.cpp:2580
void CDevice::TgDisable(int stage)
{
    TgSetTransformMode(stage, TG_DISABLE);
    TgSetTcSource(stage, TC_FROM_VERTEX, stage);
}

// orig 0x60ff20 device.cpp:2588
void CDevice::SetTextureMatrix(int stage, CMatrix const* mat)
{
    SetXFormMatrix(D3DTS_TEXTURE0 + stage, *mat);
}

// orig 0x60ff30 device.cpp:2595 - straight to the device, bypassing the stage state cache.
void CDevice::Set2x2BumpMatrix(int stage, float m00, float m01, float m10, float m11)
{
    m_pd3dDevice->SetTextureStageState(stage, D3DTSS_BUMPENVMAT00, *(DWORD*)&m00);
    m_pd3dDevice->SetTextureStageState(stage, D3DTSS_BUMPENVMAT01, *(DWORD*)&m01);
    m_pd3dDevice->SetTextureStageState(stage, D3DTSS_BUMPENVMAT10, *(DWORD*)&m10);
    m_pd3dDevice->SetTextureStageState(stage, D3DTSS_BUMPENVMAT11, *(DWORD*)&m11);
}

// orig 0x60ffa0 device.cpp:2608
bool CDevice::defineInstancingSupport()
{
    bool result = false;
    if (m_d3dCaps.VertexShaderVersion >= D3DVS_VERSION(3, 0))
    {
        return true;
    }

    // The pre-SM3 hardware instancing hack: the driver accepts the 'INST' FOURCC as a surface
    // format and enables instancing when it is set as the point size.
    HRESULT hr = m_pD3D->CheckDeviceFormat(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, D3DFMT_X8R8G8B8, 0, D3DRTYPE_SURFACE,
                                           (D3DFORMAT)MAKEFOURCC('I', 'N', 'S', 'T'));
    if (hr == D3D_OK)
    {
        m_pd3dDevice->SetRenderState(D3DRS_POINTSIZE, MAKEFOURCC('I', 'N', 'S', 'T'));
        result = true;
    }

    return result;
}

// orig 0x615100 device.cpp:2639
void CDevice::logCaps()
{
    LogMsg(CStr("Texture max size: ") + CStr(m_d3dCaps.MaxTextureWidth) + "x" + CStr(m_d3dCaps.MaxTextureHeight));
    LogMsg(CStr("Guard band: (") + CStr(m_d3dCaps.GuardBandLeft) + "," + CStr(m_d3dCaps.GuardBandTop) + "," +
           CStr(m_d3dCaps.GuardBandRight) + "," + CStr(m_d3dCaps.GuardBandBottom) + ")");
    LogMsg(CStr("MaxTextureBlendStages: ") + CStr(m_d3dCaps.MaxTextureBlendStages));
    LogMsg(CStr("MaxSimultaneousTextures: ") + CStr(m_d3dCaps.MaxSimultaneousTextures));
    LogMsg(CStr("MaxActiveLights: ") + CStr(m_d3dCaps.MaxActiveLights));
    LogMsg(CStr("MaxUserClipPlanes: ") + CStr(m_d3dCaps.MaxUserClipPlanes));
    LogMsg(CStr("VertexShaderVersion: 0x") + CStr::format_("%x", m_d3dCaps.VertexShaderVersion));
    LogMsg(CStr("PixelShaderVersion: 0x") + CStr::format_("%x", m_d3dCaps.PixelShaderVersion));

    LogMsg(CStr("Geometry instancing: ") +
           (m_featureSupported[FEATURE_HARDWARE_INSTANCING] ? "supported" : "not supported"));
    LogMsg(CStr("Depth textures: ") + (m_featureSupported[FEATURE_DEPTH_TEXTURES] ? "supported" : "not supported"));
    bool bDepthBiasSupported = (m_d3dCaps.RasterCaps & D3DPRASTERCAPS_DEPTHBIAS) != 0;
    LogMsg(CStr("Depth bias supported: ") + (bDepthBiasSupported ? "yes" : "no"));
    bool bSlopeScaleBiasSupported = (m_d3dCaps.RasterCaps & D3DPRASTERCAPS_SLOPESCALEDEPTHBIAS) != 0;
    LogMsg(CStr("Slope-scale based depth bias supported: ") + (bSlopeScaleBiasSupported ? "yes" : "no"));

    LogMsg(CStr("HwRasterization: ") + ((m_d3dCaps.DevCaps & D3DDEVCAPS_HWRASTERIZATION) ? "true" : "false"));
    LogMsg(CStr("HwTnL: ") + ((m_d3dCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT) ? "true" : "false"));
    LogMsg(CStr("PureDevice: ") + ((m_d3dCaps.DevCaps & D3DDEVCAPS_PUREDEVICE) ? "true" : "false"));
    LogMsg(CStr("CubeMap: ") + ((m_d3dCaps.TextureCaps & D3DPTEXTURECAPS_CUBEMAP) ? "true" : "false"));
    LogMsg(CStr("VolumeMap: ") + ((m_d3dCaps.TextureCaps & D3DPTEXTURECAPS_VOLUMEMAP) ? "true" : "false"));
    LogMsg("TextureAddressCaps:");
    LogMsg(CStr("\twrap: ") + ((m_d3dCaps.TextureAddressCaps & D3DPTADDRESSCAPS_WRAP) ? "true" : "false"));
    LogMsg(CStr("\tclamp: ") + ((m_d3dCaps.TextureAddressCaps & D3DPTADDRESSCAPS_CLAMP) ? "true" : "false"));
    LogMsg(CStr("\tborder: ") + ((m_d3dCaps.TextureAddressCaps & D3DPTADDRESSCAPS_BORDER) ? "true" : "false"));
    LogMsg(CStr("Projective textures: ") + ((m_d3dCaps.TextureCaps & D3DPTEXTURECAPS_PROJECTED) ? "true" : "false"));
    LogMsg(CStr("MaxPointSize: ") + CStr(m_d3dCaps.MaxPointSize));
    LogMsg(CStr("MaxPrimitiveCount: ") + CStr(m_d3dCaps.MaxPrimitiveCount));
    LogMsg(CStr("MaxVertexIndex: ") + CStr(m_d3dCaps.MaxVertexIndex));
    LogMsg(CStr("MaxStreams: ") + CStr(m_d3dCaps.MaxStreams));
    LogMsg(CStr("MaxStreamsStride: ") + CStr(m_d3dCaps.MaxStreamStride));
    LogMsg(CStr("MaxAnisotropy: ") + CStr(m_d3dCaps.MaxAnisotropy));
    LogMsg(CStr("MaxVertexBlendMatrices: ") + CStr(m_d3dCaps.MaxVertexBlendMatrices));
    LogMsg(CStr("MaxVertexBlendMatrixIndex: ") + CStr(m_d3dCaps.MaxVertexBlendMatrixIndex));
    LogMsg(CStr("FullScreenGamma: ") + ((m_d3dCaps.Caps2 & D3DCAPS2_FULLSCREENGAMMA) ? "true" : "false"));
    LogMsg(CStr("COLORWRITEENABLE: ") + ((m_d3dCaps.PrimitiveMiscCaps & D3DPMISCCAPS_COLORWRITEENABLE) ? "true" : "false"));
    LogMsg(CStr("MASKZ: ") + ((m_d3dCaps.PrimitiveMiscCaps & D3DPMISCCAPS_MASKZ) ? "true" : "false"));
    LogMsg(CStr("BLENDOP: ") + ((m_d3dCaps.PrimitiveMiscCaps & D3DPMISCCAPS_BLENDOP) ? "true" : "false"));
    LogMsg(CStr("CLIPPLANESCALEDPOINTS: ") +
           ((m_d3dCaps.PrimitiveMiscCaps & D3DPMISCCAPS_CLIPPLANESCALEDPOINTS) ? "true" : "false"));
    LogMsg(CStr("CLIPTLVERTS: ") + ((m_d3dCaps.PrimitiveMiscCaps & D3DPMISCCAPS_CLIPTLVERTS) ? "true" : "false"));
    LogMsg(CStr("TSSARGTEMP: ") + ((m_d3dCaps.PrimitiveMiscCaps & D3DPMISCCAPS_TSSARGTEMP) ? "true" : "false"));
    LogMsg(CStr("DIRECTIONALLIGHTS: ") +
           ((m_d3dCaps.VertexProcessingCaps & D3DVTXPCAPS_DIRECTIONALLIGHTS) ? "true" : "false"));
    LogMsg(CStr("LOCALVIEWER: ") + ((m_d3dCaps.VertexProcessingCaps & D3DVTXPCAPS_LOCALVIEWER) ? "true" : "false"));
    LogMsg(CStr("POSITIONALLIGHTS: ") +
           ((m_d3dCaps.VertexProcessingCaps & D3DVTXPCAPS_POSITIONALLIGHTS) ? "true" : "false"));
    LogMsg(CStr("TEXGEN:  ") + ((m_d3dCaps.VertexProcessingCaps & D3DVTXPCAPS_TEXGEN) ? "true" : "false"));
    LogMsg(CStr("TWEENING:  ") + ((m_d3dCaps.VertexProcessingCaps & D3DVTXPCAPS_TWEENING) ? "true" : "false"));
    LogMsg(CStr("Dp3: ") + ((m_d3dCaps.TextureOpCaps & D3DTEXOPCAPS_DOTPRODUCT3) ? "true" : "false"));
    LogMsg(CStr("Lerp: ") + ((m_d3dCaps.TextureOpCaps & D3DTEXOPCAPS_LERP) ? "true" : "false"));
    LogMsg(CStr("MaxNpatchTessellationLevel: ") + CStr(m_d3dCaps.MaxNpatchTessellationLevel));
    LogMsg(CStr("Query VCACHE support: ") + (m_featureSupported[FEATURE_QUERY_VCACHE] ? "true" : "false"));
    LogMsg(CStr("Query EVENT support: ") + (m_featureSupported[FEATURE_QUERY_EVENT] ? "true" : "false"));
    LogMsg(CStr("Query OCCLUSION support: ") + (m_featureSupported[FEATURE_QUERY_OCCLUSION] ? "true" : "false"));
    LogMsg(CStr("Query TIMESTAMP support: ") + (m_featureSupported[FEATURE_QUERY_TIMESTAMP] ? "true" : "false"));
    LogMsg(CStr("Query TIMESTAMPDISJOINT support: ") +
           (m_featureSupported[FEATURE_QUERY_TIMESTAMPDISJOINT] ? "true" : "false"));
    LogMsg(CStr("Query TIMESTAMPFREQ support: ") + (m_featureSupported[FEATURE_QUERY_TIMESTAMPFREQ] ? "true" : "false"));
    LogMsg(CStr("Query PIPELINETIMINGS support: ") +
           (m_featureSupported[FEATURE_QUERY_PIPELINETIMINGS] ? "true" : "false"));
    LogMsg(CStr("Query INTERFACETIMINGS support: ") +
           (m_featureSupported[FEATURE_QUERY_INTERFACETIMINGS] ? "true" : "false"));
    LogMsg(CStr("Query VERTEXTIMINGS support: ") + (m_featureSupported[FEATURE_QUERY_VERTEXTIMINGS] ? "true" : "false"));
    LogMsg(CStr("Query PIXELTIMINGS support: ") + (m_featureSupported[FEATURE_QUERY_PIXELTIMINGS] ? "true" : "false"));
    LogMsg(CStr("Query BANDWIDTHTIMINGS support: ") +
           (m_featureSupported[FEATURE_QUERY_BANDWIDTHTIMINGS] ? "true" : "false"));
    LogMsg(CStr("Query CACHEUTILIZATION support: ") +
           (m_featureSupported[FEATURE_QUERY_CACHEUTILIZATION] ? "true" : "false"));
}

// orig 0x6194d0 device.cpp:2711
void CDevice::SetGamma(float gamma, float brightness, float contrast)
{
    if (m_pd3dDevice == 0)
    {
        return;
    }

    // The three 0..1 sliders are mapped to a contrast factor of 0.5..2, a brightness offset of
    // -0.5..0.5 and a gamma exponent of 2..0.5.
    if (contrast <= 0.5f)
    {
        contrast = (contrast * 2.0f + 1.0f) * 0.5f;
    }
    else
    {
        contrast = (contrast - 0.5f) * 2.0f;
        contrast += 1.0f;
    }

    if (brightness <= 0.5f)
    {
        brightness *= 2.0f;
        brightness = brightness * 0.5f - 0.5f;
    }
    else
    {
        brightness = (brightness - 0.5f) * 2.0f;
        brightness *= 0.5f;
    }

    if (gamma <= 0.5f)
    {
        gamma *= 2.0f;
        gamma *= 1.0f;
        gamma = 2.0f - gamma;
    }
    else
    {
        gamma = (gamma - 0.5f) * 2.0f;
        gamma *= 0.5f;
        gamma = 1.0f - gamma;
    }

    for (int i = 0; i < 256; i++)
    {
        float value = ((float)i * (1.0f / 256.0f) - 0.5f) * contrast + 0.5f;
        if (value < 0.0f)
        {
            value = 0.0f;
        }
        else if (value > 1.0f)
        {
            value = 1.0f;
        }

        // The original evaluates the power and the sum on the x87 stack (double precision).
        value = (float)(pow((double)value, (double)gamma) + brightness);
        if (value < 0.0f)
        {
            value = 0.0f;
        }
        else if (value > 1.0f)
        {
            value = 1.0f;
        }
        WORD w = (WORD)(int)((double)value * 65535.0);

        m_gamma.red[i] = w;
        m_gamma.green[i] = w;
        m_gamma.blue[i] = w;
    }

    m_pd3dDevice->SetGammaRamp(0, D3DSGR_NO_CALIBRATION, &m_gamma);
}

// orig 0x610000 device.cpp:2783
void CDevice::rstCaches()
{
    m_curFVF = 0xffffffff;
    m_curVertexShader = 0;
    m_curPixelShader = 0;
    m_curVertexDecl = 0;
    for (int i = 0; i < 4; i++)
    {
        m_curVb[i] = 0;
        m_curStride[i] = 0;
    }
    m_curIb = 0;
    m_latchedIb = 0;
    for (int i = 0; i < 8; i++)
    {
        m_curTexStages[i] = 0;
        m_lastResult = m_pd3dDevice->SetTexture(i, 0);
    }
}

// orig 0x6100a0 device.cpp:2808
HRESULT CDevice::setFVF(unsigned int fvf)
{
    if (m_curFVF == fvf)
    {
        return 0;
    }
    m_curFVF = fvf;
    return m_pd3dDevice->SetFVF(fvf);
}

// orig 0x6100d0 device.cpp:2822
HRESULT CDevice::setVertexDeclaration(IDirect3DVertexDeclaration9* vd)
{
    if (vd == m_curVertexDecl)
    {
        return 0;
    }
    m_curVertexDecl = vd;
    return m_pd3dDevice->SetVertexDeclaration(vd);
}

// orig 0x610100 device.cpp:2836
HRESULT CDevice::setStreamSource(int stream, IDirect3DVertexBuffer9* vb, unsigned int stride)
{
    if (m_curVb[stream] == vb && m_curStride[stream] == stride)
    {
        return 0;
    }
    m_curVb[stream] = vb;
    m_curStride[stream] = stride;
    return m_pd3dDevice->SetStreamSource(stream, vb, 0, stride);
}

// orig 0x610150 device.cpp:2855
void CDevice::setIndices(IDirect3DIndexBuffer9* ib, unsigned int baseIdx)
{
    m_latchedCheck = true;
    m_latchedIb = ib;
    m_latchedIbBaseIdx = baseIdx;
}

// orig 0x610170 device.cpp:2864
HRESULT CDevice::setTexture(int stage, IDirect3DBaseTexture9* tex)
{
    if (m_curTexStages[stage] == tex)
    {
        return 0;
    }
    m_stats.swTextures++;
    m_curTexStages[stage] = tex;
    return m_pd3dDevice->SetTexture(stage, tex);
}

// orig 0x6101b0 device.cpp:2882
void CDevice::ActuateStates(bool bForFFP)
{
    if (m_userClipPlaneEnabled)
    {
        ActuateClipPlanes(bForFFP);
    }
    if (bForFFP)
    {
        ActuateMatrices();
    }
    if (m_latchedCheck)
    {
        m_latchedCheck = false;
        if (m_latchedIb != m_curIb)
        {
            m_curIb = m_latchedIb;
            m_lastResult = m_pd3dDevice->SetIndices(m_latchedIb);
        }
    }
}

// orig 0x610220 device.cpp:2910
HRESULT CDevice::setTextureStageState(unsigned int stage, D3DTEXTURESTAGESTATETYPE type, unsigned int value)
{
    if (m_curTexStagesStates[stage][type] == value)
    {
        return 0;
    }
    m_stats.swTextureStageStates++;
    m_curTexStagesStates[stage][type] = value;
    return m_pd3dDevice->SetTextureStageState(stage, type, value);
}

// orig 0x610270 device.cpp:2940
HRESULT CDevice::setTextureSamplerState(unsigned int stage, D3DSAMPLERSTATETYPE type, unsigned int value)
{
    if (m_curTexSamplerStates[stage][type] == value)
    {
        return 0;
    }
    m_stats.swTextureSamplerStates++;
    m_curTexSamplerStates[stage][type] = value;
    return m_pd3dDevice->SetSamplerState(stage, type, value);
}

// orig 0x6102c0 device.cpp:2958
HRESULT CDevice::setStreamSourceFreq(unsigned int stage, unsigned int freq)
{
    if (m_curStreamFreq[stage] == freq)
    {
        return 0;
    }
    m_curStreamFreq[stage] = freq;
    return m_pd3dDevice->SetStreamSourceFreq(stage, freq);
}

// orig 0x610300 device.cpp:2969
HRESULT CDevice::setRenderState(D3DRENDERSTATETYPE state, unsigned int val)
{
    if (m_curRenderState[state] == val)
    {
        return 0;
    }
    m_stats.swRenderStates++;
    m_curRenderState[state] = val;
    return m_pd3dDevice->SetRenderState(state, val);
}

// orig 0x610340 device.cpp:2986
unsigned int CDevice::getRenderState(D3DRENDERSTATETYPE state)
{
    return m_curRenderState[state];
}

// orig 0x610350 device.cpp:2995
void CDevice::ClearViewport(ClearFlags flags, unsigned int bkClr)
{
    m_lastResult = m_pd3dDevice->Clear(0, 0, flags, bkClr, 1.0f, 0);

    if (flags & M3DCLEAR_S)
        m_stencilLevel[m_activeStencilTarget] = 0;
}

// orig 0x6103a0 device.cpp:3007 - relative coordinates live in a 1024x768 space.
void CDevice::RelToAbs(float& x, float& y)
{
    float h = (float)m_d3dsdBackBuffer.Height;
    float w = (float)m_d3dsdBackBuffer.Width;
    x = x * w * (1.0f / 1024.0f);
    y = y * h * (1.0f / 768.0f);
}

// orig 0x6103f0 device.cpp:3023
void CDevice::AbsToRel(float& x, float& y)
{
    float h = (float)m_d3dsdBackBuffer.Height;
    float w = (float)m_d3dsdBackBuffer.Width;
    x = x / w * 1024.0f;
    y = y / h * 768.0f;
}

// orig 0x619670 device.cpp:3037
CStr getD3dErrorStr(HRESULT hr)
{
    switch (hr)
    {
    case D3D_OK:
        return "D3D_OK";
    case D3DERR_OUTOFVIDEOMEMORY:
        return "D3DERR_OUTOFVIDEOMEMORY";
    case E_OUTOFMEMORY:
        return "E_OUTOFMEMORY";
    case D3DERR_DRIVERINTERNALERROR:
        return "D3DERR_DRIVERINTERNALERROR";
    case D3DERR_DEVICELOST:
        return "D3DERR_DEVICELOST";
    case D3DERR_NOTAVAILABLE:
        return "D3DERR_NOTAVAILABLE";
    case D3DERR_INVALIDCALL:
        return "D3DERR_INVALIDCALL";
    case D3DXERR_INVALIDDATA:
        return "D3DXERR_INVALIDDATA";
    case D3DXERR_INVALIDMESH:
        return "D3DXERR_INVALIDMESH";
    default:
        return "unknown";
    }
}

// orig 0x610450 device.cpp:3089
int CDevice::SetActiveState(int state)
{
    m_isActive = state;
    return state;
}

// orig 0x619770 device.cpp:3099
int CDevice::CanRender()
{
    // 3105
    m_lastResult = m_pd3dDevice->TestCooperativeLevel();
    if (FAILED(m_lastResult))
    {
        // 3108
        if (m_lastResult == D3DERR_DEVICELOST)
        {
            // 3109
            return 0;
        }

        // 3112
        if (m_lastResult == D3DERR_DEVICENOTRESET)
        {
            // 3116
            if (!Reset())
            {
                // 3123
                m3d::EngineConfig& cfg = g_kernel->GetEngineCfg();
                if (SwitchDisplayModes(cfg.m_mainWnd, cfg.m_r_width.GetI(), cfg.m_r_height.GetI(),
                                       cfg.m_r_fullScreen.GetB()))
                {
                    // 3126
                    return 2;
                }
            }
            // 3129
            return 1;
        }
    }

    // 3133
    return SUCCEEDED(m_lastResult);
}

// orig 0x61c620 device.cpp:3139
TexHandle CDevice::AddTextureFromBackBuffer(int width, int height)
{
    TexHandle newTex = AddDynamicTexture("$TexFromBackBuf", width, height, TM_DTF_SAME_AS_RENDER_TARGET);
    if (newTex.IsValid())
    {
        SetTextureParameter(newTex, TM_WRAP_S, D3DTADDRESS_CLAMP);
        SetTextureParameter(newTex, TM_WRAP_T, D3DTADDRESS_CLAMP);

        if (AddTextureFromBackBuffer(newTex))
            return newTex;

        ReleaseTexture(newTex);
    }

    TexHandle invalid;
    invalid.SetInvalid();
    return invalid;
}

// orig 0x61c6c0 device.cpp:3166
unsigned int CDevice::AddTextureFromBackBuffer(TexHandle srcTex)
{
    if (TexId(srcTex) < 0)
        return 0;

    IDirect3DSurface9* pDstSurf = 0;
    HRESULT hr = m_textures[TexId(srcTex)].m_maps[0]->m_pTex2d->GetSurfaceLevel(0, &pDstSurf);
    if (FAILED(hr))
        return 0;

    IDirect3DSurface9* pBackBuffer = 0;
    hr = m_pd3dDevice->GetRenderTarget(0, &pBackBuffer);
    if (SUCCEEDED(hr))
    {
        hr = D3DXLoadSurfaceFromSurface(pDstSurf, 0, 0, pBackBuffer, 0, 0, D3DX_FILTER_POINT, 0);
        pBackBuffer->Release();
    }

    pDstSurf->Release();

    if (FAILED(hr))
        return 0;

    return 1;
}

// orig 0x61c770 device.cpp:3204 - the surface level is never released (as in the original).
int CDevice::SaveTextureToTgaFile(TexHandle tex, const char* fileName)
{
    IDirect3DSurface9* surf = 0;
    if (FAILED(m_textures[TexId(tex)].m_maps[0]->m_pTex2d->GetSurfaceLevel(0, &surf)))
        return 0;

    return SUCCEEDED(SaveSurfaceToTGAFile(m_pd3dDevice, fileName, surf, -1, -1));
}

// orig 0x61c7e0 device.cpp:3216 - ImageFileFormats and D3DXIMAGE_FILEFORMAT have the same values.
int CDevice::SaveTextureToFile(TexHandle tex, const char* fileName, ImageFileFormats format)
{
    if (!IsTexValid(tex))
        return 0;

    return SUCCEEDED(
        D3DXSaveTextureToFileA(fileName, (D3DXIMAGE_FILEFORMAT)format, m_textures[TexId(tex)].m_maps[0]->m_pTex, 0));
}

// orig 0x610460 device.cpp:3233
Viewport CDevice::GetViewport()
{
    return m_curViewport;
}

// orig 0x6104a0 device.cpp:3240
int CDevice::SetViewport(Viewport const& port)
{
    m_curViewport = port;

    m_curViewportD3D.X = port.m_x0;
    m_curViewportD3D.Y = port.m_y0;
    m_curViewportD3D.Width = port.m_width;
    m_curViewportD3D.Height = port.m_height;
    m_curViewportD3D.MinZ = port.m_zMin;
    m_curViewportD3D.MaxZ = port.m_zMax;

    return SUCCEEDED(m_pd3dDevice->SetViewport(&m_curViewportD3D));
}

// orig 0x619840 device.cpp:3258
char* CDevice::GetCurBppStr(int* bpp)
{
    static char sstr[100];

    strcpy(sstr, getD3dFmtStr(m_d3dsdBackBuffer.Format).c_str());

    if (bpp)
    {
        switch (m_d3dsdBackBuffer.Format)
        {
        default:
            *bpp = 0;
            break;
        case D3DFMT_R8G8B8:
        case D3DFMT_A8R8G8B8:
        case D3DFMT_X8R8G8B8:
            *bpp = 32;
            break;
        case D3DFMT_R5G6B5:
        case D3DFMT_X1R5G5B5:
        case D3DFMT_A1R5G5B5:
        case D3DFMT_A4R4G4B4:
        case D3DFMT_X4R4G4B4:
            *bpp = 16;
            break;
        }
    }

    return sstr;
}

// orig 0x619900 device.cpp:3293
char* CDevice::GetLastErrorStr()
{
    static char sstr[100];

    if (SUCCEEDED(m_lastResult))
    {
        return 0;
    }

    strcpy(sstr, getD3dErrorStr(m_lastResult).c_str());

    return sstr;
}

// orig 0x61d230 device.cpp:3307
void CDevice::ResetStats()
{
    memset(&m_stats, 0, sizeof(m_stats));

    for (std::map<ShaderIdData, EffectImpl*>::iterator it = m_effects.begin(); it != m_effects.end(); ++it)
    {
        it->second->m_numPrimitives = 0;
        it->second->m_numDIPs = 0;
    }
}

// orig 0x610530 device.cpp:3332
void CDevice::GetStats(RenderStats& stats)
{
    stats = m_stats;
}

// orig 0x61c830 device.cpp:3339
void CDevice::ShowStats()
{
    if (!m_pSysFont)
        return;

    RECT rc;
    int y = 20;

    SetRect(&rc, 2, 2, 0, 0);
    m_pSysFont->DrawTextA(0, "Effect file", -1, &rc, DT_NOCLIP, 0xffffff00);
    SetRect(&rc, 200, 2, 0, 0);
    m_pSysFont->DrawTextA(0, "#prims", -1, &rc, DT_NOCLIP, 0xffffff00);
    SetRect(&rc, 300, 2, 0, 0);
    m_pSysFont->DrawTextA(0, "#dips", -1, &rc, DT_NOCLIP, 0xffffff00);

    unsigned int totalPrims = 0;
    unsigned int totalDIPs = 0;

    for (std::map<ShaderIdData, EffectImpl*>::const_iterator it = m_effects.begin(); it != m_effects.end(); ++it)
    {
        EffectImpl* effect = it->second;
        if (!effect || !effect->m_numPrimitives)
            continue;

        SetRect(&rc, 2, y, 0, 0);
        m_pSysFont->DrawTextA(0, NameFromFileName(effect->m_fileName).c_str(), -1, &rc, DT_NOCLIP, 0xffffffff);
        SetRect(&rc, 200, y, 0, 0);
        m_pSysFont->DrawTextA(0, CStr(effect->m_numPrimitives).c_str(), -1, &rc, DT_NOCLIP, 0xffffffff);
        SetRect(&rc, 300, y, 0, 0);
        m_pSysFont->DrawTextA(0, CStr(effect->m_numDIPs).c_str(), -1, &rc, DT_NOCLIP, 0xffffffff);

        totalPrims += effect->m_numPrimitives;
        totalDIPs += effect->m_numDIPs;
        y += 18;
    }

    y += 18;
    SetRect(&rc, 2, y, 0, 0);
    m_pSysFont->DrawTextA(0, "Total", -1, &rc, DT_NOCLIP, 0xffffffff);
    SetRect(&rc, 200, y, 0, 0);
    m_pSysFont->DrawTextA(0, CStr(totalPrims).c_str(), -1, &rc, DT_NOCLIP, 0xffffffff);
    SetRect(&rc, 300, y, 0, 0);
    m_pSysFont->DrawTextA(0, CStr(totalDIPs).c_str(), -1, &rc, DT_NOCLIP, 0xffffffff);
}

// orig 0x610550 device.cpp:3396
void* CDevice::GetInternalData()
{
    return m_pd3dDevice;
}

// orig 0x610560 device.cpp:3403
bool CDevice::IsFeatureSupported(DeviceFeature f)
{
    return m_featureSupported[f];
}

// orig 0x610570 device.cpp:3409
bool CDevice::IsMultiSamplingSupported(int numSamples)
{
    if (FAILED(m_pD3D->CheckDeviceMultiSampleType(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, m_d3dpp.BackBufferFormat, FALSE,
                                                  (D3DMULTISAMPLE_TYPE)numSamples, NULL)))
    {
        return false;
    }

    return SUCCEEDED(m_pD3D->CheckDeviceMultiSampleType(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,
                                                        m_d3dpp.AutoDepthStencilFormat, FALSE,
                                                        (D3DMULTISAMPLE_TYPE)numSamples, NULL));
}

namespace
{
    // Parses a dotted driver version "a.b.c.d" into out[0..3]. The last number takes the rest of
    // the string, so "a.b.c.d.e" parses too; fewer than four numbers fail.
    // orig 0x619ac0 device.cpp:3608
    bool ParseFourInt(CStr const& inputStr, int* out)
    {
        int startPos = 0;
        int i = 0;

        while (i < 4)
        {
            int dotPos = inputStr.find('.', startPos);
            if (dotPos == -1 && i != 3)
            {
                return false;
            }

            if (dotPos != -1 && i != 3)
            {
                out[i++] = atoi(inputStr.substr(startPos, dotPos).c_str());
            }
            else
            {
                out[i++] = atoi(inputStr.substr(startPos, CStr_npos).c_str());
            }
            startPos += dotPos + 1;
        }

        return true;
    }
}

// orig 0x61cb80 device.cpp:3426 - the surface level is never released (as in the original).
int CDevice::SetupDXCursorForce(TexHandle const& texId, int xHotSpot, int yHotSpot, int frame)
{
    if (!m_pd3dDevice)
        return 0;

    if (!IsTexValid(texId))
        return 0;

    CTexture& tex = m_textures[TexId(texId)];

    if (!tex.m_maps[0]->Is2D())
        return 0;

    if (frame < 0 || frame >= (int)tex.m_maps.size())
        frame = 0;

    IDirect3DSurface9* surf = 0;
    if (FAILED(tex.m_maps[frame]->m_pTex2d->GetSurfaceLevel(0, &surf)))
        return 0;

    if (FAILED(m_pd3dDevice->SetCursorProperties(xHotSpot, yHotSpot, surf)))
        return 0;

    if (m_currentDXCursorInfo.m_texId != texId)
    {
        ReleaseTexture(m_currentDXCursorInfo.m_texId);
        ReferenceTexture(texId);
    }

    m_currentDXCursorInfo.SetUp(texId, xHotSpot, yHotSpot, frame);

    return 1;
}

// orig 0x619950 device.cpp:3483
int CDevice::SetupDXCursor(TexHandle const& texId, int xHotSpot, int yHotSpot, int frame)
{
    if (m_currentDXCursorInfo.m_texId == texId && m_currentDXCursorInfo.m_xHotSpot == xHotSpot &&
        m_currentDXCursorInfo.m_yHotSpot == yHotSpot && m_currentDXCursorInfo.m_frame == frame)
        return 1;

    return SetupDXCursorForce(texId, xHotSpot, yHotSpot, frame);
}

// orig 0x6199b0 device.cpp:3500
void CDevice::MoveDXCursor(int x, int y)
{
    if (m_pd3dDevice)
    {
        POINT coord;
        coord.x = x;
        coord.y = y;

        if (!g_kernel->GetEngineCfg().m_r_fullScreen.GetB())
            ClientToScreen(m_d3dpp.hDeviceWindow, &coord);

        m_pd3dDevice->SetCursorPosition(coord.x, coord.y, D3DCURSOR_IMMEDIATE_UPDATE);
    }
}

// orig 0x6105d0 device.cpp:3525
void CDevice::ShowDXCursor(bool needShow)
{
    if (m_pd3dDevice)
        m_pd3dDevice->ShowCursor(needShow);
}

// orig 0x61cc90 device.cpp:3554
int CDevice::UpdateDXCursorFrame()
{
    if (!m_pd3dDevice)
        return 0;

    if (!IsTexValid(m_currentDXCursorInfo.m_texId))
        return 0;

    int frame = GetTextureCurrentFrame(m_currentDXCursorInfo.m_texId, -1.0);
    if (frame == -1)
        return 0;

    if (frame != m_currentDXCursorInfo.m_frame)
    {
        if (!SetupDXCursor(m_currentDXCursorInfo.m_texId, m_currentDXCursorInfo.m_xHotSpot,
                           m_currentDXCursorInfo.m_yHotSpot, frame))
            return 0;
    }

    return 1;
}

// orig 0x619a30 device.cpp:3585
bool CDevice::IsHardWareCursorAvailableForTexture(TexHandle const* texId)
{
    int w = 0;
    int h = 0;
    GetDims(*texId, w, h);

    return ((m_d3dCaps.CursorCaps & D3DCURSORCAPS_COLOR) && w == 32 && h == 32) ||
           !g_kernel->GetEngineCfg().m_r_fullScreen.GetB();
}

// orig 0x619be0 device.cpp:3629
int CDevice::SetupAllFeaturesSupport(CStr const& DevCompatibleFileName)
{
    D3DADAPTER_IDENTIFIER9 id;
    m_pD3D->GetAdapterIdentifier(D3DADAPTER_DEFAULT, 0, &id);

    // The stencil support is read off the depth stencil surface the device was created with.
    IDirect3DSurface9* ps;
    D3DSURFACE_DESC dd;
    m_lastResult = m_pd3dDevice->GetDepthStencilSurface(&ps);
    ps->GetDesc(&dd);
    ps->Release();

    bool haveStencil;
    switch (dd.Format)
    {
    default:
        haveStencil = false;
        break;
    case D3DFMT_D15S1:
    case D3DFMT_D24S8:
    case D3DFMT_D24X4S4:
        haveStencil = true;
        break;
    }

    unsigned int deviceId = id.DeviceId;
    unsigned int vendorId = id.VendorId;
    WORD wProduct = HIWORD(id.DriverVersion.HighPart);
    WORD wVersion = LOWORD(id.DriverVersion.HighPart);
    WORD wSubVersion = HIWORD(id.DriverVersion.LowPart);
    WORD wBuild = LOWORD(id.DriverVersion.LowPart);

    m_lastResult = m_pd3dDevice->GetDeviceCaps(&m_d3dCaps);

    m_featureSupported[FEATURE_STENCIL] = haveStencil;
    m_featureSupported[FEATURE_2SIDED_STENCIL] = haveStencil && (m_d3dCaps.StencilCaps & D3DSTENCILCAPS_TWOSIDED) != 0;

    m_featureSupported[FEATURE_VS_1_1] = (m_d3dCaps.VertexShaderVersion & 0xffff) >= 0x0101;
    m_featureSupported[FEATURE_VS_2_0] = (m_d3dCaps.VertexShaderVersion & 0xffff) >= 0x0200;
    m_featureSupported[FEATURE_VS_3_0] = (m_d3dCaps.VertexShaderVersion & 0xffff) >= 0x0300;
    m_featureSupported[FEATURE_PS_1_1] = (m_d3dCaps.PixelShaderVersion & 0xffff) >= 0x0101;
    m_featureSupported[FEATURE_PS_1_4] = (m_d3dCaps.PixelShaderVersion & 0xffff) >= 0x0104;
    m_featureSupported[FEATURE_PS_2_0] = (m_d3dCaps.PixelShaderVersion & 0xffff) >= 0x0200;
    m_featureSupported[FEATURE_PS_3_0] = (m_d3dCaps.PixelShaderVersion & 0xffff) >= 0x0300;

    m_featureSupported[FEATURE_NON_POW2_CONDITIONAL] = (m_d3dCaps.TextureCaps & D3DPTEXTURECAPS_NONPOW2CONDITIONAL) != 0;
    m_featureSupported[FEATURE_NON_POW2_RT] = false;
    m_featureSupported[FEATURE_MRT] = false;

    m_featureSupported[FEATURE_QUERY_BANDWIDTHTIMINGS] = SUCCEEDED(m_pd3dDevice->CreateQuery(D3DQUERYTYPE_BANDWIDTHTIMINGS, NULL));
    m_featureSupported[FEATURE_QUERY_CACHEUTILIZATION] = SUCCEEDED(m_pd3dDevice->CreateQuery(D3DQUERYTYPE_CACHEUTILIZATION, NULL));
    m_featureSupported[FEATURE_QUERY_EVENT] = SUCCEEDED(m_pd3dDevice->CreateQuery(D3DQUERYTYPE_EVENT, NULL));
    m_featureSupported[FEATURE_QUERY_INTERFACETIMINGS] = SUCCEEDED(m_pd3dDevice->CreateQuery(D3DQUERYTYPE_INTERFACETIMINGS, NULL));
    m_featureSupported[FEATURE_QUERY_OCCLUSION] = SUCCEEDED(m_pd3dDevice->CreateQuery(D3DQUERYTYPE_OCCLUSION, NULL));
    m_featureSupported[FEATURE_QUERY_PIPELINETIMINGS] = SUCCEEDED(m_pd3dDevice->CreateQuery(D3DQUERYTYPE_PIPELINETIMINGS, NULL));
    m_featureSupported[FEATURE_QUERY_PIXELTIMINGS] = SUCCEEDED(m_pd3dDevice->CreateQuery(D3DQUERYTYPE_PIXELTIMINGS, NULL));
    m_featureSupported[FEATURE_QUERY_TIMESTAMP] = SUCCEEDED(m_pd3dDevice->CreateQuery(D3DQUERYTYPE_TIMESTAMP, NULL));
    m_featureSupported[FEATURE_QUERY_TIMESTAMPDISJOINT] = SUCCEEDED(m_pd3dDevice->CreateQuery(D3DQUERYTYPE_TIMESTAMPDISJOINT, NULL));
    m_featureSupported[FEATURE_QUERY_TIMESTAMPFREQ] = SUCCEEDED(m_pd3dDevice->CreateQuery(D3DQUERYTYPE_TIMESTAMPFREQ, NULL));
    m_featureSupported[FEATURE_QUERY_VCACHE] = SUCCEEDED(m_pd3dDevice->CreateQuery(D3DQUERYTYPE_VCACHE, NULL));
    m_featureSupported[FEATURE_QUERY_VERTEXTIMINGS] = SUCCEEDED(m_pd3dDevice->CreateQuery(D3DQUERYTYPE_VERTEXTIMINGS, NULL));

    m_featureSupported[FEATURE_HARDWARE_INSTANCING] = defineInstancingSupport();

    m_featureSupported[FEATURE_DEPTH_TEXTURES] = m_texFormatDepth != D3DFMT_UNKNOWN;

    // The device compatibility database: per vendor and device id range (and driver version
    // range) a list of properties overriding the detected features.
    CStr err;
    ref_ptr<m3d::cmn::XmlFile> xmlFile = m3d::ReadXmlFile(DevCompatibleFileName.c_str(), &err);
    if (!xmlFile)
    {
        LogMsg(CStr("CDevice:: cannot load data\\DeviceCompatible.xml, all features use by default: ") + err);
        return 1;
    }

    ref_ptr<m3d::cmn::XmlNode> databaseNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_EMPTY, 0);
    xmlFile->GetFirstChild(databaseNode, "Database");

    ref_ptr<m3d::cmn::XmlNode> vendorNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_EMPTY, 0);
    databaseNode->GetFirstChild(vendorNode, "Vendor");
    while (!vendorNode->IsEmpty())
    {
        unsigned int vendID;
        m3d::SafeClrAttrib(vendID, vendorNode, "ID");
        if (vendID == vendorId)
        {
            break;
        }
        vendorNode->GetNextSibling(vendorNode, "Vendor");
    }
    if (vendorNode->IsEmpty())
    {
        return 1;
    }

    ref_ptr<m3d::cmn::XmlNode> deviceNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_EMPTY, 0);
    vendorNode->GetFirstChild(deviceNode, "Device");
    while (!deviceNode->IsEmpty())
    {
        unsigned int devIDMin;
        unsigned int devIDMax;
        m3d::SafeClrAttrib(devIDMin, deviceNode, "IDMin");
        m3d::SafeClrAttrib(devIDMax, deviceNode, "IDMax");

        CStr devName;
        m3d::SafeStrAttrib(devName, deviceNode, "Name");
        if (deviceId >= devIDMin && deviceId <= devIDMax)
        {
            // As shipped: an entry is skipped when the driver is NEWER than its "MinDriverVer" or
            // OLDER than its "MaxDriverVer" (component-wise).
            int vers[4];
            CStr drivVer;
            if (m3d::SafeStrAttrib(drivVer, deviceNode, "MinDriverVer"))
            {
                if (ParseFourInt(drivVer, vers))
                {
                    if (wProduct > vers[0] || wVersion > vers[1] || wSubVersion > vers[2] || wBuild > vers[3])
                    {
                        deviceNode->GetNextSibling(deviceNode, "Device");
                        continue;
                    }
                }
            }
            if (m3d::SafeStrAttrib(drivVer, deviceNode, "MaxDriverVer"))
            {
                if (ParseFourInt(drivVer, vers))
                {
                    if (wProduct < vers[0] || wVersion < vers[1] || wSubVersion < vers[2] || wBuild < vers[3])
                    {
                        deviceNode->GetNextSibling(deviceNode, "Device");
                        continue;
                    }
                }
            }
            break;
        }
        deviceNode->GetNextSibling(deviceNode, "Device");
    }
    if (deviceNode->IsEmpty())
    {
        return 1;
    }

    ref_ptr<m3d::cmn::XmlNode> propNode = xmlFile->CreateNode(m3d::cmn::XML_NODE_EMPTY, 0);
    deviceNode->GetFirstChild(propNode, "Property");
    while (!propNode->IsEmpty())
    {
        CStr propName;
        m3d::SafeStrAttrib(propName, propNode, "Name");
        if (propName == "MaxVS")
        {
            // The "Value" attribute as the original reads it: missing or negative counts as 0.
            int value = 0;
            if (!propNode->IsEmpty())
            {
                char const* attr = propNode->GetAttribute("Value");
                if (attr)
                {
                    int v = atoi(attr);
                    if (v >= 0)
                    {
                        value = v;
                    }
                }
            }
            m_featureSupported[FEATURE_VS_1_1] = value >= 11;
            m_featureSupported[FEATURE_VS_2_0] = value >= 20;
            m_featureSupported[FEATURE_VS_3_0] = value >= 30;
        }
        else if (propName == "MaxPS")
        {
            int value = 0;
            if (!propNode->IsEmpty())
            {
                char const* attr = propNode->GetAttribute("Value");
                if (attr)
                {
                    int v = atoi(attr);
                    if (v >= 0)
                    {
                        value = v;
                    }
                }
            }
            m_featureSupported[FEATURE_PS_1_1] = value >= 11;
            m_featureSupported[FEATURE_PS_1_4] = value >= 14;
            m_featureSupported[FEATURE_PS_2_0] = value >= 20;
            m_featureSupported[FEATURE_PS_3_0] = value >= 30;
        }
        else if (propName == "NonPow2Conditional")
        {
            int value = 0;
            if (!propNode->IsEmpty())
            {
                char const* attr = propNode->GetAttribute("Value");
                if (attr)
                {
                    int v = atoi(attr);
                    if (v >= 0)
                    {
                        value = v;
                    }
                }
            }
            m_featureSupported[FEATURE_NON_POW2_CONDITIONAL] = value != 0;
        }
        else if (propName == "UnsupportedCard")
        {
            LogMsg("This videocard is Unsupported");
            return 0;
        }
        propNode->GetNextSibling(propNode, "Property");
    }

    return 1;
}

// orig 0x6105f0 device.cpp:3804
void CDevice::rstStencilLevels()
{
    m_stencilLevel[0] = -1;
    m_stencilLevel[1] = -1;
}

// orig 0x610600 device.cpp:3812
void CDevice::SingleLayerStencilStart()
{
    int& level = m_stencilLevel[m_activeStencilTarget];

    if (level < 0)
        ClearViewport(M3DCLEAR_S, 0xffffffff);

    level++;

    SetStencilPass(OP_REPLACE, false);
    SetStencilZFail(OP_KEEP, false);
    SetStencilFail(OP_KEEP, false);
    SetStencilFunc(M3DCMP_GREATER, false);
    SetStencilRef(level, false);
    SetStencilMask(0xffffffff, false);
    SetStencilWriteMask(0xffffffff, false);

    setRenderState(D3DRS_STENCILENABLE, TRUE);
}

// orig 0x6106c0 device.cpp:3837
void CDevice::SingleLayerStencilContinue()
{
    int& level = m_stencilLevel[m_activeStencilTarget];

    // original: M3D_ASSERT(level > 0) -> g_Kernel->SysError("level > 0", ".\\device.cpp", 3839)
    if (!(level > 0))
        g_kernel->SysError((__FILE__ ":") + CStr(__LINE__), "level > 0");

    SetStencilPass(OP_REPLACE, false);
    SetStencilZFail(OP_KEEP, false);
    SetStencilFail(OP_KEEP, false);
    SetStencilFunc(M3DCMP_GREATER, false);
    SetStencilRef(level, false);
    SetStencilMask(0xffffffff, false);
    SetStencilWriteMask(0xffffffff, false);

    setRenderState(D3DRS_STENCILENABLE, TRUE);
}

// orig 0x610790 device.cpp:3856
void CDevice::SingleLayerStencilFinish()
{
    setRenderState(D3DRS_STENCILENABLE, FALSE);
}

// orig 0x6107c0 device.cpp:3863
void CDevice::GetDeviceMemStats(DeviceMemStats& stats)
{
    if (!m_isDevMemStatsValid)
    {
        UpdateVBMemStats();
        UpdateIBMemStats();
        UpdateTexMemStats();
        m_isDevMemStatsValid = true;
    }

    stats = m_devMemStats;
}

// orig 0x610800 device.cpp:3879
void CDevice::SetRenderThreadId(unsigned int renderThreadId)
{
    m_renderThreadId = renderThreadId;
}

// orig 0x610810 device.cpp:3885
void CDevice::EnableThreadSafeQuard(bool enable)
{
    m_bThreadSafeGuardEnabled = enable;
}

// orig 0x61df40 device.cpp:3903 m3d::RendererFactory: allocates sizeof(CDevice) (0xf1ec) through the
// kernel's AllocMem and constructs a CDevice in it, or returns 0 when the allocation fails. Not
// defined here: createIRenderer in main.cpp is its equivalent (operator new goes through g_mar).

