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
    virtual void SetVehicleId(int vehicleId) override;
    virtual void SetPartId(int partId);
    int GetPartId() const;

    struct CBAuxInfo
    {
        CStr m_vehiclePartPaneName;
        CStr m_wndHidePictureBgName;
        CStr m_wndDisabledBgName;
        CBAuxInfo(const CBWnd::CBAuxInfo&);
        CBAuxInfo();
    };

    using VehiclePartMap = std::map<CStr, ref_ptr<VehiclePartWnd>, std::less<CStr>, std::allocator<std::pair<CStr const, ref_ptr<VehiclePartWnd> > > >;
    using VehiclePartPair = std::pair<CStr, ref_ptr<VehiclePartWnd> >;

protected:
    virtual int GameDataSetup() override;
    virtual int GameDataUpdate(void* data, int dataType) override;
    virtual int GameDataClear(bool beforeContinuousLevel) override;
    virtual int OnBeforeAddToWndStation() override;
    virtual int OnAfterRemoveFromWndStation() override;
    virtual void FullUpdate();
    void UpdateOnVehiclepartChanged(const CStr& partName);
    virtual void UpdateOnMainPartChanged();
    virtual void SetupChildVehicleParts();
    virtual void ClearChildVehicleParts();
    virtual int CreateChildVehiclePartWindow(const CStr& partName, const PointBase<float>& origin);
    virtual InventoryWnd::VehicleType GetVehicleTypeByGuiId(int guiId) const = 0;
    virtual void OnFinishTrade();
    void SetDisabledLook(bool bDisabled);
    CBWnd::CBAuxInfo m_cbAif;
    CStr m_mainPartName;
    std::map<CStr, ref_ptr<VehiclePartWnd>, std::less<CStr>, std::allocator<std::pair<CStr const, ref_ptr<VehiclePartWnd> > > > m_wndChildVehicleParts;
    InventoryWnd::VehicleType m_vehicleType;
    int m_mainPartId;
    m3d::ui::ImageWnd* m_wndHidePictureBg;
    m3d::ui::ImageWnd* m_wndDisabledBg;
    CBWnd(const CBWnd&);
    CBWnd();

public:
    virtual  ~CBWnd() override;
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetRtClass() const;
    static m3d::Class m_classCBWnd;
};
