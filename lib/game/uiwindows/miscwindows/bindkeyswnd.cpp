#include "bindkeyswnd.h"

#include <core/kernel.h>
#include <core/log.h>
#include <core/timer.h>
#include <game/m3dgame.h>
#include <game/uimanager/truxxuimanager.h>
#include <i_event.h>
#include <impulses/i_impulses.h>
#include <m3dapp.h>
#include <ui/listbox.h>
#include <ui/wndstation.h>

namespace
{
    // Control ids the window and its rows notify under.
    unsigned const ID_BTN_LOAD_FROM_PROFILE = 700000;
    unsigned const ID_BTN_SAVE_TO_PROFILE = 700001;
    unsigned const ID_BTN_LOAD_DEFAULTS = 700002;
    unsigned const ID_KEY_SET_BUTTON = 700004;

    // CallParentNotify messages a KeySetButton raises on the list.
    unsigned const MSG_BINDING_READY = 21;
    unsigned const MSG_INPUT_MODE_ON = 22;
    unsigned const MSG_INPUT_MODE_OFF = 23;

    // Pseudo key ids the impulse layer uses for the mouse and the modifiers.
    int const KEYID_MOUSE0 = 256;
    int const KEYID_MOUSE1 = 257;
    int const KEYID_MOUSE2 = 258;
    int const KEYID_CTRL = 261;
    int const KEYID_ALT = 262;
    int const KEYID_SHIFT = 263;

    // At most this many keys make up one binding.
    int const MAX_KEYS_PER_BINDING = 3;

    // Only two key sets per impulse are shown, so a longer list is trimmed.
    unsigned const MAX_KEY_SETS_PER_IMPULSE = 2;

    // Bindings are always edited for game mode 0.
    int const BINDINGS_GAME_MODE = 0;

    // BindKeysList::BindUnbind actions.
    int const ACTION_BIND = 0;
    int const ACTION_UNBIND = 1;

    // The impulses the list offers for rebinding, in the order they are listed.
    // Data at 0xA07EB8.
    Impulse const l_impulsesForEdit[] = {
        IM_CAR_ACC,
        IM_CAR_BREAK,
        IM_CAR_HAND_BREAK,
        IM_CAR_LEFT,
        IM_CAR_RIGHT,
        IM_CAR_HORN,
        IM_CAR_LIGHTS,
        IM_CAR_GET_OUT_OF_DIFFICULT_PLACE,
        IM_CAR_FIRE_0,
        IM_CAR_FIRE_1,
        IM_CAR_FIRE_2,
        IM_CAR_FIRE_3,
        IM_CAR_FIRE_4,
        IM_CAR_FIRE_ALL,
        IM_RELOAD_WEAPON,
        IM_UI_INVENTORY,
        IM_UI_VEHICLE_INFO,
        IM_UI_MENUBOOK,
        IM_UI_QUESTLOG,
        IM_UI_JOURNAL,
        IM_UI_MAP,
        IM_UI_BAR,
        IM_UI_ADDITIONAL_BUILDING,
        IM_PAUSE,
        IM_QUICK_SAVE,
        IM_QUICK_LOAD,
        IM_UI_TOGGLE_INTERFACE,
        IM_UI_PICKUP_ALL,
        IM_UI_HELP,
    };

    int const NUM_IMPULSES_FOR_EDIT = sizeof(l_impulsesForEdit) / sizeof(l_impulsesForEdit[0]);

    // Taking a key over from one of these costs nothing: they are the generic
    // movement impulses the car controls alias anyway.
    bool IsFreelyReplaceable(Impulse imp)
    {
        return imp == IM_ERROR || imp == IM_FWD || imp == IM_BK || imp == IM_LEFT || imp == IM_RIGHT;
    }

    // The list shows at most two key sets per impulse; the shipped code drops
    // the leading ones so the last two survive.
    void TrimToShownKeySets(retruxx::vector<retruxx::vector<int>>& keys)
    {
        while (keys.size() > MAX_KEY_SETS_PER_IMPULSE)
        {
            keys.erase(keys.begin());
        }
    }
}  // namespace

