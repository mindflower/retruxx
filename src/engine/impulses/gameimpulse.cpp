#include <stdexcept>
#include <impulses/i_impulses.h>
#include "gameimpulse.h"
#include <core/log.h>

#include "config.h"
#include "m3dapp.h"

RT_CLASS_EXPORT_METHOD_DEFINE(GameImpulse, BindKey3)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(GameImpulse, BindKey2)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(GameImpulse, BindKey1)
{
    auto gameImpulse = dynamic_cast<m3d::GameImpulse*>(context->asObject(0, "GameImpulse"));
    auto gameMode = context->asString(1);
    auto key1 = context->asString(2);
    auto imp = context->asString(3);
    auto res = gameImpulse->BindKey1(gameMode, key1, imp);
    context->pushInt(res);
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(GameImpulse, UnbindKey3)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(GameImpulse, UnbindKey2)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(GameImpulse, UnbindKey1)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(GameImpulse, UnbindAll)
{
    auto gameImpulse = dynamic_cast<m3d::GameImpulse*>(context->asObject(0, "GameImpulse"));
	gameImpulse->UnbindAll();
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(GameImpulse, LoadFromDefaults)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(GameImpulse, SaveToDefaults)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(GameImpulse, LoadFromProfile)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(GameImpulse, SaveToProfile)
{
    throw std::logic_error("Not implemented");
}

namespace m3d
{
    RT_CLASS_EXPORTS_BEGIN(GameImpulse)
        RT_CLASS_EXPORT(GameImpulse, METHOD, BindKey3, "", "", "")
        RT_CLASS_EXPORT(GameImpulse, METHOD, BindKey2, "", "", "")
        RT_CLASS_EXPORT(GameImpulse, METHOD, BindKey1, "", "", "")
        RT_CLASS_EXPORT(GameImpulse, METHOD, UnbindKey3, "", "", "")
        RT_CLASS_EXPORT(GameImpulse, METHOD, UnbindKey2, "", "", "")
        RT_CLASS_EXPORT(GameImpulse, METHOD, UnbindKey1, "", "", "")
        RT_CLASS_EXPORT(GameImpulse, METHOD, UnbindAll, "", "", "")
        RT_CLASS_EXPORT(GameImpulse, METHOD, LoadFromDefaults, "", "", "")
        RT_CLASS_EXPORT(GameImpulse, METHOD, SaveToDefaults, "", "", "")
        RT_CLASS_EXPORT(GameImpulse, METHOD, LoadFromProfile, "", "", "")
        RT_CLASS_EXPORT(GameImpulse, METHOD, SaveToProfile, "", "", "")
	RT_CLASS_EXPORTS_END;

    RT_CLASS_DEFINE(GameImpulse);

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

    int GameImpulse::BindKey1(CStr const& strGameMode, CStr const& strKey1, CStr const& strImp)
    {
        if (m_isInited)
        {
            return BindKey3(strGameMode, strKey1, {}, {}, strImp);
        }
        else
        {
            M3D_LOG_INFO("Key bindings: error bind key cause impulses were not inited");
            return 0;
        }
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

    int GameImpulse::BindKey3(CStr const& strGameMode, CStr const& strKey1, CStr const& strKey2, CStr const& strKey3, CStr const& strImp)
    {
        return HandleBinding(1, strGameMode, strKey1, strKey2, strKey3, strImp);
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
        if (m_isInited)
        {
	        for (auto& bind : m_bindings)
	        {
                bind.second.UnbindAll();
	        }
            m_bindings.clear();
            m_impulseStates.clear();
            m_impulseResetAfterRead.clear();
            m_curKeys.clear();
            m_isBinded = false;
            if (!m_bSuppressEvent)
            {
                m3d::Application::g_pApp->EnqueueMessage(46, -1, 0, 0, 0, {}, {});
            }
            M3D_LOG_INFO("Key bindings: unbind all is done");
        }
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
