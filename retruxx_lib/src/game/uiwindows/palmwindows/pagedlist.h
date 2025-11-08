#pragma once

class WndItem :  public m3d::ui::Wnd
{
public:
    void SetIdx(int);
    WndItem();
    int GetIdx() const ;
    virtual ~WndItem();
protected:
    virtual int OnWndNotify(class m3d::ui::Wnd *,unsigned int,unsigned int,class m3d::AIParam const &);
private:
    int m_idx;
};
