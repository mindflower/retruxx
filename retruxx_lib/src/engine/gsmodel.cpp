#include <gsmodel.h>

namespace m3d
{
    int CGSModel::Load(const CStr& fname)
    {
        throw std::logic_error("Not implemented");
    }

    CGSModel::CGSModel()
    {
        this->m_triIndices = 0;
        this->m_verts = 0;
        this->m_drawIndices = 0;
        this->m_drawVerts = 0;
        this->m_numDrawIndices = 0;
        this->m_numDrawVerts = 0;
        this->m_col_header.numFaces = 0;
        this->m_col_header.numVertices = 0;
        this->m_col_verts = 0;
        this->m_col_idx = 0;
        this->m_loadPoints = 0;
        this->m_numLoadPoints = 0;
    }

    CGSModel::~CGSModel()
    {
        Release();
    }
    void CGSModel::Release()
    {
    }
}
