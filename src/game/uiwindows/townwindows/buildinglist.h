#pragma once

class BuildingButton :  public m3d::ui::ButtonWnd
{
public:
    int SetUpForBuilding(class ai::Building const *);
    virtual ~BuildingButton();
    virtual struct m3d::Class * GetClass() const ;
    class ai::Building * GetBuilding() const ;
    virtual class m3d::Object * Clone();
    static class m3d::Object * __fastcall CreateObject();
    static struct m3d::Class * __fastcall GetBaseClass();
    int GetIdx() const ;
    void SetIdx(int);
protected:
    BuildingButton();
    BuildingButton(class BuildingButton const &);
private:
    int CreateChildren();
    CStr GetNameForBuilding(class ai::Building const *) const ;
    static void __fastcall ClearPattern();
    int CreateFromPattern();
    class m3d::rend::TexHandle GetIcoForBuilding(class ai::Building const *) const ;
    static int __fastcall LoadPattern(class m3d::ui::Wnd *);
    int m_buildingId;
    m3d::ui::ImageWnd *m_wndBuildingIco;
    m3d::ui::Wnd *m_lblBuildingName;
    int m_idx;
};

class BuildingList :  public m3d::ui::Wnd
{
public:
    virtual int GameDataClear(bool);
    virtual struct m3d::Class * GetClass() const ;
    virtual ~BuildingList();
    virtual class m3d::Object * Clone();
    static struct m3d::Class * __fastcall GetBaseClass();
    void SetUpForTown(class ai::Town *);
    int CreateFromPattern(class m3d::ui::Wnd *,bool);
    static class m3d::Object * __fastcall CreateObject();
protected:
    void RecalcLayot();
    BuildingList(class BuildingList const &);
    BuildingList();
    int CreateItems();
    void FullUpdate();
    virtual int OnWndNotify(class m3d::ui::Wnd *,unsigned int,unsigned int,class m3d::AIParam const &);
    class ai::Town const * GetTown() const ;
    void ClearItems();
    virtual int OnAfterRemoveFromWndStation();
    int AddButton(class BuildingButton *);
    void PlaceButton(class BuildingButton *);
    virtual int OnBeforeAddToWndStation();
private:
    std::vector<BuildingButton *> m_buttons;
    int m_townId;
    BuildingList::AuxInfo m_aif;
};
