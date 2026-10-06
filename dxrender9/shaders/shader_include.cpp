// Ported from the original dxrender9/shaders/shader_include.cpp: the ID3DXInclude handler that
// resolves the #include directives of shader sources through the engine's file server.
#include "shaders/shader_include.h"

#include <core/kernel.h>
#include <core/scoped_ptr.h>
#include <core/stringm3d.h>
#include <file/fileserver.h>
#include <file/filestream.h>

#include "log.h"

// orig 0x650b80 shader_include.cpp:29
HRESULT __stdcall auxShaderInclude::Open(D3DXINCLUDE_TYPE IncludeType, LPCSTR pName, LPCVOID pParentData,
                                         LPCVOID* ppData, UINT* pBytes)
{
    CStr fileName(pName);

    if (!g_kernel->GetFileServer().FileExists(fileName.c_str()))
    {
        fileName = CStr("data/shaders/") + fileName;
    }

    scoped_ptr<m3d::fs::FileStream> stream(g_kernel->GetFileServer().CreateFileStream());

    if (!stream->Open(fileName.c_str(), m3d::fs::IStream::OPEN_READ))
    {
        LogMsg(CStr::format_("auxShaderInclude: could not open include file '%s'", fileName.c_str()));
        return E_FAIL;
    }

    unsigned int fileSize = stream->GetSize();

    char* buffer = new char[fileSize];

    stream->ReadBytes(buffer, fileSize);

    *ppData = buffer;
    *pBytes = fileSize;

    return S_OK;
}

// orig 0x650b60 shader_include.cpp:65
HRESULT __stdcall auxShaderInclude::Close(LPCVOID pData)
{
    delete[] static_cast<char*>(const_cast<void*>(pData));
    return S_OK;
}