RT_CLASS_EXPORTS_BEGIN(BindKeysWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(BindKeysWnd);

// ===========================================================================
//  BindKeysWnd::KeySetButton
// ===========================================================================

BindKeysWnd::KeySetButton::AuxInfo::AuxInfo()
{
    // RVA 0x4A5D40
    m_filler = "---";
    m_normColor = 0xFF404040u;
    m_twinkleColor = 0xF0FF0000u;
    m_twinkleTime = 300;
    m_selColor = 0xFFFFCC00u;
}

BindKeysWnd::KeySetButton::KeySetButton(BoundsBase<float>& rc)
{
    // RVA 0x4A7B80
    m_impId = IM_ERROR;
    m_isInInputMode = false;
    m_isAnotherInInputMode = false;
    m_lastTwinkleTime = m3d::g_Kernel->GetTimer().GetCurTimeUnscaled();
    Create(m_aif.m_filler,
           m3d::ui::WS_SEND_NOTIFY_MESSAGES | m3d::ui::WS_TEXT_CENTERED | m3d::ui::WS_IS_VISIBLE,
           rc,
           ID_KEY_SET_BUTTON);
    m_defFont = 1;
}

BindKeysWnd::KeySetButton::KeySetButton(BindKeysWnd::KeySetButton const&)
{
    // The shipped build never instantiates this; it exists only because the
    // declaration names it.
}

BindKeysWnd::KeySetButton::~KeySetButton()
{
    // RVA 0x4A7C80 - the two key vectors, the aux info and the ButtonWnd base
    // unwind on their own.
}

int BindKeysWnd::KeySetButton::SetUp(Impulse impId, retruxx::vector<int>& keySet)
{
    // RVA 0x4A7D30
    if (static_cast<unsigned>(impId) >= IM_NUM_IMPULSES)
    {
        return 0;
    }
    m_impId = impId;
    OnRebind(keySet);
    return 1;
}

void BindKeysWnd::KeySetButton::OnRebind(retruxx::vector<int> const& newKeys)
{
    // RVA 0x4A81F0 - leaves input mode, hands the focus back to the list and
    // adopts the new key set.
    m_isInInputMode = false;
    GetStation()->CaptureFocus(RT_DYNCAST(GetParent(), m3d::ui::Wnd));
    CallParentNotify(MSG_INPUT_MODE_OFF, {}, false);
    m_newKeySet.clear();
    m_keySet = newKeys;
    SetBindText();
    SetTextColor(m_aif.m_normColor);
}

void BindKeysWnd::KeySetButton::SetBindText()
{
    // RVA 0x4A8300 - "Ctrl + Shift + A"; an unbound slot shows the filler.
    CStr btnText;
    for (auto i = 0; i < static_cast<int>(m_keySet.size()); ++i)
    {
        CStr const keyName = M3D_APP->m_pImpulses->GetKeyNameById(m_keySet[i]);
        if (!keyName.empty())
        {
            btnText += CStr(i ? " + " : "") + M3D_APP->GetStringByStringId0(keyName);
        }
    }
    if (btnText.empty())
    {
        btnText = m_aif.m_filler;
    }
    SetText(btnText);
}

void BindKeysWnd::KeySetButton::OnAnotherInputOn()
{
    // RVA 0x4A82A0 - another button started capturing, so this one stops.
    m_isInInputMode = false;
    m_isAnotherInInputMode = true;
    SetTextColor(m_aif.m_normColor);
    m_newKeySet.clear();
}

void BindKeysWnd::KeySetButton::OnAnotherInputOff()
{
    // RVA 0x4A82F0
    m_isAnotherInInputMode = false;
}

int BindKeysWnd::KeySetButton::OnKey(unsigned short key, unsigned char scanCode, unsigned state)
{
    // RVA 0x4A7D60
    if (!m_isInInputMode)
    {
        return 1;
    }

    m_newKeySet.clear();
    switch (scanCode)
    {
    case m3d::KEY_LCONTROL:
    case m3d::KEY_RCONTROL:
        // A bare modifier only counts once it is released.
        if (!state)
        {
            m_newKeySet.push_back(KEYID_CTRL);
        }
        break;
    case m3d::KEY_LSHIFT:
    case m3d::KEY_RSHIFT:
        if (!state)
        {
            m_newKeySet.push_back(KEYID_SHIFT);
        }
        break;
    case m3d::KEY_LMENU:
    case m3d::KEY_RMENU:
        if (!state)
        {
            m_newKeySet.push_back(KEYID_ALT);
        }
        break;
    default:
        m_newKeySet.push_back(scanCode);
        // NOTE: only the left shift is recognised as a chord modifier here -
        // unlike Ctrl and Alt, whose right-hand keys are both tested.
        if ((key & m3d::KBD_LSHIFT) != 0 && !m_newKeySet.empty())
        {
            m_newKeySet.push_back(KEYID_SHIFT);
        }
        if ((key & (m3d::KBD_LALT | m3d::KBD_RALT)) != 0 && !m_newKeySet.empty())
        {
            m_newKeySet.push_back(KEYID_ALT);
        }
        if ((key & (m3d::KBD_LCTRL | m3d::KBD_RCTRL)) != 0 && !m_newKeySet.empty())
        {
            m_newKeySet.push_back(KEYID_CTRL);
        }
        if (static_cast<int>(m_newKeySet.size()) > MAX_KEYS_PER_BINDING && !m_newKeySet.empty())
        {
            m_newKeySet.pop_back();
        }
        break;
    }

    if (!m_newKeySet.empty())
    {
        CallParentNotify(MSG_BINDING_READY, {}, false);
    }
    return 1;
}

int BindKeysWnd::KeySetButton::OnMouseButton0(unsigned state, PointBase<float> const& at)
{
    // RVA 0x4A8010 - the first press starts capturing, a press while capturing
    // records the left mouse button itself.
    if (state)
    {
        if (!m_isInInputMode)
        {
            m_isInInputMode = true;
            m_isAnotherInInputMode = false;
            GetStation()->CaptureFocus(this);
            CallParentNotify(MSG_INPUT_MODE_ON, {}, false);
            return 1;
        }
        m_newKeySet.clear();
        m_newKeySet.push_back(KEYID_MOUSE0);
    }
    return ButtonWnd::OnMouseButton0(state, at);
}

int BindKeysWnd::KeySetButton::OnMouseButton1(unsigned state, PointBase<float> const& at)
{
    // RVA 0x4A80F0
    if (state)
    {
        m_newKeySet.clear();
        m_newKeySet.push_back(KEYID_MOUSE1);
    }
    return Wnd::OnMouseButton1(state, at);
}

int BindKeysWnd::KeySetButton::OnMouseButton2(unsigned state, PointBase<float> const& at)
{
    // RVA 0x4A8160
    if (state)
    {
        m_newKeySet.clear();
        m_newKeySet.push_back(KEYID_MOUSE2);
    }
    return Wnd::OnMouseButton2(state, at);
}

int BindKeysWnd::KeySetButton::OnMouseWheel(int ticks, PointBase<float> const& at)
{
    // RVA 0x4A81D0 - while capturing the wheel is swallowed; otherwise it
    // scrolls the list this button sits in.
    if (m_isInInputMode)
    {
        return 1;
    }
    // The shipped code goes through the parent's vtable slot whatever the
    // parent is; in practice a key-set button only ever lives in a BindKeysList.
    auto* parent = RT_DYNCAST(GetParent(), BindKeysWnd::BindKeysList);
    if (!parent)
    {
        return 1;
    }
    return parent->OnMouseWheel(ticks, at);
}

void BindKeysWnd::KeySetButton::OnNcPaint(m3d::ui::DrawInfo const& di, unsigned)
{
    // RVA 0x4A84A0 - a hovered or capturing button gets a solid highlight; the
    // base frame is deliberately not drawn.
    if ((m_isInside || m_isInInputMode) && !m_isAnotherInInputMode)
    {
        auto const b = GetBounds();
        BoundsBase<float> const rect{0.0f, 0.0f, b.width, b.height};
        GetGfxServer()->AddFlatAxialQuad(di, rect, m_aif.m_selColor);
    }
}

void BindKeysWnd::KeySetButton::DrawWndText(m3d::ui::DrawInfo const& di)
{
    // RVA 0x4A8520 - a capturing button blinks between its normal and its
    // twinkle colour.
    if (m_isInInputMode)
    {
        auto const now = m3d::g_Kernel->GetTimer().GetCurTimeUnscaled();
        if (now - m_lastTwinkleTime >= m_aif.m_twinkleTime)
        {
            auto color = m_aif.m_normColor;
            if (m_textColor == color)
            {
                color = m_aif.m_twinkleColor;
            }
            SetTextColor(color);
            m_lastTwinkleTime = now;
        }
    }
    Wnd::DrawWndText(di);
}

// ===========================================================================
//  BindKeysWnd::BindKeysItem
// ===========================================================================

BindKeysWnd::BindKeysItem::AuxInfo::AuxInfo()
{
    // RVA 0x4A5CC0
    m_impWPerc = 48.8f;
    m_itemW = 0.0f;
    m_itemH = 32.0f;
    m_lblColor = 0xFF404040u;
    m_texIdBg = "BindKeysLineBg";
}

BindKeysWnd::BindKeysItem::BindKeysItem(float width)
{
    // RVA 0x4A76A0
    m_btns[0] = nullptr;
    m_btns[1] = nullptr;
    m_bg = nullptr;
    m_impLabel = nullptr;
    m_impId = IM_ERROR;
    m_aif.m_itemW = width;
}

BindKeysWnd::BindKeysItem::BindKeysItem(BindKeysWnd::BindKeysItem const&)
{
    // The shipped build never instantiates this; it exists only because the
    // declaration names it.
}

BindKeysWnd::BindKeysItem::~BindKeysItem()
{
    // RVA 0x4A7700 - detach and delete the row's child widgets.
    for (auto*& btn : m_btns)
    {
        if (btn)
        {
            if (auto* parent = btn->GetParent())
            {
                parent->RemoveChild(btn);
            }
            delete btn;
            btn = nullptr;
        }
    }
    if (m_bg)
    {
        if (auto* parent = m_bg->GetParent())
        {
            parent->RemoveChild(m_bg);
        }
        delete m_bg;
        m_bg = nullptr;
    }
    if (m_impLabel)
    {
        if (auto* parent = m_impLabel->GetParent())
        {
            parent->RemoveChild(m_impLabel);
        }
        delete m_impLabel;
        m_impLabel = nullptr;
    }
}

int BindKeysWnd::BindKeysItem::SetUp(Impulse impId, retruxx::vector<retruxx::vector<int>> keys, m3d::Object* parent,
                                     int idx)
{
    // RVA 0x4A77B0 - builds the row: an impulse label on the left and two
    // key-set buttons sharing the rest of the width.
    if (static_cast<unsigned>(impId) >= IM_NUM_IMPULSES)
    {
        return 0;
    }

    auto res = 1;
    float const impW = m_aif.m_impWPerc * m_aif.m_itemW * 0.01f;
    m_impId = impId;

    for (auto i = 0; i < 2; ++i)
    {
        float const btnW = (m_aif.m_itemW - impW) * 0.5f;
        float const x0 = i * btnW + impW;
        BoundsBase<float> rc{x0, 0.0f, btnW, m_aif.m_itemH};
        m_btns[i] = new KeySetButton(rc);

        m_btns[i]->m_impId = impId;
        if (i >= static_cast<int>(keys.size()))
        {
            m_btns[i]->OnRebind(retruxx::vector<int>());
        }
        else
        {
            m_btns[i]->OnRebind(keys[i]);
        }

        if (parent)
        {
            parent->AddChild(m_btns[i]);
        }
        else
        {
            res = 0;
        }
    }

    m_impLabel = RT_DYNCAST(m3d::g_Kernel->New("Wnd"), m3d::ui::Wnd);
    if (m_impLabel)
    {
        CStr const impName = M3D_APP->m_pImpulses->GetImpulseNameById(impId);
        CStr const impFullName = M3D_APP->GetStringByStringId0(impName);
        BoundsBase<float> const impB{0.0f, 0.0f, impW, m_aif.m_itemH};
        if (!m_impLabel->Create(impFullName,
                                m3d::ui::WS_NOFRAME | m3d::ui::WS_TRANSPARENT | m3d::ui::WS_IS_VISIBLE |
                                    m3d::ui::WS_TEXT_CENTERED_Y,
                                impB,
                                0))
        {
            res = 0;
        }
        m_impLabel->SetTextColor(m_aif.m_lblColor);
        m_impLabel->SetDefaultFont(1);
        if (parent)
        {
            parent->AddChild(m_impLabel);
        }
        else
        {
            res = 0;
        }
    }
    else
    {
        res = 0;
    }

    m_bg = RT_DYNCAST(m3d::g_Kernel->New("Wnd"), m3d::ui::Wnd);
    if (!m_bg)
    {
        return 0;
    }

    BoundsBase<float> const bgB{0.0f, 0.0f, m_aif.m_itemW, m_aif.m_itemH};
    if (!m_bg->Create(CStr(), m3d::ui::WS_TRANSPARENT | m3d::ui::WS_IS_VISIBLE, bgB, 0))
    {
        res = 0;
    }
    else
    {
        m_bg->SetPaneFlags(0);
        // Every other row gets a striped background.
        if (idx % 2)
        {
            m_bg->SetBackground(M3D_APP->m_pInterfaceManager->GetIcoByName(m_aif.m_texIdBg, false));
        }
    }

    if (parent)
    {
        parent->AddChild(m_bg);
    }
    else
    {
        res = 0;
    }
    return res;
}

// ===========================================================================
//  BindKeysWnd::BindKeysList
// ===========================================================================

BindKeysWnd::BindKeysList::AuxInfo::AuxInfo()
{
    // RVA 0x4A5DB0
    m_strBindReserved = "BINDKEYS_MSG_RESERVED";
    m_strBindExists = "BINDKEYS_MSG_EXISTS";
    m_strOnlyBindExists = "BINDKEYS_MSG_EXISTS_ONLY";
}

BindKeysWnd::BindKeysList::BindKeysList()
{
    // RVA 0x4A6580
    m_drawFlags = 0;
    m_curSel = -1;
    m_clientEdges[2] = 2.0;
}

BindKeysWnd::BindKeysList::BindKeysList(BindKeysWnd::BindKeysList const&)
{
    // The shipped build never instantiates this; it exists only because the
    // declaration names it.
}

BindKeysWnd::BindKeysList::~BindKeysList()
{
    // RVA 0x4A6680
    RemoveAllItems();
}

int BindKeysWnd::BindKeysList::CreateFromPattern(m3d::ui::StringsListBoxWnd* patternList, bool deleteSrc)
{
    // RVA 0x4A66D0 - the XML carries a plain strings list box; the bindings list
    // replaces it in place and inherits every visual property.
    if (!patternList)
    {
        return 0;
    }
    auto* parent = patternList->GetParent();
    if (!parent)
    {
        return 0;
    }

    Create(patternList->GetBounds(), 0x240, patternList->GetId());
    SetStyle(patternList->GetStyle());
    SetText(patternList->GetText());
    SetId(patternList->GetId());
    SetName(patternList->GetName());
    SetBounds(patternList->GetBounds(), true);
    SetDefaultFont(patternList->GetDefaultFont());
    SetWrapMode(patternList->GetWrapMode());
    SetFormatMode(patternList->GetFormatMode());
    SetColor(patternList->GetColor());
    SetTextColor(patternList->GetTextColor());
    SetTextColorDisabled(patternList->GetTextColorDisabled());
    SetClientEdges(patternList->GetClientEdges());
    SetPane(patternList->GetPaneName());
    SetPaneFlags(patternList->GetPaneFlags());
    SetScrollPane(patternList->GetScrollPaneName());
    SetBackground(patternList->GetBackground());

    CStr tooltip;
    patternList->GetProperty(PROP_WND_TOOLTIP, &tooltip);
    SetProperty(PROP_WND_TOOLTIP, &tooltip);

    SetOnShowAnimation(patternList->GetOnShowAnimation());
    SetOnHideAnimation(patternList->GetOnHideAnimation());
    m_drawFlags = 5;

    parent->AddChild(this);
    parent->MoveChildToFirstPosition(this);
    if (deleteSrc)
    {
        parent->RemoveChild(patternList);
        delete patternList;
    }
    return 1;
}

int BindKeysWnd::BindKeysList::CreateItems()
{
    // RVA 0x4A69A0 - one row per editable impulse, in the fixed order of
    // l_impulsesForEdit.
    auto res = 1;
    for (auto i = 0; i < NUM_IMPULSES_FOR_EDIT; ++i)
    {
        auto keys = M3D_APP->m_pImpulses->GetKeysForImpulse(l_impulsesForEdit[i], BINDINGS_GAME_MODE);
        TrimToShownKeySets(keys);

        auto* item = new BindKeysItem(GetClientBounds().width);
        if (!item)
        {
            return 0;
        }

        res &= item->SetUp(l_impulsesForEdit[i], keys, this, i);
        AddItem(item);
    }
    return res;
}

int BindKeysWnd::BindKeysList::Rebind(Impulse impId)
{
    // RVA 0x4A6BA0 - refreshes the buttons from the impulse layer. IM_ERROR
    // refreshes every row, anything else stops after the matching one.
    auto res = 1;
    for (auto i = 0; i < GetCount(); ++i)
    {
        auto* item = m_items[i].m_item;
        if (!item)
        {
            res = 0;
            continue;
        }
        if (impId != IM_ERROR && impId != item->m_impId)
        {
            continue;
        }

        auto keys = M3D_APP->m_pImpulses->GetKeysForImpulse(item->m_impId, BINDINGS_GAME_MODE);
        TrimToShownKeySets(keys);

        for (auto slot = 0; slot < 2; ++slot)
        {
            if (!item->m_btns[slot])
            {
                res = 0;
                continue;
            }
            if (slot >= static_cast<int>(keys.size()))
            {
                item->m_btns[slot]->OnRebind(retruxx::vector<int>());
            }
            else
            {
                item->m_btns[slot]->OnRebind(keys[slot]);
            }
        }

        if (impId != IM_ERROR)
        {
            return res;
        }
    }
    return res;
}

bool BindKeysWnd::BindKeysList::IsImpulseForEdit(Impulse impId)
{
    // RVA 0x4A7440
    for (auto i = 0; i < NUM_IMPULSES_FOR_EDIT; ++i)
    {
        if (l_impulsesForEdit[i] == impId)
        {
            return true;
        }
    }
    return false;
}

int BindKeysWnd::BindKeysList::ProcessBinding(BindKeysWnd::KeySetButton* btn)
{
    // RVA 0x4A6EA0 - a button has captured a key set; work out whether it can
    // take it over and either apply it or put the old one back.
    if (!btn)
    {
        return 0;
    }

    Impulse const impId = btn->m_impId;
    auto const keys = btn->m_newKeySet;
    auto const prevKeys = btn->m_keySet;
    auto const existing = static_cast<Impulse>(M3D_APP->m_pImpulses->GetImpulseForKeys(keys, BINDINGS_GAME_MODE));

    if (existing == impId)
    {
        btn->OnRebind(prevKeys);
        return 1;
    }

    if (IsFreelyReplaceable(existing))
    {
        static_cast<BindKeysWnd*>(GetParent())->m_bChanged = true;
        BindUnbind(ACTION_UNBIND, impId, prevKeys);
        BindUnbind(ACTION_BIND, impId, keys);
        return 1;
    }

    if (!IsImpulseForEdit(existing))
    {
        // The key belongs to something the player is not allowed to rebind.
        btn->OnRebind(prevKeys);
        M3D_APP->RunMsgBoxDlg(
            CStr(), M3D_APP->GetStringByStringId0(m_aif.m_strBindReserved), m3d::ui::MBX_OK, false);
        return 1;
    }

    CStr const impFullName =
        CStr("\"") + M3D_APP->GetStringByStringId0(M3D_APP->m_pImpulses->GetImpulseNameById(existing)) + "\"";
    bool const hasOtherBinding =
        M3D_APP->m_pImpulses->GetKeysForImpulse(existing, BINDINGS_GAME_MODE).size() > 1;

    CStr text;
    if (hasOtherBinding)
    {
        // The other impulse would keep a binding, so taking this one is offered.
        text.format(M3D_APP->GetStringByStringId0(m_aif.m_strBindExists).c_str(), impFullName.c_str());
        if (M3D_APP->RunMsgBoxDlg(CStr(), text, m3d::ui::MBX_YES_NO, false) == m3d::ui::MBX_RET_YES)
        {
            static_cast<BindKeysWnd*>(GetParent())->m_bChanged = true;
            BindUnbind(ACTION_UNBIND, existing, keys);
            BindUnbind(ACTION_UNBIND, impId, prevKeys);
            BindUnbind(ACTION_BIND, impId, keys);
            return 1;
        }
    }
    else
    {
        // It is the other impulse's only binding, so the swap is refused.
        text.format(M3D_APP->GetStringByStringId0(m_aif.m_strOnlyBindExists).c_str(), impFullName.c_str());
        M3D_APP->RunMsgBoxDlg(CStr(), text, m3d::ui::MBX_OK, false);
    }

    btn->OnRebind(prevKeys);
    return 1;
}

int BindKeysWnd::BindKeysList::BindUnbind(int action, Impulse imp, retruxx::vector<int> const& keys)
{
    // RVA 0x4A7280 - a binding is always expressed as three key names, unused
    // slots staying empty.
    if (imp == IM_ERROR)
    {
        return 0;
    }

    CStr keyNames[MAX_KEYS_PER_BINDING];
    for (auto i = 0; i < static_cast<int>(keys.size()) && i < MAX_KEYS_PER_BINDING; ++i)
    {
        keyNames[i] = M3D_APP->m_pImpulses->GetKeyNameById(keys[i]);
    }

    CStr const gameModeName = M3D_APP->m_pImpulses->GetGameModeNameById(BINDINGS_GAME_MODE);
    CStr const impName = M3D_APP->m_pImpulses->GetImpulseNameById(imp);

    if (action == ACTION_BIND)
    {
        return M3D_APP->m_pImpulses->BindKey3(gameModeName, impName, keyNames[0], keyNames[1], keyNames[2]);
    }
    return M3D_APP->m_pImpulses->UnbindKey3(gameModeName, impName, keyNames[0], keyNames[1], keyNames[2]);
}

int BindKeysWnd::BindKeysList::MeasureItem(int itemIdx, BoundsBase<float>& bounds) const
{
    // RVA 0x4A7460
    auto* item = m_items[itemIdx].m_item;
    if (!item)
    {
        return 0;
    }
    bounds.x0 = 0.0;
    bounds.y0 = 0.0;
    bounds.width = item->m_aif.m_itemW;
    bounds.height = item->m_aif.m_itemH;
    return 1;
}

int BindKeysWnd::BindKeysList::RenderItem(int itemIdx, PointBase<float> const& org, m3d::ui::DrawInfo const& di)
{
    // RVA 0x4A74D0 - the row is made of real child windows, so "rendering" it
    // only means moving them to where the row scrolled to.
    auto* item = m_items[itemIdx].m_item;
    if (!item)
    {
        return 0;
    }
    if (!item->m_bg || !item->m_impLabel)
    {
        return 0;
    }

    float const y0 = (di.m_clientRect.y0 - di.m_originalRect.y0) + org.y;
    for (auto* btn : item->m_btns)
    {
        auto b = btn->GetBounds();
        b.y0 = y0;
        btn->SetBounds(b, true);
    }

    auto bgB = item->m_bg->GetBounds();
    auto impB = item->m_impLabel->GetBounds();
    bgB.y0 = y0;
    impB.y0 = bgB.y0;
    item->m_bg->SetBounds(bgB, true);
    item->m_impLabel->SetBounds(impB, true);
    return 1;
}

int BindKeysWnd::BindKeysList::DeleteItem(int itemIdx)
{
    // RVA 0x4A7610
    delete m_items[itemIdx].m_item;
    m_items[itemIdx].m_item = nullptr;
    return 1;
}

int BindKeysWnd::BindKeysList::CompareItem(int, int)
{
    // RVA 0x4A7690 - the rows keep the order of l_impulsesForEdit.
    return 0;
}

int BindKeysWnd::BindKeysList::OnWndNotify(m3d::ui::Wnd* from, unsigned id, unsigned msg, m3d::AIParam const&)
{
    // RVA 0x4A6DB0
    if (id != ID_KEY_SET_BUTTON)
    {
        // NOTE: notifications from anything but a key-set button are reported as
        // handled, so they never reach the base class.
        return 1;
    }

    switch (msg)
    {
    case 1:
    case 2:
    case 3:
    case MSG_BINDING_READY:
        ProcessBinding(RT_DYNCAST(from, BindKeysWnd::KeySetButton));
        return 1;
    case MSG_INPUT_MODE_ON:
    case MSG_INPUT_MODE_OFF:
        // Exactly one button may capture input at a time; tell the others.
        for (auto i = 0; i < GetCount(); ++i)
        {
            auto* item = m_items[i].m_item;
            for (auto* btn : item->m_btns)
            {
                if (btn && btn != from)
                {
                    if (msg == MSG_INPUT_MODE_ON)
                    {
                        btn->OnAnotherInputOn();
                    }
                    else
                    {
                        btn->OnAnotherInputOff();
                    }
                }
            }
        }
        return 1;
    default:
        break;
    }
    return 1;
}

int BindKeysWnd::BindKeysList::OnBeforeAddToWndStation()
{
    // RVA 0x4A6950
    if (m_items.empty())
    {
        CreateItems();
    }
    GetStation()->CaptureFocus(this);
    return Wnd::OnBeforeAddToWndStation();
}

// ===========================================================================
//  BindKeysWnd
// ===========================================================================

BindKeysWnd::AuxInfo::AuxInfo()
{
    // RVA 0x4A5C70
    m_lstBindingsName = "lstBindings";
}

BindKeysWnd::BindKeysWnd()
{
    // RVA 0x4A5E90
    m_lstBindings = nullptr;
    m_isInited = false;
    m_isDirty = false;
    m_bChanged = false;
}

BindKeysWnd::BindKeysWnd(BindKeysWnd const&)
{
    // RVA 0x4A5FA0 - the shipped copy constructor copies nothing from rhs and,
    // unlike the default one, leaves the list pointer and all three flags
    // uninitialised.
}

BindKeysWnd::~BindKeysWnd()
{
    // RVA 0x4A5F60 - the list is an ordinary child window and goes with the
    // parent; only m_aif and the Wnd base need unwinding.
}

m3d::Object* BindKeysWnd::Clone()
{
    // RVA 0x4A5BB0
    return new BindKeysWnd(*this);
}

m3d::Object* BindKeysWnd::CreateObject()
{
    return new BindKeysWnd;
}

m3d::Class* BindKeysWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(Wnd);
}

