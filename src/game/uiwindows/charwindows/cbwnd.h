#pragma once
#include "childpanel.h"
#include "inventorywnd.h"
#include <map>
#include <core/ref_ptr.h>

namespace m3d
{
    namespace ui
    {
        class ImageWnd;
    }
}

class VehiclePartWnd;

class CBWnd : public ChildPanel
{
public:
    class CBAuxInfo
    {
    public:
        CBAuxInfo();

    private:
        CStr m_vehiclePartPaneName;
        CStr m_wndHidePictureBgName;
        CStr m_wndDisabledBgName;
    };

public:
    virtual void SetPartId(int);
    int GetPartId() const;
    virtual void SetVehicleId(int);
    static m3d::Class* GetBaseClass();
    virtual ~CBWnd();
    virtual m3d::Class* GetRtClass() const;

protected:
    virtual InventoryWnd::VehicleType GetVehicleTypeByGuiId(int) const;
    void UpdateOnVehiclepartChanged(CStr const&);
    virtual void UpdateOnMainPartChanged();
    virtual void SetupChildVehicleParts();
    virtual int GameDataClear(bool);
    virtual int GameDataSetup();
    virtual void ClearChildVehicleParts();
    virtual int GameDataUpdate(void*, int);
    virtual int OnBeforeAddToWndStation();
    virtual int OnAfterRemoveFromWndStation();
    virtual void OnFinishTrade();
    virtual void FullUpdate();
    CBWnd();
    void SetDisabledLook(bool);
    virtual int CreateChildVehiclePartWindow(CStr const&, PointBase<float> const&);

public:
    RT_CLASS_DECLARE(CBWnd);

private:
    CBWnd::CBAuxInfo m_cbAif;
    CStr m_mainPartName;
    std::map<CStr, ref_ptr<VehiclePartWnd>> m_wndChildVehicleParts;
    InventoryWnd::VehicleType m_vehicleType;
    int m_mainPartId;
    m3d::ui::ImageWnd* m_wndHidePictureBg;
    m3d::ui::ImageWnd* m_wndDisabledBg;
};
