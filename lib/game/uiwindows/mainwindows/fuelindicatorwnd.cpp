#include "fuelindicatorwnd.h"

#include "core/kernel.h"
#include "core/log.h"
#include "core/timer.h"

#include "server/objects/base/objcontainer.h"
#include "server/objects/vehicle.h"

#include "ui/image.h"
#include "ui/progressbarwnd.h"

RT_CLASS_EXPORTS_BEGIN(FuelIndicatorWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(FuelIndicatorWnd);

FuelIndicatorWnd::AuxInfo::AuxInfo()
{
    // RVA 0x124C20
    m_imgLowFuelIcoName = "imgLowFuelIco";
    m_wndProgressBarName = "wndProgressBar";
}

void FuelIndicatorWnd::SetVehicleId(int vehicleId)
{
    // RVA 0x125680
    bool const wasSetup = (m_gameDataFlags & 1) != 0;
    m_vehicleId = vehicleId;
    if (wasSetup)
    {
        if (ai::Vehicle* vehicle = GetVehicle())
        {
            m_wndProgressBar->SetMaxValue(vehicle->GetMaxFuel());
        }
        else
        {
            m_wndProgressBar->SetMaxValue(0.0f);
            m_imgLowFuelIco->ShowWindow(false);
        }
    }
}

m3d::Class* FuelIndicatorWnd::GetClass() const
{
    return RT_CLASS_LOCAL(FuelIndicatorWnd);
}

FuelIndicatorWnd::~FuelIndicatorWnd()
{
    // RVA 0x124DA0 - the AuxInfo CStrs and the Wnd base (which owns/destroys
    // the child m_imgLowFuelIco/m_wndProgressBar windows) clean up automatically.
}

m3d::Object* FuelIndicatorWnd::Clone()
{
    // RVA 0x124B90
    return new FuelIndicatorWnd(*this);
}

m3d::Class* FuelIndicatorWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(Wnd);
}

int FuelIndicatorWnd::CreateFromPattern(m3d::ui::Wnd* p, bool deleteSrc)
{
    // RVA 0x124E10
    if (!p || !p->IsKindOf(&m3d::ui::ProgressBarWnd::m_classProgressBarWnd))
    {
        M3D_LOG_INFO("FuelIndicatorWnd: error to create - invalid pattern wnd");
        return 0;
    }
    auto* pattern = static_cast<m3d::ui::ProgressBarWnd*>(p);

    auto* parent = static_cast<m3d::ui::Wnd*>(pattern->GetParent());
    if (!parent)
    {
        M3D_LOG_INFO("FuelIndicatorWnd: error to create - invalid parent wnd");
        return 0;
    }

    m3d::Object* icoObj = parent->GetChildByName(m_aif.m_imgLowFuelIcoName);
    if (!icoObj || !icoObj->IsKindOf(&m3d::ui::ImageWnd::m_classImageWnd))
    {
        M3D_LOG_INFO(
            CStr("FuelIndicatorWnd: error create - cannot find control ") + m_aif.m_imgLowFuelIcoName +
            " or control class mismatches");
        return 0;
    }
    m_imgLowFuelIco = static_cast<m3d::ui::ImageWnd*>(icoObj);

    m_wndProgressBar = static_cast<m3d::ui::ProgressBarWnd*>(M3D_KERNEL->New("ProgressBarWnd"));
    if (!m_wndProgressBar)
    {
        M3D_LOG_INFO("FuelIndicatorWnd: error create");
        return 0;
    }

    if (!m_wndProgressBar->Create({}, pattern->GetStyle(), pattern->GetBounds(), pattern->GetId()))
    {
        M3D_LOG_INFO("FuelIndicatorWnd: error create");
        return 0;
    }
    m_wndProgressBar->SetPane(pattern->GetPaneName());
    m_wndProgressBar->SetPaneFlags(pattern->GetPaneFlags());
    m_wndProgressBar->SetBackground(pattern->GetBackground());
    m_wndProgressBar->SetBarColor(pattern->GetBarColor());
    m_wndProgressBar->SetBarTexture(pattern->GetBarTexture());
    m_wndProgressBar->SetOrientation(pattern->GetOrientation());
    m_wndProgressBar->SetNumOfSteps(pattern->GetNumOfSteps());
    m_wndProgressBar->SetTextStyle(pattern->GetTextStyle());
    m_wndProgressBar->SetTextureStyle(pattern->GetTextureStyle());

    // NOTE: the shipped code derives this same bounding rect via a maze of
    // reused stack slots that the decompiler cannot separate from unrelated
    // CStr temporaries at the same addresses; reconstructed here as the
    // (clearly intended) union of the icon and bar rects.
    BoundsBase<float> const icoB = m_imgLowFuelIco->GetBounds();
    BoundsBase<float> const barB = m_wndProgressBar->GetBounds();
    BoundsBase<float> compositeB;
    compositeB.x0 = (icoB.x0 <= barB.x0) ? icoB.x0 : barB.x0;
    compositeB.y0 = (icoB.y0 <= barB.y0) ? icoB.y0 : barB.y0;
    float const right = (icoB.x0 + icoB.width >= barB.x0 + barB.width) ? icoB.x0 + icoB.width : barB.x0 + barB.width;
    float const bottom =
        (icoB.y0 + icoB.height >= barB.y0 + barB.height) ? icoB.y0 + icoB.height : barB.y0 + barB.height;
    compositeB.width = right - compositeB.x0;
    compositeB.height = bottom - compositeB.y0;

    if (!Create({}, 832u, compositeB, 0))
    {
        M3D_LOG_INFO("FuelIndicatorWnd: error create");
        return 0;
    }

    parent->AddChild(this);
    parent->RemoveChild(m_imgLowFuelIco);
    AddChild(m_imgLowFuelIco);
    if (deleteSrc)
    {
        pattern->DecRef();
    }
    AddChild(m_wndProgressBar);

    PointBase<float> const icoPt = ToWindow(parent->ToScreen(PointBase<float>{icoB.x0, icoB.y0}));
    BoundsBase<float> newIcoB = icoB;
    newIcoB.x0 = icoPt.x;
    newIcoB.y0 = icoPt.y;
    m_imgLowFuelIco->SetBounds(newIcoB, true);

    PointBase<float> const barPt = ToWindow(parent->ToScreen(PointBase<float>{barB.x0, barB.y0}));
    BoundsBase<float> newBarB = barB;
    newBarB.x0 = barPt.x;
    newBarB.y0 = barPt.y;
    m_wndProgressBar->SetBounds(newBarB, true);

    m_gameDataFlags |= 1u;
    M3D_LOG_INFO("FuelIndicatorWnd: is created successfully");
    return 1;
}

