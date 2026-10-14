#pragma once
#include <ui/ui.h>

namespace m3d
{
    namespace ui
    {
        class ImageWnd;
    }
}  // namespace m3d

class ElectronicDigitalWnd : public m3d::ui::Wnd
{
public:
    enum Symbol
    {
        SYMBOL_0 = 0,
        SYMBOL_PADDING = 0,
        SYMBOL_1 = 1,
        SYMBOL_2 = 2,
        SYMBOL_3 = 3,
        SYMBOL_4 = 4,
        SYMBOL_5 = 5,
        SYMBOL_6 = 6,
        SYMBOL_7 = 7,
        SYMBOL_8 = 8,
        SYMBOL_9 = 9,
        SYMBOL_COLON = 10,
        SYMBOL_METER = 11,
        SYMBOL_KILOMETER = 12,
        SYMBOL_NUM_SYMBOLS = 13,
        SYMBOL_INVALID = 13,
    };

    enum DigitalSize
    {
        DIGITAL_SIZE_SMALL = 0,
        DIGITAL_SIZE_LARGE = 1,
        DIGITAL_SIZE_NUM_DIGITAL_SIZES = 2,
        DIGITAL_SIZE_INVALID = 2,
    };

public:
    int CreateFromPattern(m3d::ui::Wnd* patternWnd, bool deleteSrc);
    int ShowNumber(int number, bool bAdd, unsigned int numReservedSlots, bool bPaddFreeSlots);
    int ShowSymbol(ElectronicDigitalWnd::Symbol symbol, bool bAdd, unsigned int numReservedSlots, bool bPaddFreeSlots);
    void Clear();
    void SetDigitalSize(ElectronicDigitalWnd::DigitalSize digitalSize);
    void SetDigitalColor(unsigned int digitalColor);

    struct DigitalTextures
    {
        DigitalTextures();
        ~DigitalTextures();
        int Init();
        void Clear();
        m3d::rend::TexHandle GetTexture(
            ElectronicDigitalWnd::Symbol symbol,
            ElectronicDigitalWnd::DigitalSize digitalSize) const;
        CStr GetTextureStrId(ElectronicDigitalWnd::Symbol symbol, ElectronicDigitalWnd::DigitalSize digitalSize) const;
        m3d::rend::TexHandle m_textures[2][13];
        int m_refCount;
    };

    struct Digital
    {
        Digital(
            m3d::ui::ImageWnd* wnd,
            ElectronicDigitalWnd::DigitalSize digitalSize,
            ElectronicDigitalWnd::Symbol symbol);
        Digital();
        void SetDigitalSize(ElectronicDigitalWnd::DigitalSize digitalSize);
        void SetSymbol(ElectronicDigitalWnd::Symbol symbol, bool bReserved);
        ElectronicDigitalWnd::Symbol GetSymbol() const;
        m3d::ui::ImageWnd* GetWnd() const;
        bool IsFree() const;
        void Show(bool bShow);
        void UpdateTexture();
        m3d::ui::ImageWnd* m_wnd;
        ElectronicDigitalWnd::Symbol m_symbol;
        bool m_bReserved;
        ElectronicDigitalWnd::DigitalSize m_digitalSize;
    };

    using DigitalVector = std::vector<ElectronicDigitalWnd::Digital, std::allocator<ElectronicDigitalWnd::Digital>>;

protected:
    virtual int GameDataClear(bool beforeContinuousLevel) override;
    int GetFirstFreeSlot() const;
    ElectronicDigitalWnd::DigitalSize m_digitalSize;
    std::vector<ElectronicDigitalWnd::Digital, std::allocator<ElectronicDigitalWnd::Digital>> m_digitals;
    unsigned int m_digitalColor;
    static inline ElectronicDigitalWnd::DigitalTextures m_digitalTextures;
    ElectronicDigitalWnd();

public:
    virtual ~ElectronicDigitalWnd() override;
    virtual m3d::Object* Clone() override;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override;
    static m3d::Class m_classElectronicDigitalWnd;
};
