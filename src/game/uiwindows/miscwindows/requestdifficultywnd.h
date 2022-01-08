#pragma once

class RequestDifficultyWnd :  public m3d::ui::ModalWnd
{
public:
    virtual struct m3d::Class * GetClass() const ;
    virtual class m3d::Object * Clone();
    virtual ~RequestDifficultyWnd();
    static class m3d::Object * CreateObject();
    static struct m3d::Class * GetBaseClass();
protected:
    RequestDifficultyWnd(class RequestDifficultyWnd const &);
    RequestDifficultyWnd();
    void FillDifficultyLevelsList();
    virtual int OnKey(unsigned short,unsigned char,unsigned int);
    virtual int OnBeforeRemoveFromWndStation();
    void Clear();
    void ApplyDifficultyLevel();
    virtual int OnBeforeAddToWndStation();
    virtual int GameDataSetup();
private:
    m3d::ui::ComboBoxWnd *m_cbDifficultyLevels;
    RequestDifficultyWnd::AuxInfo m_aif;
};
