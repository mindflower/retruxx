#pragma once
#include <game/uimisc/guihelper.h>
#include <game/uimisc/navpoint.h>
#include <ui/button.h>

class NavPointButton : public m3d::ui::ButtonWnd
{
public:
    virtual int Create(CStr const& caption, unsigned int style, BoundsBase<float> const& rc, unsigned int id) override
        /* 0x00 */;
    int SetupForQuest(help::QuestType questType, int questId);
    int GetNavPointId() const;
    NavPoint::NavPointType GetNavPointType() const;
    static CStr __fastcall NavPointType2Str(NavPoint::NavPointType npType);

    struct AuxInfo
    {
        /* 0x0000 */ CStr m_strNpButton;
        /* 0x000c */ CStr m_strTypeMainQuest;
        /* 0x0018 */ CStr m_strTypeUserQuest;
        /* 0x0024 */ CStr m_strTypeUserLocation;
        /* 0x0030 */ CStr m_strIdAddNavPoint;
        /* 0x003c */ CStr m_strIdDeleteNavPoint;
        /* 0x0048 */ CStr m_texIdOut;
        /* 0x0054 */ CStr m_texIdIn;
        /* 0x0060 */ CStr m_texIdDown;
        /* 0x006c */ PointBase<float> m_npTexSz;
        AuxInfo(NavPointButton::AuxInfo const&);
        AuxInfo();
    }; /* size: 0x0074 */

protected:
    virtual void OnNcPaint(m3d::ui::DrawInfo const& di, unsigned int clr) override /* 0x00 */;
    virtual int GameDataClear(bool beforeContinuousLevel) override /* 0x00 */;
    void FullUpdate();
    void UpdateNpTexture();
    NavPoint const* GetNavPoint() const;
    NavPoint::NavPointType GetNavPointTypeByQuest(help::QuestType questType, int questId) const;
    void UpdateTooltip();
    /* 0x023c */ int m_npId;
    /* 0x0240 */ NavPoint::NavPointType m_npType;
    static NavPointButton::AuxInfo m_aif;
    /* 0x0244 */ m3d::rend::TexHandle m_npTex;
    NavPointButton();
    NavPointButton(NavPointButton const& rhs);

public:
    virtual ~NavPointButton() override /* 0x00 */;
    virtual m3d::Object* Clone() override /* 0x00 */;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override /* 0x00 */;
    static m3d::Class m_classNavPointButton;
}; /* size: 0x0248 */
