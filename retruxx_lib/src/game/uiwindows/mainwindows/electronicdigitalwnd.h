#pragma once
#include <ui/ui.h>

namespace m3d
{
    namespace ui
    {
        class ImageWnd;
    }
}

class ElectronicDigitalWnd :  public m3d::ui::Wnd
{
public:
    enum DigitalSize
    {
        DIGITAL_SIZE_SMALL = 0x0,
        DIGITAL_SIZE_LARGE = 0x1,
        DIGITAL_SIZE_NUM_DIGITAL_SIZES = 0x2,
        DIGITAL_SIZE_INVALID = 0x2,
    };

    enum Symbol
    {
        SYMBOL_0 = 0x0,
        SYMBOL_PADDING = 0x0,
        SYMBOL_1 = 0x1,
        SYMBOL_2 = 0x2,
        SYMBOL_3 = 0x3,
        SYMBOL_4 = 0x4,
        SYMBOL_5 = 0x5,
        SYMBOL_6 = 0x6,
        SYMBOL_7 = 0x7,
        SYMBOL_8 = 0x8,
        SYMBOL_9 = 0x9,
        SYMBOL_COLON = 0xA,
        SYMBOL_METER = 0xB,
        SYMBOL_KILOMETER = 0xC,
        SYMBOL_NUM_SYMBOLS = 0xD,
        SYMBOL_INVALID = 0xD,
    };

    class Digital
    {
    public:
        void SetSymbol(ElectronicDigitalWnd::Symbol, bool);
        Digital(m3d::ui::ImageWnd*, ElectronicDigitalWnd::DigitalSize, ElectronicDigitalWnd::Symbol);
        Digital(void);
        bool IsFree(void) const;
        m3d::ui::ImageWnd* GetWnd(void) const;
        ElectronicDigitalWnd::Symbol GetSymbol(void) const;
        void SetDigitalSize(ElectronicDigitalWnd::DigitalSize);

    private:
        void UpdateTexture(void);
        void Show(bool);
        m3d::ui::ImageWnd* m_wnd;
        ElectronicDigitalWnd::Symbol m_symbol;
        bool m_bReserved;
        ElectronicDigitalWnd::DigitalSize m_digitalSize;
    };

public:
    virtual m3d::Object * Clone();
    static m3d::Class * GetBaseClass();
    virtual ~ElectronicDigitalWnd();
    void SetDigitalColor(unsigned int);
    int CreateFromPattern(m3d::ui::Wnd *,bool);
    static m3d::Object * CreateObject();
    int ShowNumber(int,bool,unsigned int,bool);
    int ShowSymbol(Symbol,bool,unsigned int,bool);
    void SetDigitalSize(DigitalSize);
    virtual m3d::Class * GetClass() const ;
    void Clear();

protected:
    int GetFirstFreeSlot() const ;
    ElectronicDigitalWnd(ElectronicDigitalWnd const &);
    ElectronicDigitalWnd();
    virtual int GameDataClear(bool);

public:
    RT_CLASS_DECLARE(ElectronicDigitalWnd);

private:
    ElectronicDigitalWnd::DigitalSize m_digitalSize;
    std::vector<ElectronicDigitalWnd::Digital> m_digitals;
    unsigned int m_digitalColor;
};
