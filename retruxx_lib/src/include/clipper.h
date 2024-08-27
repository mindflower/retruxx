#pragma once

class CMatrix;
class CPlane;
class CVector;

struct CClipper
{
    float m_planes[26][4];
    unsigned int m_indices[156];
    unsigned int m_enabled;
    unsigned int m_nfrustums;

public:
    int testVertexInside(CVector const &);
    void CreateFromWinding(int,CVector *,int,CVector const &,CPlane *);
    int clipPolyInPlace(CVector *,int);
    int testBBox(enum tbEnum,float *,CVector const &) const ;
    static void __fastcall buildfrustum(float *,CVector const &,CMatrix const &,CVector const &,float);
    static void __fastcall buildfrustum(float *,CVector const * const,CMatrix const &,CVector const &,float);
    unsigned int enableGetState() const ;
    void enableSetState(unsigned int);
    int testSphere(CVector const &,float) const ;
    int enableSetFromBox(float *,CVector const &);
    void createIndices();
    int clipLineZ(CVector *,CVector *);
    int clipPolyInPlaceZ(CVector *,int);
    void createScreenFrustums(CVector const &,CMatrix const &,float,float,float,float);
    void CreateScreenFrustums(float,float,float,float);
    void translate(CVector const &);
    void enableAll();
    void enableUpdateFromBox(float *,CVector const &);
};
