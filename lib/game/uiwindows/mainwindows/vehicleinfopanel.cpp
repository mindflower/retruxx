#include "vehicleinfopanel.h"
#include "speedometerwnd.h"
#include "fuelindicatorwnd.h"

#include "core/kernel.h"
#include "core/log.h"
#include "game/m3dgame.h"
#include "ui/progressbarwnd.h"

#include "server/objects/player.h"
#include "server/objects/vehicle.h"

RT_CLASS_EXPORTS_BEGIN(VehicleInfoPanel)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(VehicleInfoPanel);

VehicleInfoPanel::AuxInfo::AuxInfo()
{
    // RVA 0x137650
    m_wndFuelName = "wndFuel";
}

m3d::Class* VehicleInfoPanel::GetBaseClass()
{
    return RT_CLASS_LOCAL(Wnd);
}

VehicleInfoPanel::~VehicleInfoPanel()
{
    // RVA 0x137790 - m_aif's CStr, the ref_ptr<...> members (which release
    // their reference), and the Wnd base all clean up automatically.
}

m3d::Object* VehicleInfoPanel::CreateObject()
{
    // RVA 0x137620
    return new VehicleInfoPanel;
}

m3d::Object* VehicleInfoPanel::Clone()
{
    // RVA 0x1375B0
    return new VehicleInfoPanel(*this);
}

m3d::Class* VehicleInfoPanel::GetClass() const
{
    return RT_CLASS_LOCAL(VehicleInfoPanel);
}

VehicleInfoPanel::VehicleInfoPanel(VehicleInfoPanel const&) : VehicleInfoPanel()
{
    // Matches the original (RVA 0x137760): the copy ctor default-constructs
    // the base, the AuxInfo, and null ref_ptrs; nothing is copied from the
    // source.
}

VehicleInfoPanel::VehicleInfoPanel() = default;

int VehicleInfoPanel::GameDataSetup()
{
    // RVA 0x137810
    using namespace m3d::ui;

    if ((m_gameDataFlags & 2) == 0)
    {
        bool ok = false;

        if (auto* child = GetChildByName(m_aif.m_wndFuelName);
            child && child->IsKindOf(&ProgressBarWnd::m_classProgressBarWnd))
        {
            m_wndFuel = static_cast<FuelIndicatorWnd*>(M3D_KERNEL->New("FuelIndicatorWnd"));
            if (m_wndFuel)
            {
                if (m_wndFuel->CreateFromPattern(static_cast<Wnd*>(child), true))
                {
                    M3D_APP->m_pInterfaceManager->AddWindowById(m_wndFuel.get(), 29, true, true);
                    ok = true;
                }
                else
                {
                    M3D_LOG_INFO("Make control error: cannot create " + m_aif.m_wndFuelName + " from pattern class");
                }
            }
            else
            {
                M3D_LOG_INFO(
                    "Make control error: cannot create " + m_aif.m_wndFuelName +
                    " - cannot find rtti class FuelIndicatorWnd");
            }
        }
        else
        {
            M3D_LOG_INFO("Make control error: control " + m_aif.m_wndFuelName + " is not found or incorrect type");
        }

        auto speedometerWnd = M3D_APP->m_pInterfaceManager->GetWindow(30);
        if (speedometerWnd && speedometerWnd->IsKindOf(&SpeedometerWnd::m_classSpeedometerWnd))
        {
            m_wndSpeedometer = static_cast<SpeedometerWnd*>(speedometerWnd.get());

            // The speedometer is an existing top-level window (screen-space
            // bounds); reposition it to be relative to this panel and adopt
            // it as a child.
            auto const bounds = m_wndSpeedometer->GetBounds();
            auto const thisScreenOrigin = ToScreen(PointBase<float>{0.0f, 0.0f});
            auto newBounds = bounds;
            newBounds.x0 = bounds.x0 - thisScreenOrigin.x;
            newBounds.y0 = bounds.y0 - thisScreenOrigin.y;
            m_wndSpeedometer->SetBounds(newBounds, false);

            AddChild(m_wndSpeedometer.get());
            if (ok)
            {
                m_gameDataFlags |= 1u;
            }
        }
    }

    if ((m_gameDataFlags & 1) != 0)
    {
        return 1;
    }
    M3D_LOG_INFO("VehicleInfoPanel: error - fail to init because of a bad resource");
    return 0;
}

void VehicleInfoPanel::OnPlayerVehicleChanged()
{
    // RVA 0x137DA0
    if ((m_gameDataFlags & 1) != 0)
    {
        ai::Vehicle* vehicle = ai::thePlayer ? ai::thePlayer->GetVehicle() : nullptr;
        int const vehicleId = vehicle ? vehicle->GetId() : -1;
        m_wndFuel->SetVehicleId(vehicleId);
    }
}

int VehicleInfoPanel::GameDataUpdate(void*, int dataType)
{
    // RVA 0x137D70
    if ((m_gameDataFlags & 1) == 0)
    {
        return 0;
    }
    if (dataType == 64)
    {
        OnPlayerVehicleChanged();
    }
    return 1;
}
