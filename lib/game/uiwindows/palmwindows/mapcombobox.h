#pragma once
#include <ui/comboboxwnd.h>

class MapComboBox : public m3d::ui::ComboBoxWnd
{
public:
    int CreateFromPattern(m3d::ui::Wnd const* patternWnd, bool deleteSrc);
    static int const MAP_FILTER_ALL;
    CStr GetSelectedMapName() const;

protected:
    virtual int GameDataUpdate(void* data, int dataType) override /* 0x00 */;
    virtual int GameDataClear(bool beforeContinuousLevel) override /* 0x00 */;
    int OnStartLevel();
    MapComboBox();
    MapComboBox(MapComboBox const& rhs);

public:
    virtual ~MapComboBox() override /* 0x00 */;
    virtual m3d::Object* Clone() override /* 0x00 */;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override /* 0x00 */;
    static m3d::Class m_classMapComboBox;
}; /* size: 0x0258 */
