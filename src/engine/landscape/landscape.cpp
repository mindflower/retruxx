#include "roads/road.h"
#include <geomobject.h>
#include <landscape.h>
#include <stdexcept>
#include <core/kernel.h>

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

    void Landscape::setOwner(CWorld*)
    {
        throw std::logic_error("Not implemented");
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
        throw std::logic_error("Not implemented");
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

    Landscape::Landscape()
    {
        throw std::logic_error("Not implemented");
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
