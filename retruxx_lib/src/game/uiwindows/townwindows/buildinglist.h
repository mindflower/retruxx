#pragma once
#include <ui/button.h>

namespace ai
{
    class Building;
    class Town;
}

namespace m3d
{
    namespace ui
    {
        class ImageWnd;
    }
}

class BuildingButton :  public m3d::ui::ButtonWnd
{
public:
    int SetUpForBuilding(ai::Building const *);
    virtual ~BuildingButton();
    virtual m3d::Class * GetClass() const ;
    ai::Building * GetBuilding() const ;
    virtual m3d::Object * Clone();
    static m3d::Object * CreateObject();
    static m3d::Class * GetBaseClass();
    int GetIdx() const ;
    void SetIdx(int);

protected:
    BuildingButton();
    BuildingButton(BuildingButton const &);

private:
    int CreateChildren();
    CStr GetNameForBuilding(ai::Building const *) const ;
    static void __fastcall ClearPattern();
    int CreateFromPattern();
    m3d::rend::TexHandle GetIcoForBuilding(ai::Building const *) const ;
    static int __fastcall LoadPattern(m3d::ui::Wnd *);

public:
    RT_CLASS_DECLARE(BuildingButton);

private:
    int m_buildingId;
    m3d::ui::ImageWnd *m_wndBuildingIco;
    m3d::ui::Wnd *m_lblBuildingName;
    int m_idx;
};

class BuildingList :  public m3d::ui::Wnd
{
public:
    class AuxInfo
    {
    public:
        AuxInfo();

    private:
        float m_spaceY;
    };

public:
    virtual int GameDataClear(bool);
    virtual m3d::Class * GetClass() const ;
    virtual ~BuildingList();
    virtual m3d::Object * Clone();
    static m3d::Class * GetBaseClass();
    void SetUpForTown(ai::Town *);
    int CreateFromPattern(m3d::ui::Wnd *,bool);
    static m3d::Object * CreateObject();

protected:
    void RecalcLayot();
    BuildingList(BuildingList const &);
    BuildingList();
    int CreateItems();
    void FullUpdate();
    virtual int OnWndNotify(m3d::ui::Wnd *,unsigned int,unsigned int, m3d::AIParam const &);
    ai::Town const * GetTown() const ;
    void ClearItems();
    virtual int OnAfterRemoveFromWndStation();
    int AddButton(BuildingButton *);
    void PlaceButton(BuildingButton *);
    virtual int OnBeforeAddToWndStation();

public:
    RT_CLASS_DECLARE(BuildingList);

private:
    std::vector<BuildingButton *> m_buttons;
    int m_townId;
    BuildingList::AuxInfo m_aif;
};
