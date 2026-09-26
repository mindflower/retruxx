#include <algorithm>
#include <skelmodel.h>
#include <core/kernel.h>
#include <core/log.h>
#include "core/ini.h"
#include <core/scoped_ptr.h>
#include <file/fileserver.h>
#include <file/filestream.h>
#include <file/tagged.h>

#include "game/m3dgame.h"
#include <config.h>

#include "core/timer.h"
#include "math/aabb.h"
#include "math/coremath.h"

namespace m3d
{
    AnimAction actions[] = {
        {"STAND1", AT_STAND1},
        {"STAND2", AT_STAND2},
        {"MOVE1", AT_MOVE1},
        {"MOVE2", AT_MOVE2},
        {"ATTACK1", AT_ATTACK1},
        {"ATTACK2", AT_ATTACK2},
        {"PAIN1", AT_PAIN1},
        {"PAIN2", AT_PAIN2},
        {"DEATH1", AT_DEATH1},
        {"DEATH2", AT_DEATH2},
        {"BLOCK1", AT_BLOCK1},
        {"BLOCK2", AT_BLOCK2},
        {"RESERVED1", AT_RESERVED1},
        {"RESERVED2", AT_RESERVED2},
        {"RESERVED3", AT_RESERVED3},
        {"RESERVED4", AT_RESERVED4},
        {"SEL1RUS", AT_SND_SELECT},
        {"SEL2RUS", AT_SND_SELECT2},
        {"SEL3RUS", AT_SND_SELECT3},
        {"SEL4RUS", AT_SND_SELECT4},
        {"AT1RUS", AT_SND_ATTACK},
        {"AT2RUS", AT_SND_ATTACK2},
        {"AT3RUS", AT_SND_ATTACK3},
        {"AT4RUS", AT_SND_ATTACK4},
        {"WALK1RUS", AT_SND_WALK},
        {"WALK2RUS", AT_SND_WALK2},
        {"WALK3RUS", AT_SND_WALK3},
        {"WALK4RUS", AT_SND_WALK4},
        {"MOVE1A", AT_SND_MOVE1A},
        {"MOVE2A", AT_SND_MOVE2A},
        {"UNREACH1RUS", AT_SND_UNREACH},
        {"ALLFRAMES", AT_ALL_FRAMES},
    };

    AnimatedModel::Bone::Bone(Bone const& other)
    {
        // RVA 0x70D190: a straight memcpy (Bone is trivially copyable).
        memcpy(this, &other, sizeof(Bone));
    }

    AnimatedModel::Bone::Bone()
    {
    }

    AnimatedModel::Animation::~Animation()
    {
        // RVA 0x70B570: frees the two per-animation arrays (new[]'d in LoadGAM).
        delete[] m_hierChanges;
        m_hierChanges = nullptr;
        delete[] m_nodesPositions;
        m_nodesPositions = nullptr;
    }

    AnimatedModel::Animation::Animation()
    {
        this->m_fps = 30;
        this->m_numNodes = 0;
        this->m_numFrames = 0;
        this->m_numChanges = 0;
        this->m_nextAnimation = -1;
        this->m_action = AT_STAND1;
        this->m_hierChanges = 0;
        this->m_nodesPositions = 0;
    }

    void AnimatedModel::Mesh::ComputeShadowsRelatedStuff()
    {
        // RVA 0x70D310 - the positions and triangles the shadow volumes are built from, and the face normals. NOTE:
        // despite the names nothing is welded: the vertices and triangles are copied as they are.
        m_numVertsWelded = m_numVertices;
        m_vertsWelded = new CVector[m_numVertices];
        unsigned char const* vert = static_cast<unsigned char const*>(m_verts);
        for (int i = 0; i < m_numVertices; ++i)
        {
            float const* const pos = reinterpret_cast<float const*>(vert);
            vert += m_VertexTypeSize;
            m_vertsWelded[i].x = pos[0];
            m_vertsWelded[i].y = pos[1];
            m_vertsWelded[i].z = pos[2];
        }

        m_trisWelded = new unsigned short[3 * m_numFaces];
        memcpy(m_trisWelded, m_tris, 6 * m_numFaces);
        m_numFacesWelded = m_numFaces;
        m_faceNormals = new CVector[m_numFaces];
        for (int i = 0; i < m_numFacesWelded; ++i)
        {
            unsigned short const* const tri = &m_trisWelded[3 * i];
            CVector const& v0 = m_vertsWelded[tri[0]];
            CVector const& v1 = m_vertsWelded[tri[1]];
            CVector const& v2 = m_vertsWelded[tri[2]];
            float const e1x = v1.x - v0.x;
            float const e1y = v1.y - v0.y;
            float const e1z = v1.z - v0.z;
            float const e2x = v2.x - v1.x;
            float const e2y = v2.y - v1.y;
            float const e2z = v2.z - v1.z;
            float const nz = e1y * e2x - e1x * e2y;
            float const ny = e2z * e1x - e1z * e2x;
            float const nx = e1z * e2y - e1y * e2z;
            // The length is summed on the x87 stack; FLT_EPSILON keeps a degenerate face from dividing by zero.
            float const inv = static_cast<float>(
                1.0 / sqrt(static_cast<double>(nz) * nz + static_cast<double>(ny) * ny + static_cast<double>(nx) * nx +
                    1.1920929e-7f));
            m_faceNormals[i].x = inv * nx;
            m_faceNormals[i].y = inv * ny;
            m_faceNormals[i].z = inv * nz;
        }
    }

    AnimatedModel::Mesh::~Mesh()
    {
        if (m_drawVerts != m_verts)
        {
            delete[] static_cast<uint8_t*>(m_drawVerts);
        }

        delete[] static_cast<uint8_t*>(m_verts);
        delete[] m_tris;
        delete[] m_vertsInfluences;
        delete[] m_vertsRemap;

        if (m_IbPoolField.Ib.IsValid())
        {
            M3D_RENDERER->ReleaseIbPoolField(m_IbPoolField);
            if ((m_meshType == 4 || m_meshType == 1) && m_VbPoolField.Vb.IsValid())
            {
                M3D_RENDERER->ReleaseVbPoolField(m_VbPoolField);
            }
        }

        delete[] m_trisWelded;
        delete[] m_faceNormals;
    }

    AnimatedModel::Mesh::Mesh()
    {
        this->m_verts = 0;
        this->m_vertsInfluences = 0;
        this->m_tris = 0;
        this->m_lastFrameUpdated = -1;
        this->m_drawIndices = 0;
        this->m_numDrawIndices = 0;
        this->m_drawVerts = 0;
        this->m_numDrawVerts = 0;
        this->m_vertsRemap = 0;
        this->m_numFacesWelded = 0;
        this->m_trisWelded = 0;
        this->m_numVertsWelded = 0;
        this->m_vertsWelded = 0;
        this->m_faceNormals = 0;
    }

    DSurfaceMaterial& AnimatedModel::Mesh::GetMaterial(unsigned skinNumber) const
    {
        if (skinNumber < m_pModelSkins->size())
        {
            return (*m_pModelSkins)[skinNumber][m_MaterialNumber];
        }
        return m_pModelSkins->front()[m_MaterialNumber];
    }

    void AnimatedModel::CreateTexFileMapping()
    {
        scoped_ptr fileStream = g_Kernel->GetFileServer().CreateFileStream();
        if (fileStream->Open("data/models/ModelTextures.xml", fs::IStream::OPEN_READ))
        {
            ref_ptr xml = g_Kernel->CreateXmlFile();
            if (xml->Read(*fileStream))
            {
                fileStream->Close();

                ref_ptr texturesNode = xml->CreateNode(cmn::XmlNodeType::XML_NODE_EMPTY, nullptr);
                ref_ptr fileNode = xml->CreateNode(cmn::XmlNodeType::XML_NODE_EMPTY, nullptr);
                xml->GetFirstChild(texturesNode, "Textures");
                //TODO: check this
                for (texturesNode->GetFirstChild(fileNode, "file"); !fileNode->IsEmpty();
                     fileNode->GetNextSibling(fileNode, "file"))
                {
                    CStr name;
                    CStr path;
                    SafeStrAttrib(name, fileNode, "name");
                    SafeStrAttrib(path, fileNode, "path");
                    m_textureFiles.insert({name, path});
                }
            }
            else
            {
                M3D_LOG_INFO("Error: Can't read file: data/models/ModelTextures.xml");
            }
        }
        else
        {
            M3D_LOG_INFO("Error: Can't open file: data/models/ModelTextures.xml");
        }
    }

    DRAFT_HierGeom const* AnimatedModel::GetHierGeom(unsigned num) const
    {
        // RVA 0x711C30
        if (num < m_HierGeoms.size())
        {
            return &m_HierGeoms[num];
        }
        return nullptr;
    }

    AnimatedModel::AnimatedModel()
    {
        this->m_boneInitialPos = 0;
        this->m_meshes = 0;
        this->m_animations = 0;
        this->m_initialBoneInvMatrices = 0;
        this->m_composite = 0;
        this->m_passable = 0;
        this->m_bVerification = 0;
        this->m_hasCubemap = 0;
        memset(&m_Dummy, 0, sizeof(m_Dummy));
        m_Dummy._44 = 1.0;
        m_Dummy._33 = 1.0;
        m_Dummy._22 = 1.0;
        m_Dummy._11 = 1.0;
    }

    AnimatedModel::Mesh& AnimatedModel::GetMesh(unsigned MeshNum)
    {
        return this->m_meshes[MeshNum];
    }

    AnimatedModel::Mesh const& AnimatedModel::GetMesh(unsigned MeshNum) const
    {
        return this->m_meshes[MeshNum];
    }

    DRAFT_BoneBounds const& AnimatedModel::GetBoneBounds(unsigned n)
    {
        return m_BonesBounds[n];
    }

    CMatrix AnimatedModel::GetBoneMatrix(int boneIndex) const
    {
        // TODO: generated code
        // Initialize result as identity matrix
        CMatrix result;
        result.identity();

        if (boneIndex < 0)
            return result;

        Bone* bones = m_boneInitialPos;

        // Calculate local bone matrix from quaternion
        Bone const& currentBone = bones[boneIndex];
        Quaternion const& q = currentBone.m_quaternion0;

        CMatrix boneMatrix;
        memset(&boneMatrix, 0, sizeof(boneMatrix));

        // Convert quaternion to rotation matrix
        float xx = q.x * q.x;
        float yy = q.y * q.y;
        float zz = q.z * q.z;
        float xy = q.x * q.y;
        float xz = q.x * q.z;
        float yz = q.y * q.z;
        float wx = q.w * q.x;
        float wy = q.w * q.y;
        float wz = q.w * q.z;

        boneMatrix._11 = 1.0f - 2.0f * (yy + zz);
        boneMatrix._12 = 2.0f * (xy + wz);
        boneMatrix._13 = 2.0f * (xz - wy);
        boneMatrix._14 = 0.0f;

        boneMatrix._21 = 2.0f * (xy - wz);
        boneMatrix._22 = 1.0f - 2.0f * (xx + zz);
        boneMatrix._23 = 2.0f * (yz + wx);
        boneMatrix._24 = 0.0f;

        boneMatrix._31 = 2.0f * (xz + wy);
        boneMatrix._32 = 2.0f * (yz - wx);
        boneMatrix._33 = 1.0f - 2.0f * (xx + yy);
        boneMatrix._34 = 0.0f;

        // Set bone position
        boneMatrix._41 = currentBone.m_translation0[0];
        boneMatrix._42 = currentBone.m_translation0[1];
        boneMatrix._43 = currentBone.m_translation0[2];
        boneMatrix._44 = 1.0f;

        // Start with the current bone's matrix
        CMatrix finalMatrix = boneMatrix;

        // Traverse up the bone hierarchy
        int parentIndex = currentBone.m_parentIdx;
        while (parentIndex >= 0)
        {
            Bone const& parentBone = bones[parentIndex];
            Quaternion const& parentQ = parentBone.m_quaternion0;

            // Calculate parent bone matrix from quaternion
            CMatrix parentMatrix;
            memset(&parentMatrix, 0, sizeof(parentMatrix));

            float parent_xx = parentQ.x * parentQ.x;
            float parent_yy = parentQ.y * parentQ.y;
            float parent_zz = parentQ.z * parentQ.z;
            float parent_xy = parentQ.x * parentQ.y;
            float parent_xz = parentQ.x * parentQ.z;
            float parent_yz = parentQ.y * parentQ.z;
            float parent_wx = parentQ.w * parentQ.x;
            float parent_wy = parentQ.w * parentQ.y;
            float parent_wz = parentQ.w * parentQ.z;

            parentMatrix._11 = 1.0f - 2.0f * (parent_yy + parent_zz);
            parentMatrix._12 = 2.0f * (parent_xy + parent_wz);
            parentMatrix._13 = 2.0f * (parent_xz - parent_wy);
            parentMatrix._14 = 0.0f;

            parentMatrix._21 = 2.0f * (parent_xy - parent_wz);
            parentMatrix._22 = 1.0f - 2.0f * (parent_xx + parent_zz);
            parentMatrix._23 = 2.0f * (parent_yz + parent_wx);
            parentMatrix._24 = 0.0f;

            parentMatrix._31 = 2.0f * (parent_xz + parent_wy);
            parentMatrix._32 = 2.0f * (parent_yz - parent_wx);
            parentMatrix._33 = 1.0f - 2.0f * (parent_xx + parent_yy);
            parentMatrix._34 = 0.0f;

            // Set parent bone position
            parentMatrix._41 = parentBone.m_translation0[0];
            parentMatrix._42 = parentBone.m_translation0[1];
            parentMatrix._43 = parentBone.m_translation0[2];
            parentMatrix._44 = 1.0f;

            // Multiply matrices: finalMatrix = parentMatrix * finalMatrix
            CMatrix tempResult;

            tempResult._11 = parentMatrix._11 * finalMatrix._11 + parentMatrix._12 * finalMatrix._21 +
                parentMatrix._13 * finalMatrix._31 + parentMatrix._14 * finalMatrix._41;
            tempResult._12 = parentMatrix._11 * finalMatrix._12 + parentMatrix._12 * finalMatrix._22 +
                parentMatrix._13 * finalMatrix._32 + parentMatrix._14 * finalMatrix._42;
            tempResult._13 = parentMatrix._11 * finalMatrix._13 + parentMatrix._12 * finalMatrix._23 +
                parentMatrix._13 * finalMatrix._33 + parentMatrix._14 * finalMatrix._43;
            tempResult._14 = parentMatrix._11 * finalMatrix._14 + parentMatrix._12 * finalMatrix._24 +
                parentMatrix._13 * finalMatrix._34 + parentMatrix._14 * finalMatrix._44;

            tempResult._21 = parentMatrix._21 * finalMatrix._11 + parentMatrix._22 * finalMatrix._21 +
                parentMatrix._23 * finalMatrix._31 + parentMatrix._24 * finalMatrix._41;
            tempResult._22 = parentMatrix._21 * finalMatrix._12 + parentMatrix._22 * finalMatrix._22 +
                parentMatrix._23 * finalMatrix._32 + parentMatrix._24 * finalMatrix._42;
            tempResult._23 = parentMatrix._21 * finalMatrix._13 + parentMatrix._22 * finalMatrix._23 +
                parentMatrix._23 * finalMatrix._33 + parentMatrix._24 * finalMatrix._43;
            tempResult._24 = parentMatrix._21 * finalMatrix._14 + parentMatrix._22 * finalMatrix._24 +
                parentMatrix._23 * finalMatrix._34 + parentMatrix._24 * finalMatrix._44;

            tempResult._31 = parentMatrix._31 * finalMatrix._11 + parentMatrix._32 * finalMatrix._21 +
                parentMatrix._33 * finalMatrix._31 + parentMatrix._34 * finalMatrix._41;
            tempResult._32 = parentMatrix._31 * finalMatrix._12 + parentMatrix._32 * finalMatrix._22 +
                parentMatrix._33 * finalMatrix._32 + parentMatrix._34 * finalMatrix._42;
            tempResult._33 = parentMatrix._31 * finalMatrix._13 + parentMatrix._32 * finalMatrix._23 +
                parentMatrix._33 * finalMatrix._33 + parentMatrix._34 * finalMatrix._43;
            tempResult._34 = parentMatrix._31 * finalMatrix._14 + parentMatrix._32 * finalMatrix._24 +
                parentMatrix._33 * finalMatrix._34 + parentMatrix._34 * finalMatrix._44;

            tempResult._41 = parentMatrix._41 * finalMatrix._11 + parentMatrix._42 * finalMatrix._21 +
                parentMatrix._43 * finalMatrix._31 + parentMatrix._44 * finalMatrix._41;
            tempResult._42 = parentMatrix._41 * finalMatrix._12 + parentMatrix._42 * finalMatrix._22 +
                parentMatrix._43 * finalMatrix._32 + parentMatrix._44 * finalMatrix._42;
            tempResult._43 = parentMatrix._41 * finalMatrix._13 + parentMatrix._42 * finalMatrix._23 +
                parentMatrix._43 * finalMatrix._33 + parentMatrix._44 * finalMatrix._43;
            tempResult._44 = parentMatrix._41 * finalMatrix._14 + parentMatrix._42 * finalMatrix._24 +
                parentMatrix._43 * finalMatrix._34 + parentMatrix._44 * finalMatrix._44;

            finalMatrix = tempResult;
            parentIndex = parentBone.m_parentIdx;
        }

        result = finalMatrix;
        return result;
    }

    void AnimatedModel::SetSkinsToLoad(LoadSkins const& skins)
    {
        m_loadSkins = skins;
    }

    int nextAnims_0[32] = {0, -1, 2, 3, 0xA, 0xA, 0xA, 0, -1, -1, 0xA, 0xB, -1, 0, -1, -1,
                           0, 0,  0, 0, 0,   0,   0,   0, 0,  0,  0,   0,   0,  0, 0,  0};

