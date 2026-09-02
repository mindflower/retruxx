#include "npclist.h"
#include "npcmodelwnd.h"

#include <vector>

#include <core/aiparam.h>
#include <core/kernel.h>
#include <core/log.h>
#include <i_event.h>

#include <game/m3dgame.h>
#include <game/uimisc/guihelper.h>
#include <game/uimisc/reply.h>

#include <ui/button.h>
#include <ui/image.h>
#include <ui/modelwnd.h>
#include <ui/wndstation.h>

#include <server/server.h>
#include <server/objects/bar.h>
#include <server/objects/npc.h>
#include <server/objects/base/objcontainer.h>

// ===========================================================================
//  NpcButton
// ===========================================================================

RT_CLASS_EXPORTS_BEGIN(NpcButton)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(NpcButton);

NpcButton::AuxInfo NpcButton::m_aif;
NpcButton::Pattern NpcButton::m_pattern;

namespace
{
    // Copies every layout / text / style / pane / animation property from a
    // pattern window onto another window (the shipped NpcButton pattern-copy
    // inlines this block once per widget).
    void CopyWndPropsFromPattern(m3d::ui::Wnd* dst, m3d::ui::Wnd const* pat)
    {
        dst->SetStyle(pat->GetStyle());
        dst->SetText(pat->GetText());
        dst->SetId(pat->GetId());
        dst->SetName(pat->GetName());
        dst->SetBounds(pat->GetBounds(), true);
        dst->SetDefaultFont(pat->GetDefaultFont());
        dst->SetWrapMode(pat->GetWrapMode());
        dst->SetFormatMode(pat->GetFormatMode());
        dst->SetColor(pat->GetColor());
        dst->SetTextColor(pat->GetTextColor());
        dst->SetTextColorDisabled(pat->GetTextColorDisabled());
        dst->SetClientEdges(pat->GetClientEdges());
        dst->SetPane(pat->GetPaneName());
        dst->SetPaneFlags(pat->GetPaneFlags());
        dst->SetScrollPane(pat->GetScrollPaneName());
        dst->SetBackground(pat->GetBackground());

        CStr tooltip;
        pat->GetProperty(PROP_WND_TOOLTIP, &tooltip);
        dst->SetProperty(PROP_WND_TOOLTIP, &tooltip);

        dst->SetOnShowAnimation(pat->GetOnShowAnimation());
        dst->SetOnHideAnimation(pat->GetOnHideAnimation());
    }

    // Grabs a named child of `pattern`, verifies its rtti class, unlinks it and
    // (on first load) rebases its origin relative to `anchor`. Returns null (and
    // logs) when the child is missing or of the wrong type.
    m3d::ui::Wnd* GrabPatternChild(
        m3d::ui::Wnd* pattern,
        CStr const& name,
        m3d::Class const* kind,
        m3d::ui::Wnd const* anchor,
        bool firstLoad)
    {
        m3d::Object* c = pattern->GetChildByName(name);
        if (!c || !c->IsKindOf(kind))
        {
            M3D_LOG_INFO(CStr("NpcButton::LoadPattern error - cannot find child wnd ") + name);
            return nullptr;
        }
        auto* w = static_cast<m3d::ui::Wnd*>(c);
        if (!firstLoad)
        {
            w->DecRef();
            return w;
        }
        pattern->RemoveChild(w);
        if (anchor)
        {
            BoundsBase<float> b = w->GetBounds();
            BoundsBase<float> base = anchor->GetBounds();
            b.x0 -= base.x0;
            b.y0 -= base.y0;
            w->SetBounds(b, true);
        }
        return w;
    }
}  // namespace

NpcButton::AuxInfo::AuxInfo()
{
    m_wndPatternName = "wndNpcButton";
    m_wndIcoName = "wndNpcIco";
    m_wndIcoBgName = "wndNpcIcoBg";
    m_btnNameName = "btnNpcName";
    m_wndNameBgName = "wndNpcNameBg";
}

