#define NOMINMAX

#include "roads/road.h"
#include <geomobject.h>
#include <landscape.h>
#include <stdexcept>
#include <core/kernel.h>
#include <core/timer.h>
#include <core/ini.h>
#include <file/tagged.h>

#include "config.h"
#include "level.h"
#include "m3dapp.h"
#include "world.h"
#include "core/log.h"
#include "core/scoped_ptr.h"
#include "file/fileserver.h"
#include "file/filestream.h"
#include "math/vector4.h"

namespace m3d
{
    RT_CLASS_EXPORTS_BEGIN(Landscape)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(Landscape);

    void Landscape::LinkObstacleToCells(ai::Obstacle*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool Landscape::SaveShoreLine(CStr const&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    int Landscape::GetTileSize() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::SetOverlayShader(rend::IEffect*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::LinkNodeAndChildrenCollisionGeomsToCell(SgNode*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    float Landscape::GetHeightWithCollisions(float, float, bool) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::InitReflectionRefractionTextures()
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::SetAllTexturesLoading(bool)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::FreeShoresStuff()
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::BuildSolidLandscape()
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::DrawCellsOverlayedEditor(cmn::vector<unsigned> const&, unsigned)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::DrawSolidLandscape(LandRenderMode, int)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::RemoveGrassInstance(unsigned)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::QueryWaterVisibility()
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::DrawLandScapeTextures(VisibilityMode, bool, bool)
    {
        throw retruxx::logic_error("Not implemented");
    }

    rend::TexHandle Landscape::GetLightmapTexture() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::RemoveGrassRectangle(CVector2 const&, CVector2 const&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::ChangedNumberOfUsedTextures(unsigned)
    {
        throw retruxx::logic_error("Not implemented");
    }

    int Landscape::ConstructCollisionData()
    {
        //TODO: implement Landscape::ConstructCollisionData
        return 1;
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::setOwner(CWorld* world)
    {
        m_owner = world;
    }

    Class* Landscape::GetClass() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    CStr const& Landscape::GetPathToTiles() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::SetEditorRenderMode()
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::LinkNodeObstacleToCells(SgNode*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::ReadTileInfo(int)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::_dbgGenerateGrass()
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::DrawJoint(dxJoint*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    Landscape::~Landscape()
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::UnlinkNodeCollisionGeomsFromCell(SgNode*, int, int, bool)
    {
        throw retruxx::logic_error("Not implemented");
    }

    int Landscape::New(float)
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool Landscape::HandleCVar(CVar const*, CConsoleParams const&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    int Landscape::AddOneTexture(CStr const&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::HandleCommand(int, CConsoleParams const&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::AddCollisionTris(int, int, CVector*, int, unsigned short*, CMatrix const&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::DrawShoreLine()
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::ReloadWaterTextures()
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool Landscape::SaveNormalMap(CStr const&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool Landscape::SaveColorMap(CStr const&, int)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::SetLsHeight(float, float, float)
    {
        throw retruxx::logic_error("Not implemented");
    }

    float Landscape::GetLsHeight(float, float) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    unsigned Landscape::GetNumGrassModels() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::setHgtAtHfPoint(int, int, float)
    {
        throw retruxx::logic_error("Not implemented");
    }

    float Landscape::getHgtAtHfPoint(int, int) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    rend::TexHandle Landscape::GetTexHandleFromList(unsigned) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::SaveTileInfo()
    {
        throw retruxx::logic_error("Not implemented");
    }

    RETRUXX_DLL_OVERWRITE_BY_ORIGINAL_FUNCTION(0x005BDC00, Landscape::Load)
    int Landscape::Load()
    {
        M3D_LOG_FLOW();
        m_mapSize = 16 * m_owner->m_level->land_size;
        auto const fileName = m_owner->m_level->GetFullPathNameA(m_owner->m_level->m_beachsetsName);
        CStr error;
        ref_ptr xmlFile = ReadXmlFile(fileName.c_str(), &error);
        if (!xmlFile)
        {
            xmlFile = ReadXmlFile("data\\Beachsets.xml", &error);
        }
        if (xmlFile)
        {
            ref_ptr beachsets = xmlFile->CreateNode(cmn::XML_NODE_EMPTY, nullptr);
            ref_ptr wave = xmlFile->CreateNode(cmn::XML_NODE_EMPTY, nullptr);
            xmlFile->GetFirstChild(beachsets, "Beachsets");
            for (beachsets->GetFirstChild(wave, "Wave"); !wave->IsEmpty(); wave->GetNextSibling(wave, "Wave"))
            {
                WaveSets ws;
                SafeFloatAttrib(ws.m_tcomp, wave, "tcomp");
                if (ws.m_tcomp < 0.0)
                {
                    SafeFloatAttrib(ws.m_tlevel, wave, "tlevel");
                    SafeFloatAttrib(ws.m_tamplitude, wave, "tamplitude");
                    SafeFloatAttrib(ws.m_tphase, wave, "tphase");
                    SafeFloatAttrib(ws.m_tfreq, wave, "tfreq");
                }
                SafeFloatAttrib(ws.m_scomp, wave, "scomp");
                if (ws.m_scomp < 0.0)
                {
                    SafeFloatAttrib(ws.m_slevel, wave, "slevel");
                    SafeFloatAttrib(ws.m_samplitude, wave, "samplitude");
                    SafeFloatAttrib(ws.m_sphase, wave, "sphase");
                    SafeFloatAttrib(ws.m_sfreq, wave, "sfreq");
                }
                CStr tex;
                SafeStrAttrib(tex, wave, "texture");
                auto texHandle = M3D_APP->m_renderer->AddTexture(tex, 2);
                if (texHandle.IsValid())
                {
                    M3D_APP->m_renderer->SetTextureParameter(texHandle, rend::TM_WRAP_S, 3u);
                    M3D_APP->m_renderer->SetTextureParameter(texHandle, rend::TM_WRAP_T, 3u);
                    M3D_APP->m_renderer->SetTextureParameter(texHandle, rend::TM_MIP_LOD_BIAS, M3D_KERNEL->GetEngineCfg().m_g_shoresMipBias.GetF());
                    ws.m_texHandle = texHandle;
                }
                m_waves.push_back(ws);
            }
        }
        else
        {
            M3D_LOG_INFO("Error:No Beachset.xml file");
        }

        delete[] m_colormap;
        //TODO: check size
        m_colormap = new unsigned[(m_mapSize + 1) * (m_mapSize + 1)];
        
        delete[] m_texSetsmap;
        m_texSetsmap = new retruxx::set<unsigned>[m_owner->m_level->land_size * m_owner->m_level->land_size];
        
        
        if (scoped_ptr stream = M3D_KERNEL->GetFileServer().CreateFileStream();
            stream->Open(m_owner->m_level->GetFullPathNameA(m_owner->m_level->m_colormapName).c_str(), fs::IStream::OPEN_READ))
        {
            //TODO: check this!!!!
            auto const streamSize = stream->GetSize();
            auto* data = new unsigned[streamSize / sizeof(unsigned)];
            stream->ReadBytes(data, streamSize);
            for (int i = 0; i < m_mapSize; ++i)
            {
                for (int j = 0; j < m_mapSize; ++j)
                {
                    m_colormap[j + i * (m_mapSize + 1)] = data[j + i * m_mapSize];
                }
            }
            delete[] data;
        }
        else
        {
            M3D_LOG_INFO("Cannot read colormap: " + m_owner->m_level->GetFullPathNameA(m_owner->m_level->m_colormapName) + "using default values");
            for (int i = 0; i < (this->m_mapSize + 1) * (this->m_mapSize + 1); ++i)
                this->m_colormap[i] = 0xFF808080; // Default gray color
        }
        
        delete[] m_heightMap;
        m_heightMap = new float[(m_mapSize + 1) * (m_mapSize + 1)];
        
        delete[] m_cliffHeightMap;
        m_cliffHeightMap = new unsigned char[(m_mapSize + 1) * (m_mapSize + 1)];


        // TODO: check this
        if (scoped_ptr stream = M3D_KERNEL->GetFileServer().CreateFileStream();
            stream->Open(m_owner->m_level->GetFullPathNameA(m_owner->m_level->m_hfName).c_str(), fs::IStream::OPEN_READ))
        {
            auto const streamSize = stream->GetSize();
            if (streamSize == 2 * this->m_mapSize * this->m_mapSize) // 16-bit heightfield
            {
                //TODO: check this
                auto* data = new unsigned short[streamSize];
                stream->ReadBytes(data, streamSize);
                for (int i = 0; i < m_mapSize; ++i)
                {
                    for (int j = 0; j < m_mapSize; ++j)
                    {
                        m_heightMap[j + i * (m_mapSize + 1)] = data[j + i * m_mapSize] * 0.12;
                    }
                    m_heightMap[m_mapSize + i * (m_mapSize + 1)] = m_heightMap[(m_mapSize - 1) + i * (m_mapSize + 1)];
                }
                memcpy(&m_heightMap[m_mapSize * (m_mapSize + 1)],
                    &m_heightMap[(m_mapSize - 1) * (m_mapSize + 1)],
                    (m_mapSize + 1) * sizeof(float));
                delete[] data;
            }
            else
            {
                auto* data = new float[streamSize];
                stream->ReadBytes(data, streamSize);

                // Process 32-bit height data with border expansion
                for (int j = 0; j < m_mapSize; j++)
                {
                    memcpy(&m_heightMap[j * (m_mapSize + 1)],
                        &data[j * m_mapSize],
                        m_mapSize * sizeof(float));
                    // Expand last column
                    m_heightMap[m_mapSize + j * (m_mapSize + 1)] = m_heightMap[(m_mapSize - 1) + j * (m_mapSize + 1)];
                }
                // Expand last row
                memcpy(&m_heightMap[m_mapSize * (m_mapSize + 1)],
                    &m_heightMap[(m_mapSize - 1) * (m_mapSize + 1)],
                    (m_mapSize + 1) * sizeof(float));

                delete[] data;
            }
        }
        else
        {
            M3D_LOG_INFO("Cannot read heightfield: " + m_owner->m_level->GetFullPathNameA(m_owner->m_level->m_hfName));
            return 0;
        }

        delete[] m_waterMap;

        auto landSize = 4 * this->m_owner->m_level->land_size;
        m_waterMap = new short[landSize * landSize];
        size_t waterMapSize = landSize * landSize * sizeof(short);

        // TODO check this!!
        if (scoped_ptr stream = M3D_KERNEL->GetFileServer().CreateFileStream();
            stream->Open(m_owner->m_level->GetFullPathNameA(m_owner->m_level->m_waterName).c_str(), fs::IStream::OPEN_READ))
        {
            unsigned int waterDataSize = stream->GetSize();
            if (waterDataSize == waterMapSize)
            {
                // Directly read into watermap if sizes match
                stream->ReadBytes(m_waterMap, waterDataSize);
            }
            else
            {
                // Process water data with conversion
                unsigned char* tempData = new unsigned char[waterDataSize];
                stream->ReadBytes(tempData, waterDataSize);

                float waterLevel = m_owner->m_level->waterlevel * 8.333334f;
                for (int i = 0; i < landSize * landSize; i++)
                {
                    if (tempData[i] == 0xFF)
                        m_waterMap[i] = (short)waterLevel;
                    else
                        m_waterMap[i] = 0;
                }

                delete[] tempData;
            }
        }
        else
        {
            M3D_LOG_INFO("Cannot open watermap: using empty waterfield", LOG_INFO);
            memset(m_waterMap, 0, landSize * landSize * sizeof(short));
        }

        // Load normal map
        delete[] m_normalMap;
        delete[] m_vnormal;

        m_vnormal = new CVector[(m_mapSize + 1) * (m_mapSize + 1)];

        unsigned int startTime = M3D_KERNEL->GetTimer().GetCurTime();

        if (!LoadNormalMap(m_owner->m_level->GetFullPathNameA(m_owner->m_level->m_normalMapName)))
        {
            RecalcNormalMap(0, 0, m_mapSize, m_mapSize);
        }
        
        unsigned int loadTime = M3D_KERNEL->GetTimer().GetCurTime() - startTime;
        M3D_LOG_INFO(CStr("Normal map loaded in: ") + loadTime);

        // Load tiles
        startTime =  M3D_KERNEL->GetTimer().GetCurTime();
        if (!LoadTiles(m_owner->m_level->GetFullPathNameA(m_owner->m_level->m_tilesFileName)))
        {
            return 0;
        }

        loadTime = M3D_KERNEL->GetTimer().GetCurTime() - startTime;
        M3D_LOG_INFO(CStr("Tiles loaded in: ") + loadTime);

        // Create LOD
        CreateLod();

        // Initialize passed cells
        delete[] m_passedCells;

        m_passedCells = new char[landSize * landSize];

        // Initialize reflection/refraction textures
        InitReflectionRefractionTextures();

        // Set draw radius
        float viewDist = M3D_KERNEL->GetEngineCfg().m_lsViewDistanceDivider.GetF();
        m_drawRadius = (int)(viewDist * 8.0f + 4.0f);
        m_drawRadius = std::max(4, std::min(m_drawRadius, 12));

        // Load shoreline
        startTime = M3D_KERNEL->GetTimer().GetCurTime();
        if (!LoadShoreLine(m_owner->m_level->GetFullPathNameA(m_owner->m_level->m_shoreLineName)))
        {
            GenerateShoreLine();
        }

        loadTime = M3D_KERNEL->GetTimer().GetCurTime() - startTime;
        M3D_LOG_INFO(CStr("ShoreLine loaded in: ") + loadTime);

        // Initialize grass
        InitGrass();
        ReadGrassFromXmlFile(m_owner->m_level->GetFullPathNameA("grass.xml").c_str());

        // Build final landscape
        BuildSolidLandscape();

        return 1;
    }

    void Landscape::RenderGrass(unsigned, GrassInstance**, int*, RenderGrassType)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::RenderGrass(retruxx::deque<retruxx::pair<int, int>> const&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    int Landscape::GetLsSize() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::RecalcUV()
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::UpdateTexturesFilters()
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::ScaleGrassRadius(CVector const&, float, float)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::LinkNodeCollisionGeomsToCell(SgNode*, int, int, int, int)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::DrawShoresLayer()
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::drawCellOverlayedShader(int, int, rend::IEffect*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    Landscape::TileInfo const& Landscape::GetTileInfo(int, int) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::Invalidate()
    {
        throw retruxx::logic_error("Not implemented");
    }

    unsigned Landscape::AddGrassInstance(int, CVector const&, float, float, bool)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::EndWaterQuery()
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::SetNodeCollisionGeomsEnabled(SgNode*, bool)
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool Landscape::Save16bitDisplace(CStr const&, int)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::DrawCollisionGeoms(bool)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::ClearCollisionCellsMap()
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool Landscape::AddGrassModel(char const*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::GenerateOneDPVSCellMesh(int, int, CVector*, int*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    float Landscape::GetFloatToShortScale() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::drawSpriteOverlayed2(float, float, float, float, unsigned, bool)
    {
        throw retruxx::logic_error("Not implemented");
    }

    int Landscape::RecalcNormalMap(int, int, int, int)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::ChangeShoreState(int, int, bool)
    {
        throw retruxx::logic_error("Not implemented");
    }

    float Landscape::GetHeight(float, float, int, bool)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::CheckLandscapeCollisionTriMeshesForObjId(int)
    {
        throw retruxx::logic_error("Not implemented");
    }

    GeomObject* Landscape::GetTerrainGeomObject() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::GetDPVSCollisionInfo(int, int, int&, int&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::RemoveGrassRadius(CVector const&, float)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::DisableShoreRegion(int, int)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::UpdateVis(bool vp)
    {
        //m_profilerUpdateVis->StartCountdown();
        if (!m_lockVis.GetB())
        {
            m_dirtyReflection = vp;
            m_frustumCull.CreateScreenFrustums(m_drawRadius * 128.0, 1.0, 1.0, 0.78539819);
            m_owner->GetGraph().UpdateVis(true, m_frustumCull, true);
            m_owner->GetGraph().EnableVisibleCells(m_frustumCull, 1u);
            m_owner->GetRoadManager().UpdateVis();
            m_numWaterCells = 0;
            if (M3D_KERNEL->GetEngineCfg().m_g_drawWater.GetB())
            {
                throw retruxx::logic_error("Not implemented");
            }
            if (m_numWaterCells)
            {
                throw retruxx::logic_error("Not implemented");
            }
        }
        //m_profilerUpdateVis->EndCountdown();
    }

    Landscape::CollisionCellItem* Landscape::GetCollisionCellItem(int, int) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    int Landscape::GenerateShoreLine()
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::ReleaseOdeCollisionData()
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool Landscape::LoadNormalMap(CStr const& fileName)
    {
        fs::auxTaggedFile taggedFile;
        if (taggedFile.Open(fileName.c_str(), fs::auxTaggedFile::PROCESS_NORMAL_IGNORE_CRC))
        {
            M3D_LOG_ERR("Error: Couldn't load normal map from file " + fileName);
            return false;
        }

        char* formatTitle = nullptr;
        taggedFile.getFormatTitle(&formatTitle);
        if (strcmp(formatTitle, "RIV"))
        {
            M3D_LOG_ERR("Error: Wrong normal map file format: " + CStr(formatTitle));
            return false;
        }

        unsigned formatVersion = 0;
        taggedFile.getFormatVersion(formatVersion);
        if (formatVersion != 1)
        {
            M3D_LOG_ERR("Error: Wrong normal map format version: " + CStr(formatVersion));
            return false;
        }

        void* data = nullptr;
        taggedFile.getChunkData(0xBADF00Du, &data);
        auto* charData = static_cast<unsigned char*>(data);
        charData += 4;

        // TODO: check and refactor
        if ((m_mapSize + 1) * (m_mapSize + 1) > 0)
        {
            int v12 = 0;
            int v13 = 0;
            do
            {
                auto v14 = &this->m_vnormal[v13];
                v14->x = (float)*(__int16*)charData * 0.000030518509;
                m_vnormal = this->m_vnormal;
                charData += 2;
                auto p_y = &m_vnormal[v13].y;
                *p_y = (float)*(__int16*)charData * 0.000030518509;
                charData += 2;
                ++v12;
                this->m_vnormal[v13++].z = sqrt(1.0 - v14->x * v14->x - *p_y * *p_y);
            } while (v12 < (this->m_mapSize + 1) * (this->m_mapSize + 1));
        }

        taggedFile.Close();
        return true;
    }

    float Landscape::GetScaleForTile() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::SwitchDrawMode()
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::RemoveCollisionTris(int)
    {
        throw retruxx::logic_error("Not implemented");
    }

    Object* Landscape::CreateObject()
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::getMinMaxHeightForBox(float* box, float buldgeY)
    {
        //TODO: check and refactor this
        //float VISCELL_EDGE_LENGTH_0 = 128.0;
        //auto v3 = VISCELL_EDGE_LENGTH_0;
        //auto v5 = static_cast<unsigned>(*box * (1.0 / VISCELL_EDGE_LENGTH_0));
        //auto v6 = (box[3] * (1.0 / VISCELL_EDGE_LENGTH_0));
        //auto v7 = static_cast<unsigned>(box[2] * (1.0 / VISCELL_EDGE_LENGTH_0));
        //auto v8 = box[5] * (1.0 / VISCELL_EDGE_LENGTH_0);
        //box[1] = -999999.0;
        //auto v9 = v8;
        //auto x1 = v6;
        //auto z0 = v7;
        //auto boxa = v8;
        //for (box[4] = -999999.0; v5 < v6; ++v5)
        //{
        //    if (v7 < v9)
        //    {
        //        do
        //        {
        //            auto v10 = this->m_cellParams;
        //            auto v11 = v10[v5 + v7 * this->m_owner->m_level->land_size].m_h0;
        //            auto v12 = v10[v5 + v7 * this->m_owner->m_level->land_size].m_h1;
        //            if (box[1] > v11)
        //                box[1] = v11;
        //            if (v11 > box[4])
        //                box[4] = v11;
        //            if (box[1] > v12)
        //                box[1] = v12;
        //            if (v12 > box[4])
        //                box[4] = v12;
        //            auto v13 = this->m_owner->m_level->land_size;
        //            if (v5 >= 0 || v7 >= 0 || v5 < v13 || v7 < v13)
        //            {
        //                auto v14 = this->m_cellParams;
        //                if (v14[v5 + v7 * v13].m_iswatercell)
        //                {
        //                    auto v15 = v14[v5 + v7 * v13].m_maxwater;
        //                    if (v15 > box[4])
        //                        box[4] = v15;
        //                }
        //            }
        //            v9 = boxa;
        //            ++v7;
        //        } while (v7 < boxa);
        //        v6 = x1;
        //        v7 = z0;
        //    }
        //}
        //box[4] = (box[4] + v3) + buldgeY;
        //throw retruxx::logic_error("Not implemented");

        constexpr float VISCELL_EDGE_LENGTH = 128.0f;
        const float invCellSize = 1.0f / VISCELL_EDGE_LENGTH;

        // Calculate grid indices for bounding box
        const unsigned minGridX = static_cast<unsigned>(box[0] * invCellSize);
        const unsigned minGridZ = static_cast<unsigned>(box[2] * invCellSize);
        const float maxGridX = box[3] * invCellSize;
        const float maxGridZ = box[5] * invCellSize;

        const unsigned landSize = m_owner->m_level->land_size;
        const float minInit = -999999.0f;
        float& minHeight = box[1];
        float& maxHeight = box[4];

        minHeight = minInit;
        maxHeight = minInit;

        // Process all grid cells in bounding box
        for (unsigned gridX = minGridX; gridX < maxGridX; ++gridX) {
            for (unsigned gridZ = minGridZ; gridZ < maxGridZ; ++gridZ) {
                // Skip out-of-bounds cells
                if (gridX >= landSize || gridZ >= landSize)
                    continue;

                const auto& cell = m_cellParams[gridX + gridZ * landSize];

                // Update min/max with terrain heights
                minHeight = std::min(minHeight, std::min(cell.m_h0, cell.m_h1));
                maxHeight = std::max(maxHeight, std::max(cell.m_h0, cell.m_h1));

                // Process water cells
                if (cell.m_iswatercell) {
                    maxHeight = std::max(maxHeight, cell.m_maxwater);
                }
            }
        }

        // Apply final offset to max height
        maxHeight += VISCELL_EDGE_LENGTH + buldgeY;
    }

    void Landscape::SetGameRenderMode()
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::CollectGrassCell(int, int, unsigned&, GrassInstance**, int*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::DrawGeom(dxGeom*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::Update()
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::DrawWaterLayer()
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::CreateHelperStructures()
    {
        //TODO: implement Landscape::CreateHelperStructures
        return;
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::drawSpriteOverlayed2Projected(float, float, float, float, unsigned, bool, CClipper const&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    Class* Landscape::GetBaseClass()
    {
        return RT_CLASS_LOCAL(SgNode);
    }

    void Landscape::PostServersLoad()
    {
        CreateHelperStructures();
        ConstructCollisionData();
    }

    void Landscape::RemoveGrassTile(int, int)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::DrawCells(cmn::vector<unsigned> const&, unsigned)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::GetVisCellHeights(float&, float&, int, int) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::CreateIndicesTriLists(int*, int, int)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::UpdateNodeCollisionGeoms(SgNode*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::ReleaseLod()
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::DrawMassBox(dMass*, CVector const&, Quaternion const&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool Landscape::InitGrass()
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::StartWaterQuery()
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::PutGrassToLandscape(CVector2 const&, CVector2 const&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    int Landscape::GetNumTiles() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::setDrawRadius(int, int, int)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::GetDrawedCellHeights(float&, float&, int, int) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    unsigned Landscape::GetNearestGrassInstance(CVector const&) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::DoneGrass()
    {
        throw retruxx::logic_error("Not implemented");
    }

    float Landscape::getCameraHeight(float, float) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    int Landscape::getGrassModelIdByName(char const*) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    int Landscape::SaveCameraMap(CStr const&, int)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::Release()
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::EnableShoreRegion(int, int)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::Render()
    {
        throw retruxx::logic_error("Not implemented");
    }

    int Landscape::Render(SgNodeRenderFlags, void*, int, int)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::ReadGrassFromXmlFile(char const*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool Landscape::WriteGrassToXmlFile(char const*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    CVector Landscape::getNormal(float, float)
    {
        throw retruxx::logic_error("Not implemented");
    }

    unsigned char Landscape::GetColor(float, float)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::SetPresenceOnCollisionMap(int, int)
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool Landscape::IsThisVisCellHasWater(int, int) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::drawSpriteOverlayed(unsigned, CVector const&, CVector const&, float)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::ReloadLightmapTexture(CStr const&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    Object* Landscape::Clone()
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool Landscape::LoadShoreLine(CStr const&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::GetFogStartAndEnd(float&, float&) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    int Landscape::GetNumAlphas() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::CreateLod()
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::drawSpriteOverlayedProjected(unsigned, CVector const&, CMatrix const&, CClipper const&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::CreateHeights(CellParams*, int, int)
    {
        throw retruxx::logic_error("Not implemented");
    }

    int Landscape::isWaterCell(int, int) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::Restore()
    {
        throw retruxx::logic_error("Not implemented");
    }

    float Landscape::getWaterHeight(int, int) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::ReleaseReflectionRefractionTextures()
    {
        throw retruxx::logic_error("Not implemented");
    }

    char const* Landscape::GetGrassModelName(unsigned) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::renderZGuard()
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::GetWaterCellHeights(float&, float&, int, int) const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::ManageLandScapeCollisionTriMeshes()
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::LinkPassMapCellToCollisionCell(PointBase<int> const&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::ReBuildShoresVb()
    {
        throw retruxx::logic_error("Not implemented");
    }

    int Landscape::LoadTiles(CStr const&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool Landscape::traceLineThruCellLs(float&, int, int, CVector const&, CVector const&, bool)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::RecursiveDisableShore(unsigned char*, int, int)
    {
        throw retruxx::logic_error("Not implemented");
    }

    Landscape::Landscape(Landscape const&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    int CreateIndices(uint16_t* indices, int sizeIndex, int sizeVertex, int step)
    {
        const int vertexPlusOne = sizeVertex + 1;
        uint16_t* currentIndex = indices;
        int vertexOffset = 0;
        const int stepVertexOffset = step * vertexPlusOne;
        const int loopLimit = 64;
        int loopCounter = loopLimit;

        do {
            if (sizeIndex > 0) {
                int remainingIndices = sizeIndex;
                do {
                    for (int i = 0; i <= sizeIndex; i++) {
                        *currentIndex = stepVertexOffset + vertexOffset;
                        currentIndex[1] = vertexOffset;

                        if (i == sizeIndex) {
                            vertexOffset += stepVertexOffset - sizeVertex;
                        }
                        else {
                            vertexOffset += step;
                        }
                        currentIndex += 2;
                    }

                    *currentIndex = sizeVertex + vertexOffset - stepVertexOffset;
                    currentIndex[1] = vertexPlusOne + vertexOffset;
                    currentIndex += 2;
                    remainingIndices--;
                } while (remainingIndices > 0);
            }

            const uint16_t finalIndex = vertexOffset + stepVertexOffset + vertexPlusOne;
            vertexOffset += vertexPlusOne;
            loopCounter--;

            *(currentIndex - 1) = finalIndex;
        } while (loopCounter != 1);

        return (currentIndex - indices) / loopLimit;
    }

    struct VertexWaterTest
    {
        __int16 x;
        __int16 y;
        __int16 z;
        __int16 w;
    };

    Landscape::Landscape()
    {
       RETRUXX_DLL_JMP_TO_CTOR(0x005B8950, Landscape::Landscape);

        // Set up lockVis CVar
        m_lockVis = CVar("lockVis", "0", CVar::CVAR_BOOL, CVar::CVAR_ARCHIVE);

        // Initialize pointers
        m_numGrassModels = 0;
        m_grassArray = nullptr;
        m_heightMap = nullptr;
        m_cliffHeightMap = nullptr;
        m_waterMap = nullptr;
        m_normalMap = nullptr;
        m_vnormal = nullptr;
        m_colormap = nullptr;
        m_texSetsmap = nullptr;
        m_remappedIndices = nullptr;
        m_oCollisionitems = nullptr;
        m_terrainObject = nullptr;
        m_owner = nullptr;
        m_cellParams = nullptr;
        m_drawedCellParams = nullptr;
        m_lastState = RT_LIGHTPASS;
        m_tiles = nullptr;

        // Create vertex buffers
        m_landUVVb = M3D_RENDERER->AddVb(rend::VertexType::VERTEX_STREAM_UV_S1, 0xFFFF, "LandUV", 0);
        m_landVb = M3D_RENDERER->AddVb(rend::VertexType::VERTEX_XYZNCT2, 1600, "LandEditor", 512);

        // Allocate dummy VB
        m_dummyVB = new rend::VertexXYZNCT2[1600];

        // Reserve space for water cells
        for (int i = 0; i < 16; i++) {
            waterCellsToDraw[i].reserve(100);
        }

        // Create water queries
        for (int i = 0; i < 3; i++) {
            m_waterQueries[i] = M3D_RENDERER->NewQuery(rend::IQuery::Type::QUERY_OCCLUSION); // OCCLUSION query type
        }

        // Initialize water state
        m_isWaterVisible = false;
        m_currWaterQuery = 0;
        m_drawtextured = true;
        m_passedCells = 0;
        m_CurAlphaSet = 0;
        m_loadAllTextures = false;

        // Register CVar with console
        M3D_KERNEL->GetEngineCfg().m_console->RegisterCVar(&m_lockVis, false);

        // Create LOD index buffers
        unsigned int lodFactor = 1;
        m_maxLOD = 0;

        for (int size = 4; size > 0; size >>= 1)
        {
            // Create index buffer for this LOD level
            int numIndices = ((size + 1) * (size + 1)) << 9;
            auto curIb = M3D_RENDERER->AddIb(numIndices, false);

            // Lock and fill index buffer
            unsigned short* v16 = (unsigned short*)M3D_RENDERER->LockIb(curIb, 384 * size * size, 0, 0);
            int createdIndices = CreateIndices(v16, size, 4, lodFactor);
            M3D_RENDERER->UnlockIb(curIb);

            // Store number of indices
            m_lsNumIndices.push_back(createdIndices);

            // Store IB handle
            m_landIbConst.push_back(curIb);

            // Update for next LOD level
            lodFactor *= 2;
            m_maxLOD++;
        }

        // Calculate max water cells per pass based on vertex shader constants
        m_maxWaterCellPerPass = M3D_RENDERER->GetMaxVertexShaderConst() - 20;

        // Create water vertex buffer
        m_waterVb = M3D_RENDERER->AddVb(m3d::rend::VertexType::VERTEX_WATERTEST, 81 * m_maxWaterCellPerPass, "Water", 0);

        // Initialize water vertices
        VertexWaterTest* waterVertices = static_cast<VertexWaterTest*>(M3D_RENDERER->LockVb(m_waterVb, 0, 0, 0));

        constexpr uint8_t gridSize = 9;  // Since loops go up to 8 (0-8 inclusive)
        VertexWaterTest currentVertex;

        for (uint8_t zCoord = 0; zCoord < m_maxWaterCellPerPass; ++zCoord) {
            currentVertex.z = zCoord;
            const int packedZ = *reinterpret_cast<int*>(&currentVertex.z);

            for (uint8_t yCoord = 0; yCoord < gridSize; ++yCoord) {
                currentVertex.y = yCoord;

                for (uint8_t xCoord = 0; xCoord < gridSize; ++xCoord) {
                    currentVertex.x = xCoord;

                    // Copy vertex data
                    *reinterpret_cast<int*>(&waterVertices->x) = *reinterpret_cast<int*>(&currentVertex.x);
                    *reinterpret_cast<int*>(&waterVertices->z) = packedZ;

                    ++waterVertices;
                }
            }
        }

        M3D_RENDERER->UnlockVb(m_waterVb);

        // Allocate water tile info
        waterTileInfo = new CVector4[m_maxWaterCellPerPass];

        // Create water index buffers for 16 LOD levels (4 patterns x 4 levels each)
        // First loop (4 iterations)
        int i = 0;
        int v46 = 0;
        do
        {
            int v31 = 1 << i;
            int v32 = (unsigned __int16)(1 << i);
            int v35 = 16 * (8 / v32 + 2) / v32 * this->m_maxWaterCellPerPass;
            int _Val = 16 * (8 / v32 + 2) / v32;

            m_waterIb[i] = M3D_RENDERER->AddIb(v35, 0);
            unsigned short* v38 = (unsigned short*)M3D_RENDERER->LockIb(m_waterIb[i], v35, 0, 0);
            int v39 = 0;
            int v40 = this->m_maxWaterCellPerPass <= 0;
            int n = 0;
            if (!v40)
            {
                do
                {
                    int v192 = 0;
                    int v43 = 0;
                    do
                    {
                        int v41 = 0;
                        do
                        {
                            int v42 = v39 + 9 * v31;
                            *v38 = v39;
                            v38[1] = v42;
                            if (v41 == 8)
                                v39 = v42 - 8;
                            else
                                v39 += v31;
                            v41 += v32;
                            v38 += 2;
                        } while (v41 <= 8);
                        *v38 = v39 + 8;
                        v43 = v32 + v192;
                        v38[1] = v39;
                        v38 += 2;
                        v192 = v43;
                    } while (v43 < 8);
                    v39 += 9;
                    int v44 = n + 1;
                    *(v38 - 1) = v39;
                    v40 = v44 < this->m_maxWaterCellPerPass;
                    n = v44;
                } while (v40);
            }

            M3D_RENDERER->UnlockIb(m_waterIb[i]);
            m_wtNumTris[i] = _Val;
            v46 = ++i < 4;

        } while (v46);

        i = 1;
        int curIbIdx = 6;
        do
        {
            int v47 = 1 << i;
            int v48 = (unsigned __int16)(1 << i);
            int v51 = 16 * (8 / v48 + 3) / v48 * this->m_maxWaterCellPerPass;
            int v206 = 16 * (8 / v48 + 3) / v48;

            m_waterIb[curIbIdx] = M3D_RENDERER->AddIb(v51, 0);
            unsigned short* v54 = (unsigned short*)M3D_RENDERER->LockIb(m_waterIb[curIbIdx], v51, 0, 0);

            int v55 = 0;
            int v40 = this->m_maxWaterCellPerPass <= 0;
            int v192 = 0;
            if (!v40)
            {
                int _Val = (unsigned __int16)v47 >> 1;
                int step = 10 * v47;
                do
                {
                    int v56 = 0;
                    do
                    {
                        v54[1] = v55 + 9 * v47;
                        v54[2] = v55 + _Val;
                        int v57 = v55 + step;
                        *v54 = v55;
                        v54[3] = v57;
                        v56 += v48;
                        v55 += v47;
                        v54 += 4;
                    } while (v56 < 8);
                    *v54 = v55;
                    int v58 = 9 * v47 + v55;
                    v54[1] = v58;
                    v54[2] = v58;
                    auto v59 = v54 + 3;
                    int v60 = v58 - 8;
                    *v59 = v60;
                    v54 = v59 + 1;
                    int n = v48;
                    if (v48 < 8)
                    {
                        int v63 = 0;
                        do
                        {
                            int v61 = 0;
                            do
                            {
                                int v62 = v60 + 9 * v47;
                                *v54 = v60;
                                v54[1] = v62;
                                if (v61 == 8)
                                    v60 = v62 - 8;
                                else
                                    v60 += v47;
                                v61 += v48;
                                v54 += 2;
                            } while (v61 <= 8);
                            *v54 = v60 + 8;
                            v63 = v48 + n;
                            v54[1] = v60;
                            v54 += 2;
                            n = v63;
                        } while (v63 < 8);
                    }
                    v55 = v60 + 9;
                    int v64 = v192 + 1;
                    *(v54 - 1) = v55;
                    v40 = v64 < this->m_maxWaterCellPerPass;
                    v192 = v64;
                } while (v40);
            }

            M3D_RENDERER->UnlockIb(m_waterIb[curIbIdx]);
            m_wtNumTris[curIbIdx] = v206;
            curIbIdx += 4;
            v46 = ++i < 4;
        } while (v46);

        curIbIdx = 5;
        i = 1;
        do
        {
            int v66 = 1 << i;
            int v67 = (unsigned __int16)(1 << i);
            int v69 = (16 * (8 / v67 + 2) / v67 + 24 / v67) * this->m_maxWaterCellPerPass;
            int v206 = 16 * (8 / v67 + 2) / v67 + 24 / v67;

            m_waterIb[curIbIdx] = M3D_RENDERER->AddIb(v69, 0);
            unsigned short* v72 = (unsigned short*)M3D_RENDERER->LockIb(m_waterIb[curIbIdx], v69, 0, 0);

            int v73 = 0;
            int v40 = this->m_maxWaterCellPerPass <= 0;
            unsigned short* v74 = v72;
            int v192 = 0;
            if (!v40)
            {
                int _Val = 9 * v67 / 2;
                int step = 10 * v66;
                do
                {
                    int n = 0;
                    int v79 = 0;
                    do
                    {
                        v74[1] = v73 + _Val;
                        v74[3] = v73 + 9 * v66;
                        int v75 = v73 + step;
                        *v74 = v73;
                        v74[2] = v73 + v66;
                        v73 += v66;
                        v74[4] = v75;
                        int v76 = v67;

                        unsigned short* m = v74 + 5;
                        for (; v76 <= 8; m += 2)
                        {
                            int v78 = v73 + 9 * v66;
                            *m = v73;
                            m[1] = v78;
                            if (v76 == 8)
                                v73 = v78 - 8;
                            else
                                v73 += v66;
                            v76 += v67;
                        }
                        v79 = v67 + n;
                        *m = v73 + 8;
                        m[1] = v73;
                        v74 = m + 2;
                        n = v79;
                    } while (v79 < 8);
                    v73 += 9;
                    int v80 = v192 + 1;
                    *(v74 - 1) = v73;
                    v40 = v80 < this->m_maxWaterCellPerPass;
                    v192 = v80;
                } while (v40);
            }

            M3D_RENDERER->UnlockIb(m_waterIb[curIbIdx]);
            m_wtNumTris[curIbIdx] = v206;
            curIbIdx += 4;
            v46 = ++i < 4;
        } while (v46);

        i = 1;
        curIbIdx = 4;
        do
        {
            int v82 = 1 << i;
            __int16 v83 = (unsigned __int16)(1 << i);
            int step = 1 << i;

            int v86 = (16 * (8 / v83 + 2) / v83 + 24 / v83) * m_maxWaterCellPerPass;
            int formal = 16 * (8 / v83 + 2) / v83 + 24 / v83;

            m_waterIb[curIbIdx] = M3D_RENDERER->AddIb(v86, 0);
            unsigned short* v89 = (unsigned short*)M3D_RENDERER->LockIb(m_waterIb[curIbIdx], v86, 0, 0);

            int v90 = 0;
            int v40 = this->m_maxWaterCellPerPass <= 0;
            int _Val = 0;
            if (!v40)
            {
                auto v86 = (unsigned __int16)v82 >> 1;
                int v206 = 8 - v83;
                i = 9 * v82;
                int v208 = 9 * v82 + v86;
                int v207 = 10 * v82;
                while (1)
                {
                    int n = 0;
                    if (v206 > 0)
                    {
                        int v93 = 0;
                        do
                        {
                            int v91 = 0;
                            do
                            {
                                int v92 = v90 + i;
                                *v89 = v90;
                                v89[1] = v92;
                                if (v91 == 8)
                                    v90 = v92 - 8;
                                else
                                    v90 += v82;
                                v91 += v83;
                                v89 += 2;
                            } while (v91 <= 8);
                            *v89 = v90 + 8;
                            v93 = v83 + n;
                            v89[1] = v90;
                            v89 += 2;
                            n = v93;
                        } while (v93 < v206);
                    }
                    int v94 = 0;
                    int v95 = 0;
                    while (1)
                    {
                        v89[1] = v90 + i;
                        v95 = v90 + v82;
                        v89[3] = v90 + v208;
                        int v96 = v207;
                        *v89 = v90;
                        v89[4] = v96 + v90;
                        v89[2] = v95;
                        v94 += v83;
                        v89 += 5;
                        v90 = v95;
                        if (v94 >= 8)
                            break;
                        v82 = step;
                    }
                    int v97 = i;
                    int v98 = v95 + i;
                    v89[1] = v90 + i;
                    v89[2] = v98;
                    *v89 = v90;
                    v90 += v97 + 1;
                    int v99 = _Val;
                    auto v100 = v89 + 3;
                    *v100 = v90;
                    ++v99;
                    v89 = v100 + 1;
                    v40 = v99 < this->m_maxWaterCellPerPass;
                    _Val = v99;
                    if (!v40)
                        break;
                    v82 = step;
                }
            }

            M3D_RENDERER->UnlockIb(m_waterIb[curIbIdx]);
            m_wtNumTris[curIbIdx] = formal;
            curIbIdx += 4;
            v46 = ++i < 4;
        } while (v46);

        i = 1;
        curIbIdx = 7;
        do
        {
            int v102 = 1 <<i;
            auto v103 = (unsigned __int16)(1 << i);
            int v105 = (16 * (8 / v103 + 2) / v103 + 16 / v103) * this->m_maxWaterCellPerPass;
            int v208 = 16 * (8 / v103 + 2) / v103 + 16 / v103;

            m_waterIb[curIbIdx] = M3D_RENDERER->AddIb(v105, 0);
            unsigned short* v108 = (unsigned short*)M3D_RENDERER->LockIb(m_waterIb[curIbIdx], v105, 0, 0);
           
            int v109 = 0;
            int v40 = this->m_maxWaterCellPerPass <= 0;
            auto v110 = v108;
            int v206 = 0;
            if (!v40)
            {
                int _Val = 8 - v103;
                int formal = 9 * v103 / 2;
                int v207 = 10 * v102;
                do
                {
                    int v115 = 0;
                    for (int step = 0; step < 8; step = (unsigned __int16)v102 + v115)
                    {
                        for (int ii = 0; ii < _Val; v110 += 2)
                        {
                            int v112 = v109 + 9 * v102;
                            *v110 = v109;
                            v110[1] = v112;
                            if (ii == 8)
                                v109 = v112 - 8;
                            else
                                v109 += v102;
                            ii += v103;
                        }
                        v110[2] = v109 + v102;
                        int v113 = v109 + 9 * v102;
                        v110[1] = v113;
                        v110[4] = v113;
                        int v114 = v109 + v207;
                        v110[3] = formal + v109 + v102;
                        v115 = step;
                        *v110 = v109;
                        v110[5] = v114;
                        v110[6] = v114;
                        auto v116 = v110 + 7;
                        v103 = (unsigned __int16)v102;
                        v109 = v114 - 8;
                        *v116 = v114 - 8;
                        v110 = v116 + 1;
                    }
                    v109 += 9;
                    int v117 = v206 + 1;
                    *(v110 - 1) = v109;
                    v40 = v117 < this->m_maxWaterCellPerPass;
                    v206 = v117;
                } while (v40);
            }

            M3D_RENDERER->UnlockIb(m_waterIb[curIbIdx]);
            m_wtNumTris[curIbIdx] = v208;
            curIbIdx += 4;
            v46 = ++i < 4;
        } while (v46);

        // Setup profilers
        m_profilerUpdateVis = M3D_APP->GetProfilerStack().GetProfiler(M3D_APP->GetProfilerStack().AddProfiler("ls update vis", 30));
        m_profilerDraw = M3D_APP->GetProfilerStack().GetProfiler(M3D_APP->GetProfilerStack().AddProfiler("ls render", 30));
        m_profilerDrawGrass = M3D_APP->GetProfilerStack().GetProfiler(M3D_APP->GetProfilerStack().AddProfiler("ls grass", 30));
        m_profilerDrawWater = M3D_APP->GetProfilerStack().GetProfiler(M3D_APP->GetProfilerStack().AddProfiler("ls water", 30));

        // Setup debug counter
        m_countPhysicObjsInCells = M3D_APP->GetDbgCounterStack().GetCounter(M3D_APP->GetDbgCounterStack().AddCounter("PhysicObjs in cells"));

        // Initialize shaders to null
        m_solidDeepPs = nullptr;
        m_solidDeepVs = nullptr;
        m_waterPs = nullptr;
        m_waterVs = nullptr;
        m_waterDumbPs = nullptr;
        m_waterDumbVs = nullptr;

        // Load shores shader
        m_shoresShader = M3D_RENDERER->NewEffect("data/shaders/surf.fx", true);
        M3D_ASSERT(m_shoresShader);
        m_shoresShader->SetDefaultTechnique(true);

        // Load solid VS
        m_solidVs = M3D_RENDERER->NewHlslShader("data/shaders/landscapeSolid_ps11.vs", "LandscapeVS", rend::IHlslShader::Profile::VS_1_1);
        M3D_ASSERT(m_solidVs);

        // Load solid PS
        m_solidPs = M3D_RENDERER->NewHlslShader("data/shaders/landscapeSolid_ps11.ps", "LandscapePS", rend::IHlslShader::Profile::PS_1_1);
        M3D_ASSERT(m_solidPs);

        // Load solid bind VS
        m_solidBindVs = M3D_RENDERER->NewHlslShader("data/shaders/landscapeSolidBind_ps11.vs", "LandscapeVS", rend::IHlslShader::Profile::VS_1_1);
        M3D_ASSERT(m_solidBindVs);

        // Load solid bind PS
        m_solidBindPs = M3D_RENDERER->NewHlslShader("data/shaders/landscapeSolidBind_ps11.ps", "LandscapePS", rend::IHlslShader::Profile::PS_1_1);
        M3D_ASSERT(m_solidBindPs);

        // Load landscape VS
        m_landscapeVs = M3D_RENDERER->NewHlslShader("data/shaders/landscape_ps11.vs", "LandscapeVS", rend::IHlslShader::Profile::VS_1_1);
        M3D_ASSERT(m_landscapeVs);

        // Load landscape FP PS
        m_landscapePsFP = M3D_RENDERER->NewHlslShader("data/shaders/landscapeFP_ps11.ps", "LandscapePS", rend::IHlslShader::Profile::PS_1_1);
        M3D_ASSERT(m_landscapePsFP);

        // Load landscape SP PS
        m_landscapePsSP = M3D_RENDERER->NewHlslShader("data/shaders/landscapeSP_ps11.ps", "LandscapePS", rend::IHlslShader::Profile::PS_1_1);
        M3D_ASSERT(m_landscapePsSP);

        // Final null initializations
        overlayShader = nullptr;
        m_waterDumbVs = nullptr;
        m_waterDumbPs = nullptr;
    }

    void Landscape::BuildCells0(rend::VertexLandscape*, TIVChunk&, int&, cmn::vector<unsigned> const&, RenderTypes, retruxx::vector<int, retruxx::allocator<int>>&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::DrawCellsFast0(cmn::vector<unsigned> const&, TIVChunk&, RenderTypes)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::RecursiveEnableShore(unsigned char*, int, int)
    {
        throw retruxx::logic_error("Not implemented");
    }

    bool Landscape::traceLineThruCellLs0(float&, int, int, CVector const&, CVector const&)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::DrawCells0(cmn::vector<unsigned> const&, RenderTypes)
    {
        throw retruxx::logic_error("Not implemented");
    }

    int Landscape::IsBackfaced(int, int, rend::Cull)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::FreeTiles()
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::BuildUVSet()
    {
        throw retruxx::logic_error("Not implemented");
    }

    Landscape::VisibilityMode Landscape::GetCurVisMode() const
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::SetCurVisMode(VisibilityMode)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::DrawNonTransformGeom(dxGeom*)
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::RenderRoads()
    {
        throw retruxx::logic_error("Not implemented");
    }

    void Landscape::Register()
    {
        g_Kernel->AddClass(RT_CLASS_LOCAL(RoadNode));
        g_Kernel->AddClass(RT_CLASS_LOCAL(GeomObject));
        g_Kernel->AddClass(RT_CLASS_LOCAL(GeomObjectWater));
        g_Kernel->AddClass(RT_CLASS_LOCAL(GeomObjectLandscape));
        g_Kernel->AddClass(RT_CLASS_LOCAL(GeomObjectStatics));
        g_Kernel->AddClass(RT_CLASS_LOCAL(GeomObjectRoad));
        g_Kernel->AddClass(RT_CLASS_LOCAL(GeomObjectPassCell));
    }
}
