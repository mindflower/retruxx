#include "reply.h"
#include <stdexcept>

RT_CLASS_EXPORT_METHOD_DEFINE(RepliesManager, SetConditionValue)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(RepliesManager, Init)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORTS_BEGIN(RepliesManager)
RT_CLASS_EXPORT(RepliesManager, m3d::METHOD, SetConditionValue, "", "", "")
RT_CLASS_EXPORT(RepliesManager, m3d::METHOD, Init, "", "", "")
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(RepliesManager);

void RepliesManager::SetConditionValue(int)
{
    throw std::logic_error("Not implemented");
}

int RepliesManager::Init()
{
    throw std::logic_error("Not implemented");
}

Reply* RepliesManager::GetFirstFitReply(std::vector<CStr> const&, Reply::Role, ai::Npc*)
{
    throw std::logic_error("Not implemented");
}

m3d::Class* RepliesManager::GetClass() const
{
    return RT_CLASS_LOCAL(RepliesManager);
}

RepliesManager::~RepliesManager()
{
    throw std::logic_error("Not implemented");
}

std::vector<Reply*> RepliesManager::GetNextPlayerReplies(Reply const*, ai::Npc*)
{
    throw std::logic_error("Not implemented");
}

std::vector<CStr> RepliesManager::GetAllHelloReplyNamesForDynamicNpc()
{
    throw std::logic_error("Not implemented");
}

void RepliesManager::Clear()
{
    throw std::logic_error("Not implemented");
}

Reply* RepliesManager::GetPlayerRequestForDynamicQuest(ai::Npc*, ai::DynamicQuest const*)
{
    throw std::logic_error("Not implemented");
}

int RepliesManager::ExecuteScript(CStr const&, ai::Npc*) const
{
    throw std::logic_error("Not implemented");
}

int RepliesManager::LoadFromXml(CStr const&)
{
    throw std::logic_error("Not implemented");
}

Reply* RepliesManager::GetReplyByName(CStr const&) const
{
    throw std::logic_error("Not implemented");
}

m3d::Object* RepliesManager::Clone()
{
    throw std::logic_error("Not implemented");
}

std::vector<Reply*> RepliesManager::GetAllFitReplies(std::vector<CStr> const&, Reply::Role, ai::Npc*)
{
    throw std::logic_error("Not implemented");
}

m3d::Object* RepliesManager::CreateObject()
{
    return new RepliesManager;
}

Reply* RepliesManager::GetRandomFitReply(std::vector<CStr, std::allocator<CStr>> const&, Reply::Role, ai::Npc*)
{
    throw std::logic_error("Not implemented");
}

Reply* RepliesManager::GetNextNpcReply(Reply const*, ai::Npc*)
{
    throw std::logic_error("Not implemented");
}

m3d::Class* RepliesManager::GetBaseClass()
{
    return RT_CLASS_LOCAL(Object);
}

Reply* RepliesManager::GetFirstNpcReply(ai::Npc*)
{
    throw std::logic_error("Not implemented");
}

RepliesManager::RepliesManager(RepliesManager const&)
{
    throw std::logic_error("Not implemented");
}

RepliesManager::RepliesManager()
{
}