NpcButton::AuxInfo::AuxInfo(NpcButton::AuxInfo const& o)
{
    m_wndPatternName = o.m_wndPatternName;
    m_wndIcoName = o.m_wndIcoName;
    m_wndIcoBgName = o.m_wndIcoBgName;
    m_btnNameName = o.m_btnNameName;
    m_wndNameBgName = o.m_wndNameBgName;
}

NpcButton::Pattern::Pattern()
{
    m_wndPattern = nullptr;
    m_wndPatternIco = nullptr;
    m_wndPatternIcoBg = nullptr;
    m_btnPatternName = nullptr;
    m_wndPatternNameBg = nullptr;
    m_numRefs = 0;
}

NpcButton::NpcButton()
{
    m_npcId = -1;
    m_wndNpcIcoBg = nullptr;
    m_btnNpcName = nullptr;
    m_wndNpcNameBg = nullptr;
}

NpcButton::NpcButton(NpcButton const&) : NpcButton()
{
    // Matches the original: the copy ctor default-constructs the base and only
    // nulls m_wndNpcIco; nothing else is copied. Reached only via Clone().
}

NpcButton::~NpcButton()
{
    if (m_wndNpcIco)
    {
        M3D_APP->m_pInterfaceManager->RemoveWindow(m_wndNpcIco->GetGuiId());
    }
    // m_wndNpcIco releases its reference automatically.
}

m3d::Class* NpcButton::GetBaseClass()
{
    return RT_CLASS_LOCAL(Wnd);
}

m3d::Class* NpcButton::GetClass() const
{
    return RT_CLASS_LOCAL(NpcButton);
}

m3d::Object* NpcButton::CreateObject()
{
    return new NpcButton();
}

m3d::Object* NpcButton::Clone()
{
    return new NpcButton(*this);
}

ai::Npc* NpcButton::GetNpc() const
{
    if (m_npcId == -1)
    {
        return nullptr;
    }
    ai::Obj* o = ai::theObjects->GetEntityByObjId(m_npcId);
    return (o && o->IsKindOf(&ai::Npc::m_classNpc)) ? static_cast<ai::Npc*>(o) : nullptr;
}

CStr NpcButton::GetNameForNpc(ai::Npc const* npc) const
{
    if (!npc)
    {
        return CStr();
    }
    return ai::pServer->GetFullNameByObjID(npc->GetId());
}

int NpcButton::LoadPattern(m3d::ui::Wnd* pattern)
{
    ++m_pattern.m_numRefs;
    bool const firstLoad = m_pattern.m_numRefs <= 1;

    if (!pattern)
    {
        M3D_LOG_INFO("NpcButton::CreateFromPattern - error to create - invalid pattern wnd");
        return 0;
    }

    m3d::Object* c = pattern->GetChildByName(m_aif.m_wndPatternName);
    if (!c || !c->IsKindOf(&m3d::ui::Wnd::m_classWnd))
    {
        M3D_LOG_INFO("NpcButton::CreateFromPattern - error to create - invalid pattern wnd");
        return 0;
    }
    auto* wndPattern = static_cast<m3d::ui::Wnd*>(c);
    if (!firstLoad)
    {
        wndPattern->DecRef();
    }
    else
    {
        pattern->RemoveChild(wndPattern);
        m_pattern.m_wndPattern = wndPattern;
    }

    auto* ico = GrabPatternChild(
        pattern, m_aif.m_wndIcoName, &m3d::ui::ModelWnd::m_classModelWnd, m_pattern.m_wndPattern, firstLoad);
    if (!ico)
    {
        return 0;
    }
    if (firstLoad)
    {
        m_pattern.m_wndPatternIco = static_cast<m3d::ui::ModelWnd*>(ico);
    }

    auto* icoBg = GrabPatternChild(
        pattern, m_aif.m_wndIcoBgName, &m3d::ui::ImageWnd::m_classImageWnd, m_pattern.m_wndPattern, firstLoad);
    if (!icoBg)
    {
        return 0;
    }
    if (firstLoad)
    {
        m_pattern.m_wndPatternIcoBg = static_cast<m3d::ui::ImageWnd*>(icoBg);
    }

    auto* btnName = GrabPatternChild(
        pattern, m_aif.m_btnNameName, &m3d::ui::ButtonWnd::m_classButtonWnd, m_pattern.m_wndPattern, firstLoad);
    if (!btnName)
    {
        return 0;
    }
    if (firstLoad)
    {
        m_pattern.m_btnPatternName = static_cast<m3d::ui::ButtonWnd*>(btnName);
    }

    auto* nameBg = GrabPatternChild(
        pattern, m_aif.m_wndNameBgName, &m3d::ui::ImageWnd::m_classImageWnd, m_pattern.m_wndPattern, firstLoad);
    if (!nameBg)
    {
        return 0;
    }
    if (firstLoad)
    {
        m_pattern.m_wndPatternNameBg = static_cast<m3d::ui::ImageWnd*>(nameBg);
        BoundsBase<float> b = m_pattern.m_wndPattern->GetBounds();
        b.x0 = 0.0f;
        b.y0 = 0.0f;
        m_pattern.m_wndPattern->SetBounds(b, true);
    }
    return 1;
}

