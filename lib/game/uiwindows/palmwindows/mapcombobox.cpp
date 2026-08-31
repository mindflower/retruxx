#include "mapcombobox.h"

#include "taksebequestlogwnd.h"

#include <core/kernel.h>
#include <core/log.h>

#include "game/m3dgame.h"
#include "game/uimanager/truxxuimanager.h"
#include "game/uimisc/guihelper.h"
#include "game/uimisc/levelinfo.h"

RT_CLASS_EXPORTS_BEGIN(MapComboBox)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(MapComboBox);

int MapComboBox::CreateFromPattern(m3d::ui::Wnd const* patternWnd, bool deleteSrc)
{
    auto* pattern = RT_DYNCAST(patternWnd, m3d::ui::ComboBoxWnd const);
    if (!pattern)
    {
        return 0;
    }

    BoundsBase<float> bounds = pattern->GetBounds();
    unsigned int style = pattern->GetStyle();
    unsigned int comboStyle = pattern->GetComboStyle();
    float listMaxHeight = pattern->GetMaxListHeight();
    float selTextFixedHeight = pattern->GetSelTextFixedHeight();

    if (!Create(style, bounds, pattern->GetId(), comboStyle, listMaxHeight, selTextFixedHeight))
    {
        return 0;
    }

    // Copy the remaining state / appearance from the pattern control.
    SetStyle(pattern->GetStyle());
    SetText(pattern->GetText());
    SetId(pattern->GetId());
    SetPane(pattern->GetPaneName());
    SetPaneFlags(pattern->GetPaneFlags());
    SetDefaultFont(pattern->GetDefaultFont());
    SetColor(pattern->GetColor());
    SetTextColor(pattern->GetTextColor());
    SetTextColorDisabled(pattern->GetTextColorDisabled());
    SetClientEdges(pattern->GetClientEdges());
    SetName(pattern->GetName());
    SetScrollPane(pattern->GetScrollPaneName());
    SetBackground(pattern->GetBackground());

    CStr tooltip;
    pattern->GetProperty(PROP_WND_TOOLTIP, &tooltip);
    SetProperty(PROP_WND_TOOLTIP, &tooltip);

    SetOnShowAnimation(pattern->GetOnShowAnimation());
    SetOnHideAnimation(pattern->GetOnHideAnimation());

    // Take the pattern's place in the window tree, then optionally destroy it.
    if (m3d::Object* parent = pattern->GetParent())
    {
        parent->AddChild(this);
        if (deleteSrc)
        {
            delete const_cast<m3d::ui::ComboBoxWnd*>(pattern);
        }
    }

    m_gameDataFlags |= 1;
    return 1;
}

m3d::Class* MapComboBox::GetBaseClass()
{
    return RT_CLASS_LOCAL(ComboBoxWnd);
}

CStr MapComboBox::GetSelectedMapName() const
{
    if ((m_gameDataFlags & 1) == 0)
    {
        return {};
    }
    int sel = GetCurSel();
    if (sel == -1)
    {
        return {};
    }
    int levelInfoId = GetItemData(sel);
    return M3D_APP->m_pInterfaceManager->GetLevelInfoManager()->GetLevelInfoName(levelInfoId);
}

m3d::Object* MapComboBox::Clone()
{
    return new MapComboBox(*this);
}

m3d::Object* MapComboBox::CreateObject()
{
    return new MapComboBox;
}

m3d::Class* MapComboBox::GetClass() const
{
    return RT_CLASS_LOCAL(MapComboBox);
}

MapComboBox::~MapComboBox()
{
    // chains to ~ComboBoxWnd
}

int MapComboBox::GameDataClear(bool)
{
    RemoveAllItems();
    return 1;
}

int MapComboBox::GameDataUpdate(void*, int dataType)
{
    if ((m_gameDataFlags & 1) == 0)
    {
        return 0;
    }
    if (dataType == 85 /*0x55*/)
    {
        OnStartLevel();
    }
    return 1;
}

MapComboBox::MapComboBox()
{
}

int MapComboBox::OnStartLevel()
{
    if ((m_gameDataFlags & 1) == 0)
    {
        return 0;
    }

    GameDataClear(false);  // clear the current list before rebuilding it

    LevelInfoManager* lim = M3D_APP->m_pInterfaceManager->GetLevelInfoManager();

    // 1) one entry per visited level, tagged with its level-info id.
    retruxx::vector<CStr> visitedLevelNames;
    lim->GetVisitedLevelNames(visitedLevelNames);
    for (size_t i = 0; i < visitedLevelNames.size(); ++i)
    {
        int id = lim->GetLevelInfoId(visitedLevelNames[i]);
        LevelInfo* levelInfo = lim->GetLevelInfoById(id);
        if (!levelInfo)
        {
            continue;
        }
        int idx = AddItem(levelInfo->GetName());
        if (idx != -1)
        {
            SetItemData(idx, lim->GetLevelInfoId(visitedLevelNames[i]));
        }
    }

    // 2) decide what to select.
    m3d::Object* host = GetParent();
    while (host && !host->IsKindOf(RT_CLASS_LOCAL(TakSebeQuestLogWnd)))
    {
        host = host->GetParent();
    }

    if (host)
    {
        // Hosted in the quest log: add a localized "All" entry (data -2 = any map) and select it.
        int idx = AddItem(M3D_APP->GetStringByStringId0("All"));
        if (idx != -1)
        {
            SetItemData(idx, -2);
            SetCurSel(idx);
        }
    }
    else
    {
        // Standalone: select the entry that matches the current level.
        int curLevelId = lim->GetLevelInfoId(help::GetCurrentLevelName());
        if (curLevelId != -1)
        {
            int count = GetCount();
            for (int i = 0; i < count; ++i)
            {
                if (GetItemData(i) == curLevelId)
                {
                    SetCurSel(i);
                    break;
                }
            }
        }
        else
        {
            M3D_LOG_ERR("Error in updating map list: no info for current map exists");
            if (GetCount() != 0)
            {
                SetCurSel(0);
            }
        }
    }

    return 1;
}
