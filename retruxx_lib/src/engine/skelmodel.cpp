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

namespace m3d
{
    AnimatedModel::Bone::Bone(Bone const&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    AnimatedModel::Bone::Bone()
    {
    }

    AnimatedModel::Animation::~Animation()
    {
        throw retruxx::logic_error("Not implemented");
    }

    AnimatedModel::Animation::Animation()
    {
        throw retruxx::logic_error("Not implemented");
    }

    void AnimatedModel::Mesh::ComputeShadowsRelatedStuff()
    {
        throw retruxx::logic_error("Not implemented");
    }

    AnimatedModel::Mesh::~Mesh()
    {
        throw retruxx::logic_error("Not implemented");
    }

    AnimatedModel::Mesh::Mesh()
    {
        throw retruxx::logic_error("Not implemented");
    }

    DSurfaceMaterial& AnimatedModel::Mesh::GetMaterial(unsigned) const
    {
        throw retruxx::logic_error("Not implemented");
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
                xml->GetFirstChild(fileNode, "file");
                //TODO: check this
                for (; !fileNode->IsEmpty(); fileNode->GetNextSibling(fileNode, "file"))
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
        throw retruxx::logic_error("Not implemented");
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

    AnimatedModel::Mesh& AnimatedModel::GetMesh(unsigned)
    {
        throw retruxx::logic_error("Not implemented");
    }

    AnimatedModel::Mesh const& AnimatedModel::GetMesh(unsigned) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    DRAFT_BoneBounds const& AnimatedModel::GetBoneBounds(unsigned)
    {
        throw retruxx::logic_error("Not implemented");
    }

    CMatrix AnimatedModel::GetBoneMatrix(int) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void AnimatedModel::SetSkinsToLoad(LoadSkins const& skins)
    {
        m_loadSkins = skins;
    }

    bool AnimatedModel::LoadGAM(CStr const& fileName, bool bForceNextAnimation)
    {
        //TODO: implement AnimatedModel::LoadGAM
        return true;
        if (m_bVerification)
        {
            M3D_LOG_ERR("LoadGAM does work not for verification of models!!!");
            return false;
        }
        m_Name = NameFromFileName(fileName).c_str();
        m_PathToFile = DirectoryFromFileName(fileName).c_str();

        fs::auxTaggedFile taggedFile;
        if (taggedFile.Open(fileName.c_str(), fs::auxTaggedFile::PROCESS_NORMAL_IGNORE_CRC))
        {
            M3D_LOG_ERR("AnimatedModel::LoadGAM -- cannot open file " + fileName);
            return false;
        }
        char* formatTitle = nullptr;
        taggedFile.getFormatTitle(&formatTitle);
        if (strcmp(formatTitle, "IVR"))
        {
            M3D_LOG_ERR("Wrong file format!!!");
            return false;
        }
        unsigned formatVersion = 0;
        taggedFile.getFormatVersion(formatVersion);
        if (formatVersion <= 1)
        {  
            void* data = nullptr;
            taggedFile.getChunkData(1, &data);
            auto* shorts = static_cast<unsigned short*>(data);
            m_header.m_numTriMeshes = shorts[0];
            m_header.m_numSkinMeshes = shorts[1];
            m_header.m_numStaticMeshes = shorts[2];
            m_header.m_numAnimations = shorts[3];
            unsigned numMaterials = shorts[4];
            m_header.m_numNodes = shorts[5];
            m_cfgSize = static_cast<unsigned*>(data)[3];

            taggedFile.getChunkData(2, &data);
            m_boneInitialPos = new Bone[m_header.m_numNodes];
            m_initialBoneInvMatrices = new CMatrix[m_header.m_numNodes];
            for (unsigned i = 0; i < m_header.m_numNodes; ++i)
            {
                auto charData = static_cast<char*>(data);
                strcpy(m_boneInitialPos[i].m_boneName, charData);
                m_boneInitialPos[i].m_ownIdx = i;
                m_boneInitialPos[i].m_parentIdx = *((int*)charData + 10);
                m_boneInitialPos[i].m_translation0 = *(CVector*)(charData + 44);
                m_boneInitialPos[i].m_quaternion0 = *(Quaternion*)(charData + 56);

                auto invMat = (float*)(charData + 76);
                for (unsigned j = 0; j < 4; ++j)
                {
                    m_initialBoneInvMatrices[i].m[j][0] = *(invMat - 1);
                    m_initialBoneInvMatrices[i].m[j][1] = *(invMat);
                    m_initialBoneInvMatrices[i].m[j][2] = *(invMat + 1);
                    m_initialBoneInvMatrices[i].m[j][3] = *(invMat + 2);
                    invMat += 4;
                }
                charData += 136;
            }

            taggedFile.getChunkData(4, &data);
            m_numMeshes = m_header.m_numStaticMeshes + m_header.m_numTriMeshes + m_header.m_numSkinMeshes;
            m_meshes = new Mesh[m_numMeshes];
            auto charData = static_cast<char*>(data);
            for (unsigned i = 0; i < m_numMeshes; ++i)
            {
                strcpy(m_meshes[i].m_meshName, charData);
                m_meshes[i].meshId = i;
                m_meshes[i].m_meshType = *((int*)charData + 10);
                m_meshes[i].m_numNode = *((int*)charData + 11);
                m_meshes[i].groupId = *((int*)charData + 12);
                m_meshes[i].m_MaterialNumber = *((int*)charData + 13);
                m_meshes[i].m_VertexTypeSize = *((int*)charData + 14);
                m_meshes[i].m_VertexType = (rend::VertexType)*((int*)charData + 15);
                m_meshes[i].m_numVertices = *((int*)charData + 16);
                m_meshes[i].m_numFaces = *((int*)charData + 17);
                m_meshes[i].m_pModelSkins = &m_Skins;
                charData += 72;

                //TODO: check this
                m_meshes[i].m_verts = new unsigned char[m_meshes[i].m_numVertices * m_meshes[i].m_VertexTypeSize];
                memcpy(m_meshes[i].m_verts, charData, m_meshes[i].m_numVertices * m_meshes[i].m_VertexTypeSize);
                charData += m_meshes[i].m_numVertices * m_meshes[i].m_VertexTypeSize;
                if (m_meshes[i].m_meshType == 4)
                {
                    m_meshes[i].m_VbPoolField = M3D_APP->m_renderer->AddVbPoolField(m_meshes[i].m_VertexType, m_meshes[i].m_numVertices);
                    memcpy(
                        M3D_APP->m_renderer->LockVbPoolField(m_meshes[i].m_VbPoolField),
                        m_meshes[i].m_verts,
                        m_meshes[i].m_numVertices * m_meshes[i].m_VertexTypeSize
                    );
                    M3D_APP->m_renderer->UnlockVbPoolField(m_meshes[i].m_VbPoolField);
                }
                if (m_meshes[i].m_meshType == 1)
                {
                    m_meshes[i].m_VbPoolField = M3D_APP->m_renderer->AddVbPoolField(m_meshes[i].m_VertexType, m_meshes[i].m_numVertices);
                    memcpy(
                        M3D_APP->m_renderer->LockVbPoolField(m_meshes[i].m_VbPoolField),
                        charData,
                        m_meshes[i].m_numVertices * m_meshes[i].m_VertexTypeSize
                    );
                    M3D_APP->m_renderer->UnlockVbPoolField(m_meshes[i].m_VbPoolField);
                    charData += m_meshes[i].m_numVertices * m_meshes[i].m_VertexTypeSize;
                }
                if (m_meshes[i].m_meshType == 1)
                {
                    throw retruxx::logic_error("Not implemented");
                }
                m_meshes[i].m_tris = new  unsigned short[3 * m_meshes[i].m_numFaces];
                memcpy(m_meshes[i].m_tris, data, 2 * 3 * m_meshes[i].m_numFaces);
                charData += 2 * 3 * m_meshes[i].m_numFaces;
                m_meshes[i].m_numDrawIndices = 3 * m_meshes[i].m_numFaces;
                m_meshes[i].m_drawIndices = m_meshes[i].m_tris;
                m_meshes[i].m_numDrawVerts = m_meshes[i].m_numVertices;
                m_meshes[i].m_IbPoolField = M3D_APP->m_renderer->AddIbPoolField(m_meshes[i].m_numDrawIndices);
                memcpy(
                    M3D_APP->m_renderer->LockIbPoolField(m_meshes[i].m_IbPoolField),
                    m_meshes[i].m_drawIndices,
                    m_meshes[i].m_numDrawIndices * 2
                );
                M3D_APP->m_renderer->UnlockIbPoolField(m_meshes[i].m_IbPoolField);
                //TODO: check this
                charData += 188;
            }
            m_box.m_box[0] = *(float*)charData;
            m_box.m_box[1] = *(float*)charData + 1;
            m_box.m_box[2] = *(float*)charData + 2;
            m_box.m_box[3] = *(float*)charData + 3;
            m_box.m_box[4] = *(float*)charData + 4;
            m_box.m_box[5] = *(float*)charData + 5;

            taggedFile.getChunkData(8, &data);
            charData = static_cast<char*>(data);
            m_animations = new Animation[m_header.m_numAnimations];
            for (unsigned i = 0; i < m_header.m_numAnimations; ++i)
            {
                m_animations[i].m_fps = *(short*)(charData + 27);
                m_animations[i].m_numFrames = *(short*)(charData + 25);
                m_animations[i].m_nextAnimation = *(short*)(charData + 29);
                m_animations[i].m_numChanges = *(short*)(charData + 31);
                m_animations[i].m_action = (ActionType)*(int*)(charData + 35);
                m_animations[i].m_numNodes = *(int*)(charData + 33);
                strcpy(m_animations[i].m_name, charData);
                charData += 39;

                m_animations[i].m_hierChanges = new HierarchyChange[m_animations[i].m_numChanges];
                auto hierData = charData + 6;
                for (unsigned j = 0; j < m_animations[i].m_numChanges; ++j)
                {
                    m_animations[i].m_hierChanges[j].ownIdx = *((short*)hierData - 1);
                    m_animations[i].m_hierChanges[j].newParentIdx = *(short*)hierData;
                    m_animations[i].m_hierChanges[j].changeType = (DRAFT_Change)*(int*)(hierData - 6);
                    hierData += 8;
                }
                charData += 8 * m_animations[i].m_numChanges;

                auto const nodesPositions = m_animations[i].m_numNodes * m_animations[i].m_numNodes;
                auto nodesData = static_cast<char*>(data) + 36;
                m_animations[i].m_nodesPositions = new AnimationTransform[nodesPositions];
                auto nodesIdx = 0;
                for (int j = 0; j < nodesPositions; j+=4)
                {
                    m_animations[i].m_nodesPositions[j].idx = *((short*)nodesData - 18);
                    m_animations[i].m_nodesPositions[j].tx = *(float*)(nodesData - 34);
                    m_animations[i].m_nodesPositions[j].ty = *(float*)(nodesData - 30);
                    m_animations[i].m_nodesPositions[j].tz = *(float*)(nodesData - 26);
                    m_animations[i].m_nodesPositions[j].qx = *(float*)(nodesData - 22);
                    m_animations[i].m_nodesPositions[j].qy = *(float*)(nodesData - 18);
                    m_animations[i].m_nodesPositions[j].qz = *(float*)(nodesData - 14);
                    m_animations[i].m_nodesPositions[j].qw = *(float*)(nodesData - 10);
                    m_animations[i].m_nodesPositions[j + 1].idx = *((short*)nodesData - 3);
                    m_animations[i].m_nodesPositions[j + 1].tx = *((float*)nodesData - 1);
                    m_animations[i].m_nodesPositions[j + 1].ty = *(float*)nodesData;
                    m_animations[i].m_nodesPositions[j + 1].tz = *((float*)nodesData + 1);
                    m_animations[i].m_nodesPositions[j + 1].qx = *((float*)nodesData + 2);
                    m_animations[i].m_nodesPositions[j + 1].qy = *((float*)nodesData + 3);
                    m_animations[i].m_nodesPositions[j + 1].qz = *((float*)nodesData + 4);
                    m_animations[i].m_nodesPositions[j + 1].qw = *((float*)nodesData + 5);
                    m_animations[i].m_nodesPositions[j+2].idx = *((short*)nodesData + 12);
                    m_animations[i].m_nodesPositions[j+2].tx = *(float*)(nodesData + 26);
                    m_animations[i].m_nodesPositions[j+2].ty = *(float*)(nodesData + 30);
                    m_animations[i].m_nodesPositions[j+2].tz = *(float*)(nodesData + 34);
                    m_animations[i].m_nodesPositions[j+2].qx = *(float*)(nodesData + 38);
                    m_animations[i].m_nodesPositions[j+2].qy = *(float*)(nodesData + 42);
                    m_animations[i].m_nodesPositions[j+2].qz = *(float*)(nodesData + 46);
                    m_animations[i].m_nodesPositions[j+2].qw = *(float*)(nodesData + 50);
                    m_animations[i].m_nodesPositions[j+3].idx = *((short*)nodesData + 27);
                    m_animations[i].m_nodesPositions[j+3].tx = *((float*)nodesData + 14);
                    m_animations[i].m_nodesPositions[j+3].ty = *((float*)nodesData + 15);
                    m_animations[i].m_nodesPositions[j+3].tz = *((float*)nodesData + 16);
                    m_animations[i].m_nodesPositions[j+3].qx = *((float*)nodesData + 17);
                    m_animations[i].m_nodesPositions[j+3].qy = *((float*)nodesData + 18);
                    m_animations[i].m_nodesPositions[j+3].qz = *((float*)nodesData + 19);
                    m_animations[i].m_nodesPositions[j+3].qw = *((float*)nodesData + 20);
                    nodesData += 120;
                    nodesIdx += 4;
                }
                if (nodesIdx < nodesPositions)
                {
                    auto nodesDataInt = (unsigned int*)((char*)data + 30 * nodesIdx + 6);
                    for (int j = nodesPositions - nodesIdx; j >= 0; --j)
                    {
                        m_animations[i].m_nodesPositions[j].idx = *((short*)nodesDataInt - 3);
                        m_animations[i].m_nodesPositions[j].tx = *(float*)(nodesDataInt - 1);
                        m_animations[i].m_nodesPositions[j].ty = *(float*)nodesDataInt;
                        m_animations[i].m_nodesPositions[j].tz = (float)nodesDataInt[1];
                        m_animations[i].m_nodesPositions[j].qx = (float)nodesDataInt[2];
                        m_animations[i].m_nodesPositions[j].qy = (float)nodesDataInt[3];
                        m_animations[i].m_nodesPositions[j].qz = (float)nodesDataInt[4];
                        m_animations[i].m_nodesPositions[j].qw = *((float*)nodesDataInt + 5);;
                        nodesDataInt = (unsigned*)((char*)nodesDataInt + 30);
                    }
                }
                data = (char*)data + (30 * nodesPositions);
                //data = &data[30 * nodesPositions];
            }
            for (auto& remap : m_animRemap)
            {
                throw retruxx::logic_error("Not implemented");
            }
            for (unsigned i = 0; i < m_header.m_numAnimations; ++i)
            {
                throw retruxx::logic_error("Not implemented");
            }

            taggedFile.getChunkData(0xFu, &data);
            m_Skins.resize(*(int*)data);
            data = (char*)data + 4;


        }
        throw retruxx::logic_error("Not implemented");
    }

    bool AnimatedModel::LoadSAM(CStr const&, bool)
    {
        throw retruxx::logic_error("Not implemented");
    }

    AnimatedModel::Bone& AnimatedModel::GetBone(unsigned) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    unsigned AnimatedModel::GetNumMeshes() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void AnimatedModel::DeleteSkin(unsigned)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void AnimatedModel::UnloadSkin(unsigned)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void AnimatedModel::UpdateTexturesFilter()
    {
        throw retruxx::logic_error("Not implemented");
    }

    void AnimatedModel::SetComposite(bool isComposite)
    {
        m_composite = isComposite;
    }

    unsigned AnimatedModel::GetNumSkins() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    AnimatedModel::~AnimatedModel()
    {
        throw retruxx::logic_error("Not implemented");
    }

    rend::TexHandle AnimatedModel::GetTexHandle(unsigned, unsigned, unsigned) const
    {
        throw retruxx::logic_error("Not implemented");
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
        throw retruxx::logic_error("Not implemented");
    }

    int AnimatedModel::GetFrames(int, int) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void AnimatedModel::RenderNormals(CMatrix const&, AnimInfo*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void AnimatedModel::AddSkin(unsigned)
    {
        throw retruxx::logic_error("Not implemented");
    }

    int AnimatedModel::GetLoadPointIdByName(char const* lpName) const
    {
        auto m_numNodes = this->m_header.m_numNodes;
        auto v3 = 0;
        if (m_numNodes <= 0)
            return -1;
        for (auto i = this->m_boneInitialPos; strcmp(i->m_boneName, lpName); ++i)
        {
            if (++v3 >= m_numNodes)
                return -1;
        }
        return this->m_boneInitialPos[v3].m_ownIdx;
    }

    DRAFT_Geom const* AnimatedModel::GetGeom(unsigned) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    int AnimatedModel::ChangeShader(CStr const&, unsigned, unsigned)
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool AnimatedModel::bIsPassable() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void AnimatedModel::FromGroupVariants(Configuration&) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void AnimatedModel::ReloadSkins(LoadSkins const&)
    {
        // TODO: implement AnimatedModel::ReloadSkins
        // throw retruxx::logic_error("Not implemented");
    }

    void AnimatedModel::CalculateMeshes(Configuration&) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    unsigned int AnimatedModel::GetNumAnimations() const
    {
        return m_header.m_numAnimations;
    }

    void AnimatedModel::RenderHierGeoms(CMatrix const&, AnimInfo*, unsigned)
    {
        throw retruxx::logic_error("Not implemented");
    }

    int AnimatedModel::ChangeTexture(CStr const&, unsigned, unsigned, unsigned)
    {
        throw retruxx::logic_error("Not implemented");
    }

    int AnimatedModel::Render(CMatrix const&, AnimInfo*, unsigned, bool)
    {
        throw retruxx::logic_error("Not implemented");
    }

    int AnimatedModel::Render(CMatrix const&, AnimInfo*, Configuration const&, unsigned)
    {
        throw retruxx::logic_error("Not implemented");
    }

    int AnimatedModel::Render(CMatrix const&, AnimInfo*, Configuration const&, retruxx::vector<DSurfaceMaterial*, retruxx::allocator<DSurfaceMaterial*>> const&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    int AnimatedModel::GetFps(int) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void AnimatedModel::SetFps(int, short)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void AnimatedModel::RenderLoadPoints(CMatrix const&, AnimInfo*, Configuration const&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    DSurfaceMaterial& AnimatedModel::GetMeshMaterial(unsigned, unsigned)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void AnimatedModel::AddTexture(unsigned, unsigned, CStr const&, DRAFT_TextureType, unsigned)
    {
        throw retruxx::logic_error("Not implemented");
    }

    DCollisionData const& AnimatedModel::GetCollisionTrimesh() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    int AnimatedModel::GetBoneMatrixByName(CStr const&, CMatrix&, bool) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void AnimatedModel::FromCfgNum(Configuration&) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    char const* AnimatedModel::GetName() const
    {
        return m_Name.c_str();
    }

    Aabb& AnimatedModel::GetAabb()
    {
        throw retruxx::logic_error("Not implemented");
    }

    int AnimatedModel::Update(AnimInfo*, bool, Configuration*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void AnimatedModel::UpdateCubemap()
    {
        // TODO: implement AnimatedModel::UpdateCubemap
        //throw retruxx::logic_error("Not implemented");
    }

    unsigned AnimatedModel::GetNumGeoms() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    unsigned AnimatedModel::GetCfgSize() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    MeshesGroup const& AnimatedModel::GetGroup(unsigned) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    DSurfaceMaterial& AnimatedModel::GetMaterial(unsigned, unsigned)
    {
        throw retruxx::logic_error("Not implemented");
    }

    rend::IEffect* AnimatedModel::ApplyMaterial(DSurfaceMaterial&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    rend::IEffect* AnimatedModel::ApplyMaterial(unsigned)
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool AnimatedModel::Save(CStr const&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    unsigned AnimatedModel::GetNumBones() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void AnimatedModel::UpdateVertices(AnimInfo*, bool, Configuration*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void AnimatedModel::RenderMesh(unsigned, AnimInfo*, rend::IEffect*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void AnimatedModel::RenderCollisions(CMatrix const&, unsigned)
    {
        throw retruxx::logic_error("Not implemented");
    }

    unsigned AnimatedModel::GetNumMaterials() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    unsigned AnimatedModel::GetNumHierGeoms() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool AnimatedModel::SaveGAM(CStr const&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void AnimatedModel::SetCubeMapTexName(CStr const& texName)
    {
        m_cubemapTexPath = texName;
        UnifyFileName(m_cubemapTexPath);
        m_cubemapTexName = NameFromFileName(m_cubemapTexPath);
    }

    void AnimatedModel::LoadSkin(unsigned)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void AnimatedModel::SetNextForAnimation(ActionType, int)
    {
        throw retruxx::logic_error("Not implemented");
    }

    LoadSkins const& AnimatedModel::GetLoadedSkins()
    {
        throw retruxx::logic_error("Not implemented");
    }

    void AnimatedModel::CalculateCfgSize()
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool AnimatedModel::Convert(CStr const&, bool, retruxx::vector<DRAFT_Bone, retruxx::allocator<DRAFT_Bone>> const&, retruxx::vector<DMesh, retruxx::allocator<DMesh>> const&, retruxx::vector<DAnimation, retruxx::allocator<DAnimation>> const&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void AnimatedModel::MatrixForBone(AnimInfo*, int, int)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void AnimatedModel::NewEffect(retruxx::string const&, rend::IEffect*&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void AnimatedModel::CreateVariants()
    {
        throw retruxx::logic_error("Not implemented");
    }

    void AnimatedModel::SetTexture(CStr const&, DRAFT_TextureType, rend::TexHandle&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void AnimatedModel::ReadMaterial(DSurfaceMaterial&, unsigned char*&, bool)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void AnimatedModel::VertsForSkinmesh(AnimInfo*, Mesh&, void*, bool)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void AnimatedModel::DrawBones(AnimInfo*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void AnimatedModel::CheckConfigurations()
    {
        throw retruxx::logic_error("Not implemented");
    }

    CStr AnimatedModel::DefinePathToTexture(CStr const&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool AnimInfo::IsAnimation(ActionType)
    {
	    throw retruxx::logic_error("Not implemented");
    }

    void AnimInfo::CreateCopyMesh(int&, void**&, int*&, unsigned short**&, int*&, bool*&, CMatrix**&,
	    retruxx::vector<rend::VertexType, retruxx::allocator<rend::VertexType>>&,
	    retruxx::vector<unsigned, retruxx::allocator<unsigned>>&, int*)
    {
	    throw retruxx::logic_error("Not implemented");
    }

    void AnimInfo::SetEmpty()
    {
	    throw retruxx::logic_error("Not implemented");
    }

    void AnimInfo::InterpolateBones(int)
    {
	    throw retruxx::logic_error("Not implemented");
    }

    int AnimInfo::SetAnimationIdx(int)
    {
	    throw retruxx::logic_error("Not implemented");
    }

    AnimatedModel::Mesh const& AnimInfo::GetMesh(unsigned) const
    {
	    throw retruxx::logic_error("Not implemented");
    }

    void AnimInfo::CreateFor(AnimatedModel* am)
    {
        Release();
        this->m_curBox = am->m_box;
        this->m_forModel = am;
        if (am->GetNumAnimations())
        {
            throw retruxx::logic_error("Not implemented");
        }
        else
        {
            m_Empty = true;
        }
    }

    void AnimInfo::SetBoneCurMatrix(unsigned, CMatrix const&)
    {
	    throw retruxx::logic_error("Not implemented");
    }

    CMatrix const& AnimInfo::GetCurrentLoadpointMatrix(int) const
    {
	    throw retruxx::logic_error("Not implemented");
    }

    int AnimInfo::SetCurFrame(float)
    {
	    throw retruxx::logic_error("Not implemented");
    }

    void AnimInfo::Release()
    {
        m_forModel = 0;
        for (auto& vert : m_meshesVerts)
        {
            delete vert;
        }
        m_meshesVerts.clear();
        delete m_bonesAnim;
        m_bonesAnim = 0;
        delete m_bonesAnimPrev;
        m_bonesAnimPrev = 0;
    }

    AnimatedModel::Animation const* AnimInfo::GetCurAnimation() const
    {
	    throw retruxx::logic_error("Not implemented");
    }

    int AnimInfo::GetStickToLastFrame() const
    {
	    throw retruxx::logic_error("Not implemented");
    }

    void AnimInfo::RemoveCopyMesh(void**&, int*&, unsigned short**&, int*&, CMatrix**&)
    {
	    throw retruxx::logic_error("Not implemented");
    }

    bool AnimInfo::IsEmpty()
    {
        return this->m_Empty;
    }

    int& AnimInfo::CurAnimFrame()
    {
	    throw retruxx::logic_error("Not implemented");
    }

    AnimInfo::~AnimInfo()
    {
	    throw retruxx::logic_error("Not implemented");
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

    int AnimInfo::SetAnimation(ActionType)
    {
	    throw retruxx::logic_error("Not implemented");
    }

    void AnimInfo::MoveFrame(unsigned)
    {
	    throw retruxx::logic_error("Not implemented");
    }

    BoneAnim& AnimInfo::GetBoneAnim(unsigned)
    {
	    throw retruxx::logic_error("Not implemented");
    }

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
        {nullptr, AT_NUMTYPES  },
    };


    AnimAction* GetAnimActions()
    {
        return actions;
    }
}