    bool AnimatedModel::LoadGAM(CStr const& fileName, bool bForceNextAnimation)
    {
        // TODO: generated code
        if (this->m_bVerification)
        {
            M3D_LOG_ERR("LoadGAM does work not for verification of models!!!");
            return false;
        }

        // Extract name and path
        m_Name = NameFromFileName(fileName).c_str();
        m_PathToFile = DirectoryFromFileName(fileName).c_str();

        m3d::fs::auxTaggedFile file;
        if (file.Open(fileName.c_str(), fs::auxTaggedFile::PROCESS_NORMAL_IGNORE_CRC))
        {
            M3D_LOG_ERR("AnimatedModel::LoadGAM -- cannot open file " + fileName);
            return false;
        }

        char* cFormatTitle = nullptr;
        file.getFormatTitle(&cFormatTitle);

        if (strcmp(cFormatTitle, "IVR"))
        {
            M3D_LOG_ERR("Wrong file format!!!");
            return false;
        }

        unsigned int uFormatVersion = 0;
        file.getFormatVersion(uFormatVersion);

        if (uFormatVersion > 1)
        {
            M3D_LOG_ERR("AnimatedModel::LoadGAM -- tagged file version is wrong: " + CStr(uFormatVersion));
            return false;
        }

        fs::TaggedFileReader stream(file);

        // Read header chunk (chunk 1)
        stream.setChunk(1u);

        m_header.m_numTriMeshes = stream.get<int16_t>();
        m_header.m_numSkinMeshes = stream.get<int16_t>();
        m_header.m_numStaticMeshes = stream.get<int16_t>();
        m_header.m_numAnimations = stream.get<int16_t>();

        uint16_t numMaterials = stream.get<int16_t>();

        m_header.m_numNodes = stream.get<int16_t>();
        m_cfgSize = stream.get<uint32_t>();

        // Read bone data chunk (chunk 2)
        stream.setChunk(2u);

        m_boneInitialPos = new Bone[m_header.m_numNodes];
        m_initialBoneInvMatrices = new CMatrix[m_header.m_numNodes];

        for (short i = 0; i < m_header.m_numNodes; ++i)
        {
            auto& bone = m_boneInitialPos[i];
            strcpy_s(bone.m_boneName, stream.getStr(40));
            bone.m_ownIdx = i;
            bone.m_parentIdx = stream.get<int32_t>();
            bone.m_translation0 = stream.get<CVector>();
            bone.m_quaternion0 = stream.get<Quaternion>();

            // TODO: check this
            m_initialBoneInvMatrices[i] = stream.get<CMatrix>();
        }

        // Read mesh data chunk (chunk 4)
        stream.setChunk(4u);

        m_numMeshes = m_header.m_numStaticMeshes + m_header.m_numTriMeshes + m_header.m_numSkinMeshes;
        m_meshes = new Mesh[m_numMeshes];

        for (unsigned i = 0; i < m_numMeshes; ++i)
        {
            auto& mesh = m_meshes[i];
            strcpy_s(mesh.m_meshName, stream.getStr(40));
            mesh.meshId = i;
            mesh.m_meshType = stream.get<int32_t>();
            mesh.m_numNode = stream.get<int32_t>();
            mesh.groupId = stream.get<uint32_t>();
            mesh.m_MaterialNumber = stream.get<int32_t>();
            mesh.m_VertexTypeSize = stream.get<int32_t>();
            mesh.m_VertexType = stream.get<rend::VertexType>(4);
            mesh.m_numVertices = stream.get<int32_t>();
            mesh.m_numFaces = stream.get<int32_t>();
            mesh.m_pModelSkins = &m_Skins;

            auto const vertsSize = mesh.m_numVertices * mesh.m_VertexTypeSize;
            mesh.m_verts = new uint8_t[vertsSize];
            memcpy(mesh.m_verts, stream.getRaw(vertsSize), vertsSize);

            if (mesh.m_meshType == 4)
            {
                mesh.m_VbPoolField = M3D_RENDERER->AddVbPoolField(mesh.m_VertexType, mesh.m_numVertices);
                memcpy(M3D_RENDERER->LockVbPoolField(mesh.m_VbPoolField), mesh.m_verts, vertsSize);
                M3D_RENDERER->UnlockVbPoolField(mesh.m_VbPoolField);
            }
            if (mesh.m_meshType == 1)
            {
                mesh.m_VbPoolField = M3D_RENDERER->AddVbPoolField(mesh.m_VertexType, mesh.m_numVertices);
                memcpy(M3D_RENDERER->LockVbPoolField(mesh.m_VbPoolField), stream.getRaw(vertsSize), vertsSize);
                M3D_RENDERER->UnlockVbPoolField(mesh.m_VbPoolField);
            }
            if (mesh.m_meshType == 2)
            {
                mesh.m_vertsInfluences = new Influences[mesh.m_numVertices];
                for (int v = 0; v < mesh.m_numVertices; ++v)
                {
                    auto& infl = mesh.m_vertsInfluences[v];
                    infl.m_numBones = stream.get<uint16_t>();
                    for (int b = 0; b < 4; ++b)
                    {
                        // TODO: check this
                        auto& boneInfl = infl.m_influences[b];
                        boneInfl.m_boneIdx = stream.get<int16_t>();
                        boneInfl.m_boneWeight = stream.get<float>();
                        boneInfl.m_offsetVec = stream.get<CVector>();
                        boneInfl.m_offsetNormal = stream.get<CVector>();
                    }
                }
            }

            auto const trisCount = mesh.m_numFaces * 3;
            mesh.m_tris = new uint16_t[trisCount];

            auto const trisSize = trisCount * sizeof(uint16_t);
            memcpy(mesh.m_tris, stream.getRaw(trisSize), trisSize);

            mesh.m_drawVerts = mesh.m_verts;
            mesh.m_numDrawIndices = trisCount;
            mesh.m_drawIndices = mesh.m_tris;
            mesh.m_numDrawVerts = mesh.m_numVertices;

            mesh.m_IbPoolField = M3D_RENDERER->AddIbPoolField(mesh.m_numDrawIndices);
            memcpy(M3D_RENDERER->LockIbPoolField(mesh.m_IbPoolField), mesh.m_drawIndices, trisSize);
            M3D_RENDERER->UnlockIbPoolField(mesh.m_IbPoolField);
        }

        m_box.m_box[0] = stream.get<float>();
        m_box.m_box[1] = stream.get<float>();
        m_box.m_box[2] = stream.get<float>();
        m_box.m_box[3] = stream.get<float>();
        m_box.m_box[4] = stream.get<float>();
        m_box.m_box[5] = stream.get<float>();

        // Animation data (chunk 8)
        stream.setChunk(8u);

        m_animations = new Animation[m_header.m_numAnimations];
        for (int i = 0; i < m_header.m_numAnimations; ++i)
        {
            auto& animation = m_animations[i];
            strcpy_s(animation.m_name, stream.getStr(25));
            animation.m_numFrames = stream.get<int16_t>();
            animation.m_fps = stream.get<int16_t>();
            animation.m_nextAnimation = stream.get<int16_t>();
            animation.m_numChanges = stream.get<int16_t>();
            animation.m_numNodes = stream.get<int16_t>();
            animation.m_action = stream.get<ActionType>(4);

            animation.m_hierChanges = new HierarchyChange[animation.m_numChanges];
            for (int c = 0; c < animation.m_numChanges; ++c)
            {
                auto& change = animation.m_hierChanges[c];
                change.changeType = stream.get<DRAFT_Change>(4);
                change.ownIdx = stream.get<int16_t>();
                change.newParentIdx = stream.get<int16_t>();
            }

            animation.m_nodesPositions = new AnimationTransform[animation.m_numFrames * animation.m_numNodes];
            for (int t = 0; t < animation.m_numFrames * animation.m_numNodes; ++t)
            {
                auto& transform = animation.m_nodesPositions[t];
                transform.idx = stream.get<int16_t>();
                transform.tx = stream.get<float>();
                transform.ty = stream.get<float>();
                transform.tz = stream.get<float>();
                transform.qx = stream.get<float>();
                transform.qy = stream.get<float>();
                transform.qz = stream.get<float>();
                transform.qw = stream.get<float>();
            }
        }

        // Animation remapping
        // TODO: check this
        for (int i = 0; i < 32; ++i)
        {
            m_animRemap[i] = -1;
            for (int j = 0; j < m_header.m_numAnimations; ++j)
            {
                if (!strcmp(actions[i].m_name, m_animations[j].m_name))
                {
                    m_animRemap[i] = j;
                    m_animations[j].m_action = actions[i].m_action;
                    break;
                }
            }
        }

        // Set next animations
        // TODO: refactor
        for (int i = 0; i < m_header.m_numAnimations; ++i)
        {
            if (bForceNextAnimation)
            {
                int v97 = 0;
                short* v98 = &this->m_animRemap[1];

                while (*(v98 - 1) != i)
                {
                    if (*v98 == i)
                    {
                        ++v97;
                        break;
                    }
                    if (v98[1] == i)
                    {
                        v97 += 2;
                        break;
                    }
                    if (v98[2] == i)
                    {
                        v97 += 3;
                        break;
                    }
                    v97 += 4;
                    v98 += 4;
                    if (v97 >= 32)
                    {
                        break;
                    }
                }

                int const v99 = v97 == 32 ? -1 : nextAnims_0[v97];
                if (v99 < 0)
                {
                    this->m_animations[i].m_nextAnimation = -1;
                }
                else
                {
                    this->m_animations[i].m_nextAnimation = this->m_animRemap[v99];
                }
            }
            else
            {
                this->m_animations[i].m_nextAnimation = i;
            }
        }

        // Load skins/materials (chunk 15)
        stream.setChunk(0xFu);
        m_Skins.resize(stream.get<uint32_t>());

        for (int skinsIdx = 0; skinsIdx < this->m_Skins.size(); ++skinsIdx)
        {
            auto& surfaceMaterials = m_Skins[skinsIdx];
            surfaceMaterials.resize(numMaterials);

            bool load = true;
            if (!m_loadSkins.loadAllSkins)
            {
                load = m_loadSkins.loadSkins.find(skinsIdx) != m_loadSkins.loadSkins.end();
            }

            for (int matIdx = 0; matIdx < numMaterials; ++matIdx)
            {
                auto& surfaceMaterial = surfaceMaterials[matIdx];

                auto const materialSize = sizeof(rend::Material);
                assert(materialSize == 68);

                memcpy(&surfaceMaterial, stream.getRaw(materialSize), materialSize);
                unsigned int textureCount = stream.get<uint32_t>();

                // TODO: check size
                surfaceMaterial.Shader.Name = stream.getStr(100);

                if (load && !surfaceMaterial.Shader.Name.empty())
                {
                    NewEffect(surfaceMaterial.Shader.Name, surfaceMaterial.Shader.Handle);
                }
                else
                {
                    surfaceMaterial.Shader.Handle = nullptr;
                }

                // Initialize texture array
                surfaceMaterial.Textures.resize(textureCount);
                for (int texIdx = 0; texIdx < textureCount; ++texIdx)
                {
                    auto& textureInfo = surfaceMaterial.Textures[texIdx];
                    textureInfo.FileName = stream.getStr(40);
                    textureInfo.UV_Set = stream.get<uint32_t>();
                    textureInfo.Type = stream.get<DRAFT_TextureType>(4);
                    textureInfo.Handle.SetInvalid();

                    auto pathToTex = DefinePathToTexture(textureInfo.FileName.c_str());
                    if (load)
                    {
                        SetTexture(pathToTex, textureInfo.Type, textureInfo.Handle);
                    }
                }
            }
        }

        // Load collision data (chunk 16)
        if (!stream.setChunk(16u))
        {
            auto const pointsCount = stream.get<uint32_t>();
            auto const trisCount = stream.get<uint32_t>();

            m_Collision.Points.resize(pointsCount);

            auto const cvectorSize = sizeof(CVector);
            assert(cvectorSize == 12);

            auto const pointsSize = pointsCount * cvectorSize;
            memcpy(m_Collision.Points.data(), stream.getRaw(pointsSize), pointsSize);

            m_Collision.Triangles.resize(trisCount);

            auto const index3Size = sizeof(Index3);
            assert(index3Size == 6);

            auto const trisSize = trisCount * index3Size;
            memcpy(m_Collision.Triangles.data(), stream.getRaw(trisSize), trisSize);
        }

        // Load geometry data (chunk 32)
        if (!stream.setChunk(32u))
        {
            auto const count = stream.get<uint32_t>();
            m_Geoms.resize(count);

            auto const draftGeomSize = sizeof(DRAFT_Geom);
            assert(draftGeomSize == 44);

            auto const size = count * draftGeomSize;
            memcpy(m_Geoms.data(), stream.getRaw(size), size);
        }

        // Load hierarchical geometry (chunk 64)
        if (!stream.setChunk(64u))
        {
            auto const count = stream.get<uint32_t>();
            m_HierGeoms.resize(count);

            auto const draftGeomSize = sizeof(DRAFT_HierGeom);
            assert(draftGeomSize == 48);

            auto const size = count * draftGeomSize;
            memcpy(m_HierGeoms.data(), stream.getRaw(size), size);
        }

        // Load bone bounds (chunk 128)
        if (!stream.setChunk(128u))
        {
            auto const count = stream.get<uint32_t>();

            std::vector<m3d::DRAFT_BoneBounds> bonesBounds;
            bonesBounds.resize(count);

            auto const boneSize = sizeof(DRAFT_BoneBounds);
            assert(boneSize == 28);

            auto const size = count * boneSize;
            memcpy(bonesBounds.data(), stream.getRaw(size), size);

            for (auto& bound : bonesBounds)
            {
                m_BonesBounds[bound.BoneIndex] = bound;
            }
        }

        // Load mesh groups (chunk 240)
        if (!stream.setChunk(240u))
        {
            auto const groupsCount = stream.get<uint32_t>();
            m_MhGroups.resize(groupsCount);

            for (int i = 0; i < groupsCount; ++i)
            {
                auto& group = m_MhGroups[i];
                group.Name = stream.getStr(20);
                group.VisibleAtOnceMin = stream.get<uint32_t>();
                group.VisibleAtOnceMax = stream.get<uint32_t>();

                auto const meshesCount = stream.get<uint32_t>();
                group.MeshesId.resize(meshesCount);
                memcpy(group.MeshesId.data(), stream.getRaw(4 * meshesCount), 4 * meshesCount);

                auto const variantsCount = stream.get<uint32_t>();
                group.m_variants.resize(variantsCount);

                for (int v = 0; v < variantsCount; ++v)
                {
                    auto const variantSize = stream.get<uint32_t>();
                    group.m_variants[v].resize(variantSize);
                    memcpy(group.m_variants[v].data(), stream.getRaw(4 * variantSize), 4 * variantSize);
                }
            }
        }
        return true;
    }

    bool AnimatedModel::LoadSAM(CStr const& fileName, bool forceNextAnimation)
    {
        // RVA 0x789E30 - loads an uncompiled (SAM, "DFT" tagged file) model and hands it to Convert. Chunks: 6 the
        // header, 3 the bones, 1 the meshes, 4 the animations, 8 the skins (else 2, a single skin's materials), 5
        // the collision mesh, 7 and 10 the collision primitives, 11 the bone bounds, 9 the mesh groups.
        m_Name = NameFromFileName(fileName).c_str();
        m_PathToFile = DirectoryFromFileName(fileName).c_str();

        fs::auxTaggedFile File;
        unsigned char* Data = nullptr;
        if (File.Open(fileName.c_str(), fs::auxTaggedFile::PROCESS_NORMAL_IGNORE_CRC))
        {
            M3D_LOG_ERR(CStr("[AnimatedModel] : LoadFromDraft -- cannot open file ") + fileName);
            return false;
        }
        char* FormatTitle = nullptr;
        File.getFormatTitle(&FormatTitle);
        if (strcmp(FormatTitle, "DFT"))
        {
            // NOTE: the message logged is empty.
            M3D_LOG_ERR(CStr(""));
            return false;
        }
        unsigned int FormatVersion = 0;
        File.getFormatVersion(FormatVersion);
        if (FormatVersion > 1)
        {
            M3D_LOG_ERR(CStr("[AnimatedModel] : LoadFromDraft -- tagged file version is wrong ") + CStr(FormatVersion));
            return false;
        }

        File.getChunkData(6u, reinterpret_cast<void**>(&Data));
        DRAFT_Header header;
        memcpy(&header, Data, sizeof(header));

        File.getChunkData(3u, reinterpret_cast<void**>(&Data));
        DRAFT_Bone zeroBone;
        memset(&zeroBone, 0, sizeof(zeroBone));
        retruxx::vector<DRAFT_Bone> Bones(header.NumberOfBones, zeroBone);
        memcpy(Bones.data(), Data, sizeof(DRAFT_Bone) * Bones.size());

        retruxx::vector<DMesh> Meshes(header.NumberOfMeshes);
        // Per mesh, per vertex: the bone influences (an influence component points at its mesh's list).
        retruxx::vector<retruxx::vector<retruxx::vector<DInfluence>>> Infls(header.NumberOfMeshes);
        // The vertex data read so far; freed once Convert has copied it. Influence lists are owned by Infls.
        auto const freeComponents = [&Meshes]() {
            for (DMesh& mesh : Meshes)
            {
                for (unsigned int c = 0; c < mesh.VerticesComponents.size(); ++c)
                {
                    if (mesh.VertexComponentHeaders[c].Size > 0)
                    {
                        delete[] static_cast<unsigned char*>(mesh.VerticesComponents[c]);
                        mesh.VerticesComponents[c] = nullptr;
                    }
                }
            }
        };

        File.getChunkData(1u, reinterpret_cast<void**>(&Data));
        for (unsigned int i = 0; i < Meshes.size(); ++i)
        {
            DMesh& mesh = Meshes[i];
            memcpy(&mesh.Header, Data, sizeof(DRAFT_GeometryHeader));
            Data += sizeof(DRAFT_GeometryHeader);
            mesh.VertexComponentHeaders.resize(mesh.Header.NumberOfVertexComponents);
            memcpy(mesh.VertexComponentHeaders.data(), Data,
                sizeof(DRAFT_VertexComponent) * mesh.Header.NumberOfVertexComponents);
            Data += sizeof(DRAFT_VertexComponent) * mesh.Header.NumberOfVertexComponents;
            if (!DefineVertexType(mesh.VertexComponentHeaders, mesh.VertType, mesh.VertTypeSize))
            {
                // NOTE: the vertex data of the meshes read so far is leaked.
                return false;
            }
            mesh.VerticesComponents.resize(mesh.Header.NumberOfVertexComponents, nullptr);
            for (unsigned int j = 0; j < mesh.Header.NumberOfVertexComponents; ++j)
            {
                DRAFT_VertexComponent const& comp = mesh.VertexComponentHeaders[j];
                if (comp.Size > 0)
                {
                    unsigned int const size = comp.Size * mesh.Header.VertexCount;
                    mesh.VerticesComponents[j] = new unsigned char[size];
                    memcpy(mesh.VerticesComponents[j], Data, size);
                    Data += size;
                    continue;
                }
                if (comp.Type != INFLUENCES)
                {
                    // NOTE: the vertex data of the meshes read so far is leaked.
                    M3D_LOG_ERR(CStr("Unknown  component of vertex "));
                    return false;
                }
                Infls[i].resize(mesh.Header.VertexCount);
                for (unsigned int v = 0; v < mesh.Header.VertexCount; ++v)
                {
                    unsigned int const count = *reinterpret_cast<unsigned int const*>(Data);
                    Data += sizeof(unsigned int);
                    Infls[i][v].resize(count, DInfluence());
                    for (unsigned int m = 0; m < count; ++m)
                    {
                        Infls[i][v][m].BoneIndex = *reinterpret_cast<short const*>(Data);
                        Data += sizeof(short);
                        Infls[i][v][m].Weight = *reinterpret_cast<float const*>(Data);
                        Data += sizeof(float);
                    }
                }
                mesh.VerticesComponents[j] = Infls[i].data();
            }
            BinormalToTangentW(mesh);
            mesh.Triangles.resize(mesh.Header.TriangleCount, Index3());
            memcpy(mesh.Triangles.data(), Data, sizeof(Index3) * mesh.Triangles.size());
            Data += sizeof(Index3) * mesh.Triangles.size();
        }

        retruxx::vector<DAnimation> Animations(header.NumberOfAnimations, DAnimation());
        if (!File.isChunkPresent(4u))
        {
            if (m_composite)
            {
                // A composite model without animations gets a still two-frame STAND1.
                Animations.resize(1, DAnimation());
                DAnimation& anim = Animations.back();
                strcpy(anim.Info.Name, "STAND1");
                anim.Info.FPS_Number = 10;
                anim.Info.FramesNumber = 2;
                anim.Info.NextAnimation = -1;
                anim.Info.NumberOfChanges = 0;
                anim.AnimationKeys.resize(anim.Info.FramesNumber);
                for (unsigned int f = 0; f < anim.Info.FramesNumber; ++f)
                {
                    anim.AnimationKeys[f].resize(header.NumberOfBones);
                    for (DRAFT_Transform& t : anim.AnimationKeys[f])
                    {
                        t.Tx = 0.0f;
                        t.Ty = 0.0f;
                        t.Tz = 0.0f;
                        t.Sx = 1.0f;
                        t.Sy = 1.0f;
                        t.Sz = 1.0f;
                        t.Rx = 0.0f;
                        t.Ry = 0.0f;
                        t.Rz = 0.0f;
                        t.Rw = 1.0f;
                    }
                }
            }
        }
        else
        {
            File.getChunkData(4u, reinterpret_cast<void**>(&Data));
            for (DAnimation& anim : Animations)
            {
                memcpy(&anim.Info, Data, sizeof(DRAFT_AnimationHeader));
                Data += sizeof(DRAFT_AnimationHeader);
                if (anim.Info.NumberOfChanges)
                {
                    anim.HierarchyChanges.resize(anim.Info.NumberOfChanges, DRAFT_HierarchyChange());
                    memcpy(anim.HierarchyChanges.data(), Data, sizeof(DRAFT_HierarchyChange) * anim.Info.NumberOfChanges);
                    Data += sizeof(DRAFT_HierarchyChange) * anim.Info.NumberOfChanges;
                }
                anim.AnimationKeys.resize(anim.Info.FramesNumber);
                for (unsigned int f = 0; f < anim.Info.FramesNumber; ++f)
                {
                    DRAFT_Transform zero;
                    memset(&zero, 0, sizeof(zero));
                    anim.AnimationKeys[f].resize(header.NumberOfBones, zero);
                    memcpy(anim.AnimationKeys[f].data(), Data, sizeof(DRAFT_Transform) * header.NumberOfBones);
                    Data += sizeof(DRAFT_Transform) * header.NumberOfBones;
                }
            }
        }

        if (File.isChunkPresent(8u))
        {
            File.getChunkData(8u, reinterpret_cast<void**>(&Data));
            unsigned int const SkinsNumber = *reinterpret_cast<unsigned int const*>(Data);
            Data += sizeof(unsigned int);
            m_Skins.resize(SkinsNumber);
            for (unsigned int j = 0; j < SkinsNumber; ++j)
            {
                m_Skins[j].resize(header.NumberOfMaterials, DSurfaceMaterial());
                bool const load =
                    m_loadSkins.loadAllSkins || m_loadSkins.loadSkins.find(j) != m_loadSkins.loadSkins.end();
                for (unsigned int m = 0; m < header.NumberOfMaterials; ++m)
                {
                    ReadMaterial(m_Skins[j][m], Data, load);
                }
            }
        }
        else
        {
            m_Skins.resize(1);
            m_Skins[0].resize(header.NumberOfMaterials, DSurfaceMaterial());
            File.getChunkData(2u, reinterpret_cast<void**>(&Data));
            for (unsigned int m = 0; m < header.NumberOfMaterials; ++m)
            {
                ReadMaterial(m_Skins[0][m], Data, true);
            }
        }

        if (File.isChunkPresent(5u))
        {
            File.getChunkData(5u, reinterpret_cast<void**>(&Data));
            DRAFT_CollisionHeader collision;
            memcpy(&collision, Data, sizeof(collision));
            Data += sizeof(collision);
            m_Collision.Points.resize(collision.PointCount);
            memcpy(m_Collision.Points.data(), Data, sizeof(CVector) * collision.PointCount);
            Data += sizeof(CVector) * collision.PointCount;
            m_Collision.Triangles.resize(collision.TriangleCount, Index3());
            memcpy(m_Collision.Triangles.data(), Data, sizeof(Index3) * collision.TriangleCount);
            Data += sizeof(Index3) * collision.TriangleCount;
        }
        if (File.isChunkPresent(7u))
        {
            File.getChunkData(7u, reinterpret_cast<void**>(&Data));
            unsigned int const count = *reinterpret_cast<unsigned int const*>(Data);
            Data += sizeof(unsigned int);
            m_Geoms.resize(count);
            memcpy(m_Geoms.data(), Data, sizeof(DRAFT_Geom) * count);
        }
        if (File.isChunkPresent(10u))
        {
            File.getChunkData(10u, reinterpret_cast<void**>(&Data));
            unsigned int const count = *reinterpret_cast<unsigned int const*>(Data);
            Data += sizeof(unsigned int);
            m_HierGeoms.resize(count);
            memcpy(m_HierGeoms.data(), Data, sizeof(DRAFT_HierGeom) * count);
        }
        if (File.isChunkPresent(11u))
        {
            File.getChunkData(11u, reinterpret_cast<void**>(&Data));
            unsigned int const count = *reinterpret_cast<unsigned int const*>(Data);
            Data += sizeof(unsigned int);
            retruxx::vector<DRAFT_BoneBounds> bounds(count);
            memcpy(bounds.data(), Data, sizeof(DRAFT_BoneBounds) * count);
            for (DRAFT_BoneBounds const& b : bounds)
            {
                m_BonesBounds[b.BoneIndex] = b;
            }
        }

        m_MhGroups.clear();
        m_MhGroups.shrink_to_fit();
        if (!File.isChunkPresent(9u))
        {
            // Without groups every mesh is shown, in one group.
            m_MhGroups.push_back(MeshesGroup());
            MeshesGroup& group = m_MhGroups.back();
            group.Name = "Main";
            group.VisibleAtOnceMax = static_cast<int>(Meshes.size());
            group.VisibleAtOnceMin = static_cast<int>(Meshes.size());
            for (unsigned int i = 0; i < Meshes.size(); ++i)
            {
                group.MeshesId.push_back(i);
            }
        }
        else
        {
            File.getChunkData(9u, reinterpret_cast<void**>(&Data));
            unsigned int const count = *reinterpret_cast<unsigned int const*>(Data);
            Data += sizeof(unsigned int);
            m_MhGroups.resize(count);
            for (unsigned int g = 0; g < count; ++g)
            {
                MeshesGroup& group = m_MhGroups[g];
                // NOTE: the 20-character name is taken as a C string, so an unterminated one runs on.
                char GroupName[20];
                memcpy(GroupName, Data, sizeof(GroupName));
                Data += sizeof(GroupName);
                group.Name = GroupName;
                group.VisibleAtOnceMax = *reinterpret_cast<int const*>(Data);
                Data += sizeof(int);
                group.VisibleAtOnceMin = *reinterpret_cast<int const*>(Data);
                Data += sizeof(int);
                unsigned int const ids = *reinterpret_cast<unsigned int const*>(Data);
                Data += sizeof(unsigned int);
                group.MeshesId.resize(ids, 0);
                for (unsigned int k = 0; k < ids; ++k)
                {
                    group.MeshesId[k] = *reinterpret_cast<unsigned int const*>(Data);
                    Data += sizeof(unsigned int);
                }
                // A negative limit means all of the group; both are clamped to the group, the minimum to the
                // maximum.
                int const size = static_cast<int>(group.MeshesId.size());
                if (group.VisibleAtOnceMax < 0)
                {
                    group.VisibleAtOnceMax = size;
                }
                if (group.VisibleAtOnceMax < 0)
                {
                    group.VisibleAtOnceMax = 0;
                }
                if (group.VisibleAtOnceMax > size)
                {
                    group.VisibleAtOnceMax = size;
                }
                if (group.VisibleAtOnceMin < 0)
                {
                    group.VisibleAtOnceMin = size;
                }
                if (group.VisibleAtOnceMin < 0)
                {
                    group.VisibleAtOnceMin = 0;
                }
                if (group.VisibleAtOnceMin > group.VisibleAtOnceMax)
                {
                    group.VisibleAtOnceMin = group.VisibleAtOnceMax;
                }
            }
        }

        File.Close();
        // NOTE: a failed conversion is not reported; the load counts as done.
        Convert(fileName, forceNextAnimation, Bones, Meshes, Animations);
        freeComponents();
        return true;
    }

