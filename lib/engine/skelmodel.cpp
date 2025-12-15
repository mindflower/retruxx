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
#include "math/coremath.h"

namespace m3d
{
    AnimAction actions[] = {
        {"STAN", AT_STAND1},
        {"STAND2", AT_STAND2 }       ,
        {"MOVE1", AT_MOVE1}         ,
        {"MOVE2", AT_MOVE2}         ,
        {"ATTACK1", AT_ATTACK1}       ,
        {"ATTACK2", AT_ATTACK2}       ,
        {"PAIN1", AT_PAIN1 }        ,
        {"PAIN2", AT_PAIN2 }        ,
        {"DEATH1", AT_DEATH1 }       ,
        {"DEATH2", AT_DEATH2 }       ,
        {"BLOCK1", AT_BLOCK1 }       ,
        {"BLOCK2", AT_BLOCK2 }       ,
        {"RESERVED1", AT_RESERVED1 }    ,
        {"RESERVED2", AT_RESERVED2 }    ,
        {"RESERVED3", AT_RESERVED3 }    ,
        {"RESERVED4", AT_RESERVED4 }    ,
        {"SEL1RUS", AT_SND_SELECT }   ,
        {"SEL2RUS", AT_SND_SELECT2}   ,
        {"SEL3RUS", AT_SND_SELECT3}   ,
        {"SEL4RUS", AT_SND_SELECT4}   ,
        {"AT1RUS", AT_SND_ATTACK }   ,
        {"AT2RUS", AT_SND_ATTACK2}   ,
        {"AT3RUS", AT_SND_ATTACK3}   ,
        {"AT4RUS", AT_SND_ATTACK4}   ,
        {"WALK1RUS", AT_SND_WALK }     ,
        {"WALK2RUS", AT_SND_WALK2}     ,
        {"WALK3RUS", AT_SND_WALK3}     ,
        {"WALK4RUS", AT_SND_WALK4}     ,
        {"MOVE1A", AT_SND_MOVE1A }   ,
        {"MOVE2A", AT_SND_MOVE2A }   ,
        {"UNREACH1RUS", AT_SND_UNREACH}   ,
        {"ALLFRAMES", AT_ALL_FRAMES }   ,
    };

