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
    class AuxInfo
    {
    public:
        AuxInfo();

    private:
        CStr m_btnNextName;
        CStr m_btnPrevName;
        CStr m_wndListBgName;
        float m_space;
        float m_edge;
    };

public:
    virtual ~CBList(void);
    virtual m3d::Class* GetRtClass(void) const;
    static m3d::Class* GetBaseClass(void);
    int SetupForWorkshop(int);

protected:
    void ScrollPrev(void);
    virtual int OnMouseWheel(int, PointBase<float> const&);
    virtual int OnBeforeAddToWndStation(void);
    CBList(void);
    virtual int OnAfterAddToWndStation(void);
    void HackedRestore(void);
    void ClearItems(void);
    void OnBuyCB(int);
    int AddItem(int);
    virtual int OnAfterRemoveFromWndStation(void);
    void ScrollNext(void);
    bool CanScrollNext(void);
    virtual int GameDataClear(bool);
    ai::GeomRepository* GetWorkshopRepository(void) const;
    bool CanScrollPrev(void);
    void FullUpdate(void);
    void UpdateNextPrevButtonState(void);
    virtual void SelectItem(CBButton*);
    virtual int GameDataSetup(void);
    void GetCBIds(std::vector<int, std::allocator<int> >&) const;
    int CreateItems(void);
    virtual int OnWndNotify(m3d::ui::Wnd*, unsigned int, unsigned int, m3d::AIParam const&);
    ai::Workshop* GetWorkshop(void) const;
    void RecalcLayot(void);
    void AddInfoToEncyclopaedia(void);
    virtual void OnRepositoryChanged(void);
    virtual int GameDataUpdate(void*, int);

public:
    RT_CLASS_DECLARE(CBList);

private:
    m3d::ui::ButtonWnd* m_btnNext;
    m3d::ui::ButtonWnd* m_btnPrev;
    m3d::ui::Wnd* m_wndListBg;
    CBList::AuxInfo m_aif;
    std::vector<CBButton*> m_items;
    BoundsBase<float> m_listBounds;
    int m_firstItemId;
    int m_lastItemId;
    int m_workshopId;
    CBButton::Type m_type;
    int m_selItemId;
};