m3d::Object* FuelIndicatorWnd::CreateObject()
{
    // RVA 0x124BF0
    return new FuelIndicatorWnd;
}

FuelIndicatorWnd::FuelIndicatorWnd(FuelIndicatorWnd const&) : FuelIndicatorWnd()
{
    // NOTE: the shipped copy ctor default-constructs the base and the AuxInfo
    // but never initializes m_imgLowFuelIco/m_wndProgressBar/m_vehicleId/etc,
    // leaving them as uninitialized garbage; delegating to the default ctor
    // here avoids dereferencing an uninitialized pointer while still copying
    // nothing from the source, matching the original's observable behavior.
}

FuelIndicatorWnd::FuelIndicatorWnd()
{
    // RVA 0x124CB0
    m_imgLowFuelIco = nullptr;
    m_wndProgressBar = nullptr;
    // NOTE: the shipped ctor never initializes m_vehicleId; defaulted to -1
    // here for safety (GetVehicle() treats -1 as "no vehicle").
    m_vehicleId = -1;
    m_lowFuelTwinkleStart = 0;
    m_lowFuelThreshold = 0.25f;
    m_lowFuelTwinklePeriod = 300;
}

int FuelIndicatorWnd::GameDataUpdate(void*, int dataType)
{
    if ((m_gameDataFlags & 1) == 0)
    {
        return 0;
    }
    if (dataType == 89)
    {
        UpdateProgressBar();
        UpdateLowFuelIco();
    }
    return 1;
}

int FuelIndicatorWnd::UpdateProgressBar()
{
    // RVA 0x1254D0
    if ((m_gameDataFlags & 1) == 0)
    {
        return 0;
    }
    ai::Vehicle* vehicle = GetVehicle();
    if (!vehicle)
    {
        return 0;
    }
    m_wndProgressBar->SetMaxValue(vehicle->GetMaxFuel());
    m_wndProgressBar->SetCurValue(vehicle->GetFuel());
    return 1;
}

ai::Vehicle* FuelIndicatorWnd::GetVehicle() const
{
    // RVA 0x125620
    if (m_vehicleId == -1)
    {
        return nullptr;
    }
    ai::Obj* obj = ai::theObjects->GetEntityByObjId(m_vehicleId);
    return (obj && obj->IsKindOf(&ai::Vehicle::m_classVehicle)) ? static_cast<ai::Vehicle*>(obj) : nullptr;
}

int FuelIndicatorWnd::UpdateLowFuelIco()
{
    // RVA 0x125530
    if ((m_gameDataFlags & 1) == 0)
    {
        return 0;
    }
    ai::Vehicle* vehicle = GetVehicle();
    if (!vehicle)
    {
        return 0;
    }

    float const pct = vehicle->GetFuel() / vehicle->GetMaxFuel();
    if (pct <= m_lowFuelThreshold)
    {
        if (pct > 0.0f)
        {
            unsigned int const curTime = M3D_KERNEL->GetTimer().GetCurTime();
            if (curTime - m_lowFuelTwinkleStart >= m_lowFuelTwinklePeriod)
            {
                m_imgLowFuelIco->ShowWindow(!m_imgLowFuelIco->IsVisible());
                m_lowFuelTwinkleStart = M3D_KERNEL->GetTimer().GetCurTime();
            }
        }
        else
        {
            m_imgLowFuelIco->ShowWindow(true);
        }
    }
    else
    {
        m_imgLowFuelIco->ShowWindow(false);
    }
    return 1;
}

int FuelIndicatorWnd::UpdateOnNewFrame()
{
    // RVA 0x1254B0
    int const barOk = UpdateProgressBar() & 1;
    return barOk & UpdateLowFuelIco();
}
