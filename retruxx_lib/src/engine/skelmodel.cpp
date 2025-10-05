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
        {nullptr, AT_NUMTYPES  },
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

    bool AnimatedModel::LoadGAM(CStr const& FileName, bool bForceNextAnimation)
    {
        // TODO: generated code
        if (this->m_bVerification)
        {
            M3D_LOG_ERR("LoadGAM does work not for verification of models!!!");
            return false;
        }

        CStr path;
        CStr a;

        // Extract name and path
        m_Name = NameFromFileName(FileName).c_str();
        m_PathToFile = DirectoryFromFileName(FileName).c_str();

        m3d::fs::auxTaggedFile File;

        void* Data = 0;

        if (File.Open(FileName.c_str(), fs::auxTaggedFile::PROCESS_NORMAL_IGNORE_CRC))
        {
            M3D_LOG_ERR("AnimatedModel::LoadGAM -- cannot open file " + FileName);
            return false;
        }

        char* cFormatTitle = nullptr;
        File.getFormatTitle(&cFormatTitle);

        if (strcmp(cFormatTitle, "IVR"))
        {
            M3D_LOG_ERR("Wrong file format!!!");
            return false;
        }

        unsigned int uFormatVersion = 0;
        File.getFormatVersion(uFormatVersion);

        if (uFormatVersion <= 1)
        {
            // Read header chunk (chunk 1)
            File.getChunkData(1u, &Data);
            uint16_t* v14 = (uint16_t*)Data;

            this->m_header.m_numTriMeshes = v14[0];
            this->m_header.m_numSkinMeshes = v14[1];
            this->m_header.m_numStaticMeshes = v14[2];
            this->m_header.m_numAnimations = v14[3];
            this->m_header.m_numNodes = v14[5];
            this->m_cfgSize = *((uint32_t*)v14 + 3);
            uint16_t iNumMaterials = v14[4];

            // Read bone data chunk (chunk 2)
            File.getChunkData(2u, &Data);
            uint8_t* v16 = (uint8_t*)Data;

            int m_numNodes = this->m_header.m_numNodes;
            this->m_boneInitialPos = new Bone[m_numNodes];

            int v19 = this->m_header.m_numNodes;
            this->m_initialBoneInvMatrices = new CMatrix[v19];

            int v22 = 0;
            bool v23 = this->m_header.m_numNodes <= 0;
            unsigned int j = 0;
            unsigned int v152 = 0;

            if (!v23)
            {
                int v28 = 0;
                do
                {
                    Bone* v24 = (Bone*)((char*)this->m_boneInitialPos + v152);
                    strcpy(v24->m_boneName, (const char*)v16);
                    v24->m_ownIdx = v22;
                    v24->m_parentIdx = *((uint32_t*)v16 + 10);
                    v24->m_translation0 = *(CVector*)(v16 + 44);
                    v24->m_quaternion0 = *(Quaternion*)(v16 + 56);

                    int v25 = v22 * 64;
                    float* v26 = (float*)(v16 + 76);

                    for (int v27 = 0; v27 < 4; v27++) {
                        *(float*)((char*)&this->m_initialBoneInvMatrices->_11 + v25) = *(v26 - 1);
                        *(float*)((char*)&this->m_initialBoneInvMatrices->_12 + v25) = *v26;
                        *(float*)((char*)&this->m_initialBoneInvMatrices->_13 + v25) = v26[1];
                        *(float*)((char*)&this->m_initialBoneInvMatrices->_14 + v25) = v26[2];
                        v26 += 4;
                        v25 += 16;
                    }

                    v28 = this->m_header.m_numNodes;
                    v152 += 76;
                    v22 = j + 1;
                    v16 += 136;
                    ++j;
                } while ((int)j < v28);
            }

            // Read mesh data chunk (chunk 4)
            File.getChunkData(4u, &Data);
            unsigned int v29 = this->m_header.m_numStaticMeshes + this->m_header.m_numTriMeshes + this->m_header.m_numSkinMeshes;
            this->m_numMeshes = v29;

            this->m_meshes = new Mesh[v29];
            bool v4 = this->m_numMeshes == 0;
            v152 = 0;

            if (!v4)
            {
                unsigned int m_animRemap = 0;
                bool v57 = false;
                do
                {
                    AnimatedModel* v32 = this;
                    char* v33 = &this->m_meshes->m_meshName[m_animRemap];
                    uint8_t* v34 = (uint8_t*)Data;
                    uint8_t* v35 = (uint8_t*)Data;
                    char* v36 = v33;

                    // Copy mesh name
                    uint8_t v37 = 0;
                    do
                    {
                        v37 = *v35;
                        *v36++ = *v35++;
                    } while (v37);

                    *((uint32_t*)v33 + 12) = v152;
                    *((uint32_t*)v33 + 10) = *((uint32_t*)v34 + 10);
                    *((uint32_t*)v33 + 11) = *((uint32_t*)v34 + 11);
                    *((uint32_t*)v33 + 13) = *((uint32_t*)v34 + 12);
                    *((uint32_t*)v33 + 14) = *((uint32_t*)v34 + 13);
                    *((uint32_t*)v33 + 20) = *((uint32_t*)v34 + 14);
                    *((uint32_t*)v33 + 21) = *((uint32_t*)v34 + 15);
                    *((uint32_t*)v33 + 22) = *((uint32_t*)v34 + 16);
                    *((uint32_t*)v33 + 29) = *((uint32_t*)v34 + 17);
                    *((uint32_t*)v33 + 15) = (uint32_t)&v32->m_Skins;

                    Data = (uint8_t*)Data + 72;

                    // Allocate and copy vertex data
                    auto v38 = new char[*((uint32_t*)v33 + 22) * *((uint32_t*)v33 + 20)];
                    unsigned int v39 = *((uint32_t*)v33 + 22) * *((uint32_t*)v33 + 20);
                    *((uint32_t*)v33 + 23) = (uint32_t)v38;
                    memcpy(v38, Data, v39);

                    Data = (uint8_t*)Data + (*((uint32_t*)v33 + 22) * *((uint32_t*)v33 + 20));

                    // Handle different mesh types
                    if (*((uint32_t*)v33 + 10) == 4)
                    {
                        // Type 4 mesh - create VB pool field
                        m3d::rend::VbPoolField v40 = m3d::Application::g_pApp->m_renderer->AddVbPoolField(*(m3d::rend::VertexType*)((uint32_t*)v33 + 21), *((uint32_t*)v33 + 22));

                        *((uint32_t*)v33 + 33) = v40.Offset;
                        *((uint32_t*)v33 + 34) = v40.Size;
                        *((uint32_t*)v33 + 35) = v40.RealOffset;
                        *((uint32_t*)v33 + 36) = v40.VertType;
                        *((m3d::rend::VbHandle*)((uint32_t*)v33 + 37)) = v40.Vb;

                        void* lockPtr = m3d::Application::g_pApp->m_renderer->LockVbPoolField(*((m3d::rend::VbPoolField*)(v33 + 132)));
                        memcpy(lockPtr, *((const void**)v33 + 23),
                            *((uint32_t*)v33 + 22) * *((uint32_t*)v33 + 20));
                        m3d::Application::g_pApp->m_renderer->UnlockVbPoolField(*((m3d::rend::VbPoolField*)(v33 + 132)));
                    }

                    if (*((uint32_t*)v33 + 10) == 1) {
                        // Type 1 mesh
                        m3d::rend::VbPoolField v41 = m3d::Application::g_pApp->m_renderer->AddVbPoolField(*(m3d::rend::VertexType*)((uint32_t*)v33 + 21), *((uint32_t*)v33 + 22));

                        *((uint32_t*)v33 + 33) = v41.Offset;
                        *((uint32_t*)v33 + 34) = v41.Size;
                        *((uint32_t*)v33 + 35) = v41.RealOffset;
                        *((uint32_t*)v33 + 36) = v41.VertType;
                        *((m3d::rend::VbHandle*)((uint32_t*)v33 + 37)) = v41.Vb;

                        uint8_t* v42 = (uint8_t*)m3d::Application::g_pApp->m_renderer->LockVbPoolField(*((m3d::rend::VbPoolField*)(v33 + 132)));
                        memcpy(v42, Data, *((uint32_t*)v33 + 22) * *((uint32_t*)v33 + 20));
                        m3d::Application::g_pApp->m_renderer->UnlockVbPoolField(*((m3d::rend::VbPoolField*)(v33 + 132)));

                        Data = (uint8_t*)Data + (*((uint32_t*)v33 + 22) * *((uint32_t*)v33 + 20));
                    }

                    if (*((uint32_t*)v33 + 10) == 2)
                    {
                        // Type 2 mesh - skin influences
                        int v43 = *((uint32_t*)v33 + 22);
                        void* v45 = new Influences[v43];


                        *((uint32_t*)v33 + 26) = (uint32_t)v45;
                        int v46 = 0;

                        if (*((int*)v33 + 22) > 0)
                        {
                            int v47 = 0;
                            uint8_t* v48 = (uint8_t*)Data + 8;

                            do
                            {
                                int v49 = *((uint32_t*)v33 + 26);
                                *(uint16_t*)(v49 + v47) = *((uint16_t*)v48 - 4);
                                int v50 = v47 + v49;
                                *(uint16_t*)(v50 + 4) = *((uint16_t*)v48 - 3);
                                *(uint32_t*)(v50 + 8) = *((uint32_t*)v48 - 1);
                                *(uint32_t*)(v50 + 12) = *(uint32_t*)v48;
                                *(uint32_t*)(v50 + 16) = *((uint32_t*)v48 + 1);
                                *(uint32_t*)(v50 + 20) = *((uint32_t*)v48 + 2);
                                *(uint32_t*)(v50 + 24) = *((uint32_t*)v48 + 3);
                                *(uint32_t*)(v50 + 28) = *((uint32_t*)v48 + 4);
                                *(uint32_t*)(v50 + 32) = *((uint32_t*)v48 + 5);
                                *(uint16_t*)(v50 + 36) = *((uint16_t*)v48 + 12);
                                *(uint32_t*)(v50 + 40) = *(uint32_t*)(v48 + 26);
                                *(uint32_t*)(v50 + 44) = *(uint32_t*)(v48 + 30);
                                *(uint32_t*)(v50 + 48) = *(uint32_t*)(v48 + 34);
                                *(uint32_t*)(v50 + 52) = *(uint32_t*)(v48 + 38);
                                *(uint32_t*)(v50 + 56) = *(uint32_t*)(v48 + 42);
                                *(uint32_t*)(v50 + 60) = *(uint32_t*)(v48 + 46);
                                *(uint32_t*)(v50 + 64) = *(uint32_t*)(v48 + 50);
                                *(uint16_t*)(v50 + 68) = *((uint16_t*)v48 + 27);
                                *(uint32_t*)(v50 + 72) = *((uint32_t*)v48 + 14);
                                *(uint32_t*)(v50 + 76) = *((uint32_t*)v48 + 15);
                                *(uint32_t*)(v50 + 80) = *((uint32_t*)v48 + 16);
                                *(uint32_t*)(v50 + 84) = *((uint32_t*)v48 + 17);
                                *(uint32_t*)(v50 + 88) = *((uint32_t*)v48 + 18);
                                *(uint32_t*)(v50 + 92) = *((uint32_t*)v48 + 19);
                                *(uint32_t*)(v50 + 96) = *((uint32_t*)v48 + 20);
                                *(uint16_t*)(v50 + 100) = *((uint16_t*)v48 + 42);
                                *(uint32_t*)(v50 + 104) = *(uint32_t*)(v48 + 86);
                                *(uint32_t*)(v50 + 108) = *(uint32_t*)(v48 + 90);
                                *(uint32_t*)(v50 + 112) = *(uint32_t*)(v48 + 94);
                                *(uint32_t*)(v50 + 116) = *(uint32_t*)(v48 + 98);
                                *(uint32_t*)(v50 + 120) = *(uint32_t*)(v48 + 102);
                                *(uint32_t*)(v50 + 124) = *(uint32_t*)(v48 + 106);
                                ++v46;
                                *(uint32_t*)(v50 + 128) = *(uint32_t*)(v48 + 110);
                                v47 += 132;
                                v48 += 122;
                            } while (v46 < *((uint32_t*)v33 + 22));
                        }

                        Data = (uint8_t*)Data + (122 * *((uint32_t*)v33 + 22));
                    }

                    // Index buffer data
                    uint8_t* v51 = (uint8_t*) new unsigned short[3* *((uint32_t*)v33 + 29)];
                    int v52 = 3 * *((uint32_t*)v33 + 29);
                    *((uint32_t*)v33 + 28) = (uint32_t)v51;
                    memcpy(v51, Data, 2 * v52);

                    Data = (uint8_t*)Data + (6 * *((uint32_t*)v33 + 29));

                    int v53 = *((uint32_t*)v33 + 28);
                    int v54 = *((uint32_t*)v33 + 22);
                    *((uint32_t*)v33 + 25) = *((uint32_t*)v33 + 23);
                    unsigned int v55 = 3 * *((uint32_t*)v33 + 29);
                    *((uint32_t*)v33 + 31) = v55;
                    *((uint32_t*)v33 + 30) = v53;
                    *((uint32_t*)v33 + 24) = v54;

                    // Create index buffer pool field
                    m3d::rend::IbPoolField v56 = m3d::Application::g_pApp->m_renderer->AddIbPoolField(v55);

                    *((uint32_t*)v33 + 38) = v56.Offset;
                    *((uint32_t*)v33 + 39) = v56.Size;
                    *((uint32_t*)v33 + 40) = v56.RealOffset;
                    *((m3d::rend::IbHandle*)((uint32_t*)v33 + 41)) = v56.Ib;

                    void* ibLockPtr = m3d::Application::g_pApp->m_renderer->LockIbPoolField(*(m3d::rend::IbPoolField*)(v33 + 152));
                    memcpy(ibLockPtr, *((const void**)v33 + 30), 2 * *((uint32_t*)v33 + 31));
                    m3d::Application::g_pApp->m_renderer->UnlockIbPoolField(*(m3d::rend::IbPoolField*)(v33 + 152));

                    m_animRemap += 188;
                    v57 = ++v152 < this->m_numMeshes;
                } while (v57);
            }

            // Bounding box data
            float v58 = *((float*)Data + 3);
            float v59 = *((float*)Data + 4);
            float v60 = *((float*)Data + 5);
            float v61 = *(float*)Data;
            float v62 = *((float*)Data + 1);
            float v63 = *((float*)Data + 2);

            this->m_box.m_box[0] = v61;
            this->m_box.m_box[1] = v62;
            this->m_box.m_box[2] = v63;
            this->m_box.m_box[3] = v58;
            this->m_box.m_box[4] = v59;
            this->m_box.m_box[5] = v60;

            // Animation data (chunk 8)
            File.getChunkData(8u, &Data);
            unsigned int numAnimations = this->m_header.m_numAnimations;

            this->m_animations = new Animation[numAnimations + 4];
            v23 = this->m_header.m_numAnimations <= 0;
            int v155 = 0;

            if (!v23)
            {
                uint8_t* v68 = (uint8_t*)Data;
                unsigned int m_animRemap = 0;

                do {
                    Animation* v69 = (Animation*)((char*)this->m_animations + m_animRemap);
                    v69->m_fps = *((uint16_t*)(v68 + 27));
                    v69->m_numFrames = *((uint16_t*)(v68 + 25));
                    v69->m_nextAnimation = *((uint16_t*)(v68 + 29));
                    v69->m_numChanges = *((uint16_t*)(v68 + 31));
                    v69->m_action = (ActionType)(*((uint32_t*)(v68 + 35)));
                    v69->m_numNodes = *((uint16_t*)(v68 + 33));

                    // Copy animation name
                    char* v70 = (char*)((char*)v69 - (char*)v68 + 16);
                    uint8_t v71 = 0;
                    do {
                        v71 = *v68;
                        v70[(uint32_t)v68] = *v68;
                        ++v68;
                    } while (v71);

                    Data = (uint8_t*)Data + 39;

                    // Hierarchy changes
                    HierarchyChange* v72 = (HierarchyChange*)m3d::g_Kernel->g_mar.AllocMem(12 * v69->m_numChanges, 0, 0);
                    int v73 = 0;
                    v23 = v69->m_numChanges <= 0;
                    v69->m_hierChanges = v72;

                    if (!v23) {
                        int v74 = 0;
                        uint8_t* v75 = (uint8_t*)Data + 6;

                        do {
                            HierarchyChange* v76 = &v69->m_hierChanges[v74];
                            v76->ownIdx = *((uint16_t*)v75 - 1);
                            v76->newParentIdx = *(uint16_t*)v75;
                            v76->changeType = (DRAFT_Change)*((uint32_t*)(v75 - 6));
                            ++v73;
                            v75 += 8;
                            ++v74;
                        } while (v73 < v69->m_numChanges);
                    }

                    Data = (uint8_t*)Data + (8 * v69->m_numChanges);

                    // Animation transforms
                    unsigned int numTransforms = v69->m_numFrames * v69->m_numNodes;
                    v69->m_nodesPositions = (AnimationTransform*)m3d::g_Kernel->g_mar.AllocMem(32 * numTransforms, 0, 0);

                    unsigned int v78 = 0;
                    uint32_t* GroupsNum = (uint32_t*)Data;

                    if (numTransforms >= 4)
                    {
                        int v79 = 0;
                        unsigned int v80 = ((numTransforms - 4) >> 2) + 1;
                        uint8_t* v81 = (uint8_t*)Data + 36;
                        unsigned int i = 4 * v80;

                        do
                        {
                            AnimationTransform* m_nodesPositions = v69->m_nodesPositions;
                            m_nodesPositions[v79].idx = *((uint16_t*)v81 - 18);
                            m_nodesPositions[v79].tx = *(float*)(v81 - 34);
                            m_nodesPositions[v79].ty = *(float*)(v81 - 30);
                            m_nodesPositions[v79].tz = *(float*)(v81 - 26);
                            m_nodesPositions[v79].qx = *(float*)(v81 - 22);
                            m_nodesPositions[v79].qy = *(float*)(v81 - 18);
                            AnimationTransform* v83 = &m_nodesPositions[v79];
                            v83->qz = *(float*)(v81 - 14);
                            v83->qw = *(float*)(v81 - 10);

                            AnimationTransform* v84 = v69->m_nodesPositions;
                            v84[v79 + 1].idx = *((uint16_t*)v81 - 3);
                            v84[v79 + 1].tx = *((float*)v81 - 1);
                            v84[v79 + 1].ty = *(float*)v81;
                            v84[v79 + 1].tz = *((float*)v81 + 1);
                            AnimationTransform* v85 = &v84[v79 + 1];
                            v85->qx = *((float*)v81 + 2);
                            v85->qy = *((float*)v81 + 3);
                            v85->qz = *((float*)v81 + 4);
                            v85->qw = *((float*)v81 + 5);

                            AnimationTransform* v86 = &v69->m_nodesPositions[v79 + 2];
                            v86->idx = *((uint16_t*)v81 + 12);
                            v86->tx = *(float*)(v81 + 26);
                            v86->ty = *(float*)(v81 + 30);
                            v86->tz = *(float*)(v81 + 34);
                            v86->qx = *(float*)(v81 + 38);
                            v86->qy = *(float*)(v81 + 42);
                            v86->qz = *(float*)(v81 + 46);
                            v86->qw = *(float*)(v81 + 50);

                            AnimationTransform* v87 = &v69->m_nodesPositions[v79 + 3];
                            v87->idx = *((uint16_t*)v81 + 27);
                            v87->tx = *((float*)v81 + 14);
                            v87->ty = *((float*)v81 + 15);
                            v87->tz = *((float*)v81 + 16);
                            v87->qx = *((float*)v81 + 17);
                            v87->qy = *((float*)v81 + 18);
                            v87->qz = *((float*)v81 + 19);
                            float v88 = *((float*)v81 + 20);
                            v81 += 120;
                            v79 += 4;
                            --v80;
                            v87->qw = v88;
                        } while (v80);

                        v78 = i;
                    }

                    // Remaining animation transforms
                    if (v78 < numTransforms) {
                        uint32_t* v89 = (uint32_t*)((char*)GroupsNum + 30 * v78 + 6);
                        unsigned int v90 = v78;
                        unsigned int remaining = numTransforms - v78;

                        do {
                            AnimationTransform* v91 = &v69->m_nodesPositions[v90];
                            v91->idx = *((uint16_t*)v89 - 3);
                            v91->tx = *(float*)(v89 - 1);
                            v91->ty = *(float*)v89;
                            v91->tz = (float)v89[1];
                            v91->qx = (float)v89[2];
                            v91->qy = (float)v89[3];
                            v91->qz = (float)v89[4];
                            float v92 = *((float*)v89 + 5);
                            v89 = (uint32_t*)((char*)v89 + 30);
                            ++v90;
                            v4 = remaining-- == 1;
                            v91->qw = v92;
                        } while (!v4);
                    }

                    m_animRemap += 52;
                    v68 = &((uint8_t*)Data)[30 * numTransforms];
                    v23 = v155 + 1 < this->m_header.m_numAnimations;
                    Data = v68;
                    ++v155;
                } while (v23);
            }

            // Animation remapping
            v152 = 0;
            v155 = (int)m3d::actions;
            unsigned int m_animRemap = (unsigned int)this->m_animRemap;
            j = 32;

            do {
                uint16_t v93 = 0;
                if (this->m_header.m_numAnimations > 0) {
                    Animation* m_animations = this->m_animations;
                    unsigned int i = *(uint32_t*)v155;

                    do {
                        if (!strcmp((const char*)i, m_animations[v93].m_name)) {
                            break;
                        }
                        ++v93;
                    } while (v93 < this->m_header.m_numAnimations);
                }

                if (v93 >= this->m_header.m_numAnimations) {
                    *(uint16_t*)m_animRemap = -1;
                }
                else {
                    *(uint16_t*)m_animRemap = v93;
                    this->m_animations[v93].m_action = (ActionType)v152;
                }

                m_animRemap += 2;
                v155 += 8;
                ++v152;
                --j;
            } while (j);

            // Set next animations
            int v95 = 0;
            if (this->m_header.m_numAnimations > 0) {
                int v96 = 0;
                do {
                    if (bForceNextAnimation)
                    {
                        int v97 = 0;
                        short* v98 = &this->m_animRemap[1];

                        while (*(v98 - 1) != v95) {
                            if (*v98 == v95) {
                                ++v97;
                                break;
                            }
                            if (v98[1] == v95) {
                                v97 += 2;
                                break;
                            }
                            if (v98[2] == v95) {
                                v97 += 3;
                                break;
                            }
                            v97 += 4;
                            v98 += 4;
                            if (v97 >= 32) {
                                break;
                            }
                        }

                        auto v99 = nextAnims_0[v97];
                        if (v97 == 32 || (v99 < 0)) {
                            this->m_animations[v96].m_nextAnimation = -1;
                        }
                        else {
                            this->m_animations[v96].m_nextAnimation = this->m_animRemap[v99];
                        }
                    }
                    else {
                        this->m_animations[v96].m_nextAnimation = v95;
                    }
                    ++v95;
                    ++v96;
                } while (v95 < this->m_header.m_numAnimations);
            }

            // Load skins/materials (chunk 15)
            File.getChunkData(0xFu, &Data);
            m_Skins.resize(*(uint32_t*)Data);

            Data = (uint8_t*)Data + 4;
            m_animRemap = 0;
            v152 = 0;

            for (int skinsIdx = 0; skinsIdx < this->m_Skins.size(); ++skinsIdx)
            {
                auto& surfaceMaterials = m_Skins[skinsIdx];
                surfaceMaterials.resize(iNumMaterials);

                bool load = true;
                if (!m_loadSkins.loadAllSkins)
                {
                    load = m_loadSkins.loadSkins.find(skinsIdx) != m_loadSkins.loadSkins.end();
                }

                for (int matIdx = 0; matIdx < iNumMaterials; ++matIdx)
                {
                    auto& surfaceMaterial = m_Skins[skinsIdx][matIdx];

                    memcpy(&surfaceMaterial, Data, sizeof(rend::Material));
                    unsigned int textureCount = *((uint32_t*)Data + 17);

                    const char* shaderName = (const char*)((uint8_t*)Data + 72);
                    surfaceMaterial.Shader.Name = shaderName;

                    if (load && !surfaceMaterial.Shader.Name.empty())
                    {
                        NewEffect(surfaceMaterial.Shader.Name, surfaceMaterial.Shader.Handle);
                    }
                    else
                    {
                        surfaceMaterial.Shader.Handle = nullptr;
                    }

                    Data = (uint8_t*)Data + 172;

                    // Initialize texture array
                    surfaceMaterial.Textures.resize(textureCount);
                    for (int texIdx = 0; texIdx < textureCount; ++texIdx)
                    {
                        auto& textureInfo = surfaceMaterial.Textures[texIdx];
                        textureInfo.Type = (DRAFT_TextureType)*((int*)Data + 11);
                        textureInfo.UV_Set = *((int*)Data + 10);
                        textureInfo.FileName = (const char*)Data;
                        textureInfo.Handle.SetInvalid();

                        auto pathToTex = DefinePathToTexture(textureInfo.FileName.c_str());
                        if (load)
                        {
                            SetTexture(pathToTex, textureInfo.Type, textureInfo.Handle);
                        }
                        // TODO: check this
                        Data = (uint8_t*)Data + (48);
                    }
                }
            }

            // Load collision data (chunk 16)
            if (!File.getChunkData(16, &Data))
            {
                        uint32_t* v118 = (uint32_t*)Data;
                        Data = (uint8_t*)Data + 8;

                        this->m_Collision.Points.resize(*v118);
                        memcpy(this->m_Collision.Points.data(), Data, 12 * *v118);

                        uint8_t* v119 = (uint8_t*)Data + (12 * *v118);
                        Data = v119;

                        unsigned int v120 = v118[1];
                        this->m_Collision.Triangles.resize(v120);
                        memcpy(this->m_Collision.Triangles.data(), Data, 6 * v118[1]);
            }

            // Load geometry data (chunk 32)
            if (!File.getChunkData(32, &Data))
            {
                uint8_t* v123 = (uint8_t*)Data;
                Data = (uint8_t*)Data + 4;

                this->m_Geoms.resize(*(uint32_t*)v123);
                memcpy(this->m_Geoms.data(), Data, 44 * *(uint32_t*)v123);
            }

            // Load hierarchical geometry (chunk 64)
            if (!File.getChunkData(64, &Data))
            {
                uint8_t* v126 = (uint8_t*)Data;
                Data = (uint8_t*)Data + 4;

                unsigned int v127 = *(uint32_t*)v126;
                this->m_HierGeoms.resize(v127);
                memcpy(this->m_HierGeoms.data(), Data, 48 * *(uint32_t*)v126);
            }


            // Load bone bounds (chunk 128)
            if (!File.getChunkData(128, &Data))
            {
                uint8_t* v130 = (uint8_t*)Data;
                Data = (uint8_t*)Data + 4;

                unsigned int v131 = *(uint32_t*)v130;
                std::vector<m3d::DRAFT_BoneBounds> BonesBounds;

                BonesBounds.resize(v131);
                memcpy(BonesBounds.data(), Data, 28 * *(uint32_t*)v130);

                for (auto& bound : BonesBounds)
                {
                    m_BonesBounds[bound.BoneIndex] = bound;
                }
            }

            // Load mesh groups (chunk 240)
            if (!File.getChunkData(240, &Data))
            {
                uint32_t* v137 = (uint32_t*)Data;
                Data = (uint8_t*)Data + 4;

                unsigned int v138 = *v137;
                unsigned int* GroupsNum = v137;

                m_MhGroups.resize(v138);

                bool v4 = *v137 == 0;
                unsigned int i = 0;

                if (!v4) {
                    unsigned int iNumMaterials = 0;
                    uint8_t* v139 = (uint8_t*)Data;
                    bool v57 = false;
                    do {
                        m3d::MeshesGroup* v140 = &this->m_MhGroups[iNumMaterials / 0x44];
                        uint8_t* v141 = v139;

                        v140->Name = (const char*)v139;
                        v140->VisibleAtOnceMax = *((uint32_t*)v141 + 6);
                        v140->VisibleAtOnceMin = *((uint32_t*)v141 + 5);

                        v140->MeshesId.resize(*((uint32_t*)v141 + 7));
                        Data = (uint8_t*)Data + 32;
                        memcpy(v140->MeshesId.data(), Data, 4 * *((uint32_t*)v141 + 7));

                        Data = (uint8_t*)Data + (4 * *((uint32_t*)v141 + 7));
                        int v142 = 0;

                        v140->m_variants.resize(*(uint32_t*)Data);

                        v139 = (uint8_t*)Data + 4;
                        j = 0;

                        while (true) {
                            Data = v139;

                            if (j >= v140->m_variants.size()) {
                                break;
                            }


                            uint32_t* v144 = (uint32_t*)v139;
                            unsigned int v145 = *(uint32_t*)v139;
                            unsigned int* VariantsNum = v144;

                            v140->m_variants[v142].resize(v145);
                            Data = (uint8_t*)Data + 4;

                            unsigned int v146 = *v144;
                            uint32_t* v147 = v140->m_variants[v142].data();
                            ++j;

                            memcpy(v147, Data, 4 * v146);
                            v139 = (uint8_t*)Data + (4 * *VariantsNum);
                            ++v142;
                        }

                        iNumMaterials += 68;
                        v57 = ++i < *GroupsNum;
                    } while (v57);
                }
            }

            return 1;
        }
        else 
        {
            M3D_LOG_ERR("AnimatedModel::LoadGAM -- tagged file version is wrong: " + CStr(uFormatVersion));
            return 0;
        }
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
        if (m_hasCubemap)
        {
            throw retruxx::logic_error("Not implemented");
        }
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