m3d::Class* BindKeysWnd::GetClass() const
{
    return RT_CLASS_LOCAL(BindKeysWnd);
}

int BindKeysWnd::GameDataSetup()
{
    // RVA 0x4A5FC0
    using namespace m3d::ui;
    if (m_isInited)
    {
        return 1;
    }

    auto child = GetChildByName(m_aif.m_lstBindingsName);
    if (!child || !child->IsKindOf(RT_CLASS_LOCAL(StringsListBoxWnd)))
    {
        M3D_LOG_INFO("Make control error: control " + m_aif.m_lstBindingsName + " is not found or incorrect type");
        M3D_LOG_INFO("BindKeysWnd error: fail to init because of a bad resource");
        return 0;
    }

    m_lstBindings = new BindKeysList;
    if (!m_lstBindings)
    {
        M3D_LOG_INFO("Make control error: cannot create " + m_aif.m_lstBindingsName +
                     " - cannot instantiate variable");
        M3D_LOG_INFO("BindKeysWnd error: fail to init because of a bad resource");
        return 0;
    }

    if (m_lstBindings->CreateFromPattern(RT_DYNCAST(child, StringsListBoxWnd), true))
    {
        m_isInited = true;
        return 1;
    }

    M3D_LOG_INFO("Make control error: cannot create " + m_aif.m_lstBindingsName + " from pattern class");
    M3D_LOG_INFO("BindKeysWnd error: fail to init because of a bad resource");
    return 0;
}

