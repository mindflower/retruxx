#pragma once

class CVector;
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
            float x;
            float y;
            float z;
            float w;
            unsigned int c;
            float tu;
            float tv;

        public:
            void xyzw(float, float, float, float);
            void xyzw(CVector4 const&);
            void uv0(float, float);
        };

        struct VertexXYZC
        {
            float x;
            float y;
            float z;
            unsigned int c;

        public:
            void xyz(CVector const&);
        };

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
            float x;
            float y;
            float z;
            unsigned int c;
            float tu;
            float tv;

        public:
            void xyz(float, float, float);
            void xyz(CVector const&);
            void uv0(float, float);
        };

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
