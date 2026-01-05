#pragma once
#include <ui/comboboxwnd.h>

class MapComboBox :  public m3d::ui::ComboBoxWnd
{
public:
    int CreateFromPattern(m3d::ui::Wnd const *,bool);
    static m3d::Class * GetBaseClass();
    CStr GetSelectedMapName() const ;
    virtual m3d::Object * Clone();
    static m3d::Object * CreateObject();
    virtual m3d::Class * GetClass() const ;
    virtual ~MapComboBox();

protected:
    virtual int GameDataClear(bool);
    virtual int GameDataUpdate(void *,int);
    MapComboBox(MapComboBox const &);
    MapComboBox();
    int OnStartLevel();

public:
    RT_CLASS_DECLARE(MapComboBox);
};
