#pragma once

class FadingMsgItem :  public m3d::ui::Wnd
{
public:
    static struct m3d::Class * __fastcall GetBaseClass();
    virtual class m3d::Object * Clone();
    int SetupForMsg(CStr const &,class PointBase<float> const &,float,enum FadingMsgList::MsgType);
    virtual ~FadingMsgItem();
    virtual struct m3d::Class * GetClass() const ;
    static class m3d::Object * __fastcall CreateObject();
protected:
    void Update();
    void StartFade();
    void Fade();
    FadingMsgItem(class FadingMsgItem const &);
    FadingMsgItem();
private:
    FadingMsgItem::Stage m_state;
    unsigned int m_showTime;
    FadingMsgList::MsgType m_msgType;
};

class FadingMsgList :  public m3d::ui::Wnd
{
public:
    static struct m3d::Class * __fastcall GetBaseClass();
    int AddMsgT(CStr const &,class std::vector<class m3d::AIParam,class std::allocator<class m3d::AIParam> > const &);
    int AddMsgByStrId(CStr const &,class std::vector<class m3d::AIParam,class std::allocator<class m3d::AIParam> > const &);
    int AddMsg(CStr const &,class std::vector<class m3d::AIParam,class std::allocator<class m3d::AIParam> > const &);
    virtual class m3d::Object * Clone();
    virtual ~FadingMsgList();
    virtual struct m3d::Class * GetClass() const ;
    static class m3d::Object * __fastcall CreateObject();
protected:
    void OnNewFrame();
    int PushBack(class FadingMsgItem *);
    float GetSummaryHeight();
    int AddEnumeredMsg(int,struct m3d::Event const *);
    void GetParamsForEnumeredMsg(int,struct m3d::Event const *,class std::vector<class m3d::AIParam,class std::allocator<class m3d::AIParam> > &) const ;
    void RecalcLayot();
    CStr MsgId2Name(int) const ;
    int InsertMsg(class FadingMsgItem *);
    void OnFadingMessage(void *);
    FadingMsgList();
    FadingMsgList(class FadingMsgList const &);
    int PopFront();
    int Scroll(class FadingMsgItem *);
    virtual int GameDataUpdate(void *,int);
    virtual int GameDataSetup();
    virtual int GameDataClear(bool);
private:
    std::deque<FadingMsgItem *> m_msgList;
    FadingMsgList::AuxInfo m_aif;
    FadingMsgList::MsgType m_msgType;
    float m_maxSummaryH;
    float m_extraH;
    FadingMsgList::Direction m_direction;
};