    AnimatedModel::Bone& AnimatedModel::GetBone(unsigned N) const
    {
        return this->m_boneInitialPos[N];
    }

    unsigned AnimatedModel::GetNumMeshes() const
    {
        return this->m_numMeshes;
    }

    void AnimatedModel::DeleteSkin(unsigned SkinNumber)
    {
        // RVA 0x718CF0 - releases the skin's shaders and textures and drops it. NOTE: unlike UnloadSkin, the released
        // texture handles are not reset (the skin goes away anyway).
        retruxx::vector<retruxx::vector<DSurfaceMaterial>> NewSkins;
        for (unsigned i = 0; i < m_Skins.size(); ++i)
        {
            if (i != SkinNumber)
            {
                NewSkins.push_back(m_Skins[i]);
            }
        }
        if (SkinNumber < m_Skins.size())
        {
            for (DSurfaceMaterial& mat : m_Skins[SkinNumber])
            {
                if (mat.Shader.Handle)
                {
                    mat.Shader.Handle->Release();
                    mat.Shader.Handle = nullptr;
                }
                for (DTextureInfo& tex : mat.Textures)
                {
                    M3D_RENDERER->ReleaseTexture(tex.Handle);
                }
            }
        }
        m_Skins = NewSkins;
    }

    void AnimatedModel::UnloadSkin(unsigned j)
    {
        if (j < m_Skins.size())
        {
            auto& mats = m_Skins[j];
            for (auto& mat : mats)
            {
                if (mat.Shader.Handle)
                {
                    mat.Shader.Handle->Release();
                    mat.Shader.Handle = nullptr;
                }

                for (auto& tex : mat.Textures)
                {
                    M3D_RENDERER->ReleaseTexture(tex.Handle);
                    tex.Handle.SetInvalid();
                }
            }
        }
    }

    void AnimatedModel::UpdateTexturesFilter()
    {
        // RVA 0x70DD60 - the model shaders take their sampling from macros: g_texturesFilter 3 is anisotropic, 5 is
        // trilinear, anything else bilinear.
        int const filter = M3D_ENGINE_CFG.m_g_texturesFilter.GetI();
        char const* minMag = "Linear";
        char const* mip = "Point";
        if (filter == 3)
        {
            minMag = "Anisotropic";
            mip = "Linear";
        }
        else if (filter == 5)
        {
            mip = "Linear";
        }
        rend::ShaderMacro const minMagMacro("MIN_MAG_FILTER", minMag);
        M3D_RENDERER->AddChangeShaderMacro(&minMagMacro);
        rend::ShaderMacro const mipMacro("MIP_FILTER", mip);
        M3D_RENDERER->AddChangeShaderMacro(&mipMacro);
        M3D_RENDERER->ReloadShaders();
    }

    void AnimatedModel::SetComposite(bool isComposite)
    {
        m_composite = isComposite;
    }

    unsigned AnimatedModel::GetNumSkins() const
    {
        return m_Skins.size();
    }
    retruxx::vector<m3d::DSurfaceMaterial, retruxx::allocator<m3d::DSurfaceMaterial>>& AnimatedModel::GetSkin(
        unsigned int n)
    {
        return m_Skins[n];
    }

    AnimatedModel::~AnimatedModel()
    {
        delete m_boneInitialPos;
        delete[] m_meshes;
        delete[] m_animations;
        delete[] m_initialBoneInvMatrices;
        // TODO: check this
        for (auto& skin : m_Skins)
        {
            for (auto& material : skin)
            {
                if (!m_bVerification)
                {
                    for (auto& texture : material.Textures)
                    {
                        M3D_RENDERER->ReleaseTexture(texture.Handle);
                    }
                }
            }
        }
    }

    rend::TexHandle AnimatedModel::GetTexHandle(unsigned SkinNumber, unsigned MatNum, unsigned TexNum) const
    {
        // RVA 0x70E650
        if (SkinNumber < m_Skins.size())
        {
            auto const& skin = m_Skins[SkinNumber];
            if (MatNum < skin.size() && TexNum < skin[MatNum].Textures.size())
            {
                return skin[MatNum].Textures[TexNum].Handle;
            }
        }
        return rend::TexHandle();
    }

    bool AnimatedModel::Load(CStr const& FileName, bool bForceNextAnimation)
    {
        if (M3D_KERNEL->GetEngineCfg().m_loadFromGAM.GetB())
            return m3d::AnimatedModel::LoadGAM(FileName, bForceNextAnimation);
        else
            return m3d::AnimatedModel::LoadSAM(FileName, bForceNextAnimation);
    }

    unsigned AnimatedModel::GetGroupsNum() const
    {
        return m_MhGroups.size();
    }

    int AnimatedModel::GetFrames(int anim, int pol) const
    {
        // RVA 0x70B5D0
        int const remapped = m_animRemap[anim];
        if (remapped >= 0 && remapped < m_header.m_numAnimations && pol)
        {
            return m_animations[remapped].m_numFrames - 1;
        }
        return 0;
    }

    namespace
    {
        // The debug overlays draw with no texture.
        void SetNoTexture()
        {
            M3D_RENDERER->SetTexture(0, rend::TexHandle(), -1.0);
        }

        // A bone's name at its projected origin, in 14pt Tahoma.
        void DrawBoneName(CVector const& org, char const* name)
        {
            CVector const v = M3D_RENDERER->Project(org - M3D_RENDERER->MatGetOrgInv());
            M3D_APP->SetFont(CStr("Tahoma"), 14.0f, 0, M3D_APP->m_codePage.CodePage);
            CStr const a(name);
            CStr text;
            text = a;
            M3D_APP->DrawTextAbsT(v.x, v.y, 0xFFFFFFFF, text, 0, -1);
        }

        // Three axes from org, red, green and blue.
        void DrawAxes(CVector const& org, CVector const& x, CVector const& y, CVector const& z)
        {
            M3D_APP->DrawLine(org, CVector(x.x + org.x, x.y + org.y, x.z + org.z), 0xFFFF0000);
            M3D_APP->DrawLine(org, CVector(y.x + org.x, y.y + org.y, y.z + org.z), 0xFF00FF00);
            M3D_APP->DrawLine(org, CVector(z.x + org.x, z.y + org.y, z.z + org.z), 0xFF0000FF);
        }
    }  // namespace

    void AnimatedModel::RenderNormals(CMatrix const& matT, AnimInfo* ai)
    {
        // RVA 0x711020 - a line along each vertex normal, half a unit long. Rigid meshes (type 1) are drawn under
        // their bone's matrix, their vertices taken back through the bone's inverse initial matrix first; skinned
        // ones (type 4) as they are. NOTE: the normal and texture coordinates of the line vertices are left unset.
        M3D_RENDERER->MatPush(matT);
        for (unsigned cc = 0; cc < m_numMeshes; ++cc)
        {
            Mesh const& mesh = m_meshes[cc];
            if (mesh.m_meshType == 1)
            {
                M3D_RENDERER->MatPushWorld();
                M3D_RENDERER->MatSetWorld(ai->m_bonesAnim[mesh.m_numNode].m_curMatrix);
            }
            if (mesh.m_meshType == 1 || mesh.m_meshType == 4)
            {
                CMatrix const& inv = m_initialBoneInvMatrices[mesh.m_numNode];
                rend::VbHandle const vb = M3D_RENDERER->GetVbStreaming(rend::VERTEX_XYZNT1);
                float* dst = static_cast<float*>(M3D_RENDERER->LockVb(vb, 2 * mesh.m_numDrawVerts, 0, 0));
                SetNoTexture();
                unsigned char const* src = static_cast<unsigned char const*>(mesh.m_drawVerts);
                for (int i = 0; i < mesh.m_numDrawVerts; ++i)
                {
                    float const* const v = reinterpret_cast<float const*>(src);
                    float px = v[0];
                    float py = v[1];
                    float pz = v[2];
                    float nx = v[3];
                    float ny = v[4];
                    float nz = v[5];
                    if (mesh.m_meshType == 1)
                    {
                        float const tx = inv._11 * px + pz * inv._31 + py * inv._21 + inv._41;
                        float const ty = inv._12 * px + inv._32 * pz + inv._22 * py + inv._42;
                        float const tz = inv._33 * pz + inv._23 * py + px * inv._13 + inv._43;
                        float const tnx = inv._11 * nx + nz * inv._31 + ny * inv._21;
                        float const tny = inv._12 * nx + inv._32 * nz + inv._22 * ny;
                        float const tnz = inv._33 * nz + inv._23 * ny + nx * inv._13;
                        px = tx;
                        py = ty;
                        pz = tz;
                        nx = tnx;
                        ny = tny;
                        nz = tnz;
                    }
                    // Two XYZNT1 vertices (8 floats each): the vertex and the tip of its normal.
                    dst[0] = px;
                    dst[1] = py;
                    dst[2] = pz;
                    dst[8] = nx * 0.5f + px;
                    dst[9] = ny * 0.5f + py;
                    dst[10] = nz * 0.5f + pz;
                    dst += 16;
                    src += mesh.m_VertexTypeSize;
                }
                M3D_RENDERER->UnlockVb(vb);
                M3D_RENDERER->SetToStream0(vb);
                M3D_RENDERER->DrawPrimitive(rend::M3DPT_LINELIST, 0, mesh.m_numDrawVerts);
            }
            if (mesh.m_meshType == 1)
            {
                M3D_RENDERER->MatPopWorld();
            }
        }
        M3D_RENDERER->MatPop(true);
    }

    void AnimatedModel::AddSkin(unsigned CopyFrom)
    {
        // RVA 0x718AE0 - appends a copy of a skin (the first one when CopyFrom is out of range) with its own shaders
        // and textures loaded afresh.
        if (CopyFrom >= m_Skins.size())
        {
            CopyFrom = 0;
        }
        m_Skins.push_back(m_Skins[CopyFrom]);
        retruxx::vector<DSurfaceMaterial>& skin = m_Skins.back();
        for (unsigned i = 0; i < skin.size(); ++i)
        {
            DSurfaceMaterial& mat = skin[i];
            std::string ShaderFile("data/shaders/");
            ShaderFile += mat.Shader.Name;
            ShaderFile += ".fx";
            mat.Shader.Handle = M3D_RENDERER->NewEffect(ShaderFile.data(), true);
            for (unsigned k = 0; k < mat.Textures.size(); ++k)
            {
                DTextureInfo& tex = mat.Textures[k];
                CStr path = DefinePathToTexture(CStr(tex.FileName.data()));
                UnifyFileName(path);
                SetTexture(path, tex.Type, tex.Handle);
            }
        }
    }

    int AnimatedModel::GetLoadPointIdByName(char const* lpName) const
    {
        for (int i = 0; i < m_header.m_numNodes; ++i)
        {
            if (CStr(m_boneInitialPos[i].m_boneName) == lpName)
            {
                return m_boneInitialPos[i].m_ownIdx;
            }
        }
        return -1;
    }

    DRAFT_Geom const* AnimatedModel::GetGeom(unsigned num) const
    {
        if (num < m_Geoms.size())
        {
            return &m_Geoms[num];
        }
        return nullptr;
    }

    namespace
    {
        // The VertexType names, as the shaders' techniques list them.
        char const* const vertexTypesNames[] = {"VERTEX_XYZ", "VERTEX_XYZT1", "VERTEX_XYZC", "VERTEX_XYZWC",
            "VERTEX_XYZWCT1", "VERTEX_XYZNC", "VERTEX_XYZCT1", "VERTEX_XYZNT1", "VERTEX_XYZNCT1", "VERTEX_XYZNCT2",
            "VERTEX_XYZNT2", "VERTEX_XYZNT3", "VERTEX_XYZCT1_UVW", "VERTEX_XYZCT2_UVW", "VERTEX_XYZCT2",
            "VERTEX_XYZNT1T", "VERTEX_XYZNCT1T", "VERTEX_XYZNCT1_UV2_S1", "VERTEX_STREAM_UV_S1", "VERTEX_WATERTEST",
            "VERTEX_GRASSTEST", "VERTEX_IMPOSTORTEST", "VERTEX_YNI", "VERTEX_XYZT1I"};

        // Moves the part of `from` starting at the first of the characters '1', '2' or '3' into `suffix` (only
        // the first one found, trying them in that order).
        void SplitTexCoordSuffix(CStr& from, CStr& suffix)
        {
            for (char c = '1'; c <= '3'; ++c)
            {
                char const* const at = strchr(from.c_str(), c);
                if (at && (!suffix.c_str() || !strlen(suffix.c_str())))
                {
                    int const pos = static_cast<int>(at - from.c_str());
                    suffix = from.substr(pos);
                    int const length = from.c_str() ? static_cast<int>(strlen(from.c_str())) : 0;
                    from.del(pos, length - pos);
                }
            }
        }
    }  // namespace

    int AnimatedModel::ChangeShader(CStr const& Name, unsigned SkinNumber, unsigned i)
    {
        // RVA 0x713F80 - puts a new shader on material i of a skin, then checks that every technique of it can
        // draw every mesh using the material: the mesh's vertex format has to carry every component the
        // technique reads and at least as many texture coordinate sets. If not, the old shader goes back and 0 is
        // returned. NOTE: the skin number is not range-checked, and a material out of range counts as success.
        bool result = true;
        retruxx::vector<DSurfaceMaterial>& skin = m_Skins[SkinNumber];
        if (i >= skin.size())
        {
            return result;
        }
        DShader& shader = skin[i].Shader;
        CStr const OldName(shader.Name.data());
        if (shader.Handle)
        {
            shader.Handle->Release();
            shader.Handle = nullptr;
        }
        shader.Name = Name.c_str();
        NewEffect(shader.Name, shader.Handle);
        if (!shader.Handle)
        {
            result = false;
        }
        else
        {
            for (unsigned k = 0; k < shader.Handle->GetNumTechniques(); ++k)
            {
                for (unsigned j = 0; j < m_numMeshes; ++j)
                {
                    if (m_meshes[j].m_MaterialNumber != i)
                    {
                        continue;
                    }
                    rend::IEffect::TechniqueDesc const& desc = shader.Handle->GetTechniqueDesc(k);
                    CStr const meshType(vertexTypesNames[m_meshes[j].m_VertexType]);
                    CStr const shaderType(vertexTypesNames[desc.vertexFormat]);
                    char const* const meshXyz = strstr(meshType.c_str(), "XYZ");
                    int const meshPos = meshXyz ? static_cast<int>(meshXyz - meshType.c_str()) : -1;
                    char const* const shaderXyz = strstr(shaderType.c_str(), "XYZ");
                    int const shaderPos = shaderXyz ? static_cast<int>(shaderXyz - shaderType.c_str()) : -1;
                    if (meshPos == -1 || shaderPos == -1)
                    {
                        result = false;
                    }
                    // NOTE: both names are cut at the position of "XYZ" in the mesh's name.
                    CStr sub1 = meshType.substr(meshPos + 3);
                    CStr sub2 = shaderType.substr(meshPos + 3);
                    CStr suff1("");
                    CStr suff2("");
                    SplitTexCoordSuffix(sub2, suff2);
                    SplitTexCoordSuffix(sub1, suff1);
                    if (suff2.c_str() && strlen(suff2.c_str()))
                    {
                        if (!suff1.c_str() || !strlen(suff1.c_str()))
                        {
                            result = false;
                        }
                        if (suff1 < suff2)
                        {
                            result = false;
                        }
                    }
                    int const count = sub2.c_str() ? static_cast<int>(strlen(sub2.c_str())) : 0;
                    for (int m = 0; m < count; ++m)
                    {
                        if (!strchr(sub1.c_str(), sub2.c_str()[m]))
                        {
                            result = false;
                        }
                    }
                }
            }
            if (result)
            {
                return result;
            }
        }
        if (shader.Handle)
        {
            shader.Handle->Release();
            shader.Handle = nullptr;
        }
        shader.Name = OldName.c_str();
        NewEffect(shader.Name, shader.Handle);
        return result;
    }

    bool AnimatedModel::bIsPassable() const
    {
        return this->m_passable;
    }

    void AnimatedModel::FromGroupVariants(Configuration& cfg) const
    {
        // RVA 0x7F7E60: fold the per-group variant indices into a single mixed-radix
        // cfg.m_num (radix = each group's variant count), clamped to m_cfgSize - 1.
        unsigned acc = 0;
        unsigned stride = 1;
        for (int g = static_cast<int>(cfg.m_groupVariants.size()) - 1; g >= 0; --g)
        {
            acc += stride * cfg.m_groupVariants[g];
            stride *= m_MhGroups[g].m_variants.size();
        }
        unsigned const maxNum = m_cfgSize - 1;
        cfg.m_num = (acc <= maxNum) ? acc : maxNum;
    }

    void AnimatedModel::ReloadSkins(LoadSkins const& skinsToLoad)
    {
        if (skinsToLoad.loadAllSkins && m_loadSkins.loadAllSkins)
        {
            return;
        }

        for (int i = 0; i < m_Skins.size(); ++i)
        {
            // TODO: check this!!
            bool loadNewSkin = skinsToLoad.loadSkins.find(i) != skinsToLoad.loadSkins.end();
            bool unloadOldSkin = m_loadSkins.loadSkins.find(i) != m_loadSkins.loadSkins.end();

            if (skinsToLoad.loadAllSkins)
                loadNewSkin = true;

            if (this->m_loadSkins.loadAllSkins)
                unloadOldSkin = true;

            if (loadNewSkin)
            {
                if (!unloadOldSkin)
                {
                    LoadSkin(i);
                }
            }
            else if (unloadOldSkin)
            {
                UnloadSkin(i);
            }
        }

        m_loadSkins.loadSkins = skinsToLoad.loadSkins;
    }

    void AnimatedModel::CalculateMeshes(Configuration& cfg) const
    {
        cfg.m_meshes.clear();
        for (int i = 0; i < cfg.m_groupVariants.size(); ++i)
        {
            auto& group = m_MhGroups[i];
            auto& variants = group.m_variants[cfg.m_groupVariants[i]];
            for (int j = 0; j < variants.size(); ++j)
            {
                auto& mesh = m_meshes[group.MeshesId[variants[j]]];
                cfg.m_meshes.push_back(&mesh);
            }
        }
    }

    unsigned int AnimatedModel::GetNumAnimations() const
    {
        return m_header.m_numAnimations;
    }

    namespace
    {
        float const GEOM_PI_20 = 0.15707964f;
        float const GEOM_PI_10 = 0.31415927f;
        float const GEOM_PI_2 = 1.5707964f;
        float const GEOM_PI_4 = 0.78539819f;

        // v as a row vector through the rotation part of q's matrix.
        CVector RotateByQuaternion(Quaternion const& q, CVector const& v)
        {
            CMatrix const m = q.ToMatrix();
            return CVector(v.x * m._11 + v.y * m._21 + v.z * m._31, v.x * m._12 + v.y * m._22 + v.z * m._32,
                v.x * m._13 + v.y * m._23 + v.z * m._33);
        }

        // The rotation about Z (as a quaternion) by step * PI_10.
        Quaternion CircleStep(int step)
        {
            double const half = static_cast<double>(step) * GEOM_PI_10 * 0.5;
            return Quaternion(0.0f, 0.0f, static_cast<float>(sin(half)), static_cast<float>(cos(half)));
        }

        // A collision primitive's placement: its rotation (a cylinder, modelled along Z, is first turned onto Y)
        // and then its translation.
        CMatrix DraftGeomMatrix(DRAFT_Geom const& geom)
        {
            Quaternion rotation = geom.Rotation;
            if (geom.Type == CYLINDER)
            {
                Quaternion const turn(static_cast<float>(sin(static_cast<double>(GEOM_PI_4))), 0.0f, 0.0f,
                    static_cast<float>(cos(static_cast<double>(GEOM_PI_4))));
                rotation = geom.Rotation * turn;
            }
            CMatrix translation;
            translation.translation(geom.Translation);
            return rotation.ToMatrix() * translation;
        }

        // The primitive in the current world matrix: a sphere as 20x20 latitude/longitude quads, a cylinder as
        // 20-segment caps and sides, a box as its outline.
        void DrawDraftGeom(DRAFT_Geom const& geom, unsigned clr)
        {
            if (geom.Type == SPHERE)
            {
                float const radius = geom.Sizes.BoxSizes.x;
                for (int h = 0; h < 20; ++h)
                {
                    for (int v = 0; v < 20; ++v)
                    {
                        CVector corners[4];
                        for (int a = 0; a <= 1; ++a)
                        {
                            double const lat = (static_cast<double>(a + v) * GEOM_PI_20 - GEOM_PI_2) * 0.5;
                            Quaternion const q2(0.0f, 0.0f, static_cast<float>(sin(lat)), static_cast<float>(cos(lat)));
                            for (int b = 0; b <= 1; ++b)
                            {
                                double const lon = static_cast<double>(b + h) * GEOM_PI_10 * 0.5;
                                Quaternion const q1(
                                    0.0f, static_cast<float>(sin(lon)), 0.0f, static_cast<float>(cos(lon)));
                                corners[2 * a + b] = RotateByQuaternion(q1 * q2, CVector(radius, 0.0f, 0.0f));
                            }
                        }
                        CVector tri[3] = {corners[0], corners[1], corners[2]};
                        M3D_APP->DrawTri(tri, clr);
                        tri[0] = corners[3];
                        tri[1] = corners[2];
                        tri[2] = corners[1];
                        M3D_APP->DrawTri(tri, clr);
                    }
                }
            }
            if (geom.Type == CYLINDER)
            {
                float const radius = geom.Sizes.BoxSizes.x;
                float const top = geom.Sizes.BoxSizes.y * 0.5f;
                float const bottom = geom.Sizes.BoxSizes.y * -0.5f;
                CVector tri[4];
                for (int k = 0; k < 20; ++k)
                {
                    tri[0] = CVector(0.0f, 0.0f, top);
                    for (int b = 0; b <= 1; ++b)
                    {
                        tri[1 + b] = RotateByQuaternion(CircleStep(b + k), CVector(radius, 0.0f, top));
                    }
                    M3D_APP->DrawTri(tri, clr);
                    tri[0].z = bottom;
                    tri[1].z = bottom;
                    tri[2].z = bottom;
                    M3D_APP->DrawTri(tri, clr);
                }
                for (int k = 0; k < 20; ++k)
                {
                    for (int b = 0; b <= 1; ++b)
                    {
                        CVector const p = RotateByQuaternion(CircleStep(b + k), CVector(radius, 0.0f, top));
                        tri[b] = p;
                        tri[b + 2] = CVector(p.x, p.y, bottom);
                    }
                    M3D_APP->DrawTri(tri, clr);
                    M3D_APP->DrawTri(&tri[1], clr);
                }
            }
            if (geom.Type == BOX)
            {
                float const hx = geom.Sizes.BoxSizes.x * 0.5f;
                float const hy = geom.Sizes.BoxSizes.y * 0.5f;
                float const hz = geom.Sizes.BoxSizes.z * 0.5f;
                Aabb aabb;
                aabb.m_box[0] = 0.0f - hx;
                aabb.m_box[1] = 0.0f - hy;
                aabb.m_box[2] = 0.0f - hz;
                aabb.m_box[3] = hx;
                aabb.m_box[4] = hy;
                aabb.m_box[5] = hz;
                aabb.Draw(clr);
            }
        }

