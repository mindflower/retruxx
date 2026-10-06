// Ported from the original dxrender9/shaders/effects/effect_strings.cpp: the names of the effect
// parameters and of the compile parameters.
#include "device.h"

#include <string.h>

// The original's table has 49 entries in the order of its IEffect::Parameter (StringTable @ 0x852740). This
// one is in the order of HTA's 31-value IEffect::Parameter (lib/include/renderer/i_renderer.h);
// every entry is the original string of the enumerator with the same name, except LightPlant, which takes
// the string of the original's LightPlantAmbient (the enumerator at that position of the original list).
static const char* StringTable[IEffect::NumParameters] = {
    "WORLD_MATRIX",        // World
    "VIEW_MATRIX",         // View
    "PROJECTION_MATRIX",   // Projection
    "MODEL_VIEW_MATRIX",   // ModelView
    "INV_WORLD_MATRIX",    // InvWorld
    "TOTAL_MATRIX",        // ModelViewProjection
    "VIEW_POS",            // ViewPos
    "DIFFUSE_MAP_0",       // DiffMap0
    "CUBE_MAP_0",          // CubeMap0
    "BUMP_MAP_0",          // BumpMap0
    "DETAIL_MAP_0",        // DetailMap0
    "LIGHT_MAP_0",         // LightMap0
    "NORMAL_CUBE_MAP",     // NormalizationCubemap
    "TIME_LINEAR",         // Time_Linear
    "TREE_BEND_TERM",      // Tree_Bend_Term
    "LIGHT_AMBIENT",       // LightAmbient
    "LIGHT_DIFFUSE",       // LightDiffuse
    "LIGHT_PLANT",         // LightPlant (original: LightPlantAmbient)
    "LIGHT_SPECULAR",      // LightSpecular
    "FOG_TERM",            // FogTerm
    "TRANSPARENCY",        // Transparency
    "TRANS_START_DIST",    // TransStartDist
    "TRANS_OBJECT_WIDTH",  // TransObjectWidth
    "TMP_LIGHT0_DIR",      // TmpLight0Dir
    "USER_FLOAT_PARAM",    // User_float_param
    "USER_FLOAT_PARAM2",   // User_float_param2
    "USER_FLOAT_PARAM3",   // User_float_param3
    "USER_FLOAT3_PARAM",   // User_float3_param
    "USER_FLOAT3_PARAM2",  // User_float3_param2
    "USER_FLOAT4_PARAM",   // User_float4_param
    "USER_FLOAT4x4_PARAM", // User_float4x4_param
};

// CompileParamsNames @ 0x852804
static const char* CompileParamsNames[NUM_COMPILE_PARAMS] = {
    "COMPILE_INSTANCING_VERSION",
    "COMPILE_SUPPORT_SHADOWMAP",
};

// orig 0x645190 effect_strings.cpp:86
const char* CDevice::EffectParameterToString(IEffect::Parameter p)
{
    return StringTable[p];
}

// orig 0x6451a0 effect_strings.cpp:94
IEffect::Parameter CDevice::StringToEffectParameter(const char* str)
{
    for (int i = 0; i < IEffect::NumParameters; i++)
    {
        if (strcmp(str, StringTable[i]) == 0)
            return (IEffect::Parameter)i;
    }

    return IEffect::InvalidParameter;
}

// orig 0x645210 effect_strings.cpp:120
const char* CDevice::CompileParamToString(CompileParam p)
{
    return CompileParamsNames[p];
}

// orig 0x645220 effect_strings.cpp:127
CompileParam CDevice::StringToCompileParam(const char* str)
{
    for (int i = 0; i < NUM_COMPILE_PARAMS; i++)
    {
        if (strcmp(str, CompileParamsNames[i]) == 0)
            return (CompileParam)i;
    }

    return NUM_COMPILE_PARAMS;
}
