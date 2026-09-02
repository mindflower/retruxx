#include "videownd.h"

#include <core/kernel.h>
#include <core/log.h>
#include <game/m3dgame.h>
#include <game/uiwindows/commonwindows/contextmodelwnd.h>
#include <server/objects/vehicle.h>

RT_CLASS_EXPORTS_BEGIN(VideoWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(VideoWnd);

VideoWnd::AuxInfo::AuxInfo()
{
    m_wndModelName = "wndModel";
}

m3d::Object* VideoWnd::CreateObject()
{
    return new VideoWnd;
}

int VideoWnd::ShowModelByPrototypeId(int prototypeId, ref_ptr<m3d::ui::Wnd> srcWindow)
{
    if ((m_gameDataFlags & 1) == 0)
    {
        return 0;
    }
    if (!IsChildOf(m3d::Application::g_pApp) || (m_style & 2) != 0 || (m_style & 0x80000) != 0)
    {
        return 0;
    }
    if (!srcWindow)
    {
        return 0;
    }
    m_srcWindow = srcWindow;
    return m_wndModel->ShowImageByPrototypeId0(prototypeId, true);
}

int VideoWnd::GetPrototypeId() const
{
    if ((m_gameDataFlags & 1) != 0)
    {
        return m_wndModel->GetPrototypeId();
    }
    return -1;
}

m3d::Class* VideoWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(ChildPanel);
}

m3d::Object* VideoWnd::Clone()
{
    return new VideoWnd(*this);
}

int VideoWnd::HideModel(ref_ptr<m3d::ui::Wnd> srcWindow)
{
    if ((m_gameDataFlags & 1) == 0)
    {
        return 0;
    }
    if (!srcWindow)
    {
        return 0;
    }
    if (!m_srcWindow || srcWindow.get() != m_srcWindow.get())
    {
        return 0;
    }

    m_srcWindow = nullptr;
    m_wndModel->DestroyObjToShow();
    m_wndModel->ClearCurrentModelWnd();
    return 1;
}

m3d::Class* VideoWnd::GetClass() const
{
    return RT_CLASS_LOCAL(VideoWnd);
}

int VideoWnd::GetObjId() const
{
    if ((m_gameDataFlags & 1) != 0)
    {
        return m_wndModel->GetObjId();
    }
    return -1;
}

int VideoWnd::ShowModelByObjId(int objId, ref_ptr<m3d::ui::Wnd> srcWindow)
{
    if ((m_gameDataFlags & 1) == 0)
    {
        return 0;
    }
    if (!IsChildOf(m3d::Application::g_pApp) || (m_style & 2) != 0 || (m_style & 0x80000) != 0)
    {
        return 0;
    }
    if (!srcWindow)
    {
        return 0;
    }
    m_srcWindow = srcWindow;
    return m_wndModel->ShowImageByObjId0(objId, true);
}

ref_ptr<m3d::ui::Wnd> VideoWnd::GetSrcWindow() const
{
    return m_srcWindow;
}

VideoWnd::~VideoWnd()
{
    if (m_wndModel)
    {
        m_wndModel->GameDataClear(false);
    }
    // m_srcWindow (ref_ptr) and m_aif (CStr) release themselves; ~ChildPanel / ~Wnd run.
}

int VideoWnd::GameDataSetup()
{
    if ((m_gameDataFlags & 2) == 0)
    {
        m3d::Object* child = GetChildByName(m_aif.m_wndModelName);
        if (child != nullptr && child->IsKindOf(&m3d::ui::Wnd::m_classWnd))
        {
            m_wndModel = static_cast<ContextModelWnd*>(M3D_KERNEL->New("ContextModelWnd"));
            if (m_wndModel)
            {
                if (m_wndModel->CreateFromPattern(static_cast<m3d::ui::Wnd*>(child), true))
                {
                    m_gameDataFlags |= 1u;
                }
                else
                {
                    M3D_LOG_INFO("Make control error: cannot create " + m_aif.m_wndModelName + " from pattern class");
                }
            }
            else
            {
                M3D_LOG_INFO(
                    "Make control error: cannot create " + m_aif.m_wndModelName +
                    " - cannot find rtti class ContextModelWnd");
            }
        }
        else
        {
            M3D_LOG_INFO("Make control error: control " + m_aif.m_wndModelName + " is not found or incorrect type");
        }
    }

    if ((m_gameDataFlags & 1) == 0)
    {
        M3D_LOG_INFO("VideoWnd: error - fail to init because of a bad resource");
        return 0;
    }
    return 1;
}

