#pragma once
#include <map>
#include <vector>
#include <core/stringm3d.h>
#include <math/vector.h>

namespace m3d
{
    namespace cmn
    {
        struct XmlNode;
        class XmlFile;
    }  // namespace cmn
}  // namespace m3d

class NavPoint
{
    friend class NavPointManager;

public:
    enum NavPointType
    {
        NAVPOINT_TYPE_MAIN_QUEST = 0,
        NAVPOINT_TYPE_USER_QUEST = 1,
        NAVPOINT_TYPE_USER_LOCATION = 2,
        NAVPOINT_TYPE_NUM_NAVPOINT_TYPES = 3,
        NAVPOINT_TYPE_INVALID = 3,
    };

    enum ObjectType
    {
        OBJECT_TYPE_STATIC_QUEST = 0,
        OBJECT_TYPE_DYNAMIC_QUEST = 1,
        OBJECT_TYPE_NUM_OBJECT_TYPES = 2,
        OBJECT_TYPE_INVALID = 2,
    };

public:
    NavPoint::NavPointType GetNavPointType() const;
    NavPoint::ObjectType GetObjectType() const;
    int GetObjectId() const;
    int GetId() const;
    CStr const& GetLevelName() const;
    CVector const* GetCoordinate() const;
    bool IsValid() const;
    int LoadFromXml(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode);
    int SaveToXml(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const;

protected:
    NavPoint(NavPoint const&);
    NavPoint();
    ~NavPoint();
    void Invalidate();

    /* 0x0000 */ NavPoint::NavPointType m_navPointType;
    /* 0x0004 */ NavPoint::ObjectType m_objectType;
    /* 0x0008 */ int m_objectId;
    /* 0x000c */ int m_id;
    /* 0x0010 */ CStr m_levelName;
    /* 0x001c */ CVector m_coordinate;
}; /* size: 0x0028 */

class NavPointManager
{
    using NavPointIdVector = std::vector<int, std::allocator<int>>;

public:
    NavPointManager(NavPointManager const&);
    NavPointManager();
    ~NavPointManager();
    std::vector<int, std::allocator<int>> GetNavPointsForLevel(CStr const& levelName) const;
    std::vector<int, std::allocator<int>> GetNavPointsByType(CStr const& levelName, NavPoint::NavPointType npType)
        const;
    NavPoint const* GetNavPointById(int id) const;
    NavPoint const* GetNavPointByObjectId(CStr const& levelName, int objectId, NavPoint::ObjectType objectType) const;
    std::vector<int, std::allocator<int>> GetAllNavPointsByQuestId(int questId) const;
    int AddNavPointObjectDependend(
        CStr const& levelName,
        NavPoint::NavPointType npType,
        NavPoint::ObjectType objectType,
        int objectId,
        bool bByUser);
    int AddNavPointUserLocation(CStr const& levelName, CVector const& coordinate);
    int RemoveNavPointById(int id);
    int GetMaxNavPointsNumByType(NavPoint::NavPointType npType) const;
    bool CanNavPointBeAdded(
        CStr const& levelName,
        int objectId,
        NavPoint::ObjectType objectType,
        NavPoint::NavPointType npType,
        bool bByUser) const;
    bool CanNavPointOfTypeBeAdded(CStr const& levelName, NavPoint::NavPointType npType, bool bByUser) const;
    int Init();
    void Clear();
    int LoadFromXml(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode);
    int SaveToXml(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const;
    int GameDataUpdate(void* data, int dataType);

    using NavPointMap = std::map<int, NavPoint*, std::less<int>, std::allocator<std::pair<int const, NavPoint*>>>;
    using NavPointPair = std::pair<int, NavPoint*>;

private:
    int AddNavPoint(NavPoint* np);
    int UpdateOnQuestTaken(void* data);
    int UpdateOnQuestStateChanged(void* data);
    int UpdateOnDynamicQuestStateChanged(void* data);
    int MakeNavPointOnFirstFitStaticQuest(CStr const& levelName, bool bIsMainQuest);
    int MakeNavPointOnFirstFitDynamicQuest();
    /* 0x0000 */ std::map<int, NavPoint*, std::less<int>, std::allocator<std::pair<int const, NavPoint*>>> m_navPoints;
    /* 0x000c */ int m_nextNavPointId = 0;
}; /* size: 0x0010 */
