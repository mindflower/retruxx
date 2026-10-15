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
    // 26 planes of (normal, distance): the PDB dump prints the dimensions the other way round, but the binary
    // strides planes by 16 bytes.
    float m_planes[26][4];
    unsigned int m_indices[156];
    unsigned int m_enabled;
    unsigned int m_nfrustums;

    void createIndices();
    static void buildfrustum(float* fr, CVector const* v, CMatrix const& m, CVector const& org, float offset);
    static void buildfrustum(float* fr, CVector const& n, CMatrix const& m, CVector const& org, float offset);
    void createScreenFrustums(
        CVector const& origin,
        CMatrix const& rotMat,
        float fovx,
        float fovy,
        float znear,
        float zfar);
    void CreateScreenFrustums(float farz, float lessen, float nearz, float fov);
    void enableAll();
    int enableSetFromBox(float* minmaxs, CVector const& ofs);
    void enableUpdateFromBox(float* minmaxs, CVector const& ofs);
    unsigned int enableGetState() const;
    void enableSetState(unsigned int news);
    int testBBox(tbEnum test, float* minmaxs, CVector const& ofs) const;
    int testSphere(CVector const& o, float r) const;
    int testVertexInside(CVector const& v);
    int clipPolyInPlace(CVector* verts, int nverts);
    int clipPolyInPlaceZ(CVector* verts, int nverts);
    int clipLineZ(CVector* dst, CVector* src);
    void CreateFromWinding(int dir, CVector* wnd, int n, CVector const& campos, CPlane* nearplane);
    void translate(CVector const& add);
};
