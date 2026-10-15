#pragma once
#include <core/ref_ptr.h>
#include <ui/ui.h>
#include <vector>

namespace ai
{
    class Workshop;
    class GeomRepository;
}  // namespace ai

namespace m3d
{
    namespace ui
    {
        class ButtonWnd;
    }
}  // namespace m3d

class RepositoryItemWnd;

class CBButton : public m3d::ui::Wnd
{
    // CBList lays the buttons out, drives their selection pane and reads the
    // object each one stands for.
    friend class CBList;

public:
    // NOTE: the shipped declaration keeps Type private; it is public here
    // because CBList stores one as a member and names the values.
    enum Type
    {
        TYPE_CABIN = 0,
        TYPE_BASKET = 1,
        NUM_TYPES = 2,
    };

public:
    int SetupForCB(int cbId);
    int GetCBId() const;
    int GetPrice() const;
    void Select(bool bSelect);
    bool IsSelected() const;

    struct AuxInfo
    {
        CStr m_wndNameName;
        CStr m_btnBuyName;
        CStr m_wndPictureName;
        CStr m_wndDurabilityName;
        CStr m_wndResistPiercingName;
        CStr m_wndResistBlastName;
        CStr m_wndResistEnergyName;
        CStr m_wndWeightName;
        CStr m_wndPriceName;
        CStr m_selPaneName;
        AuxInfo(CBButton::AuxInfo const&);
        AuxInfo();
    };

protected:
    virtual int CreateFromPattern();
    virtual int CreateChildren();
    static int LoadPattern(ref_ptr<m3d::ui::Wnd> pattern, CBButton::Type type);
    static void ClearPattern(CBButton::Type type);
    virtual int OnWndNotify(m3d::ui::Wnd* from, unsigned int idFrom, unsigned int message, m3d::AIParam const& data)
        override;
    virtual int FullUpdate();
    int m_cbId;
    static CBButton::AuxInfo m_aif;
    static ref_ptr<m3d::ui::Wnd> m_wndPattern[2];
    m3d::ui::Wnd* m_wndName;
    m3d::ui::ButtonWnd* m_btnBuy;
    ref_ptr<RepositoryItemWnd> m_wndPicture;
    m3d::ui::Wnd* m_wndDurability;
    m3d::ui::Wnd* m_wndResistPiercing;
    m3d::ui::Wnd* m_wndResistBlast;
    m3d::ui::Wnd* m_wndResistEnergy;
    m3d::ui::Wnd* m_wndWeight;
    m3d::ui::Wnd* m_wndPrice;
    CBButton::Type m_type;
    bool m_bSelected;
    CBButton();
    CBButton(CBButton const& rhs);

public:
    virtual ~CBButton() override;
    virtual m3d::Object* Clone() override;
    static m3d::Object* CreateObject();
    static m3d::Class* GetBaseClass();
    virtual m3d::Class* GetClass() const override;
    RT_CLASS_DECLARE(CBButton);
};

class CBList : public m3d::ui::Wnd
{
public:
    int SetupForWorkshop(int workshopId);

    struct AuxInfo
    {
        CStr m_btnNextName;
        CStr m_btnPrevName;
        CStr m_wndListBgName;
        float m_space;
        float m_edge;
        AuxInfo(CBList::AuxInfo const&);
        AuxInfo();
    };

    using CBButtonVector = std::vector<CBButton*, std::allocator<CBButton*>>;

protected:
    virtual int GameDataSetup() override;
    virtual int GameDataUpdate(void* data, int dataType) override;
    virtual int GameDataClear(bool beforeContinuousLevel) override;
    virtual int OnWndNotify(m3d::ui::Wnd* from, unsigned int idFrom, unsigned int message, m3d::AIParam const& data)
        override;
    virtual int OnBeforeAddToWndStation() override;
    virtual int OnAfterAddToWndStation() override;
    virtual int OnAfterRemoveFromWndStation() override;
    virtual int OnMouseWheel(int ticks, PointBase<float> const& at) override;
    int AddItem(int cbId);
    // NOTE: declared in the PDB but no code was emitted for it in the shipped
    // binary and nothing references it, so it is left without a definition.
    int RemoveItem(int);
    void RecalcLayot();
    void ScrollNext();
    void ScrollPrev();
    void UpdateNextPrevButtonState();
    bool CanScrollNext();
    bool CanScrollPrev();
    void ClearItems();
    void OnBuyCB(int cbId);
    ai::Workshop* GetWorkshop() const;
    ai::GeomRepository* GetWorkshopRepository() const;
    void FullUpdate();
    int CreateItems();
    void GetCBIds(std::vector<int, std::allocator<int>>& cbIds) const;
    virtual int GetCBResourceId() const = 0;
    virtual CBButton* CreateItem() const = 0;
    virtual void OnRepositoryChanged();
    virtual void SelectItem(CBButton* btn);
    void HackedRestore();
    void AddInfoToEncyclopaedia();
    virtual void PostTriggerEventOnBuyCb() = 0;
    m3d::ui::ButtonWnd* m_btnNext;
    m3d::ui::ButtonWnd* m_btnPrev;
    m3d::ui::Wnd* m_wndListBg;
    CBList::AuxInfo m_aif;
    std::vector<CBButton*, std::allocator<CBButton*>> m_items;
    BoundsBase<float> m_listBounds;
    int m_firstItemId;
    int m_lastItemId;
    int m_workshopId;
    CBButton::Type m_type;
    int m_selItemId;
    CBList(CBList const&);
    CBList();

public:
    virtual ~CBList() override;
    static m3d::Class* GetBaseClass();
    virtual m3d::Class* GetRtClass() const;
    RT_CLASS_DECLARE(CBList);
};
