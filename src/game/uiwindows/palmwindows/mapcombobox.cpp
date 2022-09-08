#include "mapcombobox.h"

RT_CLASS_EXPORTS_BEGIN(MapComboBox)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(MapComboBox);

int MapComboBox::CreateFromPattern(m3d::ui::Wnd const*, bool)
{
    throw std::logic_error("Not implemented");
}

m3d::Class* MapComboBox::GetBaseClass()
{
    return RT_CLASS_LOCAL(ComboBoxWnd);
}

CStr MapComboBox::GetSelectedMapName() const
{
    throw std::logic_error("Not implemented");
}

m3d::Object* MapComboBox::Clone()
{
    throw std::logic_error("Not implemented");
}

m3d::Object* MapComboBox::CreateObject()
{
    throw std::logic_error("Not implemented");
}

m3d::Class* MapComboBox::GetClass() const
{
    throw std::logic_error("Not implemented");
}

MapComboBox::~MapComboBox()
{
    throw std::logic_error("Not implemented");
}

int MapComboBox::GameDataClear(bool)
{
    throw std::logic_error("Not implemented");
}

int MapComboBox::GameDataUpdate(void*, int)
{
    throw std::logic_error("Not implemented");
}

MapComboBox::MapComboBox(MapComboBox const&)
{
    throw std::logic_error("Not implemented");
}

MapComboBox::MapComboBox()
{
    throw std::logic_error("Not implemented");
}

int MapComboBox::OnStartLevel()
{
    throw std::logic_error("Not implemented");
}