int BindKeysWnd::GameDataUpdate(void* data, int dataType)
{
    // RVA 0x4A6520 - dataType 17 is "the bindings changed under us".
    if (dataType != 17)
    {
        return 1;
    }
    if (!IsChildOf(M3D_APP))
    {
        // Off screen: remember to catch up the next time the page opens.
        m_isDirty = true;
        return 1;
    }
    if (data)
    {
        m_lstBindings->Rebind(IM_ERROR);
    }
    return 1;
}

int BindKeysWnd::OnBeforeAddToWndStation()
{
    // RVA 0x4A62D0
    m_bChanged = false;
    if (m_isInited && m_isDirty)
    {
        m_lstBindings->Rebind(IM_ERROR);
    }
    return Wnd::OnBeforeAddToWndStation();
}

int BindKeysWnd::OnWndNotify(m3d::ui::Wnd*, unsigned id, unsigned msg, m3d::AIParam const&)
{
    // RVA 0x4A6310
    if (msg != 1)
    {
        return 0;
    }
    switch (id)
    {
    case ID_BTN_LOAD_FROM_PROFILE:
        M3D_APP->m_pImpulses->LoadFromProfile();
        m_bChanged = false;
        break;
    case ID_BTN_SAVE_TO_PROFILE:
        M3D_APP->m_pImpulses->SaveToProfile();
        m_bChanged = false;
        return 1;
    case ID_BTN_LOAD_DEFAULTS:
        M3D_APP->m_pImpulses->LoadFromDefaults();
        m_bChanged = true;
        return 1;
    }
    // NOTE: every notification with msg == 1 is reported as handled, whatever
    // the control id.
    return 1;
}

