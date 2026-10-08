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
        TABTYPE_GUNS_AND_GADGETS = 0,
        TABTYPE_GOODS = 1,
        TABTYPE_NUM_TABTYPES = 2,
        TABTYPE_INVALID = 2,
    };

public:
    virtual int SetUpForWorkshop(int workshopId) /* 0x128 */;
    ai::Workshop* GetWorkshop() const;
    static std::vector<int, std::allocator<int>> __fastcall GetResourceIdsForTabItem(SaleWnd::WorkshopTabType tabType);

    struct AuxInfo
    {
        /* 0x0000 */ CStr m_wndTabName;
        /* 0x000c */ CStr m_tabNames[2];
        /* 0x0024 */ CStr m_wndRepositoryTabItemName;
        /* 0x0030 */ CStr m_wndGoodsTabItemName;
        /* 0x003c */ int m_wndTabItemsIds[2];
        /* 0x0044 */ PointBase<float> m_tabBtnSz;
        /* 0x004c */ float m_tabBtnSpace;
        /* 0x0050 */ CStr m_wndTownPictureName;
        /* 0x005c */ CStr m_townPictureTexName;
        AuxInfo(SaleWnd::AuxInfo const& rhs);
        AuxInfo();
    }; /* size: 0x0068 */

    using TabItemsMap = std::map<
        enum SaleWnd::WorkshopTabType,
        ref_ptr<m3d::ui::Wnd>,
        std::less<enum SaleWnd::WorkshopTabType>,
        std::allocator<std::pair<enum SaleWnd::WorkshopTabType const, ref_ptr<m3d::ui::Wnd>>>>;
    using TabItemsPair = std::pair<enum SaleWnd::WorkshopTabType, ref_ptr<m3d::ui::Wnd>>;
    using TabTypeVector = std::vector<enum SaleWnd::WorkshopTabType, std::allocator<enum SaleWnd::WorkshopTabType>>;

protected:
    virtual int GameDataSetup() override /* 0x104 */;
    virtual int GameDataClear(bool beforeContinuousLevel) override /* 0x108 */;
    virtual int GameDataUpdate(void* data, int dataType) override /* 0x10c */;
    virtual int OnWndNotify(m3d::ui::Wnd* from, unsigned int id, unsigned int msg, m3d::AIParam const& data) override
        /* 0xc8 */;
    int LaunchTownDlg();
    virtual void UpdateOnChangeTabSel(int newTabIdx, int oldTabIdx, m3d::ui::TabWnd::SelectionType selectionType)
        /* 0x12c */;
    virtual m3d::ui::Wnd* CreateTabItem(SaleWnd::WorkshopTabType type) /* 0x130 */;
    virtual int SetupTabItem(m3d::ui::Wnd* tabItem, SaleWnd::WorkshopTabType tabType) /* 0x134 */;
    int SetupTabInfo(m3d::ui::Wnd* itemWnd, SaleWnd::WorkshopTabType type);
    void AddTabItem(SaleWnd::WorkshopTabType tabType);
    void CreateTabItems();
    virtual void ClearTabItems() /* 0x138 */;
    virtual void SetupListOfTabTypes() /* 0x13c */;
    bool IsTabCreatedFromRepositorySource(SaleWnd::WorkshopTabType tabType) const;
    static ai::WorkshopRepositoryType __fastcall GetWorkshopRepositoryType(SaleWnd::WorkshopTabType);
    virtual int OnBeforeAddToWndStation() override /* 0x68 */;
    virtual ai::BuildingType GetBuildingType() = 0 /* 0x140 */;
    ref_ptr<m3d::ui::Wnd> GetWorkWndForTabItem(int tabId) const;
    virtual void UpdateTownPicture() /* 0x144 */;
    int GetTabIdByTabType(SaleWnd::WorkshopTabType tabType) const;
    SaleWnd::WorkshopTabType GetTabTypeByTabId(int tabId) const;

    /* 0x0224 */ m3d::ui::TabWnd* m_wndTab;
    /* 0x0228 */ TabItemsMap m_wndTabItems;
    /* 0x0234 */ TabItemsMap m_allTabItems;
    static SaleWnd::AuxInfo m_aif;
    /* 0x0240 */ int m_workshopId;
    /* 0x0244 */ TabTypeVector m_tabTypes;
    /* 0x0254 */ ai::BuildingType m_buildingType;
    /* 0x0258 */ m3d::ui::ImageWnd* m_wndTownPicture;

    SaleWnd(SaleWnd const& rhs);
    SaleWnd();

public:
    virtual ~SaleWnd() override;
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetRtClass() const /* 0x148 */;
    static m3d::Class m_classSaleWnd;
}; /* size: 0x025c */
