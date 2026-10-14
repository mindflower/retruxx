#pragma once
#include <game/uimisc/guihelper.h>
#include <game/uimisc/navpoint.h>
#include <ui/button.h>

class NavPointButton : public m3d::ui::ButtonWnd
{
public:
    virtual int Create(CStr const& caption, unsigned int style, BoundsBase<float> const& rc, unsigned int id) override
       ;
    int SetupForQuest(help::QuestType questType, int questId);
    int GetNavPointId() const;
    NavPoint::NavPointType GetNavPointType() const;
    static CStr __fastcall NavPointType2Str(NavPoint::NavPointType npType);

    struct AuxInfo
    {
        CStr m_strNpButton;
        CStr m_strTypeMainQuest;
        CStr m_strTypeUserQuest;
        CStr m_strTypeUserLocation;
        CStr m_strIdAddNavPoint;
        CStr m_strIdDeleteNavPoint;
        CStr m_texIdOut;
        CStr m_texIdIn;
        CStr m_texIdDown;
        PointBase<float> m_npTexSz;
        AuxInfo(NavPointButton::AuxInfo const&);
        AuxInfo();
    };

protected:
    virtual void OnNcPaint(m3d::ui::DrawInfo const& di, unsigned int clr) override;
    virtual int GameDataClear(bool beforeContinuousLevel) override;
    void FullUpdate();
    void UpdateNpTexture();
    NavPoint const* GetNavPoint() const;
    NavPoint::NavPointType GetNavPointTypeByQuest(help::QuestType questType, int questId) const;
    void UpdateTooltip();
    int m_npId;
    NavPoint::NavPointType m_npType;
    static NavPointButton::AuxInfo m_aif;
    m3d::rend::TexHandle m_npTex;
    NavPointButton();
    NavPointButton(NavPointButton const& rhs);

public:
    virtual ~NavPointButton() override;
    virtual m3d::Object* Clone() override;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override;
    static m3d::Class m_classNavPointButton;
};
