#include "skinswnd.h"

#include <core/aiparam.h>
#include <core/kernel.h>
#include <core/log.h>
#include <game/m3dgame.h>
#include <m3dapp.h>
#include <server/objects/base/objcontainer.h>
#include <server/objects/cabin.h>
#include <server/objects/player.h>
#include <server/objects/vehicle.h>
#include <skelmodel.h>
#include <ui/button.h>
#include <ui/wndstation.h>

namespace
{
    // Ids the XML gives the three skin buttons.
    unsigned const ID_BTN_PREV = 300700;
    unsigned const ID_BTN_NEXT = 300701;
    unsigned const ID_BTN_BUY = 300702;

    // SkinsWnd tells its parent a skin was bought with this CallParentNotify
    // message (GarageWnd::OnBuySkin listens for it).
    unsigned const MSG_BUY_SKIN = 21;
}  // namespace

// ===========================================================================
//  SkinSwitcher
// ===========================================================================

SkinSwitcher::SkinSwitcher() : m_vehicleId(-1), m_btnSkinNext(nullptr), m_btnSkinPrev(nullptr), m_curSkin(-1)
{
    // RVA 0x581AC0
}

SkinSwitcher::~SkinSwitcher()
{
    // RVA 0x581AE0 - the switcher owns nothing.
}

void SkinSwitcher::Reset()
{
    // RVA 0x581AF0
    m_vehicleId = -1;
    m_btnSkinNext = nullptr;
    m_btnSkinPrev = nullptr;
    m_curSkin = -1;
}

bool SkinSwitcher::IsValid() const
{
    // RVA 0x581B80 - only the two buttons are checked; a switcher whose vehicle
    // has gone away still reports valid, and the callers guard that separately
    // by testing GetVehicle().
    if (!m_btnSkinNext || !M3D_APP->IsWndAlive(m_btnSkinNext, -1) || !m_btnSkinNext->Valid())
    {
        return false;
    }
    if (!m_btnSkinPrev || !M3D_APP->IsWndAlive(m_btnSkinPrev, -1) || !m_btnSkinPrev->Valid())
    {
        return false;
    }
    return true;
}

ai::Vehicle* SkinSwitcher::GetVehicle() const
{
    // RVA 0x581BE0
    if (m_vehicleId == -1)
    {
        return nullptr;
    }
    return RT_DYNCAST(ai::theObjects->GetEntityByObjId(m_vehicleId), ai::Vehicle);
}

int SkinSwitcher::SetUp(int vehicleId, m3d::ui::ButtonWnd* btnNext, m3d::ui::ButtonWnd* btnPrev)
{
    // RVA 0x581B10
    m_curSkin = -1;
    m_vehicleId = vehicleId;
    m_btnSkinNext = btnNext;
    m_btnSkinPrev = btnPrev;

    if (!IsValid())
    {
        Reset();
        return 0;
    }

    ai::Vehicle* vehicle = GetVehicle();
    m_curSkin = vehicle ? static_cast<int>(vehicle->GetSkin()) : -1;
    UpdateSkinButtonsStates();
    return 1;
}

int SkinSwitcher::GetMaxSkinCount(ai::Vehicle* vehicle)
{
    // RVA 0x581C40
    if (!vehicle)
    {
        return 0;
    }
    ai::Cabin* cabin = vehicle->GetCabin();
    if (!cabin)
    {
        return 0;
    }
    m3d::AnimatedModel* model = cabin->GetModel();
    if (!model)
    {
        return 0;
    }
    return static_cast<int>(model->GetNumSkins());
}

bool SkinSwitcher::IsSkinApplicable(int skin, ai::Vehicle* vehicle)
{
    // RVA 0x581C80 - a model either declares that it loaded every skin, or it
    // carries the explicit set of the ones it did.
    if (!vehicle || static_cast<unsigned int>(skin) >= SKIN_DEATH)
    {
        return false;
    }
    ai::Cabin* cabin = vehicle->GetCabin();
    if (!cabin)
    {
        return false;
    }
    m3d::AnimatedModel* model = cabin->GetModel();
    if (!model)
    {
        return false;
    }

    m3d::LoadSkins const& loadedSkins = model->GetLoadedSkins();
    if (loadedSkins.loadAllSkins)
    {
        return true;
    }
    return loadedSkins.loadSkins.find(skin) != loadedSkins.loadSkins.end();
}

