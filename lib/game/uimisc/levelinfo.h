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
    LevelInfo(const LevelInfo&);
    LevelInfo();
    ~LevelInfo();
    int LoadFromXml(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode);
    const CStr& GetName() const;
    const CStr& GetFullName() const;
    const CStr& GetFile() const;
    const CStr& GetDiz0() const;
    const CStr& GetDiz1() const;
    m3d::rend::TexHandle GetImage0() const;
    m3d::rend::TexHandle GetImage1() const;
    retruxx::vector<m3d::rend::TexHandle> GetSplashes();
    CStr GetRandomMusicBlock() const;
    const CVector& GetNorth() const;

    using TexMap = retruxx::map<CStr, m3d::rend::TexHandle>;
    using TexPair = retruxx::pair<CStr, m3d::rend::TexHandle>;

private:
    int LoadBigImage();
    /* 0x0000 */ CStr m_name;
    /* 0x000c */ CStr m_fullName;
    /* 0x0018 */ CStr m_file;
    /* 0x0024 */ CStr m_diz0;
    /* 0x0030 */ CStr m_diz1;
    /* 0x003c */ CStr m_imageFile0;
    /* 0x0048 */ CStr m_imageFile1;
    /* 0x0054 */ retruxx::vector<CStr> m_splasheNames;
    /* 0x0064 */ retruxx::vector<CStr> m_musicBlockNames;
    /* 0x0074 */ m3d::rend::TexHandle m_image0;
    /* 0x0078 */ m3d::rend::TexHandle m_image1;
    /* 0x007c */ retruxx::map<CStr, m3d::rend::TexHandle> m_splashes;
    /* 0x0088 */ CVector m_north;
}; /* size: 0x0094 */

static_assert(sizeof(LevelInfo) == 0x94);

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
    LevelInfo* GetLevelInfoByName(const CStr& levelName);
    const LevelInfo* GetLevelInfoByName(const CStr& levelName) const;
    void GetAllLevelNames(retruxx::vector<CStr, retruxx::allocator<CStr> >& allLevelNames) const;
    void GetVisitedLevelNames(retruxx::vector<CStr, retruxx::allocator<CStr> >& visitedLevelNames) const;
    void GetKnownLevelNames(retruxx::vector<CStr, retruxx::allocator<CStr> >& knownLevelNames) const;
    bool IsLevelKnown(const CStr& levelName) const;
    bool IsLevelVisited(const CStr& levelName) const;
    float GetLevelSize(const CStr& levelName) const;
    retruxx::map<CStr, ObjectInfo*, retruxx::less<CStr>, retruxx::allocator<retruxx::pair<CStr const, ObjectInfo*> > >* GetObjectsForLevel(const CStr& levelName);
    const retruxx::map<CStr, ObjectInfo*, retruxx::less<CStr>, retruxx::allocator<retruxx::pair<CStr const, ObjectInfo*> > >* GetObjectsForLevel(const CStr& levelName) const;
    ObjectInfo* GetObjectInfo(const CStr& objectName, const CStr& levelName) const;
    VisibilityMap* GetVisibilityMapForLevel(const CStr& levelName) const;
    int GetVisibilityRadius() const;
    int AddKnownLevel(const CStr& levelName);
    int AddVisibilityCircleForLevel(const CStr& levelName, const CVector& origin, float radius);
    int AddVisibilityRectForLevel(const CStr& levelName, float x0, float y0, float w, float h);
    void ClearVisibilityMapForLevel(const CStr& levelName);
    int LoadFromXml(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode);
    int SaveToXml(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const;

protected:
    LevelInfoManager();
    LevelInfoManager(const LevelInfoManager& rhs);

public:
    virtual  ~LevelInfoManager() override /* 0x00 */;
    virtual m3d::Object* Clone() override /* 0x04 */;
    static m3d::Object* CreateObject();
    static m3d::Class* GetBaseClass();
    virtual m3d::Class* GetClass() const override /* 0x34 */;
    static m3d::Class m_classLevelInfoManager;

    using LevelInfoMap = retruxx::map<int, LevelInfo*, retruxx::less<int>, retruxx::allocator<retruxx::pair<int const, LevelInfo*> > >;
    using LevelInfoPair = retruxx::pair<int, LevelInfo*>;
    using LevelSizeMap = retruxx::map<CStr, float, retruxx::less<CStr>, retruxx::allocator<retruxx::pair<CStr const, float> > >;
    using LevelSizePair = retruxx::pair<CStr, float>;
    class LevelSizeMapConstIterator;
    using LevelObjectsMap = retruxx::map<CStr, retruxx::map<CStr, ObjectInfo*, retruxx::less<CStr>, retruxx::allocator<retruxx::pair<CStr const, ObjectInfo*> > >, retruxx::less<CStr>, retruxx::allocator<retruxx::pair<CStr const, retruxx::map<CStr, ObjectInfo*, retruxx::less<CStr>, retruxx::allocator<retruxx::pair<CStr const, ObjectInfo*> > > > > >;
    using LevelObjectsPair = retruxx::pair<CStr, retruxx::map<CStr, ObjectInfo*, retruxx::less<CStr>, retruxx::allocator<retruxx::pair<CStr const, ObjectInfo*> > > >;
    class LevelObjectsConstIterator;
    using VisibilityMapMap = retruxx::map<CStr, VisibilityMap*, retruxx::less<CStr>, retruxx::allocator<retruxx::pair<CStr const, VisibilityMap*> > >;
    using VisibilityMapPair = retruxx::pair<CStr, VisibilityMap*>;

public:
    void ClearBeforeNewLevel();
    void ClearBeforeContinuousLevel();
    void ClearOnce();
    void ClearLevelObjects();
    void ClearNonPersistantPriceInfoForLevel(const CStr& levelName);
    void ClearObjectIds(const CStr& levelName);
    void ClearKnownLevels();
    void ClearVisitedLevels();
    void ClearVisibilityMaps();
    int LoadLevelInfoFromXml(const CStr& fileName);
    int LoadLevelSizes();
    int LoadLevelSizeFromXml(const CStr& fileName);
    VisibilityMap* AddVisibilityMapForLevel(const CStr& levelName);
    int AddVisitedLevel(const CStr& levelName);
    void UpdateLevelImages();
    LevelInfo* GetLevelInfoById(int id) const;
    int GetLevelInfoId(const CStr& name) const;
    CStr GetLevelInfoName(int id) const;
    void OnStartLevel();
    void OnEndLevel();
    void OnEndLevelBeforeContinuousLevel();
    bool IsObjectCharted(const ai::Obj* o) const;
    void OnNewFrame();
    void UpdateVisibilityMaps();
    void UpdateObjectPositions();
    void OnLocationStateChanged(void* data);
    void OnTownRuined(void* data);
    int SaveObjectInfoForObject(const ai::Obj* o);
    void UpdateObjectInfoForCurrentLevel();
    void UpdateKnownLevels();
    int AddObjectInfo(ObjectInfo* oi);

    /* 0x0034 */ retruxx::map<int, LevelInfo*> m_levels;
    /* 0x0040 */ int m_nextLevelInfoId;
    /* 0x0044 */ retruxx::map<CStr, float> m_levelSizes;
    /* 0x0050 */ retruxx::map<CStr, retruxx::map<CStr, ObjectInfo*>> m_levelObjects;
    /* 0x005c */ retruxx::map<CStr, VisibilityMap*> m_visibilityMaps;
    /* 0x0068 */ retruxx::set<CStr> m_knownLevels;
    /* 0x0074 */ retruxx::set<CStr> m_visitedLevels;
    /* 0x0080 */ m3d::CVar m_cvVisibilityRadius;
}; /* size: 0x00ac */

