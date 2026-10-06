#pragma once
// The parts of the original renderer interface (engine/renderer/i_renderer.h)
// that Hard Truck Apocalypse's i_renderer.h does not have. The driver is built from the original
// sources, so the original-only types are declared here; the methods that take them are plain member
// functions of CDevice and not part of the exported vtable.
#include <renderer/i_renderer.h>

namespace m3d
{
    namespace rend
    {
        enum CompileParam : int32_t
        {
            COMPILE_INSTANCING_VERSION = 0,
            COMPILE_SUPPORT_SHADOWMAP = 1,
            NUM_COMPILE_PARAMS = 2,
        };

        enum StreamDataType : int32_t
        {
            M3DSDT_DEFAULT_DATA = 0,
            M3DSDT_INDEXED_DATA = 1,
            M3DSDT_INSTANCED_DATA = 2,
        };

        enum ImageFileFormats : int32_t
        {
            M3DIFF_BMP = 0,
            M3DIFF_JPG = 1,
            M3DIFF_TGA = 2,
            M3DIFF_PNG = 3,
            M3DIFF_DDS = 4,
            M3DIFF_PPM = 5,
            M3DIFF_DIB = 6,
            M3DIFF_HDR = 7,
            M3DIFF_PFM = 8,
        };

        // The original DeviceFeature values past HTA's FEATURE_NUM_FEATURES (25). m_featureSupported is sized
        // for all 27 as in the original; the executable never asks for the last two.
        enum
        {
            FEATURE_HARDWARE_INSTANCING = 25,
            FEATURE_DEPTH_TEXTURES = 26,
            FEATURE_NUM_FEATURES_15 = 27,
        };

        // The original TexDynFormat values past HTA's TM_DTF_SHADOW_TARGET.
        enum
        {
            TM_DTF_COMPRESSED_RGBA_FORMAT = 7,
            TM_DTF_COMPRESSED_RGB_FORMAT = 8,
            TM_DTF_DEPTH_TARGET = 9,
        };

        class MeshHandle : public Handle<MeshHandle>
        {
        };

        static_assert(sizeof(MeshHandle) == 0x0004);

#pragma pack(push, 4)
        struct SubmeshInfo
        {
            /* 0x0000 */ int m_firstVert;
            /* 0x0004 */ int m_numVerts;
            /* 0x0008 */ int m_firstTri;
            /* 0x000c */ int m_numTris;
        }; /* size: 0x0010 */
#pragma pack(pop)

        static_assert(sizeof(SubmeshInfo) == 0x0010);
    }  // namespace rend
}  // namespace m3d