int SkinSwitcher::ApplySkin(int skin, ai::Vehicle* vehicle)
{
    // RVA 0x581DC0
    if (!vehicle || static_cast<unsigned int>(skin) >= SKIN_DEATH)
    {
        return 0;
    }
    vehicle->SetSkin(vehicle->CheckSkin(skin));
    return 1;
}

int SkinSwitcher::GetVehicleSkin(ai::Vehicle* vehicle)
{
    // RVA 0x581F10
    return vehicle ? static_cast<int>(vehicle->GetSkin()) : -1;
}

int SkinSwitcher::GetNextSkin() const
{
    // RVA 0x581CF0 - the first applicable skin strictly above the current one.
    ai::Vehicle* vehicle = GetVehicle();
    if (!IsValid() || !vehicle)
    {
        return -1;
    }
    int const maxSkinCount = GetMaxSkinCount(vehicle);
    int skin = m_curSkin + 1;
    if (skin >= maxSkinCount)
    {
        return -1;
    }
    if (skin < 0)
    {
        skin = 0;
    }
    if (skin >= maxSkinCount)
    {
        return -1;
    }
    while (!IsSkinApplicable(skin, vehicle))
    {
        if (++skin >= maxSkinCount)
        {
            return -1;
        }
    }
    return skin;
}

int SkinSwitcher::GetPrevSkin() const
{
    // RVA 0x581D50 - the first applicable skin strictly below the current one.
    ai::Vehicle* vehicle = GetVehicle();
    if (!IsValid() || !vehicle)
    {
        return -1;
    }
    int const maxSkinCount = GetMaxSkinCount(vehicle);
    int skin = m_curSkin - 1;
    if (skin < 0)
    {
        return -1;
    }
    if (skin >= maxSkinCount)
    {
        skin = maxSkinCount - 1;
    }
    if (skin < 0)
    {
        return -1;
    }
    while (!IsSkinApplicable(skin, vehicle))
    {
        if (--skin < 0)
        {
            return -1;
        }
    }
    return skin;
}

void SkinSwitcher::UpdateSkinButtonsStates()
{
    // RVA 0x581DF0 - each arrow is enabled only while there is somewhere to go.
    if (!IsValid())
    {
        return;
    }
    m_btnSkinNext->EnableWindow(GetNextSkin() != -1);
    m_btnSkinPrev->EnableWindow(GetPrevSkin() != -1);
}

int SkinSwitcher::SetCurSkin(int skin)
{
    // RVA 0x581E40
    if (!IsValid() || !GetVehicle())
    {
        m_curSkin = -1;
        UpdateSkinButtonsStates();
        return 0;
    }

    m_curSkin = skin;
    UpdateSkinButtonsStates();

    ai::Vehicle* vehicle = GetVehicle();
    if (!vehicle || static_cast<unsigned int>(skin) >= SKIN_DEATH)
    {
        return 0;
    }
    vehicle->SetSkin(vehicle->CheckSkin(skin));
    return 1;
}

void SkinSwitcher::ShowNextSkin()
{
    // RVA 0x581EB0
    if (!IsValid() || !GetVehicle())
    {
        return;
    }
    int const nextSkin = GetNextSkin();
    if (nextSkin != -1)
    {
        SetCurSkin(nextSkin);
    }
}

void SkinSwitcher::ShowPrevSkin()
{
    // RVA 0x581EE0
    if (!IsValid() || !GetVehicle())
    {
        return;
    }
    int const prevSkin = GetPrevSkin();
    if (prevSkin != -1)
    {
        SetCurSkin(prevSkin);
    }
}

int SkinSwitcher::GetCurSkin() const
{
    // RVA 0x581F20
    return m_curSkin;
}

// ===========================================================================
//  SkinsWnd
// ===========================================================================

