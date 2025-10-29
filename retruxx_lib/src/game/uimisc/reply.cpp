#include "reply.h"
#include <stdexcept>

RT_CLASS_EXPORT_METHOD_DEFINE(RepliesManager, SetConditionValue)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORT_METHOD_DEFINE(RepliesManager, Init)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORTS_BEGIN(RepliesManager)
RT_CLASS_EXPORT(RepliesManager, m3d::METHOD, SetConditionValue, "", "", "")
RT_CLASS_EXPORT(RepliesManager, m3d::METHOD, Init, "", "", "")
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(RepliesManager);

void RepliesManager::SetConditionValue(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int RepliesManager::Init()
{
    RETRUXX_NOT_IMPLEMENTED;
}

Reply* RepliesManager::GetFirstFitReply(std::vector<CStr> const&, Reply::Role, ai::Npc*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* RepliesManager::GetClass() const
{
    return RT_CLASS_LOCAL(RepliesManager);
}

RepliesManager::~RepliesManager()
{
    RETRUXX_NOT_IMPLEMENTED;
}

std::vector<Reply*> RepliesManager::GetNextPlayerReplies(Reply const*, ai::Npc*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

std::vector<CStr> RepliesManager::GetAllHelloReplyNamesForDynamicNpc()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void RepliesManager::Clear()
{
    RETRUXX_NOT_IMPLEMENTED;
}

Reply* RepliesManager::GetPlayerRequestForDynamicQuest(ai::Npc*, ai::DynamicQuest const*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int RepliesManager::ExecuteScript(CStr const&, ai::Npc*) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int RepliesManager::LoadFromXml(CStr const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

Reply* RepliesManager::GetReplyByName(CStr const&) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* RepliesManager::Clone()
{
    RETRUXX_NOT_IMPLEMENTED;
}

std::vector<Reply*> RepliesManager::GetAllFitReplies(std::vector<CStr> const&, Reply::Role, ai::Npc*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* RepliesManager::CreateObject()
{
    return new RepliesManager;
}

Reply* RepliesManager::GetRandomFitReply(std::vector<CStr, std::allocator<CStr>> const&, Reply::Role, ai::Npc*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

Reply* RepliesManager::GetNextNpcReply(Reply const*, ai::Npc*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* RepliesManager::GetBaseClass()
{
    return RT_CLASS_LOCAL(Object);
}

Reply* RepliesManager::GetFirstNpcReply(ai::Npc*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RepliesManager::RepliesManager(RepliesManager const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RepliesManager::RepliesManager()
{
}
