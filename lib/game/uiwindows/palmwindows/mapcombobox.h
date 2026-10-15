#pragma once
#include <ui/comboboxwnd.h>

class MapComboBox : public m3d::ui::ComboBoxWnd
{
public:
    int CreateFromPattern(m3d::ui::Wnd const* patternWnd, bool deleteSrc);
    static int const MAP_FILTER_ALL;
    CStr GetSelectedMapName() const;

protected:
    virtual int GameDataUpdate(void* data, int dataType) override;
    virtual int GameDataClear(bool beforeContinuousLevel) override;
    int OnStartLevel();
    MapComboBox();

public:
    virtual ~MapComboBox() override;
    virtual m3d::Object* Clone() override;
    static m3d::Object* CreateObject();
    static m3d::Class* GetBaseClass();
    virtual m3d::Class* GetClass() const override;
    static m3d::Class m_classMapComboBox;
};
