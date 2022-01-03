#include <stdexcept>
#include <posteffects/posteffectmanager.h>

char const* PostEffectManager::GetCallbackName() const
{
    return nullptr;
}

void PostEffectManager::OnBeforeDeviceReset()
{
    throw std::logic_error("Not implemented");
}

void PostEffectManager::OnAfterDeviceReset()
{
    throw std::logic_error("Not implemented");
}

PostEffectManager::~PostEffectManager()
{
    throw std::logic_error("Not implemented");
}

PostEffectManager::PostEffectManager()
{
    throw std::logic_error("Not implemented");
}

bool PostEffectManager::GetParam(CStr const&, float&)
{
    throw std::logic_error("Not implemented");
}

void PostEffectManager::UnregisterEffect(PostEffect*)
{
    throw std::logic_error("Not implemented");
}

bool PostEffectManager::Initialize()
{
    throw std::logic_error("Not implemented");
}

bool PostEffectManager::SetParam(CStr const&, float)
{
    throw std::logic_error("Not implemented");
}

void PostEffectManager::Render(bool)
{
    throw std::logic_error("Not implemented");
}

bool PostEffectManager::AddEffect(CStr const&, float, unsigned)
{
    throw std::logic_error("Not implemented");
}

bool PostEffectManager::KillEffect(CStr const&)
{
    throw std::logic_error("Not implemented");
}

void PostEffectManager::Destroy()
{
    throw std::logic_error("Not implemented");
}

void PostEffectManager::LoadFromXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
{
    throw std::logic_error("Not implemented");
}

void PostEffectManager::InitShaders()
{
    throw std::logic_error("Not implemented");
}

bool PostEffectManager::Reload()
{
    Destroy();
    return Initialize();
}
