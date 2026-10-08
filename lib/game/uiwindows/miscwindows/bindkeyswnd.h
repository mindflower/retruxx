#pragma once
#include <ui/button.h>
#include <ui/uilistboxwnd.h>

namespace m3d
{
    namespace ui
    {
        class StringsListBoxWnd;
    }
}

enum Impulse
{
    IM_ERROR = -1,
    IM_MODE_GAME_MENU = 0,
    IM_MODE_MAINMENU = 1,
    IM_MODE_CINEMATIC = 2,
    IM_MODE_GAME = 3,
    IM_FWD = 4,
    IM_BK = 5,
    IM_LEFT = 6,
    IM_RIGHT = 7,
    IM_GAME_ZOOM_CAMERA = 8,
    IM_GAME_DRAG_CAMERA = 9,
    IM_GAME_MS_MOVE = 10,
    IM_GAME_MS_MOVE_ON_UI = 11,
    IM_DEBUG_0 = 12,
    IM_DEBUG_1 = 13,
    IM_DEBUG_2 = 14,
    IM_DEBUG_3 = 15,
    IM_DEBUG_4 = 16,
    IM_DEBUG_5 = 17,
    IM_DEBUG_6 = 18,
    IM_DEBUG_7 = 19,
    IM_DEBUG_8 = 20,
    IM_DEBUG_9 = 21,
    IM_DEBUG_WIREFRAME = 22,
    IM_CAR_ACC = 23,
    IM_CAR_BREAK = 24,
    IM_CAR_HAND_BREAK = 25,
    IM_CAR_LEFT = 26,
    IM_CAR_RIGHT = 27,
    IM_CAR_HORN = 28,
    IM_CAR_TURNTOWHEELS = 29,
    IM_CAR_SWITCHCAMERA = 30,
    IM_CAR_LIGHTS = 31,
    IM_CAR_GET_OUT_OF_DIFFICULT_PLACE = 32,
    IM_CAR_FIRE_0 = 33,
    IM_CAR_FIRE_1 = 34,
    IM_CAR_FIRE_2 = 35,
    IM_CAR_FIRE_3 = 36,
    IM_CAR_FIRE_4 = 37,
    IM_CAR_FIRE_ALL = 38,
    IM_RELOAD_WEAPON = 39,
    IM_CINEMATIC_SKIP = 40,
    IM_CINEMATIC_SKIP_MSG = 41,
    IM_UI_INVENTORY = 42,
    IM_UI_MENUBOOK = 43,
    IM_UI_QUESTLOG = 44,
    IM_UI_JOURNAL = 45,
    IM_UI_MAP = 46,
    IM_UI_VEHICLE_INFO = 47,
    IM_UI_BAR = 48,
    IM_UI_ADDITIONAL_BUILDING = 49,
    IM_UI_HELP = 50,
    IM_PAUSE = 51,
    IM_UI_TOGGLE_INTERFACE = 52,
    IM_QUICK_SAVE = 53,
    IM_QUICK_LOAD = 54,
    IM_UI_PICKUP_ALL = 55,
    IM_NUM_IMPULSES = 56,
};

class BindKeysWnd : public m3d::ui::Wnd
{
public:
    int ApplyChanges(bool bForce);

    struct AuxInfo
    {
        /* 0x0000 */ CStr m_lstBindingsName;
        AuxInfo(BindKeysWnd::AuxInfo const&);
        AuxInfo();
    }; /* size: 0x000c */

    struct KeySetButton : public m3d::ui::ButtonWnd
    {
        KeySetButton(BindKeysWnd::KeySetButton const&);
        KeySetButton(BoundsBase<float>& rc);
        virtual ~KeySetButton() override /* 0x00 */;

        struct AuxInfo
        {
            /* 0x0000 */ CStr m_filler;
            /* 0x000c */ unsigned int m_normColor;
            /* 0x0010 */ unsigned int m_twinkleColor;
            /* 0x0014 */ unsigned int m_twinkleTime;
            /* 0x0018 */ unsigned int m_selColor;
            AuxInfo(BindKeysWnd::KeySetButton::AuxInfo const&);
            AuxInfo();
        }; /* size: 0x001c */

        virtual int OnKey(unsigned short key, unsigned char scanCode, unsigned int state) override /* 0x00 */;
        virtual int OnMouseButton0(unsigned int state, PointBase<float> const& at) override /* 0x00 */;
        virtual int OnMouseButton1(unsigned int state, PointBase<float> const& at) override /* 0x00 */;
        virtual int OnMouseButton2(unsigned int state, PointBase<float> const& at) override /* 0x00 */;
        virtual int OnMouseWheel(int ticks, PointBase<float> const& at) override /* 0x00 */;
        virtual void OnNcPaint(m3d::ui::DrawInfo const& di, unsigned int clr) override /* 0x00 */;
        virtual void DrawWndText(m3d::ui::DrawInfo const& di) override /* 0x00 */;
        int SetUp(Impulse impId, retruxx::vector<int>& keySet);
        void OnRebind(retruxx::vector<int> const& newKeys);
        void OnAnotherInputOn();
        void OnAnotherInputOff();
        void SetBindText();