        // The render state both collision views draw in: wireframe, no culling, no depth test, no textures.
        void BeginGeomsDraw(CMatrix const& matT)
        {
            M3D_RENDERER->MatPush(matT);
            M3D_RENDERER->PushFillMode(rend::M3DFILL_WIREFRAME);
            M3D_RENDERER->SetCull(rend::M3DCULL_NONE, false);
            M3D_RENDERER->PushZbState(rend::ZB_DISABLE);
            M3D_RENDERER->SetTexture(0, rend::TexHandle(), -1.0);
            M3D_RENDERER->SetTexture(1, rend::TexHandle(), -1.0);
        }

        void EndGeomsDraw()
        {
            M3D_RENDERER->MatPop(true);
            M3D_RENDERER->PopFillMode();
            M3D_RENDERER->PopZbState();
        }
    }  // namespace

    void AnimatedModel::RenderHierGeoms(CMatrix const& matT, AnimInfo* ai, unsigned clr)
    {
        // RVA 0x70ED60 - the collision primitives attached to bones, in the bones' current placement.
        BeginGeomsDraw(matT);
        for (unsigned j = 0; j < m_HierGeoms.size(); ++j)
        {
            DRAFT_HierGeom const& hierGeom = m_HierGeoms[j];
            CMatrix const parent =
                ai->m_bonesAnim ? ai->m_bonesAnim[hierGeom.parentBone].m_curMatrix : GetBoneMatrix(hierGeom.parentBone);
            M3D_RENDERER->MatPushWorld();
            M3D_RENDERER->MatSetWorld(DraftGeomMatrix(hierGeom.geom) * parent);
            DrawDraftGeom(hierGeom.geom, clr);
            M3D_RENDERER->MatPopWorld();
        }
        EndGeomsDraw();
    }

    int AnimatedModel::ChangeTexture(CStr const& name, unsigned SkinNumber, unsigned MatNum, unsigned TexNum)
    {
        // RVA 0x713ED0
        DTextureInfo& tex = m_Skins[SkinNumber][MatNum].Textures[TexNum];
        M3D_RENDERER->ReleaseTexture(tex.Handle);
        SetTexture(name, tex.Type, tex.Handle);
        tex.FileName = NameFromFileName(name).c_str();
        return tex.Handle.IsValid();
    }

    int AnimatedModel::Render(CMatrix const& matT, AnimInfo* ai, unsigned SkinNum, bool setShaderParams)
    {
        // RVA 0x714EA0: draw every mesh of the model, no configuration filtering.
        M3D_RENDERER->MatPush(matT);
        for (unsigned i = 0; i < m_numMeshes; ++i)
        {
            Mesh& mesh = m_meshes[i];
            if (mesh.m_numNode < 0)
            {
                continue;
            }
            rend::IEffect* shader = ApplyMaterial(mesh.GetMaterial(SkinNum));
            if (setShaderParams)
            {
                SetShaderParams(shader);
            }
            RenderMesh(i, ai, shader);
        }
        M3D_RENDERER->MatPop(true);
        return 1;
    }

    int AnimatedModel::Render(CMatrix const& matT, AnimInfo* ai, Configuration const& cfg, unsigned SkinNum)
    {
        // RVA 0x715070: draw only the meshes the configuration selected.
        M3D_RENDERER->MatPush(matT);
        for (Mesh* mesh : cfg.m_meshes)
        {
            if (mesh->m_numNode < 0)
            {
                continue;
            }
            DSurfaceMaterial& material = (mesh->m_MaterialNumber < 0) ? m_Skins[0][0] : mesh->GetMaterial(SkinNum);
            rend::IEffect* shader = ApplyMaterial(material);
            SetShaderParams(shader);
            RenderMesh(mesh->meshId, ai, shader);
        }
        M3D_RENDERER->MatPop(true);
        return 1;
    }

    int AnimatedModel::Render(
        CMatrix const& matT,
        AnimInfo* ai,
        Configuration const& cfg,
        retruxx::vector<DSurfaceMaterial*, retruxx::allocator<DSurfaceMaterial*>> const& MeshMaterials)
    {
        // RVA 0x714FA0: as above, but the caller supplies a per-mesh material
        // override table (indexed by mesh id) that wins when it is not empty.
        M3D_RENDERER->MatPush(matT);
        for (Mesh* mesh : cfg.m_meshes)
        {
            unsigned const meshId = mesh->meshId;
            if (mesh->m_numNode < 0)
            {
                continue;
            }
            DSurfaceMaterial* material;
            if (!MeshMaterials.empty())
            {
                material = MeshMaterials[meshId];
            }
            else if (mesh->m_MaterialNumber < 0)
            {
                material = &m_Skins[0][0];
            }
            else
            {
                material = &mesh->GetMaterial(0);
            }
            rend::IEffect* shader = ApplyMaterial(*material);
            SetShaderParams(shader);
            RenderMesh(meshId, ai, shader);
        }
        M3D_RENDERER->MatPop(true);
        return 1;
    }

    int AnimatedModel::GetFps(int anim) const
    {
        // RVA 0x76DB30
        int const remapped = m_animRemap[anim];
        if (remapped >= 0 && remapped < m_header.m_numAnimations)
        {
            return m_animations[remapped].m_fps;
        }
        return 0;
    }

    void AnimatedModel::SetFps(int anim, short fps)
    {
        // RVA 0x801D60
        int const remapped = m_animRemap[anim];
        if (remapped >= 0 && remapped < m_header.m_numAnimations)
        {
            m_animations[remapped].m_fps = fps;
        }
    }

    void AnimatedModel::RenderLoadPoints(CMatrix const& matT, AnimInfo* ai, Configuration const& cfg)
    {
        // RVA 0x70E730 - the load points (bones named "LP_...") shown by the configuration: a load point is left
        // out when any mesh hanging on its parent bone is not in the configuration. Each gets its name and its axes
        // (the matrix rows, half length).
        M3D_RENDERER->MatPush(matT);
        M3D_RENDERER->PushZbState(rend::ZB_DISABLE);
        M3D_RENDERER->PushLighting(false);
        M3D_RENDERER->PushBlend(rend::BM_NONE);
        SetNoTexture();
        for (unsigned i = 0; i < static_cast<unsigned>(m_header.m_numNodes); ++i)
        {
            Bone const b = m_boneInitialPos[i];
            bool visible = true;
            for (unsigned j = 0; j < m_numMeshes; ++j)
            {
                if (m_meshes[j].m_numNode == b.m_parentIdx &&
                    std::find(cfg.m_meshes.begin(), cfg.m_meshes.end(), &m_meshes[j]) == cfg.m_meshes.end())
                {
                    visible = false;
                    break;
                }
            }
            if (strstr(b.m_boneName, "LP_") != b.m_boneName || !visible)
            {
                continue;
            }
            CMatrix const m = ai->m_bonesAnim ? ai->m_bonesAnim[i].m_curMatrix : GetBoneMatrix(i);
            CVector const org(m._41, m._42, m._43);
            CVector const x(m._11 * 0.5f, m._12 * 0.5f, m._13 * 0.5f);
            CVector const y(m._21 * 0.5f, m._22 * 0.5f, m._23 * 0.5f);
            CVector const z(m._31 * 0.5f, m._32 * 0.5f, m._33 * 0.5f);
            DrawBoneName(org, m_boneInitialPos[i].m_boneName);
            M3D_RENDERER->PushBlend(rend::BM_NONE);
            M3D_RENDERER->SetAlphaTest(false);
            SetNoTexture();
            DrawAxes(org, x, y, z);
            M3D_RENDERER->PopBlend();
        }
        M3D_RENDERER->MatPop(true);
        M3D_RENDERER->PopZbState();
        M3D_RENDERER->PopLighting();
        M3D_RENDERER->PopBlend();
    }

    DSurfaceMaterial& AnimatedModel::GetMeshMaterial(unsigned MeshNum, unsigned SkinNumber)
    {
        // RVA 0x7B6510
        return m_Skins[SkinNumber][m_meshes[MeshNum].m_MaterialNumber];
    }

    void AnimatedModel::AddTexture(
        unsigned SkinNumber, unsigned MatNum, CStr const& Name, DRAFT_TextureType TexType, unsigned UVSet)
    {
        // RVA 0x7171D0 - the texture is loaded from the name as given; only its file name is stored.
        retruxx::vector<DTextureInfo>& textures = m_Skins[SkinNumber][MatNum].Textures;
        textures.push_back(DTextureInfo());
        DTextureInfo& tex = textures.back();
        tex.FileName = NameFromFileName(Name).c_str();
        tex.Type = TexType;
        tex.UV_Set = UVSet;
        tex.Handle = M3D_RENDERER->AddTexture(Name, 2);
    }

    DCollisionData const& AnimatedModel::GetCollisionTrimesh() const
    {
        return this->m_Collision;
    }

    int AnimatedModel::GetBoneMatrixByName(CStr const& boneName, CMatrix& res, bool theLastOneOnly) const
    {
        // TODO: generated code
        // Initialize result matrix to identity
        res.identity();

        int numNodes = m_header.m_numNodes;
        if (numNodes <= 0)
            return false;

        // Find bone by name
        int boneIndex = -1;
        for (int i = 0; i < numNodes; i++)
        {
            if (m_boneInitialPos[i].m_boneName == boneName)
            {
                boneIndex = i;
                break;
            }
        }

        if (boneIndex == -1)
            return false;

        // Get the bone data
        Bone const& bone = m_boneInitialPos[boneIndex];

        // Convert quaternion to rotation matrix
        float x = bone.m_quaternion0.x;
        float y = bone.m_quaternion0.y;
        float z = bone.m_quaternion0.z;
        float w = bone.m_quaternion0.w;

        float x2 = x * x;
        float y2 = y * y;
        float z2 = z * z;
        float xy = x * y;
        float xz = x * z;
        float yz = y * z;
        float wx = w * x;
        float wy = w * y;
        float wz = w * z;

        // Set rotation matrix from quaternion
        res._11 = 1.0f - 2.0f * (y2 + z2);
        res._12 = 2.0f * (xy + wz);
        res._13 = 2.0f * (xz - wy);
        res._14 = 0.0f;

        res._21 = 2.0f * (xy - wz);
        res._22 = 1.0f - 2.0f * (x2 + z2);
        res._23 = 2.0f * (yz + wx);
        res._24 = 0.0f;

        res._31 = 2.0f * (xz + wy);
        res._32 = 2.0f * (yz - wx);
        res._33 = 1.0f - 2.0f * (x2 + y2);
        res._34 = 0.0f;

        res._41 = bone.m_translation0.x;
        res._42 = bone.m_translation0.y;
        res._43 = bone.m_translation0.z;
        res._44 = 1.0f;

        // If we only want this bone, return now
        if (theLastOneOnly)
            return true;

        // Apply parent transformations up the hierarchy
        int parentIndex = bone.m_parentIdx;
        while (parentIndex >= 0)
        {
            Bone const& parentBone = m_boneInitialPos[parentIndex];

            // Convert parent quaternion to rotation matrix
            float px = parentBone.m_quaternion0.x;
            float py = parentBone.m_quaternion0.y;
            float pz = parentBone.m_quaternion0.z;
            float pw = parentBone.m_quaternion0.w;

            float px2 = px * px;
            float py2 = py * py;
            float pz2 = pz * pz;
            float pxy = px * py;
            float pxz = px * pz;
            float pyz = py * pz;
            float pwx = pw * px;
            float pwy = pw * py;
            float pwz = pw * pz;

            // Create parent rotation matrix
            CMatrix parentRot;
            parentRot._11 = 1.0f - 2.0f * (py2 + pz2);
            parentRot._12 = 2.0f * (pxy + pwz);
            parentRot._13 = 2.0f * (pxz - pwy);
            parentRot._14 = 0.0f;

            parentRot._21 = 2.0f * (pxy - pwz);
            parentRot._22 = 1.0f - 2.0f * (px2 + pz2);
            parentRot._23 = 2.0f * (pyz + pwx);
            parentRot._24 = 0.0f;

            parentRot._31 = 2.0f * (pxz + pwy);
            parentRot._32 = 2.0f * (pyz - pwx);
            parentRot._33 = 1.0f - 2.0f * (px2 + py2);
            parentRot._34 = 0.0f;

            parentRot._41 = parentBone.m_translation0.x;
            parentRot._42 = parentBone.m_translation0.y;
            parentRot._43 = parentBone.m_translation0.z;
            parentRot._44 = 1.0f;

            // Combine matrices: result = parentRot * currentResult
            CMatrix tempResult;

            // Row 1
            tempResult._11 =
                parentRot._11 * res._11 + parentRot._12 * res._21 + parentRot._13 * res._31 + parentRot._14 * res._41;
            tempResult._12 =
                parentRot._11 * res._12 + parentRot._12 * res._22 + parentRot._13 * res._32 + parentRot._14 * res._42;
            tempResult._13 =
                parentRot._11 * res._13 + parentRot._12 * res._23 + parentRot._13 * res._33 + parentRot._14 * res._43;
            tempResult._14 =
                parentRot._11 * res._14 + parentRot._12 * res._24 + parentRot._13 * res._34 + parentRot._14 * res._44;

            // Row 2
            tempResult._21 =
                parentRot._21 * res._11 + parentRot._22 * res._21 + parentRot._23 * res._31 + parentRot._24 * res._41;
            tempResult._22 =
                parentRot._21 * res._12 + parentRot._22 * res._22 + parentRot._23 * res._32 + parentRot._24 * res._42;
            tempResult._23 =
                parentRot._21 * res._13 + parentRot._22 * res._23 + parentRot._23 * res._33 + parentRot._24 * res._43;
            tempResult._24 =
                parentRot._21 * res._14 + parentRot._22 * res._24 + parentRot._23 * res._34 + parentRot._24 * res._44;

            // Row 3
            tempResult._31 =
                parentRot._31 * res._11 + parentRot._32 * res._21 + parentRot._33 * res._31 + parentRot._34 * res._41;
            tempResult._32 =
                parentRot._31 * res._12 + parentRot._32 * res._22 + parentRot._33 * res._32 + parentRot._34 * res._42;
            tempResult._33 =
                parentRot._31 * res._13 + parentRot._32 * res._23 + parentRot._33 * res._33 + parentRot._34 * res._43;
            tempResult._34 =
                parentRot._31 * res._14 + parentRot._32 * res._24 + parentRot._33 * res._34 + parentRot._34 * res._44;

            // Row 4
            tempResult._41 =
                parentRot._41 * res._11 + parentRot._42 * res._21 + parentRot._43 * res._31 + parentRot._44 * res._41;
            tempResult._42 =
                parentRot._41 * res._12 + parentRot._42 * res._22 + parentRot._43 * res._32 + parentRot._44 * res._42;
            tempResult._43 =
                parentRot._41 * res._13 + parentRot._42 * res._23 + parentRot._43 * res._33 + parentRot._44 * res._43;
            tempResult._44 =
                parentRot._41 * res._14 + parentRot._42 * res._24 + parentRot._43 * res._34 + parentRot._44 * res._44;

            // Copy temp result back to result
            res = tempResult;

            // Move up hierarchy
            parentIndex = parentBone.m_parentIdx;
        }

        return true;
    }

    void AnimatedModel::FromCfgNum(Configuration& cfg) const
    {
        auto v2 = this->m_cfgSize - 1;
        if (cfg.m_num > v2)
            cfg.m_num = v2;

        auto cfgSize = m_cfgSize;
        auto num = cfg.m_num;
        cfg.m_groupVariants.resize(m_MhGroups.size());
        for (int i = 0; i < cfg.m_groupVariants.size(); ++i)
        {
            auto& group = m_MhGroups[i];
            auto variantsSize = group.m_variants.size();
            auto size = cfgSize / variantsSize;

            cfg.m_groupVariants[i] = num / size;
            cfgSize = size;
            num = num % size;
        }
    }

    char const* AnimatedModel::GetName() const
    {
        return m_Name.c_str();
    }
    char const* AnimatedModel::GetPath() const
    {
        return m_PathToFile.c_str();
    }

    Aabb& AnimatedModel::GetAabb()
    {
        // RVA 0x6ABF50
        return m_box;
    }

    int AnimatedModel::Update(AnimInfo* ai, bool notFirstTime, Configuration* cfg)
    {
        if (!ai->IsEmpty())
        {
            auto const curFrame = m3d::g_Kernel->GetTimer().GetCurFrame();
            ai->InterpolateBones(curFrame);
            for (int i = 0; i < this->m_header.m_numNodes; ++i)
            {
                MatrixForBone(ai, curFrame, i);
            }
            UpdateVertices(ai, notFirstTime, cfg);
        }
        return 1;
    }

    void AnimatedModel::UpdateCubemap()
    {
        // RVA 0x77F780 - points every cubemap texture at the current shared cubemap, reloading it for the skins
        // that are loaded.
        if (m_hasCubemap)
        {
            for (unsigned j = 0; j < m_Skins.size(); ++j)
            {
                retruxx::vector<DSurfaceMaterial>& skin = m_Skins[j];
                bool const load =
                    m_loadSkins.loadAllSkins || m_loadSkins.loadSkins.find(j) != m_loadSkins.loadSkins.end();
                for (unsigned i = 0; i < skin.size(); ++i)
                {
                    for (unsigned k = 0; k < skin[i].Textures.size(); ++k)
                    {
                        DTextureInfo& tex = skin[i].Textures[k];
                        if (tex.Type == CUBEMAP && m_cubemapTexName != tex.FileName.data())
                        {
                            M3D_RENDERER->ReleaseTexture(tex.Handle);
                            tex.FileName = m_cubemapTexName.c_str();
                            tex.Handle.SetInvalid();
                            if (load)
                            {
                                SetTexture(m_cubemapTexPath, CUBEMAP, tex.Handle);
                            }
                        }
                    }
                }
            }
        }
    }

    unsigned AnimatedModel::GetNumGeoms() const
    {
        return m_Geoms.size();
    }

    unsigned AnimatedModel::GetCfgSize() const
    {
        return this->m_cfgSize;
    }

    MeshesGroup const& AnimatedModel::GetGroup(unsigned Num) const
    {
        return this->m_MhGroups[Num];
    }

    DSurfaceMaterial& AnimatedModel::GetMaterial(unsigned SkinNumber, unsigned MatNum)
    {
        return this->m_Skins[SkinNumber][MatNum];
    }

    rend::IEffect* AnimatedModel::ApplyMaterial(DSurfaceMaterial& material)
    {
        if (!material.Shader.Handle)
        {
            M3D_RENDERER->MaterialSet(material.material);
            for (int i = 0; i < material.Textures.size(); ++i)
            {
                if (material.Textures[i].Handle.IsValid())
                {
                    M3D_RENDERER->SetTexture(i, material.Textures[i].Handle, -1.0);
                    M3D_RENDERER->TgSetTcSource(i, rend::TC_FROM_VERTEX, material.Textures[i].UV_Set);

                    auto typ = material.Textures[i].Type;
                    if (typ)
                    {
                        if (typ == LIGHTMAP)
                        {
                            M3D_RENDERER->SetStageState(i, rend::BM_COLOR, rend::TS_NONE);
                        }
                        else
                        {
                            M3D_RENDERER->SetStageState(i, rend::BM_COLOR, rend::TS_TEX_MODULATE2X_PREV);
                        }
                    }
                    else
                    {
                        M3D_RENDERER->SetStageState(i, rend::BM_COLOR, rend::TS_MODULATE);
                    }
                }
            }
            return material.Shader.Handle;
        }

        M3D_RENDERER->ResetTextureStates();
        for (int i = 0; i < material.Textures.size(); ++i)
        {
            if (material.Textures[i].Handle.IsValid())
            {
                switch (material.Textures[i].Type)
                {
                case DIFFUSE:
                    material.Shader.Handle->SetTexture(rend::IEffect::DiffMap0, &material.Textures[i].Handle);
                    break;

                case BUMP:
                    material.Shader.Handle->SetTexture(rend::IEffect::BumpMap0, &material.Textures[i].Handle);
                    break;

                case LIGHTMAP:
                    material.Shader.Handle->SetTexture(rend::IEffect::LightMap0, &material.Textures[i].Handle);
                    break;

                case CUBEMAP:
                    material.Shader.Handle->SetTexture(rend::IEffect::CubeMap0, &material.Textures[i].Handle);
                    break;

                case DETAIL:
                    material.Shader.Handle->SetTexture(rend::IEffect::DetailMap0, &material.Textures[i].Handle);
                    break;
                }
            }
        }

        return material.Shader.Handle;
    }

    rend::IEffect* AnimatedModel::ApplyMaterial(unsigned meshnumber)
    {
        // RVA 0x713E80
        Mesh& mesh = m_meshes[meshnumber];
        if (mesh.m_MaterialNumber < 0)
        {
            return ApplyMaterial(m_Skins[0][0]);
        }
        return ApplyMaterial(mesh.GetMaterial(0));
    }

