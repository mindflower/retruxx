#pragma once
#include <game/uiwindows/commonwindows/itemmodelwnd.h>

namespace ai
{
    class Npc;
}

class NpcModelWnd : public ItemModelWnd
{
public:
    ai::Npc* GetNpc() const;
    int GetNpcId() const;
    int SetUpForNpc(int npcId);

protected:
    virtual int GameDataUpdate(void* data, int dataType) override;
    virtual int GameDataClear(bool beforeContinuousLevel) override;
    virtual int OnNewFrame() override;
    void OnNpcReplyShown(void* data);
    virtual bool IsValid() const override;
    void RandomizeAnimationFrame();
    void UpdateAnimation();
    int m_npcId;
    NpcModelWnd();
    NpcModelWnd(NpcModelWnd const& rhs);

public:
    virtual ~NpcModelWnd() override;
    virtual m3d::Object* Clone() override;
    static m3d::Object* CreateObject();
    static m3d::Class* GetBaseClass();
    virtual m3d::Class* GetClass() const override;
    static m3d::Class m_classNpcModelWnd;
};
