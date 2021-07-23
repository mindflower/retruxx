#pragma once

class CounterWnd :  public m3d::ui::Wnd
{
public:
    virtual struct m3d::Class * GetClass() const ;
    static struct m3d::Class * __fastcall GetBaseClass();
    static class m3d::Object * __fastcall CreateObject();
    bool NeedShow() const ;
    virtual ~CounterWnd();
    virtual class m3d::Object * Clone();
protected:
    void ClearCounter();
    CounterWnd(class CounterWnd const &);
    CounterWnd();
    void OnStartLevel(void *);
    int SetupForHuntQuest(int);
    void GetMinAndSecFromTime(float,int &,int &) const ;
    virtual int GameDataClear(bool);
    class ai::DynamicQuestHunt const * GetQuestHunt() const ;
    virtual int GameDataSetup();
    void OnDynamicQuestStateChanged(void *);
    void CheckAndShow();
    float GetHuntTimeLeft() const ;
    virtual int GameDataUpdate(void *,int);
    void UpdateCounter();
    void OnNewFrame();
private:
    CounterWnd::AuxInfo m_aif;
    ElectronicDigitalWnd *m_wndCounter;
    CounterWnd::CounterType m_counterType;
    int m_dQuestId;
};