void NpcButton::ClearPattern()
{
    if (--m_pattern.m_numRefs > 0)
    {
        return;
    }
    if (m_pattern.m_wndPattern)
    {
        m_pattern.m_wndPattern->DecRef();
    }
    m_pattern.m_wndPattern = nullptr;
    if (m_pattern.m_wndPatternIco)
    {
        m_pattern.m_wndPatternIco->DecRef();
    }
    m_pattern.m_wndPatternIco = nullptr;
    if (m_pattern.m_wndPatternIcoBg)
    {
        m_pattern.m_wndPatternIcoBg->DecRef();
    }
    m_pattern.m_wndPatternIcoBg = nullptr;
    if (m_pattern.m_btnPatternName)
    {
        m_pattern.m_btnPatternName->DecRef();
    }
    m_pattern.m_btnPatternName = nullptr;
    if (m_pattern.m_wndPatternNameBg)
    {
        m_pattern.m_wndPatternNameBg->DecRef();
    }
    m_pattern.m_wndPatternNameBg = nullptr;
}

int NpcButton::CreateFromPattern()
{
    if (Valid())
    {
        return 0;
    }

    m3d::ui::Wnd* pat = m_pattern.m_wndPattern;
    if (!pat || !M3D_APP->IsWndAlive(pat, -1) || !pat->Valid())
    {
        M3D_LOG_INFO("NpcButton::CreateFromPattern error - null pattern");
        return 0;
    }

    if (!m3d::ui::Wnd::Create(CStr(), pat->GetStyle(), pat->GetBounds(), pat->GetId()))
    {
        M3D_LOG_INFO("NpcButton::CreateFromPattern error - cannot create window");
        return 0;
    }

    CopyWndPropsFromPattern(this, pat);

    if (!CreateChildren())
    {
        M3D_LOG_INFO("NpcButton::CreateFromPattern error - cannot create children");
        return 0;
    }

    m_gameDataFlags |= 1u;
    return 1;
}

