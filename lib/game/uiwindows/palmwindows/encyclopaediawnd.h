#pragma once
#include "checklist.h"
#include <ui/ui.h>

class ContextModelWnd;

namespace m3d
{
    namespace ui
    {
        class ComboBoxWnd;
        class TextBoxWnd;
    }  // namespace ui
}  // namespace m3d

class SubjectList;

namespace ai
{
    class Vehicle;
    class PrototypeInfo;
}  // namespace ai

class EncyclopaediaWnd : public m3d::ui::Wnd
{
public:
    enum Theme
    {
        THEME_VEHICLES = 0,
        THEME_CB = 1,
        THEME_WEAPON = 2,
        THEME_GADGETS = 3,
        THEME_AFFIXES = 4,
        THEME_WARES = 5,
        THEME_CLANS = 6,
        THEME_QUEST_ITEMS = 7,
        THEME_NUM_THEMES = 8,
        THEME_INVALID = 8,
    };

public:
    int AddPrototype(int prototypeId);
    bool IsPrototypeVisible(int prototypeId) const;
    std::vector<int, std::allocator<int>> const* GetPrototypesForTheme(EncyclopaediaWnd::Theme theme) const;
    int AddClan(int clanBelong);
    bool IsClanVisible(int clanBelong) const;
    std::vector<int, std::allocator<int>> const* GetClans() const;
    void ShowAll();

    struct AuxInfo
    {
        CStr m_subjectListName;
        CStr m_wndDizName;
        CStr m_wndModelName;
        CStr m_comboThemesName;
        AuxInfo(EncyclopaediaWnd::AuxInfo const&);
        AuxInfo();
    };

    using ThemePrototypeMap = std::map<
        enum EncyclopaediaWnd::Theme,
        std::vector<int, std::allocator<int>>,
        std::less<enum EncyclopaediaWnd::Theme>,
        std::allocator<std::pair<enum EncyclopaediaWnd::Theme const, std::vector<int, std::allocator<int>>>>>;
    using ThemePrototypePair = std::pair<enum EncyclopaediaWnd::Theme, std::vector<int, std::allocator<int>>>;
    using PrototypeSet = std::set<int, std::less<int>, std::allocator<int>>;
    using RelatedPrototypeMap = std::map<int, int, std::less<int>, std::allocator<std::pair<int const, int>>>;
    using RelatedPrototypePair = std::pair<int, int>;

protected:
    virtual int GameDataClear(bool beforeContinouosLevel) override;
    virtual int GameDataSetup() override;
    virtual int GameDataUpdate(void* data, int dataType) override;
    virtual int OnWndNotify(m3d::ui::Wnd* from, unsigned int id, unsigned int msg, m3d::AIParam const& data) override
       ;
    virtual int GameDataSave(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* guiNode) override;
    virtual int GameDataLoad(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* guiNode) override;
    virtual int OnAfterAddToWndStation() override;
    CStr Theme2Str(EncyclopaediaWnd::Theme theme) const;
    EncyclopaediaWnd::Theme Str2Theme(CStr const& themeName) const;
    void OnThemeChanged();
    void OnSubjectChanged();
    int UpdateModel();
    int UpdateDiz();
    EncyclopaediaWnd::Theme GetCurTheme() const;
    CStr GetCurSubjectName() const;
    m3d::rend::TexHandle GetImageForAffix(CStr const& affixName) const;
    m3d::rend::TexHandle GetImageForClan(CStr const& clanName) const;
    int InitThemes();
    int CachePrototype(ai::PrototypeInfo const*);
    EncyclopaediaWnd::Theme GetThemeByPrototypeId(int prototypeId) const;
    EncyclopaediaWnd::Theme GetThemeByResourceId(int resourceId) const;
    void OnPrototypeMet(void* data);
    void OnObjMet(void* data);
    void OnBelongMet(void* data);
    void OnStartLevel(void* data);
    void OnPlayerVehicleChanged();
    void OnGadgetChanged(void* data);
    void OnPlayerVehicleDamaged(void* data);
    void OnQuestItemsChanged();
    void OnNewFrameForce();
    int AddPrototypeByThemeUnsafe(int prototypeId, EncyclopaediaWnd::Theme theme);
    int AddObj(int objId);
    int AddVehicle(ai::Vehicle const* vehicle);
    int InitOnce();
    int GetRelatedVisiblePrototypeId(int unvisiblePrototypeId) const;
    void CacheRelatedPrototypes();
    void InitAlwaysVisiblePrototypes();
    void GetPrototypeIdsKindOfResource(int resourceId, std::vector<int, std::allocator<int>>& prototypeIds) const;
    void CheckAndAddTargetInfoVehicle();
    void CheckAndAddTargetCapturingVehicle();
    void CheckAndAddTargetCapturedVehicle();
    SubjectList* m_subjectList;
    m3d::ui::TextBoxWnd* m_wndDiz;
    m3d::ui::ComboBoxWnd* m_comboThemes;
    ContextModelWnd* m_wndModel;
    EncyclopaediaWnd::AuxInfo m_aif;
    std::map<
        enum EncyclopaediaWnd::Theme,
        std::vector<int, std::allocator<int>>,
        std::less<enum EncyclopaediaWnd::Theme>,
        std::allocator<std::pair<enum EncyclopaediaWnd::Theme const, std::vector<int, std::allocator<int>>>>>
        m_prototypesByTheme;
    std::set<int, std::less<int>, std::allocator<int>> m_visiblePrototypeIds;
    std::map<int, int, std::less<int>, std::allocator<std::pair<int const, int>>> m_relatedPrototypeIds;
    bool m_bInitedOnce;
    char Padding_206[3];
    std::vector<int, std::allocator<int>> m_clanBelongsVector;
    std::set<int, std::less<int>, std::allocator<int>> m_clanBelongsSet;
    int m_prevInfoObjId;
    int m_prevCapturingObjId;
    int m_prevCapturedObjId;
    EncyclopaediaWnd();
    EncyclopaediaWnd(EncyclopaediaWnd const& rhs);

public:
    virtual ~EncyclopaediaWnd() override;
    virtual m3d::Object* Clone() override;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override;
    static m3d::Class m_classEncyclopaediaWnd;
};

class SubjectButton : public CheckButton
{
public:
    int SetUp(CStr const& name, PointBase<float> const& origin, float width, CStr const& fullName);
    int SetUp(CStr const& name, PointBase<float> const& origin, float width, EncyclopaediaWnd::Theme theme, int id);

protected:
    virtual CStr CalcFullName() const override;
    EncyclopaediaWnd::Theme m_theme;
    int m_id;
    SubjectButton();
    SubjectButton(SubjectButton const& rhs);

public:
    virtual ~SubjectButton() override;
    virtual m3d::Object* Clone() override;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override;
    static m3d::Class m_classSubjectButton;
};

class SubjectList : public CheckList
{
public:
    int SetUpForTheme(EncyclopaediaWnd::Theme theme);
    CStr GetCurSubjectName() const;
    int Update();

protected:
    virtual int OnBeforeAddToWndStation() override;
    virtual int AddButtonByNameAndId(CStr const& name, int id);
    virtual int AddButtonByName(CStr const& name, CStr const& fullName) override;
    int CreateItemsForPrototypes(EncyclopaediaWnd::Theme theme);
    int CreateItemsForAffixes();
    int CreateItemsForClans();
    EncyclopaediaWnd::Theme m_theme;
    SubjectList();
    SubjectList(SubjectList const& rhs);

public:
    virtual ~SubjectList() override;
    virtual m3d::Object* Clone() override;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override;
    static m3d::Class m_classSubjectList;
};
