#include "core/console/console.h"
#include "console_internal.h"
#include <stdexcept>
#include <core/ini.h>
#include <core/kernel.h>
#include <core/log.h>
#include <core/ref_ptr.h>
#include <core/console/cvar.h>

#include "m3dapp.h"

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

    void Gfx_PrintFixed(float,float,int,char const *,unsigned int,unsigned int)
    {
        throw std::logic_error("Not implemented");
    }
}

RT_CLASS_EXPORT_METHOD_DEFINE(IConsole, Clear)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(IConsole, PrintF)
{
    auto console = dynamic_cast<m3d::IConsole*>(context->asObject(0, "IConsole"));
    auto msg = context->asString(1);
    console->PrintF(msg);
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(IConsole, InputLine)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(IConsole, executeCommand)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(IConsole, Toggle)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(IConsole, SetScreenSize)
{
    throw std::logic_error("Not implemented");
}

namespace m3d
{
    RT_CLASS_EXPORTS_BEGIN(IConsole)
        RT_CLASS_EXPORT(IConsole, METHOD, Clear, "", "", "")
        RT_CLASS_EXPORT(IConsole, METHOD, PrintF, "", "", "")
        RT_CLASS_EXPORT(IConsole, METHOD, InputLine, "", "", "")
        RT_CLASS_EXPORT(IConsole, METHOD, executeCommand, "", "", "")
        RT_CLASS_EXPORT(IConsole, METHOD, Toggle, "", "", "")
        RT_CLASS_EXPORT(IConsole, METHOD, SetScreenSize, "", "", "")
	RT_CLASS_EXPORTS_END;

    RT_CLASS_DEFINE(IConsole);

    void CConsoleParams::Set(char const* buf)
    {
        //TODO: check this and refactor
        int v3; // edi
        char* v4; // edx
        const char* v5; // ecx
        char v6; // al

        v3 = strlen(buf) + 1;
        if (v3 > this->length)
        {
            delete[] string;
            this->length = v3;
            this->string = new char[length];
        }
        v4 = this->string;
        v5 = buf;
        do
        {
            v6 = *v5;
            *v4++ = *v5++;
        } while (v6);
        this->numTokens = 0;
    }

    char const* CConsoleParams::UnsafeStringToken(int num, char delim) const
    {
        return StringToken(num, szParmBuffer, 1024, delim);
    }

    CConsoleParams::~CConsoleParams()
    {
        delete[] string;
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

    CConsoleParams::CConsoleParams(char const* buf)
    {
        Set(buf);
    }

    CConsoleParams::CConsoleParams(int)
    {
        throw std::logic_error("Not implemented");
    }

    int CConsoleParams::NumOfTokens(char delim) const
    {
        //TODO: check this and refactor
        int result; // eax
        char* v3; // esi
        char v4; // dl
        char v5; // bl
        int v6; // edi
        char i; // al

        result = this->numTokens;
        if (result)
            return result;
        v3 = this->string;
        v4 = *this->string;
        v5 = 1;
        v6 = 1;
        for (i = 1; v4; ++v3)
        {
            if (v4 == delim)
            {
                if (!i)
                    v5 = 0;
            }
            else
            {
                i = 0;
                if (!v5)
                {
                    ++v6;
                    v5 = 1;
                }
            }
            v4 = v3[1];
        }
        this->numTokens = v6;
        return v6;
    }

    char* CConsoleParams::StringToken(int num, char* outString, int stringlen, char delim) const
    {
        char* v5; // edx
        char v6; // cl
        char* v7; // esi
        char v8; // al
        int v9; // edi
        char v10; // bl
        char* result; // eax
        char i; // cl

        v5 = this->string;
        v6 = *this->string;
        v7 = outString;
        v8 = 0;
        v9 = 0;
        if (v6)
        {
            v10 = delim;
            while (1)
            {
                if (v6 == delim)
                {
                    v8 = 0;
                }
                else if (!v8)
                {
                    if (v9 == num)
                    {
                        if (*v5 == 34)
                        {
                            v10 = 34;
                            ++v5;
                        }
                        for (i = *v5; *v5; ++v7)
                        {
                            auto temp = reinterpret_cast<int>(&v7[1 - reinterpret_cast<int>(outString)]);
                            if (reinterpret_cast<int>(&v7[1 - reinterpret_cast<int>(outString)]) >= stringlen)
                                break;
                            if (i == v10)
                                break;
                            ++v5;
                            *v7 = i;
                            i = *v5;
                        }
                        break;
                    }
                    ++v9;
                    v8 = 1;
                }
                v6 = *++v5;
                if (!v6)
                {
                    result = outString;
                    *outString = 0;
                    return result;
                }
            }
        }
        result = outString;
        *v7 = 0;
        return result;
    }

    float CConsoleParams::FloatToken(int, char) const
    {
        throw std::logic_error("Not implemented");
    }

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
        return RT_CLASS_LOCAL(Object);
    }

    IConsole* ConsoleFactory()
    {
        return new ConsoleImp;
    }
}

RT_CLASS_EXPORTS_BEGIN(ConsoleImp)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(ConsoleImp);

ConsoleImp::auxConsoleCmd::auxConsoleCmd(char const* rname, int rid, IConHandler* rhandler) :
    name(rname),
    id(rid),
    handler(rhandler)
{
}


void ConsoleImp::Render()
{
    if (m_csCurState)
    {
        if (m_csCurState == CONSOLE_CLOSED)
        {
            if (m_bDrawNotify)
            {
                RenderNotify();
            }
        }
        else
        {
            throw std::logic_error("Not implemented");
        }
    }
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

void ConsoleImp::executeCommand(CStr const& command)
{
    //TODO: check this!!!!!!!!1
    m3d::CConsoleParams params(command.c_str() + 1);
    auto cmdName = params.UnsafeStringToken(0, 32);

    bool found = false;
    for (auto& cmd : m_lCmds)
    {
	    if (cmd.name == cmdName)
	    {
            found = true;
            cmd.handler->HandleCommand(cmd.id, params);
            break;
	    }
    }

    if (!found)
    {
        for (auto cvar : m_lCVars)
        {
            CStr name = cvar->GetName();
            if (name == cmdName)
            {
                found = true;
                if ((cvar->GetFlags() & 2) != 0)
                {
                    PrintF(name + " is a read-only variable\n");
                }
                else
                {
                    if (!cvar->GetHandler() || cvar->GetHandler()->HandleCVar(cvar, params))
                    {
                        auto numOfTokens = params.NumOfTokens(' ');
                        if (numOfTokens > 1)
                        {
                            auto token = params.UnsafeStringToken(1, ' ');
                            auto type = cvar->GetType();
                            if (type == m3d::CVar::CVAR_STRING || type == m3d::CVar::CVAR_COLOR)
                            {
                                CStr newstr = token;
                                for (int i = 2; i < numOfTokens; ++i)
                                {
                                    token = params.UnsafeStringToken(i, ' ');
                                    newstr += " ";
                                    newstr += token;
                                }
                                if ((cvar->GetFlags() & 2) == 0)
                                {
                                    cvar->Set(newstr.c_str(), true);
                                }
                            }
                            else if ((cvar->GetFlags() & 2) == 0)
                            {
                                cvar->Set(token, true);
                            }
                            PrintF(name + " changed to \"" + CStr(token) + "\"\n");
                        }
                    }
                }
                break;
            }
        }
    }
    if (!found)
    {
        PrintF("No command or variable found\n");
    }
}

m3d::Class* ConsoleImp::GetBaseClass()
{
    return RT_CLASS_LOCAL(IConsole);
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
        xmlFile->GetFirstChild(node, "config");
        if (node->IsEmpty())
        {
            M3D_LOG_INFO("Config::cannot find 'config' node");
            return 0;
        }
        
        ref_ptr attrib = node->CreateAttribute();
        for (node->GetFirstAttribute(attrib); !attrib->IsEmpty(); attrib->GetNextSibling(attrib))
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

void ConsoleImp::PrintF(CStr const& s)
{
    if (m_csCurState)
        Print(s.c_str());
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
    return RT_CLASS_LOCAL(ConsoleImp);
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
    auto v2 = this->m_FontSizeY * this->m_nNumNotify;
    auto v3 = this->m_con.height;
    auto v4 = (v3 * this->m_screensize);

    if (v4 > v2)
        v4 = v2;
    if (v4 > 0)
    {
        if (v4 > v3)
            v4 = v3;
        M3D_RENDERER->PushZbState(m3d::rend::ZB_DISABLE);
        M3D_RENDERER->PushFog(false);
        m3d::Application::g_pApp->SetFont("Impact", 9.0, 0, m3d::Application::g_pApp->m_codePage.CodePage);
        m_con.vislines = v4;
        auto v6 = m_con.display;
        for (int i = 0; i < v4 / m_FontSizeY; ++i)
        {
	        if (v6 < 0)
	        {
                break;
	        }
            auto v7 = m_con.totallines;
            if (m_con.current - v6 >= v7)
            {
                break;
            }
            //TODO: add Gfx_PrintFixed
            Gfx_PrintFixed(3.0, v4, m_FontSizeX, &m_con.text[this->m_con.linewidth * (v6 % v7)], 0xFFA0A0A0, m_con.linewidth);
            v4 -= m_FontSizeY;
            --v6;
        }
        M3D_RENDERER->PopZbState();
        M3D_RENDERER->PopFog();
    }
    throw std::logic_error("Not implemented");
}

void ConsoleImp::CompleteScriptCommand()
{
    throw std::logic_error("Not implemented");
}

void ConsoleImp::HandleCommand(int cmdId, m3d::CConsoleParams const& params)
{
    switch(cmdId)
    {
    case 0:
	    {
            Clear();
            break;
	    }
    case 1:
	    {
		    if (params.NumOfTokens(' ') == 2)
		    {
                auto file = params.UnsafeStringToken(1, ' ');
                if (DumpToFile(file))
                {
                    PrintF("Dumped console text to file " + CStr(file) + "\n");
                }
                else
                {
                    PrintF("Failed write file " + CStr(file) + "\n");
                }
		    }
            else
            {
                PrintF("Usage: /conDump <file_name>\n");
            }
            break;
	    }
    case 6:
	    {
		    if (params.NumOfTokens(' ') == 2)
		    {
                auto file = params.UnsafeStringToken(1, ' ');
                if (auto res = m3d::g_Kernel->GetScriptServer().executeScriptFile(file))
                {
                    PrintF(getFormatedScriptErrorDesc(res) + "\n");
                }
		    }
            else
            {
                PrintF("Usage: /conScript <filename>\n");
            }
            break;
	    }
    default:
	    {
			throw std::logic_error("Not implemented");
	    }
    }
}

bool ConsoleImp::HandleCVar(m3d::CVar const*, m3d::CConsoleParams const&)
{
    throw std::logic_error("Not implemented");
}

void ConsoleImp::Print(char const* txt)
{
    static int cr = 0;
    if (txt)
    {
        CStr str;
        if (txt[0] == '\t')
        {
            str = "    ";
        }
        auto text = new char[strlen(txt) + 1];
        strcpy(text, txt);
        auto token = strtok(text, "\t");
        if (token)
        {
	        while(1)
	        {
                str += token;
                token = strtok(nullptr, "\t");
                if (!token)
                {
                    break;
                }
                str += "    ";
	        }
        }

        delete[] text;

        //TODO: check this and refactor!!!
        const char* v5; // ecx
        char v6; // bl
        const char* v7; // ebp
        int v8; // ecx
        int v9; // eax
        int v10; // eax
        int v11; // edx
        int v12; // eax
        CStr v13; // [esp+Ch] [ebp-24h] BYREF

        v5 = str.c_str();
        v6 = *str.c_str();
        v7 = str.c_str();
        if (*str.c_str())
        {
            while (1)
            {
                v8 = this->m_con.linewidth;
                v9 = 0;
                if (v8 > 0)
                {
                    do
                    {
                        if (v7[v9] <= 32)
                            break;
                        ++v9;
                    } while (v9 < this->m_con.linewidth);
                }
                if (v9 != v8 && v9 + this->m_con.x > v8)
                    this->m_con.x = 0;
                ++v7;
                if (cr)
                {
                    --this->m_con.current;
                    cr = 0;
                }
                if (!this->m_con.x)
                {
                    v10 = this->m_con.current;
                    this->m_con.x = 0;
                    v11 = this->m_con.display;
                    if (v11 == v10)
                        this->m_con.display = v11 + 1;
                    v12 = v10 + 1;
                    this->m_con.current = v12;
                    memset(&this->m_con.text[v8 * (v12 % this->m_con.totallines)], 0x20u, v8);
                }
                if (v6 != 10)
                {
                    if (v6 != 13)
                    {
                        this->m_con.text[this->m_con.x + this->m_con.linewidth * (this->m_con.current % this->m_con.totallines)] = v6;
                        if (++this->m_con.x >= this->m_con.linewidth)
                            this->m_con.x = 0;
                        goto LABEL_32;
                    }
                    cr = 1;
                }
                this->m_con.x = 0;
            LABEL_32:
                v6 = *v7;
                if (!*v7)
                {
                    v5 = str.c_str();
                    break;
                }
            }
        }

    }
}
