#include "fadingmsg.h"

RT_CLASS_EXPORTS_BEGIN(FadingMsgList)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(FadingMsgList);

FadingMsgList::AuxInfo::AuxInfo()
{
    throw std::logic_error("Not implemented");
}

m3d::Class* FadingMsgList::GetBaseClass()
{
    throw std::logic_error("Not implemented");
}

int FadingMsgList::AddMsgT(CStr const&, std::vector<m3d::AIParam, std::allocator<m3d::AIParam>> const&)
{
    throw std::logic_error("Not implemented");
}

int FadingMsgList::AddMsgByStrId(CStr const&, std::vector<m3d::AIParam, std::allocator<m3d::AIParam>> const&)
{
    throw std::logic_error("Not implemented");
}

int FadingMsgList::AddMsg(CStr const&, std::vector<m3d::AIParam, std::allocator<m3d::AIParam>> const&)
{
    throw std::logic_error("Not implemented");
}

m3d::Object* FadingMsgList::Clone()
{
    throw std::logic_error("Not implemented");
}

FadingMsgList::~FadingMsgList()
{
    throw std::logic_error("Not implemented");
}

m3d::Class* FadingMsgList::GetClass() const
{
    throw std::logic_error("Not implemented");
}

m3d::Object* FadingMsgList::CreateObject()
{
    throw std::logic_error("Not implemented");
}

void FadingMsgList::OnNewFrame()
{
    throw std::logic_error("Not implemented");
}

int FadingMsgList::PushBack(FadingMsgItem*)
{
    throw std::logic_error("Not implemented");
}

float FadingMsgList::GetSummaryHeight()
{
    throw std::logic_error("Not implemented");
}

int FadingMsgList::AddEnumeredMsg(int, m3d::Event const*)
{
    throw std::logic_error("Not implemented");
}

void FadingMsgList::GetParamsForEnumeredMsg(int, m3d::Event const*, std::vector<m3d::AIParam, std::allocator<m3d::AIParam>>&) const
{
    throw std::logic_error("Not implemented");
}

void FadingMsgList::RecalcLayot()
{
    throw std::logic_error("Not implemented");
}

CStr FadingMsgList::MsgId2Name(int) const
{
    throw std::logic_error("Not implemented");
}

int FadingMsgList::InsertMsg(FadingMsgItem*)
{
    throw std::logic_error("Not implemented");
}

void FadingMsgList::OnFadingMessage(void*)
{
    throw std::logic_error("Not implemented");
}

FadingMsgList::FadingMsgList()
{
    throw std::logic_error("Not implemented");
}

FadingMsgList::FadingMsgList(FadingMsgList const&)
{
    throw std::logic_error("Not implemented");
}

int FadingMsgList::PopFront()
{
    throw std::logic_error("Not implemented");
}

int FadingMsgList::Scroll(FadingMsgItem*)
{
    throw std::logic_error("Not implemented");
}

int FadingMsgList::GameDataUpdate(void*, int)
{
    throw std::logic_error("Not implemented");
}

int FadingMsgList::GameDataSetup()
{
    throw std::logic_error("Not implemented");
}

int FadingMsgList::GameDataClear(bool)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORTS_BEGIN(FadingMsgItem)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(FadingMsgItem);

m3d::Class* FadingMsgItem::GetBaseClass()
{
    throw std::logic_error("Not implemented");
}

m3d::Object* FadingMsgItem::Clone()
{
    throw std::logic_error("Not implemented");
}

int FadingMsgItem::SetupForMsg(CStr const&, PointBase<float> const&, float, FadingMsgList::MsgType)
{
    throw std::logic_error("Not implemented");
}

FadingMsgItem::~FadingMsgItem()
{
    throw std::logic_error("Not implemented");
}

m3d::Class* FadingMsgItem::GetClass() const
{
    throw std::logic_error("Not implemented");
}

m3d::Object* FadingMsgItem::CreateObject()
{
    throw std::logic_error("Not implemented");
}

void FadingMsgItem::Update()
{
    throw std::logic_error("Not implemented");
}

void FadingMsgItem::StartFade()
{
    throw std::logic_error("Not implemented");
}

void FadingMsgItem::Fade()
{
    throw std::logic_error("Not implemented");
}

FadingMsgItem::FadingMsgItem(FadingMsgItem const&)
{
    throw std::logic_error("Not implemented");
}

FadingMsgItem::FadingMsgItem()
{
    throw std::logic_error("Not implemented");
}