    bool AnimatedModel::Save(CStr const& FileName)
    {
        // RVA 0x780180 - writes the model back as a SAM ("DFT") file, the chunks LoadSAM reads. The vertices are
        // written as they are held (m_verts), the influences as bone and weight only, scale as 1.
        // NOTE: every skin writes as many materials as the first one has.
        fs::auxTaggedFile File;
        if (File.Open(FileName.c_str(), fs::auxTaggedFile::CREATE_IGNORE_CRC))
        {
            M3D_LOG_INFO(CStr("AnimatedModel::Save -- cannot open file ") + FileName);
            return false;
        }
        File.setFormatTitle("DFT");
        File.setFormatVersion(1u);

        DRAFT_Header Header;
        Header.NumberOfAnimations = m_header.m_numAnimations;
        Header.NumberOfBones = m_header.m_numNodes;
        Header.NumberOfMaterials = static_cast<unsigned int>(m_Skins[0].size());
        Header.NumberOfMeshes = m_numMeshes;
        File.addChunk(6u);
        File.addChunkDataCopy(6u, sizeof(Header), &Header);

        // NOTE: the records below are built in buffers only filled up to each string's terminator; the bytes after
        // it (uninitialized in the shipped build) are zero here.
        File.addChunk(3u);
        for (int i = 0; i < m_header.m_numNodes; ++i)
        {
            Bone const& bone = m_boneInitialPos[i];
            DRAFT_Bone record;
            memset(&record, 0, sizeof(record));
            strcpy(record.Name, bone.m_boneName);
            record.ParentIndex = static_cast<__int16>(bone.m_parentIdx);
            record.Tx = bone.m_translation0.x;
            record.Ty = bone.m_translation0.y;
            record.Tz = bone.m_translation0.z;
            record.Rx = bone.m_quaternion0.x;
            record.Ry = bone.m_quaternion0.y;
            record.Rz = bone.m_quaternion0.z;
            record.Rw = bone.m_quaternion0.w;
            record.Sx = 1.0f;
            record.Sy = 1.0f;
            record.Sz = 1.0f;
            File.addChunkDataCopy(3u, sizeof(record), &record);
        }

        File.addChunk(1u);
        for (unsigned i = 0; i < m_numMeshes; ++i)
        {
            Mesh& mesh = m_meshes[i];
            DRAFT_GeometryHeader MeshHeader;
            MeshHeader.MaterialIndex = static_cast<__int16>(mesh.m_MaterialNumber);
            MeshHeader.Type = static_cast<DRAFT_MeshType>(mesh.m_meshType);
            MeshHeader.ParentBone = static_cast<__int16>(mesh.m_numNode);
            MeshHeader.TriangleCount = mesh.m_numFaces;
            MeshHeader.VertexCount = mesh.m_numVertices;
            MeshHeader.NumberOfVertexComponents = static_cast<unsigned int>(mesh.m_VCHs.size());
            File.addChunkDataCopy(1u, sizeof(MeshHeader), &MeshHeader);
            File.addChunkDataCopy(
                1u, static_cast<unsigned>(sizeof(DRAFT_VertexComponent) * mesh.m_VCHs.size()), mesh.m_VCHs.data());
            for (unsigned c = 0; c < mesh.m_VCHs.size(); ++c)
            {
                DRAFT_VertexComponent const& comp = mesh.m_VCHs[c];
                if (comp.Size > 0)
                {
                    unsigned char const* vert = static_cast<unsigned char const*>(mesh.m_verts);
                    for (int v = 0; v < mesh.m_numVertices; ++v, vert += mesh.m_VertexTypeSize)
                    {
                        File.addChunkDataCopy(1u, comp.Size, vert + CompOff(mesh.m_VCHs, comp.Type));
                    }
                    continue;
                }
                if (comp.Type != INFLUENCES)
                {
                    // NOTE: the file is abandoned without being closed.
                    M3D_LOG_ERR(CStr("Unknown  component of vertex "));
                    return false;
                }
                for (int v = 0; v < mesh.m_numVertices; ++v)
                {
                    Influences const& inf = mesh.m_vertsInfluences[v];
                    unsigned int const NumBones = inf.m_numBones;
                    File.addChunkDataCopy(1u, sizeof(NumBones), &NumBones);
                    for (unsigned int b = 0; b < NumBones; ++b)
                    {
                        File.addChunkDataCopy(1u, sizeof(short), &inf.m_influences[b].m_boneIdx);
                        File.addChunkDataCopy(1u, sizeof(float), &inf.m_influences[b].m_boneWeight);
                    }
                }
            }
            File.addChunkDataCopy(1u, sizeof(Index3) * mesh.m_numFaces, mesh.m_tris);
        }

        File.addChunk(4u);
        for (int a = 0; a < m_header.m_numAnimations; ++a)
        {
            Animation const& anim = m_animations[a];
            DRAFT_AnimationHeader info;
            memset(&info, 0, sizeof(info));
            strcpy(info.Name, anim.m_name);
            info.FPS_Number = anim.m_fps;
            info.FramesNumber = anim.m_numFrames;
            info.NextAnimation = anim.m_nextAnimation;
            info.NumberOfChanges = anim.m_numChanges;
            File.addChunkDataCopy(4u, sizeof(info), &info);
            for (int c = 0; c < anim.m_numChanges; ++c)
            {
                DRAFT_HierarchyChange change;
                change.Type = anim.m_hierChanges[c].changeType;
                change.Index = anim.m_hierChanges[c].ownIdx;
                change.NewParent = anim.m_hierChanges[c].newParentIdx;
                File.addChunkDataCopy(4u, sizeof(change), &change);
            }
            int n = 0;
            for (int f = 0; f < anim.m_numFrames; ++f)
            {
                for (int b = 0; b < m_header.m_numNodes; ++b, ++n)
                {
                    AnimationTransform const& src = anim.m_nodesPositions[n];
                    DRAFT_Transform t;
                    t.Tx = src.tx;
                    t.Ty = src.ty;
                    t.Tz = src.tz;
                    t.Rx = src.qx;
                    t.Ry = src.qy;
                    t.Rz = src.qz;
                    t.Rw = src.qw;
                    t.Sx = 1.0f;
                    t.Sy = 1.0f;
                    t.Sz = 1.0f;
                    File.addChunkDataCopy(4u, sizeof(t), &t);
                }
            }
        }

        File.addChunk(8u);
        unsigned int const SkinNumber = static_cast<unsigned int>(m_Skins.size());
        File.addChunkDataCopy(8u, sizeof(SkinNumber), &SkinNumber);
        for (unsigned i = 0; i < m_Skins.size(); ++i)
        {
            for (unsigned j = 0; j < m_Skins[0].size(); ++j)
            {
                DSurfaceMaterial const& mat = m_Skins[i][j];
                File.addChunkDataCopy(8u, sizeof(rend::Material), &mat.material);
                unsigned int const TexturesNum = static_cast<unsigned int>(mat.Textures.size());
                File.addChunkDataCopy(8u, sizeof(TexturesNum), &TexturesNum);
                unsigned int const ShaderLength = static_cast<unsigned int>(mat.Shader.Name.size());
                File.addChunkDataCopy(8u, sizeof(ShaderLength), &ShaderLength);
                File.addChunkDataCopy(8u, ShaderLength, mat.Shader.Name.data());
                for (unsigned k = 0; k < TexturesNum; ++k)
                {
                    DRAFT_TextureInfo Texture;
                    memset(&Texture, 0, sizeof(Texture));
                    strcpy(Texture.FileName, mat.Textures[k].FileName.data());
                    Texture.UVSet = mat.Textures[k].UV_Set;
                    Texture.Type = mat.Textures[k].Type;
                    File.addChunkDataCopy(8u, sizeof(Texture), &Texture);
                }
            }
        }

        if (!m_Collision.Points.empty())
        {
            File.addChunk(5u);
            unsigned int const PointsNum = static_cast<unsigned int>(m_Collision.Points.size());
            unsigned int const PolygonsNum = static_cast<unsigned int>(m_Collision.Triangles.size());
            File.addChunkDataCopy(5u, sizeof(PointsNum), &PointsNum);
            File.addChunkDataCopy(5u, sizeof(PolygonsNum), &PolygonsNum);
            File.addChunkDataCopy(5u, sizeof(CVector) * PointsNum, m_Collision.Points.data());
            File.addChunkDataCopy(5u, sizeof(Index3) * PolygonsNum, m_Collision.Triangles.data());
        }
        if (!m_Geoms.empty())
        {
            File.addChunk(7u);
            unsigned int const GeomsNum = static_cast<unsigned int>(m_Geoms.size());
            File.addChunkDataCopy(7u, sizeof(GeomsNum), &GeomsNum);
            File.addChunkDataCopy(7u, sizeof(DRAFT_Geom) * GeomsNum, m_Geoms.data());
        }
        if (!m_HierGeoms.empty())
        {
            File.addChunk(10u);
            unsigned int const GeomsNum = static_cast<unsigned int>(m_HierGeoms.size());
            File.addChunkDataCopy(10u, sizeof(GeomsNum), &GeomsNum);
            File.addChunkDataCopy(10u, sizeof(DRAFT_HierGeom) * GeomsNum, m_HierGeoms.data());
        }
        if (!m_BonesBounds.empty())
        {
            File.addChunk(11u);
            unsigned int const BonesBoundsCount = static_cast<unsigned int>(m_BonesBounds.size());
            File.addChunkDataCopy(11u, sizeof(BonesBoundsCount), &BonesBoundsCount);
            for (auto const& bounds : m_BonesBounds)
            {
                File.addChunkDataCopy(11u, sizeof(DRAFT_BoneBounds), &bounds.second);
            }
        }

        unsigned int const GroupsNum = static_cast<unsigned int>(m_MhGroups.size());
        File.addChunk(9u);
        File.addChunkDataCopy(9u, sizeof(GroupsNum), &GroupsNum);
        for (MeshesGroup const& group : m_MhGroups)
        {
            // NOTE: a name of 20 characters or more overruns the buffer.
            char GroupName[20];
            memset(GroupName, 0, sizeof(GroupName));
            strcpy(GroupName, group.Name.data());
            unsigned int const MeshesNum = static_cast<unsigned int>(group.MeshesId.size());
            File.addChunkDataCopy(9u, sizeof(GroupName), GroupName);
            File.addChunkDataCopy(9u, sizeof(int), &group.VisibleAtOnceMax);
            File.addChunkDataCopy(9u, sizeof(int), &group.VisibleAtOnceMin);
            File.addChunkDataCopy(9u, sizeof(MeshesNum), &MeshesNum);
            for (unsigned k = 0; k < MeshesNum; ++k)
            {
                File.addChunkDataCopy(9u, sizeof(unsigned int), &group.MeshesId[k]);
            }
        }
        File.Close();
        return true;
    }

    unsigned AnimatedModel::GetNumBones() const
    {
        // RVA 0x6CEE60
        return m_header.m_numNodes;
    }

    void AnimatedModel::UpdateVertices(AnimInfo* ai, bool notFirstTime, Configuration* cfg)
    {
        if (cfg)
        {
            for (auto& mesh : cfg->m_meshes)
            {
                if (mesh->m_numNode != -1 && mesh->m_meshType == 2)
                {
                    VertsForSkinmesh(ai, *mesh, ai->m_meshesVerts[mesh->meshId], notFirstTime);
                }
            }
        }
        else
        {
            for (int i = 0; i < m_numMeshes; ++i)
            {
                auto& mesh = m_meshes[i];
                if (mesh.m_numNode != -1 && mesh.m_meshType == 2)
                {
                    VertsForSkinmesh(ai, mesh, ai->m_meshesVerts[i], notFirstTime);
                }
            }
        }
    }

    void AnimatedModel::RenderMesh(unsigned cc, AnimInfo* ai, rend::IEffect* Shader)
    {
        // RVA 0x713C80: bind the mesh's vertex/index source, then draw it.
        Mesh& mesh = m_meshes[cc];
        unsigned baseVertex = 0;
        bool bindIndices = true;

        if (mesh.m_meshType == 1)
        {
            // Rigid mesh riding a single bone - draw it in that bone's animated space.
            M3D_RENDERER->MatPushWorld();
            M3D_RENDERER->MatSetWorld(ai->m_bonesAnim[mesh.m_numNode].m_curMatrix);
            M3D_RENDERER->SetToStream0(mesh.m_VbPoolField);
            baseVertex = mesh.m_VbPoolField.RealOffset;
        }
        else if (mesh.m_meshType == 2)
        {
            // Skinned mesh - push the CPU-transformed vertices through the shared
            // streaming vertex buffer.
            rend::VbHandle const vb = M3D_RENDERER->GetVbStreaming(mesh.m_VertexType);
            int firstVertex = 0;
            void* dest = M3D_RENDERER->LockVbStreaming(vb, mesh.m_numVertices, firstVertex, nullptr);
            memcpy(dest, ai->m_meshesVerts[cc], mesh.m_numVertices * mesh.m_VertexTypeSize);
            M3D_RENDERER->UnlockVb(vb);
            M3D_RENDERER->SetToStream0(vb);
            baseVertex = static_cast<unsigned>(firstVertex);
        }
        else if (mesh.m_meshType == 4)
        {
            M3D_RENDERER->SetToStream0(mesh.m_VbPoolField);
            baseVertex = mesh.m_VbPoolField.RealOffset;
        }
        else
        {
            // Other mesh types reuse whatever stream and indices are already bound.
            bindIndices = false;
        }

        if (bindIndices)
        {
            M3D_RENDERER->SetIndices(mesh.m_IbPoolField, static_cast<int>(baseVertex));
        }

        unsigned const numPrimitives = mesh.m_numDrawIndices / 3;
        unsigned const startIndex = mesh.m_IbPoolField.RealOffset;
        unsigned const numVertices = mesh.m_numVertices;
        if (Shader)
        {
            M3D_RENDERER->DrawIndexedPrimitiveEffect(
                rend::M3DPT_TRIANGLELIST, Shader, 0, numVertices, startIndex, numPrimitives);
        }
        else
        {
            M3D_RENDERER->DrawIndexedPrimitive(rend::M3DPT_TRIANGLELIST, 0, numVertices, startIndex, numPrimitives);
        }

        if (mesh.m_meshType == 1)
        {
            M3D_RENDERER->MatPopWorld();
        }
    }

    void SetShaderParams(rend::IEffect* pShader)
    {
        // RVA 0x70D1F0
        if (!pShader)
        {
            return;
        }
        if (pShader->IsParameterUsed(rend::IEffect::LightAmbient))
        {
            pShader->SetVector3(rend::IEffect::LightAmbient, CVector(0.5f, 0.5f, 0.5f));
        }
        if (pShader->IsParameterUsed(rend::IEffect::LightDiffuse))
        {
            pShader->SetVector3(rend::IEffect::LightDiffuse, CVector(0.7f, 0.7f, 0.7f));
        }
        if (pShader->IsParameterUsed(rend::IEffect::LightSpecular))
        {
            pShader->SetVector3(rend::IEffect::LightSpecular, CVector(1.0f, 1.0f, 1.0f));
        }
        if (pShader->IsParameterUsed(rend::IEffect::FogTerm))
        {
            pShader->SetVector3(rend::IEffect::FogTerm, CVector(1075.0f, 665.0f, 409.0f));
        }
        if (pShader->IsParameterUsed(rend::IEffect::Time_Linear))
        {
            pShader->SetFloat(
                rend::IEffect::Time_Linear, static_cast<float>(g_Kernel->GetTimer().GetFrameStartTimeSec()));
        }
    }

    void AnimatedModel::RenderCollisions(CMatrix const& matT, unsigned clr)
    {
        // RVA 0x7122D0 - the model's own collision primitives, then its collision trimesh.
        BeginGeomsDraw(matT);
        for (unsigned j = 0; j < m_Geoms.size(); ++j)
        {
            DRAFT_Geom const& geom = m_Geoms[j];
            M3D_RENDERER->MatPushWorld();
            M3D_RENDERER->MatSetWorld(DraftGeomMatrix(geom));
            DrawDraftGeom(geom, clr);
            M3D_RENDERER->MatPopWorld();
        }
        if (!m_Collision.Points.empty())
        {
            for (unsigned m = 0; m < m_Collision.Triangles.size(); ++m)
            {
                Index3 const& t = m_Collision.Triangles[m];
                CVector tvec[3] = {m_Collision.Points[t.I[0]], m_Collision.Points[t.I[1]], m_Collision.Points[t.I[2]]};
                M3D_APP->DrawTri(tvec, clr);
            }
        }
        EndGeomsDraw();
    }

    unsigned AnimatedModel::GetNumMaterials() const
    {
        // RVA 0x712040
        return m_Skins.empty() ? 0u : m_Skins[0].size();
    }

    unsigned AnimatedModel::GetNumHierGeoms() const
    {
        // RVA 0x80F1A0
        return m_HierGeoms.size();
    }

    namespace
    {
        // The records of the compiled (GAM, "IVR") model file, as LoadGAM reads them.
#pragma pack(push, 1)
        struct tGamHeader
        {
            /* 0x0000 */ short iNumTriMeshes;
            /* 0x0002 */ short iNumSkinMeshes;
            /* 0x0004 */ short iNumStaticMeshes;
            /* 0x0006 */ short iNumAnimations;
            /* 0x0008 */ short iNumMaterials;
            /* 0x000a */ short iNumBones;
            /* 0x000c */ unsigned int uCfgSize;
        }; /* size: 0x0010 */

        struct tGamBone
        {
            /* 0x0000 */ char cName[40];
            /* 0x0028 */ int iParentIndex;
            /* 0x002c */ float fTranslationX;
            /* 0x0030 */ float fTranslationY;
            /* 0x0034 */ float fTranslationZ;
            /* 0x0038 */ float fQuaterninonX;
            /* 0x003c */ float fQuaterninonY;
            /* 0x0040 */ float fQuaterninonZ;
            /* 0x0044 */ float fQuaterninonW;
            /* 0x0048 */ float fInitialMatrixInv[4][4];
        }; /* size: 0x0088 */

        struct tGamMeshHeader
        {
            /* 0x0000 */ char cName[40];
            /* 0x0028 */ int iMeshType;
            /* 0x002c */ int iParentId;
            /* 0x0030 */ unsigned int uGroupId;
            /* 0x0034 */ int iMaterialNumber;
            /* 0x0038 */ unsigned int uVertexTypeSize;
            /* 0x003c */ rend::VertexType vtVertexType;
            /* 0x0040 */ int iNumVertices;
            /* 0x0044 */ int iNumFaces;
        }; /* size: 0x0048 */

        struct tGamInfluence
        {
            /* 0x0000 */ short sBoneIdx;
            /* 0x0002 */ float fBoneWeight;
            /* 0x0006 */ float fOffsetVecX;
            /* 0x000a */ float fOffsetVecY;
            /* 0x000e */ float fOffsetVecZ;
            /* 0x0012 */ float fOffsetNormalX;
            /* 0x0016 */ float fOffsetNormalY;
            /* 0x001a */ float fOffsetNormalZ;
        }; /* size: 0x001e */

        struct tGamInfluences
        {
            /* 0x0000 */ unsigned short uNumBones;
            /* 0x0002 */ tGamInfluence pIfluences[4];
        }; /* size: 0x007a */

        struct tGamAabb
        {
            /* 0x0000 */ float pBox[6];
        }; /* size: 0x0018 */

        struct tGamAnimationHeader
        {
            /* 0x0000 */ char cName[25];
            /* 0x0019 */ short sNumFrames;
            /* 0x001b */ short sNumFPS;
            /* 0x001d */ short sNextAnimation;
            /* 0x001f */ short sNumChanges;
            /* 0x0021 */ short sNumNodes;
            /* 0x0023 */ int Action;
        }; /* size: 0x0027 */

        struct tGamAnimationHierarchyChange
        {
            /* 0x0000 */ int Type;
            /* 0x0004 */ short sIdx;
            /* 0x0006 */ short sNewParent;
        }; /* size: 0x0008 */

        struct tGamAnimationTransform
        {
            /* 0x0000 */ short sIdx;
            /* 0x0002 */ float fTranslationX;
            /* 0x0006 */ float fTranslationY;
            /* 0x000a */ float fTranslationZ;
            /* 0x000e */ float fQuaterninonX;
            /* 0x0012 */ float fQuaterninonY;
            /* 0x0016 */ float fQuaterninonZ;
            /* 0x001a */ float fQuaterninonW;
        }; /* size: 0x001e */

        struct tGamMaterialHeader
        {
            /* 0x0000 */ rend::Material mMaterial;
            /* 0x0044 */ unsigned int uTextureLayersNumber;
            /* 0x0048 */ char cShaderName[100];
        }; /* size: 0x00ac */

        struct tGamTextureInfo
        {
            /* 0x0000 */ char cFileName[40];
            /* 0x0028 */ unsigned int uUVSet;
            /* 0x002c */ int Type;
        }; /* size: 0x0030 */

        struct tGamCollisionHeader
        {
            /* 0x0000 */ unsigned int uPointsNumber;
            /* 0x0004 */ unsigned int uTrianglesNumber;
        }; /* size: 0x0008 */

        struct tGamMeshGroupHeader
        {
            /* 0x0000 */ char cGroupName[20];
            /* 0x0014 */ int iVisibleAtOnceMin;
            /* 0x0018 */ int iVisibleAtOnceMax;
            /* 0x001c */ unsigned int uNumMeshes;
        }; /* size: 0x0020 */
#pragma pack(pop)

        static_assert(sizeof(tGamBone) == 0x0088);
        static_assert(sizeof(tGamMeshHeader) == 0x0048);
        static_assert(sizeof(tGamInfluences) == 0x007a);
        static_assert(sizeof(tGamAnimationHeader) == 0x0027);
        static_assert(sizeof(tGamAnimationTransform) == 0x001e);
        static_assert(sizeof(tGamMaterialHeader) == 0x00ac);
        static_assert(sizeof(tGamMeshGroupHeader) == 0x0020);
    }  // namespace

