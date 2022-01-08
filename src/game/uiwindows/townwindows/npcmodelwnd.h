#pragma once

class NpcModelWnd :  public ItemModelWnd
{
public:
    static struct m3d::Class * GetBaseClass();
    virtual class m3d::Object * Clone();
    int GetNpcId() const ;
    class ai::Npc * GetNpc() const ;
    static class m3d::Object * CreateObject();
    virtual ~NpcModelWnd();
    int SetUpForNpc(int);
    virtual struct m3d::Class * GetClass() const ;
protected:
    virtual int GameDataClear(bool);
    void RandomizeAnimationFrame();
    virtual bool IsValid() const ;
    virtual int GameDataUpdate(void *,int);
    void OnNpcReplyShown(void *);
    virtual int OnNewFrame();
    void UpdateAnimation();
    NpcModelWnd();
    NpcModelWnd(class NpcModelWnd const &);
private:
    int m_npcId;
};
