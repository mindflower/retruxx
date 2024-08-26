#pragma once
#include "childpanel.h"
#include <map>
#include <core/ref_ptr.h>
#include <server/objects/building.h>
#include <server/objects/workshop.h>
#include <ui/tabwnd.h>

namespace m3d
{
    namespace ui
    {
        class ImageWnd;
    }
}

class SaleWnd : public ChildPanel
{
public:
    enum WorkshopTabType
    {
        TABTYPE_GUNS_AND_GADGETS = 0x0,
        TABTYPE_GOODS = 0x1,
        TABTYPE_NUM_TABTYPES = 0x2,
        TABTYPE_INVALID = 0x2,
    };

public:
    virtual m3d::Class* GetRtClass() const;
    static m3d::Class* GetBaseClass();
    static std::vector<int> GetResourceIdsForTabItem(WorkshopTabType);
    virtual int SetUpForWorkshop(int);
    virtual ~SaleWnd();
    ai::Workshop* GetWorkshop(void) const;

protected:
    ref_ptr<m3d::ui::Wnd> GetWorkWndForTabItem(int) const;
    virtual int OnWndNotify(m3d::ui::Wnd*, unsigned int, unsigned int, m3d::AIParam const&);
    void AddTabItem(WorkshopTabType);
    virtual int OnBeforeAddToWndStation();
    SaleWnd();
    int LaunchTownDlg();
    int SetupTabInfo(m3d::ui::Wnd*, WorkshopTabType);
    virtual int SetupTabItem(m3d::ui::Wnd*, WorkshopTabType);
    virtual void SetupListOfTabTypes();
    int GetTabIdByTabType(WorkshopTabType) const;
    virtual void ClearTabItems();
    bool IsTabCreatedFromRepositorySource(WorkshopTabType) const;
    virtual m3d::ui::Wnd* CreateTabItem(WorkshopTabType);
    void CreateTabItems();
    WorkshopTabType GetTabTypeByTabId(int) const;
    virtual int GameDataClear(bool);
    static ai::WorkshopRepositoryType GetWorkshopRepositoryType(WorkshopTabType);
    virtual void UpdateTownPicture();
    virtual int GameDataSetup();
    virtual void UpdateOnChangeTabSel(int, int, m3d::ui::TabWnd::SelectionType);
    virtual int GameDataUpdate(void*, int);

public:
    RT_CLASS_DECLARE(SaleWnd);

private:
    m3d::ui::TabWnd* m_wndTab;
    std::map<SaleWnd::WorkshopTabType, ref_ptr<m3d::ui::Wnd>> m_wndTabItems;
    std::map<SaleWnd::WorkshopTabType, ref_ptr<m3d::ui::Wnd>> m_allTabItems;
    int m_workshopId;
    std::vector<enum SaleWnd::WorkshopTabType> m_tabTypes;
    ai::BuildingType m_buildingType;
    m3d::ui::ImageWnd* m_wndTownPicture;
};
