#pragma once
#include <map>
#include <vector>
#include <core/stringm3d.h>
#include <math/vector.h>

namespace m3d
{
    namespace cmn
    {
        class XmlNode;
        class XmlFile;
    }
}

class NavPoint
{
public:
    enum NavPointType
    {
        NAVPOINT_TYPE_MAIN_QUEST = 0x0,
        NAVPOINT_TYPE_USER_QUEST = 0x1,
        NAVPOINT_TYPE_USER_LOCATION = 0x2,
        NAVPOINT_TYPE_NUM_NAVPOINT_TYPES = 0x3,
        NAVPOINT_TYPE_INVALID = 0x3,
    };

    enum ObjectType
    {
        OBJECT_TYPE_STATIC_QUEST = 0x0,
        OBJECT_TYPE_DYNAMIC_QUEST = 0x1,
        OBJECT_TYPE_NUM_OBJECT_TYPES = 0x2,
        OBJECT_TYPE_INVALID = 0x2,
    };

public:
    CStr const& GetLevelName() const;
    CVector const* GetCoordinate() const;
    ObjectType GetObjectType() const;
    NavPointType GetNavPointType() const;
    bool IsValid() const;
    int SaveToXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const;
    int GetId() const;
    int LoadFromXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);
    int GetObjectId() const;

protected:
    NavPoint();
    ~NavPoint();
    void Invalidate();

private:
    NavPointType m_navPointType;
    ObjectType m_objectType;
    int m_objectId;
    int m_id;
    CStr m_levelName;
    CVector m_coordinate;
};

class NavPointManager
{
public:
    std::vector<int> GetNavPointsByType(CStr const&, NavPoint::NavPointType) const;
    int Init();
    int LoadFromXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);
    int GetMaxNavPointsNumByType(NavPoint::NavPointType) const;
    NavPoint const* GetNavPointByObjectId(CStr const&, int, NavPoint::ObjectType) const;
    int RemoveNavPointById(int);
    void Clear();
    bool CanNavPointOfTypeBeAdded(CStr const&, NavPoint::NavPointType, bool) const;
    NavPointManager();
    NavPoint const* GetNavPointById(int) const;
    std::vector<int> GetAllNavPointsByQuestId(int) const;
    int AddNavPointUserLocation(CStr const&, CVector const&);
    int AddNavPointObjectDependend(CStr const&, NavPoint::NavPointType, NavPoint::ObjectType, int, bool);
    std::vector<int> GetNavPointsForLevel(CStr const&) const;
    int SaveToXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const;
    int GameDataUpdate(void*, int);
    ~NavPointManager();
    bool CanNavPointBeAdded(CStr const&, int, NavPoint::ObjectType, NavPoint::NavPointType, bool) const;

private:
    int UpdateOnQuestStateChanged(void*);
    int MakeNavPointOnFirstFitDynamicQuest();
    int UpdateOnQuestTaken(void*);
    int UpdateOnDynamicQuestStateChanged(void*);
    int MakeNavPointOnFirstFitStaticQuest(CStr const&, bool);
    int AddNavPoint(NavPoint*);

private:
    std::map<int, NavPoint*> m_navPoints;
    int m_nextNavPointId = 0;
};
