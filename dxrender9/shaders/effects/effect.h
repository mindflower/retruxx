#pragma once
// Ported from the original dxrender9/shaders/effects/effect.h: the ID3DXEffect wrapper handed out
// by CDevice::NewEffect.
#include <d3d9.h>
#include <d3dx9.h>

#include <vector>

#include <core/stringm3d.h>
#include <math/matrix.h>
#include <math/vector.h>
#include <math/vector4.h>
#include <renderer/i_renderer.h>

#include "i_renderer_orig.h"
#include "shaders/nshaderarg.h"

class CDevice;

class EffectImpl : public m3d::rend::IEffect
{
public:
    // Where a parameter's D3DX handle lives and what the SPACE annotation says about it.
    struct auxParamDesc
    {
        int space;
        bool isShared;
        D3DXHANDLE d3dxHandle;
    };

    // In the original IEffect::TechniqueDesc carries two more fields than HTA's (maxInstances and the user
    // render parameters, 0x44 bytes in all). The executable reads only the HTA part, so the original
    // description derives from it here.
    struct TechniqueDesc : m3d::rend::IEffect::TechniqueDesc
    {
        enum UserParamType
        {
            UPT_MATRIX = 0,
            UPT_VECTOR4 = 1,
            UPT_VECTOR = 2,
            UPT_FLOAT = 3,
        };

        struct UserParam
        {
            CStr name;
            UserParamType type;
        };

        int maxInstances;
        std::vector<UserParam> userParams;
    };

    struct TechniqueDescInternal
    {
        TechniqueDesc publicDesc;
        D3DXHANDLE handle;
    };

    bool m_bApplyGlobalParams;
    CStr m_fileName;
    std::vector<m3d::rend::CompileParam> m_compileParams;
    ID3DXEffect* m_effect;
    bool m_hasBeenValidated;
    bool m_didNotValidate;
    auxParamDesc m_parameterHandles[m3d::rend::IEffect::NumParameters];
    /* ...... */ nShaderParams m_curParams;
    /* ...... */ unsigned int m_numTechniques;
    /* ...... */ int m_curTechnique;
    /* ...... */ std::vector<TechniqueDescInternal> m_techDescs;
    /* ...... */ int m_defaultTechnique;
    /* ...... */ int m_defaultPS20Technique;
    /* ...... */ unsigned int m_numPrimitives;
    /* ...... */ unsigned int m_numDIPs;

    static CDevice* m_dev;

    EffectImpl(bool bApplyGlobalParams);
    virtual ~EffectImpl();

    bool LoadFromFile(const char* fileName, std::vector<m3d::rend::CompileParam> const& compileParams);
    bool LoadFromString(const char* str, unsigned int length);

    // IRenderResource
    bool IsValid() const override;

    // IEffect
    unsigned int GetNumTechniques() const override;
    m3d::rend::IEffect::TechniqueDesc const& GetTechniqueDesc(unsigned int i) const override;
    void SetCurTechnique(unsigned int i) override;
    void SetCurTechniqueByName(const char* name);
    unsigned int GetCurTechnique() const override;
    const char* GetCurTechniqueName() const;
    void SetDefaultTechnique(bool bPS20) override;
    bool IsParameterUsed(Parameter p) override;
    void SetInt(Parameter p, int val) override;
    void SetFloat(Parameter p, float val) override;
    void SetVector4(Parameter p, CVector4 const& val) override;
    void SetVector3(Parameter p, CVector const& val) override;
    void SetFloat4(Parameter p, nFloat4 const& val) override;
    void SetMatrix(Parameter p, CMatrix const& val) override;
    void SetTexture(Parameter p, m3d::rend::TexHandle* val) override;
    void SetIntArray(Parameter p, const int* vals, int count) override;
    void SetFloatArray(Parameter p, const float* vals, int count) override;
    void SetFloat4Array(Parameter p, const nFloat4* vals, int count) override;
    void SetVector4Array(Parameter p, const CVector4* vals, int count) override;
    void SetMatrixArray(Parameter p, const CMatrix* vals, int count) override;
    void SetMatrixPointerArray(Parameter p, const CMatrix** vals, int count) override;
    void SetParams(nShaderParams& params);

    int Begin();
    void BeginPass(unsigned int pass);
    void EndPass();
    void End();
    void CommitChanges();
    void ApplyGlobalFxParams();
    void OnDeviceReset();
    void OnDeviceRestore();
    void Invalidate();
    void ValidateEffect();
    void UpdateParameterHandles();
    bool getBoolAnnotation(D3DXHANDLE techHandle, const char* annotName, bool defaultValue);
    int getIntAnnotation(D3DXHANDLE techHandle, const char* annotName, int defaultValue);
    CStr getStringAnnotation(D3DXHANDLE techHandle, const char* annotName, CStr const& defaultValue);
    void getUserRenderParams(CStr const& userParamsStr, std::vector<TechniqueDesc::UserParam>& userParams);
};
