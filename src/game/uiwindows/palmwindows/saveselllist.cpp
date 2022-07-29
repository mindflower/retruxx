#include "saveselllist.h"

RT_CLASS_EXPORTS_BEGIN(SaveSellButton)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(SaveSellButton);

SaveSellButton::~SaveSellButton()
{
    throw std::logic_error("Not implemented");
}

m3d::Object* SaveSellButton::Clone()
{
    throw std::logic_error("Not implemented");
}

int SaveSellButton::SetUp(ObjectInfo*, float, int)
{
    throw std::logic_error("Not implemented");
}

m3d::Class* SaveSellButton::GetBaseClass()
{
    throw std::logic_error("Not implemented");
}

void SaveSellButton::Select(bool)
{
    throw std::logic_error("Not implemented");
}

m3d::Object* SaveSellButton::CreateObject()
{
    throw std::logic_error("Not implemented");
}

bool SaveSellButton::IsSelected() const
{
    throw std::logic_error("Not implemented");
}

m3d::Class* SaveSellButton::GetClass() const
{
    throw std::logic_error("Not implemented");
}

ObjectInfo* SaveSellButton::GetObjectInfo() const
{
    throw std::logic_error("Not implemented");
}

SaveSellButton::SaveSellButton()
{
    throw std::logic_error("Not implemented");
}

SaveSellButton::SaveSellButton(SaveSellButton const&)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORTS_BEGIN(SaveSellList)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(SaveSellList);

SaveSellList::AuxInfo::AuxInfo()
{
    throw std::logic_error("Not implemented");
}

m3d::Object* SaveSellList::Clone()
{
    throw std::logic_error("Not implemented");
}

m3d::Class* SaveSellList::GetBaseClass()
{
    throw std::logic_error("Not implemented");
}

void SaveSellList::ShowAtBegin()
{
    throw std::logic_error("Not implemented");
}

m3d::Object* SaveSellList::CreateObject()
{
    throw std::logic_error("Not implemented");
}

int SaveSellList::CreateFromPattern(m3d::ui::Wnd const*, bool)
{
    throw std::logic_error("Not implemented");
}

void SaveSellList::GetSavedObjectInfos(std::vector<ObjectInfo*, std::allocator<ObjectInfo*>>&) const
{
    throw std::logic_error("Not implemented");
}

SaveSellList::~SaveSellList()
{
    throw std::logic_error("Not implemented");
}

void SaveSellList::SetCurSel(int)
{
    throw std::logic_error("Not implemented");
}

int SaveSellList::AddSaveButton(ObjectInfo*)
{
    throw std::logic_error("Not implemented");
}

int SaveSellList::GetSaveButtonIdByObjectInfo(ObjectInfo*) const
{
    throw std::logic_error("Not implemented");
}

ObjectInfo* SaveSellList::GetObjectInfoBySaveButtonId(int) const
{
    throw std::logic_error("Not implemented");
}

int SaveSellList::GetCurSel() const
{
    throw std::logic_error("Not implemented");
}

int SaveSellList::RemoveSaveButton(ObjectInfo*)
{
    throw std::logic_error("Not implemented");
}

m3d::Class* SaveSellList::GetClass() const
{
    throw std::logic_error("Not implemented");
}

void SaveSellList::ShowAtEnd()
{
    throw std::logic_error("Not implemented");
}

SaveSellList::SaveSellList(SaveSellList const&)
{
    throw std::logic_error("Not implemented");
}

SaveSellList::SaveSellList()
{
    throw std::logic_error("Not implemented");
}

void SaveSellList::ScrollNext()
{
    throw std::logic_error("Not implemented");
}

int SaveSellList::GameDataSave(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*)
{
    throw std::logic_error("Not implemented");
}

void SaveSellList::UpdateNextPrevButtonState()
{
    throw std::logic_error("Not implemented");
}

void SaveSellList::EnsureShowButton(int)
{
    throw std::logic_error("Not implemented");
}

void SaveSellList::ScrollPrev()
{
    throw std::logic_error("Not implemented");
}

bool SaveSellList::CanScrollPrev()
{
    throw std::logic_error("Not implemented");
}

void SaveSellList::RecalcLayot()
{
    throw std::logic_error("Not implemented");
}

int SaveSellList::GameDataClear(bool)
{
    throw std::logic_error("Not implemented");
}

bool SaveSellList::CanScrollNext()
{
    throw std::logic_error("Not implemented");
}

void SaveSellList::SelectButton(int)
{
    throw std::logic_error("Not implemented");
}

int SaveSellList::OnWndNotify(m3d::ui::Wnd*, unsigned, unsigned, m3d::AIParam const&)
{
    throw std::logic_error("Not implemented");
}

int SaveSellList::GameDataLoad(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*)
{
    throw std::logic_error("Not implemented");
}
