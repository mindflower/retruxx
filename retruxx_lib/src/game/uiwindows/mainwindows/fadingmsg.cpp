#include "fadingmsg.h"

RT_CLASS_EXPORTS_BEGIN(FadingMsgList)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(FadingMsgList);

FadingMsgList::AuxInfo::AuxInfo()
{
    m_space = 8.0;
    m_unimportantExtraH = 40.0;
    m_importantExtraH = 46.0;
    m_unimportantMaxNumOfMsgs = 3;
    m_importantMaxNumOfMsgs = 1;
}

m3d::Class* FadingMsgList::GetBaseClass()
{
    return RT_CLASS_LOCAL(Wnd);
}

int FadingMsgList::AddMsgT(CStr const&, std::vector<m3d::AIParam, std::allocator<m3d::AIParam>> const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int FadingMsgList::AddMsgByStrId(CStr const&, std::vector<m3d::AIParam, std::allocator<m3d::AIParam>> const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int FadingMsgList::AddMsg(CStr const&, std::vector<m3d::AIParam, std::allocator<m3d::AIParam>> const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* FadingMsgList::Clone()
{
    RETRUXX_NOT_IMPLEMENTED;
}

FadingMsgList::~FadingMsgList()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* FadingMsgList::GetClass() const
{
    return RT_CLASS_LOCAL(FadingMsgList);
}

m3d::Object* FadingMsgList::CreateObject()
{
    return new FadingMsgList;
}

void FadingMsgList::OnNewFrame()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int FadingMsgList::PushBack(FadingMsgItem*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

float FadingMsgList::GetSummaryHeight()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int FadingMsgList::AddEnumeredMsg(int, m3d::Event const*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void FadingMsgList::GetParamsForEnumeredMsg(int, m3d::Event const*, std::vector<m3d::AIParam, std::allocator<m3d::AIParam>>&) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void FadingMsgList::RecalcLayot()
{
    RETRUXX_NOT_IMPLEMENTED;
}

CStr FadingMsgList::MsgId2Name(int) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int FadingMsgList::InsertMsg(FadingMsgItem*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void FadingMsgList::OnFadingMessage(void*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

FadingMsgList::FadingMsgList()
{
    m_direction = DIR_UP;
    m_msgType = MSGTYPE_INVALID;
    m_maxSummaryH = 0.0;
    m_extraH = 0.0;
}

FadingMsgList::FadingMsgList(FadingMsgList const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int FadingMsgList::PopFront()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int FadingMsgList::Scroll(FadingMsgItem*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int FadingMsgList::GameDataUpdate(void*, int)
{
    // TODO: implement GameDataUpdate
    //  RETRUXX_NOT_IMPLEMENTED;
    return 0;
}

int FadingMsgList::GameDataSetup()
{
    // TODO: implement FadingMsgList::GameDataSetup
    // RETRUXX_NOT_IMPLEMENTED;
    return 1;
}

int FadingMsgList::GameDataClear(bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORTS_BEGIN(FadingMsgItem)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(FadingMsgItem);

m3d::Class* FadingMsgItem::GetBaseClass()
{
    return RT_CLASS_LOCAL(Wnd);
}

m3d::Object* FadingMsgItem::Clone()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int FadingMsgItem::SetupForMsg(CStr const&, PointBase<float> const&, float, FadingMsgList::MsgType)
{
    RETRUXX_NOT_IMPLEMENTED;
}

FadingMsgItem::~FadingMsgItem()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* FadingMsgItem::GetClass() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* FadingMsgItem::CreateObject()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void FadingMsgItem::Update()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void FadingMsgItem::StartFade()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void FadingMsgItem::Fade()
{
    RETRUXX_NOT_IMPLEMENTED;
}

FadingMsgItem::FadingMsgItem(FadingMsgItem const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

FadingMsgItem::FadingMsgItem()
{
    RETRUXX_NOT_IMPLEMENTED;
}
