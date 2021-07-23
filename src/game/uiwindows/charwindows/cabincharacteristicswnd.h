#pragma once

class CabinCharacteristicsWnd :  public CBCharacteristicsWnd
{
public:
    void SetupForCabin(int);
    static class m3d::Object * __fastcall CreateObject();
    virtual struct m3d::Class * GetClass() const ;
    virtual class m3d::Object * Clone();
    static struct m3d::Class * __fastcall GetBaseClass();
    virtual ~CabinCharacteristicsWnd();
protected:
    void SetupForCB(int);
    class ai::Cabin const * GetCabin() const ;
    CabinCharacteristicsWnd();
    CabinCharacteristicsWnd(class CabinCharacteristicsWnd const &);
    virtual void ClearCharacteristics();
    virtual void UpdateCharacteristics();
    virtual int GameDataSetup();
private:
    GetBaseClass();
    m3d::ui::Wnd *m_wndMaxSpeedVal;
    m3d::ui::Wnd *m_wndTorqueVal;
    m3d::ui::Wnd *m_wndControlVal;
    CabinCharacteristicsWnd::CabinAuxInfo m_cabinAif;
};
