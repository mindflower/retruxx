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
    throw std::logic_error("Not implemented");
}

m3d::Object* NpcButton::Clone()
{
    throw std::logic_error("Not implemented");
}

NpcButton::~NpcButton()
{
    throw std::logic_error("Not implemented");
}

int NpcButton::SetUpForNpc(ai::Npc const*)
{
    throw std::logic_error("Not implemented");
}

m3d::Object* NpcButton::CreateObject()
{
    throw std::logic_error("Not implemented");
}

m3d::Class* NpcButton::GetClass() const
{
    throw std::logic_error("Not implemented");
}

CStr NpcButton::GetNameForNpc(ai::Npc const*) const
{
    throw std::logic_error("Not implemented");
}

void NpcButton::ClearPattern()
{
    throw std::logic_error("Not implemented");
}

int NpcButton::OnWndNotify(m3d::ui::Wnd*, unsigned, unsigned, m3d::AIParam const&)
{
    throw std::logic_error("Not implemented");
}

int NpcButton::LoadPattern(m3d::ui::Wnd*)
{
    throw std::logic_error("Not implemented");
}

NpcButton::NpcButton()
{
    throw std::logic_error("Not implemented");
}

NpcButton::NpcButton(NpcButton const&)
{
    throw std::logic_error("Not implemented");
}

int NpcButton::CreateChildren()
{
    throw std::logic_error("Not implemented");
}

int NpcButton::CreateFromPattern()
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORTS_BEGIN(NpcList)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(NpcList);

NpcList::AuxInfo::AuxInfo()
{
    throw std::logic_error("Not implemented");
}

m3d::Class* NpcList::GetClass() const
{
    throw std::logic_error("Not implemented");
}

m3d::Object* NpcList::CreateObject()
{
    throw std::logic_error("Not implemented");
}

ai::Bar* NpcList::GetBar() const
{
    throw std::logic_error("Not implemented");
}

int NpcList::CreateFromPattern(m3d::ui::Wnd*, bool)
{
    throw std::logic_error("Not implemented");
}

int NpcList::GameDataClear(bool)
{
    throw std::logic_error("Not implemented");
}

int NpcList::SetUpForBar(int)
{
    throw std::logic_error("Not implemented");
}

m3d::Object* NpcList::Clone()
{
    throw std::logic_error("Not implemented");
}

NpcList::~NpcList()
{
    throw std::logic_error("Not implemented");
}

m3d::Class* NpcList::GetBaseClass()
{
    return RT_CLASS_LOCAL(Wnd);
}

int NpcList::OnAfterAddToWndStation()
{
    throw std::logic_error("Not implemented");
}

int NpcList::MeasureItem(int, BoundsBase<float>&) const
{
    throw std::logic_error("Not implemented");
}

void NpcList::FullUpdate()
{
    throw std::logic_error("Not implemented");
}

int NpcList::AddButton(NpcButton*)
{
    throw std::logic_error("Not implemented");
}

int NpcList::CompareItem(int, int)
{
    throw std::logic_error("Not implemented");
}

int NpcList::OnBeforeAddToWndStation()
{
    throw std::logic_error("Not implemented");
}

int NpcList::OnAfterRemoveFromWndStation()
{
    throw std::logic_error("Not implemented");
}

int NpcList::CreateItems()
{
    throw std::logic_error("Not implemented");
}

void NpcList::AddNpcsToEncyclopaedia()
{
    throw std::logic_error("Not implemented");
}

int NpcList::RenderItem(int, PointBase<float> const&, m3d::ui::DrawInfo const&)
{
    throw std::logic_error("Not implemented");
}

NpcList::NpcList()
{
    throw std::logic_error("Not implemented");
}

NpcList::NpcList(NpcList const&)
{
    throw std::logic_error("Not implemented");
}

int NpcList::DeleteItem(int)
{
    throw std::logic_error("Not implemented");
}