    AnimatedModel::Bone::Bone(Bone const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    AnimatedModel::Bone::Bone()
    {
    }

    AnimatedModel::Animation::~Animation()
    {
        RETRUXX_NOT_IMPLEMENTED;
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
        RETRUXX_NOT_IMPLEMENTED;
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
                for (texturesNode->GetFirstChild(fileNode, "file"); !fileNode->IsEmpty(); fileNode->GetNextSibling(fileNode, "file"))
                {
                    CStr name;
                    CStr path;
                    SafeStrAttrib(name, fileNode, "name");
                    SafeStrAttrib(path, fileNode, "path");
                    m_textureFiles.insert({ name, path });
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

    DRAFT_HierGeom const* AnimatedModel::GetHierGeom(unsigned) const
    {
        RETRUXX_NOT_IMPLEMENTED;
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
        const Bone& currentBone = bones[boneIndex];
        const Quaternion& q = currentBone.m_quaternion0;

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
            const Bone& parentBone = bones[parentIndex];
            const Quaternion& parentQ = parentBone.m_quaternion0;

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

            tempResult._11 = parentMatrix._11 * finalMatrix._11 + parentMatrix._12 * finalMatrix._21 + parentMatrix._13 * finalMatrix._31 + parentMatrix._14 * finalMatrix._41;
            tempResult._12 = parentMatrix._11 * finalMatrix._12 + parentMatrix._12 * finalMatrix._22 + parentMatrix._13 * finalMatrix._32 + parentMatrix._14 * finalMatrix._42;
            tempResult._13 = parentMatrix._11 * finalMatrix._13 + parentMatrix._12 * finalMatrix._23 + parentMatrix._13 * finalMatrix._33 + parentMatrix._14 * finalMatrix._43;
            tempResult._14 = parentMatrix._11 * finalMatrix._14 + parentMatrix._12 * finalMatrix._24 + parentMatrix._13 * finalMatrix._34 + parentMatrix._14 * finalMatrix._44;

            tempResult._21 = parentMatrix._21 * finalMatrix._11 + parentMatrix._22 * finalMatrix._21 + parentMatrix._23 * finalMatrix._31 + parentMatrix._24 * finalMatrix._41;
            tempResult._22 = parentMatrix._21 * finalMatrix._12 + parentMatrix._22 * finalMatrix._22 + parentMatrix._23 * finalMatrix._32 + parentMatrix._24 * finalMatrix._42;
            tempResult._23 = parentMatrix._21 * finalMatrix._13 + parentMatrix._22 * finalMatrix._23 + parentMatrix._23 * finalMatrix._33 + parentMatrix._24 * finalMatrix._43;
            tempResult._24 = parentMatrix._21 * finalMatrix._14 + parentMatrix._22 * finalMatrix._24 + parentMatrix._23 * finalMatrix._34 + parentMatrix._24 * finalMatrix._44;

            tempResult._31 = parentMatrix._31 * finalMatrix._11 + parentMatrix._32 * finalMatrix._21 + parentMatrix._33 * finalMatrix._31 + parentMatrix._34 * finalMatrix._41;
            tempResult._32 = parentMatrix._31 * finalMatrix._12 + parentMatrix._32 * finalMatrix._22 + parentMatrix._33 * finalMatrix._32 + parentMatrix._34 * finalMatrix._42;
            tempResult._33 = parentMatrix._31 * finalMatrix._13 + parentMatrix._32 * finalMatrix._23 + parentMatrix._33 * finalMatrix._33 + parentMatrix._34 * finalMatrix._43;
            tempResult._34 = parentMatrix._31 * finalMatrix._14 + parentMatrix._32 * finalMatrix._24 + parentMatrix._33 * finalMatrix._34 + parentMatrix._34 * finalMatrix._44;

            tempResult._41 = parentMatrix._41 * finalMatrix._11 + parentMatrix._42 * finalMatrix._21 + parentMatrix._43 * finalMatrix._31 + parentMatrix._44 * finalMatrix._41;
            tempResult._42 = parentMatrix._41 * finalMatrix._12 + parentMatrix._42 * finalMatrix._22 + parentMatrix._43 * finalMatrix._32 + parentMatrix._44 * finalMatrix._42;
            tempResult._43 = parentMatrix._41 * finalMatrix._13 + parentMatrix._42 * finalMatrix._23 + parentMatrix._43 * finalMatrix._33 + parentMatrix._44 * finalMatrix._43;
            tempResult._44 = parentMatrix._41 * finalMatrix._14 + parentMatrix._42 * finalMatrix._24 + parentMatrix._43 * finalMatrix._34 + parentMatrix._44 * finalMatrix._44;

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

    int nextAnims_0[32] = { 0, -1, 2, 3, 0xA, 0xA, 0xA, 0, -1, -1, 0xA, 0xB, -1, 0,  -1, -1 , 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0,0,0, 0 };

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

            const auto vertsSize = mesh.m_numVertices * mesh.m_VertexTypeSize;
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

            const auto trisCount = mesh.m_numFaces * 3;
            mesh.m_tris = new uint16_t[trisCount];

            const auto trisSize = trisCount * sizeof(uint16_t);
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

                auto v99 = nextAnims_0[v97];
                if (v97 == 32 || (v99 < 0))
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

                const auto materialSize = sizeof(rend::Material);
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
            const auto pointsCount = stream.get<uint32_t>();
            const auto trisCount = stream.get<uint32_t>();

            m_Collision.Points.resize(pointsCount);

            const auto cvectorSize = sizeof(CVector);
            assert(cvectorSize == 12);

            const auto pointsSize = pointsCount * cvectorSize;
            memcpy(m_Collision.Points.data(), stream.getRaw(pointsSize), pointsSize);

            m_Collision.Triangles.resize(trisCount);

            const auto index3Size = sizeof(Index3);
            assert(index3Size == 6);

            const auto trisSize = trisCount * index3Size;
            memcpy(m_Collision.Triangles.data(), stream.getRaw(trisSize), trisSize);
        }

        // Load geometry data (chunk 32)
        if (!stream.setChunk(32u))
        {
            const auto count = stream.get<uint32_t>();
            m_Geoms.resize(count);

            const auto draftGeomSize = sizeof(DRAFT_Geom);
            assert(draftGeomSize == 44);

            const auto size = count * draftGeomSize;
            memcpy(m_Geoms.data(), stream.getRaw(size), size);
        }

        // Load hierarchical geometry (chunk 64)
        if (!stream.setChunk(64u))
        {
            const auto count = stream.get<uint32_t>();
            m_HierGeoms.resize(count);

            const auto draftGeomSize = sizeof(DRAFT_HierGeom);
            assert(draftGeomSize == 48);

            const auto size = count * draftGeomSize;
            memcpy(m_HierGeoms.data(), stream.getRaw(size), size);
        }


        // Load bone bounds (chunk 128)
        if (!stream.setChunk(128u))
        {
            const auto count = stream.get<uint32_t>();

            std::vector<m3d::DRAFT_BoneBounds> bonesBounds;
            bonesBounds.resize(count);

            const auto boneSize = sizeof(DRAFT_BoneBounds);
            assert(boneSize == 28);

            const auto size = count * boneSize;
            memcpy(bonesBounds.data(), stream.getRaw(size), size);

            for (auto& bound : bonesBounds)
            {
                m_BonesBounds[bound.BoneIndex] = bound;
            }
        }

        // Load mesh groups (chunk 240)
        if (!stream.setChunk(240u))
        {
            const auto groupsCount = stream.get<uint32_t>();
            m_MhGroups.resize(groupsCount);

            for (int i = 0; i < groupsCount; ++i)
            {
                auto& group = m_MhGroups[i];
                group.Name = stream.getStr(20);
                group.VisibleAtOnceMin = stream.get<uint32_t>();
                group.VisibleAtOnceMax = stream.get<uint32_t>();

                const auto meshesCount = stream.get<uint32_t>();
                group.MeshesId.resize(meshesCount);
                memcpy(group.MeshesId.data(), stream.getRaw(4 * meshesCount), 4 * meshesCount);

                const auto variantsCount = stream.get<uint32_t>();
                group.m_variants.resize(variantsCount);

                for (int v = 0; v < variantsCount; ++v)
                {
                    const auto variantSize = stream.get<uint32_t>();
                    group.m_variants[v].resize(variantSize);
                    memcpy(group.m_variants[v].data(), stream.getRaw(4 * variantSize), 4 * variantSize);
                }
            }
        }
        return true;
    }

    bool AnimatedModel::LoadSAM(CStr const&, bool)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    AnimatedModel::Bone& AnimatedModel::GetBone(unsigned N) const
    {
        return this->m_boneInitialPos[N];
    }

    unsigned AnimatedModel::GetNumMeshes() const
    {
        return this->m_numMeshes;
    }

    void AnimatedModel::DeleteSkin(unsigned)
    {
        RETRUXX_NOT_IMPLEMENTED;
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
        RETRUXX_NOT_IMPLEMENTED;
    }

    void AnimatedModel::SetComposite(bool isComposite)
    {
        m_composite = isComposite;
    }

    unsigned AnimatedModel::GetNumSkins() const
    {
        return m_Skins.size();
    }
    retruxx::vector<m3d::DSurfaceMaterial, retruxx::allocator<m3d::DSurfaceMaterial>>& AnimatedModel::GetSkin(unsigned int)
    {
        RETRUXX_NOT_IMPLEMENTED;
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

    rend::TexHandle AnimatedModel::GetTexHandle(unsigned, unsigned, unsigned) const
    {
        RETRUXX_NOT_IMPLEMENTED;
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

    int AnimatedModel::GetFrames(int, int) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void AnimatedModel::RenderNormals(CMatrix const&, AnimInfo*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void AnimatedModel::AddSkin(unsigned)
    {
        RETRUXX_NOT_IMPLEMENTED;
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

    int AnimatedModel::ChangeShader(CStr const&, unsigned, unsigned)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool AnimatedModel::bIsPassable() const
    {
        return this->m_passable;
    }

    void AnimatedModel::FromGroupVariants(Configuration&) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void AnimatedModel::ReloadSkins(LoadSkins const& skinsToLoad)
    {
        if (skinsToLoad.loadAllSkins && m_loadSkins.loadAllSkins)
        {
            return;
        }

        for (int i= 0; i < m_Skins.size(); ++i)
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

    void AnimatedModel::RenderHierGeoms(CMatrix const&, AnimInfo*, unsigned)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int AnimatedModel::ChangeTexture(CStr const&, unsigned, unsigned, unsigned)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int AnimatedModel::Render(CMatrix const&, AnimInfo*, unsigned, bool)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int AnimatedModel::Render(CMatrix const&, AnimInfo*, Configuration const&, unsigned)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int AnimatedModel::Render(CMatrix const&, AnimInfo*, Configuration const&, retruxx::vector<DSurfaceMaterial*, retruxx::allocator<DSurfaceMaterial*>> const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int AnimatedModel::GetFps(int) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void AnimatedModel::SetFps(int, short)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void AnimatedModel::RenderLoadPoints(CMatrix const&, AnimInfo*, Configuration const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    DSurfaceMaterial& AnimatedModel::GetMeshMaterial(unsigned, unsigned)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void AnimatedModel::AddTexture(unsigned, unsigned, CStr const&, DRAFT_TextureType, unsigned)
    {
        RETRUXX_NOT_IMPLEMENTED;
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
        const Bone& bone = m_boneInitialPos[boneIndex];

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
            const Bone& parentBone = m_boneInitialPos[parentIndex];

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
            tempResult._11 = parentRot._11 * res._11 + parentRot._12 * res._21 + parentRot._13 * res._31 + parentRot._14 * res._41;
            tempResult._12 = parentRot._11 * res._12 + parentRot._12 * res._22 + parentRot._13 * res._32 + parentRot._14 * res._42;
            tempResult._13 = parentRot._11 * res._13 + parentRot._12 * res._23 + parentRot._13 * res._33 + parentRot._14 * res._43;
            tempResult._14 = parentRot._11 * res._14 + parentRot._12 * res._24 + parentRot._13 * res._34 + parentRot._14 * res._44;

            // Row 2
            tempResult._21 = parentRot._21 * res._11 + parentRot._22 * res._21 + parentRot._23 * res._31 + parentRot._24 * res._41;
            tempResult._22 = parentRot._21 * res._12 + parentRot._22 * res._22 + parentRot._23 * res._32 + parentRot._24 * res._42;
            tempResult._23 = parentRot._21 * res._13 + parentRot._22 * res._23 + parentRot._23 * res._33 + parentRot._24 * res._43;
            tempResult._24 = parentRot._21 * res._14 + parentRot._22 * res._24 + parentRot._23 * res._34 + parentRot._24 * res._44;

            // Row 3
            tempResult._31 = parentRot._31 * res._11 + parentRot._32 * res._21 + parentRot._33 * res._31 + parentRot._34 * res._41;
            tempResult._32 = parentRot._31 * res._12 + parentRot._32 * res._22 + parentRot._33 * res._32 + parentRot._34 * res._42;
            tempResult._33 = parentRot._31 * res._13 + parentRot._32 * res._23 + parentRot._33 * res._33 + parentRot._34 * res._43;
            tempResult._34 = parentRot._31 * res._14 + parentRot._32 * res._24 + parentRot._33 * res._34 + parentRot._34 * res._44;

            // Row 4
            tempResult._41 = parentRot._41 * res._11 + parentRot._42 * res._21 + parentRot._43 * res._31 + parentRot._44 * res._41;
            tempResult._42 = parentRot._41 * res._12 + parentRot._42 * res._22 + parentRot._43 * res._32 + parentRot._44 * res._42;
            tempResult._43 = parentRot._41 * res._13 + parentRot._42 * res._23 + parentRot._43 * res._33 + parentRot._44 * res._43;
            tempResult._44 = parentRot._41 * res._14 + parentRot._42 * res._24 + parentRot._43 * res._34 + parentRot._44 * res._44;

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
    const char* AnimatedModel::GetPath() const
    {
        return m_PathToFile.c_str();
    }

    Aabb& AnimatedModel::GetAabb()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int AnimatedModel::Update(AnimInfo* ai, bool notFirstTime, Configuration* cfg)
    {
        if (!ai->IsEmpty())
        {
            const auto curFrame = m3d::g_Kernel->GetTimer().GetCurFrame();
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
        if (m_hasCubemap)
        {
            RETRUXX_NOT_IMPLEMENTED;
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

    rend::IEffect* AnimatedModel::ApplyMaterial(unsigned)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool AnimatedModel::Save(CStr const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    unsigned AnimatedModel::GetNumBones() const
    {
        RETRUXX_NOT_IMPLEMENTED;
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

    void AnimatedModel::RenderMesh(unsigned, AnimInfo*, rend::IEffect*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void AnimatedModel::RenderCollisions(CMatrix const&, unsigned)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    unsigned AnimatedModel::GetNumMaterials() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    unsigned AnimatedModel::GetNumHierGeoms() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool AnimatedModel::SaveGAM(CStr const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void AnimatedModel::SetCubeMapTexName(CStr const& texName)
    {
        m_cubemapTexPath = texName;
        UnifyFileName(m_cubemapTexPath);
        m_cubemapTexName = NameFromFileName(m_cubemapTexPath);
    }

    void AnimatedModel::LoadSkin(unsigned)
    {
        // TODO: implement AnimatedModel::LoadSkin
        // RETRUXX_NOT_IMPLEMENTED;
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
        return  m_header;
    }

    LoadSkins const& AnimatedModel::GetLoadedSkins()
    {
        return this->m_loadSkins;
    }

    void AnimatedModel::CalculateCfgSize()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool AnimatedModel::Convert(CStr const&, bool, retruxx::vector<DRAFT_Bone, retruxx::allocator<DRAFT_Bone>> const&, retruxx::vector<DMesh, retruxx::allocator<DMesh>> const&, retruxx::vector<DAnimation, retruxx::allocator<DAnimation>> const&)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void AnimatedModel::MatrixForBone(AnimInfo* ai, int curFrame, int boneIndex)
    {
        // TODO: generated code
        if (curFrame < 0 || ai->GetBoneAnim(boneIndex).m_lastUpdatedFrame == curFrame) {
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
            if (curFrame >= 0 && currentBone->m_lastUpdatedFrame == curFrame) {
                break;
            }
            if (currentBone->m_parentIdx < 0) { // Root bone
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
            const Quaternion& rot = bone->m_rotation;
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
        if (!shader && ! m_bVerification)
        {
            auto shaderFile = "data/shaders/" + name + ".fx";
            shader = M3D_RENDERER->NewEffect(shaderFile.c_str(), true);
            if (shader)
            {
                shader->SetDefaultTechnique(true);
            }
        }
    }

    void AnimatedModel::CreateVariants()
    {
        RETRUXX_NOT_IMPLEMENTED;
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

    void AnimatedModel::ReadMaterial(DSurfaceMaterial&, unsigned char*&, bool)
    {
        RETRUXX_NOT_IMPLEMENTED;
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
                const auto& influence = vertexInfluences.m_influences[boneIndex];
                const auto& boneAnim = ai->m_bonesAnim[influence.m_boneIdx];
                const auto& boneMatrix = boneAnim.m_curMatrix;
                float weight = influence.m_boneWeight;

                // Transform position by bone matrix
                CVector weightedPos = influence.m_offsetVec;
                CVector transformedWeightedPos;
                transformedWeightedPos.x = boneMatrix._11 * weightedPos.x + boneMatrix._21 * weightedPos.y + boneMatrix._31 * weightedPos.z + boneMatrix._41;
                transformedWeightedPos.y = boneMatrix._12 * weightedPos.x + boneMatrix._22 * weightedPos.y + boneMatrix._32 * weightedPos.z + boneMatrix._42;
                transformedWeightedPos.z = boneMatrix._13 * weightedPos.x + boneMatrix._23 * weightedPos.y + boneMatrix._33 * weightedPos.z + boneMatrix._43;

                transformedPos += transformedWeightedPos * weight;

                // Transform normal by bone matrix (3x3 rotation part only)
                CVector weightedNormal = influence.m_offsetNormal;
                CVector transformedWeightedNormal;
                transformedWeightedNormal.x = boneMatrix._11 * weightedNormal.x + boneMatrix._21 * weightedNormal.y + boneMatrix._31 * weightedNormal.z;
                transformedWeightedNormal.y = boneMatrix._12 * weightedNormal.x + boneMatrix._22 * weightedNormal.y + boneMatrix._32 * weightedNormal.z;
                transformedWeightedNormal.z = boneMatrix._13 * weightedNormal.x + boneMatrix._23 * weightedNormal.y + boneMatrix._33 * weightedNormal.z;

                transformedNormal += transformedWeightedNormal * weight;
            }

            // Write transformed vertex data to output buffer
            float* outputVertex = reinterpret_cast<float*>(
                static_cast<char*>(dstVerts) + vertexIndex * mesh.m_VertexTypeSize);

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

    void AnimatedModel::DrawBones(AnimInfo*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void AnimatedModel::CheckConfigurations()
    {
        RETRUXX_NOT_IMPLEMENTED;
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

    bool AnimInfo::IsAnimation(ActionType)
    {
	    RETRUXX_NOT_IMPLEMENTED;
    }

    void AnimInfo::CreateCopyMesh(int&, void**&, int*&, unsigned short**&, int*&, bool*&, CMatrix**&,
	    retruxx::vector<rend::VertexType, retruxx::allocator<rend::VertexType>>&,
	    retruxx::vector<unsigned, retruxx::allocator<unsigned>>&, int*)
    {
	    RETRUXX_NOT_IMPLEMENTED;
    }

    void AnimInfo::SetEmpty()
    {
	    RETRUXX_NOT_IMPLEMENTED;
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
                const auto& currentTransform = currentFrameData[nodeIndex];
                boneAnim.m_rotation = Quaternion(currentTransform.qx, currentTransform.qy, currentTransform.qz, currentTransform.qw);
                boneAnim.m_translation = CVector(currentTransform.tx, currentTransform.ty, currentTransform.tz);
            }
            else
            {
                // Interpolate between current and next frame
                const auto& currentTransform = currentFrameData[nodeIndex];
                const auto& nextTransform = nextFrameData[nodeIndex];

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
            float blendFactor = 1.0f - (float)((float)(m_timeOutToNextFrame + anim->m_fps * (m_blendFramesNum - 1)) /
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
        const auto numAnimations = m_forModel->m_header.m_numAnimations;
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

    AnimatedModel::Mesh const& AnimInfo::GetMesh(unsigned) const
    {
	    RETRUXX_NOT_IMPLEMENTED;
    }

    void AnimInfo::CreateFor(AnimatedModel* am)
    {
        // TODO: implement AnimInfo::CreateFor
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

    void AnimInfo::SetBoneCurMatrix(unsigned, CMatrix const&)
    {
	    RETRUXX_NOT_IMPLEMENTED;
    }

    CMatrix const& AnimInfo::GetCurrentLoadpointMatrix(int lpId) const
    {
        if (lpId < 0 || this->m_Empty)
            return this->m_forModel->m_Dummy;
        else
            return this->m_bonesAnim[lpId].m_curMatrix;
    }

    int AnimInfo::SetCurFrame(float)
    {
	    RETRUXX_NOT_IMPLEMENTED;
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

    void AnimInfo::RemoveCopyMesh(void**&, int*&, unsigned short**&, int*&, CMatrix**&)
    {
	    RETRUXX_NOT_IMPLEMENTED;
    }

    bool AnimInfo::IsEmpty()
    {
        return this->m_Empty;
    }

    int& AnimInfo::CurAnimFrame()
    {
	    RETRUXX_NOT_IMPLEMENTED;
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
}
