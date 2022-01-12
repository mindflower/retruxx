#include "core/console/console.h"
#include "console_internal.h"
#include <stdexcept>
#include <core/ini.h>
#include <core/kernel.h>
#include <core/log.h>
#include <core/ref_ptr.h>
#include <core/console/cvar.h>

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

    IConHandler::IConHandler(IConHandler const&)
    {
        throw std::logic_error("Not implemented");
    }

    IConHandler::IConHandler()
    {
    }

    RT_CLASS_DEFINE(IConsole);

    IConsole::IConsole(IConsole const&)
    {
        throw std::logic_error("Not implemented");
    }

    IConsole::IConsole()
    {
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

ConsoleImp::auxConsoleCmd::auxConsoleCmd(char const* rname, int rid, IConHandler* rhandler) :
    name(rname),
    id(rid),
    handler(rhandler)
{
}


void ConsoleImp::Render()
{
    throw std::logic_error("Not implemented");
}

int ConsoleImp::InputLine(CStr const&)
{
    throw std::logic_error("Not implemented");
}

ConsoleImp::ConsoleImp()
{
}

ConsoleImp::ConsoleImp(ConsoleImp const&)
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

void ConsoleImp::RegisterCommand(char const* cmdname, int id, m3d::IConHandler* handler)
{
    m_lCmds.emplace_back(cmdname, id, handler);
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

int ConsoleImp::Load(CStr const& fname)
{
    CStr err;
    if (ref_ptr xmlFile = m3d::ReadXmlFile(fname.c_str(), &err))
    {
        ref_ptr node = xmlFile->CreateNode(m3d::cmn::XML_NODE_EMPTY, nullptr);
        xmlFile->GetFirstChild_(node, "config");
        if (node->IsEmpty())
        {
            M3D_LOG_INFO("Config::cannot find 'config' node");
            return 0;
        }
        
        ref_ptr attrib = node->CreateAttribute();
        for (node->GetFirstAttribute(attrib); !attrib->IsEmpty(); attrib->GetNextSibling_(attrib))
        {
            CVarLoadedValue val;
            val.m_name = attrib->GetName();
            val.m_stringValue = attrib->GetValue();

            //TODO: check this
            auto itVars = std::find_if(m_lCVars.begin(), m_lCVars.end(), [&val](const auto* elem)
            {
                return elem->GetName() == val.m_name;
            });
            if (itVars != m_lCVars.end())
            {
                (*itVars)->Set(val.m_stringValue.c_str(), true);
            }

            auto itLoaded = std::find_if(m_loadedValues.begin(), m_loadedValues.end(), [&val](const auto& elem)
            {
                return elem.m_name == val.m_name;
            });
            if (itLoaded != m_loadedValues.end())
            {
                *itLoaded = val;
            }
            else
            {
                m_loadedValues.push_back(val);
            }
        }
        return 1;
    }
    M3D_LOG_INFO("Console:: cannot load " + fname);
    return 0;
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

void ConsoleImp::RegisterCVar(m3d::CVar* var, IConHandler* handler)
{
    //TODO: check correctness
    if (handler)
    {
        var->SetHandler(handler);
    }
    
    for (auto& value : m_loadedValues)
    {
        if (value.m_name == var->GetName())
        {
            var->Set(value.m_stringValue.c_str(), true);
            break;
        }
    }
    m_lCVars.push_back(var);
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
