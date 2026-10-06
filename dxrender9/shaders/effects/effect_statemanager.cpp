// Ported from the original dxrender9/shaders/effects/effect_statemanager.cpp: the
// ID3DXEffectStateManager handed to every effect, routing the states D3DX sets through CDevice's
// state cache (setRenderState & co.) so the device's own bookkeeping stays in sync.
#include "shaders/effects/effect_statemanager.h"

#include "device.h"
#include "log.h"

// orig 0x650b40 effect_statemanager.cpp:29
StateManager::StateManager() : m_dev(0), m_nRefs(1)
{
}

// orig 0x650850 effect_statemanager.cpp:35
void StateManager::SetDevice(CDevice* owner)
{
    m_dev = owner;
}

// orig 0x650860 effect_statemanager.cpp:45
HRESULT StateManager::QueryInterface(REFIID iid, LPVOID* ppv)
{
    *ppv = NULL;
    return E_UNEXPECTED;
}

// orig 0x650880 effect_statemanager.cpp:53
ULONG StateManager::AddRef()
{
    return ++m_nRefs;
}

// orig 0x650890 effect_statemanager.cpp:60
ULONG StateManager::Release()
{
    return --m_nRefs;
}

// orig 0x6508a0 effect_statemanager.cpp:77
HRESULT StateManager::SetTransform(D3DTRANSFORMSTATETYPE State, CONST D3DMATRIX* pMatrix)
{
    return m_dev->SetXFormMatrix(State, *reinterpret_cast<CMatrix const*>(pMatrix));
}

// orig 0x6508c0 effect_statemanager.cpp:83
HRESULT StateManager::SetMaterial(CONST D3DMATERIAL9* pMaterial)
{
    return m_dev->GetDevice()->SetMaterial(pMaterial);
}

// orig 0x6508e0 effect_statemanager.cpp:89
HRESULT StateManager::SetLight(DWORD Index, CONST D3DLIGHT9* pLight)
{
    return m_dev->LightSet(Index, *pLight);
}

// orig 0x650900 effect_statemanager.cpp:95
HRESULT StateManager::LightEnable(DWORD Index, BOOL Enable)
{
    m_dev->LightEnable(Index, Enable);
    return S_OK;
}

// orig 0x650920 effect_statemanager.cpp:102
HRESULT StateManager::SetRenderState(D3DRENDERSTATETYPE State, DWORD Value)
{
    return m_dev->setRenderState(State, Value);
}

// orig 0x650940 effect_statemanager.cpp:108
HRESULT StateManager::SetTexture(DWORD Stage, LPDIRECT3DBASETEXTURE9 pTexture)
{
    return m_dev->setTexture(Stage, pTexture);
}

// orig 0x650960 effect_statemanager.cpp:114
HRESULT StateManager::SetTextureStageState(DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD Value)
{
    return m_dev->setTextureStageState(Stage, Type, Value);
}

// orig 0x650980 effect_statemanager.cpp:120
HRESULT StateManager::SetSamplerState(DWORD Sampler, D3DSAMPLERSTATETYPE Type, DWORD Value)
{
    return m_dev->setTextureSamplerState(Sampler, Type, Value);
}

// orig 0x6509a0 effect_statemanager.cpp:126
HRESULT StateManager::SetNPatchMode(FLOAT NumSegments)
{
    return S_OK;
}

// orig 0x6509b0 effect_statemanager.cpp:133
HRESULT StateManager::SetFVF(DWORD FVF)
{
    return m_dev->setFVF(FVF);
}

// orig 0x6509d0 effect_statemanager.cpp:139
HRESULT StateManager::SetVertexShader(LPDIRECT3DVERTEXSHADER9 pShader)
{
    return m_dev->setVertexShader(pShader);
}

// orig 0x6509f0 effect_statemanager.cpp:145
HRESULT StateManager::SetVertexShaderConstantF(UINT RegisterIndex, CONST FLOAT* pConstantData, UINT RegisterCount)
{
    m_dev->SetVsFloatConst(RegisterIndex, pConstantData, RegisterCount);
    return m_dev->GetLastResult();
}

// orig 0x650a20 effect_statemanager.cpp:152
HRESULT StateManager::SetVertexShaderConstantI(UINT RegisterIndex, CONST INT* pConstantData, UINT RegisterCount)
{
    m_dev->SetVsIntConst(RegisterIndex, pConstantData, RegisterCount);
    return m_dev->GetLastResult();
}

// orig 0x650a50 effect_statemanager.cpp:159
HRESULT StateManager::SetVertexShaderConstantB(UINT RegisterIndex, CONST BOOL* pConstantData, UINT RegisterCount)
{
    m_dev->SetVsBoolConst(RegisterIndex, pConstantData, RegisterCount);
    return m_dev->GetLastResult();
}

// orig 0x650a80 effect_statemanager.cpp:166
HRESULT StateManager::SetPixelShader(LPDIRECT3DPIXELSHADER9 pShader)
{
    return m_dev->setPixelShader(pShader);
}

// orig 0x650aa0 effect_statemanager.cpp:172
HRESULT StateManager::SetPixelShaderConstantF(UINT RegisterIndex, CONST FLOAT* pConstantData, UINT RegisterCount)
{
    m_dev->SetPsFloatConst(RegisterIndex, pConstantData, RegisterCount);
    return m_dev->GetLastResult();
}

// orig 0x650ad0 effect_statemanager.cpp:179
HRESULT StateManager::SetPixelShaderConstantI(UINT RegisterIndex, CONST INT* pConstantData, UINT RegisterCount)
{
    m_dev->SetPsIntConst(RegisterIndex, pConstantData, RegisterCount);
    return m_dev->GetLastResult();
}

// orig 0x650b00 effect_statemanager.cpp:186
HRESULT StateManager::SetPixelShaderConstantB(UINT RegisterIndex, CONST BOOL* pConstantData, UINT RegisterCount)
{
    m_dev->SetPsBoolConst(RegisterIndex, pConstantData, RegisterCount);
    return m_dev->GetLastResult();
}
