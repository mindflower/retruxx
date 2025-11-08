#pragma once

struct CMatrix;
class CPlane;
struct CVector;

enum tbEnum
{
    tbFullTest = 0,
    tbRejectOnly = 1,
};

class CClipper
{
public:
    /* 0x0000 */ float m_planes[4][26];
    /* 0x01a0 */ unsigned int m_indices[156];
    /* 0x0410 */ unsigned int m_enabled;
    /* 0x0414 */ unsigned int m_nfrustums;

    void createIndices();
    static void __fastcall buildfrustum(float* fr, const CVector* v, const CMatrix& m, const CVector& org, float offset);
    static void __fastcall buildfrustum(float* fr, const CVector& n, const CMatrix& m, const CVector& org, float offset);
    void createScreenFrustums(const CVector& origin, const CMatrix& rotMat, float fovx, float fovy, float znear, float zfar);
    void CreateScreenFrustums(float farz, float lessen, float nearz, float fov);
    void enableAll();
    int enableSetFromBox(float* minmaxs, const CVector& ofs);
    void enableUpdateFromBox(float* minmaxs, const CVector& ofs);
    unsigned int enableGetState() const;
    void enableSetState(unsigned int news);
    int testBBox(tbEnum test, float* minmaxs, const CVector& ofs) const;
    int testSphere(const CVector& o, float r) const;
    int testVertexInside(const CVector& v);
    int clipPolyInPlace(CVector* verts, int nverts);
    int clipPolyInPlaceZ(CVector* verts, int nverts);
    int clipLineZ(CVector* dst, CVector* src);
    void CreateFromWinding(int dir, CVector* wnd, int n, const CVector& campos, CPlane* nearplane);
    void translate(const CVector& add);
}; /* size: 0x0418 */
