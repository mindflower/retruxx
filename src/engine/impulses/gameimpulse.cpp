#include <stdexcept>
#include <impulses/i_impulses.h>
#include "gameimpulse.h"
#include <core/log.h>

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

    int GameImpulse::LoadFromFile(CStr const&)
    {
        throw std::logic_error("Not implemented");
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
        throw std::logic_error("Not implemented");
    }

    int GameImpulse::LoadFromDefaults()
    {
        throw std::logic_error("Not implemented");
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
