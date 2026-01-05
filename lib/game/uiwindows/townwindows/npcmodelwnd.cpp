#include "npcmodelwnd.h"

RT_CLASS_EXPORTS_BEGIN(NpcModelWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(NpcModelWnd);

m3d::Class* NpcModelWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(ItemModelWnd);
}

m3d::Object* NpcModelWnd::Clone()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int NpcModelWnd::GetNpcId() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

ai::Npc* NpcModelWnd::GetNpc() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* NpcModelWnd::CreateObject()
{
    RETRUXX_NOT_IMPLEMENTED;
}

NpcModelWnd::~NpcModelWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int NpcModelWnd::SetUpForNpc(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* NpcModelWnd::GetClass() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int NpcModelWnd::GameDataClear(bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void NpcModelWnd::RandomizeAnimationFrame()
{
    RETRUXX_NOT_IMPLEMENTED;
}

bool NpcModelWnd::IsValid() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int NpcModelWnd::GameDataUpdate(void*, int)
{
    // TODO: implement GameDataUpdate
    //  RETRUXX_NOT_IMPLEMENTED;
    return 0;
}

void NpcModelWnd::OnNpcReplyShown(void*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int NpcModelWnd::OnNewFrame()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void NpcModelWnd::UpdateAnimation()
{
    RETRUXX_NOT_IMPLEMENTED;
}

NpcModelWnd::NpcModelWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

NpcModelWnd::NpcModelWnd(NpcModelWnd const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}
