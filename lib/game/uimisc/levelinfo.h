#pragma once
#include <core/clazz.h>
#include <core/console/cvar.h>
#include <math/vector.h>
#include <renderer/i_renderer.h>

namespace ai
{
    class Obj;
}

class ObjectInfo;
class VisibilityMap;

class LevelInfo
{
    friend class LevelInfoManager;

public:
    LevelInfo(LevelInfo const&);
    LevelInfo();
    ~LevelInfo();
    int LoadFromXml(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode);
    CStr const& GetName() const;
    CStr const& GetFullName() const;
    CStr const& GetFile() const;
    CStr const& GetDiz0() const;
    CStr const& GetDiz1() const;
    m3d::rend::TexHandle GetImage0() const;
    m3d::rend::TexHandle GetImage1() const;
    retruxx::vector<m3d::rend::TexHandle> GetSplashes();
    CStr GetRandomMusicBlock() const;
    CVector const& GetNorth() const;

    using TexMap = retruxx::map<CStr, m3d::rend::TexHandle>;
    using TexPair = retruxx::pair<CStr, m3d::rend::TexHandle>;

private:
    int LoadBigImage();
    CStr m_name;
    CStr m_fullName;
    CStr m_file;
    CStr m_diz0;
    CStr m_diz1;
    CStr m_imageFile0;
    CStr m_imageFile1;
    retruxx::vector<CStr> m_splasheNames;
    retruxx::vector<CStr> m_musicBlockNames;
    m3d::rend::TexHandle m_image0;
    m3d::rend::TexHandle m_image1;
    retruxx::map<CStr, m3d::rend::TexHandle> m_splashes;
    CVector m_north;
};

