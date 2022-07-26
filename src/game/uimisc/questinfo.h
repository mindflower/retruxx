#pragma once
#include "guihelper.h"
#include <map>
#include <vector>
#include <core/stringm3d.h>

namespace m3d {
    namespace cmn {
        class XmlFile;
        class XmlNode;
    }
}

class CVector;

class QuestInfo
{
public:
    class AuxLevelInfo
    {
    public:
        enum CoordType
        {
            COORDTYPE_DIRECT = 0x0,
            COORDTYPE_OBJECT = 0x1,
            COORDTYPE_NONE = 0x2,
            COORDTYPE_NUM_COORDTYPES = 0x3,
            COORDTYPE_INVALID = 0x3,
        };
    public:
        void SetUp(CStr const&);
        void SetUp(CStr const&, CVector const&);
        AuxLevelInfo(AuxLevelInfo const&);
        AuxLevelInfo();
        CVector const* GetCoordinate() const;
        int LoadFromXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);
        CStr const& GetLevelName() const;
        QuestInfo::AuxLevelInfo& operator=(QuestInfo::AuxLevelInfo const&);
        void SetUpForObj(CStr const&, CStr const&);
        ~AuxLevelInfo();
        bool IsValid() const;
        int SaveToXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const;
        void Invalidate();
        CoordType GetCoordType() const;

    private:
        void SetCoordinate(CVector const*);
        CStr m_levelName;
        CVector* m_coordinate;
        CStr m_targetObjName;
        CoordType m_coordType;
    };
public:
    CStr const& GetBriefDiz() const;
    bool ExistsOnMap(CStr const&) const;
    bool IsMainQuest() const;
    CStr const& GetHirerName() const;
    int GetQuestId() const;
    CStr const& GetSmallImageFie() const;
    QuestInfo();
    std::vector<CStr, std::allocator<CStr> > GetMapNames() const;
    CStr const& GetFullDiz() const;
    CStr const& GetQuestInfoName() const;
    bool IsCoordinateModified() const;
    help::QuestType GetQuestType() const;
    CVector const* GetCoordinateForMap(CStr const&) const;
    int LoadFromXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);
    ~QuestInfo();
    int SaveCoordinatesToXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const;
    int LoadCoordinatesFromXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);
    CStr const& GetLargeImageFile() const;
    CStr const& GetLiteraryDiz() const;
    void SetCoordinateForMap(CStr const&, CVector const&);

private:
    void UpdateQuestId();
    void OnStartLevel();

private:
    CStr m_questInfoName;
    int m_questId;
    help::QuestType m_questType;
    CStr m_briefDiz;
    CStr m_fullDiz;
    CStr m_literaryDiz;
    CStr m_smallImageFile;
    CStr m_largeImageFile;
    std::map<CStr, AuxLevelInfo> m_levelInfo;
    CStr m_hirerName;
    bool m_isMainQuest;
    bool m_bCoordinateModified;
};


class QuestInfoManager
{
public:
    int SaveModifiedQuestInfosToXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const;
    QuestInfoManager();
    QuestInfo const* GetQuestInfoByQuestInfoName(CStr const&) const;
    void SetCoordinateForQuest(CStr const&, CStr const&, CVector const&);
    static CStr __fastcall GetDynamicQuestInfoPatternNameByType(int);
    QuestInfo const* GetQuestInfoForStaticQuest(int) const;
    void Clear();
    ~QuestInfoManager();
    int Init();
    int LoadFromXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);
    static bool __fastcall IsQuestDynamic(CStr const&);
    int GameDataUpdate(void*, int);
    QuestInfo const* GetQuestInfoForDynamicQuest(int);
    int LoadModifiedQuestInfosFromXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);

private:
    void OnEndLevel();
    void ClearQuestInfos();
    QuestInfo* CreateQuestInfoForDynamicQuest(int) const;
    void OnStartLevel();
    void OnDynamicQuestStateChanged(void*);
    void GetAllQuestInfosByNamePattern(CStr const&, std::vector<CStr>&) const;
    int LoadFromFile(CStr const&);
    QuestInfo const* AddQuestInfoForDynamicQuest(int);
    void ClearDynamicQuestInfos();
    void OnEndLevelBeforeContinuousLevel();
    int RemoveQuestInfoForDynamicQuest(int);

private:
    bool m_isInited = false;
    std::map<CStr, QuestInfo*> m_questInfos;
    std::map<int, QuestInfo*> m_dynamicQuestInfos;
};