int NpcButton::CreateChildren()
{
    if (!help::IsWndValid(m_pattern.m_wndPatternIco) || !help::IsWndValid(m_pattern.m_wndPatternIcoBg) ||
        !help::IsWndValid(m_pattern.m_btnPatternName) || !help::IsWndValid(m_pattern.m_wndPatternNameBg))
    {
        return 0;
    }

    if (m_wndNpcIco || m_wndNpcIcoBg || m_btnNpcName || m_wndNpcNameBg)
    {
        M3D_LOG_INFO("NpcButton::CreateChildren error - children already exist");
        return 0;
    }

    // --- npc model icon ----------------------------------------------------
    m_wndNpcIco = static_cast<NpcModelWnd*>(M3D_KERNEL->New("NpcModelWnd"));
    if (!m_wndNpcIco)
    {
        return 0;
    }
    {
        m3d::ui::ModelWnd* patIco = m_pattern.m_wndPatternIco;
        if (!m_wndNpcIco->CreateModelWnd(
                patIco->GetImage(),
                patIco->GetStyle(),
                patIco->GetBounds(),
                patIco->GetId(),
                patIco->GetTargetTexture()))
        {
            m_wndNpcIco = nullptr;
            return 0;
        }
        CopyWndPropsFromPattern(m_wndNpcIco.get(), patIco);
        m_wndNpcIco->SetImage(patIco->GetImage());
        m_wndNpcIco->SetDefaultTranslation(patIco->Translation());
        m_wndNpcIco->SetGameDataFlags(m_wndNpcIco->GetGameDataFlags() | 1);
        AddChild(m_wndNpcIco.get());

        int npcIcoGuiId = -1;
        M3D_APP->m_pInterfaceManager->AddWindow(m_wndNpcIco.get(), npcIcoGuiId, false, true);
        std::vector<int> npcIcoEvents;
        npcIcoEvents.push_back(89);
        M3D_APP->m_pInterfaceManager->SetEventsForWindow(npcIcoGuiId, npcIcoEvents);
    }

    // --- icon background -------------------------------------------------
    m_wndNpcIcoBg = static_cast<m3d::ui::ImageWnd*>(M3D_KERNEL->New("ImageWnd"));
    if (!m_wndNpcIcoBg)
    {
        return 0;
    }
    {
        m3d::ui::ImageWnd* patIcoBg = m_pattern.m_wndPatternIcoBg;
        if (!m_wndNpcIcoBg->Create(CStr(), patIcoBg->GetStyle(), patIcoBg->GetBounds(), patIcoBg->GetId()))
        {
            m_wndNpcIcoBg->DecRef();
            m_wndNpcIcoBg = nullptr;
            return 0;
        }
        CopyWndPropsFromPattern(m_wndNpcIcoBg, patIcoBg);
        m_wndNpcIcoBg->SetImage(patIcoBg->GetImage());
        AddChild(m_wndNpcIcoBg);
    }

    // --- name button --------------------------------------------------------
    m_btnNpcName = static_cast<m3d::ui::ButtonWnd*>(M3D_KERNEL->New("ButtonWnd"));
    if (!m_btnNpcName)
    {
        return 0;
    }
    {
        m3d::ui::ButtonWnd* patBtn = m_pattern.m_btnPatternName;
        if (!m_btnNpcName->Create(CStr(), patBtn->GetStyle(), patBtn->GetBounds(), patBtn->GetId()))
        {
            m_btnNpcName->DecRef();
            m_btnNpcName = nullptr;
            return 0;
        }
        CopyWndPropsFromPattern(m_btnNpcName, patBtn);
        if (patBtn->IsImaged())
        {
            m_btnNpcName->SetImaged(
                patBtn->GetImageRegular(), patBtn->GetImageDown(), patBtn->GetImageIn(), m3d::rend::TexHandle{});
        }
        else
        {
            m_btnNpcName->SetRegular();
        }
        AddChild(m_btnNpcName);
    }

    // --- name background --------------------------------------------------
    m_wndNpcNameBg = static_cast<m3d::ui::ImageWnd*>(M3D_KERNEL->New("ImageWnd"));
    if (!m_wndNpcNameBg)
    {
        return 0;
    }
    {
        m3d::ui::ImageWnd* patNameBg = m_pattern.m_wndPatternNameBg;
        if (!m_wndNpcNameBg->Create(CStr(), patNameBg->GetStyle(), patNameBg->GetBounds(), patNameBg->GetId()))
        {
            m_wndNpcNameBg->DecRef();
            m_wndNpcNameBg = nullptr;
            return 0;
        }
        CopyWndPropsFromPattern(m_wndNpcNameBg, patNameBg);
        m_wndNpcNameBg->SetImage(patNameBg->GetImage());
        AddChild(m_wndNpcNameBg);
    }

    return 1;
}

