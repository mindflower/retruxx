#pragma once
#include <core/ref_ptr.h>
#include <ui/ui.h>

namespace ai
{
    class Workshop;
    class GeomRepository;
}

namespace m3d
{
    namespace ui
    {
        class ButtonWnd;
    }
}

class RepositoryItemWnd;

class CBButton :  public m3d::ui::Wnd
{
public:
    enum Type
    {
        TYPE_CABIN = 0x0,
        TYPE_BASKET = 0x1,
        NUM_TYPES = 0x2,
    };

public:
    static m3d::Class * GetBaseClass();
    int GetCBId() const ;
    int SetupForCB(int);
    static m3d::Object * CreateObject();
    virtual ~CBButton();
    int GetPrice() const ;
    virtual m3d::Object * Clone();
    virtual m3d::Class * GetClass() const ;
    void Select(bool);

protected:
    virtual int CreateChildren();
    virtual int CreateFromPattern();
    CBButton();
    CBButton(CBButton const &);
    static void __fastcall ClearPattern(Type);
    virtual int FullUpdate();
    virtual int OnWndNotify(m3d::ui::Wnd *,unsigned int,unsigned int, m3d::AIParam const &);
    static int __fastcall LoadPattern(ref_ptr<m3d::ui::Wnd>, Type);

public:
    RT_CLASS_DECLARE(CBButton);

private:
    int m_cbId;
    m3d::ui::Wnd *m_wndName;
    m3d::ui::ButtonWnd *m_btnBuy;
    ref_ptr<RepositoryItemWnd> m_wndPicture;
    m3d::ui::Wnd *m_wndDurability;
    m3d::ui::Wnd *m_wndResistPiercing;
    m3d::ui::Wnd *m_wndResistBlast;
    m3d::ui::Wnd *m_wndResistEnergy;
    m3d::ui::Wnd *m_wndWeight;
    m3d::ui::Wnd *m_wndPrice;
    CBButton::Type m_type;
    bool m_bSelected;
};

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
        AuxInfo(const CBList::AuxInfo&);
        AuxInfo();
    }; /* size: 0x002c */

    using CBButtonVector = std::vector<CBButton*, std::allocator<CBButton*>>;

protected:
    virtual int GameDataSetup() override /* 0x104 */;
    virtual int GameDataUpdate(void* data, int dataType) override /* 0x10c */;
    virtual int GameDataClear(bool beforeContinuousLevel) override /* 0x108 */;
    virtual int OnWndNotify(m3d::ui::Wnd* from, unsigned int idFrom, unsigned int message, const m3d::AIParam& data) override /* 0xc8 */;
    virtual int OnBeforeAddToWndStation() override /* 0x68 */;
    virtual int OnAfterAddToWndStation() override /* 0x6c */;
    virtual int OnAfterRemoveFromWndStation() override /* 0x74 */;
    virtual int OnMouseWheel(int ticks, const PointBase<float>& at) override /* 0xb8 */;
    int AddItem(int cbId);
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
    CBList(const CBList&);
    CBList();

public:
    virtual ~CBList() override /* 0x00 */;
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetRtClass() const /* 0x130 */;
    static m3d::Class m_classCBList;
}; /* size: 0x028c */
