#pragma once
#include <ui/ui.h>

namespace ai
{
    class DynamicQuestHunt;
}

class ElectronicDigitalWnd;

class CounterWnd :  public m3d::ui::Wnd
{
public:
    class AuxInfo
    {
    public:
        AuxInfo();

    private:
        CStr m_wndCounterName;
        CStr m_strHuntTime;
    };

    enum CounterType
    {
        COUNTERTYPE_QUEST_HUNT = 0x0,
        COUNTERTYPE_NUM_COUNTER_TYPES = 0x1,
        COUNTERTYPE_INVALID = 0x1,
    };

public:
    virtual m3d::Class * GetClass() const ;
    static m3d::Class * GetBaseClass();
    static m3d::Object * CreateObject();
    bool NeedShow() const ;
    virtual ~CounterWnd();
    virtual m3d::Object * Clone();

protected:
    void ClearCounter();
    CounterWnd(CounterWnd const &);
    CounterWnd();
    void OnStartLevel(void *);
    int SetupForHuntQuest(int);
    void GetMinAndSecFromTime(float,int &,int &) const ;
    virtual int GameDataClear(bool);
    ai::DynamicQuestHunt const * GetQuestHunt() const ;
    virtual int GameDataSetup();
    void OnDynamicQuestStateChanged(void *);
    void CheckAndShow();
    float GetHuntTimeLeft() const ;
    virtual int GameDataUpdate(void *,int);
    void UpdateCounter();
    void OnNewFrame();

public:
    RT_CLASS_DECLARE(CounterWnd);

private:
    CounterWnd::AuxInfo m_aif;
    ElectronicDigitalWnd *m_wndCounter;
    CounterWnd::CounterType m_counterType;
    int m_dQuestId;
};
