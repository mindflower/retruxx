#pragma once
// Ported from the original dxrender9/shaders/effects/effect_statemanager.h: the
// ID3DXEffectStateManager that routes the states an effect sets through CDevice's state cache.
#include <d3dx9.h>

class CDevice;

class StateManager : public ID3DXEffectStateManager
{
public:
    /* 0x0004 */ CDevice* m_dev;
    /* 0x0008 */ unsigned int m_nRefs;

    StateManager();
    void SetDevice(CDevice* dev);

    HRESULT __stdcall QueryInterface(REFIID iid, LPVOID* ppv) override;
    ULONG __stdcall AddRef() override;
    ULONG __stdcall Release() override;
    HRESULT __stdcall SetTransform(D3DTRANSFORMSTATETYPE State, CONST D3DMATRIX* pMatrix) override;
    HRESULT __stdcall SetMaterial(CONST D3DMATERIAL9* pMaterial) override;
    HRESULT __stdcall SetLight(DWORD Index, CONST D3DLIGHT9* pLight) override;
    HRESULT __stdcall LightEnable(DWORD Index, BOOL Enable) override;
    HRESULT __stdcall SetRenderState(D3DRENDERSTATETYPE State, DWORD Value) override;
    HRESULT __stdcall SetTexture(DWORD Stage, LPDIRECT3DBASETEXTURE9 pTexture) override;
    HRESULT __stdcall SetTextureStageState(DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD Value) override;
    HRESULT __stdcall SetSamplerState(DWORD Sampler, D3DSAMPLERSTATETYPE Type, DWORD Value) override;
    HRESULT __stdcall SetNPatchMode(FLOAT NumSegments) override;
    HRESULT __stdcall SetFVF(DWORD FVF) override;
    HRESULT __stdcall SetVertexShader(LPDIRECT3DVERTEXSHADER9 pShader) override;
    HRESULT __stdcall SetVertexShaderConstantF(UINT RegisterIndex, CONST FLOAT* pConstantData,
                                               UINT RegisterCount) override;
    HRESULT __stdcall SetVertexShaderConstantI(UINT RegisterIndex, CONST INT* pConstantData,
                                               UINT RegisterCount) override;
    HRESULT __stdcall SetVertexShaderConstantB(UINT RegisterIndex, CONST BOOL* pConstantData,
                                               UINT RegisterCount) override;
    HRESULT __stdcall SetPixelShader(LPDIRECT3DPIXELSHADER9 pShader) override;
    HRESULT __stdcall SetPixelShaderConstantF(UINT RegisterIndex, CONST FLOAT* pConstantData,
                                              UINT RegisterCount) override;
    HRESULT __stdcall SetPixelShaderConstantI(UINT RegisterIndex, CONST INT* pConstantData,
                                              UINT RegisterCount) override;
    HRESULT __stdcall SetPixelShaderConstantB(UINT RegisterIndex, CONST BOOL* pConstantData,
                                              UINT RegisterCount) override;
}; /* size: 0x000c */
