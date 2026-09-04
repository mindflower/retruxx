#include "talkwithnpcdlg.h"

#include <algorithm>

#include <core/kernel.h>
#include <core/log.h>

#include <game/m3dgame.h>
#include <game/uimanager/truxxuimanager.h>
#include <game/uimisc/reply.h>
#include <game/uiwindows/charwindows/playermoneywnd.h>

#include <ui/image.h>
#include <ui/modelwnd.h>

#include "bardlg.h"
#include "conversationwnd.h"
#include "npcmodelwnd.h"
#include "towndlg.h"

#include <server/server.h>
#include <server/objects/bar.h>
#include <server/objects/location.h>
#include <server/objects/npc.h>
#include <server/objects/town.h>
#include <server/objects/base/objcontainer.h>

RT_CLASS_EXPORT_METHOD_DEFINE(TalkWithNpcDlg, Show)
{
    auto* dlg = static_cast<TalkWithNpcDlg*>(context->asObject(0, "TalkWithNpcDlg"));
    context->pushInt(dlg->Show(context->asInt(1)));
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(TalkWithNpcDlg, Hide)
{
    auto* dlg = static_cast<TalkWithNpcDlg*>(context->asObject(0, "TalkWithNpcDlg"));
    dlg->Hide(context->asBool(1));
    return 0;
}

RT_CLASS_EXPORTS_BEGIN(TalkWithNpcDlg)
	RT_CLASS_EXPORT(TalkWithNpcDlg, m3d::METHOD, Show, "", "", "")
	RT_CLASS_EXPORT(TalkWithNpcDlg, m3d::METHOD, Hide, "", "", "")
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(TalkWithNpcDlg);

// ===========================================================================
//  TalkWithNpcDlg::AuxInfo
// ===========================================================================

TalkWithNpcDlg::AuxInfo::AuxInfo()
{
    m_wndConversationName = "wndConversation";
    m_wndNpcImageName = "wndNpcImage";
    m_wndTopPanelName = "wndTopPanel";
    m_wndBottomPanelName = "wndBottomPanel";
    m_lblNpcNameName = "lblNpcName";
}

TalkWithNpcDlg::AuxInfo::AuxInfo(TalkWithNpcDlg::AuxInfo const& o)
{
    m_wndConversationName = o.m_wndConversationName;
    m_wndNpcImageName = o.m_wndNpcImageName;
    m_wndTopPanelName = o.m_wndTopPanelName;
    m_wndBottomPanelName = o.m_wndBottomPanelName;
    m_lblNpcNameName = o.m_lblNpcNameName;
}

// ===========================================================================
//  TalkWithNpcDlg
// ===========================================================================

TalkWithNpcDlg::TalkWithNpcDlg()
{
    m_wndTopPanel = nullptr;
    m_wndBottomPanel = nullptr;
    m_lblNpcName = nullptr;
    m_showType = SHOWTYPE_UNDEFINED;
    m_npcId = -1;
}

TalkWithNpcDlg::TalkWithNpcDlg(TalkWithNpcDlg const&) : TalkWithNpcDlg()
{
    // Matches the original: the copy ctor default-constructs the base, the
    // AuxInfo and null ref_ptrs. Reached only via Clone().
}

TalkWithNpcDlg::~TalkWithNpcDlg()
{
    // RVA 0xA0EA0
    TalkWithNpcDlg::GameDataClear(false);
    // The three child ref_ptrs release their references automatically.
}

m3d::Object* TalkWithNpcDlg::Clone()
{
    // RVA 0xA0A60
    return new TalkWithNpcDlg(*this);
}

m3d::Object* TalkWithNpcDlg::CreateObject()
{
    return new TalkWithNpcDlg;
}

m3d::Class* TalkWithNpcDlg::GetBaseClass()
{
    return RT_CLASS_LOCAL(ChildPanel);
}

m3d::Class* TalkWithNpcDlg::GetClass() const
{
    return RT_CLASS_LOCAL(TalkWithNpcDlg);
}

ai::Npc* TalkWithNpcDlg::GetNpc() const
{
    // RVA 0xA2DA0
    if (m_npcId == -1)
    {
        return nullptr;
    }
    ai::Obj* obj = ai::theObjects->GetEntityByObjId(m_npcId);
    if (obj && obj->IsKindOf(&ai::Npc::m_classNpc))
    {
        return static_cast<ai::Npc*>(obj);
    }
    return nullptr;
}

bool TalkWithNpcDlg::IsValid() const
{
    // RVA 0xA2050
    return (m_gameDataFlags & 1) != 0;
}

ai::Bar const* TalkWithNpcDlg::GetBar() const
{
    // RVA 0xA2880
    if (m_npcId == -1)
    {
        return nullptr;
    }
    ai::Npc* npc = GetNpc();
    if (!npc)
    {
        return nullptr;
    }
    ai::Obj* parent = npc->GetParent();
    if (parent && parent->IsKindOf(&ai::Bar::m_classBar))
    {
        return static_cast<ai::Bar const*>(parent);
    }
    return nullptr;
}

ref_ptr<TownDlg> TalkWithNpcDlg::GetTownDlg() const
{
    // RVA 0xA2650
    ref_ptr<m3d::ui::Wnd> w = M3D_APP->m_pInterfaceManager->GetWindow(4);
    if (!w || !w->IsKindOf(&TownDlg::m_classTownDlg))
    {
        return {};
    }
    return ref_ptr<TownDlg>(static_cast<TownDlg*>(w.get()));
}

ref_ptr<BarDlg> TalkWithNpcDlg::GetBarDlg() const
{
    // RVA 0xA26F0
    if (m_npcId == -1)
    {
        return {};
    }
    ai::Npc* npc = GetNpc();
    if (!npc)
    {
        return {};
    }
    ai::Obj* bar = npc->GetParent();
    if (!bar || !bar->IsKindOf(&ai::Bar::m_classBar))
    {
        return {};
    }

    int const barGuiIds[2] = {2, 3};
    for (int id : barGuiIds)
    {
        ref_ptr<m3d::ui::Wnd> w = M3D_APP->m_pInterfaceManager->GetWindow(id);
        if (w && w->IsKindOf(&BarDlg::m_classBarDlg))
        {
            auto* barDlg = static_cast<BarDlg*>(w.get());
            if (barDlg->GetBar() == bar)
            {
                return ref_ptr<BarDlg>(barDlg);
            }
        }
    }
    return {};
}

bool TalkWithNpcDlg::IsTownDlgShowingNow() const
{
    // RVA 0xA28C0
    ref_ptr<TownDlg> townDlg = GetTownDlg();
    return townDlg && townDlg->IsChildOf(static_cast<m3d::Object*>(M3D_APP));
}

bool TalkWithNpcDlg::IsBarDlgShowingNow() const
{
    // RVA 0xA2920
    ref_ptr<BarDlg> barDlg = GetBarDlg();
    return barDlg && barDlg->IsChildOf(static_cast<m3d::Object*>(M3D_APP));
}

int TalkWithNpcDlg::GetNpcToTalkWithFromLocation(int locationId) const
{
    // RVA 0xA2980
    if (locationId == -1)
    {
        return -1;
    }
    ai::Obj* obj = ai::theObjects->GetEntityByObjId(locationId);
    if (obj && obj->IsKindOf(&ai::Location::m_classLocation))
    {
        auto* location = static_cast<ai::Location*>(obj);
        for (ai::Npc* npc : location->GetNpcs())
        {
            if (!npc)
            {
                continue;
            }
            RepliesManager* replies = M3D_APP->m_pInterfaceManager->GetRepliesManager();
            if (replies->GetFirstNpcReply(npc))
            {
                return npc->GetId();
            }
        }
    }
    return -1;
}

int TalkWithNpcDlg::GameDataSetup()
{
    // RVA 0xA0F20
    if ((m_gameDataFlags & 2) == 0)
    {
        SetStyle(GetStyle() | 0x20u);

        m3d::Object* topPanel = GetChildByName(m_aif.m_wndTopPanelName);
        if (topPanel && topPanel->IsKindOf(&m3d::ui::Wnd::m_classWnd))
        {
            m_wndTopPanel = static_cast<m3d::ui::Wnd*>(topPanel);
        }
        else
        {
            M3D_LOG_INFO(
                CStr("Get control error: control ") + m_aif.m_wndTopPanelName + " is not found or incorrect type");
        }

        m3d::Object* bottomPanel = GetChildByName(m_aif.m_wndBottomPanelName);
        if (bottomPanel && bottomPanel->IsKindOf(&m3d::ui::Wnd::m_classWnd))
        {
            m_wndBottomPanel = static_cast<m3d::ui::Wnd*>(bottomPanel);
        }
        else
        {
            M3D_LOG_INFO(
                CStr("Get control error: control ") + m_aif.m_wndBottomPanelName + " is not found or incorrect type");
        }

        m3d::Object* npcName = GetChildByName(m_aif.m_lblNpcNameName);
        if (npcName && npcName->IsKindOf(&m3d::ui::Wnd::m_classWnd))
        {
            m_lblNpcName = static_cast<m3d::ui::Wnd*>(npcName);
        }
        else
        {
            M3D_LOG_INFO(
                CStr("Get control error: control ") + m_aif.m_lblNpcNameName + " is not found or incorrect type");
        }

        m3d::Object* npcImage = GetChildByName(m_aif.m_wndNpcImageName);
        if (npcImage && npcImage->IsKindOf(&m3d::ui::ModelWnd::m_classModelWnd))
        {
            m_wndNpcImage = static_cast<NpcModelWnd*>(M3D_KERNEL->New("NpcModelWnd"));
            if (!m_wndNpcImage || !m_wndNpcImage->CreateFromPattern(static_cast<m3d::ui::Wnd*>(npcImage), true))
            {
                M3D_LOG_INFO(
                    CStr("Make control error: cannot create ") + m_aif.m_wndNpcImageName + " from pattern class");
            }
        }
        else
        {
            M3D_LOG_INFO(
                CStr("Make control error: control ") + m_aif.m_wndNpcImageName + " is not found or incorrect type");
        }

        bool conversationOk = false;
        m3d::Object* conversation = GetChildByName(m_aif.m_wndConversationName);
        if (conversation && conversation->IsKindOf(&m3d::ui::Wnd::m_classWnd))
        {
            m_wndConversation = static_cast<ConversationWnd*>(M3D_KERNEL->New("ConversationWnd"));
            if (m_wndConversation &&
                m_wndConversation->CreateFromPattern(static_cast<m3d::ui::Wnd*>(conversation), true))
            {
                conversationOk = true;
            }
            else
            {
                M3D_LOG_INFO(
                    CStr("Make control error: cannot create ") + m_aif.m_wndConversationName + " from pattern class");
            }
        }
        else
        {
            M3D_LOG_INFO(
                CStr("Make control error: control ") + m_aif.m_wndConversationName + " is not found or incorrect type");
        }

        ref_ptr<m3d::ui::Wnd> money = M3D_APP->m_pInterfaceManager->GetWindow(71);
        if (money && money->IsKindOf(&PlayerMoneyWnd::m_classPlayerMoneyWnd))
        {
            m_wndPlayerMoney = ref_ptr<PlayerMoneyWnd>(static_cast<PlayerMoneyWnd*>(money.get()));
            AddChild(static_cast<m3d::Object*>(m_wndPlayerMoney.get()));
        }

        if (conversationOk && m_wndNpcImage && m_wndConversation && m_wndPlayerMoney)
        {
            M3D_APP->m_pInterfaceManager->AddWindowById(
                ref_ptr<m3d::ui::Wnd>(static_cast<m3d::ui::Wnd*>(m_wndConversation.get())), 37, true, false);
            M3D_APP->m_pInterfaceManager->AddWindowById(
                ref_ptr<m3d::ui::Wnd>(static_cast<m3d::ui::Wnd*>(m_wndNpcImage.get())), 87, true, false);
            m_wndConversation->SetStyle(m_wndConversation->GetStyle() | 0x100000u);
            FillPanels();
            m_gameDataFlags |= 1u;
        }
    }

    if ((m_gameDataFlags & 1) != 0)
    {
        return 1;
    }
    M3D_LOG_INFO("TalkWithNpcDlg: error - fail to init because of a bad resource");
    return 0;
}

int TalkWithNpcDlg::GameDataClear(bool beforeContinuousLevel)
{
    // RVA 0xA1F20
    if ((m_gameDataFlags & 1) == 0)
    {
        return 0;
    }
    m_npcId = -1;
    m_wndConversation->GameDataClear(beforeContinuousLevel);
    static_cast<m3d::ui::Wnd*>(m_wndNpcImage.get())->GameDataClear(beforeContinuousLevel);
    m_lblNpcName->SetText(CStr());
    return 1;
}

int TalkWithNpcDlg::GameDataUpdate(void* data, int dataType)
{
    // RVA 0xA1FF0
    if ((m_gameDataFlags & 1) == 0)
    {
        return 0;
    }
    if (dataType == 25)
    {
        if (data)
        {
            m_showType = SHOWTYPE_IN_BAR;
            SetupForNpc(reinterpret_cast<int*>(data)[13]);
        }
    }
    else if (dataType == 70)
    {
        OnLocationNpc(data);
        return 1;
    }
    return 1;
}

void TalkWithNpcDlg::OnLocationNpc(void* data)
{
    // RVA 0xA2110
    if (!data)
    {
        return;
    }
    int const npcId = GetNpcToTalkWithFromLocation(reinterpret_cast<int*>(data)[13]);
    if (npcId == -1)
    {
        Hide(false);
        return;
    }
    m_showType = SHOWTYPE_IN_FIELD;
    SetupForNpc(npcId);
}

void TalkWithNpcDlg::OnBarNpc(void* data)
{
    // RVA 0xA2150
    if (!data)
    {
        return;
    }
    m_showType = SHOWTYPE_IN_BAR;
    SetupForNpc(reinterpret_cast<int*>(data)[13]);
}

int TalkWithNpcDlg::Show(int npcId)
{
    // RVA 0xA2060
    bool const townShowing = IsTownDlgShowingNow();
    bool const barShowing = IsBarDlgShowingNow();

    bool inTown = true;
    if (!townShowing)
    {
        if (!IsChildOf(static_cast<m3d::Object*>(M3D_APP)) || m_showType != SHOWTYPE_IN_TOWN)
        {
            inTown = false;
        }
    }

    bool inBar = true;
    if (!barShowing)
    {
        if (!IsChildOf(static_cast<m3d::Object*>(M3D_APP)) || m_showType != SHOWTYPE_IN_BAR)
        {
            inBar = false;
        }
    }

    if (inTown)
    {
        m_showType = SHOWTYPE_IN_TOWN;
    }
    else
    {
        m_showType = static_cast<ShowType>(2 - (inBar ? 1 : 0));
    }
    return SetupForNpc(npcId);
}

int TalkWithNpcDlg::SetupForNpc(int npcId)
{
    // RVA 0xA2170
    if ((m_gameDataFlags & 1) == 0)
    {
        M3D_LOG_INFO("TalkWithNpcDlg::SetupForNpc error - the TalkWithNpcDlg was not initialised properly");
        return 0;
    }
    if (npcId == -1)
    {
        M3D_LOG_INFO("TalkWithNpcDlg::SetupForNpc error - invalid npcId");
        return 0;
    }

    ai::Obj* obj = ai::theObjects->GetEntityByObjId(npcId);
    if (obj && obj->IsKindOf(&ai::Npc::m_classNpc))
    {
        auto* npc = static_cast<ai::Npc*>(obj);
        GameDataClear(false);
        m_npcId = npcId;
        M3D_APP->Pause();
        m_wndConversation->StartConversation(npc->GetId());
        return 1;
    }

    M3D_LOG_INFO(
        CStr("TalkWithNpcDlg::SetupForNpc error - NPC with id ") + CStr(npcId) + " does not exist");
    return 0;
}

void TalkWithNpcDlg::Hide(bool bLeaveTown)
{
    // RVA 0xA23A0
    if ((m_gameDataFlags & 1) == 0)
    {
        return;
    }
    if (!IsChildOf(static_cast<m3d::Object*>(M3D_APP)))
    {
        return;
    }

    unsigned int const oldShowType = m_showType;
    m_showType = SHOWTYPE_UNDEFINED;

    if (bLeaveTown)
    {
        int const recipient = (oldShowType < 2) ? 4 : GetGuiId();
        M3D_APP->EnqueueMessage(65673, recipient, 0, 0, 0, {}, {});
        return;
    }

    switch (oldShowType)
    {
    case SHOWTYPE_IN_TOWN:
    {
        ref_ptr<TownDlg> townDlg = GetTownDlg();
        if (townDlg)
        {
            if (ai::Town* town = townDlg->GetTown())
            {
                M3D_APP->EnqueueMessage(66540, town->GetId(), 0, 0, 0, {}, {});
            }
        }
        break;
    }
    case SHOWTYPE_IN_BAR:
        if (ai::Bar const* bar = GetBar())
        {
            M3D_APP->EnqueueMessage(65657, bar->GetId(), 0, 0, 0, {}, {});
        }
        break;
    case SHOWTYPE_IN_FIELD:
        M3D_APP->EnqueueMessage(65673, GetGuiId(), 0, 0, 0, {}, {});
        break;
    default:
        break;
    }
}

void TalkWithNpcDlg::UpdateOnStartConversation()
{
    // RVA 0xA2AD0
    if (!Valid())
    {
        return;
    }

    m_wndNpcImage->SetUpForNpc(m_npcId);
    static_cast<m3d::ui::ImageWnd*>(m_wndNpcImage.get())->SetImage(GetNpcBg(m_npcId));
    m_lblNpcName->SetText(ai::pServer->GetFullNameByObjID(m_npcId));

    if (!IsChildOf(static_cast<m3d::Object*>(M3D_APP)))
    {
        M3D_APP->EnqueueMessage(65674, GetGuiId(), 0, 0, 0, {}, {});
    }
}

void TalkWithNpcDlg::FillPanels()
{
    // RVA 0xA2C40
    if (!m_wndTopPanel || !m_wndBottomPanel)
    {
        return;
    }

    std::vector<m3d::ui::Wnd*> topPanelChilds;
    topPanelChilds.push_back(static_cast<m3d::ui::Wnd*>(m_wndNpcImage.get()));
    topPanelChilds.push_back(m_lblNpcName);

    for (m3d::Object* raw = GetFirstChild(); raw;)
    {
        m3d::Object* next = raw->GetNextSibling();
        auto* child = RT_DYNCAST(raw, m3d::ui::Wnd);
        if (child && child != m_wndTopPanel && child != m_wndBottomPanel)
        {
            m3d::ui::Wnd* dst = m_wndBottomPanel;
            if (std::find(topPanelChilds.begin(), topPanelChilds.end(), child) != topPanelChilds.end())
            {
                dst = m_wndTopPanel;
            }

            RemoveChild(child);
            dst->AddChild(child);
            dst->MoveChildToFirstPosition(child);

            BoundsBase<float> b = child->GetBounds();
            BoundsBase<float> const dstB = dst->GetBounds();
            b.x0 -= dstB.x0;
            b.y0 -= dstB.y0;
            child->SetBounds(b, true);
        }
        raw = next;
    }
}

m3d::rend::TexHandle TalkWithNpcDlg::GetNpcBg(int npcId) const
{
    // RVA 0xA3000
    if (npcId < 0)
    {
        return m3d::rend::TexHandle();
    }
    ai::Obj* obj = ai::theObjects->GetEntityByObjId(npcId);
    if (!obj || !obj->IsKindOf(&ai::Npc::m_classNpc))
    {
        return m3d::rend::TexHandle();
    }
    auto* npc = static_cast<ai::Npc*>(obj);

    m3d::rend::TexHandle bgTex = M3D_APP->m_pInterfaceManager->GetIcoByName(npc->GetName(), 0);
    if (bgTex.IsValid())
    {
        return bgTex;
    }

    ai::Obj* parent = npc->GetParent();
    if (!parent)
    {
        return m3d::rend::TexHandle();
    }

    bgTex = M3D_APP->m_pInterfaceManager->GetIcoByName(parent->GetName(), 0);
    if (bgTex.IsValid())
    {
        return bgTex;
    }

    if (parent->IsKindOf(&ai::Bar::m_classBar))
    {
        return M3D_APP->m_pInterfaceManager->GetIcoByName("NpcBgBar", 0);
    }
    if (parent->IsKindOf(&ai::Location::m_classLocation))
    {
        return M3D_APP->m_pInterfaceManager->GetIcoByName("NpcBgLocation", 0);
    }
    return m3d::rend::TexHandle();
}

int TalkWithNpcDlg::OnWndNotify(m3d::ui::Wnd* from, unsigned id, unsigned msg, m3d::AIParam const& data)
{
    // RVA 0xA2A40
    if ((m_gameDataFlags & 1) == 0)
    {
        return 0;
    }
    if (ChildPanel::OnWndNotify(from, id, msg, data))
    {
        return 1;
    }
    if (id != 500000)
    {
        return 0;
    }
    switch (msg)
    {
    case 0xC364u:  // 50020
        Hide(false);
        break;
    case 0xC365u:  // 50021
        UpdateOnStartConversation();
        return 1;
    case 0xC366u:  // 50022
        Hide(true);
        return 1;
    }
    return 1;
}

int TalkWithNpcDlg::OnActivate(bool on)
{
    // RVA 0xA2EC0 - a thunk straight to the base.
    return m3d::ui::Wnd::OnActivate(on);
}

int TalkWithNpcDlg::OnAfterAddToWndStation()
{
    // RVA 0xA2ED0
    int const res = m3d::ui::Wnd::OnAfterAddToWndStation();
    if (ai::Npc* npc = GetNpc())
    {
        M3D_APP->EnqueueMessage(65686, npc->GetBelong(), 0, 0, 0, {}, {});
    }
    M3D_APP->EnqueueMessage(65691, 0, 0, 0, 0, {}, {});
    return res;
}

void TalkWithNpcDlg::OnExit()
{
    // RVA 0xA2E00
    ref_ptr<BarDlg> barDlg = GetBarDlg();
    if (barDlg)
    {
        M3D_APP->EnqueueMessage(65674, barDlg->GetGuiId(), 0, 0, 0, {}, {});
    }
}
