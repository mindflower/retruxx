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

        public:
            CStr m_strBindReserved = "BINDKEYS_MSG_RESERVED";
            CStr m_strBindExists = "BINDKEYS_MSG_EXISTS";
            CStr m_strOnlyBindExists = "BINDKEYS_MSG_EXISTS_ONLY";
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

    public:
        CStr m_lstBindingsName = "lstBindings";
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
    bool m_isInited = false;
    bool m_isDirty = false;
    BindKeysList *m_lstBindings = false;
    bool m_bChanged = false;
};