static_assert(sizeof(LevelInfoManager) == 0xac);

class ObjectInfo
{
    friend class LevelInfoManager;

public:
    ObjectInfo(const ObjectInfo&);
    ObjectInfo();
    ~ObjectInfo();
    int GetId() const;
    const CStr& GetName() const;
    const CStr& GetFullName() const;
    const CStr& GetPrototypeName() const;
    int GetPrototypeId() const;
    const CVector& GetPosition() const;
    const CStr& GetLevelName() const;
    int GetBelong() const;
    const CStr* GetPassageAddress() const;
    const retruxx::map<int, CVector2, retruxx::less<int>, retruxx::allocator<retruxx::pair<int const, CVector2> > >* GetSavedPrices() const;
    int SavePrices();
    int AddSavedPriceByPrototypeId(int prototypeId, int sellPrice, int buyPrice);
    void DeleteSavedPrices();
    void SetSavedPricesPersistant(bool bPersistant);
    bool IsSavedPricesPersistant() const;
    bool IsRuined() const;
    int LoadFromXml(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode);
    int SaveToXml(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const;
    bool IsTown() const;
    bool IsLocation() const;
    bool IsActive() const;

private:
    const m3d::Class* GetObjectClass() const;

    /* 0x0000 */ CStr m_name;
    /* 0x000c */ CStr m_fullName;
    /* 0x0018 */ CStr m_prototypeName;
    /* 0x0024 */ int m_prototypeId;
    /* 0x0028 */ CVector m_position;
    /* 0x0034 */ CStr m_levelName;
    /* 0x0040 */ retruxx::map<int, CVector2, retruxx::less<int>, retruxx::allocator<retruxx::pair<int const, CVector2> > > m_savedPrices;
    /* 0x004c */ bool m_bSavedPricesPersistant;
    /* 0x004d */ bool m_bRuined;
    /* 0x004e */ char Padding_133[2];
    /* 0x0050 */ CStr m_passageAddress;
    /* 0x005c */ bool m_bIsActive;
    /* 0x005d */ char Padding_134[3];
    /* 0x0060 */ int m_objectId;
    /* 0x0064 */ int m_belong;
}; /* size: 0x0068 */

static_assert(sizeof(ObjectInfo) == 0x68);