class LevelInfoManager : public m3d::Object
{
public:
    using CellsVector = retruxx::vector<int>;
    using ObjectInfoMap = retruxx::map<CStr, ObjectInfo*>;
    using ObjectInfoPair = retruxx::pair<CStr, ObjectInfo*>;
    class ObjectInfoConstIterator;

public:
    int Init();
    int GameDataUpdate(void* data, int dataType);
    LevelInfo* GetLevelInfoByName(CStr const& levelName);
    LevelInfo const* GetLevelInfoByName(CStr const& levelName) const;
    void GetAllLevelNames(retruxx::vector<CStr, retruxx::allocator<CStr>>& allLevelNames) const;
    void GetVisitedLevelNames(retruxx::vector<CStr, retruxx::allocator<CStr>>& visitedLevelNames) const;
    void GetKnownLevelNames(retruxx::vector<CStr, retruxx::allocator<CStr>>& knownLevelNames) const;
    bool IsLevelKnown(CStr const& levelName) const;
    bool IsLevelVisited(CStr const& levelName) const;
    float GetLevelSize(CStr const& levelName) const;
    retruxx::map<CStr, ObjectInfo*, retruxx::less<CStr>, retruxx::allocator<retruxx::pair<CStr const, ObjectInfo*>>>*
        GetObjectsForLevel(CStr const& levelName);
    retruxx::
        map<CStr, ObjectInfo*, retruxx::less<CStr>, retruxx::allocator<retruxx::pair<CStr const, ObjectInfo*>>> const*
        GetObjectsForLevel(CStr const& levelName) const;
    ObjectInfo* GetObjectInfo(CStr const& objectName, CStr const& levelName) const;
    VisibilityMap* GetVisibilityMapForLevel(CStr const& levelName) const;
    int GetVisibilityRadius() const;
    int AddKnownLevel(CStr const& levelName);
    int AddVisibilityCircleForLevel(CStr const& levelName, CVector const& origin, float radius);
    int AddVisibilityRectForLevel(CStr const& levelName, float x0, float y0, float w, float h);
    void ClearVisibilityMapForLevel(CStr const& levelName);
    int LoadFromXml(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode);
    int SaveToXml(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const;

protected:
    LevelInfoManager();
    LevelInfoManager(LevelInfoManager const& rhs);

public:
    virtual ~LevelInfoManager() override;
    virtual m3d::Object* Clone() override;
    static m3d::Object* CreateObject();
    static m3d::Class* GetBaseClass();
    virtual m3d::Class* GetClass() const override;
    static m3d::Class m_classLevelInfoManager;

    using LevelInfoMap =
        retruxx::map<int, LevelInfo*, retruxx::less<int>, retruxx::allocator<retruxx::pair<int const, LevelInfo*>>>;
    using LevelInfoPair = retruxx::pair<int, LevelInfo*>;
    using LevelSizeMap =
        retruxx::map<CStr, float, retruxx::less<CStr>, retruxx::allocator<retruxx::pair<CStr const, float>>>;
    using LevelSizePair = retruxx::pair<CStr, float>;
    class LevelSizeMapConstIterator;
    using LevelObjectsMap = retruxx::map<
        CStr,
        retruxx::
            map<CStr, ObjectInfo*, retruxx::less<CStr>, retruxx::allocator<retruxx::pair<CStr const, ObjectInfo*>>>,
        retruxx::less<CStr>,
        retruxx::allocator<retruxx::pair<
            CStr const,
            retruxx::map<
                CStr,
                ObjectInfo*,
                retruxx::less<CStr>,
                retruxx::allocator<retruxx::pair<CStr const, ObjectInfo*>>>>>>;
    using LevelObjectsPair = retruxx::pair<
        CStr,
        retruxx::
            map<CStr, ObjectInfo*, retruxx::less<CStr>, retruxx::allocator<retruxx::pair<CStr const, ObjectInfo*>>>>;
    class LevelObjectsConstIterator;
    using VisibilityMapMap = retruxx::
        map<CStr, VisibilityMap*, retruxx::less<CStr>, retruxx::allocator<retruxx::pair<CStr const, VisibilityMap*>>>;
    using VisibilityMapPair = retruxx::pair<CStr, VisibilityMap*>;

public:
    void ClearBeforeNewLevel();
    void ClearBeforeContinuousLevel();
    void ClearOnce();
    void ClearLevelObjects();
    void ClearNonPersistantPriceInfoForLevel(CStr const& levelName);
    void ClearObjectIds(CStr const& levelName);
    void ClearKnownLevels();
    void ClearVisitedLevels();
    void ClearVisibilityMaps();
    int LoadLevelInfoFromXml(CStr const& fileName);
    int LoadLevelSizes();
    int LoadLevelSizeFromXml(CStr const& fileName);
    VisibilityMap* AddVisibilityMapForLevel(CStr const& levelName);
    int AddVisitedLevel(CStr const& levelName);
    void UpdateLevelImages();
    LevelInfo* GetLevelInfoById(int id) const;
    int GetLevelInfoId(CStr const& name) const;
    CStr GetLevelInfoName(int id) const;
    void OnStartLevel();
    void OnEndLevel();
    void OnEndLevelBeforeContinuousLevel();
    bool IsObjectCharted(ai::Obj const* o) const;
    void OnNewFrame();
    void UpdateVisibilityMaps();
    void UpdateObjectPositions();
    void OnLocationStateChanged(void* data);
    void OnTownRuined(void* data);
    int SaveObjectInfoForObject(ai::Obj const* o);
    void UpdateObjectInfoForCurrentLevel();
    void UpdateKnownLevels();
    int AddObjectInfo(ObjectInfo* oi);

    retruxx::map<int, LevelInfo*> m_levels;
    int m_nextLevelInfoId;
    retruxx::map<CStr, float> m_levelSizes;
    retruxx::map<CStr, retruxx::map<CStr, ObjectInfo*>> m_levelObjects;
    retruxx::map<CStr, VisibilityMap*> m_visibilityMaps;
    retruxx::set<CStr> m_knownLevels;
    retruxx::set<CStr> m_visitedLevels;
    m3d::CVar m_cvVisibilityRadius;
};

class ObjectInfo
{
    friend class LevelInfoManager;

public:
    ObjectInfo(ObjectInfo const&);
    ObjectInfo();
    ~ObjectInfo();
    int GetId() const;
    CStr const& GetName() const;
    CStr const& GetFullName() const;
    CStr const& GetPrototypeName() const;
    int GetPrototypeId() const;
    CVector const& GetPosition() const;
    CStr const& GetLevelName() const;
    int GetBelong() const;
    CStr const* GetPassageAddress() const;
    retruxx::map<int, CVector2, retruxx::less<int>, retruxx::allocator<retruxx::pair<int const, CVector2>>> const*
        GetSavedPrices() const;
    int SavePrices();
    int AddSavedPriceByPrototypeId(int prototypeId, int sellPrice, int buyPrice);
    void DeleteSavedPrices();
    void SetSavedPricesPersistant(bool bPersistant);
    bool IsSavedPricesPersistant() const;
    bool IsRuined() const;
    int LoadFromXml(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode);
    int SaveToXml(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const;
    bool IsTown() const;
    bool IsLocation() const;
    bool IsActive() const;

private:
    m3d::Class const* GetObjectClass() const;

    CStr m_name;
    CStr m_fullName;
    CStr m_prototypeName;
    int m_prototypeId;
    CVector m_position;
    CStr m_levelName;
    retruxx::map<int, CVector2, retruxx::less<int>, retruxx::allocator<retruxx::pair<int const, CVector2>>>
        m_savedPrices;
    bool m_bSavedPricesPersistant;
    bool m_bRuined;
    CStr m_passageAddress;
    bool m_bIsActive;
    int m_objectId;
    int m_belong;
};
