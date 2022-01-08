#pragma once

class DevideDlg :  public m3d::ui::ModalWnd
{
public:
    virtual struct m3d::Class * GetClass() const ;
    static class m3d::Object * CreateObject();
    virtual int GameDataSetup();
    void UpdateLabels();
    virtual class m3d::Object * Clone();
    virtual int OnWndNotify(class m3d::ui::Wnd *,unsigned int,unsigned int,class m3d::AIParam const &);
    int GetDragAmount();
    int GetOriginalAmount();
    virtual ~DevideDlg();
    static struct m3d::Class * GetBaseClass();
    void SetAmount(int);
protected:
    DevideDlg(class DevideDlg const &);
    DevideDlg();
private:
    int m_maxAmount;
    m3d::ui::Wnd *m_lblOriginalAmount;
    m3d::ui::Wnd *m_lblDragAmount;
    m3d::ui::SliderWnd *m_slider;
    m3d::ui::ButtonWnd *m_btnPlus;
    m3d::ui::ButtonWnd *m_btnMinus;
    DevideDlg::AuxInfo m_aif;
    bool m_isInited;
};
