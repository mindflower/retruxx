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

class MainGameInterfaceWnd : public m3d::ui::Wnd
{
public:
    void CheckAndShowCounterWnd();
    void CheckAndShowTargetInfoWnd(bool bForceRemove);
    void CheckAndShowMainCursorWnd();
    int SetupForBoss(int bossId);
    int GetBossId() const;
    const ai::Obj* GetBoss() const;

    struct AuxInfo
    {
        /* 0x0000 */ CStr m_wndNearbyChestsIcoName;
        /* 0x000c */ CStr m_wndNearbyTownIcoName;
        AuxInfo(const MainGameInterfaceWnd::AuxInfo&);
        AuxInfo();
    }; /* size: 0x0018 */

protected:
    virtual int GameDataSetup() override /* 0x104 */;
    virtual int GameDataClear(bool beforeContinouisLevel) override /* 0x108 */;
    virtual int GameDataUpdate(void* data, int dataType) override /* 0x10c */;
    virtual int OnBeforeAddToWndStation() override /* 0x68 */;
    virtual int OnBeforeRemoveFromWndStation() override /* 0x70 */;
    virtual int OnAfterRemoveFromWndStation() override /* 0x74 */;
    virtual int GameDataSave(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* guiNode) override /* 0x110 */;
    virtual int GameDataLoad(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* guiNode) override /* 0x114 */;
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
    /* 0x0220 */ ref_ptr<RadarWnd> m_wndRadar;
    /* 0x0224 */ ref_ptr<DamageInfoWnd> m_wndDamageInfo;
    /* 0x0228 */ ref_ptr<WeaponInfoList> m_wndWeaponInfoList;
    /* 0x022c */ ref_ptr<FadingMsgList> m_wndFadingMsgList;
    /* 0x0230 */ ref_ptr<FadingMsgList> m_wndImportantFadingMsgList;
    /* 0x0234 */ ref_ptr<CounterWnd> m_wndCounter;
    /* 0x0238 */ ref_ptr<TargetInfoWnd> m_wndTargetInfo;
    /* 0x023c */ ref_ptr<MainCursorWnd> m_wndMainCursor;
    /* 0x0240 */ ref_ptr<IgrokaMochatWnd> m_wndIgrokaMochat;
    /* 0x0244 */ ref_ptr<SpeedometerWnd> m_wndSpeedometer;
    /* 0x0248 */ ref_ptr<BossIndicatorWnd> m_wndBossIndicator;
    /* 0x024c */ int m_contouredInfoObjId;
    /* 0x0250 */ int m_contouredCapturedObjId;
    /* 0x0254 */ ai::eTolerance m_oldInfoObjTolerance;
    /* 0x0258 */ m3d::ui::ImageWnd* m_wndNearbyChestsIco;
    /* 0x025c */ m3d::ui::ImageWnd* m_wndNearbyTownIco;
    /* 0x0260 */ MainGameInterfaceWnd::AuxInfo m_aif;
    /* 0x0278 */ int m_nearbyTownId;
    /* 0x027c */ bool m_bNearbyChests;
    /* 0x027d */ char Padding_78[3];
    /* 0x0280 */ int m_bossId;
    MainGameInterfaceWnd();
    MainGameInterfaceWnd(const MainGameInterfaceWnd& rhs);

public:
    virtual ~MainGameInterfaceWnd() override /* 0x00 */;
    virtual m3d::Object* Clone() override /* 0x04 */;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override /* 0x34 */;
    static m3d::Class m_classMainGameInterfaceWnd;
}; /* size: 0x0284 */
