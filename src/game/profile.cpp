#include "profile.h"

Profile* ProfileManager::GetCurProfile() const
{
    return _GetProfileByName(m_curProfileName);
}
