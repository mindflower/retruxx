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
}  // namespace m3d

class SaleWnd : public ChildPanel
{
public:
    enum WorkshopTabType
    {
        TABTYPE_GUNS_AND_GADGETS = 0,
        TABTYPE_GOODS = 1,
        TABTYPE_NUM_TABTYPES = 2,
        TABTYPE_INVALID = 2,
    };

public:
    virtual int SetUpForWorkshop(int workshopId);
    ai::Workshop* GetWorkshop() const;
    static std::vector<int, std::allocator<int>> __fastcall GetResourceIdsForTabItem(SaleWnd::WorkshopTabType tabType);

    struct AuxInfo
    {
        CStr m_wndTabName;
        CStr m_tabNames[2];
        CStr m_wndRepositoryTabItemName;
        CStr m_wndGoodsTabItemName;
        int m_wndTabItemsIds[2];
        PointBase<float> m_tabBtnSz;
        float m_tabBtnSpace;
        CStr m_wndTownPictureName;
        CStr m_townPictureTexName;
        AuxInfo(SaleWnd::AuxInfo const& rhs);
        AuxInfo();
    };

    using TabItemsMap = std::map<
        enum SaleWnd::WorkshopTabType,
        ref_ptr<m3d::ui::Wnd>,
        std::less<enum SaleWnd::WorkshopTabType>,
        std::allocator<std::pair<enum SaleWnd::WorkshopTabType const, ref_ptr<m3d::ui::Wnd>>>>;
    using TabItemsPair = std::pair<enum SaleWnd::WorkshopTabType, ref_ptr<m3d::ui::Wnd>>;
    using TabTypeVector = std::vector<enum SaleWnd::WorkshopTabType, std::allocator<enum SaleWnd::WorkshopTabType>>;

protected:
    virtual int GameDataSetup() override;
    virtual int GameDataClear(bool beforeContinuousLevel) override;
    virtual int GameDataUpdate(void* data, int dataType) override;
    virtual int OnWndNotify(m3d::ui::Wnd* from, unsigned int id, unsigned int msg, m3d::AIParam const& data) override;
    int LaunchTownDlg();
    virtual void UpdateOnChangeTabSel(int newTabIdx, int oldTabIdx, m3d::ui::TabWnd::SelectionType selectionType);
    virtual m3d::ui::Wnd* CreateTabItem(SaleWnd::WorkshopTabType type);
    virtual int SetupTabItem(m3d::ui::Wnd* tabItem, SaleWnd::WorkshopTabType tabType);
    int SetupTabInfo(m3d::ui::Wnd* itemWnd, SaleWnd::WorkshopTabType type);
    void AddTabItem(SaleWnd::WorkshopTabType tabType);
    void CreateTabItems();
    virtual void ClearTabItems();
    virtual void SetupListOfTabTypes();
    bool IsTabCreatedFromRepositorySource(SaleWnd::WorkshopTabType tabType) const;
    static ai::WorkshopRepositoryType __fastcall GetWorkshopRepositoryType(SaleWnd::WorkshopTabType);
    virtual int OnBeforeAddToWndStation() override;
    virtual ai::BuildingType GetBuildingType() = 0;
    ref_ptr<m3d::ui::Wnd> GetWorkWndForTabItem(int tabId) const;
    virtual void UpdateTownPicture();
    int GetTabIdByTabType(SaleWnd::WorkshopTabType tabType) const;
    SaleWnd::WorkshopTabType GetTabTypeByTabId(int tabId) const;

    m3d::ui::TabWnd* m_wndTab;
    TabItemsMap m_wndTabItems;
    TabItemsMap m_allTabItems;
    static SaleWnd::AuxInfo m_aif;
    int m_workshopId;
    TabTypeVector m_tabTypes;
    ai::BuildingType m_buildingType;
    m3d::ui::ImageWnd* m_wndTownPicture;

    SaleWnd(SaleWnd const& rhs);
    SaleWnd();

public:
    virtual ~SaleWnd() override;
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetRtClass() const;
    static m3d::Class m_classSaleWnd;
};
