#include "core/console/console.h"
#include "console_internal.h"
#include <stdexcept>

namespace m3d
{
    void CConsoleParams::Set(char const*)
    {
        throw std::logic_error("Not implemented");
    }

    char const* CConsoleParams::UnsafeStringToken(int, char) const
    {
        throw std::logic_error("Not implemented");
    }

    CConsoleParams::~CConsoleParams()
    {
        throw std::logic_error("Not implemented");
    }

    CConsoleParams& CConsoleParams::operator=(char const*)
    {
        throw std::logic_error("Not implemented");
    }

    int CConsoleParams::IntToken(int, char) const
    {
        throw std::logic_error("Not implemented");
    }

    CConsoleParams::CConsoleParams(CConsoleParams const&)
    {
        throw std::logic_error("Not implemented");
    }

    CConsoleParams::CConsoleParams(char const*)
    {
        throw std::logic_error("Not implemented");
    }

    CConsoleParams::CConsoleParams(int)
    {
        throw std::logic_error("Not implemented");
    }

    int CConsoleParams::NumOfTokens(char) const
    {
        throw std::logic_error("Not implemented");
    }

    char* CConsoleParams::StringToken(int, char*, int, char) const
    {
        throw std::logic_error("Not implemented");
    }

    float CConsoleParams::FloatToken(int, char) const
    {
        throw std::logic_error("Not implemented");
    }

    Class* IConsole::GetRtClass() const
    {
        throw std::logic_error("Not implemented");
    }

    Class* IConsole::GetBaseClass()
    {
        throw std::logic_error("Not implemented");
    }

    IConsole* ConsoleFactory()
    {
        return new ConsoleImp;
    }
}

ConsoleImp::auxConsoleCmd::auxConsoleCmd(char const*, int, IConHandler*)
{
    throw std::logic_error("Not implemented");
}

ConsoleImp::auxConsoleCmd::auxConsoleCmd(auxConsoleCmd const&)
{
    throw std::logic_error("Not implemented");
}

ConsoleImp::auxConsoleCmd::~auxConsoleCmd()
{
    throw std::logic_error("Not implemented");
}

void ConsoleImp::Render()
{
    throw std::logic_error("Not implemented");
}

int ConsoleImp::InputLine(CStr const&)
{
    throw std::logic_error("Not implemented");
}

ConsoleImp::~ConsoleImp()
{
    throw std::logic_error("Not implemented");
}

m3d::Object* ConsoleImp::Clone()
{
    throw std::logic_error("Not implemented");
}

void ConsoleImp::UnregisterCVar(m3d::CVar*)
{
    throw std::logic_error("Not implemented");
}

int ConsoleImp::HandleEvent(m3d::Event const&)
{
    throw std::logic_error("Not implemented");
}

int ConsoleImp::Save(CStr const&)
{
    throw std::logic_error("Not implemented");
}

bool ConsoleImp::isActive() const
{
    throw std::logic_error("Not implemented");
}

void ConsoleImp::RegisterCommand(char const*, int, m3d::IConHandler*)
{
    throw std::logic_error("Not implemented");
}

m3d::Object* ConsoleImp::CreateObject()
{
    throw std::logic_error("Not implemented");
}

void ConsoleImp::executeCommand(CStr const&)
{
    throw std::logic_error("Not implemented");
}

m3d::Class* ConsoleImp::GetBaseClass()
{
    throw std::logic_error("Not implemented");
}

void ConsoleImp::ScrollDown(int)
{
    throw std::logic_error("Not implemented");
}

void ConsoleImp::Init(int, int)
{
    throw std::logic_error("Not implemented");
}

int ConsoleImp::Load(CStr const&)
{
    throw std::logic_error("Not implemented");
}

void ConsoleImp::SetScreenSize(float)
{
    throw std::logic_error("Not implemented");
}

void ConsoleImp::PrintF(CStr const&)
{
    throw std::logic_error("Not implemented");
}

void ConsoleImp::ScrollUp(int)
{
    throw std::logic_error("Not implemented");
}

void ConsoleImp::CheckResize(int, int)
{
    throw std::logic_error("Not implemented");
}

void ConsoleImp::RegisterCVar(m3d::CVar*, IConHandler*)
{
    throw std::logic_error("Not implemented");
}

void ConsoleImp::NotifyMode(bool)
{
    throw std::logic_error("Not implemented");
}

void ConsoleImp::ForceRender()
{
    throw std::logic_error("Not implemented");
}

void ConsoleImp::setNumNotifyLines(int)
{
    throw std::logic_error("Not implemented");
}

bool ConsoleImp::DumpToFile(char const*) const
{
    throw std::logic_error("Not implemented");
}

m3d::Class* ConsoleImp::GetClass() const
{
    throw std::logic_error("Not implemented");
}

void ConsoleImp::ProcessInputChar(unsigned short)
{
    throw std::logic_error("Not implemented");
}

bool ConsoleImp::Toggle(bool)
{
    throw std::logic_error("Not implemented");
}

bool ConsoleImp::Toggle()
{
    throw std::logic_error("Not implemented");
}

void ConsoleImp::Clear()
{
    throw std::logic_error("Not implemented");
}

void ConsoleImp::RegisterConsoleCommands()
{
    throw std::logic_error("Not implemented");
}

void ConsoleImp::executeCmdFile(char const*)
{
    throw std::logic_error("Not implemented");
}

void ConsoleImp::executeScriptCommand(CStr const&)
{
    throw std::logic_error("Not implemented");
}

void ConsoleImp::Linefeed()
{
    throw std::logic_error("Not implemented");
}

void ConsoleImp::DrawInput()
{
    throw std::logic_error("Not implemented");
}

void ConsoleImp::CompleteInput()
{
    throw std::logic_error("Not implemented");
}

CStr ConsoleImp::getFormatedScriptErrorDesc(m3d::eScriptError) const
{
    throw std::logic_error("Not implemented");
}

void ConsoleImp::RenderNotify()
{
    throw std::logic_error("Not implemented");
}

void ConsoleImp::CompleteScriptCommand()
{
    throw std::logic_error("Not implemented");
}

void ConsoleImp::HandleCommand(int, m3d::CConsoleParams const&)
{
    throw std::logic_error("Not implemented");
}

bool ConsoleImp::HandleCVar(m3d::CVar const*, m3d::CConsoleParams const&)
{
    throw std::logic_error("Not implemented");
}

void ConsoleImp::Print(char const*)
{
    throw std::logic_error("Not implemented");
}
