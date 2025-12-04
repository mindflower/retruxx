#include "msgmanager.h"

#include "config.h"
#include "core/kernel.h"
#include <client.h>

#include "level.h"
#include "world.h"

RT_CLASS_EXPORT_METHOD_DEFINE(MsgManager, ShowMsgBox)
{
    auto manager = (MsgManager*)context->asObject(0, "MsgManager");
    auto msg = context->asInt(1);
    auto pause = context->asBool(2);
    auto res = manager->ShowMsgBox(msg, pause);
    context->pushInt(res);
    return 1;
}

RT_CLASS_EXPORTS_BEGIN(MsgManager)
	RT_CLASS_EXPORT(MsgManager, m3d::METHOD, ShowMsgBox, "", "", "")
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(MsgManager);

MsgInfo::~MsgInfo()
{
    RETRUXX_NOT_IMPLEMENTED;
}

MsgInfo::MsgType MsgInfo::GetMsgType() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int MsgManager::ShowMsgBox(int, bool)
{
    // TODO: implement MsgManager::ShowMsgBox
    // RETRUXX_NOT_IMPLEMENTED;
    return 1;
}

int MsgManager::Init(bool bGlobal)
{
    Clear(bGlobal);

    CStr fileName;
    if (bGlobal)
    {
        fileName = M3D_ENGINE_CFG.m_ui_pathToStrings.GetS();
    }
    else
    {
        fileName = m3d::pClient->GetWorld().m_level->GetFullPathNameA(m3d::pClient->GetWorld().m_level->m_dialogStrings);
    }
    return LoadFromXml(fileName, bGlobal);
}

void MsgManager::Clear(bool bGlobal)
{
    for (auto& info : m_levelMsgs)
    {
        delete info.second;
    }
    m_levelMsgs.clear();

    if (bGlobal)
    {
        for (auto& info : m_globalMsgs)
        {
            delete info.second;
        }
        m_globalMsgs.clear();
    }
}

m3d::Class* MsgManager::GetClass() const
{
    return RT_CLASS_LOCAL(MsgManager);
}

m3d::Class* MsgManager::GetBaseClass()
{
    return RT_CLASS_LOCAL(Object);
}

const MsgInfo* MsgManager::GetMsgInfo(int msgId) const
{
    auto it = m_levelMsgs.find(msgId);
    if (it != m_levelMsgs.end())
    {
        return it->second;
    }

    auto globalIt = m_globalMsgs.find(msgId);
    if (globalIt != m_globalMsgs.end())
    {
        return globalIt->second;
    }
    return nullptr;
}

m3d::Object* MsgManager::CreateObject()
{
    return new MsgManager;
}

MsgManager::~MsgManager()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* MsgManager::Clone()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int MsgManager::AddMsg(MsgInfo*, bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

MsgManager::MsgManager(MsgManager const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

MsgManager::MsgManager()
{
}

int MsgManager::LoadFromXml(CStr const&, bool)
{
    // TODO: implement MsgManager::LoadFromXml
    //RETRUXX_NOT_IMPLEMENTED;
    return 0;
}