RT_CLASS_EXPORTS_BEGIN(SkinsWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(SkinsWnd);

SkinsWnd::AuxInfo::AuxInfo() :
    m_btnPrevName("btnPrev"),
    m_btnNextName("btnNext"),
    m_btnBuyName("btnBuy"),
    m_wndPriceName("wndPrice")
{
    // RVA 0x4739B0
}

SkinsWnd::AuxInfo::AuxInfo(SkinsWnd::AuxInfo const& rhs) :
    m_btnPrevName(rhs.m_btnPrevName),
    m_btnNextName(rhs.m_btnNextName),
    m_btnBuyName(rhs.m_btnBuyName),
    m_wndPriceName(rhs.m_wndPriceName)
{
}

m3d::Class* SkinsWnd::GetBaseClass()
{
    return &m3d::ui::Wnd::m_classWnd;
}

m3d::Class* SkinsWnd::GetClass() const
{
    return RT_CLASS_LOCAL(SkinsWnd);
}

m3d::Object* SkinsWnd::CreateObject()
{
    return new SkinsWnd;
}

m3d::Object* SkinsWnd::Clone()
{
    // RVA 0x473920 - a fresh default-constructed window, not a copy of *this.
    return new SkinsWnd;
}

SkinsWnd::SkinsWnd() :
    m_skinSwitcher(nullptr),
    m_btnPrev(nullptr),
    m_btnNext(nullptr),
    m_btnBuy(nullptr),
    m_wndPrice(nullptr),
    m_vehicleId(-1),
    m_savedSkinId(-1)
{
    // RVA 0x473AC0
}

SkinsWnd::SkinsWnd(SkinsWnd const&) :
    // NOTE: the shipped copy ctor (RVA 0x473BF0) runs the Wnd base ctor and the
    // AuxInfo ctor and then stops - it leaves every pointer and both ids
    // uninitialised rather than copying or nulling them. Initialised here so
    // nothing can read garbage; nothing in the exe reaches this ctor.
    m_skinSwitcher(nullptr),
    m_btnPrev(nullptr),
    m_btnNext(nullptr),
    m_btnBuy(nullptr),
    m_wndPrice(nullptr),
    m_vehicleId(-1),
    m_savedSkinId(-1)
{
}

SkinsWnd::~SkinsWnd()
{
    // RVA 0x473C10
    delete m_skinSwitcher;
    m_skinSwitcher = nullptr;
}

int SkinsWnd::GameDataSetup()
{
    // RVA 0x473C70
    if ((m_gameDataFlags & 2) == 0)
    {
        int res = 1;

        m3d::Object* btnPrev = GetChildByName(m_aif.m_btnPrevName);
        if (btnPrev && btnPrev->IsKindOf(&m3d::ui::ButtonWnd::m_classButtonWnd))
        {
            m_btnPrev = static_cast<m3d::ui::ButtonWnd*>(btnPrev);
        }
        else
        {
            M3D_LOG_INFO("Get control error: control " + m_aif.m_btnPrevName + " is not found or incorrect type");
            res = 0;
        }

        m3d::Object* btnNext = GetChildByName(m_aif.m_btnNextName);
        if (btnNext && btnNext->IsKindOf(&m3d::ui::ButtonWnd::m_classButtonWnd))
        {
            m_btnNext = static_cast<m3d::ui::ButtonWnd*>(btnNext);
        }
        else
        {
            M3D_LOG_INFO("Get control error: control " + m_aif.m_btnNextName + " is not found or incorrect type");
            res = 0;
        }

        m3d::Object* btnBuy = GetChildByName(m_aif.m_btnBuyName);
        if (btnBuy && btnBuy->IsKindOf(&m3d::ui::ButtonWnd::m_classButtonWnd))
        {
            m_btnBuy = static_cast<m3d::ui::ButtonWnd*>(btnBuy);
        }
        else
        {
            M3D_LOG_INFO("Get control error: control " + m_aif.m_btnBuyName + " is not found or incorrect type");
            res = 0;
        }

        m3d::Object* wndPrice = GetChildByName(m_aif.m_wndPriceName);
        if (wndPrice && wndPrice->IsKindOf(&m3d::ui::Wnd::m_classWnd))
        {
            m_wndPrice = static_cast<m3d::ui::Wnd*>(wndPrice);
            if (res)
            {
                m_skinSwitcher = new SkinSwitcher;
                m_gameDataFlags |= 1u;
            }
        }
        else
        {
            M3D_LOG_INFO("Get control error: control " + m_aif.m_wndPriceName + " is not found or incorrect type");
        }
    }

    if ((m_gameDataFlags & 1) != 0)
    {
        return 1;
    }
    M3D_LOG_INFO("SkinsWnd::GameDataSetup error");
    return 0;
}

int SkinsWnd::OnWndNotify(m3d::ui::Wnd* from, unsigned int id, unsigned int msg, m3d::AIParam const& data)
{
    // RVA 0x4741B0
    if ((m_gameDataFlags & 1) == 0)
    {
        return 0;
    }
    if ((m_style & 0x100000) != 0)
    {
        ReflectChildNotifyToParent(from, id, msg, data);
    }
    if (msg != 1)
    {
        return 0;
    }

    switch (id)
    {
    case ID_BTN_PREV:
        ShowPrevSkin();
        return 1;
    case ID_BTN_NEXT:
        ShowNextSkin();
        return 1;
    case ID_BTN_BUY:
        BuySkin();
        return 1;
    default:
        return 0;
    }
}

ai::Vehicle* SkinsWnd::GetVehicle() const
{
    // RVA 0x4743D0
    if (m_vehicleId == -1)
    {
        return nullptr;
    }
    return RT_DYNCAST(ai::theObjects->GetEntityByObjId(m_vehicleId), ai::Vehicle);
}

int SkinsWnd::GetSkinPrice(int, int) const
{
    // RVA 0x474640 - every skin costs the same flat 10 regardless of vehicle or
    // skin; both parameters are ignored, as shipped.
    return 10;
}

int SkinsWnd::SetupForVehicle(int vehicleId)
{
    // RVA 0x474430
    if ((m_gameDataFlags & 1) == 0)
    {
        return 0;
    }
    m_vehicleId = vehicleId;
    if (IsChildOf(M3D_APP))
    {
        FullUpdate();
    }
    return m_vehicleId == -1 || GetVehicle();
}

void SkinsWnd::FullUpdate()
{
    // RVA 0x474580 - remember whichever skin the vehicle arrived wearing, so
    // OnAfterRemoveFromWndStation can put it back if nothing was bought.
    if ((m_gameDataFlags & 1) == 0)
    {
        return;
    }
    m_skinSwitcher->SetUp(m_vehicleId, m_btnNext, m_btnPrev);
    ai::Vehicle* vehicle = GetVehicle();
    m_savedSkinId = vehicle ? static_cast<int>(vehicle->GetSkin()) : -1;
    OnChangeSkin();
}

void SkinsWnd::ShowPrevSkin()
{
    // RVA 0x474490
    if ((m_gameDataFlags & 1) == 0)
    {
        return;
    }
    m_skinSwitcher->ShowPrevSkin();
    OnChangeSkin();
}

void SkinsWnd::ShowNextSkin()
{
    // RVA 0x4744E0
    if ((m_gameDataFlags & 1) == 0)
    {
        return;
    }
    m_skinSwitcher->ShowNextSkin();
    OnChangeSkin();
}

void SkinsWnd::OnChangeSkin()
{
    // RVA 0x474530 - the price label is rewritten with the flat skin price. Note
    // the shipped code builds the string from the literal 10 rather than calling
    // GetSkinPrice(), so the two would drift apart if the price ever varied.
    if ((m_gameDataFlags & 1) == 0)
    {
        return;
    }
    m_wndPrice->SetText(CStr(10));
}

void SkinsWnd::BuySkin()
{
    // RVA 0x474240 - "buying" only pins the current skin down: it becomes the
    // one restored on close, and the money is taken.
    if ((m_gameDataFlags & 1) == 0)
    {
        return;
    }
    int const curSkin = m_skinSwitcher->GetCurSkin();
    if (curSkin == -1 || !ai::thePlayer || !ai::thePlayer->GetVehicle())
    {
        return;
    }

    if (curSkin == m_savedSkinId)
    {
        M3D_APP->RunMsgBoxDlg(CStr(), M3D_APP->GetStringByStringId0(CStr("SkinAlreadySet")), 1u, false);
        return;
    }

    if (ai::thePlayer->GetMoney() < 10)
    {
        M3D_APP->RunMsgBoxDlg(CStr(), M3D_APP->GetStringByStringId0(CStr("NotEnoughGold")), 1u, false);
        return;
    }

    ai::thePlayer->AddMoney(-10);
    m_savedSkinId = curSkin;

    m3d::AIParam data;
    CallParentNotify(MSG_BUY_SKIN, data, 0);
}

int SkinsWnd::OnBeforeAddToWndStation()
{
    // RVA 0x4745E0
    FullUpdate();
    return m3d::ui::Wnd::OnBeforeAddToWndStation();
}

int SkinsWnd::OnAfterRemoveFromWndStation()
{
    // RVA 0x474600 - anything the player browsed but did not buy is rolled back
    // to the skin the vehicle had when the window opened.
    int const res = m3d::ui::Wnd::OnAfterRemoveFromWndStation();
    ai::Vehicle* vehicle = GetVehicle();
    if (vehicle && m_savedSkinId != -1 && static_cast<int>(vehicle->GetSkin()) != m_savedSkinId)
    {
        SkinSwitcher::ApplySkin(m_savedSkinId, vehicle);
    }
    return res;
}
