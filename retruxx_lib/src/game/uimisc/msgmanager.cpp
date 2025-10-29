#include "msgmanager.h"

#include "config.h"
#include "core/kernel.h"
#include <client.h>

#include "level.h"
#include "world.h"

RT_CLASS_EXPORT_METHOD_DEFINE(MsgManager, ShowMsgBox)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORTS_BEGIN(MsgManager)
	RT_CLASS_EXPORT(MsgManager, m3d::METHOD, ShowMsgBox, "", "", "")
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(MsgManager);

int MsgManager::ShowMsgBox(int, bool)
{
    RETRUXX_NOT_IMPLEMENTED;
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

void MsgManager::Clear(bool)
{
    // TODO: implement MsgManager::Clear
    //RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* MsgManager::GetClass() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* MsgManager::GetBaseClass()
{
    return RT_CLASS_LOCAL(Object);
}

MsgInfo const* MsgManager::GetMsgInfo(int) const
{
    RETRUXX_NOT_IMPLEMENTED;
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
