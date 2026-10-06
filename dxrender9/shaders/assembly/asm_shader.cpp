// Ported from the original dxrender9/shaders/assembly/asm_shader.cpp: a shader assembled from
// vs_x_x / ps_x_x assembly source with D3DXAssembleShader, and the CDevice side of its cache.
#include "shaders/assembly/asm_shader.h"

#include <d3dx9.h>

#include <core/kernel.h>
#include <core/scoped_ptr.h>
#include <file/fileserver.h>
#include <file/filestream.h>

#include "device.h"
#include "log.h"
#include "shaders/shader_include.h"

CDevice* AsmShaderImpl::m_dev = 0;

// orig 0x64c700 asm_shader.cpp:36
AsmShaderImpl::AsmShaderImpl()
{
    m_shader = 0;
}

// orig 0x64d380 asm_shader.cpp:42
AsmShaderImpl::~AsmShaderImpl()
{
    if (m_shader)
    {
        if (IsVertexShader())
            m_vs->Release();
        else
            m_ps->Release();
    }

    m_dev->OnAsmShaderDestructor(this);
}

// orig 0x64c550 asm_shader.cpp:58
bool AsmShaderImpl::IsVertexShader() const
{
    return m_type == VERTEX_SHADER;
}

// orig 0x64c820 asm_shader.cpp:65
bool AsmShaderImpl::LoadFromFile(const char* fileName, Type type)
{
    m_fileName = fileName;
    m_type = type;

    scoped_ptr<m3d::fs::FileStream> stream(g_kernel->GetFileServer().CreateFileStream());

    if (!stream->Open(fileName, m3d::fs::IStream::OPEN_READ))
    {
        LogMsg(CStr::format_("AsmShaderImpl: could not open shader file '%s'", fileName));
        return false;
    }

    unsigned int fileSize = stream->GetSize();

    char* buffer = new char[fileSize];

    stream->ReadBytes(buffer, fileSize);

    ID3DXBuffer* shaderCode;
    ID3DXBuffer* errorBuffer = 0;
    auxShaderInclude includeHandler;

    HRESULT hr = D3DXAssembleShader(buffer, fileSize, 0, &includeHandler, 0, &shaderCode, &errorBuffer);

    delete[] buffer;

    if (FAILED(hr))
    {
        LogMsg(CStr::format_("AsmShaderImpl: failed to compile shader file '%s' with: %s", fileName,
                             errorBuffer ? (const char*)errorBuffer->GetBufferPointer() : "No D3DX error message."));

        if (errorBuffer)
        {
            errorBuffer->Release();
            errorBuffer = 0;
        }

        return false;
    }

    if (IsVertexShader())
    {
        m_dev->m_pd3dDevice->CreateVertexShader((DWORD*)shaderCode->GetBufferPointer(), &m_vs);
    }
    else
    {
        m_dev->m_pd3dDevice->CreatePixelShader((DWORD*)shaderCode->GetBufferPointer(), &m_ps);
    }

    return true;
}

// orig 0x64c560 asm_shader.cpp:143
bool AsmShaderImpl::LoadFromString(const char* shaderBuf, unsigned int dataLen, Type type)
{
    return false;
}

// orig 0x64c570 asm_shader.cpp:150
bool AsmShaderImpl::IsValid() const
{
    return false;
}

// orig 0x64c580 asm_shader.cpp:309
void AsmShaderImpl::Invalidate()
{
    if (m_shader)
    {
        if (IsVertexShader())
            m_vs->Release();
        else
            m_ps->Release();
    }

    m_shader = 0;
}

// orig 0x64c5b0 asm_shader.cpp:324
void AsmShaderImpl::OnDeviceReset()
{
}

// orig 0x64c5c0 asm_shader.cpp:330
void AsmShaderImpl::OnDeviceRestore()
{
}

// orig 0x64c5d0 asm_shader.cpp:336
void AsmShaderImpl::Apply()
{
    if (IsVertexShader())
        m_dev->setVertexShader(m_vs);
    else
        m_dev->setPixelShader(m_ps);
}

// orig 0x64d1e0 asm_shader.cpp:348
IAsmShader* CDevice::NewAsmShader(const char* fileName, IAsmShader::Type type)
{
    CStr fName(fileName);
    UnifyFileName(fName);
    std::map<CStr, AsmShaderImpl*>::iterator it = m_AsmShaders.find(fName);
    if (it != m_AsmShaders.end())
    {
        AsmShaderImpl* shader = it->second;

        shader->AddRef();
        return shader;
    }

    AsmShaderImpl* shader = new AsmShaderImpl();
    if (!shader->LoadFromFile(fileName, type))
    {
        delete shader;
        return 0;
    }

    shader->AddRef();
    m_AsmShaders[fName] = shader;
    return shader;
}

// orig 0x64d310 asm_shader.cpp:378
void CDevice::OnAsmShaderDestructor(AsmShaderImpl* shader)
{
    CStr fName(shader->m_fileName);
    UnifyFileName(fName);
    m_AsmShaders.erase(fName);
}

// orig 0x64cc50 asm_shader.cpp:387
void CDevice::ReleaseAsmShaders()
{
    for (std::map<CStr, AsmShaderImpl*>::iterator it = m_AsmShaders.begin(); it != m_AsmShaders.end(); ++it)
    {
        AsmShaderImpl* shader = it->second;

        LogMsg(CStr::format_("Warning: Asm shader '%s' is not released (refs = %d)", shader->m_fileName.c_str(),
                             shader->GetRefCount()));
    }
}
