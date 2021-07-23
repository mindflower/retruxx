#pragma once

class ElectronicDigitalWnd :  public m3d::ui::Wnd
{
public:
    virtual class m3d::Object * Clone();
    static struct m3d::Class * __fastcall GetBaseClass();
    virtual ~ElectronicDigitalWnd();
    void SetDigitalColor(unsigned int);
    int CreateFromPattern(class m3d::ui::Wnd *,bool);
    static class m3d::Object * __fastcall CreateObject();
    int ShowNumber(int,bool,unsigned int,bool);
    int ShowSymbol(enum Symbol,bool,unsigned int,bool);
    void SetDigitalSize(enum DigitalSize);
    virtual struct m3d::Class * GetClass() const ;
    void Clear();
protected:
    int GetFirstFreeSlot() const ;
    ElectronicDigitalWnd(class ElectronicDigitalWnd const &);
    ElectronicDigitalWnd();
    virtual int GameDataClear(bool);
private:
    std::fill<Digital *,Digital>(Digital *,Digital *,Digital const &);
    std::_Ptr_cat<Digital *,Digital *>(Digital * &,Digital * &);
    std::copy_backward<Digital *,Digital *>(Digital *,Digital *,Digital *);
    std::_Construct<Digital,Digital>(Digital *,Digital const &);
    std::_Copy_backward_opt<Digital *,Digital *>(Digital *,Digital *,Digital *,std::_Nonscalar_ptr_iterator_tag);
    std::_Allocate<Digital>(uint,Digital *);
    std::allocator<Digital>::allocator<Digital>();
    std::allocator<Digital>::allocator<Digital>();
    std::_Destroy<Digital>(Digital *);
    ElectronicDigitalWnd::DigitalSize m_digitalSize;
    std::vector<ElectronicDigitalWnd::Digital> m_digitals;
    unsigned int m_digitalColor;
};
