#pragma once

class MapComboBox :  public m3d::ui::ComboBoxWnd
{
public:
    int CreateFromPattern(class m3d::ui::Wnd const *,bool);
    static struct m3d::Class * GetBaseClass();
    CStr GetSelectedMapName() const ;
    virtual class m3d::Object * Clone();
    static class m3d::Object * CreateObject();
    virtual struct m3d::Class * GetClass() const ;
    virtual ~MapComboBox();
protected:
    virtual int GameDataClear(bool);
    virtual int GameDataUpdate(void *,int);
    MapComboBox(class MapComboBox const &);
    MapComboBox();
    int OnStartLevel();
private:
};
