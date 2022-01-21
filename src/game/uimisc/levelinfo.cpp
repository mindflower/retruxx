#include "levelinfo.h"
#include <stdexcept>

RT_CLASS_DEFINE(LevelInfoManager);

void LevelInfoManager::ClearBeforeNewLevel()
{
    throw std::logic_error("Not implemented");
}

void LevelInfoManager::GetVisitedLevelNames(std::vector<CStr>&) const
{
    throw std::logic_error("Not implemented");
}

void LevelInfoManager::ClearLevelObjects()
{
    throw std::logic_error("Not implemented");
}

void LevelInfoManager::GetAllLevelNames(std::vector<CStr>&) const
{
    throw std::logic_error("Not implemented");
}

int LevelInfoManager::AddVisibilityCircleForLevel(CStr const&, CVector const&, float)
{
    throw std::logic_error("Not implemented");
}

void LevelInfoManager::OnTownRuined(void*)
{
    throw std::logic_error("Not implemented");
}

void LevelInfoManager::UpdateKnownLevels()
{
    throw std::logic_error("Not implemented");
}

m3d::Object* LevelInfoManager::Clone()
{
    throw std::logic_error("Not implemented");
}

void LevelInfoManager::UpdateLevelImages()
{
    throw std::logic_error("Not implemented");
}

void LevelInfoManager::UpdateObjectInfoForCurrentLevel()
{
    throw std::logic_error("Not implemented");
}

void LevelInfoManager::UpdateObjectPositions()
{
    throw std::logic_error("Not implemented");
}

void LevelInfoManager::OnEndLevel()
{
    throw std::logic_error("Not implemented");
}

int LevelInfoManager::AddVisibilityRectForLevel(CStr const&, float, float, float, float)
{
    throw std::logic_error("Not implemented");
}

LevelInfo* LevelInfoManager::GetLevelInfoById(int) const
{
    throw std::logic_error("Not implemented");
}

int LevelInfoManager::SaveObjectInfoForObject(ai::Obj const*)
{
    throw std::logic_error("Not implemented");
}

m3d::Object* LevelInfoManager::CreateObject()
{
    throw std::logic_error("Not implemented");
}

void LevelInfoManager::ClearVisibilityMapForLevel(CStr const&)
{
    throw std::logic_error("Not implemented");
}

VisibilityMap* LevelInfoManager::GetVisibilityMapForLevel(CStr const&) const
{
    throw std::logic_error("Not implemented");
}

bool LevelInfoManager::IsObjectCharted(ai::Obj const*) const
{
    throw std::logic_error("Not implemented");
}

VisibilityMap* LevelInfoManager::AddVisibilityMapForLevel(CStr const&)
{
    throw std::logic_error("Not implemented");
}

m3d::Class* LevelInfoManager::GetBaseClass()
{
    throw std::logic_error("Not implemented");
}

void LevelInfoManager::ClearNonPersistantPriceInfoForLevel(CStr const&)
{
    throw std::logic_error("Not implemented");
}

void LevelInfoManager::ClearOnce()
{
    throw std::logic_error("Not implemented");
}

int LevelInfoManager::LoadFromXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
{
    throw std::logic_error("Not implemented");
}

void LevelInfoManager::ClearObjectIds(CStr const&)
{
    throw std::logic_error("Not implemented");
}

bool LevelInfoManager::IsLevelKnown(CStr const&) const
{
    throw std::logic_error("Not implemented");
}

int LevelInfoManager::Init()
{
    throw std::logic_error("Not implemented");
}

int LevelInfoManager::GetVisibilityRadius() const
{
    throw std::logic_error("Not implemented");
}

void LevelInfoManager::OnNewFrame()
{
    throw std::logic_error("Not implemented");
}

int LevelInfoManager::AddVisitedLevel(CStr const&)
{
    throw std::logic_error("Not implemented");
}

std::map<CStr, ObjectInfo*> const* LevelInfoManager::GetObjectsForLevel(CStr const&) const
{
    throw std::logic_error("Not implemented");
}

std::map<CStr, ObjectInfo*>* LevelInfoManager::GetObjectsForLevel(CStr const&)
{
    throw std::logic_error("Not implemented");
}

int LevelInfoManager::LoadLevelSizes()
{
    throw std::logic_error("Not implemented");
}

void LevelInfoManager::ClearBeforeContinuousLevel()
{
    throw std::logic_error("Not implemented");
}

void LevelInfoManager::ClearVisitedLevels()
{
    throw std::logic_error("Not implemented");
}

LevelInfoManager::~LevelInfoManager()
{
    throw std::logic_error("Not implemented");
}

void LevelInfoManager::OnEndLevelBeforeContinuousLevel()
{
    throw std::logic_error("Not implemented");
}

int LevelInfoManager::GameDataUpdate(void*, int)
{
    throw std::logic_error("Not implemented");
}

int LevelInfoManager::LoadLevelInfoFromXml(CStr const&)
{
    throw std::logic_error("Not implemented");
}

void LevelInfoManager::OnStartLevel()
{
    throw std::logic_error("Not implemented");
}

LevelInfo* LevelInfoManager::GetLevelInfoByName(CStr const&)
{
    throw std::logic_error("Not implemented");
}

LevelInfo const* LevelInfoManager::GetLevelInfoByName(CStr const&) const
{
    throw std::logic_error("Not implemented");
}

int LevelInfoManager::AddObjectInfo(ObjectInfo*)
{
    throw std::logic_error("Not implemented");
}

void LevelInfoManager::OnLocationStateChanged(void*)
{
    throw std::logic_error("Not implemented");
}

int LevelInfoManager::LoadLevelSizeFromXml(CStr const&)
{
    throw std::logic_error("Not implemented");
}

ObjectInfo* LevelInfoManager::GetObjectInfo(CStr const&, CStr const&) const
{
    throw std::logic_error("Not implemented");
}

int LevelInfoManager::AddKnownLevel(CStr const&)
{
    throw std::logic_error("Not implemented");
}

int LevelInfoManager::GetLevelInfoId(CStr const&) const
{
    throw std::logic_error("Not implemented");
}

void LevelInfoManager::ClearKnownLevels()
{
    throw std::logic_error("Not implemented");
}

void LevelInfoManager::GetKnownLevelNames(std::vector<CStr>&) const
{
    throw std::logic_error("Not implemented");
}

CStr LevelInfoManager::GetLevelInfoName(int) const
{
    throw std::logic_error("Not implemented");
}

int LevelInfoManager::SaveToXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
{
    throw std::logic_error("Not implemented");
}

void LevelInfoManager::UpdateVisibilityMaps()
{
    throw std::logic_error("Not implemented");
}

m3d::Class* LevelInfoManager::GetClass() const
{
    throw std::logic_error("Not implemented");
}

float LevelInfoManager::GetLevelSize(CStr const&) const
{
    throw std::logic_error("Not implemented");
}

void LevelInfoManager::ClearVisibilityMaps()
{
    throw std::logic_error("Not implemented");
}

bool LevelInfoManager::IsLevelVisited(CStr const&) const
{
    throw std::logic_error("Not implemented");
}

LevelInfoManager::LevelInfoManager()
{
    throw std::logic_error("Not implemented");
}

LevelInfoManager::LevelInfoManager(LevelInfoManager const&)
{
    throw std::logic_error("Not implemented");
}
