#pragma once
#include <game/uiwindows/commonwindows/itemmodelwnd.h>

namespace ai
{
    class Npc;
}

class NpcModelWnd :  public ItemModelWnd
{
public:
    static m3d::Class * GetBaseClass();
    virtual m3d::Object * Clone();
    int GetNpcId() const ;
    ai::Npc * GetNpc() const ;
    static m3d::Object * CreateObject();
    virtual ~NpcModelWnd();
    int SetUpForNpc(int);
    virtual m3d::Class * GetClass() const ;

protected:
    virtual int GameDataClear(bool);
    void RandomizeAnimationFrame();
    virtual bool IsValid() const ;
    virtual int GameDataUpdate(void *,int);
    void OnNpcReplyShown(void *);
    virtual int OnNewFrame();
    void UpdateAnimation();
    NpcModelWnd();
    NpcModelWnd(NpcModelWnd const &);

public:
    RT_CLASS_DECLARE(NpcModelWnd);

private:
    int m_npcId;
};