VideoWnd::VideoWnd()
{
    m_wndModel = nullptr;
    m_srcWindow = nullptr;
    m_bgMode = BGMODE_NUM_BGMODES;
}

VideoWnd::VideoWnd(VideoWnd const&) : VideoWnd()
{
}

bool VideoWnd::IsDisabled() const
{
    return (m_style & 2) != 0 || (m_style & 0x80000) != 0;
}

void VideoWnd::ShowBgModel(BgMode bgMode)
{
    HideModel(ref_ptr<m3d::ui::Wnd>(this));
    if (bgMode == BGMODE_BUYVEHICLE)
    {
        ai::Vehicle* vehicle = M3D_APP->m_pInterfaceManager->GetVehicleSellingInWorkshop();
        if (vehicle)
        {
            ShowModelByObjId(vehicle->GetId(), ref_ptr<m3d::ui::Wnd>(this));
        }
    }
}

void VideoWnd::OnNewFrame()
{
    UpdateBgMode();
}

int VideoWnd::GameDataClear(bool beforeContinuousLevel)
{
    if (m_wndModel)
    {
        m_wndModel->GameDataClear(beforeContinuousLevel);
    }
    return 1;
}

void VideoWnd::UpdateBgMode()
{
    BgMode const oldMode = m_bgMode;
    BgMode const curMode = DetectBgMode();
    m_bgMode = curMode;

    if (curMode == oldMode)
    {
        if (curMode != BGMODE_NUM_BGMODES)
        {
            UpdateBgModel(curMode);
        }
    }
    else if (curMode == BGMODE_NUM_BGMODES)
    {
        HideModel(ref_ptr<m3d::ui::Wnd>(this));
    }
    else
    {
        ShowBgModel(curMode);
    }
}

void VideoWnd::UpdateBgModel(BgMode bgMode)
{
    if ((m_gameDataFlags & 1) == 0 || bgMode != BGMODE_BUYVEHICLE)
    {
        return;
    }

    int const shownObjId = m_wndModel->GetObjId();
    int sellingObjId = -1;
    ai::Vehicle* vehicle = M3D_APP->m_pInterfaceManager->GetVehicleSellingInWorkshop();
    if (vehicle)
    {
        sellingObjId = vehicle->GetId();
    }

    if (sellingObjId != shownObjId)
    {
        if (sellingObjId == -1)
        {
            HideModel(ref_ptr<m3d::ui::Wnd>(this));
        }
        else
        {
            ShowModelByObjId(sellingObjId, ref_ptr<m3d::ui::Wnd>(this));
        }
    }
}

int VideoWnd::GameDataUpdate(void*, int dataType)
{
    if (dataType == 89)
    {
        UpdateBgMode();
    }
    return 1;
}

VideoWnd::BgMode VideoWnd::DetectBgMode() const
{
    if (m_srcWindow.get() != nullptr && m_srcWindow.get() != this)
    {
        return BGMODE_NUM_BGMODES;
    }

    struct
    {
        int windowId;
        BgMode mode;
    } const probes[] = {
        {73, BGMODE_BUYVEHICLE},
        {67, BGMODE_WORKSHOP},
        {66, BGMODE_SHOP},
        {63, BGMODE_GROUND},
    };

    for (auto const& probe : probes)
    {
        ref_ptr<m3d::ui::Wnd> wnd = M3D_APP->m_pInterfaceManager->GetWindow(probe.windowId);
        if (wnd && wnd->IsChildOf(m3d::Application::g_pApp))
        {
            return probe.mode;
        }
    }
    return BGMODE_NUM_BGMODES;
}
