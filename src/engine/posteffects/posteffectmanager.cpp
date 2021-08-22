#include <posteffects/posteffectmanager.h>

char const* PostEffectManager::GetCallbackName() const
{
    return nullptr;
}

bool PostEffectManager::Reload()
{
    Destroy();
    return Initialize();
}
