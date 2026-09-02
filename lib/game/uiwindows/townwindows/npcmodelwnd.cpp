#include "npcmodelwnd.h"

#include <cstdlib>

#include <game/m3dgame.h>
#include <game/uimisc/guihelper.h>
#include <skelmodel.h>

#include <server/objects/npc.h>
#include <server/objects/base/objcontainer.h>

RT_CLASS_EXPORTS_BEGIN(NpcModelWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(NpcModelWnd);

m3d::Class* NpcModelWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(ItemModelWnd);
}

m3d::Class* NpcModelWnd::GetClass() const
{
    return RT_CLASS_LOCAL(NpcModelWnd);
}

NpcModelWnd::NpcModelWnd()
{
    m_npcId = -1;
}

NpcModelWnd::NpcModelWnd(NpcModelWnd const&) : NpcModelWnd()
{
    // Matches the original: the copy ctor default-constructs the base and does
    // not copy any NpcModelWnd state; only reached via Clone().
}

NpcModelWnd::~NpcModelWnd()
{
}

m3d::Object* NpcModelWnd::CreateObject()
{
    return new NpcModelWnd();
}

m3d::Object* NpcModelWnd::Clone()
{
    return new NpcModelWnd(*this);
}

int NpcModelWnd::GetNpcId() const
{
    return m_npcId;
}

ai::Npc* NpcModelWnd::GetNpc() const
{
    if (m_npcId == -1)
    {
        return nullptr;
    }
    ai::Obj* o = ai::theObjects->GetEntityByObjId(m_npcId);
    return (o && o->IsKindOf(&ai::Npc::m_classNpc)) ? static_cast<ai::Npc*>(o) : nullptr;
}

bool NpcModelWnd::IsValid() const
{
    return m_Model && m_Animation && GetNpc();
}

int NpcModelWnd::GameDataClear(bool)
{
    delete m_Animation;
    m_Animation = nullptr;
    m_Model = nullptr;
    m_cfg.m_num = 0;
    m_SkinNum = 0;
    m_npcId = -1;
    return 1;
}

int NpcModelWnd::GameDataUpdate(void* data, int dataType)
{
    if ((m_gameDataFlags & 1) == 0)
    {
        return 0;
    }
    if (dataType == 23)
    {
        OnNpcReplyShown(data);
    }
    else if (dataType == 89)
    {
        OnNewFrame();
        return 1;
    }
    return 1;
}

int NpcModelWnd::SetUpForNpc(int npcId)
{
    GameDataClear(false);
    m_npcId = npcId;

    ai::Npc* npc = GetNpc();
    if (!npc || !SetModelByName(npc->GetModelName(), npc->GetSkinNumber(), npc->GetCfgNumber()))
    {
        return 0;
    }

    if (!m_Animation->SetAnimation(help::GetRandomStandAnimation(m_Model)))
    {
        m_Animation->SetAnimation(AT_STAND1);
    }
    RandomizeAnimationFrame();
    return 1;
}

void NpcModelWnd::OnNpcReplyShown(void* data)
{
    if (!IsChildOf(static_cast<m3d::Object*>(M3D_APP)) || (m_gameDataFlags & 1) == 0 || !IsValid() || !data)
    {
        return;
    }

    int replyNpcId = reinterpret_cast<const int*>(data)[13];
    if (replyNpcId != m_npcId)
    {
        return;
    }

    if (!m_Animation->SetAnimation(help::GetRandomMoveAnimation(m_Model)))
    {
        m_Animation->SetAnimation(AT_MOVE1);
    }
}

int NpcModelWnd::OnNewFrame()
{
    int res = ItemModelWnd::OnNewFrame();
    if (m_Model && m_Animation)
    {
        UpdateAnimation();
    }
    return res;
}

void NpcModelWnd::RandomizeAnimationFrame()
{
    if (m_Model && m_Animation)
    {
        m_Animation->MoveFrame(static_cast<unsigned int>(rand() % 1000));
        m_Model->Update(m_Animation, false, &m_cfg);
    }
}

void NpcModelWnd::UpdateAnimation()
{
    if (!m_Model || !m_Animation)
    {
        return;
    }

    const m3d::AnimatedModel::Animation* curAnim = m_Animation->GetCurAnimation();
    if (!curAnim)
    {
        return;
    }

    short numFrames = curAnim->m_numFrames;
    if (numFrames && m_Animation->CurAnimFrame() < numFrames - 2)
    {
        return;
    }

    ActionType action = curAnim->m_action;
    if (action != AT_NUMTYPES)
    {
        m_Model->SetNextForAnimation(action, help::GetRandomStandAnimation(m_Model));
    }
}
