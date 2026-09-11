#pragma once

struct CVector;
class CVector4;

namespace m3d
{
    namespace rend
    {
        enum VertexType
        {
            VERTEX_XYZ = 0x0,
            VERTEX_XYZT1 = 0x1,
            VERTEX_XYZC = 0x2,
            VERTEX_XYZWC = 0x3,
            VERTEX_XYZWCT1 = 0x4,
            VERTEX_XYZNC = 0x5,
            VERTEX_XYZCT1 = 0x6,
            VERTEX_XYZNT1 = 0x7,
            VERTEX_XYZNCT1 = 0x8,
            VERTEX_XYZNCT2 = 0x9,
            VERTEX_XYZNT2 = 0xA,
            VERTEX_XYZNT3 = 0xB,
            VERTEX_XYZCT1_UVW = 0xC,
            VERTEX_XYZCT2_UVW = 0xD,
            VERTEX_XYZCT2 = 0xE,
            VERTEX_XYZNT1T = 0xF,
            VERTEX_XYZNCT1T = 0x10,
            VERTEX_XYZNCT1_UV2_S1 = 0x11,
            VERTEX_STREAM_UV_S1 = 0x12,
            VERTEX_WATERTEST = 0x13,
            VERTEX_GRASSTEST = 0x14,
            VERTEX_IMPOSTORTEST = 0x15,
            VERTEX_YNI = 0x16,
            VERTEX_XYZT1I = 0x17,
        };

        struct VertexXYZWC
        {
            float x;
            float y;
            float z;
            float w;
            unsigned int c;

        public:
            void xyzw(float,float,float,float);
        };

        struct VertexXYZNT1
        {
            float x;
            float y;
            float z;
            float nx;
            float ny;
            float nz;
            float tu;
            float tv;

        public:
            void xyz(CVector const&);
            void uv0(float, float);
            void n(CVector const&);
        };

        struct VertexXYZ
        {
            float x;
            float y;
            float z;

        public:
            void xyz(CVector const&);
        };

        struct VertexXYZWCT1
        {
            void xyzw(const CVector4& v);
            void xyzw(float xx, float yy, float zz, float ww);
            void uv0(float u, float v);
            /* 0x0000 */ float x;
            /* 0x0004 */ float y;
            /* 0x0008 */ float z;
            /* 0x000c */ float w;
            /* 0x0010 */ unsigned int c;
            /* 0x0014 */ float tu;
            /* 0x0018 */ float tv;
        }; /* size: 0x001c */

        static_assert(sizeof(VertexXYZWCT1) == 0x001c);

        struct VertexXYZCT2
        {
            void xyz(const CVector&);
            void xyz(float, float, float);
            void uv0(float, float);
            void uv1(float, float);
            /* 0x0000 */ float x;
            /* 0x0004 */ float y;
            /* 0x0008 */ float z;
            /* 0x000c */ unsigned int c;
            /* 0x0010 */ float tu0;
            /* 0x0014 */ float tv0;
            /* 0x0018 */ float tu1;
            /* 0x001c */ float tv1;
        }; /* size: 0x0020 */

        static_assert(sizeof(VertexXYZCT2) == 0x0020);

        struct VertexXYZC
        {
            float x;
            float y;
            float z;
            unsigned int c;

        public:
            void xyz(CVector const&);
        };

        struct VertexXYZNC
        {
            void xyz(const CVector& v);
            void xyz(float xx, float yy, float zz);
            void n(float nnx, float nny, float nnz);
            void n(const CVector& nn);
            /* 0x0000 */ float x;
            /* 0x0004 */ float y;
            /* 0x0008 */ float z;
            /* 0x000c */ float nx;
            /* 0x0010 */ float ny;
            /* 0x0014 */ float nz;
            /* 0x0018 */ unsigned int c;
        }; /* size: 0x001c */

        static_assert(sizeof(VertexXYZNC) == 0x001c);

        struct VertexXYZT1I
        {
            float x;
            float y;
            float z;
            float tu;
            float tv;
            __int16 i1;
            __int16 i2;

        public:
            void uv0(float, float);
            void xyz(CVector const&);
        };

        struct VertexXYZCT1
        {
            void xyz(const CVector& v);
            void xyz(float xx, float yy, float zz);
            void uv0(float u, float v);
            /* 0x0000 */ float x;
            /* 0x0004 */ float y;
            /* 0x0008 */ float z;
            /* 0x000c */ unsigned int c;
            /* 0x0010 */ float tu;
            /* 0x0014 */ float tv;
        }; /* size: 0x0018 */

        static_assert(sizeof(VertexXYZCT1) == 0x0018);

        struct VertexXYZNCT2
        {
            float x;
            float y;
            float z;
            float nx;
            float ny;
            float nz;
            unsigned int c;
            float tu0;
            float tv0;
            float tu1;
            float tv1;
        };

        struct VertexLandscape
        {
            float y;
            __int16 xz;
            __int16 uv;
        };
    }
}
