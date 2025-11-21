#include "weaponinfolist.h"

#include "game/m3dgame.h"
#include "core/log.h"
#include "game/uimanager/uidefs.h"
#include "ui/image.h"

RT_CLASS_EXPORTS_BEGIN(WeaponInfoList)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(WeaponInfoList);

WeaponInfoList::AuxInfo::AuxInfo()
{
    m_smallSpace = 3.0f;
    m_bigSpace = 5.0f;
    m_bottom = 22.0f;
    m_groupInMainUIW = 12.0f;
    m_groupInCharWndW = 12.0f;
    m_groupInFignyaWndW = 20.0f;
    m_groupInCharWndSpace = 10.0f;

    m_groupTexInMainUi = "WeaponGroupInMainUi";
    m_groupTexInCharWnd = "WeaponGroupInCharWnd";
    m_groupTexInFignyaWnd = "WeaponGroupInFignyaWnd";
    m_wndDecorTopName = "wndDecorTop";
    m_wndDecorBottomName = "wndDecorBottom";
}

m3d::Class* WeaponInfoList::GetBaseClass()
{
    return RT_CLASS_LOCAL(Wnd);
}

m3d::Object* WeaponInfoList::CreateObject()
{
    return new WeaponInfoList;
}

int WeaponInfoList::CreateFromPattern(m3d::ui::Wnd*, bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* WeaponInfoList::Clone()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* WeaponInfoList::GetClass() const
{
    return RT_CLASS_LOCAL(WeaponInfoList);
}

WeaponInfoList::~WeaponInfoList()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void WeaponInfoList::UpdateOnKeyBindingsChanged()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int WeaponInfoList::RecalcLayotInMainInterface()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int WeaponInfoList::GameDataSetup()
{
    using namespace m3d::ui;

    int res = 1;
    if ((m_gameDataFlags & 2) == 0)
    {
        if (m_guiId == IW_WND_WEAPON_INFO_LIST)
        {
            m_type = TYPE_IN_MAIN_INTERFACE;

            auto child = GetChildByName(m_aif.m_wndDecorTopName);
            if (child && IS_KIND_OF(child, ImageWnd))
            {
                m_wndDecorTop = RT_DYNCAST(child, ImageWnd);
            }
            else
            {
                M3D_LOG_ERR("Get control error: control " + m_aif.m_wndDecorTopName + " is not found or incorrect type");
                res = 0;
            }

            child = GetChildByName(m_aif.m_wndDecorBottomName);
            if (child && IS_KIND_OF(child, ImageWnd))
            {
                m_wndDecorBottom = RT_DYNCAST(child, ImageWnd);
            }
            else
            {
                M3D_LOG_ERR("Get control error: control " + m_aif.m_wndDecorBottomName + " is not found or incorrect type");
                res = 0;
            }
        }
        else if (m_guiId <= IW_WND_WEAPON_INFO_LIST || m_guiId > IW_WND_WEAPON_INFO_LIST_IN_CHAR_WND_RIGHT)
        {
            res = 0;
        }
        else
        {
            m_type = TYPE_IN_CHARACTERISTIC_WND;
        }
        InitGroupTextures();
        if (WeaponInfoWnd::LoadPattern(this, m_type))
        {
            if (res)
            {
                m_gameDataFlags |= 1u;
            }
        }
        else
        {
            M3D_LOG_ERR("WeaponInfoList::GameDataSetup error - cannot load pattern for WeaponInfoWnd");
        }
    }

    if ((m_gameDataFlags & 1) != 0)
    {
        return 1;
    }

    M3D_LOG_ERR("WeaponInfoList: error - fail to init because of a bad resource");
    return 0;
}

int WeaponInfoList::OnWndNotify(m3d::ui::Wnd*, unsigned, unsigned, m3d::AIParam const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

WeaponInfoList::WeaponInfoList()
{
    m_wndDecorTop = nullptr;
    m_wndDecorBottom = nullptr;
    m_type = TYPE_NUM_TYPES;
}

WeaponInfoList::WeaponInfoList(WeaponInfoList const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void WeaponInfoList::GetItemsByGroup(int, std::vector<ref_ptr<WeaponInfoWnd>>&) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

ai::Vehicle* WeaponInfoList::GetVehicle() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int WeaponInfoList::GetNumOfGroups() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int WeaponInfoList::CreateItems()
{
    // TODO: implement WeaponInfoList::CreateItems
    // RETRUXX_NOT_IMPLEMENTED;
    return 1;
}

int WeaponInfoList::RecalcLayot()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int WeaponInfoList::GameDataUpdate(void* data, int dataType)
{
    if ((m_gameDataFlags & 1) == 0)
    {
        return 0;
    }
    switch (dataType)
    {
    case 17:
        if (IsChildOf(M3D_APP))
        {
            UpdateOnKeyBindingsChanged();
        }
        return 1;

    case 30:
        UpdateOnWeaponGroupChanged();
        return 1;

    case 64:
        UpdateOnPlayerVehicleChanged();
        return 1;

    case 65:
        UpdateOnVehiclePartChanged(data);
        return 1;

    default:
        break;
    }
    return 1;
}

int WeaponInfoList::AddGroupButton(int, BoundsBase<float> const&, WeaponInfoWnd const*, int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

bool WeaponInfoList::CanGunShotToTarget(ai::Obj const*) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int WeaponInfoList::UpdateOnVehiclePartChanged(void*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void WeaponInfoList::UpdateOnWeaponGroupChanged()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::rend::TexHandle WeaponInfoList::GetGroupTextureForWeaponBtn(WeaponInfoWnd const*, int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int WeaponInfoList::RecalcLayotInCharacteristicWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int WeaponInfoList::GetGroupByVehiclePartName(CStr const&) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int WeaponInfoList::OnBeforeAddToWndStation()
{
    CreateItems();
    return Wnd::OnBeforeAddToWndStation();
}

int WeaponInfoList::UpdateOnPlayerVehicleChanged()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int WeaponInfoList::AddItem(int, int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void WeaponInfoList::InitGroupTextures()
{
    ReleaseGroupTextures();
    CStr groupTexName;
    if (m_type)
    {
        if (m_type != 1)
        {
            return;
        }
        groupTexName = m_aif.m_groupTexInCharWnd;
    }
    else
    {
        groupTexName = m_aif.m_groupTexInMainUi;
    }

    const auto unselTex = M3D_APP->m_pInterfaceManager->GetIcoByName(groupTexName, 0);
    const auto selTex = M3D_APP->m_pInterfaceManager->GetIcoByName(groupTexName, 1);
    M3D_RENDERER->ReferenceTexture(unselTex);
    M3D_RENDERER->ReferenceTexture(selTex);
    m_groupTextures.push_back({unselTex, selTex});
}

void WeaponInfoList::ReleaseGroupTextures()
{
    for (auto& tex : m_groupTextures)
    {
        M3D_RENDERER->ReleaseTexture(tex.first);
        M3D_RENDERER->ReleaseTexture(tex.second);
    }

    m_groupTextures.clear();
}

void WeaponInfoList::SetTooltipForGroupButton(m3d::ui::Wnd*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int WeaponInfoList::GameDataClear(bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORTS_BEGIN(WeaponInfoWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(WeaponInfoWnd);

int WeaponInfoWnd::GetGroupId() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

ai::Obj const* WeaponInfoWnd::GetGun() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* WeaponInfoWnd::CreateObject()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* WeaponInfoWnd::GetClass() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* WeaponInfoWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(Wnd);
}

WeaponInfoWnd::~WeaponInfoWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int WeaponInfoWnd::SetupForGun(int, int, WeaponInfoList::Type)
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* WeaponInfoWnd::Clone()
{
    RETRUXX_NOT_IMPLEMENTED;
}

WeaponInfoWnd::WeaponInfoWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

WeaponInfoWnd::WeaponInfoWnd(WeaponInfoWnd const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::rend::TexHandle WeaponInfoWnd::GetIco() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int WeaponInfoWnd::GetAmmo() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void WeaponInfoWnd::UpdateAmmo()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void WeaponInfoWnd::UpdateCanShotBg()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void WeaponInfoWnd::UpdateOnNewFrame()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int WeaponInfoWnd::CreateFromPattern(WeaponInfoList::Type)
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::rend::TexHandle WeaponInfoWnd::GetBarTexture(bool) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int WeaponInfoWnd::GameDataUpdate(void*, int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int WeaponInfoWnd::GameDataClear(bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

ai::DamageType WeaponInfoWnd::GetWeaponTypeForGun(ai::Obj const*) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void WeaponInfoWnd::UpdateCharge()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void WeaponInfoWnd::ClearPattern(WeaponInfoList::Type)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int WeaponInfoWnd::LoadPattern(m3d::ui::Wnd*, WeaponInfoList::Type)
{
    // TODO: implement WeaponInfoWnd::LoadPattern
    // RETRUXX_NOT_IMPLEMENTED;
    return 0;
}

void WeaponInfoWnd::UpdateIco()
{
    RETRUXX_NOT_IMPLEMENTED;
}

bool WeaponInfoWnd::IsGunReady() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int WeaponInfoWnd::CreateChildren(WeaponInfoList::Type)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORTS_BEGIN(WeaponGroupChoiceDlg)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(WeaponGroupChoiceDlg);

WeaponGroupChoiceDlg::AuxInfo::AuxInfo()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* WeaponGroupChoiceDlg::Clone()
{
    RETRUXX_NOT_IMPLEMENTED;
}

CStr WeaponGroupChoiceDlg::CreateTooltipForWeaponGroup(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* WeaponGroupChoiceDlg::CreateObject()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* WeaponGroupChoiceDlg::GetBaseClass()
{
    return RT_CLASS_LOCAL(ModalWnd);
}

WeaponGroupChoiceDlg::~WeaponGroupChoiceDlg()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* WeaponGroupChoiceDlg::GetClass() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

WeaponGroupChoiceDlg::WeaponGroupChoiceDlg()
{
    RETRUXX_NOT_IMPLEMENTED;
}

WeaponGroupChoiceDlg::WeaponGroupChoiceDlg(WeaponGroupChoiceDlg const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int WeaponGroupChoiceDlg::GameDataSetup()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void WeaponGroupChoiceDlg::SelectButton(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int WeaponGroupChoiceDlg::OnKey(unsigned short, unsigned char, unsigned)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void WeaponGroupChoiceDlg::OnKeyBindingsChanged()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int WeaponGroupChoiceDlg::GameDataUpdate(void*, int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int WeaponGroupChoiceDlg::OnWndNotify(m3d::ui::Wnd*, unsigned, unsigned, m3d::AIParam const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}
