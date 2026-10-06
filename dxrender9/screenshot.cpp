// screenshot.cpp - the TGA writers and ScreenShot.
// Ported from the original dxrender9/screenshot.cpp.
#include "device.h"
#include "log.h"

#include <cstdint>
#include <cstdio>
#include <cstring>

#include <config.h>
#include <core/kernel.h>

namespace
{
    // The original reads g_Kernel->GetEngineCfg().m_r_screenshotTGA.GetB() here (EngineConfig+0x544, registered
    // in EngineConfig::EngineConfig as ("r_screenshotTGA", "yes", bool)). HTA's EngineConfig
    // (lib/include/config.h) has no such cvar, so this returns the original default.
    bool screenshotTGA()
    {
        return true;
    }
}  // namespace

// orig 0x63cd30 screenshot.cpp:43
HRESULT SaveSurfaceToTGAFile(IDirect3DDevice9* pD3DDevice, char const* szFileName, IDirect3DSurface9* pSurface,
                             int width, int height)
{
    HRESULT hr;
    D3DSURFACE_DESC d3dsd;
    IDirect3DSurface9* pSurfaceCopy;
    D3DLOCKED_RECT d3dLR;
    char targaheader[18];
    uint8_t* pImage;
    unsigned long x, y;

    pSurface->GetDesc(&d3dsd);

    if (width != -1)
        d3dsd.Width = width;
    if (height != -1)
        d3dsd.Height = height;

    hr = pD3DDevice->CreateOffscreenPlainSurface(d3dsd.Width, d3dsd.Height, d3dsd.Format, D3DPOOL_SCRATCH,
                                                 &pSurfaceCopy, NULL);
    if (FAILED(hr))
        return hr;

    D3DXLoadSurfaceFromSurface(pSurfaceCopy, NULL, NULL, pSurface, NULL, NULL, D3DX_FILTER_POINT, 0);

    hr = pSurfaceCopy->LockRect(&d3dLR, NULL, D3DLOCK_READONLY | D3DLOCK_NOSYSLOCK | D3DLOCK_NO_DIRTY_UPDATE);
    if (SUCCEEDED(hr))
    {
        FILE* f = fopen(szFileName, "wb");
        if (f)
        {
            setvbuf(f, NULL, _IOFBF, 0x10000);

            memset(targaheader, 0, sizeof(targaheader));
            targaheader[2] = 2;
            targaheader[12] = (char)(d3dsd.Width & 0xff);
            targaheader[13] = (char)((d3dsd.Width >> 8) & 0xff);
            targaheader[14] = (char)(d3dsd.Height & 0xff);
            targaheader[15] = (char)((d3dsd.Height >> 8) & 0xff);
            targaheader[16] = (d3dsd.Format == D3DFMT_A8R8G8B8 || d3dsd.Format == D3DFMT_A1R5G5B5) ? 32 : 24;
            targaheader[17] = 0x20;

            fwrite(targaheader, 18, 1, f);

            pImage = (uint8_t*)d3dLR.pBits;
            for (y = 0; y < d3dsd.Height; y++)
            {
                if (d3dsd.Format == D3DFMT_X8R8G8B8)
                {
                    for (x = 0; x < d3dsd.Width; x++)
                    {
                        putc(pImage[x * 4 + 0], f);
                        putc(pImage[x * 4 + 1], f);
                        putc(pImage[x * 4 + 2], f);
                    }
                }
                else if (d3dsd.Format == D3DFMT_A8R8G8B8)
                {
                    fwrite(pImage, d3dsd.Width, 4, f);
                }
                else if (d3dsd.Format == D3DFMT_R5G6B5)
                {
                    for (x = 0; x < d3dsd.Width; x++)
                    {
                        unsigned short c = ((unsigned short*)pImage)[x];

                        uint8_t b = (c & 0x1f) << 3;
                        putc(b, f);

                        uint8_t g = ((c >> 5) & 0x3f) << 2;
                        putc(g, f);

                        uint8_t r = ((c >> 11) & 0x1f) << 3;
                        putc(r, f);
                    }
                }
                else if (d3dsd.Format == D3DFMT_X1R5G5B5)
                {
                    for (x = 0; x < d3dsd.Width; x++)
                    {
                        unsigned short c = ((unsigned short*)pImage)[x];

                        uint8_t b = (c & 0x1f) << 3;
                        putc(b, f);

                        uint8_t g = ((c >> 5) & 0x1f) << 3;
                        putc(g, f);

                        uint8_t r = ((c >> 10) & 0x1f) << 3;
                        putc(r, f);
                    }
                }
                else if (d3dsd.Format == D3DFMT_A1R5G5B5)
                {
                    for (x = 0; x < d3dsd.Width; x++)
                    {
                        unsigned short c = ((unsigned short*)pImage)[x];

                        uint8_t b = (c & 0x1f) << 3;
                        putc(b, f);

                        uint8_t g = ((c >> 5) & 0x1f) << 3;
                        putc(g, f);

                        uint8_t r = ((c >> 10) & 0x1f) << 3;
                        putc(r, f);

                        uint8_t a = (c & 0x8000) ? 0xff : 0;
                        putc(a, f);
                    }
                }

                pImage += d3dLR.Pitch;
            }

            fclose(f);
        }

        pSurfaceCopy->UnlockRect();
    }

    pSurfaceCopy->Release();

    return hr;
}

