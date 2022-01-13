#include "core/console/console.h"
#include "console_internal.h"
#include <stdexcept>
#include <core/ini.h>
#include <core/kernel.h>
#include <core/log.h>
#include <core/ref_ptr.h>
#include <core/console/cvar.h>

namespace
{
    m3d::CConsoleCommands conCommands[] = {
        {"conClear" , 0},
        {"conDump" , 1},
        {"conCVarList" , 2},
        {"conCmdList" , 3},
        {"conDebug" , 4},
        {"conExec" , 5},
        {"conScript" , 6},
    };
}

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

void ConsoleImp::Init(int width, int height)
{
    RegisterConsoleCommands();
    m_con.width = width;
    m_con.text[0] = 0;
    m_con.current = 0;
    m_con.x = 0;
    m_con.display = 0;
    m_con.totallines = 0;
    m_con.vislines = 0;
    m_con.linewidth = -1;
    m_con.height = height;
    m_FontSizeX = 10;
    m_FontSizeY = 14;
    m_screensize = 0.5;
    CheckResize(width, height);
    m_csCurState = CONSOLE_CLOSED;

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

void ConsoleImp::CheckResize(int newWidth, int newHeight)
{
    m_con.width = newWidth;
    m_con.height = newHeight;
    auto newLineWidth = newWidth / m_FontSizeX - 2;
    if (newLineWidth != m_con.linewidth)
    {
        if (newLineWidth >=1)
        {
            auto lineWidth = m_con.linewidth;
            auto totalLines = m_con.totallines;
            m_con.linewidth = newLineWidth;
            m_con.totallines = sizeof(m_con.text) / m_con.linewidth;
            //TODO: console text reorder
            //if(m_con.totallines < totalLines)
            //{
            //    totalLines = m_con.totallines;
            //}
            //if (m_con.linewidth < lineWidth)
            //{
            //    lineWidth = m_con.linewidth;
            //}
            //char buffer[sizeof(m_con.text)] = { 0 };
            //memcpy(buffer, m_con.text, sizeof(m_con.text));
            //memset(m_con.text, 0x20, sizeof(m_con.text));
        }
        else
        {
            m_con.linewidth = 38;
            m_con.totallines = 1724;
            memset(m_con.text, 0x20, sizeof(m_con.text));
        }
        m_con.current = m_con.totallines - 1;
        m_con.display = m_con.totallines - 1;
    }
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
    for (auto const& command : conCommands)
    {
        RegisterCommand(command.m_name, command.m_id, this);
    }
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
