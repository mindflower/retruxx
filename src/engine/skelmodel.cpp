#include <skelmodel.h>
#include <core/kernel.h>
#include <core/log.h>
#include <core/scoped_ptr.h>
#include <file/fileserver.h>
#include <file/filestream.h>
#include <file/tagged.h>

namespace m3d
{
    LoadSkins::LoadSkins()
    {
    }

    AnimatedModel::Bone::Bone(Bone const&)
    {
        throw std::logic_error("Not implemented");
    }

    AnimatedModel::Bone::Bone()
    {
        throw std::logic_error("Not implemented");
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
                xml->GetFirstChild_(texturesNode, "Textures");
                xml->GetFirstChild_(fileNode, "file");
                //TODO: check this
                for (; !fileNode->IsEmpty(); fileNode->GetNextSibling_(fileNode, "file"))
                {
                    CStr name;
                    CStr path;
                    SafeStrAttrib(name, fileNode, "name");
                    SafeStrAttrib(path, fileNode, "path");
                    m_textureFiles.emplace(name, path);
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
        throw std::logic_error("Not implemented");
    }

    AnimatedModel::AnimatedModel()
    {
        memset(&m_Dummy, 0, sizeof(m_Dummy));
        m_Dummy._44 = 1.0;
        m_Dummy._33 = 1.0;
        m_Dummy._22 = 1.0;
        m_Dummy._11 = 1.0;
    }

    AnimatedModel::Mesh& AnimatedModel::GetMesh(unsigned)
    {
        throw std::logic_error("Not implemented");
    }

    AnimatedModel::Mesh const& AnimatedModel::GetMesh(unsigned) const
    {
        throw std::logic_error("Not implemented");
    }

    DRAFT_BoneBounds const& AnimatedModel::GetBoneBounds(unsigned)
    {
        throw std::logic_error("Not implemented");
    }

    CMatrix AnimatedModel::GetBoneMatrix(int) const
    {
        throw std::logic_error("Not implemented");
    }

    void AnimatedModel::SetSkinsToLoad(LoadSkins const&)
    {
        throw std::logic_error("Not implemented");
    }

    bool AnimatedModel::LoadGAM(CStr const& fileName, bool bForceNextAnimation)
    {
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
            if (m_header.m_numNodes > 0)
            {
                for (unsigned i = 0; i < m_header.m_numNodes; ++i)
                {
                    auto charData = static_cast<char*>(data);
                    strcpy(m_boneInitialPos[i].m_boneName, charData);
                    m_boneInitialPos[i].m_ownIdx = i;
                    m_boneInitialPos[i].m_parentIdx = *((int*)charData + 10);
                    m_boneInitialPos[i].m_translation0 = *(CVector*)(charData + 44);
                    m_boneInitialPos[i].m_quaternion0 = *(Quaternion*)(charData + 56);
                    
                }
            }
        }
        throw std::logic_error("Not implemented");
    }

    bool AnimatedModel::LoadSAM(CStr const&, bool)
    {
        throw std::logic_error("Not implemented");
    }

    AnimatedModel::Bone& AnimatedModel::GetBone(unsigned) const
    {
        throw std::logic_error("Not implemented");
    }

    unsigned AnimatedModel::GetNumMeshes() const
    {
        throw std::logic_error("Not implemented");
    }

    void AnimatedModel::DeleteSkin(unsigned)
    {
        throw std::logic_error("Not implemented");
    }

    void AnimatedModel::UnloadSkin(unsigned)
    {
        throw std::logic_error("Not implemented");
    }

    void AnimatedModel::UpdateTexturesFilter()
    {
        throw std::logic_error("Not implemented");
    }

    unsigned AnimatedModel::GetNumSkins() const
    {
        throw std::logic_error("Not implemented");
    }

    AnimatedModel::~AnimatedModel()
    {
        throw std::logic_error("Not implemented");
    }

    rend::TexHandle AnimatedModel::GetTexHandle(unsigned, unsigned, unsigned) const
    {
        throw std::logic_error("Not implemented");
    }

    bool AnimatedModel::Load(CStr const&, bool)
    {
        throw std::logic_error("Not implemented");
    }

    unsigned AnimatedModel::GetGroupsNum() const
    {
        throw std::logic_error("Not implemented");
    }

    int AnimatedModel::GetFrames(int, int) const
    {
        throw std::logic_error("Not implemented");
    }

    void AnimatedModel::RenderNormals(CMatrix const&, AnimInfo*)
    {
        throw std::logic_error("Not implemented");
    }

    void AnimatedModel::AddSkin(unsigned)
    {
        throw std::logic_error("Not implemented");
    }

    int AnimatedModel::GetLoadPointIdByName(char const*) const
    {
        throw std::logic_error("Not implemented");
    }

    DRAFT_Geom const* AnimatedModel::GetGeom(unsigned) const
    {
        throw std::logic_error("Not implemented");
    }

    int AnimatedModel::ChangeShader(CStr const&, unsigned, unsigned)
    {
        throw std::logic_error("Not implemented");
    }

