#include "navpoint.h"
#include <stdexcept>

std::vector<int> NavPointManager::GetNavPointsByType(CStr const&, NavPoint::NavPointType) const
{
    throw std::logic_error("Not implemented");
}

int NavPointManager::Init()
{
    throw std::logic_error("Not implemented");
}

int NavPointManager::LoadFromXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
{
    throw std::logic_error("Not implemented");
}

int NavPointManager::GetMaxNavPointsNumByType(NavPoint::NavPointType) const
{
    throw std::logic_error("Not implemented");
}

NavPoint const* NavPointManager::GetNavPointByObjectId(CStr const&, int, NavPoint::ObjectType) const
{
    throw std::logic_error("Not implemented");
}

int NavPointManager::RemoveNavPointById(int)
{
    throw std::logic_error("Not implemented");
}

void NavPointManager::Clear()
{
    throw std::logic_error("Not implemented");
}

bool NavPointManager::CanNavPointOfTypeBeAdded(CStr const&, NavPoint::NavPointType, bool) const
{
    throw std::logic_error("Not implemented");
}

NavPointManager::NavPointManager()
{
}

NavPoint const* NavPointManager::GetNavPointById(int) const
{
    throw std::logic_error("Not implemented");
}

std::vector<int> NavPointManager::GetAllNavPointsByQuestId(int) const
{
    throw std::logic_error("Not implemented");
}

int NavPointManager::AddNavPointUserLocation(CStr const&, CVector const&)
{
    throw std::logic_error("Not implemented");
}

int NavPointManager::AddNavPointObjectDependend(CStr const&, NavPoint::NavPointType, NavPoint::ObjectType, int, bool)
{
    throw std::logic_error("Not implemented");
}

std::vector<int> NavPointManager::GetNavPointsForLevel(CStr const&) const
{
    throw std::logic_error("Not implemented");
}

int NavPointManager::SaveToXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
{
    throw std::logic_error("Not implemented");
}

int NavPointManager::GameDataUpdate(void*, int)
{
    throw std::logic_error("Not implemented");
}

NavPointManager::~NavPointManager()
{
    throw std::logic_error("Not implemented");
}

bool NavPointManager::CanNavPointBeAdded(CStr const&, int, NavPoint::ObjectType, NavPoint::NavPointType, bool) const
{
    throw std::logic_error("Not implemented");
}

int NavPointManager::UpdateOnQuestStateChanged(void*)
{
    throw std::logic_error("Not implemented");
}

int NavPointManager::MakeNavPointOnFirstFitDynamicQuest()
{
    throw std::logic_error("Not implemented");
}

int NavPointManager::UpdateOnQuestTaken(void*)
{
    throw std::logic_error("Not implemented");
}

int NavPointManager::UpdateOnDynamicQuestStateChanged(void*)
{
    throw std::logic_error("Not implemented");
}

int NavPointManager::MakeNavPointOnFirstFitStaticQuest(CStr const&, bool)
{
    throw std::logic_error("Not implemented");
}

int NavPointManager::AddNavPoint(NavPoint*)
{
    throw std::logic_error("Not implemented");
}
