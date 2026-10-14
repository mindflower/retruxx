#pragma once
#include <core/ref_ptr.h>
#include <server/relationship.h>
#include <ui/ui.h>

namespace m3d
{
    namespace ui
    {
        class ImageWnd;
    }
}  // namespace m3d

class BossIndicatorWnd;
class SpeedometerWnd;
class IgrokaMochatWnd;
class MainCursorWnd;
class TargetInfoWnd;
class CounterWnd;
class FadingMsgList;
class WeaponInfoList;
class DamageInfoWnd;
class RadarWnd;

namespace ai
{
    class Obj;
}

class MainGameInterfaceWnd : public m3d::ui::Wnd
{
public:
    void CheckAndShowCounterWnd();
    void CheckAndShowTargetInfoWnd(bool bForceRemove);
    void CheckAndShowMainCursorWnd();
    int SetupForBoss(int bossId);
    int GetBossId() const;
    ai::Obj const* GetBoss() const;

    struct AuxInfo
    {
        CStr m_wndNearbyChestsIcoName;
        CStr m_wndNearbyTownIcoName;
        AuxInfo(MainGameInterfaceWnd::AuxInfo const&);
        AuxInfo();
    };

protected:
    virtual int GameDataSetup() override;
    virtual int GameDataClear(bool beforeContinouisLevel) override;
    virtual int GameDataUpdate(void* data, int dataType) override;
    virtual int OnBeforeAddToWndStation() override;
    virtual int OnBeforeRemoveFromWndStation() override;
    virtual int OnAfterRemoveFromWndStation() override;
    virtual int GameDataSave(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* guiNode) override;
    virtual int GameDataLoad(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* guiNode) override;
    void OnNewFrame();
    void UpdateContours();
    void UpdateInfoContour();
    void UpdateCapturedContour();
    void ShowInfoContour(int infoObjId);
    void ShowCapturedContour(int capturedObjId);
    void ClearContours();
    void ClearInfoContour();
    void ClearCapturedContour();
    void ShowContour(int targetObjId, unsigned int color, float size);
    void HideContour(int targetObjId);
    int GetInfoObjId() const;
    int GetCapturedObjId() const;
    unsigned int GetColorForInfoContour(int infoObjId) const;
    void CheckAndShowNearbyChestsIco();
    void CheckAndShowNearbyTownIco();
    void OnNearbyChests(void* data);
    void OnApproachTown(void* data);
    void OnTownRuined(void* data);
    bool IsInBossMode() const;
    void CheckAndShowBossIndicator();
    void OnBossModeChanged();
    void UpdateBossMode();
    ref_ptr<RadarWnd> m_wndRadar;
    ref_ptr<DamageInfoWnd> m_wndDamageInfo;
    ref_ptr<WeaponInfoList> m_wndWeaponInfoList;
    ref_ptr<FadingMsgList> m_wndFadingMsgList;
    ref_ptr<FadingMsgList> m_wndImportantFadingMsgList;
    ref_ptr<CounterWnd> m_wndCounter;
    ref_ptr<TargetInfoWnd> m_wndTargetInfo;
    ref_ptr<MainCursorWnd> m_wndMainCursor;
    ref_ptr<IgrokaMochatWnd> m_wndIgrokaMochat;
    ref_ptr<SpeedometerWnd> m_wndSpeedometer;
    ref_ptr<BossIndicatorWnd> m_wndBossIndicator;
    int m_contouredInfoObjId;
    int m_contouredCapturedObjId;
    ai::eTolerance m_oldInfoObjTolerance;
    m3d::ui::ImageWnd* m_wndNearbyChestsIco;
    m3d::ui::ImageWnd* m_wndNearbyTownIco;
    MainGameInterfaceWnd::AuxInfo m_aif;
    int m_nearbyTownId;
    bool m_bNearbyChests;
    int m_bossId;
    MainGameInterfaceWnd();
    MainGameInterfaceWnd(MainGameInterfaceWnd const& rhs);

public:
    virtual ~MainGameInterfaceWnd() override;
    virtual m3d::Object* Clone() override;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override;
    static m3d::Class m_classMainGameInterfaceWnd;
};
