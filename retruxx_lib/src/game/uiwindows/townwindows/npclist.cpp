#include "npclist.h"
#include "npcmodelwnd.h"

RT_CLASS_EXPORTS_BEGIN(NpcButton)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(NpcButton);

m3d::Class* NpcButton::GetBaseClass()
{
    return RT_CLASS_LOCAL(Wnd);
}

ai::Npc* NpcButton::GetNpc() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* NpcButton::Clone()
{
    RETRUXX_NOT_IMPLEMENTED;
}

NpcButton::~NpcButton()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int NpcButton::SetUpForNpc(ai::Npc const*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* NpcButton::CreateObject()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* NpcButton::GetClass() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

CStr NpcButton::GetNameForNpc(ai::Npc const*) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void NpcButton::ClearPattern()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int NpcButton::OnWndNotify(m3d::ui::Wnd*, unsigned, unsigned, m3d::AIParam const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int NpcButton::LoadPattern(m3d::ui::Wnd*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

NpcButton::NpcButton()
{
    RETRUXX_NOT_IMPLEMENTED;
}

NpcButton::NpcButton(NpcButton const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int NpcButton::CreateChildren()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int NpcButton::CreateFromPattern()
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORTS_BEGIN(NpcList)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(NpcList);

NpcList::AuxInfo::AuxInfo()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* NpcList::GetClass() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* NpcList::CreateObject()
{
    RETRUXX_NOT_IMPLEMENTED;
}

ai::Bar* NpcList::GetBar() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int NpcList::CreateFromPattern(m3d::ui::Wnd*, bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int NpcList::GameDataClear(bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int NpcList::SetUpForBar(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* NpcList::Clone()
{
    RETRUXX_NOT_IMPLEMENTED;
}

NpcList::~NpcList()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* NpcList::GetBaseClass()
{
    return RT_CLASS_LOCAL(Wnd);
}

int NpcList::OnAfterAddToWndStation()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int NpcList::MeasureItem(int, BoundsBase<float>&) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void NpcList::FullUpdate()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int NpcList::AddButton(NpcButton*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int NpcList::CompareItem(int, int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int NpcList::OnBeforeAddToWndStation()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int NpcList::OnAfterRemoveFromWndStation()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int NpcList::CreateItems()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void NpcList::AddNpcsToEncyclopaedia()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int NpcList::RenderItem(int, PointBase<float> const&, m3d::ui::DrawInfo const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

NpcList::NpcList()
{
    RETRUXX_NOT_IMPLEMENTED;
}

NpcList::NpcList(NpcList const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int NpcList::DeleteItem(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}