int NpcButton::SetUpForNpc(ai::Npc const* npc)
{
    if (Valid() || !npc || !CreateFromPattern())
    {
        return 0;
    }

    m_npcId = npc->GetId();
    m_btnNpcName->SetText(ai::pServer->GetFullNameByObjID(npc->GetId()));
    M3D_ASSERT(m_wndNpcIco);
    m_wndNpcIco->SetUpForNpc(m_npcId);
    return 1;
}

int NpcButton::OnWndNotify(m3d::ui::Wnd* from, unsigned int id, unsigned int msg, m3d::AIParam const& data)
{
    if ((m_gameDataFlags & 1) == 0)
    {
        return 0;
    }
    if ((m_style & 0x100000) != 0)
    {
        ReflectChildNotifyToParent(from, id, msg, data);
    }
    if (id != 200100 || msg != 1)
    {
        return 0;
    }
    m3d::AIParam empty;
    CallParentNotify(0x15u, empty, true);
    return 1;
}

// ===========================================================================
//  NpcList
// ===========================================================================

RT_CLASS_EXPORTS_BEGIN(NpcList)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(NpcList);

NpcList::AuxInfo::AuxInfo()
{
    m_space = 14.0f;
}

NpcList::NpcList()
{
    m_barId = -1;
    m_drawFlags = 0;
    m_curSel = -1;
    if (m_clientEdges.size() > 2)
    {
        m_clientEdges[2] = 2.0f;
    }
}

NpcList::NpcList(NpcList const&) : NpcList()
{
    // Matches the original: the copy ctor default-constructs everything; nothing
    // is copied from the source. Reached only via Clone().
}

NpcList::~NpcList()
{
    m_barId = -1;
    RemoveAllItems();
    NpcButton::ClearPattern();
    // ListBoxWnd / Wnd release their own storage.
}

m3d::Class* NpcList::GetBaseClass()
{
    return RT_CLASS_LOCAL(Wnd);
}

m3d::Class* NpcList::GetClass() const
{
    return RT_CLASS_LOCAL(NpcList);
}

m3d::Object* NpcList::CreateObject()
{
    return new NpcList();
}

m3d::Object* NpcList::Clone()
{
    return new NpcList(*this);
}

ai::Bar* NpcList::GetBar() const
{
    if (m_barId == -1)
    {
        return nullptr;
    }
    ai::Obj* o = ai::theObjects->GetEntityByObjId(m_barId);
    return (o && o->IsKindOf(&ai::Bar::m_classBar)) ? static_cast<ai::Bar*>(o) : nullptr;
}

int NpcList::CreateFromPattern(m3d::ui::Wnd* patternWnd, bool deleteSrc)
{
    if (!patternWnd)
    {
        M3D_LOG_INFO("NpcList::CreateFromPattern error - null patternWnd");
        return 0;
    }

    if (!Create(patternWnd->GetBounds(), patternWnd->GetStyle(), patternWnd->GetId()))
    {
        M3D_LOG_INFO("NpcList::CreateFromPattern error - cannot create window");
        return 0;
    }

    CopyWndPropsFromPattern(this, patternWnd);
    m_drawFlags |= 5u;

    auto* parent = RT_DYNCAST(patternWnd->GetParent(), m3d::ui::Wnd);
    if (!parent)
    {
        M3D_LOG_INFO("NpcList::CreateFromPattern error - null parent for paternWnd");
        return 0;
    }
    parent->AddChild(this);

    if (!NpcButton::LoadPattern(parent))
    {
        M3D_LOG_INFO("NpcList::CreateFromPattern error - cannot load pattern for NpcButton");
        return 0;
    }

    // Trim our client width by the difference between our client area and the
    // grabbed button pattern's width so the buttons fit the list exactly.
    BoundsBase<float> clientB = GetClientBounds();
    BoundsBase<float> patB = NpcButton::m_pattern.m_wndPattern->GetBounds();
    BoundsBase<float> b = GetBounds();
    b.width -= (clientB.width - patB.width);
    SetBounds(b, true);

    if (deleteSrc)
    {
        parent->RemoveChild(patternWnd);
        patternWnd->DecRef();
    }

    m_gameDataFlags |= 1u;
    return 1;
}

