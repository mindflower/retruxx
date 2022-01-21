#pragma once
#include <map>
#include <vector>
#include <core/clazz.h>
#include <core/console/cvar.h>
#include <math/vector.h>

namespace ai
{
    class Obj;
}

class ObjectInfo;
class VisibilityMap;

class LevelInfoManager :  public m3d::Object
{
public:
    void ClearBeforeNewLevel();
    void GetVisitedLevelNames(std::vector<CStr> &) const ;
    void ClearLevelObjects();
    void GetAllLevelNames(std::vector<CStr> &) const ;
    int AddVisibilityCircleForLevel(CStr const &,CVector const &,float);
    void OnTownRuined(void *);
    void UpdateKnownLevels();
    virtual m3d::Object * Clone();
    void UpdateLevelImages();
    void UpdateObjectInfoForCurrentLevel();
    void UpdateObjectPositions();
    void OnEndLevel();
    int AddVisibilityRectForLevel(CStr const &,float,float,float,float);
    class LevelInfo * GetLevelInfoById(int) const ;
    int SaveObjectInfoForObject(ai::Obj const *);
    static m3d::Object * CreateObject();
    void ClearVisibilityMapForLevel(CStr const &);
    VisibilityMap * GetVisibilityMapForLevel(CStr const &) const ;
    bool IsObjectCharted(ai::Obj const *) const ;
    VisibilityMap * AddVisibilityMapForLevel(CStr const &);
    static m3d::Class * GetBaseClass();
    void ClearNonPersistantPriceInfoForLevel(CStr const &);
    void ClearOnce();
    int LoadFromXml(m3d::cmn::XmlFile *,m3d::cmn::XmlNode const *);
    void ClearObjectIds(CStr const &);
    bool IsLevelKnown(CStr const &) const ;
    int Init();
    int GetVisibilityRadius() const ;
    void OnNewFrame();
    int AddVisitedLevel(CStr const &);
    std::map<CStr,ObjectInfo *> const * GetObjectsForLevel(CStr const &) const ;
    std::map<CStr,ObjectInfo *> * GetObjectsForLevel(CStr const &);
    int LoadLevelSizes();
    void ClearBeforeContinuousLevel();
    void ClearVisitedLevels();
    virtual ~LevelInfoManager();
    void OnEndLevelBeforeContinuousLevel();
    int GameDataUpdate(void *,int);
    int LoadLevelInfoFromXml(CStr const &);
    void OnStartLevel();
    LevelInfo * GetLevelInfoByName(CStr const &);
    LevelInfo const * GetLevelInfoByName(CStr const &) const ;
    int AddObjectInfo(ObjectInfo *);
    void OnLocationStateChanged(void *);
    int LoadLevelSizeFromXml(CStr const &);
    class ObjectInfo * GetObjectInfo(CStr const &,CStr const &) const ;
    int AddKnownLevel(CStr const &);
    int GetLevelInfoId(CStr const &) const ;
    void ClearKnownLevels();
    void GetKnownLevelNames(std::vector<CStr> &) const ;
    CStr GetLevelInfoName(int) const ;
    int SaveToXml(m3d::cmn::XmlFile *,m3d::cmn::XmlNode *) const ;
    void UpdateVisibilityMaps();
    virtual m3d::Class * GetClass() const ;
    float GetLevelSize(CStr const &) const ;
    void ClearVisibilityMaps();
    bool IsLevelVisited(CStr const &) const ;

protected:
    LevelInfoManager();
    LevelInfoManager(LevelInfoManager const &);

private:
    std::map<int,LevelInfo *> m_levels;
    int m_nextLevelInfoId;
    std::map<CStr,float> m_levelSizes;
    std::map<CStr,std::map<CStr,ObjectInfo *>> m_levelObjects;
    std::map<CStr,VisibilityMap *> m_visibilityMaps;
    std::set<CStr> m_knownLevels;
    std::set<CStr> m_visitedLevels;
    m3d::CVar m_cvVisibilityRadius;
};

class ObjectInfo
{
public:
    ObjectInfo();
    bool IsSavedPricesPersistant() const ;
    int SavePrices();
    int GetId() const ;
    int GetBelong() const ;
    int SaveToXml(m3d::cmn::XmlFile *,m3d::cmn::XmlNode *) const ;
    void SetSavedPricesPersistant(bool);
    CStr const * GetPassageAddress() const ;
    int GetPrototypeId() const ;
    bool IsTown() const ;
    int LoadFromXml(m3d::cmn::XmlFile *,m3d::cmn::XmlNode const *);
    bool IsLocation() const ;
    ~ObjectInfo();
    CStr const & GetLevelName() const ;
    CStr const & GetName() const ;
    int AddSavedPriceByPrototypeId(int,int,int);
    std::map<int,CVector2> const * GetSavedPrices() const ;
    CStr const & GetPrototypeName() const ;
    CVector const & GetPosition() const ;
    bool IsActive() const ;
    bool IsRuined() const ;
    void DeleteSavedPrices();
    CStr const & GetFullName() const ;

private:
    m3d::Class const * GetObjectClass() const ;
    CStr m_name;
    CStr m_fullName;
    CStr m_prototypeName;
    int m_prototypeId;
    CVector m_position;
    CStr m_levelName;
    std::map<int,CVector2> m_savedPrices;
    bool m_bSavedPricesPersistant;
    bool m_bRuined;
    CStr m_passageAddress;
    bool m_bIsActive;
    int m_objectId;
    int m_belong;
};