    bool AnimatedModel::SaveGAM(CStr const& FileName)
    {
        // RVA 0x8C84E0 - writes the model as a compiled GAM ("IVR") file, the chunks LoadGAM reads: 1 the header,
        // 2 the bones with their inverse bind matrices, 4 the meshes and the bounding box, 8 the animations, 15 the
        // skins, 16 the collision mesh, 32 and 64 the collision primitives, 128 the bone bounds, 240 the groups with
        // their variants. NOTE: every skin writes as many materials as the first one has.
        fs::auxTaggedFile File;
        if (File.Open(FileName.c_str(), fs::auxTaggedFile::CREATE_IGNORE_CRC))
        {
            M3D_LOG_INFO(CStr("AnimatedModel::SaveGAM -- cannot open file ") + FileName);
            return false;
        }
        File.setFormatTitle("IVR");
        File.setFormatVersion(1u);

        tGamHeader Header;
        Header.iNumTriMeshes = m_header.m_numTriMeshes;
        Header.iNumStaticMeshes = m_header.m_numStaticMeshes;
        Header.iNumSkinMeshes = m_header.m_numSkinMeshes;
        Header.iNumAnimations = m_header.m_numAnimations;
        Header.iNumMaterials = static_cast<short>(m_Skins[0].size());
        Header.iNumBones = m_header.m_numNodes;
        Header.uCfgSize = m_cfgSize;
        File.addChunk(1u);
        File.addChunkDataCopy(1u, sizeof(Header), &Header);

        retruxx::vector<tGamBone> pBonesBuf(m_header.m_numNodes);
        memset(pBonesBuf.data(), 0, sizeof(tGamBone) * pBonesBuf.size());
        for (int i = 0; i < m_header.m_numNodes; ++i)
        {
            Bone const& bone = m_boneInitialPos[i];
            tGamBone& dst = pBonesBuf[i];
            strcpy(dst.cName, bone.m_boneName);
            dst.iParentIndex = bone.m_parentIdx;
            dst.fTranslationX = bone.m_translation0.x;
            dst.fTranslationY = bone.m_translation0.y;
            dst.fTranslationZ = bone.m_translation0.z;
            dst.fQuaterninonX = bone.m_quaternion0.x;
            dst.fQuaterninonY = bone.m_quaternion0.y;
            dst.fQuaterninonZ = bone.m_quaternion0.z;
            dst.fQuaterninonW = bone.m_quaternion0.w;
            memcpy(dst.fInitialMatrixInv, &m_initialBoneInvMatrices[i], sizeof(dst.fInitialMatrixInv));
        }
        File.addChunk(2u);
        File.addChunkDataCopy(2u, sizeof(tGamBone) * m_header.m_numNodes, pBonesBuf.data());

        File.addChunk(4u);
        for (unsigned i = 0; i < m_numMeshes; ++i)
        {
            Mesh& mesh = m_meshes[i];
            tGamMeshHeader MeshHeader;
            memset(&MeshHeader, 0, sizeof(MeshHeader));
            strcpy(MeshHeader.cName, mesh.m_meshName);
            MeshHeader.iMeshType = mesh.m_meshType;
            MeshHeader.iParentId = mesh.m_numNode;
            MeshHeader.uGroupId = mesh.groupId;
            MeshHeader.iMaterialNumber = mesh.m_MaterialNumber;
            MeshHeader.uVertexTypeSize = mesh.m_VertexTypeSize;
            MeshHeader.vtVertexType = mesh.m_VertexType;
            MeshHeader.iNumVertices = mesh.m_numVertices;
            MeshHeader.iNumFaces = mesh.m_numFaces;
            File.addChunkDataCopy(4u, sizeof(MeshHeader), &MeshHeader);
            unsigned int const vertsSize = mesh.m_VertexTypeSize * mesh.m_numVertices;
            File.addChunkDataCopy(4u, vertsSize, mesh.m_verts);
            if (MeshHeader.iMeshType == TRI_MESH)
            {
                // Rigid meshes also carry their vertices in bone space, as drawn.
                if (m_bVerification)
                {
                    File.addChunkDataCopy(4u, vertsSize, mesh.m_drawVerts);
                }
                else
                {
                    File.addChunkDataCopy(4u, vertsSize, M3D_RENDERER->LockVbPoolField(mesh.m_VbPoolField));
                    M3D_RENDERER->UnlockVbPoolField(mesh.m_VbPoolField);
                }
            }
            if (MeshHeader.iMeshType == SKINED_MESH)
            {
                retruxx::vector<tGamInfluences> buffer(mesh.m_numVertices);
                memset(buffer.data(), 0, sizeof(tGamInfluences) * buffer.size());
                for (int v = 0; v < mesh.m_numVertices; ++v)
                {
                    Influences const& src = mesh.m_vertsInfluences[v];
                    tGamInfluences& dst = buffer[v];
                    dst.uNumBones = src.m_numBones;
                    for (int k = 0; k < 4; ++k)
                    {
                        dst.pIfluences[k].sBoneIdx = src.m_influences[k].m_boneIdx;
                        dst.pIfluences[k].fBoneWeight = src.m_influences[k].m_boneWeight;
                        dst.pIfluences[k].fOffsetVecX = src.m_influences[k].m_offsetVec.x;
                        dst.pIfluences[k].fOffsetVecY = src.m_influences[k].m_offsetVec.y;
                        dst.pIfluences[k].fOffsetVecZ = src.m_influences[k].m_offsetVec.z;
                        dst.pIfluences[k].fOffsetNormalX = src.m_influences[k].m_offsetNormal.x;
                        dst.pIfluences[k].fOffsetNormalY = src.m_influences[k].m_offsetNormal.y;
                        dst.pIfluences[k].fOffsetNormalZ = src.m_influences[k].m_offsetNormal.z;
                    }
                }
                File.addChunkDataCopy(4u, sizeof(tGamInfluences) * mesh.m_numVertices, buffer.data());
            }
            File.addChunkDataCopy(4u, sizeof(Index3) * mesh.m_numFaces, mesh.m_tris);
        }
        tGamAabb BoundingBox;
        memcpy(BoundingBox.pBox, m_box.m_box, sizeof(BoundingBox.pBox));
        File.addChunkDataCopy(4u, sizeof(BoundingBox), &BoundingBox);

        File.addChunk(8u);
        for (int a = 0; a < m_header.m_numAnimations; ++a)
        {
            Animation const& anim = m_animations[a];
            tGamAnimationHeader AnimHeader;
            memset(&AnimHeader, 0, sizeof(AnimHeader));
            AnimHeader.sNumFPS = anim.m_fps;
            AnimHeader.sNumFrames = anim.m_numFrames;
            strcpy(AnimHeader.cName, anim.m_name);
            AnimHeader.sNextAnimation = anim.m_nextAnimation;
            AnimHeader.sNumChanges = anim.m_numChanges;
            AnimHeader.sNumNodes = anim.m_numNodes;
            AnimHeader.Action = anim.m_action;
            File.addChunkDataCopy(8u, sizeof(AnimHeader), &AnimHeader);

            retruxx::vector<tGamAnimationHierarchyChange> changes(anim.m_numChanges);
            for (int c = 0; c < anim.m_numChanges; ++c)
            {
                changes[c].sIdx = anim.m_hierChanges[c].ownIdx;
                changes[c].sNewParent = anim.m_hierChanges[c].newParentIdx;
                changes[c].Type = anim.m_hierChanges[c].changeType;
            }
            File.addChunkDataCopy(8u, sizeof(tGamAnimationHierarchyChange) * anim.m_numChanges, changes.data());

            int const count = anim.m_numFrames * anim.m_numNodes;
            retruxx::vector<tGamAnimationTransform> transforms(count);
            for (int t = 0; t < count; ++t)
            {
                AnimationTransform const& src = anim.m_nodesPositions[t];
                tGamAnimationTransform& dst = transforms[t];
                dst.sIdx = src.idx;
                dst.fTranslationX = src.tx;
                dst.fTranslationY = src.ty;
                dst.fTranslationZ = src.tz;
                dst.fQuaterninonX = src.qx;
                dst.fQuaterninonY = src.qy;
                dst.fQuaterninonZ = src.qz;
                dst.fQuaterninonW = src.qw;
            }
            File.addChunkDataCopy(8u, sizeof(tGamAnimationTransform) * count, transforms.data());
        }

        File.addChunk(15u);
        unsigned int const uSkinsNumber = static_cast<unsigned int>(m_Skins.size());
        File.addChunkDataCopy(15u, sizeof(uSkinsNumber), &uSkinsNumber);
        for (unsigned i = 0; i < m_Skins.size(); ++i)
        {
            for (unsigned j = 0; j < m_Skins[0].size(); ++j)
            {
                DSurfaceMaterial const& mat = m_Skins[i][j];
                tGamMaterialHeader GamMaterial;
                memset(&GamMaterial, 0, sizeof(GamMaterial));
                memcpy(&GamMaterial.mMaterial, &mat.material, sizeof(rend::Material));
                GamMaterial.uTextureLayersNumber = static_cast<unsigned int>(mat.Textures.size());
                strcpy(GamMaterial.cShaderName, mat.Shader.Name.data());
                File.addChunkDataCopy(15u, sizeof(GamMaterial), &GamMaterial);
                retruxx::vector<tGamTextureInfo> pTexInfoBuf(GamMaterial.uTextureLayersNumber);
                memset(pTexInfoBuf.data(), 0, sizeof(tGamTextureInfo) * pTexInfoBuf.size());
                for (unsigned k = 0; k < GamMaterial.uTextureLayersNumber; ++k)
                {
                    pTexInfoBuf[k].Type = mat.Textures[k].Type;
                    pTexInfoBuf[k].uUVSet = mat.Textures[k].UV_Set;
                    strcpy(pTexInfoBuf[k].cFileName, mat.Textures[k].FileName.data());
                }
                File.addChunkDataCopy(15u, sizeof(tGamTextureInfo) * GamMaterial.uTextureLayersNumber, pTexInfoBuf.data());
            }
        }

        if (!m_Collision.Points.empty())
        {
            File.addChunk(16u);
            tGamCollisionHeader CollisionHeader;
            CollisionHeader.uPointsNumber = static_cast<unsigned int>(m_Collision.Points.size());
            CollisionHeader.uTrianglesNumber = static_cast<unsigned int>(m_Collision.Triangles.size());
            File.addChunkDataCopy(16u, sizeof(CollisionHeader), &CollisionHeader);
            File.addChunkDataCopy(16u, sizeof(CVector) * CollisionHeader.uPointsNumber, m_Collision.Points.data());
            File.addChunkDataCopy(16u, sizeof(Index3) * CollisionHeader.uTrianglesNumber, m_Collision.Triangles.data());
        }
        if (!m_Geoms.empty())
        {
            File.addChunk(32u);
            unsigned int const count = static_cast<unsigned int>(m_Geoms.size());
            File.addChunkDataCopy(32u, sizeof(count), &count);
            File.addChunkDataCopy(32u, sizeof(DRAFT_Geom) * count, m_Geoms.data());
        }
        if (!m_HierGeoms.empty())
        {
            File.addChunk(64u);
            unsigned int const count = static_cast<unsigned int>(m_HierGeoms.size());
            File.addChunkDataCopy(64u, sizeof(count), &count);
            File.addChunkDataCopy(64u, sizeof(DRAFT_HierGeom) * count, m_HierGeoms.data());
        }
        if (!m_BonesBounds.empty())
        {
            File.addChunk(128u);
            unsigned int const count = static_cast<unsigned int>(m_BonesBounds.size());
            File.addChunkDataCopy(128u, sizeof(count), &count);
            retruxx::vector<DRAFT_BoneBounds> pBonesBoundsBuf(count);
            unsigned int k = 0;
            for (auto const& bounds : m_BonesBounds)
            {
                pBonesBoundsBuf[k++] = bounds.second;
            }
            File.addChunkDataCopy(128u, sizeof(DRAFT_BoneBounds) * count, pBonesBoundsBuf.data());
        }

        unsigned int const GroupsNum = static_cast<unsigned int>(m_MhGroups.size());
        File.addChunk(240u);
        File.addChunkDataCopy(240u, sizeof(GroupsNum), &GroupsNum);
        for (MeshesGroup const& group : m_MhGroups)
        {
            tGamMeshGroupHeader MeshGroupHeader;
            memset(&MeshGroupHeader, 0, sizeof(MeshGroupHeader));
            // NOTE: a name of 20 characters or more overruns the field.
            strcpy(MeshGroupHeader.cGroupName, group.Name.data());
            MeshGroupHeader.uNumMeshes = static_cast<unsigned int>(group.MeshesId.size());
            MeshGroupHeader.iVisibleAtOnceMax = group.VisibleAtOnceMax;
            MeshGroupHeader.iVisibleAtOnceMin = group.VisibleAtOnceMin;
            File.addChunkDataCopy(240u, sizeof(MeshGroupHeader), &MeshGroupHeader);
            File.addChunkDataCopy(240u, sizeof(unsigned int) * MeshGroupHeader.uNumMeshes, group.MeshesId.data());
            unsigned int const VariantsNum = static_cast<unsigned int>(group.m_variants.size());
            File.addChunkDataCopy(240u, sizeof(VariantsNum), &VariantsNum);
            for (auto const& variant : group.m_variants)
            {
                unsigned int const VariantsNumber = static_cast<unsigned int>(variant.size());
                File.addChunkDataCopy(240u, sizeof(VariantsNumber), &VariantsNumber);
                File.addChunkDataCopy(240u, sizeof(unsigned int) * VariantsNumber, variant.data());
            }
        }
        File.Close();
        return true;
    }

    void AnimatedModel::SetCubeMapTexName(CStr const& texName)
    {
        m_cubemapTexPath = texName;
        UnifyFileName(m_cubemapTexPath);
        m_cubemapTexName = NameFromFileName(m_cubemapTexPath);
    }

    void AnimatedModel::LoadSkin(unsigned j)
    {
        // RVA 0x7146C0 - loads whatever of the skin is not loaded yet; cubemaps take the shared cubemap.
        if (j >= m_Skins.size())
        {
            return;
        }
        // NOTE: the materials are counted in the first skin, not in skin j.
        for (unsigned i = 0; i < m_Skins[0].size(); ++i)
        {
            DSurfaceMaterial& mat = m_Skins[j][i];
            if (!mat.Shader.Handle)
            {
                NewEffect(mat.Shader.Name, mat.Shader.Handle);
            }
            for (unsigned k = 0; k < mat.Textures.size(); ++k)
            {
                DTextureInfo& tex = mat.Textures[k];
                if (tex.Handle.IsValid())
                {
                    continue;
                }
                if (tex.Type == CUBEMAP)
                {
                    tex.FileName = m_cubemapTexName.c_str();
                    SetTexture(m_cubemapTexPath, CUBEMAP, tex.Handle);
                    continue;
                }
                CStr const path = DefinePathToTexture(CStr(tex.FileName.c_str()));
                SetTexture(path, tex.Type, tex.Handle);
            }
        }
    }

    void AnimatedModel::SetNextForAnimation(ActionType Action, int NextAction)
    {
        if (this->m_header.m_numAnimations)
        {
            auto v3 = this->m_animRemap[Action];
            auto v4 = -1;
            if (NextAction >= 0)
                v4 = this->m_animRemap[NextAction];
            if (v3 >= 0)
                this->m_animations[v3].m_nextAnimation = v4;
        }
    }

    m3d::AnimatedModel::ModelInfo& AnimatedModel::GetHeader()
    {
        return m_header;
    }

    LoadSkins const& AnimatedModel::GetLoadedSkins()
    {
        return this->m_loadSkins;
    }

    void AnimatedModel::CalculateCfgSize()
    {
        // RVA 0x7F7DD0: total configuration count = product of each mesh group's
        // variant count.
        if (m_MhGroups.empty())
        {
            m_cfgSize = 0;
            return;
        }
        m_cfgSize = m_MhGroups[0].m_variants.size();
        for (unsigned i = 1; i < m_MhGroups.size(); ++i)
        {
            m_cfgSize *= m_MhGroups[i].m_variants.size();
        }
    }

    int AnimatedModel::sortBonesFunc(void const* a, void const* b)
    {
        // RVA 0x70A340 - influences by weight, strongest first.
        float const d = static_cast<Influences::Infl const*>(a)->m_boneWeight - static_cast<Influences::Infl const*>(b)->m_boneWeight;
        if (d < 0.0f)
        {
            return 1;
        }
        if (d > 0.0f)
        {
            return -1;
        }
        return 0;
    }

