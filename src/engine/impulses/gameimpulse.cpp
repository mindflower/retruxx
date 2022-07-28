#include <stdexcept>
#include <impulses/i_impulses.h>
#include "gameimpulse.h"
#include <core/log.h>

#include "config.h"
#include "m3dapp.h"

namespace m3d
{
    void AuxImpulseInfo::UnpackXy(float*, float*, float*, float*) const
    {
        throw std::logic_error("Not implemented");
    }

    AuxImpulseInfo::AuxImpulseInfo(int, bool, int, unsigned, unsigned)
    {
        throw std::logic_error("Not implemented");
    }

    float AuxImpulseInfo::UnpackWheel() const
    {
        throw std::logic_error("Not implemented");
    }

    RT_CLASS_DEFINE(GameImpulse);

    int GameImpulse::LoadFromFile(CStr const& bindFile)
    {
        int res = 0;
        if (m_isInited)
        {
            res = 1;
            m_bSuppressEvent = true;
	        if (m_isBinded)
	        {
                UnbindAll();
	        }
            auto err = g_Kernel->GetScriptServer().executeScriptFile(bindFile.c_str());
            if (err)
            {
                M3D_LOG_ERR("Key bindings: loading error - " + GetFormattedScriptErrorDesc(err));
                res = 0;
            }
            else
            {
                Application::g_pApp->EnqueueMessage(46, -1, 0, 0, 0, {}, {});
                M3D_LOG_INFO("Key bindings: were loaded successfully from file " + bindFile);
            }
            m_bSuppressEvent = false;
        }
        else
        {
            M3D_LOG_INFO("Key bindings: error loading from file cause is not inited");
            res = 0;
        }
        return res;
    }

    int GameImpulse::DecRef()
    {
        throw std::logic_error("Not implemented");
    }

    void GameImpulse::ResetImpulseWithoutNotification(int)
    {
        throw std::logic_error("Not implemented");
    }

    bool GameImpulse::GetImpulseStateAndReset(int)
    {
        throw std::logic_error("Not implemented");
    }

    GameImpulse::~GameImpulse()
    {
        throw std::logic_error("Not implemented");
    }

    void* GameImpulse::QueryIface(char const*)
    {
        throw std::logic_error("Not implemented");
    }

    int GameImpulse::Init()
    {
        if (m_isInited)
        {
            M3D_LOG_INFO("Key bindings: fail to init because already is inited");
            return 0;
        }
        g_Kernel->UnRegisterGlobal("IMPULSES");
        g_Kernel->RegisterGlobal(this, "IMPULSES");
        m_isInited = true;
        M3D_LOG_INFO("Key bindings: were inited successfully");
        return 1;
    }

    int GameImpulse::SaveToDefaults()
    {
        throw std::logic_error("Not implemented");
    }

    int GameImpulse::Done()
    {
        throw std::logic_error("Not implemented");
    }

    void GameImpulse::ResetAllImpulses(bool)
    {
        throw std::logic_error("Not implemented");
    }

    int GameImpulse::SetImpulsesStateBySet(KeysSet, bool, int, ui::Wnd*)
    {
        throw std::logic_error("Not implemented");
    }

    int GameImpulse::SetImpulseState(AuxImpulseInfo const&, ui::Wnd*)
    {
        throw std::logic_error("Not implemented");
    }

    void GameImpulse::RaiseOneTimeImpulse(AuxImpulseInfo const&)
    {
        throw std::logic_error("Not implemented");
    }

    bool GameImpulse::GetImpulseState(int)
    {
        throw std::logic_error("Not implemented");
    }

    int GameImpulse::GetImpulseForKeys(std::vector<int, std::allocator<int>>, int)
    {
        throw std::logic_error("Not implemented");
    }

    void GameImpulse::BindKey0(int, KeysSet const&, int)
    {
        throw std::logic_error("Not implemented");
    }

    int GameImpulse::BindKey1(CStr const&, CStr const&, CStr const&)
    {
        throw std::logic_error("Not implemented");
    }

    int GameImpulse::IncRef()
    {
        throw std::logic_error("Not implemented");
    }

    std::vector<std::vector<int>> GameImpulse::GetKeysForImpulse(int, int)
    {
        throw std::logic_error("Not implemented");
    }

    int GameImpulse::HandleKeyboardMouseEvent(Event const&, ui::Wnd*)
    {
        throw std::logic_error("Not implemented");
    }

    int GameImpulse::FilterShifts(int&, bool&)
    {
        throw std::logic_error("Not implemented");
    }

    int GameImpulse::BindKey2(CStr const&, CStr const&, CStr const&, CStr const&)
    {
        throw std::logic_error("Not implemented");
    }

    int GameImpulse::SaveToProfile()
    {
        throw std::logic_error("Not implemented");
    }

    int GameImpulse::HandleBinding(int, CStr const&, CStr const&, CStr const&, CStr const&, CStr const&)
    {
        throw std::logic_error("Not implemented");
    }

    int GameImpulse::BindKey3(CStr const&, CStr const&, CStr const&, CStr const&, CStr const&)
    {
        throw std::logic_error("Not implemented");
    }

    CStr GameImpulse::GetFormattedScriptErrorDesc(eScriptError) const
    {
        throw std::logic_error("Not implemented");
    }

    int GameImpulse::UnbindKey3(CStr const&, CStr const&, CStr const&, CStr const&, CStr const&)
    {
        throw std::logic_error("Not implemented");
    }

    Class* GameImpulse::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Object);
    }

    int GameImpulse::LoadFromDefaults()
    {
        if (m_isInited)
        {
            auto res = LoadFromFile(g_Kernel->GetEngineCfg().m_pathToDefaultKeyBindings.GetS());
	        if (res)
	        {
                M3D_LOG_INFO("Key bindings: were bind from defaults");
	        }
            return res;
        }
        M3D_LOG_INFO("Key bindings: error load from defaults cause impulses were not inited");
        return 0;
    }

    int GameImpulse::UnbindKey2(CStr const&, CStr const&, CStr const&, CStr const&)
    {
        throw std::logic_error("Not implemented");
    }

    void GameImpulse::UnbindAll()
    {
        throw std::logic_error("Not implemented");
    }

    int GameImpulse::GetKeyIdByName(CStr const&)
    {
        throw std::logic_error("Not implemented");
    }

    int GameImpulse::UnbindKey1(CStr const&, CStr const&, CStr const&)
    {
        throw std::logic_error("Not implemented");
    }

    CStr GameImpulse::GetKeyNameById(int)
    {
        throw std::logic_error("Not implemented");
    }

    Class* GameImpulse::GetRtClass() const
    {
        throw std::logic_error("Not implemented");
    }

    int GameImpulse::LoadFromProfile()
    {
        throw std::logic_error("Not implemented");
    }

    void GameImpulse::UnbindKey0(int, KeysSet const&, int)
    {
        throw std::logic_error("Not implemented");
    }

    int GameImpulse::SaveToFile(CStr const&)
    {
        throw std::logic_error("Not implemented");
    }

    GameImpulse::GameImpulse()
    {
    }
}
