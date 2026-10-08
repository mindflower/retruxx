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
        /* 0x0000 */ CStr m_wndNameName;
        /* 0x000c */ CStr m_btnBuyName;
        /* 0x0018 */ CStr m_wndPictureName;
        /* 0x0024 */ CStr m_wndDurabilityName;
        /* 0x0030 */ CStr m_wndResistPiercingName;
        /* 0x003c */ CStr m_wndResistBlastName;
        /* 0x0048 */ CStr m_wndResistEnergyName;
        /* 0x0054 */ CStr m_wndWeightName;
        /* 0x0060 */ CStr m_wndPriceName;
        /* 0x006c */ CStr m_selPaneName;
        AuxInfo(CBButton::AuxInfo const&);
        AuxInfo();
    }; /* size: 0x0078 */

protected:
    virtual int CreateFromPattern() /* 0x11c */;
    virtual int CreateChildren() /* 0x120 */;
    static int __fastcall LoadPattern(ref_ptr<m3d::ui::Wnd> pattern, CBButton::Type type);
    static void __fastcall ClearPattern(CBButton::Type type);
    virtual int OnWndNotify(m3d::ui::Wnd* from, unsigned int idFrom, unsigned int message, m3d::AIParam const& data)
        override /* 0xc8 */;
    virtual int FullUpdate() /* 0x124 */;
    /* 0x0220 */ int m_cbId;
    static CBButton::AuxInfo m_aif;
    static ref_ptr<m3d::ui::Wnd> m_wndPattern[2];
    /* 0x0224 */ m3d::ui::Wnd* m_wndName;
    /* 0x0228 */ m3d::ui::ButtonWnd* m_btnBuy;
    /* 0x022c */ ref_ptr<RepositoryItemWnd> m_wndPicture;
    /* 0x0230 */ m3d::ui::Wnd* m_wndDurability;
    /* 0x0234 */ m3d::ui::Wnd* m_wndResistPiercing;
    /* 0x0238 */ m3d::ui::Wnd* m_wndResistBlast;
    /* 0x023c */ m3d::ui::Wnd* m_wndResistEnergy;
    /* 0x0240 */ m3d::ui::Wnd* m_wndWeight;
    /* 0x0244 */ m3d::ui::Wnd* m_wndPrice;
    /* 0x0248 */ CBButton::Type m_type;
    /* 0x024c */ bool m_bSelected;
    CBButton();
    CBButton(CBButton const& rhs);

public:
    virtual ~CBButton() override /* 0x00 */;
    virtual m3d::Object* Clone() override /* 0x04 */;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override /* 0x34 */;
    RT_CLASS_DECLARE(CBButton);
}; /* size: 0x0250 */

class CBList : public m3d::ui::Wnd
{
public:
    int SetupForWorkshop(int workshopId);

    struct AuxInfo
    {
        /* 0x0000 */ CStr m_btnNextName;
        /* 0x000c */ CStr m_btnPrevName;
        /* 0x0018 */ CStr m_wndListBgName;
        /* 0x0024 */ float m_space;
        /* 0x0028 */ float m_edge;
        AuxInfo(CBList::AuxInfo const&);
        AuxInfo();
    }; /* size: 0x002c */

    using CBButtonVector = std::vector<CBButton*, std::allocator<CBButton*>>;

protected:
    virtual int GameDataSetup() override /* 0x104 */;
    virtual int GameDataUpdate(void* data, int dataType) override /* 0x10c */;
    virtual int GameDataClear(bool beforeContinuousLevel) override /* 0x108 */;
    virtual int OnWndNotify(m3d::ui::Wnd* from, unsigned int idFrom, unsigned int message, m3d::AIParam const& data)
        override /* 0xc8 */;
    virtual int OnBeforeAddToWndStation() override /* 0x68 */;
    virtual int OnAfterAddToWndStation() override /* 0x6c */;
    virtual int OnAfterRemoveFromWndStation() override /* 0x74 */;
    virtual int OnMouseWheel(int ticks, PointBase<float> const& at) override /* 0xb8 */;
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
    virtual int GetCBResourceId() const = 0 /* 0x11c */;
    virtual CBButton* CreateItem() const = 0 /* 0x120 */;
    virtual void OnRepositoryChanged() /* 0x124 */;
    virtual void SelectItem(CBButton* btn) /* 0x128 */;
    void HackedRestore();
    void AddInfoToEncyclopaedia();
    virtual void PostTriggerEventOnBuyCb() = 0 /* 0x12c */;
    /* 0x0220 */ m3d::ui::ButtonWnd* m_btnNext;
    /* 0x0224 */ m3d::ui::ButtonWnd* m_btnPrev;
    /* 0x0228 */ m3d::ui::Wnd* m_wndListBg;
    /* 0x022c */ CBList::AuxInfo m_aif;
    /* 0x0258 */ std::vector<CBButton*, std::allocator<CBButton*>> m_items;
    /* 0x0268 */ BoundsBase<float> m_listBounds;
    /* 0x0278 */ int m_firstItemId;
    /* 0x027c */ int m_lastItemId;
    /* 0x0280 */ int m_workshopId;
    /* 0x0284 */ CBButton::Type m_type;
    /* 0x0288 */ int m_selItemId;
    CBList(CBList const&);
    CBList();

public:
    virtual ~CBList() override /* 0x00 */;
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetRtClass() const /* 0x130 */;
    RT_CLASS_DECLARE(CBList);
}; /* size: 0x028c */
