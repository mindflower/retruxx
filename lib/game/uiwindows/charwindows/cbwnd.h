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
    virtual void SetVehicleId(int vehicleId) override /* 0x11c */;
    virtual void SetPartId(int partId) /* 0x128 */;
    int GetPartId() const;

    struct CBAuxInfo
    {
        /* 0x0000 */ CStr m_vehiclePartPaneName;
        /* 0x000c */ CStr m_wndHidePictureBgName;
        /* 0x0018 */ CStr m_wndDisabledBgName;
        CBAuxInfo(const CBWnd::CBAuxInfo&);
        CBAuxInfo();
    }; /* size: 0x0024 */

    using VehiclePartMap = std::map<CStr, ref_ptr<VehiclePartWnd>, std::less<CStr>, std::allocator<std::pair<CStr const, ref_ptr<VehiclePartWnd> > > >;
    using VehiclePartPair = std::pair<CStr, ref_ptr<VehiclePartWnd> >;

protected:
    virtual int GameDataSetup() override /* 0x00 */;
    virtual int GameDataUpdate(void* data, int dataType) override /* 0x10c */;
    virtual int GameDataClear(bool beforeContinuousLevel) override /* 0x108 */;
    virtual int OnBeforeAddToWndStation() override /* 0x00 */;
    virtual int OnAfterRemoveFromWndStation() override /* 0x00 */;
    virtual void FullUpdate() /* 0x12c */;
    void UpdateOnVehiclepartChanged(const CStr& partName);
    virtual void UpdateOnMainPartChanged() /* 0x130 */;
    virtual void SetupChildVehicleParts() /* 0x134 */;
    virtual void ClearChildVehicleParts() /* 0x138 */;
    virtual int CreateChildVehiclePartWindow(const CStr& partName, const PointBase<float>& origin) /* 0x13c */;
    virtual InventoryWnd::VehicleType GetVehicleTypeByGuiId(int guiId) const = 0 /* 0x140 */;
    virtual void OnFinishTrade() /* 0x144 */;
    void SetDisabledLook(bool bDisabled);
    /* 0x0224 */ CBWnd::CBAuxInfo m_cbAif;
    /* 0x0248 */ CStr m_mainPartName;
    /* 0x0254 */ std::map<CStr, ref_ptr<VehiclePartWnd>, std::less<CStr>, std::allocator<std::pair<CStr const, ref_ptr<VehiclePartWnd> > > > m_wndChildVehicleParts;
    /* 0x0260 */ InventoryWnd::VehicleType m_vehicleType;
    /* 0x0264 */ int m_mainPartId;
    /* 0x0268 */ m3d::ui::ImageWnd* m_wndHidePictureBg;
    /* 0x026c */ m3d::ui::ImageWnd* m_wndDisabledBg;
    CBWnd(const CBWnd&);
    CBWnd();

public:
    virtual  ~CBWnd() override /* 0x00 */;
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetRtClass() const /* 0x148 */;
    static m3d::Class m_classCBWnd;
}; /* size: 0x0270 */
