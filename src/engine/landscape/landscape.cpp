#include "roads/road.h"
#include <geomobject.h>
#include <landscape.h>
#include <stdexcept>
#include <core/kernel.h>

#include "config.h"
#include "level.h"
#include "m3dapp.h"
#include "world.h"
#include "core/log.h"
#include "math/vector4.h"

namespace m3d
{
    RT_CLASS_EXPORTS_BEGIN(Landscape)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(Landscape);

    void Landscape::LinkObstacleToCells(ai::Obstacle*)
    {
        throw std::logic_error("Not implemented");
    }

    bool Landscape::SaveShoreLine(CStr const&)
    {
        throw std::logic_error("Not implemented");
    }

    int Landscape::GetTileSize() const
    {
        throw std::logic_error("Not implemented");
    }

    void Landscape::SetOverlayShader(rend::IEffect*)
    {
        throw std::logic_error("Not implemented");
    }

    void Landscape::LinkNodeAndChildrenCollisionGeomsToCell(SgNode*)
    {
        throw std::logic_error("Not implemented");
    }

    float Landscape::GetHeightWithCollisions(float, float, bool) const
    {
        throw std::logic_error("Not implemented");
    }

    void Landscape::InitReflectionRefractionTextures()
    {
        throw std::logic_error("Not implemented");
    }

    void Landscape::SetAllTexturesLoading(bool)
    {
        throw std::logic_error("Not implemented");
    }

    void Landscape::FreeShoresStuff()
    {
        throw std::logic_error("Not implemented");
    }

    void Landscape::BuildSolidLandscape()
    {
        throw std::logic_error("Not implemented");
    }

    void Landscape::DrawCellsOverlayedEditor(std::vector<unsigned> const&, unsigned)
    {
        throw std::logic_error("Not implemented");
    }

    void Landscape::DrawSolidLandscape(LandRenderMode, int)
    {
        throw std::logic_error("Not implemented");
    }

    void Landscape::RemoveGrassInstance(unsigned)
    {
        throw std::logic_error("Not implemented");
    }

    void Landscape::QueryWaterVisibility()
    {
        throw std::logic_error("Not implemented");
    }

    void Landscape::DrawLandScapeTextures(VisibilityMode, bool, bool)
    {
        throw std::logic_error("Not implemented");
    }

    rend::TexHandle Landscape::GetLightmapTexture() const
    {
        throw std::logic_error("Not implemented");
    }

    void Landscape::RemoveGrassRectangle(CVector2 const&, CVector2 const&)
    {
        throw std::logic_error("Not implemented");
    }

    void Landscape::ChangedNumberOfUsedTextures(unsigned)
    {
        throw std::logic_error("Not implemented");
    }

    int Landscape::ConstructCollisionData()
    {
        throw std::logic_error("Not implemented");
    }

    void Landscape::setOwner(CWorld* world)
    {
        m_owner = world;
    }

    Class* Landscape::GetClass() const
    {
        throw std::logic_error("Not implemented");
    }

    CStr const& Landscape::GetPathToTiles() const
    {
        throw std::logic_error("Not implemented");
    }

    void Landscape::SetEditorRenderMode()
    {
        throw std::logic_error("Not implemented");
    }

    void Landscape::LinkNodeObstacleToCells(SgNode*)
    {
        throw std::logic_error("Not implemented");
    }

    void Landscape::ReadTileInfo(int)
    {
        throw std::logic_error("Not implemented");
    }

    void Landscape::_dbgGenerateGrass()
    {
        throw std::logic_error("Not implemented");
    }

    void Landscape::DrawJoint(dxJoint*)
    {
        throw std::logic_error("Not implemented");
    }

    Landscape::~Landscape()
    {
        throw std::logic_error("Not implemented");
    }

    void Landscape::UnlinkNodeCollisionGeomsFromCell(SgNode*, int, int, bool)
    {
        throw std::logic_error("Not implemented");
    }