    bool AnimatedModel::bIsPassable() const
    {
        throw std::logic_error("Not implemented");
    }

    void AnimatedModel::FromGroupVariants(Configuration&) const
    {
        throw std::logic_error("Not implemented");
    }

    void AnimatedModel::ReloadSkins(LoadSkins const&)
    {
        throw std::logic_error("Not implemented");
    }

    void AnimatedModel::CalculateMeshes(Configuration&) const
    {
        throw std::logic_error("Not implemented");
    }

    void AnimatedModel::RenderHierGeoms(CMatrix const&, AnimInfo*, unsigned)
    {
        throw std::logic_error("Not implemented");
    }

    int AnimatedModel::ChangeTexture(CStr const&, unsigned, unsigned, unsigned)
    {
        throw std::logic_error("Not implemented");
    }

    int AnimatedModel::Render(CMatrix const&, AnimInfo*, unsigned, bool)
    {
        throw std::logic_error("Not implemented");
    }

    int AnimatedModel::Render(CMatrix const&, AnimInfo*, Configuration const&, unsigned)
    {
        throw std::logic_error("Not implemented");
    }

    int AnimatedModel::Render(CMatrix const&, AnimInfo*, Configuration const&, std::vector<DSurfaceMaterial*, std::allocator<DSurfaceMaterial*>> const&)
    {
        throw std::logic_error("Not implemented");
    }

    int AnimatedModel::GetFps(int) const
    {
        throw std::logic_error("Not implemented");
    }

    void AnimatedModel::SetFps(int, short)
    {
        throw std::logic_error("Not implemented");
    }

    void AnimatedModel::RenderLoadPoints(CMatrix const&, AnimInfo*, Configuration const&)
    {
        throw std::logic_error("Not implemented");
    }

    DSurfaceMaterial& AnimatedModel::GetMeshMaterial(unsigned, unsigned)
    {
        throw std::logic_error("Not implemented");
    }

    void AnimatedModel::AddTexture(unsigned, unsigned, CStr const&, DRAFT_TextureType, unsigned)
    {
        throw std::logic_error("Not implemented");
    }

    DCollisionData const& AnimatedModel::GetCollisionTrimesh() const
    {
        throw std::logic_error("Not implemented");
    }

    int AnimatedModel::GetBoneMatrixByName(CStr const&, CMatrix&, bool) const
    {
        throw std::logic_error("Not implemented");
    }

    void AnimatedModel::FromCfgNum(Configuration&) const
    {
        throw std::logic_error("Not implemented");
    }

    char const* AnimatedModel::GetName() const
    {
        throw std::logic_error("Not implemented");
    }

    Aabb& AnimatedModel::GetAabb()
    {
        throw std::logic_error("Not implemented");
    }

    int AnimatedModel::Update(AnimInfo*, bool, Configuration*)
    {
        throw std::logic_error("Not implemented");
    }

    void AnimatedModel::UpdateCubemap()
    {
        throw std::logic_error("Not implemented");
    }

    unsigned AnimatedModel::GetNumGeoms() const
    {
        throw std::logic_error("Not implemented");
    }

    unsigned AnimatedModel::GetCfgSize() const
    {
        throw std::logic_error("Not implemented");
    }

    MeshesGroup const& AnimatedModel::GetGroup(unsigned) const
    {
        throw std::logic_error("Not implemented");
    }

    DSurfaceMaterial& AnimatedModel::GetMaterial(unsigned, unsigned)
    {
        throw std::logic_error("Not implemented");
    }

    rend::IEffect* AnimatedModel::ApplyMaterial(DSurfaceMaterial&)
    {
        throw std::logic_error("Not implemented");
    }

    rend::IEffect* AnimatedModel::ApplyMaterial(unsigned)
    {
        throw std::logic_error("Not implemented");
    }

    bool AnimatedModel::Save(CStr const&)
    {
        throw std::logic_error("Not implemented");
    }

    unsigned AnimatedModel::GetNumBones() const
    {
        throw std::logic_error("Not implemented");
    }

    void AnimatedModel::UpdateVertices(AnimInfo*, bool, Configuration*)
    {
        throw std::logic_error("Not implemented");
    }

    void AnimatedModel::RenderMesh(unsigned, AnimInfo*, rend::IEffect*)
    {
        throw std::logic_error("Not implemented");
    }

    void AnimatedModel::RenderCollisions(CMatrix const&, unsigned)
    {
        throw std::logic_error("Not implemented");
    }

    unsigned AnimatedModel::GetNumMaterials() const
    {
        throw std::logic_error("Not implemented");
    }

    unsigned AnimatedModel::GetNumHierGeoms() const
    {
        throw std::logic_error("Not implemented");
    }

    bool AnimatedModel::SaveGAM(CStr const&)
    {
        throw std::logic_error("Not implemented");
    }

    void AnimatedModel::SetCubeMapTexName(CStr const& texName)
    {
        m_cubemapTexPath = texName;
        UnifyFileName(m_cubemapTexPath);
        m_cubemapTexName = NameFromFileName(m_cubemapTexPath);
    }

