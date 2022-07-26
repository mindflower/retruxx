#pragma once
#include <ui/wnd.h>

namespace m3d
{
    namespace ui
    {
        class ComboBoxWnd;
    }
}

class RequestDifficultyWnd :  public m3d::ui::ModalWnd
{
public:
    class AuxInfo
    {
    public:
        AuxInfo();

    public:
        CStr m_cbDifficultyLevelsName = "cbDifficultyLevels";
    };

public:
    virtual m3d::Class * GetClass() const ;
    virtual m3d::Object * Clone();
    virtual ~RequestDifficultyWnd();
    static m3d::Object * CreateObject();
    static m3d::Class * GetBaseClass();

protected:
    RequestDifficultyWnd(RequestDifficultyWnd const &);
    RequestDifficultyWnd();
    void FillDifficultyLevelsList();
    virtual int OnKey(unsigned short,unsigned char,unsigned int);
    virtual int OnBeforeRemoveFromWndStation();
    void Clear();
    void ApplyDifficultyLevel();
    virtual int OnBeforeAddToWndStation();
    virtual int GameDataSetup();

public:
    RT_CLASS_DECLARE(RequestDifficultyWnd);

private:
    m3d::ui::ComboBoxWnd *m_cbDifficultyLevels = nullptr;
    RequestDifficultyWnd::AuxInfo m_aif;
};
