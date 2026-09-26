#include <cmath>
#include <cstring>
#include <draftstructures.h>
#include <core/kernel.h>
#include <core/log.h>
#include <math/vector4.h>

namespace m3d
{
    DRAFT_VertexComponent::DRAFT_VertexComponent()
    {
        // RVA 0x77BA20
    }

    DRAFT_VertexComponent::DRAFT_VertexComponent(VERTEX_COMPONENT t, int s) :
        Type(t),
        Size(s)
    {
        // RVA 0x77BA30
    }

    bool DRAFT_VertexComponent::operator==(DRAFT_VertexComponent const& A)
    {
        // RVA 0x77BA50 - components match by type alone.
        return Type == A.Type;
    }
}  // namespace m3d

bool operator==(m3d::DRAFT_Transform const& A, m3d::DRAFT_Transform const& B)
{
    // RVA 0x77C1F0 - equal to within 1e-4 in every component.
    return fabs(static_cast<double>(A.Tx) - B.Tx) < 0.0001 && fabs(static_cast<double>(A.Ty) - B.Ty) < 0.0001 &&
        fabs(static_cast<double>(A.Tz) - B.Tz) < 0.0001 && fabs(static_cast<double>(A.Rx) - B.Rx) < 0.0001 &&
        fabs(static_cast<double>(A.Ry) - B.Ry) < 0.0001 && fabs(static_cast<double>(A.Rz) - B.Rz) < 0.0001 &&
        fabs(static_cast<double>(A.Rw) - B.Rw) < 0.0001 && fabs(static_cast<double>(A.Sx) - B.Sx) < 0.0001 &&
        fabs(static_cast<double>(A.Sy) - B.Sy) < 0.0001 && fabs(static_cast<double>(A.Sz) - B.Sz) < 0.0001;
}

bool VertCompPresent(retruxx::vector<m3d::DRAFT_VertexComponent>& VC, m3d::VERTEX_COMPONENT c)
{
    // RVA 0x77E7C0
    for (m3d::DRAFT_VertexComponent const& comp : VC)
    {
        if (comp.Type == c)
        {
            return true;
        }
    }
    return false;
}

unsigned int CompOff(retruxx::vector<m3d::DRAFT_VertexComponent>& VC, m3d::VERTEX_COMPONENT Cmp)
{
    // RVA 0x77EB00 - the byte offset of a component in the interleaved vertex: position, normal, colour, then
    // the texture coordinate sets and the tangent.
    switch (Cmp)
    {
    case m3d::NORMAL:
        return 12;
    case m3d::COLOR:
        return 24;
    case m3d::TEXTURE1:
        return VertCompPresent(VC, m3d::COLOR) ? 28 : 24;
    case m3d::TEXTURE2:
        return CompOff(VC, m3d::TEXTURE1) + 8;
    case m3d::TEXTURE3:
        return CompOff(VC, m3d::TEXTURE2) + 8;
    case m3d::TANGENT:
        if (VertCompPresent(VC, m3d::TEXTURE3))
        {
            return CompOff(VC, m3d::TEXTURE3) + 8;
        }
        if (VertCompPresent(VC, m3d::TEXTURE2))
        {
            return CompOff(VC, m3d::TEXTURE2) + 8;
        }
        return CompOff(VC, m3d::TEXTURE1) + 8;
    default:
        return 0;
    }
}

bool DefineVertexType(
    retruxx::vector<m3d::DRAFT_VertexComponent>& VC, m3d::rend::VertexType& VertType, unsigned int& VertTypeSize)
{
    // RVA 0x77E7F0 - picks the vertex format for a set of components. Position, normal and a first texture set are
    // required; a tangent is only supported with a single texture set.
    if (!VertCompPresent(VC, m3d::COORDINATE) || !VertCompPresent(VC, m3d::NORMAL) ||
        !VertCompPresent(VC, m3d::TEXTURE1))
    {
        M3D_LOG_ERR(CStr("Unsupported type of vertex"));
        return false;
    }
    int texSets = 1;
    if (VertCompPresent(VC, m3d::TEXTURE3))
    {
        texSets = 3;
    }
    else if (VertCompPresent(VC, m3d::TEXTURE2))
    {
        texSets = 2;
    }

    if (VertCompPresent(VC, m3d::COLOR))
    {
        if (VertCompPresent(VC, m3d::TANGENT))
        {
            if (texSets == 1)
            {
                VertType = m3d::rend::VERTEX_XYZNCT1T;
                VertTypeSize = 52;
                return true;
            }
            M3D_LOG_ERR(CStr("Unsupported type of vertex"));
            return false;
        }
        if (texSets == 1)
        {
            VertType = m3d::rend::VERTEX_XYZNCT1;
            VertTypeSize = 36;
            return true;
        }
        if (texSets == 2)
        {
            VertType = m3d::rend::VERTEX_XYZNCT2;
            VertTypeSize = 44;
            return true;
        }
        M3D_LOG_ERR(CStr("Unsupported type of vertex"));
        return false;
    }

    if (VertCompPresent(VC, m3d::TANGENT))
    {
        if (texSets != 1)
        {
            M3D_LOG_ERR(CStr("Unsupported type of vertex"));
            return false;
        }
        VertType = m3d::rend::VERTEX_XYZNT1T;
        VertTypeSize = 48;
        return true;
    }
    if (texSets == 1)
    {
        VertType = m3d::rend::VERTEX_XYZNT1;
        VertTypeSize = 32;
    }
    else if (texSets == 2)
    {
        VertType = m3d::rend::VERTEX_XYZNT2;
        VertTypeSize = 40;
    }
    else
    {
        VertType = m3d::rend::VERTEX_XYZNT3;
        VertTypeSize = 48;
    }
    return true;
}

