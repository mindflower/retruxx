#pragma once

class NewProfileWnd :  public m3d::ui::ModalWnd
{
public:
    virtual class m3d::Object * Clone();
    virtual struct m3d::Class * GetClass() const ;
    static class m3d::Object * CreateObject();
    static struct m3d::Class * GetBaseClass();
    virtual ~NewProfileWnd();
protected:
    virtual int OnWndNotify(class m3d::ui::Wnd *,unsigned int,unsigned int,class m3d::AIParam const &);
    virtual int OnBeforeAddToWndStation();
    virtual int GameDataSetup();
    void OnEditChange();
    virtual int OnKey(unsigned short,unsigned char,unsigned int);
    int CreateNewProfile();
    NewProfileWnd(class NewProfileWnd const &);
    NewProfileWnd();
private:
    m3d::ui::EditWnd *m_editName;
    m3d::ui::ButtonWnd *m_btnCancel;
    NewProfileWnd::AuxInfo m_aif;
};
