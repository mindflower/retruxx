#pragma once

class MsgManager :  public m3d::Object
{
public:
    int ShowMsgBox(int,bool);
    void Clear(bool);
    virtual m3d::Class * GetClass() const ;
    static m3d::Class * GetBaseClass();
    class MsgInfo const * GetMsgInfo(int) const ;
    static m3d::Object * CreateObject();
    virtual ~MsgManager();
    virtual m3d::Object * Clone();

protected:
    int AddMsg(MsgInfo *,bool);
    MsgManager(MsgManager const &);
    MsgManager();
    int LoadFromXml(CStr const &,bool);

private:
    std::map<int,MsgInfo *> m_globalMsgs;
    std::map<int,MsgInfo *> m_levelMsgs;
};
