#pragma once
// Ported from the original dxrender9/shaders/shader_include.h: the ID3DXInclude that resolves
// #include in shader sources through the engine's file server.
#include <d3dx9.h>

class auxShaderInclude : public ID3DXInclude
{
public:
    HRESULT __stdcall Open(D3DXINCLUDE_TYPE IncludeType, LPCSTR pName, LPCVOID pParentData, LPCVOID* ppData,
                           UINT* pBytes) override;
    HRESULT __stdcall Close(LPCVOID pData) override;
};
