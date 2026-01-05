#pragma once
#include <game/uimisc/guihelper.h>
#include <game/uimisc/navpoint.h>
#include <ui/button.h>

class NavPointButton :  public m3d::ui::ButtonWnd
{
public:
    int SetupForQuest(help::QuestType,int);
    virtual m3d::Object * Clone();
    virtual ~NavPointButton();
    static m3d::Object * CreateObject();
    static m3d::Class * GetBaseClass();
    virtual int Create(CStr const &,unsigned int, BoundsBase<float> const &,unsigned int);
    int GetNavPointId() const ;
    NavPoint::NavPointType GetNavPointType() const ;
    static CStr __fastcall NavPointType2Str(NavPoint::NavPointType);
    virtual m3d::Class * GetClass() const ;

protected:
    NavPointButton();
    NavPointButton(NavPointButton const &);
    void FullUpdate();
    NavPoint const * GetNavPoint() const ;
    void UpdateNpTexture();
    NavPoint::NavPointType GetNavPointTypeByQuest(help::QuestType,int) const ;
    virtual int GameDataClear(bool);
    virtual void OnNcPaint(m3d::ui::DrawInfo const &,unsigned int);
    void UpdateTooltip();

public:
    RT_CLASS_DECLARE(NavPointButton);

private:
    int m_npId;
    NavPoint::NavPointType m_npType;
    m3d::rend::TexHandle m_npTex;
};