int BindKeysWnd::ApplyChanges(bool bForce)
{
    // RVA 0x4A63A0 - returns 0 only when the player cancelled out of the
    // "you have unsaved bindings" prompt.
    if (!m_bChanged)
    {
        return 1;
    }
    if (!bForce)
    {
        auto const ret = M3D_APP->RunMsgBoxDlg(
            CStr(), M3D_APP->GetStringByStringId0("BindingsChanged"), m3d::ui::MBX_YES_NO_CANCEL, false);
        if (ret == m3d::ui::MBX_RET_NO)
        {
            CancelBindings();
            return 1;
        }
        if (ret == m3d::ui::MBX_RET_CANCEL)
        {
            return 0;
        }
    }
    M3D_APP->m_pImpulses->SaveToProfile();
    m_bChanged = false;
    return 1;
}

void BindKeysWnd::CancelBindings()
{
    // RVA 0x4A64C0
    M3D_APP->m_pImpulses->LoadFromProfile();
    m_bChanged = false;
}

void BindKeysWnd::ApplyBindings()
{
    // RVA 0x4A64E0
    M3D_APP->m_pImpulses->SaveToProfile();
    m_bChanged = false;
}

void BindKeysWnd::LoadDefaultBindings()
{
    // RVA 0x4A6500
    M3D_APP->m_pImpulses->LoadFromDefaults();
    m_bChanged = true;
}
