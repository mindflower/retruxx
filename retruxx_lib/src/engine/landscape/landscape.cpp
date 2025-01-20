#include "roads/road.h"
#include <geomobject.h>
#include <landscape.h>
#include <stdexcept>
#include <core/kernel.h>
#include <core/ini.h>

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

    int Landscape::Load()
    {
        //TODO: implement Landscape::Load
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
                this->m_colormap[i] = -8421505;
        }
        
        delete[] m_heightMap;
        m_heightMap = new float[(m_mapSize + 1) * (m_mapSize + 1)];
        
        delete[] m_cliffHeightMap;
        m_cliffHeightMap = new unsigned char[(m_mapSize + 1) * (m_mapSize + 1)];

        return 1;
        
        if (scoped_ptr stream = M3D_KERNEL->GetFileServer().CreateFileStream();
            stream->Open(m_owner->m_level->GetFullPathNameA(m_owner->m_level->m_hfName).c_str(), fs::IStream::OPEN_READ))
        {
            auto const streamSize = stream->GetSize();
            if (streamSize == 2 * this->m_mapSize * this->m_mapSize)
            {
                //TODO: check this
                auto* data = new unsigned[streamSize / sizeof(float)];
                stream->ReadBytes(data, streamSize);
                for (int i = 0; i < m_mapSize; ++i)
                {
                    for (int j = 0; j < m_mapSize; ++j)
                    {
                        m_heightMap[j + i * (m_mapSize + 1)] = data[j + i * m_mapSize];
                    }
                }
                delete[] data;
            }
            else
            {
                throw retruxx::logic_error("Not implemented");
            }
        }
        else
        {
            M3D_LOG_INFO("Cannot read heightfield: " + m_owner->m_level->GetFullPathNameA(m_owner->m_level->m_hfName));
            return 0;
        }
        
        throw retruxx::logic_error("Not implemented");
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

    bool Landscape::LoadNormalMap(CStr const&)
    {
        throw retruxx::logic_error("Not implemented");
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
        float VISCELL_EDGE_LENGTH_0 = 128.0;
        auto v3 = VISCELL_EDGE_LENGTH_0;
        auto v5 = static_cast<unsigned>(*box * (1.0 / VISCELL_EDGE_LENGTH_0));
        auto v6 = (box[3] * (1.0 / VISCELL_EDGE_LENGTH_0));
        auto v7 = static_cast<unsigned>(box[2] * (1.0 / VISCELL_EDGE_LENGTH_0));
        auto v8 = box[5] * (1.0 / VISCELL_EDGE_LENGTH_0);
        box[1] = -999999.0;
        auto v9 = v8;
        auto x1 = v6;
        auto z0 = v7;
        auto boxa = v8;
        for (box[4] = -999999.0; v5 < v6; ++v5)
        {
            if (v7 < v9)
            {
                do
                {
                    auto v10 = this->m_cellParams;
                    auto v11 = v10[v5 + v7 * this->m_owner->m_level->land_size].m_h0;
                    auto v12 = v10[v5 + v7 * this->m_owner->m_level->land_size].m_h1;
                    if (box[1] > v11)
                        box[1] = v11;
                    if (v11 > box[4])
                        box[4] = v11;
                    if (box[1] > v12)
                        box[1] = v12;
                    if (v12 > box[4])
                        box[4] = v12;
                    auto v13 = this->m_owner->m_level->land_size;
                    if (v5 >= 0 || v7 >= 0 || v5 < v13 || v7 < v13)
                    {
                        auto v14 = this->m_cellParams;
                        if (v14[v5 + v7 * v13].m_iswatercell)
                        {
                            auto v15 = v14[v5 + v7 * v13].m_maxwater;
                            if (v15 > box[4])
                                box[4] = v15;
                        }
                    }
                    v9 = boxa;
                    ++v7;
                } while (v7 < boxa);
                v6 = x1;
                v7 = z0;
            }
        }
        box[4] = (box[4] + v3) + buldgeY;
        //throw retruxx::logic_error("Not implemented");
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

    int CreateIndices(unsigned short*, int, unsigned short, unsigned short)
    {
        throw retruxx::logic_error("Not implemented");
    }

    struct VertexWaterTest
    {
        __int16 x;
        __int16 y;
        __int16 z;
        __int16 w;
    };

    Landscape::Landscape() :
        m_lockVis("lockVis", "0", CVar::CVAR_BOOL, CVar::CVAR_ARCHIVE)
    {
        RETRUXX_DLL_JMP_TO_CTOR(0x005B8950, Landscape::Landscape);

        this->m_numGrassModels = 0;
        this->m_grassArray = 0;
        this->m_heightMap = 0;
        this->m_cliffHeightMap = 0;
        this->m_waterMap = 0;
        this->m_normalMap = 0;
        this->m_vnormal = 0;
        this->m_colormap = 0;
        this->m_texSetsmap = 0;
        this->m_remappedIndices = 0;
        this->m_oCollisionitems = 0;
        this->m_terrainObject = 0;
        this->m_owner = 0;
        this->m_cellParams = 0;
        this->m_drawedCellParams = 0;
        this->m_lastState = RT_LIGHTPASS;
        this->m_tiles = 0;
        m_landUVVb = M3D_RENDERER->AddVb(rend::VERTEX_STREAM_UV_S1, 0xFFFF, "LandUV", 0);
        m_landVb = M3D_RENDERER->AddVb(rend::VERTEX_XYZNCT2, 1600, "LandEditor", 512);
        for (auto& waterCellToDraw : waterCellsToDraw)
        {
            waterCellToDraw.reserve(0x64);
        }
        //m_dummyVB =
        for (auto& query : m_waterQueries)
        {
            query = M3D_RENDERER->NewQuery(rend::IQuery::QUERY_OCCLUSION);
        }
        this->m_isWaterVisible = 0;
        this->m_currWaterQuery = 0;
        this->m_drawtextured = 1;
        this->m_passedCells = 0;
        this->m_CurAlphaSet = 0;
        this->m_loadAllTextures = 0;
        m3d::g_Kernel->GetEngineCfg().m_console->RegisterCVar(&this->m_lockVis, 0);
        this->m_maxLOD = 0;
        int szindex = 4;
        int i = 1;
        do
        {
            auto curIb = M3D_RENDERER->AddIb(((szindex + 1) * (szindex + 1)) << 9, false);
            auto ptr = M3D_RENDERER->LockIb(curIb, 384 * szindex * szindex, 0, 0);
            auto indices = CreateIndices(static_cast<unsigned short*>(ptr), szindex, 4, i);
            m_lsNumIndices.push_back(indices);
            M3D_RENDERER->UnlockIb(curIb);
            m_landIbConst.push_back(curIb);
            i *= 2;
            ++m_maxLOD;
            szindex >>= 1;
        } while (szindex);
        m_maxWaterCellPerPass = M3D_RENDERER->GetMaxVertexShaderConst();
        m_waterVb = M3D_RENDERER->AddVb(rend::VERTEX_WATERTEST, 81 * m_maxWaterCellPerPass, "Water", 0);
        
        //TODO: check this
        auto waterVbPtr = static_cast<VertexWaterTest*>(M3D_RENDERER->LockVb(this->m_waterVb, 0, 0, 0));
        VertexWaterTest vertexWaterTest;
        for (int j = 0; j < this->m_maxWaterCellPerPass; ++j)
        {
            vertexWaterTest.z = j;
            auto v28 = *&vertexWaterTest.z;
            for (int k = 0; k <= 8u; ++k)
            {
                auto v30 = 0;
                vertexWaterTest.y = k;
                do
                {
                    vertexWaterTest.x = v30;
                    *&waterVbPtr->x = *&vertexWaterTest.x;
                    *&waterVbPtr->z = v28;
                    ++v30;
                    ++waterVbPtr;
                } while (v30 <= 8u);
            }
        }
        M3D_RENDERER->UnlockVb(this->m_waterVb);
        waterTileInfo = new CVector4[m_maxWaterCellPerPass];
        i = 0;
        for (int i = 0; i < 4; ++i)
        {
            auto v32 = (1 << i);
            auto v35 = 16 * (8 / v32 + 2) / (unsigned)v32 * this->m_maxWaterCellPerPass;
            m_waterIb[i] = M3D_RENDERER->AddIb(v35, false);
            auto waterIbPtr = (WORD*)M3D_RENDERER->LockIb(m_waterIb[i], v35, 0, 0);
            //TODO: refactor this shit
            auto n = 0;
            auto v39 = 0;
            auto v40 = this->m_maxWaterCellPerPass <= 0;
            auto v43 = 0;
            if (!v40)
            {
                do
                {
                    auto v192 = 0;
                    do
                    {
                        auto v41 = 0;
                        do
                        {
                            auto v42 = v39 + 9 * i;
                            *waterIbPtr = v39;
                            waterIbPtr[1] = v42;
                            if (v41 == 8)
                                v39 = v42 - 8;
                            else
                                v39 += i;
                            v41 += v32;
                            waterIbPtr += 2;
                        } while (v41 <= 8);
                        *waterIbPtr = v39 + 8;
                        auto v43 = v32 + v192;
                        waterIbPtr[1] = v39;
                        waterIbPtr += 2;
                        v192 = v43;
                    } while (v43 < 8);
                    v39 += 9;
                    auto v44 = n + 1;
                    *(waterIbPtr - 1) = v39;
                    v40 = v44 < this->m_maxWaterCellPerPass;
                    n = v44;
                } while (v40);
            }
            M3D_RENDERER->UnlockIb(m_waterIb[i]);
        }

        throw retruxx::logic_error("Not implemented");
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
