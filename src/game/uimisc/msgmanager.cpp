#include "msgmanager.h"

RT_CLASS_EXPORT_METHOD_DEFINE(MsgManager, ShowMsgBox)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORTS_BEGIN(MsgManager)
	RT_CLASS_EXPORT(MsgManager, m3d::METHOD, ShowMsgBox, "", "", "")
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(MsgManager);

int MsgManager::ShowMsgBox(int, bool)
{
    throw std::logic_error("Not implemented");
}

void MsgManager::Clear(bool)
{
    throw std::logic_error("Not implemented");
}

m3d::Class* MsgManager::GetClass() const
{
    throw std::logic_error("Not implemented");
}

m3d::Class* MsgManager::GetBaseClass()
{
    throw std::logic_error("Not implemented");
}

MsgInfo const* MsgManager::GetMsgInfo(int) const
{
    throw std::logic_error("Not implemented");
}

m3d::Object* MsgManager::CreateObject()
{
    return new MsgManager;
}

MsgManager::~MsgManager()
{
    throw std::logic_error("Not implemented");
}

m3d::Object* MsgManager::Clone()
{
    throw std::logic_error("Not implemented");
}

int MsgManager::AddMsg(MsgInfo*, bool)
{
    throw std::logic_error("Not implemented");
}

MsgManager::MsgManager(MsgManager const&)
{
    throw std::logic_error("Not implemented");
}

MsgManager::MsgManager()
{
}

int MsgManager::LoadFromXml(CStr const&, bool)
{
    throw std::logic_error("Not implemented");
}