    void AnimatedModel::LoadSkin(unsigned)
    {
        throw std::logic_error("Not implemented");
    }

    void AnimatedModel::SetNextForAnimation(ActionType, int)
    {
        throw std::logic_error("Not implemented");
    }

    LoadSkins const& AnimatedModel::GetLoadedSkins()
    {
        throw std::logic_error("Not implemented");
    }

    void AnimatedModel::CalculateCfgSize()
    {
        throw std::logic_error("Not implemented");
    }

    bool AnimatedModel::Convert(CStr const&, bool, std::vector<DRAFT_Bone, std::allocator<DRAFT_Bone>> const&, std::vector<DMesh, std::allocator<DMesh>> const&, std::vector<DAnimation, std::allocator<DAnimation>> const&)
    {
        throw std::logic_error("Not implemented");
    }

    void AnimatedModel::MatrixForBone(AnimInfo*, int, int)
    {
        throw std::logic_error("Not implemented");
    }

    void AnimatedModel::NewEffect(std::basic_string<char, std::char_traits<char>, std::allocator<char>> const&, rend::IEffect*&)
    {
        throw std::logic_error("Not implemented");
    }

    void AnimatedModel::CreateVariants()
    {
        throw std::logic_error("Not implemented");
    }

    void AnimatedModel::SetTexture(CStr const&, DRAFT_TextureType, rend::TexHandle&)
    {
        throw std::logic_error("Not implemented");
    }

    void AnimatedModel::ReadMaterial(DSurfaceMaterial&, unsigned char*&, bool)
    {
        throw std::logic_error("Not implemented");
    }

    void AnimatedModel::VertsForSkinmesh(AnimInfo*, Mesh&, void*, bool)
    {
        throw std::logic_error("Not implemented");
    }

    void AnimatedModel::DrawBones(AnimInfo*)
    {
        throw std::logic_error("Not implemented");
    }

    void AnimatedModel::CheckConfigurations()
    {
        throw std::logic_error("Not implemented");
    }

    CStr AnimatedModel::DefinePathToTexture(CStr const&)
    {
        throw std::logic_error("Not implemented");
    }

    bool AnimInfo::IsAnimation(ActionType)
    {
	    throw std::logic_error("Not implemented");
    }

    void AnimInfo::CreateCopyMesh(int&, void**&, int*&, unsigned short**&, int*&, bool*&, CMatrix**&,
	    std::vector<rend::VertexType, std::allocator<rend::VertexType>>&,
	    std::vector<unsigned, std::allocator<unsigned>>&, int*)
    {
	    throw std::logic_error("Not implemented");
    }

    void AnimInfo::SetEmpty()
    {
	    throw std::logic_error("Not implemented");
    }

    void AnimInfo::InterpolateBones(int)
    {
	    throw std::logic_error("Not implemented");
    }

    int AnimInfo::SetAnimationIdx(int)
    {
	    throw std::logic_error("Not implemented");
    }

    AnimatedModel::Mesh const& AnimInfo::GetMesh(unsigned) const
    {
	    throw std::logic_error("Not implemented");
    }

    void AnimInfo::CreateFor(AnimatedModel*)
    {
	    throw std::logic_error("Not implemented");
    }

    void AnimInfo::SetBoneCurMatrix(unsigned, CMatrix const&)
    {
	    throw std::logic_error("Not implemented");
    }

    CMatrix const& AnimInfo::GetCurrentLoadpointMatrix(int) const
    {
	    throw std::logic_error("Not implemented");
    }

    int AnimInfo::SetCurFrame(float)
    {
	    throw std::logic_error("Not implemented");
    }

    void AnimInfo::Release()
    {
	    throw std::logic_error("Not implemented");
    }

    AnimatedModel::Animation const* AnimInfo::GetCurAnimation() const
    {
	    throw std::logic_error("Not implemented");
    }

    int AnimInfo::GetStickToLastFrame() const
    {
	    throw std::logic_error("Not implemented");
    }

    void AnimInfo::RemoveCopyMesh(void**&, int*&, unsigned short**&, int*&, CMatrix**&)
    {
	    throw std::logic_error("Not implemented");
    }

    bool AnimInfo::IsEmpty()
    {
	    throw std::logic_error("Not implemented");
    }

    int& AnimInfo::CurAnimFrame()
    {
	    throw std::logic_error("Not implemented");
    }

    AnimInfo::~AnimInfo()
    {
	    throw std::logic_error("Not implemented");
    }

    AnimInfo::AnimInfo()
    {
	    throw std::logic_error("Not implemented");
    }

    int AnimInfo::SetAnimation(ActionType)
    {
	    throw std::logic_error("Not implemented");
    }

    void AnimInfo::MoveFrame(unsigned)
    {
	    throw std::logic_error("Not implemented");
    }

    BoneAnim& AnimInfo::GetBoneAnim(unsigned)
    {
	    throw std::logic_error("Not implemented");
    }
}