    bool AnimatedModel::Convert(
        CStr const&,
        bool forcenextanimation,
        retruxx::vector<DRAFT_Bone, retruxx::allocator<DRAFT_Bone>> const& Bones,
        retruxx::vector<DMesh, retruxx::allocator<DMesh>> const& Meshes,
        retruxx::vector<DAnimation, retruxx::allocator<DAnimation>> const& Animations)
    {
        // RVA 0x783D60 - builds the runtime model from draft data: the bones; the meshes with their components
        // interleaved into one vertex format, skinned vertices keeping their 4 strongest influences; the
        // animations; the bind pose, which takes rigid meshes and skin offsets into bone space; the action remap.
        // NOTE: the previous bones, animations and inverse bind matrices are not freed.
        short numSkin = 0;
        short numTri = 0;
        short numStatic = 0;
        for (DMesh const& mesh : Meshes)
        {
            if (mesh.Header.Type == SKINED_MESH)
            {
                ++numSkin;
            }
            if (mesh.Header.Type == TRI_MESH)
            {
                ++numTri;
            }
            if (mesh.Header.Type == STATIC_MESH)
            {
                ++numStatic;
            }
        }
        m_header.m_numTriMeshes = numTri;
        m_header.m_numStaticMeshes = numStatic;
        m_header.m_numNodes = static_cast<short>(Bones.size());
        m_header.m_numSkinMeshes = numSkin;
        m_header.m_numAnimations = static_cast<short>(Animations.size());

        m_boneInitialPos = new Bone[m_header.m_numNodes];
        for (int i = 0; i < m_header.m_numNodes; ++i)
        {
            Bone& bone = m_boneInitialPos[i];
            DRAFT_Bone const& src = Bones[i];
            memcpy(bone.m_boneName, src.Name, sizeof(bone.m_boneName));
            bone.m_ownIdx = i;
            bone.m_parentIdx = src.ParentIndex;
            bone.m_translation0 = CVector(src.Tx, src.Ty, src.Tz);
            bone.m_quaternion0 = Quaternion(src.Rx, src.Ry, src.Rz, src.Rw);
        }

        m_numMeshes = static_cast<unsigned>(Meshes.size());
        delete[] m_meshes;
        m_meshes = new Mesh[m_numMeshes];
        for (int m = 0; m < 3; ++m)
        {
            m_box.m_box[m] = 10000.0f;
            m_box.m_box[m + 3] = -10000.0f;
        }

        for (unsigned i = 0; i < m_numMeshes; ++i)
        {
            Mesh& mesh = m_meshes[i];
            DMesh const& src = Meshes[i];
            mesh.meshId = i;
            mesh.m_MaterialNumber = src.Header.MaterialIndex;
            mesh.groupId = static_cast<unsigned>(-1);
            mesh.m_VertexType = src.VertType;
            mesh.m_VertexTypeSize = src.VertTypeSize;
            mesh.m_meshType = src.Header.Type;
            mesh.m_numFaces = static_cast<int>(src.Triangles.size());
            mesh.m_numVertices = src.Header.VertexCount;
            mesh.m_numNode = src.Header.Type == SKINED_MESH ? 0 : src.Header.ParentBone;
            mesh.m_VCHs = src.VertexComponentHeaders;
            mesh.m_pModelSkins = &m_Skins;

            // A composite model draws its static meshes as rigid ones; otherwise a rigid mesh that never moves (or
            // any mesh of an unanimated model) becomes static.
            if (m_composite)
            {
                if (mesh.m_meshType == STATIC_MESH)
                {
                    mesh.m_meshType = TRI_MESH;
                    --m_header.m_numStaticMeshes;
                    ++m_header.m_numTriMeshes;
                }
            }
            else if (m_header.m_numAnimations)
            {
                if (IsStaticTriMesh(src, Animations, Bones) && mesh.m_meshType == TRI_MESH)
                {
                    mesh.m_meshType = STATIC_MESH;
                    ++m_header.m_numStaticMeshes;
                    --m_header.m_numTriMeshes;
                }
            }
            else if (mesh.m_meshType == TRI_MESH)
            {
                mesh.m_meshType = STATIC_MESH;
                ++m_header.m_numStaticMeshes;
                --m_header.m_numTriMeshes;
            }
            else if (mesh.m_meshType == SKINED_MESH)
            {
                mesh.m_meshType = STATIC_MESH;
                ++m_header.m_numStaticMeshes;
                --m_header.m_numSkinMeshes;
            }

            char Idx[8];
            _itoa(i, Idx, 10);
            std::string const MeshName = std::string("mesh") + Idx;
            strcpy(mesh.m_meshName, MeshName.data());

            if (!mesh.m_numVertices || !mesh.m_numFaces)
            {
                // An empty mesh is kept but disabled (no name, no bone).
                M3D_LOG_ERR(CStr("ERROR: SAM model contains empty mesh named ") + CStr(mesh.m_meshName));
                mesh.m_meshName[0] = 0;
                mesh.m_numNode = -1;
                continue;
            }

            delete[] static_cast<unsigned char*>(mesh.m_verts);
            mesh.m_verts = new unsigned char[mesh.m_numVertices * mesh.m_VertexTypeSize];
            // The components into the interleaved vertices. vertexComponents does not change across the loops.
            retruxx::vector<DRAFT_VertexComponent>& vchs = mesh.m_VCHs;
            if (mesh.m_meshType == SKINED_MESH)
            {
                delete[] mesh.m_vertsInfluences;
                mesh.m_vertsInfluences = new Influences[mesh.m_numVertices];
                unsigned char* dst = static_cast<unsigned char*>(mesh.m_verts);
                for (int v = 0; v < mesh.m_numVertices; ++v, dst += mesh.m_VertexTypeSize)
                {
                    for (unsigned c = 0; c < vchs.size(); ++c)
                    {
                        DRAFT_VertexComponent const& comp = vchs[c];
                        if (comp.Size > 0)
                        {
                            memcpy(dst + CompOff(vchs, comp.Type),
                                static_cast<unsigned char const*>(src.VerticesComponents[c]) + v * comp.Size, comp.Size);
                            continue;
                        }
                        if (comp.Type != INFLUENCES)
                        {
                            M3D_LOG_ERR(CStr("Unknown  component of vertex "));
                            return false;
                        }
                        retruxx::vector<DInfluence> const& list =
                            static_cast<retruxx::vector<DInfluence> const*>(src.VerticesComponents[c])[v];
                        Influences& inf = mesh.m_vertsInfluences[v];
                        inf.m_numBones = static_cast<unsigned short>(list.size());
                        // NOTE: only 32 influences fit, but all of them are sorted, overrunning the buffer when a
                        // vertex has more.
                        Influences::Infl infl[32];
                        for (int k = 0; k < inf.m_numBones; ++k)
                        {
                            if (k < 32)
                            {
                                infl[k].m_boneIdx = list[k].BoneIndex;
                                infl[k].m_boneWeight = list[k].Weight;
                            }
                        }
                        qsort(infl, inf.m_numBones, sizeof(Influences::Infl), sortBonesFunc);
                        // The 4 strongest are kept (the unused slots copy whatever the buffer held).
                        for (int k = 0; k < 4; ++k)
                        {
                            inf.m_influences[k] = infl[k];
                        }
                        if (inf.m_numBones > 4)
                        {
                            inf.m_numBones = 4;
                        }
                    }
                }
            }
            else if (mesh.m_meshType == TRI_MESH || mesh.m_meshType == STATIC_MESH)
            {
                unsigned char* dst = static_cast<unsigned char*>(mesh.m_verts);
                for (int v = 0; v < mesh.m_numVertices; ++v, dst += mesh.m_VertexTypeSize)
                {
                    for (unsigned c = 0; c < vchs.size(); ++c)
                    {
                        DRAFT_VertexComponent const& comp = vchs[c];
                        if (comp.Size <= 0)
                        {
                            M3D_LOG_ERR(CStr("Unknown  component of vertex "));
                            return false;
                        }
                        memcpy(dst + CompOff(vchs, comp.Type),
                            static_cast<unsigned char const*>(src.VerticesComponents[c]) + v * comp.Size, comp.Size);
                    }
                }
            }

            for (int v = 0; v < mesh.m_numVertices; ++v)
            {
                float const* const pos =
                    reinterpret_cast<float const*>(static_cast<unsigned char const*>(mesh.m_verts) + v * mesh.m_VertexTypeSize);
                for (int k = 0; k < 3; ++k)
                {
                    if (m_box.m_box[k] > pos[k])
                    {
                        m_box.m_box[k] = pos[k];
                    }
                    if (pos[k] > m_box.m_box[k + 3])
                    {
                        m_box.m_box[k + 3] = pos[k];
                    }
                }
            }

            delete[] mesh.m_tris;
            mesh.m_tris = new unsigned short[3 * mesh.m_numFaces];
            memcpy(mesh.m_tris, src.Triangles.data(), sizeof(Index3) * mesh.m_numFaces);
            mesh.m_numDrawVerts = mesh.m_numVertices;
            mesh.m_numDrawIndices = 3 * mesh.m_numFaces;
            mesh.m_drawVerts = mesh.m_verts;
            mesh.m_drawIndices = mesh.m_tris;
            if (!m_bVerification)
            {
                mesh.m_IbPoolField = M3D_RENDERER->AddIbPoolField(mesh.m_numDrawIndices);
                memcpy(M3D_RENDERER->LockIbPoolField(mesh.m_IbPoolField), mesh.m_drawIndices,
                    sizeof(unsigned short) * mesh.m_numDrawIndices);
                M3D_RENDERER->UnlockIbPoolField(mesh.m_IbPoolField);
            }
        }

        if (!Animations.empty())
        {
            m_animations = new Animation[m_header.m_numAnimations];
            for (int a = 0; a < m_header.m_numAnimations; ++a)
            {
                Animation& anim = m_animations[a];
                DAnimation const& src = Animations[a];
                anim.m_numNodes = static_cast<short>(Bones.size());
                anim.m_numFrames = static_cast<short>(src.Info.FramesNumber);
                anim.m_fps = static_cast<short>(src.Info.FPS_Number);
                anim.m_numChanges = static_cast<short>(src.Info.NumberOfChanges);
                anim.m_nextAnimation = static_cast<short>(src.Info.NextAnimation);
                memcpy(anim.m_name, src.Info.Name, sizeof(anim.m_name));
                if (anim.m_numChanges)
                {
                    anim.m_hierChanges = new HierarchyChange[anim.m_numChanges];
                    for (int c = 0; c < anim.m_numChanges; ++c)
                    {
                        anim.m_hierChanges[c].ownIdx = src.HierarchyChanges[c].Index;
                        anim.m_hierChanges[c].changeType = src.HierarchyChanges[c].Type;
                        anim.m_hierChanges[c].newParentIdx = src.HierarchyChanges[c].NewParent;
                    }
                }
                anim.m_nodesPositions = new AnimationTransform[anim.m_numNodes * anim.m_numFrames];
                int n = 0;
                for (int f = 0; f < anim.m_numFrames; ++f)
                {
                    for (int b = 0; b < anim.m_numNodes; ++b, ++n)
                    {
                        DRAFT_Transform const& t = src.AnimationKeys[f][b];
                        AnimationTransform& dst = anim.m_nodesPositions[n];
                        dst.idx = static_cast<short>(b);
                        dst.tx = t.Tx;
                        dst.ty = t.Ty;
                        dst.tz = t.Tz;
                        dst.qx = t.Rx;
                        dst.qy = t.Ry;
                        dst.qz = t.Rz;
                        dst.qw = t.Rw;
                    }
                }
            }
        }

        // The bind pose: every bone's initial matrix, inverted.
        AnimInfo ai;
        ai.m_bonesAnim = new BoneAnim[m_header.m_numNodes];
        for (int b = 0; b < m_header.m_numNodes; ++b)
        {
            ai.m_bonesAnim[b].m_rotation = m_boneInitialPos[b].m_quaternion0;
            ai.m_bonesAnim[b].m_translation = m_boneInitialPos[b].m_translation0;
            ai.m_bonesAnim[b].m_parentIdx = m_boneInitialPos[b].m_parentIdx;
        }
        m_initialBoneInvMatrices = new CMatrix[m_header.m_numNodes];
        for (int b = 0; b < m_header.m_numNodes; ++b)
        {
            MatrixForBone(&ai, -1, b);
            m_initialBoneInvMatrices[b] = ai.m_bonesAnim[b].m_curMatrix.getInverse();
        }
        delete[] ai.m_bonesAnim;
        ai.m_bonesAnim = nullptr;

        for (unsigned j = 0; j < m_numMeshes; ++j)
        {
            Mesh& mesh = m_meshes[j];
            if (mesh.m_numNode < 0)
            {
                continue;
            }
            if (mesh.m_meshType == SKINED_MESH)
            {
                // Each influence keeps the vertex (and normal) in its bone's bind space.
                float const* src = static_cast<float const*>(mesh.m_drawVerts);
                for (int v = 0; v < mesh.m_numDrawVerts; ++v)
                {
                    Influences& inf = mesh.m_vertsInfluences[v];
                    for (int k = 0; k < inf.m_numBones; ++k)
                    {
                        CMatrix const& inv = m_initialBoneInvMatrices[inf.m_influences[k].m_boneIdx];
                        float const x = src[0];
                        float const y = src[1];
                        float const z = src[2];
                        float const nx = src[3];
                        float const ny = src[4];
                        float const nz = src[5];
                        inf.m_influences[k].m_offsetVec.x = y * inv._21 + x * inv._11 + z * inv._31 + inv._41;
                        inf.m_influences[k].m_offsetVec.y = x * inv._12 + z * inv._32 + inv._22 * y + inv._42;
                        inf.m_influences[k].m_offsetVec.z = x * inv._13 + z * inv._33 + y * inv._23 + inv._43;
                        inf.m_influences[k].m_offsetNormal.x = nx * inv._11 + nz * inv._31 + ny * inv._21;
                        inf.m_influences[k].m_offsetNormal.y = inv._12 * nx + nz * inv._32 + ny * inv._22;
                        inf.m_influences[k].m_offsetNormal.z = nz * inv._33 + ny * inv._23 + nx * inv._13;
                    }
                    src = reinterpret_cast<float const*>(reinterpret_cast<unsigned char const*>(src) + mesh.m_VertexTypeSize);
                }
            }
            else if (mesh.m_meshType == STATIC_MESH)
            {
                if (!m_bVerification)
                {
                    mesh.m_VbPoolField = M3D_RENDERER->AddVbPoolField(mesh.m_VertexType, mesh.m_numDrawVerts);
                    memcpy(M3D_RENDERER->LockVbPoolField(mesh.m_VbPoolField), mesh.m_drawVerts,
                        mesh.m_numDrawVerts * mesh.m_VertexTypeSize);
                    M3D_RENDERER->UnlockVbPoolField(mesh.m_VbPoolField);
                }
            }
            else if (mesh.m_meshType == TRI_MESH)
            {
                // Rigid meshes are drawn under their bone's matrix, so their vertices go into the bone's bind space.
                unsigned int const size = mesh.m_numDrawVerts * mesh.m_VertexTypeSize;
                unsigned char* const buffer = new unsigned char[size];
                memcpy(buffer, mesh.m_drawVerts, size);
                CMatrix const& inv = m_initialBoneInvMatrices[mesh.m_numNode];
                retruxx::vector<DRAFT_VertexComponent>& vchs = mesh.m_VCHs;
                unsigned char const* srcVert = static_cast<unsigned char const*>(mesh.m_drawVerts);
                unsigned char* dstVert = buffer;
                auto const rotate = [&inv](float const* v, float* out) {
                    float const x = v[0];
                    float const y = v[1];
                    float const z = v[2];
                    out[0] = inv._31 * z + inv._21 * y + inv._11 * x;
                    out[1] = x * inv._12 + z * inv._32 + y * inv._22;
                    out[2] = inv._23 * y + x * inv._13 + z * inv._33;
                };
                for (int v = 0; v < mesh.m_numDrawVerts; ++v, srcVert += mesh.m_VertexTypeSize, dstVert += mesh.m_VertexTypeSize)
                {
                    float const* const s = reinterpret_cast<float const*>(srcVert);
                    float* const d = reinterpret_cast<float*>(dstVert);
                    float const x = s[0];
                    float const y = s[1];
                    float const z = s[2];
                    d[0] = inv._31 * z + inv._21 * y + inv._11 * x + inv._41;
                    d[1] = x * inv._12 + z * inv._32 + y * inv._22 + inv._42;
                    d[2] = inv._23 * y + x * inv._13 + z * inv._33 + inv._43;
                    rotate(s + 3, d + 3);
                    if (VertCompPresent(vchs, BINORMAL))
                    {
                        // NOTE: meant for the binormal, but the position is rotated (without its translation) over
                        // the transformed one.
                        rotate(s, d);
                    }
                    if (VertCompPresent(vchs, TANGENT))
                    {
                        VERTEX_COMPONENT const lastTex = VertCompPresent(vchs, TEXTURE3) ? TEXTURE3
                            : VertCompPresent(vchs, TEXTURE2)                            ? TEXTURE2
                                                                                         : TEXTURE1;
                        unsigned int const off = CompOff(vchs, lastTex) + 8;
                        rotate(reinterpret_cast<float const*>(srcVert + off), reinterpret_cast<float*>(dstVert + off));
                    }
                }
                if (m_bVerification)
                {
                    // NOTE: the vertices keep their draft copy in m_verts; only the drawn ones are replaced.
                    mesh.m_drawVerts = new unsigned char[size];
                    memcpy(mesh.m_drawVerts, buffer, size);
                }
                else
                {
                    mesh.m_VbPoolField = M3D_RENDERER->AddVbPoolField(mesh.m_VertexType, mesh.m_numDrawVerts);
                    memcpy(M3D_RENDERER->LockVbPoolField(mesh.m_VbPoolField), buffer, size);
                    M3D_RENDERER->UnlockVbPoolField(mesh.m_VbPoolField);
                }
                delete[] buffer;
            }
        }

        for (int i = 0; i < 32; ++i)
        {
            short a = 0;
            while (a < m_header.m_numAnimations && strcmp(actions[i].m_name, m_animations[a].m_name))
            {
                ++a;
            }
            if (a >= m_header.m_numAnimations)
            {
                m_animRemap[i] = -1;
            }
            else
            {
                m_animRemap[i] = a;
                m_animations[a].m_action = static_cast<ActionType>(AT_STAND1 + i);
            }
        }
        for (int a = 0; a < m_header.m_numAnimations; ++a)
        {
            if (forcenextanimation)
            {
                int k = 0;
                while (k < 32 && m_animRemap[k] != a)
                {
                    ++k;
                }
                if (k == 32 || nextAnims_0[k] < 0)
                {
                    m_animations[a].m_nextAnimation = -1;
                }
                else
                {
                    m_animations[a].m_nextAnimation = m_animRemap[nextAnims_0[k]];
                }
            }
            else
            {
                m_animations[a].m_nextAnimation = static_cast<short>(a);
            }
        }
        CreateVariants();
        CalculateCfgSize();
        CheckConfigurations();
        return true;
    }

    void AnimatedModel::MatrixForBone(AnimInfo* ai, int curFrame, int boneIndex)
    {
        // RVA 0x715650 - a negative frame always recomputes (Convert uses -1 for the bind pose).
        // TODO: generated code
        if (curFrame >= 0 && ai->GetBoneAnim(boneIndex).m_lastUpdatedFrame == curFrame)
        {
            return;
        }

        // Build a stack of bones from the target bone up to the root
        retruxx::stack<int> boneStack;
        int currentBoneIdx = boneIndex;

        while (true)
        {
            BoneAnim* currentBone = &ai->GetBoneAnim(currentBoneIdx);
            boneStack.push(currentBoneIdx);

            // Stop if this bone is already updated or if we reached the root
            if (curFrame >= 0 && currentBone->m_lastUpdatedFrame == curFrame)
            {
                break;
            }
            if (currentBone->m_parentIdx < 0)
            {  // Root bone
                break;
            }
            currentBoneIdx = currentBone->m_parentIdx;
        }

        // Process bones from root to target (reverse order of stack)
        while (!boneStack.empty())
        {
            int boneIdx = boneStack.top();
            boneStack.pop();

            BoneAnim* bone = &ai->GetBoneAnim(boneIdx);
            BoneAnim* parentBone = nullptr;
            CMatrix* parentMatrix = nullptr;

            if (bone->m_parentIdx >= 0)
            {
                parentBone = &ai->GetBoneAnim(bone->m_parentIdx);
                parentMatrix = &parentBone->m_curMatrix;
            }

            // Convert quaternion rotation to matrix
            Quaternion const& rot = bone->m_rotation;
            CMatrix rotationMatrix;

            // Calculate quaternion components for matrix conversion
            float xx = rot.x * rot.x;
            float xy = rot.x * rot.y;
            float xz = rot.x * rot.z;
            float xw = rot.x * rot.w;

            float yy = rot.y * rot.y;
            float yz = rot.y * rot.z;
            float yw = rot.y * rot.w;

            float zz = rot.z * rot.z;
            float zw = rot.z * rot.w;

            // Build rotation matrix from quaternion
            rotationMatrix._11 = 1.0f - 2.0f * (yy + zz);
            rotationMatrix._12 = 2.0f * (xy + zw);
            rotationMatrix._13 = 2.0f * (xz - yw);
            rotationMatrix._14 = 0.0f;

            rotationMatrix._21 = 2.0f * (xy - zw);
            rotationMatrix._22 = 1.0f - 2.0f * (xx + zz);
            rotationMatrix._23 = 2.0f * (yz + xw);
            rotationMatrix._24 = 0.0f;

            rotationMatrix._31 = 2.0f * (xz + yw);
            rotationMatrix._32 = 2.0f * (yz - xw);
            rotationMatrix._33 = 1.0f - 2.0f * (xx + yy);
            rotationMatrix._34 = 0.0f;

            rotationMatrix._41 = bone->m_translation.x;
            rotationMatrix._42 = bone->m_translation.y;
            rotationMatrix._43 = bone->m_translation.z;
            rotationMatrix._44 = 1.0f;

            // Combine with parent matrix if exists
            if (parentMatrix)
            {
                // Multiply bone's local matrix by parent's world matrix
                bone->m_curMatrix = rotationMatrix;
                bone->m_curMatrix *= (*parentMatrix);
            }
            else
            {
                // No parent, so local matrix is world matrix
                bone->m_curMatrix = rotationMatrix;
            }

            bone->m_lastUpdatedFrame = curFrame;
        }
    }

    void AnimatedModel::NewEffect(retruxx::string const& name, rend::IEffect*& shader)
    {
        if (!shader && !m_bVerification)
        {
            auto shaderFile = "data/shaders/" + name + ".fx";
            shader = M3D_RENDERER->NewEffect(shaderFile.c_str(), true);
            if (shader)
            {
                shader->SetDefaultTechnique(true);
            }
        }
    }

    namespace
    {
        // RVA 0x7F7D30 - advances an ascending combination of mesh indices (each at most Last minus the number of
        // slots after it); a combination that cannot advance any further is cleared.
        void NextVariant(retruxx::vector<unsigned int>& variant, int Last, int CurrentToMove)
        {
            if (CurrentToMove < 0 || variant.empty())
            {
                retruxx::vector<unsigned int>().swap(variant);
                return;
            }
            if (variant[CurrentToMove] < static_cast<unsigned int>(Last))
            {
                ++variant[CurrentToMove];
                return;
            }
            NextVariant(variant, Last - 1, CurrentToMove - 1);
            if (!variant.empty())
            {
                variant[CurrentToMove] = variant[CurrentToMove - 1] + 1;
            }
        }
    }  // namespace

    void MeshesGroup::GetNextVariant(retruxx::vector<unsigned int>& variant)
    {
        // RVA 0x7F82B0 - an empty variant starts with the first VisibleAtOnceMax meshes; after the last combination
        // of a size comes the first one of the next smaller size, down to VisibleAtOnceMin; then the empty variant.
        int const size = static_cast<int>(variant.size());
        if (variant.empty())
        {
            for (int i = 0; i < VisibleAtOnceMax; ++i)
            {
                variant.push_back(i);
            }
            return;
        }
        NextVariant(variant, static_cast<int>(MeshesId.size()) - 1, size - 1);
        if (variant.empty() && size > VisibleAtOnceMin)
        {
            variant.resize(size - 1, 0);
            for (unsigned int i = 0; i < variant.size(); ++i)
            {
                variant[i] = i;
            }
        }
    }

    void AnimatedModel::CreateVariants()
    {
        // RVA 0x7F8650 - lists every variant of each mesh group, up to the empty one that ends the sequence. The
        // empty variant is kept too unless the group needs at least one mesh visible.
        for (unsigned i = 0; i < m_MhGroups.size(); ++i)
        {
            MeshesGroup& group = m_MhGroups[i];
            retruxx::vector<unsigned int> variant;
            group.m_variants.clear();
            do
            {
                group.GetNextVariant(variant);
                if (!variant.empty() || !group.VisibleAtOnceMin)
                {
                    group.m_variants.push_back(variant);
                }
            } while (!variant.empty());
        }
    }

    void AnimatedModel::SetTexture(CStr const& path, DRAFT_TextureType type, rend::TexHandle& texHandle)
    {
        if (m_bVerification)
        {
            return;
        }

        switch (type)
        {
        case DETAIL:
        case LIGHTMAP:
        case DIFFUSE:
        {
            texHandle = M3D_RENDERER->AddTexture(path, 2);
            M3D_RENDERER->SetTextureParameter(texHandle, rend::TM_WRAP_S, 1);
            M3D_RENDERER->SetTextureParameter(texHandle, rend::TM_WRAP_T, 1);
            M3D_RENDERER->SetTextureParameter(texHandle, rend::TM_TEX_FILTER, 4);
            break;
        }
        case BUMP:
        {
            texHandle = M3D_RENDERER->AddTexture(path, 5);
            M3D_RENDERER->SetTextureParameter(texHandle, rend::TM_WRAP_S, 1);
            M3D_RENDERER->SetTextureParameter(texHandle, rend::TM_WRAP_T, 1);
            M3D_RENDERER->SetTextureParameter(texHandle, rend::TM_TEX_FILTER, 5);
        }
        case CUBEMAP:
        {
            texHandle = M3D_RENDERER->AddTexture(path, 2);
            M3D_RENDERER->SetTextureParameter(texHandle, rend::TM_WRAP_S, 3);
            M3D_RENDERER->SetTextureParameter(texHandle, rend::TM_WRAP_T, 3);
            M3D_RENDERER->SetTextureParameter(texHandle, rend::TM_WRAP_R, 3);
            M3D_RENDERER->SetTextureParameter(texHandle, rend::TM_TEX_FILTER, 4);
        }
        }
        if (!texHandle.IsValid())
        {
            M3D_LOG_ERR("Error: Couldn't load texture " + path + " for model " + CStr(m_Name.c_str()));
        }
    }

    bool DTextureInfo::operator<(DTextureInfo const& A)
    {
        // RVA 0x77BA70 - texture layers are ordered by type.
        return Type < A.Type;
    }

    void AnimatedModel::ReadMaterial(DSurfaceMaterial& SurfaceMaterial, unsigned char*& Data, bool load)
    {
        // RVA 0x7866F0 - a DRAFT_MaterialHeader, the shader name and the texture layers. Texture names are lowered;
        // the layers end up sorted by DTextureInfo::operator<.
        SurfaceMaterial.Textures.clear();
        SurfaceMaterial.Textures.shrink_to_fit();
        memcpy(&SurfaceMaterial.material, Data, sizeof(rend::Material));
        Data += sizeof(rend::Material);
        unsigned int const TextureLayersNumber = *reinterpret_cast<unsigned int const*>(Data);
        Data += sizeof(unsigned int);
        unsigned int const ShaderStringLength = *reinterpret_cast<unsigned int const*>(Data);
        Data += sizeof(unsigned int);
        // NOTE: a shader name of 100 characters or more overruns the buffer.
        char HStr[100];
        memcpy(HStr, Data, ShaderStringLength);
        HStr[ShaderStringLength] = 0;
        SurfaceMaterial.Shader.Name = HStr;
        Data += ShaderStringLength;
        if (load && ShaderStringLength)
        {
            NewEffect(SurfaceMaterial.Shader.Name, SurfaceMaterial.Shader.Handle);
        }
        else
        {
            SurfaceMaterial.Shader.Handle = nullptr;
        }

        for (unsigned int n = 0; n < TextureLayersNumber; ++n)
        {
            SurfaceMaterial.Textures.push_back(DTextureInfo());
            DRAFT_TextureInfo Texture;
            memcpy(&Texture, Data, sizeof(Texture));
            Data += sizeof(Texture);
            DTextureInfo& tex = SurfaceMaterial.Textures.back();
            if (Texture.Type == CUBEMAP)
            {
                tex.FileName = m_cubemapTexName.c_str();
                tex.Type = CUBEMAP;
                tex.UV_Set = Texture.UVSet;
                tex.Handle.SetInvalid();
                m_hasCubemap = true;
                if (load)
                {
                    SetTexture(m_cubemapTexPath, CUBEMAP, tex.Handle);
                }
                continue;
            }
            CStr name(Texture.FileName);
            if (name.c_str() && strlen(name.c_str()))
            {
                char* const chars = const_cast<char*>(name.c_str());
                int const length = static_cast<int>(strlen(chars)) + 1;
                LCMapStringA(LOCALE_USER_DEFAULT, LCMAP_LOWERCASE, chars, length, chars, length);
            }
            tex.FileName = name.c_str();
            tex.Type = Texture.Type;
            tex.UV_Set = Texture.UVSet;
            tex.Handle.SetInvalid();
            CStr const path = DefinePathToTexture(name);
            if (load)
            {
                SetTexture(path, Texture.Type, tex.Handle);
            }
        }
        std::sort(SurfaceMaterial.Textures.begin(), SurfaceMaterial.Textures.end());
    }