// orig 0x63d070 screenshot.cpp:218
HRESULT SaveSurfaceToGrayscaleTGAFile(IDirect3DDevice9* pD3DDevice, char const* szFileName,
                                      IDirect3DSurface9* pSurface)
{
    HRESULT hr = S_OK;
    D3DSURFACE_DESC d3dsd;
    IDirect3DSurface9* pSurfaceCopy = NULL;
    D3DLOCKED_RECT d3dLR;
    char targaheader[18];
    uint8_t* pImage;
    unsigned long y;

    pSurface->GetDesc(&d3dsd);

    if (d3dsd.Pool != D3DPOOL_SYSTEMMEM)
    {
        hr = pD3DDevice->CreateOffscreenPlainSurface(d3dsd.Width, d3dsd.Height, d3dsd.Format, D3DPOOL_SCRATCH,
                                                     &pSurfaceCopy, NULL);
        if (SUCCEEDED(hr))
            hr = D3DXLoadSurfaceFromSurface(pSurfaceCopy, NULL, NULL, pSurface, NULL, NULL, D3DX_FILTER_POINT, 0);
    }
    else
        pSurfaceCopy = pSurface;

    if (SUCCEEDED(hr))
    {
        hr = pSurfaceCopy->LockRect(&d3dLR, NULL, D3DLOCK_READONLY | D3DLOCK_NOSYSLOCK | D3DLOCK_NO_DIRTY_UPDATE);
        if (SUCCEEDED(hr))
        {
            FILE* f = fopen(szFileName, "wb");
            if (f)
            {
                setvbuf(f, NULL, _IOFBF, 0x10000);

                memset(targaheader, 0, sizeof(targaheader));
                targaheader[2] = 3;
                targaheader[12] = (char)(d3dsd.Width & 0xff);
                targaheader[13] = (char)((d3dsd.Width >> 8) & 0xff);
                targaheader[14] = (char)(d3dsd.Height & 0xff);
                targaheader[15] = (char)((d3dsd.Height >> 8) & 0xff);
                targaheader[16] = 8;
                targaheader[17] = 0x20;

                fwrite(targaheader, 18, 1, f);

                pImage = (uint8_t*)d3dLR.pBits;
                for (y = 0; y < d3dsd.Height; y++)
                {
                    for (unsigned long x = 0; x < d3dsd.Width; x++)
                    {
                        if (d3dsd.Format == D3DFMT_X8R8G8B8 || d3dsd.Format == D3DFMT_A8R8G8B8)
                        {
                            putc((pImage[x * 4 + 0] + pImage[x * 4 + 1] + pImage[x * 4 + 2]) / 3, f);
                        }
                        else if (d3dsd.Format == D3DFMT_R5G6B5)
                        {
                            unsigned short c = ((unsigned short*)pImage)[x];

                            unsigned long r = ((c & 0xf800) | 0x0700) >> 8;
                            unsigned long g = ((c & 0x07e0) | 0x0018) >> 3;
                            unsigned long b = ((c & 0x001f) << 3) | 0x0007;

                            putc((r + g + b) / 3, f);
                        }
                        else if (d3dsd.Format == D3DFMT_X1R5G5B5)
                        {
                            unsigned short c = ((unsigned short*)pImage)[x];

                            unsigned long r = ((c & 0x7c00) | 0x0380) >> 7;
                            unsigned long g = ((c & 0x03e0) | 0x001c) >> 2;
                            unsigned long b = ((c & 0x001f) << 3) | 0x0007;

                            putc((r + g + b) / 3, f);
                        }
                        else
                        {
                            // The original leaves the surface locked here.
                            fclose(f);

                            if (d3dsd.Pool != D3DPOOL_SYSTEMMEM)
                                if (pSurfaceCopy)
                                    pSurfaceCopy->Release();

                            return 0;
                        }
                    }

                    pImage += d3dLR.Pitch;
                }

                fclose(f);
            }

            pSurfaceCopy->UnlockRect();
        }
    }

    if (d3dsd.Pool != D3DPOOL_SYSTEMMEM)
        if (pSurfaceCopy)
            pSurfaceCopy->Release();

    return hr;
}

// orig 0x63d320 screenshot.cpp:371
void CDevice::ScreenShot(const char* fileName, int width, int height)
{
    IDirect3DSurface9* pBackBuffer;
    if (FAILED(m_pd3dDevice->GetRenderTarget(0, &pBackBuffer)))
        return;

    ImageFileFormats format = screenshotTGA() ? M3DIFF_TGA : M3DIFF_JPG;

    CStr fName(fileName);
    if (fName.empty())
    {
        CStr fExt;

        switch (format)
        {
            case M3DIFF_TGA:
                fExt = ".tga";
                break;
            case M3DIFF_JPG:
                fExt = ".jpg";
                break;
            default:
                // original: assert(!"Unsupported screenshot file format"), active in the release build
                g_kernel->SysError(CStr(__FILE__ ":") + CStr(__LINE__), "!\"Unsupported screenshot file format\"");
                break;
        }

        unsigned int i = 0;
        do
        {
            fName.format("screen%05u", i++);
        }
        while (GetFileAttributesA((fName + fExt).c_str()) != INVALID_FILE_ATTRIBUTES);

        fName += fExt;
    }

    if (width == -1)
        width = g_kernel->GetEngineCfg().m_r_width.GetI();
    if (height == -1)
        height = g_kernel->GetEngineCfg().m_r_height.GetI();

    if (format == M3DIFF_TGA)
    {
        SaveSurfaceToTGAFile(m_pd3dDevice, fName.c_str(), pBackBuffer, width, height);
    }
    else
    {
        TexHandle tmp = AddTextureFromBackBuffer(width, height);
        SaveTextureToFile(tmp, fName.c_str(), format);
        ReleaseTexture(tmp);
    }

    pBackBuffer->Release();
}
