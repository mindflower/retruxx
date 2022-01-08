#pragma once

class MsgManager :  public m3d::Object
{
public:
    int ShowMsgBox(int,bool);
    void Clear(bool);
    virtual struct m3d::Class * GetClass() const ;
    void m3d::FadingInit();
    m3d::Fading~FadingMsgManager();
    void m3d::FadingShowMessage(CStr const &);
    m3d::FadingFadingMsgManager();
    static struct m3d::Class * GetBaseClass();
    class MsgInfo const * GetMsgInfo(int) const ;
    static class m3d::Object * CreateObject();
    virtual ~MsgManager();
    virtual class m3d::Object * Clone();
protected:
    int AddMsg(class MsgInfo *,bool);
    MsgManager(class MsgManager const &);
    MsgManager();
    int LoadFromXml(CStr const &,bool);
private:
    m3d::FadingInit();
    std::map<int,MsgInfo *> m_globalMsgs;
    std::map<int,MsgInfo *> m_levelMsgs;
};