        /* 0x023c */ BindKeysWnd::KeySetButton::AuxInfo m_aif;
        /* 0x0258 */ Impulse m_impId;
        /* 0x025c */ retruxx::vector<int> m_keySet;
        /* 0x026c */ retruxx::vector<int> m_newKeySet;
        /* 0x027c */ bool m_isInInputMode;
        /* 0x027d */ bool m_isAnotherInInputMode;
        /* 0x0280 */ unsigned int m_lastTwinkleTime;
    }; /* size: 0x0284 */

    struct BindKeysItem
    {
        struct AuxInfo
        {
            AuxInfo(BindKeysWnd::BindKeysItem::AuxInfo const&);
            AuxInfo();
            /* 0x0000 */ float m_impWPerc;
            /* 0x0004 */ float m_itemW;
            /* 0x0008 */ float m_itemH;
            /* 0x000c */ unsigned int m_lblColor;
            /* 0x0010 */ CStr m_texIdBg;
        }; /* size: 0x001c */

        BindKeysItem(BindKeysWnd::BindKeysItem const&);
        BindKeysItem(float width);
        ~BindKeysItem();
        int SetUp(Impulse impId, retruxx::vector<retruxx::vector<int>> keys, m3d::Object* parent, int idx);

        /* 0x0000 */ BindKeysWnd::KeySetButton* m_btns[2];
        /* 0x0008 */ m3d::ui::Wnd* m_bg;
        /* 0x000c */ m3d::ui::Wnd* m_impLabel;
        /* 0x0010 */ Impulse m_impId;
        /* 0x0014 */ BindKeysWnd::BindKeysItem::AuxInfo m_aif;
    }; /* size: 0x0030 */

    struct BindKeysList : public m3d::ui::ListBoxWnd<BindKeysWnd::BindKeysItem*>
    {
        struct AuxInfo
        {
            /* 0x0000 */ CStr m_strBindReserved;
            /* 0x000c */ CStr m_strBindExists;
            /* 0x0018 */ CStr m_strOnlyBindExists;
            AuxInfo(BindKeysWnd::BindKeysList::AuxInfo const&);
            AuxInfo();
        }; /* size: 0x0024 */

        BindKeysList(BindKeysWnd::BindKeysList const&);
        BindKeysList();
        virtual ~BindKeysList() override /* 0x00 */;
        int CreateFromPattern(m3d::ui::StringsListBoxWnd* patternList, bool deleteSrc);
        int Rebind(Impulse impId);
        int CreateItems();
        bool IsImpulseForEdit(Impulse impId);
        int ProcessBinding(BindKeysWnd::KeySetButton* btn);
        int BindUnbind(int action, Impulse imp, retruxx::vector<int> const& keys);
        virtual int MeasureItem(int itemIdx, BoundsBase<float>& bounds) const override /* 0x134 */;
        virtual int RenderItem(int itemIdx, PointBase<float> const& org, m3d::ui::DrawInfo const& di) override /* 0x138 */;
        virtual int DeleteItem(int itemIdx) override /* 0x13c */;
        virtual int CompareItem(int itemIdx0, int itemIdx1) override /* 0x140 */;
        virtual int OnWndNotify(m3d::ui::Wnd* from, unsigned int id, unsigned int msg, m3d::AIParam const& data) override /* 0x00 */;
        virtual int OnBeforeAddToWndStation() override /* 0x00 */;

        /* 0x0238 */ BindKeysWnd::BindKeysList::AuxInfo m_aif;
    }; /* size: 0x025c */

protected:
    virtual int OnWndNotify(m3d::ui::Wnd* from, unsigned int id, unsigned int msg, m3d::AIParam const& data) override /* 0xc8 */;
    virtual int GameDataSetup() override /* 0x104 */;
    virtual int GameDataUpdate(void* data, int dataType) override /* 0x10c */;
    virtual int OnBeforeAddToWndStation() override /* 0x68 */;
    void CancelBindings();
    void ApplyBindings();
    void LoadDefaultBindings();

    /* 0x0220 */ BindKeysWnd::AuxInfo m_aif;
    /* 0x022c */ bool m_isInited;
    /* 0x022d */ bool m_isDirty;
    /* 0x0230 */ BindKeysWnd::BindKeysList* m_lstBindings;
    /* 0x0234 */ bool m_bChanged;

    BindKeysWnd();
    BindKeysWnd(BindKeysWnd const& rhs);

public:
    virtual ~BindKeysWnd() override /* 0x00 */;
    virtual m3d::Object* Clone() override /* 0x04 */;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override /* 0x34 */;
    RT_CLASS_DECLARE(BindKeysWnd);
}; /* size: 0x0238 */
