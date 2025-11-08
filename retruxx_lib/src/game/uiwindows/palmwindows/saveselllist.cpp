#include "saveselllist.h"

RT_CLASS_EXPORTS_BEGIN(SaveSellButton)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(SaveSellButton);

SaveSellButton::~SaveSellButton()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* SaveSellButton::Clone()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int SaveSellButton::SetUp(ObjectInfo*, float, int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* SaveSellButton::GetBaseClass()
{
    return RT_CLASS_LOCAL(ButtonWnd);
}

void SaveSellButton::Select(bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* SaveSellButton::CreateObject()
{
    RETRUXX_NOT_IMPLEMENTED;
}

bool SaveSellButton::IsSelected() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* SaveSellButton::GetClass() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

ObjectInfo* SaveSellButton::GetObjectInfo() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

SaveSellButton::SaveSellButton()
{
    RETRUXX_NOT_IMPLEMENTED;
}

SaveSellButton::SaveSellButton(SaveSellButton const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORTS_BEGIN(SaveSellList)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(SaveSellList);

SaveSellList::AuxInfo::AuxInfo()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* SaveSellList::Clone()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* SaveSellList::GetBaseClass()
{
    return RT_CLASS_LOCAL(Wnd);
}

void SaveSellList::ShowAtBegin()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* SaveSellList::CreateObject()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int SaveSellList::CreateFromPattern(m3d::ui::Wnd const*, bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void SaveSellList::GetSavedObjectInfos(std::vector<ObjectInfo*, std::allocator<ObjectInfo*>>&) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

SaveSellList::~SaveSellList()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void SaveSellList::SetCurSel(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int SaveSellList::AddSaveButton(ObjectInfo*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int SaveSellList::GetSaveButtonIdByObjectInfo(ObjectInfo*) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

ObjectInfo* SaveSellList::GetObjectInfoBySaveButtonId(int) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int SaveSellList::GetCurSel() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int SaveSellList::RemoveSaveButton(ObjectInfo*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* SaveSellList::GetClass() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void SaveSellList::ShowAtEnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

SaveSellList::SaveSellList(SaveSellList const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

SaveSellList::SaveSellList()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void SaveSellList::ScrollNext()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int SaveSellList::GameDataSave(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void SaveSellList::UpdateNextPrevButtonState()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void SaveSellList::EnsureShowButton(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void SaveSellList::ScrollPrev()
{
    RETRUXX_NOT_IMPLEMENTED;
}

bool SaveSellList::CanScrollPrev()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void SaveSellList::RecalcLayot()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int SaveSellList::GameDataClear(bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

bool SaveSellList::CanScrollNext()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void SaveSellList::SelectButton(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int SaveSellList::OnWndNotify(m3d::ui::Wnd*, unsigned, unsigned, m3d::AIParam const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int SaveSellList::GameDataLoad(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*)
{
    RETRUXX_NOT_IMPLEMENTED;
}