int NpcList::GameDataClear(bool)
{
    m_barId = -1;
    RemoveAllItems();
    return 1;
}

int NpcList::SetUpForBar(int barId)
{
    m_barId = barId;
    if (IsChildOf(static_cast<m3d::Object*>(M3D_APP)))
    {
        CreateItems();
    }
    return GetBar() != nullptr;
}

int NpcList::CreateItems()
{
    RemoveAllItems();
    if ((m_gameDataFlags & 1) == 0)
    {
        return 0;
    }

    ai::Bar* bar = GetBar();
    if (!bar)
    {
        return 1;
    }

    int res = 1;
    RepliesManager* replies = M3D_APP->m_pInterfaceManager->GetRepliesManager();
    for (ai::Npc* npc : bar->GetNpcs())
    {
        if (!replies || !replies->GetFirstNpcReply(npc))
        {
            continue;
        }
        auto* btn = static_cast<NpcButton*>(M3D_KERNEL->New("NpcButton"));
        if (!btn)
        {
            return 0;
        }
        if (btn->SetUpForNpc(npc))
        {
            AddButton(btn);
        }
        else
        {
            btn->DecRef();
            res = 0;
        }
    }
    return res;
}

int NpcList::AddButton(NpcButton* btn)
{
    if (!btn)
    {
        return 0;
    }
    AddItem(btn);
    AddChild(btn);
    return 1;
}

void NpcList::FullUpdate()
{
    CreateItems();
}

void NpcList::AddNpcsToEncyclopaedia()
{
    for (int i = 0; i < static_cast<int>(m_items.size()); ++i)
    {
        NpcButton* btn = m_items[i].m_item;
        if (!btn)
        {
            continue;
        }
        ai::Npc* npc = btn->GetNpc();
        if (npc)
        {
            M3D_APP->EnqueueMessage(65686, npc->GetId(), 0, 0, 0, {}, {});
        }
    }
}

int NpcList::MeasureItem(int itemIdx, BoundsBase<float>& bounds) const
{
    NpcButton* btn = m_items[itemIdx].m_item;
    BoundsBase<float> b = btn->GetBounds();
    bounds.x0 = 0.0f;
    bounds.y0 = 0.0f;
    bounds.width = b.width;
    bounds.height = b.height + m_aif.m_space;
    return 1;
}

int NpcList::RenderItem(int itemIdx, PointBase<float> const& org, m3d::ui::DrawInfo const& di)
{
    NpcButton* btn = m_items[itemIdx].m_item;
    BoundsBase<float> b = btn->GetBounds();
    b.y0 = (di.m_clientRect.y0 - di.m_originalRect.y0) + org.y;
    btn->SetBounds(b, true);
    return 1;
}

int NpcList::DeleteItem(int itemIdx)
{
    NpcButton*& slot = m_items[itemIdx].m_item;
    if (slot)
    {
        slot->DecRef();
    }
    slot = nullptr;
    return 1;
}

int NpcList::CompareItem(int, int)
{
    return 0;
}

int NpcList::OnBeforeAddToWndStation()
{
    CreateItems();
    return m3d::ui::Wnd::OnBeforeAddToWndStation();
}

int NpcList::OnAfterAddToWndStation()
{
    int res = m3d::ui::Wnd::OnAfterAddToWndStation();
    AddNpcsToEncyclopaedia();
    return res;
}

int NpcList::OnAfterRemoveFromWndStation()
{
    int res = m3d::ui::Wnd::OnAfterRemoveFromWndStation();
    RemoveAllItems();
    return res;
}