void BinormalToTangentW(m3d::DMesh& Mh)
{
    // RVA 0x782960 - folds a binormal component into the tangent's w: the tangent becomes 4 floats, w being the
    // handedness (+1 when the binormal agrees with normal x tangent, else -1), and the binormal is dropped.
    auto& headers = Mh.VertexComponentHeaders;
    auto const find = [&headers](m3d::VERTEX_COMPONENT type) {
        auto it = headers.begin();
        while (it != headers.end() && it->Type != type)
        {
            ++it;
        }
        return it;
    };
    auto const binormalIt = find(m3d::BINORMAL);
    auto const tangentIt = find(m3d::TANGENT);
    auto const normalIt = find(m3d::NORMAL);
    if (binormalIt == headers.end())
    {
        return;
    }
    if (tangentIt == headers.end())
    {
        M3D_LOG_INFO(CStr("Unknown vertex format "));
        return;
    }

    size_t const tangentIdx = tangentIt - headers.begin();
    size_t const binormalIdx = binormalIt - headers.begin();
    size_t const normalIdx = normalIt - headers.begin();
    unsigned int const count = Mh.Header.VertexCount;
    float* const realTangents = reinterpret_cast<float*>(new unsigned char[sizeof(CVector4) * count]);
    float const* const tangents = static_cast<float const*>(Mh.VerticesComponents[tangentIdx]);
    float const* const normals = static_cast<float const*>(Mh.VerticesComponents[normalIdx]);
    float const* const binormals = static_cast<float const*>(Mh.VerticesComponents[binormalIdx]);
    for (unsigned int i = 0; i < count; ++i)
    {
        float const* const t = &tangents[3 * i];
        float const* const n = &normals[3 * i];
        float const* const b = &binormals[3 * i];
        float* const dst = &realTangents[4 * i];
        dst[0] = t[0];
        dst[1] = t[1];
        dst[2] = t[2];
        float const handedness = b[2] * (t[1] * n[0] - n[1] * t[0]) + b[1] * (n[2] * t[0] - t[2] * n[0]) +
            (n[1] * t[2] - n[2] * t[1]) * b[0];
        dst[3] = handedness <= 0.0f ? -1.0f : 1.0f;
    }

    headers[tangentIdx].Size = 16;
    delete[] static_cast<unsigned char*>(Mh.VerticesComponents[tangentIdx]);
    Mh.VerticesComponents[tangentIdx] = realTangents;
    delete[] static_cast<unsigned char*>(Mh.VerticesComponents[binormalIdx]);
    Mh.VerticesComponents.erase(Mh.VerticesComponents.begin() + binormalIdx);
    headers.erase(headers.begin() + binormalIdx);
    --Mh.Header.NumberOfVertexComponents;
}

bool IsStaticTriMesh(m3d::DMesh const& TriMesh, retruxx::vector<m3d::DAnimation> const& Animations,
    retruxx::vector<m3d::DRAFT_Bone> const& Bones)
{
    // RVA 0x783BF0 - a rigid mesh whose bone chain holds the same transforms in every frame of every animation as
    // in the first frame of the first one. Skinned meshes never count.
    if (TriMesh.Header.Type == m3d::SKINED_MESH)
    {
        return false;
    }
    retruxx::vector<m3d::DRAFT_Transform> Transforms;
    for (int bone = TriMesh.Header.ParentBone; bone >= 0; bone = Bones[bone].ParentIndex)
    {
        Transforms.push_back(Animations[0].AnimationKeys[0][bone]);
    }
    for (m3d::DAnimation const& anim : Animations)
    {
        for (auto const& frame : anim.AnimationKeys)
        {
            m3d::DRAFT_Transform const* first = Transforms.data();
            for (int bone = TriMesh.Header.ParentBone; bone >= 0; bone = Bones[bone].ParentIndex)
            {
                if (!(*first == frame[bone]))
                {
                    return false;
                }
                ++first;
            }
        }
    }
    return true;
}
