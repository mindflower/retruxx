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
        throw retruxx::logic_error("Not implemented");
    }

    AnimatedModel::Mesh::~Mesh()
    {
        throw retruxx::logic_error("Not implemented");
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

    int nextAnims_0[32] = { 0, -1, 2, 3, 0xA, 0xA, 0xA, 0, -1, -1, 0xA, 0xB, -1, 0,  -1, -1 , 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0,0,0, 0 };

    namespace 
    {
        class TaggedFileReader
        {
        public:
            TaggedFileReader(m3d::fs::auxTaggedFile& file) : m_file(file)
            {
            }

            fs::auxTaggedFile::eError setChunk(unsigned id)
            {
                return m_file.getChunkData(id, &m_data);
            }

            template <class T>
            T get(const size_t byteCount)
            {
                auto res = *static_cast<T*>(m_data);
                m_data = static_cast<uint8_t*>(m_data) + byteCount;
                return res;
            }

            template <class T>
            T get()
            {
                return get<T>(sizeof(T));
            }

            const char* getStr(const size_t byteCount)
            {
                auto res = static_cast<const char*>(m_data);
                m_data = static_cast<uint8_t*>(m_data) + byteCount;
                return res;
            }

            void* getRaw(const size_t byteCount)
            {
                auto res = m_data;
                m_data = static_cast<uint8_t*>(m_data) + byteCount;
                return res;
            }

        private:
            m3d::fs::auxTaggedFile& m_file;
            void* m_data = nullptr;
        };
    }

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

        TaggedFileReader stream(file);

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
                auto& surfaceMaterial = m_Skins[skinsIdx][matIdx];

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
            m_Collision.Points.resize(pointsCount);

            const auto cvectorSize = sizeof(CVector);
            assert(cvectorSize == 12);

            const auto pointsSize = pointsCount * cvectorSize;
            memcpy(m_Collision.Points.data(), stream.getRaw(pointsSize), pointsSize);

            const auto trisCount = stream.get<uint32_t>();
            m_Collision.Triangles.resize(trisCount);

            const auto index3Size = sizeof(Index3);
            assert(cvectorSize == 6);

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
        return 1;
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
        return m_Skins.size();
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
        throw retruxx::logic_error("Not implemented");
    }

    bool AnimatedModel::bIsPassable() const
    {
        return this->m_passable;
    }

    void AnimatedModel::FromGroupVariants(Configuration&) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void AnimatedModel::ReloadSkins(LoadSkins const& skinsToLoad)
    {
        if (skinsToLoad.loadAllSkins && m_loadSkins.loadAllSkins)
        {
            return;
        }

        for (auto& skin : m_Skins)
        {
            
        }
        throw retruxx::logic_error("Not implemented");
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
        return this->m_Collision;
    }

    int AnimatedModel::GetBoneMatrixByName(CStr const&, CMatrix&, bool) const
    {
        throw retruxx::logic_error("Not implemented");
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
        if (m_hasCubemap)
        {
            throw retruxx::logic_error("Not implemented");
        }
    }

    unsigned AnimatedModel::GetNumGeoms() const
    {
        return m_Geoms.size();
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
        throw retruxx::logic_error("Not implemented");
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
        // // TODO: implement AnimInfo::SetAnimationIdx
	    //throw retruxx::logic_error("Not implemented");
        return 0;
    }

    AnimatedModel::Mesh const& AnimInfo::GetMesh(unsigned) const
    {
	    throw retruxx::logic_error("Not implemented");
    }

    void AnimInfo::CreateFor(AnimatedModel* am)
    {
        // TODO: implement AnimInfo::CreateFor
        Release();
        this->m_curBox = am->m_box;
        this->m_forModel = am;
         if (false)
        //if (am->GetNumAnimations())
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

    void AnimInfo::MoveFrame(unsigned)
    {
	    throw retruxx::logic_error("Not implemented");
    }

    BoneAnim& AnimInfo::GetBoneAnim(unsigned)
    {
	    throw retruxx::logic_error("Not implemented");
    }

    AnimAction* GetAnimActions()
    {
        return actions;
    }
}
