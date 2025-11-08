#include "navpoint.h"
#include <stdexcept>

#include "retruxx/common.h"

std::vector<int> NavPointManager::GetNavPointsByType(CStr const&, NavPoint::NavPointType) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int NavPointManager::Init()
{
    Clear();
    return 1;
}

int NavPointManager::LoadFromXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int NavPointManager::GetMaxNavPointsNumByType(NavPoint::NavPointType) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

NavPoint const* NavPointManager::GetNavPointByObjectId(CStr const&, int, NavPoint::ObjectType) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int NavPointManager::RemoveNavPointById(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void NavPointManager::Clear()
{
    for (auto& point : m_navPoints)
    {
        RemoveNavPointById(point.first);
    }
    m_navPoints.clear();
    m_nextNavPointId = 0;
}

bool NavPointManager::CanNavPointOfTypeBeAdded(CStr const&, NavPoint::NavPointType, bool) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

NavPointManager::NavPointManager()
{
}

NavPoint const* NavPointManager::GetNavPointById(int) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

std::vector<int> NavPointManager::GetAllNavPointsByQuestId(int) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int NavPointManager::AddNavPointUserLocation(CStr const&, CVector const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int NavPointManager::AddNavPointObjectDependend(CStr const&, NavPoint::NavPointType, NavPoint::ObjectType, int, bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

std::vector<int> NavPointManager::GetNavPointsForLevel(CStr const&) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int NavPointManager::SaveToXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int NavPointManager::GameDataUpdate(void*, int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

NavPointManager::~NavPointManager()
{
    RETRUXX_NOT_IMPLEMENTED;
}

bool NavPointManager::CanNavPointBeAdded(CStr const&, int, NavPoint::ObjectType, NavPoint::NavPointType, bool) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int NavPointManager::UpdateOnQuestStateChanged(void*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int NavPointManager::MakeNavPointOnFirstFitDynamicQuest()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int NavPointManager::UpdateOnQuestTaken(void*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int NavPointManager::UpdateOnDynamicQuestStateChanged(void*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int NavPointManager::MakeNavPointOnFirstFitStaticQuest(CStr const&, bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int NavPointManager::AddNavPoint(NavPoint*)
{
    RETRUXX_NOT_IMPLEMENTED;
}
