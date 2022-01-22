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
    IM_ERROR = 0xFFFFFFFF,
    IM_MODE_GAME_MENU = 0x0,
    IM_MODE_MAINMENU = 0x1,
    IM_MODE_CINEMATIC = 0x2,
    IM_MODE_GAME = 0x3,
    IM_FWD = 0x4,
    IM_BK = 0x5,
    IM_LEFT = 0x6,
    IM_RIGHT = 0x7,
    IM_GAME_ZOOM_CAMERA = 0x8,
    IM_GAME_DRAG_CAMERA = 0x9,
    IM_GAME_MS_MOVE = 0xA,
    IM_GAME_MS_MOVE_ON_UI = 0xB,
    IM_DEBUG_0 = 0xC,
    IM_DEBUG_1 = 0xD,
    IM_DEBUG_2 = 0xE,
    IM_DEBUG_3 = 0xF,
    IM_DEBUG_4 = 0x10,
    IM_DEBUG_5 = 0x11,
    IM_DEBUG_6 = 0x12,
    IM_DEBUG_7 = 0x13,
    IM_DEBUG_8 = 0x14,
    IM_DEBUG_9 = 0x15,
    IM_DEBUG_WIREFRAME = 0x16,
    IM_CAR_ACC = 0x17,
    IM_CAR_BREAK = 0x18,
    IM_CAR_HAND_BREAK = 0x19,
    IM_CAR_LEFT = 0x1A,
    IM_CAR_RIGHT = 0x1B,
    IM_CAR_HORN = 0x1C,
    IM_CAR_TURNTOWHEELS = 0x1D,
    IM_CAR_SWITCHCAMERA = 0x1E,
    IM_CAR_LIGHTS = 0x1F,
    IM_CAR_GET_OUT_OF_DIFFICULT_PLACE = 0x20,
    IM_CAR_FIRE_0 = 0x21,
    IM_CAR_FIRE_1 = 0x22,
    IM_CAR_FIRE_2 = 0x23,
    IM_CAR_FIRE_3 = 0x24,
    IM_CAR_FIRE_4 = 0x25,
    IM_CAR_FIRE_ALL = 0x26,
    IM_RELOAD_WEAPON = 0x27,
    IM_CINEMATIC_SKIP = 0x28,
    IM_CINEMATIC_SKIP_MSG = 0x29,
    IM_UI_INVENTORY = 0x2A,
    IM_UI_MENUBOOK = 0x2B,
    IM_UI_QUESTLOG = 0x2C,
    IM_UI_JOURNAL = 0x2D,
    IM_UI_MAP = 0x2E,
    IM_UI_VEHICLE_INFO = 0x2F,
    IM_UI_BAR = 0x30,
    IM_UI_ADDITIONAL_BUILDING = 0x31,
    IM_UI_HELP = 0x32,
    IM_PAUSE = 0x33,
    IM_UI_TOGGLE_INTERFACE = 0x34,
    IM_QUICK_SAVE = 0x35,
    IM_QUICK_LOAD = 0x36,
    IM_UI_PICKUP_ALL = 0x37,
    IM_NUM_IMPULSES = 0x38,
};

class BindKeysWnd :  public m3d::ui::Wnd
{
public:
    class KeySetButton : public m3d::ui::ButtonWnd
    {
    public:
        class AuxInfo
        {
        public:
            AuxInfo();

        private:
            CStr m_filler;
            unsigned int m_normColor;
            unsigned int m_twinkleColor;
            unsigned int m_twinkleTime;
            unsigned int m_selColor;
        };

    public:
        virtual int OnMouseButton0(unsigned int, PointBase<float> const&);
        virtual int OnMouseButton1(unsigned int, PointBase<float> const&);
        virtual int OnMouseButton2(unsigned int, PointBase<float> const&);
        virtual int OnKey(unsigned short, unsigned char, unsigned int);
        void OnAnotherInputOn();
        virtual void DrawWndText(m3d::ui::DrawInfo const&);
        void OnRebind(std::vector<int> const&);
        virtual ~KeySetButton();
        void SetBindText();
        void OnAnotherInputOff();
        int SetUp(Impulse, std::vector<int>&);
        virtual void OnNcPaint(m3d::ui::DrawInfo const&, unsigned int);
        virtual int OnMouseWheel(int, PointBase<float> const&);
        KeySetButton(BoundsBase<float>&);

    private:
        AuxInfo m_aif;
        Impulse m_impId;
        std::vector<int> m_keySet;
        std::vector<int> m_newKeySet;
        bool m_isInInputMode;
        bool m_isAnotherInInputMode;
        unsigned int m_lastTwinkleTime;
    };

    class BindKeysItem
    {
    public:
        class AuxInfo
        {
        public:
            AuxInfo();

        private:
            float m_impWPerc;
            float m_itemW;
            float m_itemH;
            unsigned int m_lblColor;
            CStr m_texIdBg;
        };

    public:
        int SetUp(Impulse, std::vector<std::vector<int>>, m3d::Object*, int);
        ~BindKeysItem();
        BindKeysItem(float);

    private:
        KeySetButton* m_btns[2];
        m3d::ui::Wnd* m_bg;
        m3d::ui::Wnd* m_impLabel;
        Impulse m_impId;
        AuxInfo m_aif;
    };

    class BindKeysList : public m3d::ui::ListBoxWnd<BindKeysItem*>
    {
    public:
        class AuxInfo
        {
        public:
            AuxInfo();

        private:
            CStr m_strBindReserved;
            CStr m_strBindExists;
            CStr m_strOnlyBindExists;
        };

    public:
        int CreateItems();
        virtual int MeasureItem(int, BoundsBase<float>&) const;
        int Rebind(Impulse);
        virtual int RenderItem(int, PointBase<float> const&, m3d::ui::DrawInfo const&);
        int CreateFromPattern(m3d::ui::StringsListBoxWnd*, bool);
        int BindUnbind(int, Impulse, std::vector<int> const&);
        virtual int DeleteItem(int);
        int ProcessBinding(KeySetButton*);
        BindKeysList();
        bool IsImpulseForEdit(Impulse);
        virtual int CompareItem(int, int);
        virtual int OnWndNotify(m3d::ui::Wnd*, unsigned int, unsigned int, m3d::AIParam const&);
        virtual int OnBeforeAddToWndStation();
        virtual ~BindKeysList();

    private:
        AuxInfo m_aif;
    };

    class AuxInfo
    {
    public:
        AuxInfo();

    private:
        CStr m_lstBindingsName;
    };

public:
    static m3d::Object * CreateObject();
    virtual m3d::Class * GetClass() const ;
    static m3d::Class * GetBaseClass();
    virtual m3d::Object * Clone();
    virtual ~BindKeysWnd();
    int ApplyChanges(bool);

protected:
    virtual int OnWndNotify(m3d::ui::Wnd *,unsigned int,unsigned int,m3d::AIParam const &);
    virtual int OnBeforeAddToWndStation();
    void LoadDefaultBindings();
    virtual int GameDataSetup();
    BindKeysWnd(BindKeysWnd const &);
    BindKeysWnd();
    virtual int GameDataUpdate(void *,int);
    void ApplyBindings();
    void CancelBindings();

public:
    RT_CLASS_DECLARE(BindKeysWnd);

private:
    AuxInfo m_aif;
    bool m_isInited;
    bool m_isDirty;
    BindKeysList *m_lstBindings;
    bool m_bChanged;
};
