#pragma once

class NavPointButton :  public m3d::ui::ButtonWnd
{
public:
    int SetupForQuest(enum help::QuestType,int);
    virtual class m3d::Object * Clone();
    virtual ~NavPointButton();
    static class m3d::Object * __fastcall CreateObject();
    static struct m3d::Class * __fastcall GetBaseClass();
    virtual int Create(CStr const &,unsigned int,struct BoundsBase<float> const &,unsigned int);
    int GetNavPointId() const ;
    enum NavPoint::NavPointType GetNavPointType() const ;
    static CStr __fastcall NavPointType2Str(enum NavPoint::NavPointType);
    virtual struct m3d::Class * GetClass() const ;
protected:
    NavPointButton();
    NavPointButton(class NavPointButton const &);
    void FullUpdate();
    class NavPoint const * GetNavPoint() const ;
    void UpdateNpTexture();
    enum NavPoint::NavPointType GetNavPointTypeByQuest(enum help::QuestType,int) const ;
    virtual int GameDataClear(bool);
    virtual void OnNcPaint(struct m3d::ui::DrawInfo const &,unsigned int);
    void UpdateTooltip();
private:
    int m_npId;
    NavPoint::NavPointType m_npType;
    m3d::rend::TexHandle m_npTex;
};
