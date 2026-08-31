#include "mapcombobox.h"

RT_CLASS_EXPORTS_BEGIN(MapComboBox)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(MapComboBox);

int MapComboBox::CreateFromPattern(m3d::ui::Wnd const*, bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* MapComboBox::GetBaseClass()
{
    return RT_CLASS_LOCAL(ComboBoxWnd);
}

CStr MapComboBox::GetSelectedMapName() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* MapComboBox::Clone()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* MapComboBox::CreateObject()
{
    return new MapComboBox;
}

m3d::Class* MapComboBox::GetClass() const
{
    return RT_CLASS_LOCAL(MapComboBox);
}

MapComboBox::~MapComboBox()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int MapComboBox::GameDataClear(bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int MapComboBox::GameDataUpdate(void*, int)
{
    // TODO: implement GameDataUpdate
    RETRUXX_NOT_IMPLEMENTED;
    return 0;
}

MapComboBox::MapComboBox(MapComboBox const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

MapComboBox::MapComboBox()
{
}

int MapComboBox::OnStartLevel()
{
    RETRUXX_NOT_IMPLEMENTED;
}