    int Landscape::New(float)
    {
        throw std::logic_error("Not implemented");
    }

    bool Landscape::HandleCVar(CVar const*, CConsoleParams const&)
    {
        throw std::logic_error("Not implemented");
    }

    int Landscape::AddOneTexture(CStr const&)
    {
        throw std::logic_error("Not implemented");
    }

    void Landscape::HandleCommand(int, CConsoleParams const&)
    {
        throw std::logic_error("Not implemented");
    }

    void Landscape::AddCollisionTris(int, int, CVector*, int, unsigned short*, CMatrix const&)
    {
        throw std::logic_error("Not implemented");
    }

    void Landscape::DrawShoreLine()
    {
        throw std::logic_error("Not implemented");
    }

    void Landscape::ReloadWaterTextures()
    {
        throw std::logic_error("Not implemented");
    }

    bool Landscape::SaveNormalMap(CStr const&)
    {
        throw std::logic_error("Not implemented");
    }

    bool Landscape::SaveColorMap(CStr const&, int)
    {
        throw std::logic_error("Not implemented");
    }

    void Landscape::SetLsHeight(float, float, float)
    {
        throw std::logic_error("Not implemented");
    }

    float Landscape::GetLsHeight(float, float) const
    {
        throw std::logic_error("Not implemented");
    }

    unsigned Landscape::GetNumGrassModels() const
    {
        throw std::logic_error("Not implemented");
    }

    void Landscape::setHgtAtHfPoint(int, int, float)
    {
        throw std::logic_error("Not implemented");
    }

    float Landscape::getHgtAtHfPoint(int, int) const
    {
        throw std::logic_error("Not implemented");
    }

    rend::TexHandle Landscape::GetTexHandleFromList(unsigned) const
    {
        throw std::logic_error("Not implemented");
    }

    void Landscape::SaveTileInfo()
    {
        throw std::logic_error("Not implemented");
    }

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
            xmlFile->GetFirstChild_(beachsets, "Beachsets");
            for (beachsets->GetFirstChild_(wave, "Wave"); !wave->IsEmpty(); wave->GetNextSibling_(wave, "Wave"))
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

