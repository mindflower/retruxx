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
    virtual int GameDataUpdate(void* data, int dataType) override /* 0x00 */;
    virtual int GameDataClear(bool beforeContinuousLevel) override /* 0x00 */;
    virtual int OnNewFrame() override /* 0x130 */;
    void OnNpcReplyShown(void* data);
    virtual bool IsValid() const override /* 0x13c */;
    void RandomizeAnimationFrame();
    void UpdateAnimation();
    /* 0x02bc */ int m_npcId;
    NpcModelWnd();
    NpcModelWnd(NpcModelWnd const& rhs);

public:
    virtual ~NpcModelWnd() override /* 0x00 */;
    virtual m3d::Object* Clone() override /* 0x04 */;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override /* 0x34 */;
    static m3d::Class m_classNpcModelWnd;
}; /* size: 0x02c0 */
