#pragma once
#include <deque>
#include <ui/ui.h>

namespace m3d
{
    struct Event;
}

class FadingMsgList : public m3d::ui::Wnd
{
public:
    class AuxInfo
    {
    public:
        AuxInfo();

    private:
        float m_space;
        float m_unimportantExtraH;
        float m_importantExtraH;
        int m_unimportantMaxNumOfMsgs;
        int m_importantMaxNumOfMsgs;
    };

    enum MsgType
    {
        MSGTYPE_UNIMPORTANT = 0x0,
        MSGTYPE_IMPORTANT = 0x1,
        MSGTYPE_INVALID = 0x2,
    };

    enum Direction
    {
        DIR_UP = 0x0,
        DIR_DOWN = 0x1,
    };

public:
    static m3d::Class* GetBaseClass();
    int AddMsgT(CStr const&, std::vector<m3d::AIParam, std::allocator<m3d::AIParam> > const&);
    int AddMsgByStrId(CStr const&, std::vector<m3d::AIParam, std::allocator<m3d::AIParam> > const&);
    int AddMsg(CStr const&, std::vector<m3d::AIParam, std::allocator<m3d::AIParam> > const&);
    virtual m3d::Object* Clone();
    virtual ~FadingMsgList();
    virtual m3d::Class* GetClass() const;
    static m3d::Object* CreateObject();

protected:
    void OnNewFrame();
    int PushBack(class FadingMsgItem*);
    float GetSummaryHeight();
    int AddEnumeredMsg(int, m3d::Event const*);
    void GetParamsForEnumeredMsg(int, m3d::Event const*, std::vector<m3d::AIParam, std::allocator<m3d::AIParam> >&) const;
    void RecalcLayot();
    CStr MsgId2Name(int) const;
    int InsertMsg(FadingMsgItem*);
    void OnFadingMessage(void*);
    FadingMsgList();
    FadingMsgList(FadingMsgList const&);
    int PopFront();
    int Scroll(FadingMsgItem*);
    virtual int GameDataUpdate(void*, int);
    virtual int GameDataSetup();
    virtual int GameDataClear(bool);

public:
    RT_CLASS_DECLARE(FadingMsgList);

private:
    std::deque<FadingMsgItem*> m_msgList;
    FadingMsgList::AuxInfo m_aif;
    FadingMsgList::MsgType m_msgType;
    float m_maxSummaryH;
    float m_extraH;
    FadingMsgList::Direction m_direction;
};

class FadingMsgItem :  public m3d::ui::Wnd
{
public:
    enum Stage
    {
        STATE_SHOW = 0x0,
        STATE_FADE = 0x1,
        STATE_HIDE = 0x2,
    };

public:
    static m3d::Class * GetBaseClass();
    virtual m3d::Object * Clone();
    int SetupForMsg(CStr const &, PointBase<float> const &,float, FadingMsgList::MsgType);
    virtual ~FadingMsgItem();
    virtual m3d::Class * GetClass() const ;
    static m3d::Object * CreateObject();

protected:
    void Update();
    void StartFade();
    void Fade();
    FadingMsgItem(FadingMsgItem const &);
    FadingMsgItem();

public:
    RT_CLASS_DECLARE(FadingMsgItem);

private:
    FadingMsgItem::Stage m_state;
    unsigned int m_showTime;
    FadingMsgList::MsgType m_msgType;
};