    void AnimatedModel::VertsForSkinmesh(AnimInfo* ai, Mesh& mesh, void* dstVerts, bool onlyXYZN)
    {
        // TODO: generated code
        // Copy original vertices if we need full vertex data
        if (!onlyXYZN)
        {
            size_t vertexDataSize = mesh.m_VertexTypeSize * mesh.m_numDrawVerts;
            memcpy(dstVerts, mesh.m_drawVerts, vertexDataSize);
        }

        // Process each vertex through skinning
        for (int vertexIndex = 0; vertexIndex < mesh.m_numDrawVerts; ++vertexIndex)
        {
            auto& vertexInfluences = mesh.m_vertsInfluences[vertexIndex];

            // Accumulators for transformed position and normal
            CVector transformedPos(0.0f, 0.0f, 0.0f);
            CVector transformedNormal(0.0f, 0.0f, 0.0f);

            // Apply all bone influences for this vertex
            for (int boneIndex = 0; boneIndex < vertexInfluences.m_numBones; ++boneIndex)
            {
                auto const& influence = vertexInfluences.m_influences[boneIndex];
                auto const& boneAnim = ai->m_bonesAnim[influence.m_boneIdx];
                auto const& boneMatrix = boneAnim.m_curMatrix;
                float weight = influence.m_boneWeight;

                // Transform position by bone matrix
                CVector weightedPos = influence.m_offsetVec;
                CVector transformedWeightedPos;
                transformedWeightedPos.x = boneMatrix._11 * weightedPos.x + boneMatrix._21 * weightedPos.y +
                    boneMatrix._31 * weightedPos.z + boneMatrix._41;
                transformedWeightedPos.y = boneMatrix._12 * weightedPos.x + boneMatrix._22 * weightedPos.y +
                    boneMatrix._32 * weightedPos.z + boneMatrix._42;
                transformedWeightedPos.z = boneMatrix._13 * weightedPos.x + boneMatrix._23 * weightedPos.y +
                    boneMatrix._33 * weightedPos.z + boneMatrix._43;

                transformedPos += transformedWeightedPos * weight;

                // Transform normal by bone matrix (3x3 rotation part only)
                CVector weightedNormal = influence.m_offsetNormal;
                CVector transformedWeightedNormal;
                transformedWeightedNormal.x = boneMatrix._11 * weightedNormal.x + boneMatrix._21 * weightedNormal.y +
                    boneMatrix._31 * weightedNormal.z;
                transformedWeightedNormal.y = boneMatrix._12 * weightedNormal.x + boneMatrix._22 * weightedNormal.y +
                    boneMatrix._32 * weightedNormal.z;
                transformedWeightedNormal.z = boneMatrix._13 * weightedNormal.x + boneMatrix._23 * weightedNormal.y +
                    boneMatrix._33 * weightedNormal.z;

                transformedNormal += transformedWeightedNormal * weight;
            }

            // Write transformed vertex data to output buffer
            float* outputVertex =
                reinterpret_cast<float*>(static_cast<char*>(dstVerts) + vertexIndex * mesh.m_VertexTypeSize);

            outputVertex[0] = transformedPos.x;
            outputVertex[1] = transformedPos.y;
            outputVertex[2] = transformedPos.z;
            outputVertex[3] = transformedNormal.x;
            outputVertex[4] = transformedNormal.y;
            outputVertex[5] = transformedNormal.z;

            // Note: If !onlyXYZN, the rest of vertex data (texcoords, colors, etc.)
            // was already copied and remains unchanged
        }
    }

    void AnimatedModel::DrawBones(AnimInfo* ai)
    {
        // RVA 0x7115E0 - every bone that has children: its axes (the matrix columns, half length), a white line to
        // its parent and its name.
        M3D_RENDERER->PushLighting(false);
        M3D_RENDERER->PushZbState(rend::ZB_DISABLE);
        for (int i = 0; i < m_header.m_numNodes; ++i)
        {
            int child = 0;
            while (child < m_header.m_numNodes && m_boneInitialPos[child].m_parentIdx != i)
            {
                ++child;
            }
            if (child == m_header.m_numNodes)
            {
                continue;
            }
            CMatrix const m = ai->m_bonesAnim ? ai->m_bonesAnim[i].m_curMatrix : GetBoneMatrix(i);
            CVector const org(m._41, m._42, m._43);
            CVector const x(m._11 * 0.5f, m._21 * 0.5f, m._31 * 0.5f);
            CVector const y(m._12 * 0.5f, m._22 * 0.5f, m._32 * 0.5f);
            CVector const z(m._13 * 0.5f, m._23 * 0.5f, m._33 * 0.5f);
            M3D_RENDERER->PushBlend(rend::BM_NONE);
            M3D_RENDERER->SetAlphaTest(false);
            SetNoTexture();
            DrawAxes(org, x, y, z);
            M3D_RENDERER->PopBlend();

            int const parentIdx = m_boneInitialPos[i].m_parentIdx;
            if (parentIdx >= 0)
            {
                CVector parentOrg;
                if (ai->m_bonesAnim)
                {
                    CMatrix const& pm = ai->m_bonesAnim[parentIdx].m_curMatrix;
                    parentOrg = CVector(pm._41, pm._42, pm._43);
                }
                else
                {
                    // NOTE: without animation data the bone's own matrix is taken, so the line has no length.
                    CMatrix const pm = GetBoneMatrix(i);
                    parentOrg = CVector(pm._41, pm._42, pm._43);
                }
                M3D_RENDERER->PushBlend(rend::BM_NONE);
                M3D_RENDERER->SetAlphaTest(false);
                SetNoTexture();
                M3D_APP->DrawLine(org, parentOrg, 0xFFFFFFFF);
                M3D_RENDERER->PopBlend();
            }
            DrawBoneName(org, m_boneInitialPos[i].m_boneName);
        }
        M3D_RENDERER->PopZbState();
        M3D_RENDERER->PopLighting();
    }

    void AnimatedModel::CheckConfigurations()
    {
        // RVA 0x7F7F00: stamp each mesh's groupId from the group it belongs to.
        for (unsigned g = 0; g < m_MhGroups.size(); ++g)
        {
            auto const& group = m_MhGroups[g];
            for (unsigned k = 0; k < group.MeshesId.size(); ++k)
            {
                m_meshes[group.MeshesId[k]].groupId = g;
            }
        }
    }

    CStr AnimatedModel::DefinePathToTexture(CStr const& fileName)
    {
        auto it = m_textureFiles.find(fileName);
        CStr path;
        if (it == m_textureFiles.end())
        {
            path = m_PathToFile.c_str();
            path += "\\" + fileName;
        }
        else
        {
            path = it->second;
        }
        UnifyFileName(path);
        return path;
    }

    bool AnimInfo::IsAnimation(ActionType action)
    {
        // RVA 0x70CBA0: true when the remapped animation exists and has >1 frame.
        int const remapped = m_forModel->m_animRemap[action];
        int frames = 0;
        if (remapped >= 0 && remapped < m_forModel->m_header.m_numAnimations)
        {
            frames = m_forModel->m_animations[remapped].m_numFrames - 1;
        }
        return frames != 0;
    }

    void AnimInfo::CreateCopyMesh(
        int& numMesh,
        void**& verts,
        int*& numVerts,
        unsigned short**& indxs,
        int*& numIndxs,
        bool*& strips,
        CMatrix**& mat,
        retruxx::vector<rend::VertexType, retruxx::allocator<rend::VertexType>>& VertexTypes,
        retruxx::vector<unsigned, retruxx::allocator<unsigned>>& VertexTypeSizes,
        int*)
    {
        // RVA 0x716D30 - per mesh, the vertices as currently drawn (the CPU-skinned copy for skinned meshes, which
        // need no matrix; otherwise the mesh's own vertices and its bone's matrix), its indices and vertex format.
        // The arrays are released with RemoveCopyMesh. NOTE: a mesh of any other type leaves its vertex and matrix
        // entries unset.
        numMesh = m_forModel->m_numMeshes;
        VertexTypes.resize(numMesh, rend::VERTEX_XYZ);
        VertexTypeSizes.resize(numMesh, 0);
        verts = new void*[numMesh];
        numVerts = new int[numMesh];
        indxs = new unsigned short*[numMesh];
        numIndxs = new int[numMesh];
        strips = new bool[numMesh];
        mat = new CMatrix*[numMesh];
        for (int i = 0; i < numMesh; ++i)
        {
            AnimatedModel::Mesh const& mesh = m_forModel->m_meshes[i];
            if (mesh.m_meshType == 2)
            {
                verts[i] = m_meshesVerts[i];
                mat[i] = nullptr;
            }
            else if (mesh.m_meshType == 4 || mesh.m_meshType == 1)
            {
                verts[i] = mesh.m_drawVerts;
                mat[i] = &m_bonesAnim[mesh.m_numNode].m_curMatrix;
            }
            numVerts[i] = mesh.m_numVertices;
            indxs[i] = mesh.m_drawIndices;
            VertexTypes[i] = mesh.m_VertexType;
            VertexTypeSizes[i] = mesh.m_VertexTypeSize;
            strips[i] = false;
            numIndxs[i] = mesh.m_numDrawIndices / 3;
        }
    }

    void AnimInfo::SetEmpty()
    {
        // RVA 0x80EAE0
        m_Empty = true;
    }

    void AnimInfo::InterpolateBones(int curUpdateFrame)
    {
        // TODO generated code
        // Early exit if we already processed this frame
        if (curUpdateFrame < 0 || m_lastInterpolationUpdate == curUpdateFrame)
        {
            return;
        }

        m_lastInterpolationUpdate = curUpdateFrame;

        if (!m_curAnimation)
        {
            return;
        }

        // Get animation data pointers and interpolation factor
        auto* anim = m_curAnimation;
        AnimatedModel::AnimationTransform* currentFrameData = nullptr;
        AnimatedModel::AnimationTransform* nextFrameData = nullptr;
        float interpolationFactor = 0.0f;

        if (m_stickToLastFrame)
        {
            // Stick to the current frame (no interpolation)
            currentFrameData = &anim->m_nodesPositions[m_curAnimFrame * anim->m_numNodes];
            interpolationFactor = 0.0f;
        }
        else
        {
            // Normal interpolation between frames
            currentFrameData = &anim->m_nodesPositions[m_curAnimFrame * anim->m_numNodes];
            int nextFrame = (m_curAnimFrame + 1) % anim->m_numFrames;
            nextFrameData = &anim->m_nodesPositions[nextFrame * anim->m_numNodes];

            // Calculate interpolation factor based on time to next frame
            interpolationFactor = 1.0f - ((float)m_timeOutToNextFrame / (float)anim->m_fps);
            interpolationFactor = std::clamp(interpolationFactor, 0.0f, 1.0f);
        }

        // Interpolate bone transformations
        for (int nodeIndex = 0; nodeIndex < anim->m_numNodes; ++nodeIndex)
        {
            auto& boneAnim = m_bonesAnim[nodeIndex];

            if (m_stickToLastFrame)
            {
                // Just copy current frame data
                auto const& currentTransform = currentFrameData[nodeIndex];
                boneAnim.m_rotation =
                    Quaternion(currentTransform.qx, currentTransform.qy, currentTransform.qz, currentTransform.qw);
                boneAnim.m_translation = CVector(currentTransform.tx, currentTransform.ty, currentTransform.tz);
            }
            else
            {
                // Interpolate between current and next frame
                auto const& currentTransform = currentFrameData[nodeIndex];
                auto const& nextTransform = nextFrameData[nodeIndex];

                // Interpolate rotation using quaternion lerp
                Quaternion q1(currentTransform.qx, currentTransform.qy, currentTransform.qz, currentTransform.qw);
                Quaternion q2(nextTransform.qx, nextTransform.qy, nextTransform.qz, nextTransform.qw);
                boneAnim.m_rotation.Lerp(q1, q2, interpolationFactor);

                // Interpolate translation using linear interpolation
                CVector v1(currentTransform.tx, currentTransform.ty, currentTransform.tz);
                CVector v2(nextTransform.tx, nextTransform.ty, nextTransform.tz);
                boneAnim.m_translation = lerp(v1, v2, interpolationFactor);
            }
        }

        // Handle animation blending if active
        if (m_isBlending)
        {
            // Calculate blend factor
            float blendFactor = 1.0f -
                (float)((float)(m_timeOutToNextFrame + anim->m_fps * (m_blendFramesNum - 1)) /
                        (float)(7.0f * (float)anim->m_fps));
            blendFactor = std::clamp(blendFactor, 0.0f, 1.0f);

            // Apply blending between previous and current bone animations
            for (int nodeIndex = 0; nodeIndex < anim->m_numNodes; ++nodeIndex)
            {
                auto& prevBone = m_bonesAnimPrev[nodeIndex];
                auto& currentBone = m_bonesAnim[nodeIndex];

                // Only blend if bones have the same parent (same hierarchy)
                if (prevBone.m_parentIdx == currentBone.m_parentIdx)
                {
                    // Store the current state before blending
                    Quaternion currentRotation = currentBone.m_rotation;
                    CVector currentTranslation = currentBone.m_translation;

                    // Blend rotation from previous to current
                    currentBone.m_rotation.Lerp(prevBone.m_rotation, currentRotation, blendFactor);

                    // Blend translation from previous to current
                    currentBone.m_translation = lerp(prevBone.m_translation, currentTranslation, blendFactor);
                }
            }
        }
    }

    int AnimInfo::SetAnimationIdx(int num)
    {
        // TODO: check and refactor this
        auto const numAnimations = m_forModel->m_header.m_numAnimations;
        int v5 = 0;
        if (!numAnimations)
            return 0;
        if (num >= numAnimations)
            return 0;
        if (num < 0)
            return 0;

        auto v7 = &m_forModel->m_animations[num];
        if (!v7)
            return 0;
        m_curAnimation = this->m_curAnimation;
        if (!m_curAnimation || this->m_lastInterpolationUpdate == -1)
        {
            this->m_isBlending = 0;
        }
        else
        {
            this->m_curAnimationPrev = m_curAnimation;
            this->m_curAnimFramePrev = m_curAnimFrame;
            memcpy(m_bonesAnimPrev, this->m_bonesAnim, sizeof(BoneAnim) * m_forModel->m_header.m_numNodes);

            this->m_timeOutToNextFramePrev = this->m_timeOutToNextFrame;
            this->m_lastInterpolationUpdatePrev = m_lastInterpolationUpdate;
            this->m_stickToLastFramePrev = m_stickToLastFrame;
            this->m_blendFramesNum = 7;
            this->m_isBlending = true;
            v5 = 0;
        }

        auto v13 = this->m_forModel;
        this->m_curAnimation = v7;
        this->m_curAnimFrame = 0;
        this->m_stickToLastFrame = 0;
        this->m_timeOutToNextFrame = 0;
        this->m_lastInterpolationUpdate = -1;
        int v14 = 0;
        if (v13->m_header.m_numNodes > 0)
        {
            int v15 = 0;
            do
            {
                this->m_bonesAnim[v15].m_parentIdx = this->m_forModel->m_boneInitialPos[v5].m_parentIdx;
                this->m_bonesAnim[v15].m_lastUpdatedFrame = -1000;
                auto p_m_quaternion0 = &this->m_forModel->m_boneInitialPos[v5].m_quaternion0;
                auto p_m_rotation = &this->m_bonesAnim[v15].m_rotation;
                p_m_rotation->x = p_m_quaternion0->x;
                p_m_rotation->y = p_m_quaternion0->y;
                p_m_rotation->z = p_m_quaternion0->z;
                p_m_rotation->w = p_m_quaternion0->w;

                auto p_m_translation0 = &this->m_forModel->m_boneInitialPos[v5].m_translation0;
                auto p_m_translation = &this->m_bonesAnim[v15].m_translation;
                p_m_translation->x = p_m_translation0->x;
                p_m_translation->y = p_m_translation0->y;
                p_m_translation->z = p_m_translation0->z;
                ++v14;
                ++v5;
                ++v15;
            } while (v14 < this->m_forModel->m_header.m_numNodes);
        }
        auto v20 = this->m_curAnimation;
        if (v20 && v20->m_numChanges > 0)
        {
            auto m_hierChanges = v20->m_hierChanges;
            auto v22 = 0;
            do
            {
                if (m_hierChanges->changeType == NEW_PARENT)
                    this->m_bonesAnim[m_hierChanges->ownIdx].m_parentIdx = m_hierChanges->newParentIdx;
                ++v22;
                ++m_hierChanges;
            } while (v22 < this->m_curAnimation->m_numChanges);
        }
        this->m_timeOutToNextFrame = this->m_curAnimation->m_fps;
        return 1;
    }

    AnimatedModel::Mesh const& AnimInfo::GetMesh(unsigned MeshNum) const
    {
        // RVA 0x6CEF10
        return m_forModel->m_meshes[MeshNum];
    }

    void AnimInfo::CreateFor(AnimatedModel* am)
    {
        // RVA 0x716AF0
        Release();
        m_curBox = am->m_box;
        m_forModel = am;
        if (am->GetNumAnimations())
        {
            m_Empty = false;
            m_bonesAnim = new BoneAnim[am->m_header.m_numNodes];
            m_bonesAnimPrev = new BoneAnim[am->m_header.m_numNodes];
            for (int i = 0; i < am->m_header.m_numNodes; ++i)
            {
                auto& boneAnim = m_bonesAnim[i];
                boneAnim.m_rotation = am->m_boneInitialPos[i].m_quaternion0;
                boneAnim.m_translation = am->m_boneInitialPos[i].m_translation0;
                boneAnim.m_parentIdx = am->m_boneInitialPos[i].m_parentIdx;
            }

            m_meshesVerts.resize(m_forModel->m_numMeshes, 0);
            m_VertTypes.resize(m_forModel->m_numMeshes, rend::VERTEX_XYZ);
            m_VertTypesSize.resize(m_forModel->m_numMeshes, 0);
            for (unsigned i = 0; i < m_forModel->m_numMeshes; ++i)
            {
                auto& mesh = m_forModel->m_meshes[i];
                m_VertTypes[i] = mesh.m_VertexType;
                m_VertTypesSize[i] = mesh.m_VertexTypeSize;
                if (mesh.m_meshType == 2)
                {
                    m_meshesVerts[i] = new uint8_t[mesh.m_numVertices * mesh.m_VertexTypeSize];
                }
                else
                {
                    m_meshesVerts[i] = nullptr;
                }
            }

            m_timeOutToNextFrame = 0;
            m_isBlending = 0;
            m3d::AnimInfo::MoveFrame(0);
            am->Update(this, 0, 0);
        }
        else
        {
            m_Empty = true;
        }
    }

    void AnimInfo::SetBoneCurMatrix(unsigned j, CMatrix const& m)
    {
        // RVA 0x80EAF0
        m_bonesAnim[j].m_curMatrix = m;
    }

    CMatrix const& AnimInfo::GetCurrentLoadpointMatrix(int lpId) const
    {
        if (lpId < 0 || this->m_Empty)
            return this->m_forModel->m_Dummy;
        else
            return this->m_bonesAnim[lpId].m_curMatrix;
    }

    int AnimInfo::SetCurFrame(float progress)
    {
        // RVA 0x70CBF0: map a 0..1 progress onto the current clip's frame index.
        if (m_curAnimation)
        {
            float p = progress;
            if (p < 0.0f)
            {
                p = 0.0f;
            }
            else if (p > 1.0f)
            {
                p = 1.0f;
            }
            int const fps = m_curAnimation->m_fps;
            int const numFrames = m_curAnimation->m_numFrames;
            int const t = static_cast<int>(static_cast<float>(fps * numFrames) * p);
            int const maxFrame = numFrames - 2;
            m_curAnimFrame = t / fps;
            if (m_curAnimFrame < 0)
            {
                m_curAnimFrame = 0;
            }
            if (m_curAnimFrame > maxFrame)
            {
                m_curAnimFrame = maxFrame;
            }
            m_timeOutToNextFrame = fps - t % fps - 1;
        }
        return 1;
    }

    void AnimInfo::Release()
    {
        m_forModel = 0;
        for (auto& vert : m_meshesVerts)
        {
            delete[] (uint8_t*)vert;
        }
        m_meshesVerts.clear();
        delete m_bonesAnim;
        m_bonesAnim = 0;
        delete m_bonesAnimPrev;
        m_bonesAnimPrev = 0;
    }

    AnimatedModel::Animation const* AnimInfo::GetCurAnimation() const
    {
        return m_curAnimation;
    }

    int AnimInfo::GetStickToLastFrame() const
    {
        return m_stickToLastFrame;
    }

    void
        AnimInfo::RemoveCopyMesh(void**& verts, int*& numVerts, unsigned short**& indxs, int*& numIndxs, CMatrix**& mat)
    {
        // RVA 0x70A730: frees and nulls the arrays CreateCopyMesh handed out.
        delete[] verts;
        verts = nullptr;
        delete[] numVerts;
        numVerts = nullptr;
        delete[] indxs;
        indxs = nullptr;
        delete[] numIndxs;
        numIndxs = nullptr;
        delete[] mat;
        mat = nullptr;
    }

    bool AnimInfo::IsEmpty()
    {
        return this->m_Empty;
    }

    int& AnimInfo::CurAnimFrame()
    {
        return m_curAnimFrame;
    }

    AnimInfo::~AnimInfo()
    {
        Release();
    }

    AnimInfo::AnimInfo()
    {
        this->m_bonesAnim = 0;
        this->m_curAnimation = 0;
        this->m_curAnimFrame = 0;
        this->m_timeOutToNextFrame = 0;
        this->m_forModel = 0;
        this->m_Empty = 1;
        this->m_bonesAnimPrev = 0;
        this->m_stickToLastFrame = -1;
        this->m_lastInterpolationUpdate = -1;
        this->m_curBox.m_box[0] = 0.0;
        this->m_curBox.m_box[1] = 0.0;
        this->m_curBox.m_box[2] = 0.0;
        this->m_curBox.m_box[3] = 0.0;
        this->m_curBox.m_box[4] = 0.0;
        this->m_curBox.m_box[5] = 0.0;
    }

    int AnimInfo::SetAnimation(ActionType action)
    {
        if (m_forModel->m_header.m_numAnimations)
            return SetAnimationIdx(m_forModel->m_animRemap[action]);
        else
            return 0;
    }

    void AnimInfo::MoveFrame(unsigned dti)
    {
        if (m_curAnimation && m_curAnimation->m_fps >= 1)
        {
            auto v5 = m_curAnimation->m_fps * m_curAnimation->m_numFrames;
            if (m_timeOutToNextFrame > v5)
            {
                this->m_timeOutToNextFrame = m_timeOutToNextFrame % v5;
            }
            this->m_timeOutToNextFrame -= dti;
            while (this->m_timeOutToNextFrame <= 0)
            {
                if (++this->m_curAnimFrame >= m_curAnimation->m_numFrames - 1)
                {
                    if (m_curAnimation->m_nextAnimation < 0)
                    {
                        this->m_stickToLastFrame = 1;
                        this->m_curAnimFrame = m_curAnimation->m_numFrames - 1;
                    }
                    else if (m_curAnimation->m_nextAnimation == m_curAnimation - this->m_forModel->m_animations)
                    {
                        this->m_curAnimFrame = 0;
                    }
                    else
                    {
                        SetAnimationIdx(m_curAnimation->m_nextAnimation);
                    }
                }

                this->m_timeOutToNextFrame += m_curAnimation->m_fps;
                if (this->m_isBlending && --this->m_blendFramesNum < 0)
                    this->m_isBlending = 0;
            }
            this->m_lastInterpolationUpdate = -1;
        }
        else
        {
            this->m_curAnimFrame = 0;
        }
    }

    BoneAnim& AnimInfo::GetBoneAnim(unsigned j)
    {
        return m_bonesAnim[j];
    }

    AnimAction* GetAnimActions()
    {
        return actions;
    }

    ActionType GetActionByName(char const* name)
    {
        for (auto const& action : actions)
        {
            if (!strcmp(action.m_name, name))
            {
                return action.m_action;
            }
        }
        return AT_STAND1;
    }
}  // namespace m3d
