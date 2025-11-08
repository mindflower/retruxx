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
}

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

class MainGameInterfaceWnd :  public m3d::ui::Wnd
{
public:
    class AuxInfo
    {
    public:
        AuxInfo();

    private:
        CStr m_wndNearbyChestsIcoName;
        CStr m_wndNearbyTownIcoName;
    };

public:
    static m3d::Class * GetBaseClass();
    int GetBossId() const ;
    void CheckAndShowTargetInfoWnd(bool);
    virtual m3d::Object * Clone();
    ai::Obj const * GetBoss() const ;
    void CheckAndShowCounterWnd();
    void CheckAndShowMainCursorWnd();
    int SetupForBoss(int);
    virtual ~MainGameInterfaceWnd();

protected:
    MainGameInterfaceWnd();
    MainGameInterfaceWnd(MainGameInterfaceWnd const &);
    void CheckAndShowNearbyTownIco();
    virtual int GameDataSetup();
    virtual int OnBeforeAddToWndStation();
    void CheckAndShowNearbyChestsIco();
    void OnBossModeChanged();
    virtual int OnAfterRemoveFromWndStation();
    virtual int GameDataSave(m3d::cmn::XmlFile *, m3d::cmn::XmlNode *);
    void OnTownRuined(void *);
    void UpdateInfoContour();
    void HideContour(int);
    void ShowContour(int,unsigned int,float);
    void UpdateContours();
    unsigned int GetColorForInfoContour(int) const ;
    void ShowCapturedContour(int);
    void OnNearbyChests(void *);
    void ClearCapturedContour();
    void OnApproachTown(void *);
    int GetCapturedObjId() const ;
    void ClearContours();
    void CheckAndShowBossIndicator();
    virtual int GameDataLoad(m3d::cmn::XmlFile *, m3d::cmn::XmlNode *);
    virtual int GameDataUpdate(void *,int);
    virtual int OnBeforeRemoveFromWndStation();
    void UpdateBossMode();
    virtual int GameDataClear(bool);
    void UpdateCapturedContour();
    int GetInfoObjId() const ;
    void ShowInfoContour(int);
    void ClearInfoContour();
    bool IsInBossMode() const ;
    void OnNewFrame();

public:
    RT_CLASS_DECLARE(MainGameInterfaceWnd);

private:
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
    m3d::ui::ImageWnd *m_wndNearbyChestsIco;
    m3d::ui::ImageWnd *m_wndNearbyTownIco;
    MainGameInterfaceWnd::AuxInfo m_aif;
    int m_nearbyTownId;
    bool m_bNearbyChests;
    int m_bossId;
};
