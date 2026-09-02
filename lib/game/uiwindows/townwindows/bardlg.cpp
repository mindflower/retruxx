#include "bardlg.h"

#include "npclist.h"

#include <core/kernel.h>
#include <core/log.h>
#include <game/m3dgame.h>
#include <game/uimisc/guihelper.h>
#include <ui/textbox.h>
#include <server/objects/bar.h>
#include <server/objects/npc.h>
#include <server/objects/base/objcontainer.h>

RT_CLASS_EXPORTS_BEGIN(BarDlg)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(BarDlg);

BarDlg::AuxInfo::AuxInfo()
{
    m_lstNPCsPatternName = "wndNpcList";
}

m3d::Object* BarDlg::Clone()
{
    return new BarDlg(*this);
}

ai::Bar* BarDlg::GetBar() const
{
    if (m_barId == -1)
    {
        return nullptr;
    }
    auto* obj = ai::theObjects->GetEntityByObjId(m_barId);
    if (obj && obj->IsKindOf(&ai::Bar::m_classBar))
    {
        return static_cast<ai::Bar*>(obj);
    }
    return nullptr;
}

m3d::Object* BarDlg::CreateObject()
{
    return new BarDlg;
}

m3d::Class* BarDlg::GetBaseClass()
{
    return RT_CLASS_LOCAL(ChildPanel);
}

int BarDlg::SetUpForBar(int barId)
{
    m_barId = barId;
    m_lstNPCs->SetUpForBar(barId);
    return 1;
}

m3d::Class* BarDlg::GetClass() const
{
    return RT_CLASS_LOCAL(BarDlg);
}

BarDlg::~BarDlg()
{
    if ((m_gameDataFlags & 1) != 0)
    {
        m_barId = -1;
        m_lstNPCs->GameDataClear(false);
    }
    // m_aif and the Wnd base are destroyed by the compiler-chained destructors.
}

BarDlg::BarDlg(BarDlg const&) : BarDlg()
{
}

BarDlg::BarDlg()
{
    m_barId = -1;
    m_lstNPCs = nullptr;
}

int BarDlg::OnWndNotify(m3d::ui::Wnd* from, unsigned id, unsigned msg, m3d::AIParam const& data)
{
    if ((m_gameDataFlags & 2) == 0)
    {
        return 0;
    }
    if (ChildPanel::OnWndNotify(from, id, msg, data))
    {
        return 1;
    }
    if (id == 200000 && help::IsWndValid(from) && from->IsKindOf(RT_CLASS_LOCAL(NpcButton)) && msg == 21)
    {
        OnBeginConversation(static_cast<NpcButton*>(from)->GetNpc());
        return 1;
    }
    return 0;
}

void BarDlg::FullUpdate()
{
}

int BarDlg::GameDataSetup()
{
    if ((m_gameDataFlags & 2) == 0)
    {
        auto* child = GetChildByName(m_aif.m_lstNPCsPatternName);
        if (child && child->IsKindOf(&m3d::ui::TextBoxWnd::m_classTextBoxWnd))
        {
            m_lstNPCs = static_cast<NpcList*>(M3D_KERNEL->New("NpcList"));
            if (m_lstNPCs)
            {
                if (m_lstNPCs->CreateFromPattern(static_cast<m3d::ui::Wnd*>(child), true))
                {
                    m_gameDataFlags |= 1u;
                }
                else
                {
                    M3D_LOG_INFO(
                        "Make control error: cannot create " + m_aif.m_lstNPCsPatternName + " from pattern class");
                }
            }
            else
            {
                M3D_LOG_INFO("Make control error: cannot create " + m_aif.m_lstNPCsPatternName +
                             " - cannot find rtti class NpcList");
            }
        }
        else
        {
            M3D_LOG_INFO(
                "Make control error: control " + m_aif.m_lstNPCsPatternName + " is not found or incorrect type");
        }
    }

    if ((m_gameDataFlags & 1) != 0)
    {
        return 1;
    }
    M3D_LOG_INFO("BarDlg: error - fail to init because of a bad resource");
    return 0;
}

int BarDlg::GameDataClear(bool beforeContinuousLevel)
{
    if ((m_gameDataFlags & 1) == 0)
    {
        return 0;
    }
    m_barId = -1;
    m_lstNPCs->GameDataClear(beforeContinuousLevel);
    return 1;
}

int BarDlg::OnAfterRemoveFromWndStation()
{
    int const res = Wnd::OnAfterRemoveFromWndStation();
    M3D_APP->m_pInterfaceManager->LaunchEvent(105, GUI_EVENT_CUSTOM, nullptr);
    return res;
}

int BarDlg::OnBeginConversation(ai::Npc* npc)
{
    if (!npc)
    {
        return 0;
    }
    M3D_APP->EnqueueMessage(UM_BAR_NPC, npc->GetId(), 0, 0, 0, {}, {});
    return 1;
}

int BarDlg::OnAfterAddToWndStation()
{
    int const res = Wnd::OnAfterAddToWndStation();
    M3D_APP->EnqueueMessage(65691, 0, 0, 0, 0, {}, {});
    return res;
}

int BarDlg::OnBeforeAddToWndStation()
{
    M3D_APP->m_pInterfaceManager->LaunchEvent(104, GUI_EVENT_CUSTOM, reinterpret_cast<void*>(m_barId));
    return Wnd::OnBeforeAddToWndStation();
}