        throw std::logic_error("Not implemented");
    }

    void Landscape::RenderGrass(unsigned, GrassInstance**, int*, RenderGrassType)
    {
        throw std::logic_error("Not implemented");
    }

    void Landscape::RenderGrass(std::deque<std::pair<int, int>> const&)
    {
        throw std::logic_error("Not implemented");
    }

    int Landscape::GetLsSize() const
    {
        throw std::logic_error("Not implemented");
    }

    void Landscape::RecalcUV()
    {
        throw std::logic_error("Not implemented");
    }

    void Landscape::UpdateTexturesFilters()
    {
        throw std::logic_error("Not implemented");
    }

    void Landscape::ScaleGrassRadius(CVector const&, float, float)
    {
        throw std::logic_error("Not implemented");
    }

    void Landscape::LinkNodeCollisionGeomsToCell(SgNode*, int, int, int, int)
    {
        throw std::logic_error("Not implemented");
    }

    void Landscape::DrawShoresLayer()
    {
        throw std::logic_error("Not implemented");
    }

    void Landscape::drawCellOverlayedShader(int, int, rend::IEffect*)
    {
        throw std::logic_error("Not implemented");
    }

    Landscape::TileInfo const& Landscape::GetTileInfo(int, int) const
    {
        throw std::logic_error("Not implemented");
    }

    void Landscape::Invalidate()
    {
        throw std::logic_error("Not implemented");
    }

    unsigned Landscape::AddGrassInstance(int, CVector const&, float, float, bool)
    {
        throw std::logic_error("Not implemented");
    }

    void Landscape::EndWaterQuery()
    {
        throw std::logic_error("Not implemented");
    }

    void Landscape::SetNodeCollisionGeomsEnabled(SgNode*, bool)
    {
        throw std::logic_error("Not implemented");
    }

    bool Landscape::Save16bitDisplace(CStr const&, int)
    {
        throw std::logic_error("Not implemented");
    }

    void Landscape::DrawCollisionGeoms(bool)
    {
        throw std::logic_error("Not implemented");
    }

    void Landscape::ClearCollisionCellsMap()
    {
        throw std::logic_error("Not implemented");
    }

    bool Landscape::AddGrassModel(char const*)
    {
        throw std::logic_error("Not implemented");
    }

    void Landscape::GenerateOneDPVSCellMesh(int, int, CVector*, int*)
    {
        throw std::logic_error("Not implemented");
    }

    float Landscape::GetFloatToShortScale() const
    {
        throw std::logic_error("Not implemented");
    }

    void Landscape::drawSpriteOverlayed2(float, float, float, float, unsigned, bool)
    {
        throw std::logic_error("Not implemented");
    }

    int Landscape::RecalcNormalMap(int, int, int, int)
    {
        throw std::logic_error("Not implemented");
    }

    void Landscape::ChangeShoreState(int, int, bool)
    {
        throw std::logic_error("Not implemented");
    }

    float Landscape::GetHeight(float, float, int, bool)
    {
        throw std::logic_error("Not implemented");
    }

    void Landscape::CheckLandscapeCollisionTriMeshesForObjId(int)
    {
        throw std::logic_error("Not implemented");
    }

    GeomObject* Landscape::GetTerrainGeomObject() const
    {
        throw std::logic_error("Not implemented");
    }

    void Landscape::GetDPVSCollisionInfo(int, int, int&, int&)
    {
        throw std::logic_error("Not implemented");
    }

    void Landscape::RemoveGrassRadius(CVector const&, float)
    {
        throw std::logic_error("Not implemented");
    }

    void Landscape::DisableShoreRegion(int, int)
    {
        throw std::logic_error("Not implemented");
    }

    void Landscape::UpdateVis(bool)
    {
        throw std::logic_error("Not implemented");
    }

    Landscape::CollisionCellItem* Landscape::GetCollisionCellItem(int, int) const
    {
        throw std::logic_error("Not implemented");
    }

    int Landscape::GenerateShoreLine()
    {
        throw std::logic_error("Not implemented");
    }

    void Landscape::ReleaseOdeCollisionData()
    {
        throw std::logic_error("Not implemented");
    }

    bool Landscape::LoadNormalMap(CStr const&)
    {
        throw std::logic_error("Not implemented");
    }

    float Landscape::GetScaleForTile() const
    {
        throw std::logic_error("Not implemented");
    }

    void Landscape::SwitchDrawMode()
    {
        throw std::logic_error("Not implemented");
    }

    void Landscape::RemoveCollisionTris(int)
    {
        throw std::logic_error("Not implemented");
    }

    Object* Landscape::CreateObject()
    {
        throw std::logic_error("Not implemented");
    }

    void Landscape::getMinMaxHeightForBox(float*, float)
    {
        throw std::logic_error("Not implemented");
    }

    void Landscape::SetGameRenderMode()
    {
        throw std::logic_error("Not implemented");
    }

    void Landscape::CollectGrassCell(int, int, unsigned&, GrassInstance**, int*)
    {
        throw std::logic_error("Not implemented");
    }

    void Landscape::DrawGeom(dxGeom*)
    {
        throw std::logic_error("Not implemented");
    }

    void Landscape::Update()
    {
        throw std::logic_error("Not implemented");
    }

    void Landscape::DrawWaterLayer()
    {
        throw std::logic_error("Not implemented");
    }

    void Landscape::CreateHelperStructures()
    {
        throw std::logic_error("Not implemented");
    }

    void Landscape::drawSpriteOverlayed2Projected(float, float, float, float, unsigned, bool, CClipper const&)
    {
        throw std::logic_error("Not implemented");
    }

    Class* Landscape::GetBaseClass()
    {
        return RT_CLASS_LOCAL(SgNode);
    }

    void Landscape::PostServersLoad()
    {
        throw std::logic_error("Not implemented");
    }

    void Landscape::RemoveGrassTile(int, int)
    {
        throw std::logic_error("Not implemented");
    }

    void Landscape::DrawCells(std::vector<unsigned> const&, unsigned)
    {
        throw std::logic_error("Not implemented");
    }

    void Landscape::GetVisCellHeights(float&, float&, int, int) const
    {
        throw std::logic_error("Not implemented");
    }

    void Landscape::CreateIndicesTriLists(int*, int, int)
    {
        throw std::logic_error("Not implemented");
    }

    void Landscape::UpdateNodeCollisionGeoms(SgNode*)
    {
        throw std::logic_error("Not implemented");
    }

    void Landscape::ReleaseLod()
    {
        throw std::logic_error("Not implemented");
    }

    void Landscape::DrawMassBox(dMass*, CVector const&, Quaternion const&)
    {
        throw std::logic_error("Not implemented");
    }

    bool Landscape::InitGrass()
    {
        throw std::logic_error("Not implemented");
    }

    void Landscape::StartWaterQuery()
    {
        throw std::logic_error("Not implemented");
    }

    void Landscape::PutGrassToLandscape(CVector2 const&, CVector2 const&)
    {
        throw std::logic_error("Not implemented");
    }

    int Landscape::GetNumTiles() const
    {
        throw std::logic_error("Not implemented");
    }

    void Landscape::setDrawRadius(int, int, int)
    {
        throw std::logic_error("Not implemented");
    }

    void Landscape::GetDrawedCellHeights(float&, float&, int, int) const
    {
        throw std::logic_error("Not implemented");
    }

    unsigned Landscape::GetNearestGrassInstance(CVector const&) const
    {
        throw std::logic_error("Not implemented");
    }

    void Landscape::DoneGrass()
    {
        throw std::logic_error("Not implemented");
    }

    float Landscape::getCameraHeight(float, float) const
    {
        throw std::logic_error("Not implemented");
    }

    int Landscape::getGrassModelIdByName(char const*) const
    {
        throw std::logic_error("Not implemented");
    }

    int Landscape::SaveCameraMap(CStr const&, int)
    {
        throw std::logic_error("Not implemented");
    }

    void Landscape::Release()
    {
        throw std::logic_error("Not implemented");
    }

    void Landscape::EnableShoreRegion(int, int)
    {
        throw std::logic_error("Not implemented");
    }

    void Landscape::Render()
    {
        throw std::logic_error("Not implemented");
    }

    int Landscape::Render(SgNodeRenderFlags, void*, int, int)
    {
        throw std::logic_error("Not implemented");
    }

    void Landscape::ReadGrassFromXmlFile(char const*)
    {
        throw std::logic_error("Not implemented");
    }

    bool Landscape::WriteGrassToXmlFile(char const*)
    {
        throw std::logic_error("Not implemented");
    }

    CVector Landscape::getNormal(float, float)
    {
        throw std::logic_error("Not implemented");
    }

    unsigned char Landscape::GetColor(float, float)
    {
        throw std::logic_error("Not implemented");
    }

    void Landscape::SetPresenceOnCollisionMap(int, int)
    {
        throw std::logic_error("Not implemented");
    }

    bool Landscape::IsThisVisCellHasWater(int, int) const
    {
        throw std::logic_error("Not implemented");
    }

    void Landscape::drawSpriteOverlayed(unsigned, CVector const&, CVector const&, float)
    {
        throw std::logic_error("Not implemented");
    }

    void Landscape::ReloadLightmapTexture(CStr const&)
    {
        throw std::logic_error("Not implemented");
    }

    Object* Landscape::Clone()
    {
        throw std::logic_error("Not implemented");
    }

    bool Landscape::LoadShoreLine(CStr const&)
    {
        throw std::logic_error("Not implemented");
    }

    void Landscape::GetFogStartAndEnd(float&, float&) const
    {
        throw std::logic_error("Not implemented");
    }

    int Landscape::GetNumAlphas() const
    {
        throw std::logic_error("Not implemented");
    }

    void Landscape::CreateLod()
    {
        throw std::logic_error("Not implemented");
    }

    void Landscape::drawSpriteOverlayedProjected(unsigned, CVector const&, CMatrix const&, CClipper const&)
    {
        throw std::logic_error("Not implemented");
    }

    void Landscape::CreateHeights(CellParams*, int, int)
    {
        throw std::logic_error("Not implemented");
    }

    int Landscape::isWaterCell(int, int) const
    {
        throw std::logic_error("Not implemented");
    }

    void Landscape::Restore()
    {
        throw std::logic_error("Not implemented");
    }

    float Landscape::getWaterHeight(int, int) const
    {
        throw std::logic_error("Not implemented");
    }

    void Landscape::ReleaseReflectionRefractionTextures()
    {
        throw std::logic_error("Not implemented");
    }

    char const* Landscape::GetGrassModelName(unsigned) const
    {
        throw std::logic_error("Not implemented");
    }

    void Landscape::renderZGuard()
    {
        throw std::logic_error("Not implemented");
    }

    void Landscape::GetWaterCellHeights(float&, float&, int, int) const
    {
        throw std::logic_error("Not implemented");
    }

    void Landscape::ManageLandScapeCollisionTriMeshes()
    {
        throw std::logic_error("Not implemented");
    }

    void Landscape::LinkPassMapCellToCollisionCell(PointBase<int> const&)
    {
        throw std::logic_error("Not implemented");
    }

    void Landscape::ReBuildShoresVb()
    {
        throw std::logic_error("Not implemented");
    }

    int Landscape::LoadTiles(CStr const&)
    {
        throw std::logic_error("Not implemented");
    }

    bool Landscape::traceLineThruCellLs(float&, int, int, CVector const&, CVector const&, bool)
    {
        throw std::logic_error("Not implemented");
    }

    void Landscape::RecursiveDisableShore(unsigned char*, int, int)
    {
        throw std::logic_error("Not implemented");
    }

    Landscape::Landscape(Landscape const&)
    {
        throw std::logic_error("Not implemented");
    }

    int CreateIndices(unsigned short*, int, unsigned short, unsigned short)
    {
        throw std::logic_error("Not implemented");
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
        //this->m_numGrassModels = 0;
        //this->m_grassArray = 0;
        //this->m_heightMap = 0;
        //this->m_cliffHeightMap = 0;
        //this->m_waterMap = 0;
        //this->m_normalMap = 0;
        //this->m_vnormal = 0;
        //this->m_colormap = 0;
        //this->m_texSetsmap = 0;
        //this->m_remappedIndices = 0;
        //this->m_oCollisionitems = 0;
        //this->m_terrainObject = 0;
        //this->m_owner = 0;
        //this->m_cellParams = 0;
        //this->m_drawedCellParams = 0;
        //this->m_lastState = RT_LIGHTPASS;
        //this->m_tiles = 0;
        //m_landUVVb = m3d::Application::g_pApp->m_renderer->AddVb(rend::VERTEX_STREAM_UV_S1, 0xFFFF, "LandUV", 0);
        //m_landVb = m3d::Application::g_pApp->m_renderer->AddVb(rend::VERTEX_XYZNCT2, 1600, "LandEditor", 512);
        ////TODO: m_dummyVB =
        //for (auto& query : m_waterQueries)
        //{
        //    query = m3d::Application::g_pApp->m_renderer->NewQuery(rend::IQuery::QUERY_OCCLUSION);
        //}
        //this->m_isWaterVisible = 0;
        //this->m_currWaterQuery = 0;
        //this->m_drawtextured = 1;
        //this->m_passedCells = 0;
        //this->m_CurAlphaSet = 0;
        //this->m_loadAllTextures = 0;
        //m3d::g_Kernel->GetEngineCfg().m_console->RegisterCVar(&this->m_lockVis, 0);
        //this->m_maxLOD = 0;
        //int szindex = 4;
        //int i = 1;
        //do
        //{
        //    auto curIb = Application::g_pApp->m_renderer->AddIb(((szindex + 1) * (szindex + 1)) << 9, false);
        //    auto ptr = Application::g_pApp->m_renderer->LockIb(curIb, 384 * szindex * szindex, 0, 0);
        //    auto indices = CreateIndices(static_cast<unsigned short*>(ptr), szindex, 4, i);
        //    m_lsNumIndices.push_back(indices);
        //    Application::g_pApp->m_renderer->UnlockIb(curIb);
        //    m_landIbConst.push_back(curIb);
        //    i *= 2;
        //    ++m_maxLOD;
        //    szindex >>= 1;
        //} while (szindex);
        //m_maxWaterCellPerPass = m3d::Application::g_pApp->m_renderer->GetMaxVertexShaderConst();
        //m_waterVb = Application::g_pApp->m_renderer->AddVb(rend::VERTEX_WATERTEST, 81 * m_maxWaterCellPerPass, "Water", 0);
        //
        ////TODO: check this
        //auto waterVbPtr = static_cast<VertexWaterTest*>(Application::g_pApp->m_renderer->LockVb(this->m_waterVb, 0, 0, 0));
        //VertexWaterTest vertexWaterTest;
        //for (int j = 0; j < this->m_maxWaterCellPerPass; ++j)
        //{
        //    vertexWaterTest.z = j;
        //    auto v28 = *&vertexWaterTest.z;
        //    for (int k = 0; k <= 8u; ++k)
        //    {
        //        auto v30 = 0;
        //        vertexWaterTest.y = k;
        //        do
        //        {
        //            vertexWaterTest.x = v30;
        //            *&waterVbPtr->x = *&vertexWaterTest.x;
        //            *&waterVbPtr->z = v28;
        //            ++v30;
        //            ++waterVbPtr;
        //        } while (v30 <= 8u);
        //    }
        //}
        //m3d::Application::g_pApp->m_renderer->UnlockVb(this->m_waterVb);
        //waterTileInfo = new CVector4[m_maxWaterCellPerPass];
        //i = 0;
        //do
        //{
        //    m_waterIb
        //} while ();
            //TODO: IMPORTANT! IMPLEMENT LANDSCAPE
    }

    void Landscape::BuildCells0(rend::VertexLandscape*, TIVChunk&, int&, std::vector<unsigned> const&, RenderTypes, std::vector<int, std::allocator<int>>&)
    {
        throw std::logic_error("Not implemented");
    }

    void Landscape::DrawCellsFast0(std::vector<unsigned> const&, TIVChunk&, RenderTypes)
    {
        throw std::logic_error("Not implemented");
    }

    void Landscape::RecursiveEnableShore(unsigned char*, int, int)
    {
        throw std::logic_error("Not implemented");
    }

    bool Landscape::traceLineThruCellLs0(float&, int, int, CVector const&, CVector const&)
    {
        throw std::logic_error("Not implemented");
    }

    void Landscape::DrawCells0(std::vector<unsigned> const&, RenderTypes)
    {
        throw std::logic_error("Not implemented");
    }

    int Landscape::IsBackfaced(int, int, rend::Cull)
    {
        throw std::logic_error("Not implemented");
    }

    void Landscape::FreeTiles()
    {
        throw std::logic_error("Not implemented");
    }

    void Landscape::BuildUVSet()
    {
        throw std::logic_error("Not implemented");
    }

    Landscape::VisibilityMode Landscape::GetCurVisMode() const
    {
        throw std::logic_error("Not implemented");
    }

    void Landscape::SetCurVisMode(VisibilityMode)
    {
        throw std::logic_error("Not implemented");
    }

    void Landscape::DrawNonTransformGeom(dxGeom*)
    {
        throw std::logic_error("Not implemented");
    }

    void Landscape::RenderRoads()
    {
        throw std::logic_error("Not implemented");
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
