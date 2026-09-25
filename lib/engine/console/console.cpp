#include "core/console/console.h"
#include "console_internal.h"
#include <stdexcept>
#include <core/ini.h>
#include <core/kernel.h>
#include <core/log.h>
#include <core/ref_ptr.h>
#include <core/console/cvar.h>
#include "i_event.h"

#include "m3dapp.h"

#include <algorithm>
#include <string>
#include <vector>
#include <config.h>
#include <core/scoped_ptr.h>
#include <core/timer.h>
#include <file/fileserver.h>
#include <file/filestream.h>
#include <script/funcstack.h>

namespace
{
    m3d::CConsoleCommands conCommands[] = {
        {"conClear", 0},
        {"conDump", 1},
        {"conCVarList", 2},
        {"conCmdList", 3},
        {"conDebug", 4},
        {"conExec", 5},
        {"conScript", 6},
    };

    // RVA 0x94F770 - prints up to len characters (all of s if len is 0 or too big) on a grid of
    // FontSizeX-wide cells starting at (x, y), each character centred in its cell. Spaces and
    // control characters only advance the cell.
    void Gfx_PrintFixed(float x, float y, int FontSizeX, char const* s, unsigned int dwColor, unsigned int len)
    {
        float const cellWidth = static_cast<float>(FontSizeX);
        float cellCenter = cellWidth * 0.5f + x;
        if (!len || len > strlen(s))
        {
            len = static_cast<unsigned int>(strlen(s));
        }
        for (unsigned int i = 0; i < len; ++i)
        {
            if (s[i] > 32)
            {
                char const ch[2] = {s[i], 0};
                PointBase<float> size;
                m3d::Application::g_pApp->GetTextExtent(
                    CStr(ch), size, -1, nullptr, nullptr, nullptr, nullptr, nullptr);
                m3d::Application::g_pApp->DrawTextAbs(cellCenter - size.x * 0.5f, y, dwColor, CStr(ch), 0, -1);
            }
            cellCenter = cellWidth + cellCenter;
        }
    }

    // RVA 0x94F920 - one character centred in a FontSizeX-wide cell at (x, y).
    void Gfx_PrintCharFixed(float x, float y, int FontSizeX, char ch, unsigned int dwColor)
    {
        float const cellCenter = static_cast<float>(FontSizeX) * 0.5f + x;
        char const str[2] = {ch, 0};
        PointBase<float> size;
        m3d::Application::g_pApp->GetTextExtent(CStr(str), size, -1, nullptr, nullptr, nullptr, nullptr, nullptr);
        m3d::Application::g_pApp->DrawTextAbs(cellCenter - size.x * 0.5f, y, dwColor, CStr(str), 0, -1);
    }
}  // namespace

RT_CLASS_EXPORT_METHOD_DEFINE(IConsole, Clear)
{
    // RVA 0x8D84B0
    auto console = static_cast<m3d::IConsole*>(context->asObject(0, "IConsole"));
    console->Clear();
    return 1;
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
    // RVA 0x8D8E70
    auto console = static_cast<m3d::IConsole*>(context->asObject(0, "IConsole"));
    CStr const line(context->asString(1));
    context->pushInt(console->InputLine(line));
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(IConsole, executeCommand)
{
    auto console = dynamic_cast<m3d::IConsole*>(context->asObject(0, "IConsole"));
    auto cmd = context->asString(1);
    console->executeCommand(cmd);
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(IConsole, Toggle)
{
    // RVA 0x8D84D0
    auto console = static_cast<m3d::IConsole*>(context->asObject(0, "IConsole"));
    context->pushBool(console->Toggle(context->asBool(1)));
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(IConsole, SetScreenSize)
{
    // RVA 0x8D8510
    auto console = static_cast<m3d::IConsole*>(context->asObject(0, "IConsole"));
    console->SetScreenSize(context->asFloat(1));
    return 1;
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
        int v3;          // edi
        char* v4;        // edx
        char const* v5;  // ecx
        char v6;         // al

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

    CConsoleParams& CConsoleParams::operator=(char const* istr)
    {
        // RVA 0x694290
        Set(istr);
        return *this;
    }

    int CConsoleParams::IntToken(int num, char delim) const
    {
        // RVA 0x694150 - NOTE: StringToken never returns null, so the -1 fallback is dead.
        char const* token = StringToken(num, szParmBuffer, 1024, delim);
        return token ? atoi(token) : -1;
    }

    CConsoleParams::CConsoleParams(CConsoleParams const& parms)
    {
        // RVA 0x6942D0
        length = 0;
        numTokens = 0;
        Set(parms.string);
    }

    CConsoleParams::CConsoleParams(char const* buf)
    {
        Set(buf);
    }

    CConsoleParams::CConsoleParams(int len)
    {
        // RVA 0x6941C0 - an empty buffer of len characters.
        length = len;
        string = new char[length];
        memset(string, 0, length);
        numTokens = 0;
    }

    int CConsoleParams::NumOfTokens(char delim) const
    {
        //TODO: check this and refactor
        int result;  // eax
        char* v3;    // esi
        char v4;     // dl
        char v5;     // bl
        int v6;      // edi
        char i;      // al

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
        char* v5;      // edx
        char v6;       // cl
        char* v7;      // esi
        char v8;       // al
        int v9;        // edi
        char v10;      // bl
        char* result;  // eax
        char i;        // cl

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

    float CConsoleParams::FloatToken(int num, char delim) const
    {
        // RVA 0x694180 - NOTE: as in IntToken, the -1 fallback is dead.
        char const* token = StringToken(num, szParmBuffer, 1024, delim);
        return token ? static_cast<float>(atof(token)) : -1.0f;
    }

    IConsole::IConsole(IConsole const&)
    {
        // Implicit in the shipped build: the interface has no state of its own.
    }

    IConsole::IConsole()
    {
    }

    Class* IConsole::GetRtClass() const
    {
        // RVA 0x8D8560
        return RT_CLASS_LOCAL(IConsole);
    }

    Class* IConsole::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Object);
    }

    IConsole* ConsoleFactory()
    {
        return new ConsoleImp;
    }
}  // namespace m3d

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
    // RVA 0x94FC70 - the open console: a translucent panel over the top m_screensize of the
    // screen with the build string in its corner, the scroll-back text (with a row of '^' when not
    // scrolled to the bottom) and the input line. A closed console may still show notify lines.
    if (!m_csCurState)
    {
        return;
    }
    if (m_csCurState == CONSOLE_CLOSED)
    {
        if (m_bDrawNotify)
        {
            RenderNotify();
        }
        return;
    }

    float const screenSize = M3D_ENGINE_CFG.m_consoleScreenSize.GetF();
    if (m_screensize != screenSize)
    {
        SetScreenSize(screenSize);
    }
    int lines = static_cast<int>(static_cast<float>(m_con.height) * m_screensize);
    if (lines <= 0)
    {
        return;
    }
    if (lines > m_con.height)
    {
        lines = m_con.height;
    }

    auto* app = m3d::Application::g_pApp;
    auto* renderer = M3D_RENDERER;
    renderer->PushZbState(m3d::rend::ZB_DISABLE);
    renderer->PushFog(false);
    renderer->SetStageState(0, m3d::rend::BM_COLOR, m3d::rend::TS_TEXTURE);
    renderer->SetStageState(0, m3d::rend::BM_ALPHA, m3d::rend::TS_DIFFUSE);
    renderer->SetAlphaTest(0);
    app->SetFont(CStr("Lucida Console"), 9.0f, 1, app->m_codePage.CodePage);
    renderer->PushBlend(m3d::rend::BM_ALPHA);
    renderer->SetWhiteTexture(0);
    float const panelHeight = static_cast<float>(lines);
    app->PutSprite2Abs(
        0.0f, 0.0f, 0.0f, 1.0f - m_screensize, static_cast<float>(m_con.width), panelHeight, 1.0f, 1.0f, 0xA0000000);
    app->PutSprite2Abs(
        0.0f, panelHeight - 2.0f, 0.0f, 0.0f, static_cast<float>(m_con.width), panelHeight, 1.0f, 1.0f, 0x70FFFFFF);
    renderer->PopBlend();

    CStr const version("retruxx - release version build v0.01");
    PointBase<float> size;
    app->GetTextExtent(version, size, -1, nullptr, nullptr, nullptr, nullptr, nullptr);
    Gfx_PrintFixed(
        static_cast<float>(static_cast<int>(m_con.width - version.length() * m_FontSizeX)),
        static_cast<float>(lines - static_cast<int>(size.y) - 3),
        m_FontSizeX,
        version.c_str(),
        0xFFA8D6FD,
        0);

    m_con.vislines = lines;
    int y = lines - 3 * m_FontSizeY;
    int rows = lines / m_FontSizeY - 2;
    if (m_con.display != m_con.current)
    {
        // Scrolled back: mark the gap below.
        for (int i = 0; i < m_con.linewidth; i += 4)
        {
            Gfx_PrintCharFixed(
                static_cast<float>(i + 1) * static_cast<float>(m_FontSizeX),
                static_cast<float>(y + 4),
                m_FontSizeX,
                '^',
                0xFFFFFFFF);
        }
        y -= m_FontSizeY;
        --rows;
    }
    int row = m_con.display;
    for (int i = 0; i < rows; ++i, --row)
    {
        if (row < 0 || m_con.current - row >= m_con.totallines)
        {
            break;
        }
        Gfx_PrintFixed(
            static_cast<float>(m_FontSizeX),
            static_cast<float>(y),
            m_FontSizeX,
            &m_con.text[m_con.linewidth * (row % m_con.totallines)],
            0xFFA0A0A0,
            m_con.linewidth);
        y -= m_FontSizeY;
    }
    DrawInput();
    renderer->PopZbState();
    renderer->PopFog();
}

int ConsoleImp::InputLine(CStr const& command)
{
    // RVA 0x8DD320 - types a whole line as if at the keyboard: Enter, the characters, Enter.
    ProcessInputChar(4);
    for (int i = 0; i < static_cast<int>(command.length()); ++i)
    {
        ProcessInputChar(static_cast<unsigned char>(command.c_str()[i]));
    }
    ProcessInputChar(4);
    return 1;
}

ConsoleImp::ConsoleImp()
{
}

ConsoleImp::ConsoleImp(ConsoleImp const&)
{
    // RVA 0x8DBED0 - NOTE: only the containers are set up; the state and buffers are not copied.
}

ConsoleImp::~ConsoleImp()
{
    // RVA 0x8DBF40 - the containers clean up after themselves.
}

m3d::Object* ConsoleImp::Clone()
{
    // RVA 0x8DD380
    return new ConsoleImp(*this);
}

void ConsoleImp::UnregisterCVar(m3d::CVar* var)
{
    // RVA 0x8DA3E0
    auto const it = std::find(m_lCVars.begin(), m_lCVars.end(), var);
    if (it != m_lCVars.end())
    {
        m_lCVars.erase(it);
    }
}

int ConsoleImp::HandleEvent(m3d::Event const& ev)
{
    int result = 0;
    switch (ev.m_eventType)
    {
    case m3d::EV_LOOSING_FOCUS:
        Toggle(false);
        result = 1;
        break;

    case m3d::EV_OBTAINED_FOCUS:
        Toggle(true);
        result = 1;
        break;

    case m3d::EV_KEY_DOWN:
        ProcessInputChar(ev.m_ushortEv[0]);
        result = 1;
        break;

    case m3d::EV_KEY_UP:
        result = 1;
        break;

    case m3d::EV_MOUSE_WHEEL:
        if (ev.m_shortEv[2] <= 0)
        {
            ScrollDown(-ev.m_shortEv[2]);
        }
        else
        {
            ScrollUp(ev.m_shortEv[2]);
        }
        result = 1;
        break;

    default:
        break;
    }
    return result;
}

int ConsoleImp::Save(CStr const& name)
{
    // RVA 0x94E780 - every cvar as an attribute of a <config> element.
    // NOTE: two further nodes ("test" and a text node) are created and never used, and the result
    // is always 1, even when the file could not be written.
    ref_ptr xmlFile = M3D_KERNEL->CreateXmlFile();
    ref_ptr config = xmlFile->CreateNode(m3d::cmn::XML_NODE_ELEMENT, "config");
    ref_ptr test = xmlFile->CreateNode(m3d::cmn::XML_NODE_ELEMENT, "test");
    ref_ptr test1 = xmlFile->CreateNode(m3d::cmn::XML_NODE_TEXT, nullptr);
    for (m3d::CVar* var : m_lCVars)
    {
        config->SetAttribute(var->GetName(), var->GetS());
    }
    xmlFile->AddChild(config);
    CStr err;
    if (!m3d::WriteXmlFile(name.c_str(), xmlFile, &err))
    {
        M3D_LOG_INFO("Console:: cannot save " + name + " err: " + err);
    }
    return 1;
}

bool ConsoleImp::isActive() const
{
    return m_csCurState && m_csCurState != CONSOLE_CLOSED;
}

void ConsoleImp::RegisterCommand(char const* cmdname, int id, m3d::IConHandler* handler)
{
    m_lCmds.emplace_back(cmdname, id, handler);
}

m3d::Object* ConsoleImp::CreateObject()
{
    // RVA 0x8DD3B0
    return new ConsoleImp();
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

void ConsoleImp::ScrollDown(int lines)
{
    // RVA 0x8D8880
    if (m_csCurState)
    {
        m_con.display += lines;
        if (m_con.display > m_con.current)
        {
            m_con.display = m_con.current;
        }
    }
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
            auto itVars = std::find_if(
                m_lCVars.begin(),
                m_lCVars.end(),
                [&val](auto const* elem)
                {
                    return elem->GetName() == val.m_name;
                });
            if (itVars != m_lCVars.end())
            {
                (*itVars)->Set(val.m_stringValue.c_str(), true);
            }

            auto itLoaded = std::find_if(
                m_loadedValues.begin(),
                m_loadedValues.end(),
                [&val](auto const& elem)
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

void ConsoleImp::SetScreenSize(float size)
{
    // RVA 0x8D9340 - between a fifth of the screen and all of it.
    m_screensize = size;
    if (m_screensize < 0.2f)
    {
        m_screensize = 0.2f;
    }
    else if (m_screensize > 1.0f)
    {
        m_screensize = 1.0f;
    }
}

void ConsoleImp::PrintF(CStr const& s)
{
    if (m_csCurState)
        Print(s.c_str());
}

void ConsoleImp::ScrollUp(int lines)
{
    // RVA 0x8D8860 - NOTE: not clamped; Render stops at the oldest line itself.
    if (m_csCurState)
    {
        m_con.display -= lines;
    }
}

void ConsoleImp::CheckResize(int newWidth, int newHeight)
{
    // RVA 0x8D86C0 - a new line width re-flows the ring buffer: the newest lines are copied, as
    // many as still fit and each cut to the new width, to the end of the new layout.
    m_con.width = newWidth;
    m_con.height = newHeight;
    int const oldLineWidth = m_con.linewidth;
    int const newLineWidth = newWidth / m_FontSizeX - 2;
    if (newLineWidth == oldLineWidth)
    {
        return;
    }
    if (newLineWidth >= 1)
    {
        int const oldTotalLines = m_con.totallines;
        m_con.linewidth = newLineWidth;
        m_con.totallines = static_cast<int>(sizeof(m_con.text)) / newLineWidth;
        int const numLines = (std::min)(m_con.totallines, oldTotalLines);
        int const numChars = (std::min)(newLineWidth, oldLineWidth);
        std::vector<char> old(m_con.text, m_con.text + sizeof(m_con.text));
        memset(m_con.text, ' ', sizeof(m_con.text));
        for (int i = 0; i < numLines; ++i)
        {
            for (int j = 0; j < numChars; ++j)
            {
                m_con.text[j + m_con.linewidth * (m_con.totallines - i - 1)] =
                    old[j + oldLineWidth * ((m_con.current + oldTotalLines - i) % oldTotalLines)];
            }
        }
    }
    else
    {
        m_con.linewidth = 38;
        m_con.totallines = 1724;
        memset(m_con.text, ' ', sizeof(m_con.text));
    }
    m_con.current = m_con.totallines - 1;
    m_con.display = m_con.totallines - 1;
}

void ConsoleImp::RegisterCVar(m3d::CVar* var, IConHandler* handler)
{
    // RVA 0x8DBB00 - the list is kept sorted by name, and a value already loaded from the config
    // file is applied at once.
    if (handler)
    {
        var->SetHandler(handler);
    }
    auto it = m_lCVars.begin();
    while (it != m_lCVars.end() && strcmp((*it)->GetName(), var->GetName()) <= 0)
    {
        ++it;
    }
    for (auto const& value : m_loadedValues)
    {
        if (value.m_name == var->GetName())
        {
            var->Set(value.m_stringValue.c_str(), true);
            break;
        }
    }
    m_lCVars.insert(it, var);
}

void ConsoleImp::NotifyMode(bool on)
{
    // RVA 0x8DBE70
    m_bDrawNotify = on;
}

void ConsoleImp::ForceRender()
{
    // RVA 0x94F700 - a frame of just the console, outside the normal loop.
    auto* renderer = M3D_RENDERER;
    if (renderer->CanRender() && renderer->BeginScene())
    {
        Render();
        renderer->EndScene();
        renderer->PresentScene();
    }
}

void ConsoleImp::setNumNotifyLines(int num)
{
    // RVA 0x8DBE80
    m_nNumNotify = num;
}

bool ConsoleImp::DumpToFile(char const* fileName) const
{
    // RVA 0x94E490 - the scroll-back as plain text, from the first non-blank line, with trailing
    // spaces trimmed and the top bit of every character cleared.
    if (m_csCurState == CONSOLE_ERROR)
    {
        return false;
    }
    scoped_ptr stream = M3D_KERNEL->GetFileServer().CreateFileStream();
    if (!stream->Open(fileName, m3d::fs::IStream::OPEN_WRITE))
    {
        return false;
    }
    int line = m_con.current - m_con.totallines + 1;
    for (; line <= m_con.current; ++line)
    {
        int i = 0;
        for (; i < m_con.linewidth; ++i)
        {
            if (m_con.text[m_con.linewidth * (line % m_con.totallines) + i] != ' ')
            {
                break;
            }
        }
        if (i != m_con.linewidth)
        {
            break;
        }
    }
    char buffer[1024];
    buffer[m_con.linewidth] = 0;
    for (; line <= m_con.current; ++line)
    {
        strncpy(buffer, &m_con.text[m_con.linewidth * (line % m_con.totallines)], m_con.linewidth);
        for (int j = m_con.linewidth - 1; j >= 0 && buffer[j] == ' '; --j)
        {
            buffer[j] = 0;
        }
        for (char* p = buffer; *p; ++p)
        {
            *p &= ~0x80;
        }
        *stream << buffer << "\n";
    }
    stream->Close();
    return true;
}

m3d::Class* ConsoleImp::GetClass() const
{
    return RT_CLASS_LOCAL(ConsoleImp);
}

void ConsoleImp::ProcessInputChar(unsigned short ch)
{
    // RVA 0x8DC860 - the console's line editor. ch is a character or one of the engine's key
    // codes; the high bits carry Ctrl (0x4000/0x8000) and Shift/Alt (0x1000/0x2000).
    static char const DELIMITERS[] = " ,.()\t|+-=/*";
    char* line = m_key_lines[m_edit_line];
    auto setLine = [this](CStr const& text)
    {
        strcpy(m_key_lines[m_edit_line], text.c_str());
    };
    bool const ctrl = (ch & 0x8000) != 0 || (ch & 0x4000) != 0;

    if (ch == 27)
    {
        // History: the previous non-empty line.
        while (true)
        {
            m_history_line = (m_history_line - 1) & 0x1F;
            if (m_history_line == m_edit_line)
            {
                m_history_line = (m_edit_line + 1) & 0x1F;
                break;
            }
            if (m_key_lines[m_history_line][0])
            {
                break;
            }
        }
        strcpy(line, m_key_lines[m_history_line]);
        m_key_linepos = m_line_len = static_cast<int>(strlen(line));
        return;
    }
    if (ch == 28)
    {
        // History: the next non-empty line, or an empty one past the newest.
        if (m_history_line == m_edit_line)
        {
            return;
        }
        do
        {
            m_history_line = (m_history_line + 1) & 0x1F;
            if (m_history_line == m_edit_line)
            {
                // NOTE: the line itself is left as it was.
                m_key_linepos = 0;
                return;
            }
        } while (!m_key_lines[m_history_line][0]);
        strcpy(line, m_key_lines[m_history_line]);
        m_key_linepos = m_line_len = static_cast<int>(strlen(line));
        return;
    }
    if ((ctrl && static_cast<unsigned char>(ch) == 'c') || (static_cast<unsigned char>(ch) == 22 && ctrl))
    {
        // Ctrl+C, Ctrl+Insert.
        M3D_KERNEL->SetClipboardData(line);
        return;
    }
    if ((ctrl && static_cast<unsigned char>(ch) == 'v') || (static_cast<unsigned char>(ch) == 22 && (ch & 0x3000) != 0))
    {
        // Ctrl+V, Shift+Insert: the clipboard up to its first line break, as much as fits.
        CStr cbd = M3D_KERNEL->GetClipboardData();
        if (cbd.empty())
        {
            return;
        }
        int count = static_cast<int>(cbd.length());
        int const lineEnd = cbd.findOneOf("\n\r\b", 0);
        if (lineEnd != -1)
        {
            // NOTE: the line break itself is pasted too.
            count = lineEnd + 1;
        }
        if (m_line_len + count > 256)
        {
            count = 256 - m_line_len;
            cbd = cbd.substr(0, 256 - m_line_len);
        }
        if (count > 0)
        {
            CStr str = CStr(line).substr(0, m_key_linepos);
            str += cbd;
            str += CStr(line).substr(m_key_linepos, CStr_npos);
            setLine(str);
            m_key_linepos += count;
            m_line_len += count;
        }
        return;
    }
    if (ch == 3)
    {
        CompleteInput();
        return;
    }
    if (ch == 0x8003)
    {
        // Ctrl+Tab: complete a /command, or a script name otherwise.
        CStr const str(line);
        if (!str.empty())
        {
            if (str.c_str()[0] != '\\' && str.c_str()[0] != '/')
            {
                CompleteScriptCommand();
                return;
            }
            CompleteInput();
        }
        return;
    }
    if (ch == 1)
    {
        line[0] = 0;
        m_key_linepos = 0;
        m_line_len = 0;
        return;
    }
    if (ch == 4)
    {
        // Enter: echo the line, move to a fresh one and run it.
        CStr const cmd(line);
        PrintF(cmd + CStr("\n"));
        m_edit_line = (m_edit_line + 1) & 0x1F;
        memset(m_key_lines[m_edit_line], 0, sizeof(m_key_lines[m_edit_line]));
        m_history_line = m_edit_line;
        m_key_linepos = 0;
        m_line_len = 0;
        if (!cmd.empty())
        {
            if (cmd.c_str()[0] == '\\' || cmd.c_str()[0] == '/')
            {
                executeCommand(cmd);
            }
            else
            {
                executeScriptCommand(cmd);
            }
        }
        return;
    }

    switch (ch)
    {
    case 2:
        // Backspace.
        if (m_line_len > 0 && m_key_linepos > 0)
        {
            CStr str = CStr(line).substr(0, m_key_linepos - 1);
            str += CStr(line).substr(m_key_linepos, CStr_npos);
            setLine(str);
            --m_key_linepos;
            --m_line_len;
        }
        return;
    case 0x1F:
        // Delete.
        if (m_line_len > 0 && m_key_linepos < m_line_len)
        {
            CStr str = CStr(line).substr(0, m_key_linepos);
            str += CStr(line).substr(m_key_linepos + 1, CStr_npos);
            setLine(str);
            --m_line_len;
        }
        return;
    case 0x19:
        if (m_key_linepos > 0)
        {
            --m_key_linepos;
        }
        return;
    case 0x1A:
        if (m_key_linepos < m_line_len)
        {
            ++m_key_linepos;
        }
        return;
    case 0x17:
        m_key_linepos = 0;
        return;
    case 0x18:
        m_key_linepos = m_line_len;
        return;
    case 0x8019:
    case 0x4019:
    {
        // Ctrl+Left: to the start of the word.
        if (!m_key_linepos)
        {
            return;
        }
        std::string const inputStr = std::string(line).substr(0, m_key_linepos - 1);
        size_t const pos = inputStr.find_last_of(DELIMITERS);
        m_key_linepos = pos == std::string::npos ? 0 : static_cast<int>(pos) + 1;
        return;
    }
    case 0x801A:
    {
        // Ctrl+Right: past the end of the word.
        if (m_key_linepos == m_line_len)
        {
            return;
        }
        std::string const inputStr = std::string(line).substr(m_key_linepos);
        size_t const pos = inputStr.find_first_of(DELIMITERS);
        if (pos == std::string::npos)
        {
            m_key_linepos = m_line_len;
        }
        else
        {
            m_key_linepos += static_cast<int>(pos) + 1;
        }
        return;
    }
    default:
        break;
    }

    unsigned short key = ch;
    if ((ch & 0x3000) != 0)
    {
        key = static_cast<unsigned char>(ch);
    }
    if (key == 29)
    {
        ScrollUp(2);
    }
    else if (key == 30)
    {
        ScrollDown(2);
    }
    else if (key >= 0x20 && key <= 0x7F && m_key_linepos < 256)
    {
        // A printable character is inserted at the cursor.
        CStr str;
        if (m_line_len > 0)
        {
            str += CStr(line).substr(0, m_key_linepos);
        }
        char const typed[2] = {static_cast<char>(key), 0};
        str += CStr(typed);
        if (line[0])
        {
            str += CStr(line).substr(m_key_linepos, CStr_npos);
        }
        setLine(str);
        ++m_key_linepos;
        ++m_line_len;
    }
}

bool ConsoleImp::Toggle(bool bOpen)
{
    if (m_csCurState == CONSOLE_ERROR)
    {
        return false;
    }
    if (bOpen)
    {
        if (m_csCurState != CONSOLE_OPENED)
        {
            m_csCurState = CONSOLE_OPENED;
            return true;
        }
    }
    else if (m_csCurState != CONSOLE_CLOSED)
    {
        m_csCurState = CONSOLE_CLOSED;
    }
    return true;
}

bool ConsoleImp::Toggle()
{
    // RVA 0x8D8620 - closed opens, open closes.
    // NOTE: a console in any other state (down/up) is put into the error state.
    if (m_csCurState == CONSOLE_ERROR)
    {
        return false;
    }
    if (m_csCurState == CONSOLE_CLOSED)
    {
        m_csCurState = CONSOLE_OPENED;
    }
    else if (m_csCurState == CONSOLE_OPENED)
    {
        m_csCurState = CONSOLE_CLOSED;
    }
    else
    {
        m_csCurState = CONSOLE_ERROR;
    }
    return true;
}

void ConsoleImp::Clear()
{
    // RVA 0x8D86A0 - blanks the text; the line positions stay.
    memset(m_con.text, ' ', sizeof(m_con.text));
}

void ConsoleImp::RegisterConsoleCommands()
{
    for (auto const& command : conCommands)
    {
        RegisterCommand(command.m_name, command.m_id, this);
    }
}

void ConsoleImp::executeCmdFile(char const* fileName)
{
    // RVA 0x8DD400 - runs a file of console lines as if typed. Leading blanks are skipped and
    // lines starting with '#' are comments.
    // NOTE: a blank line is still entered, which echoes an empty line.
    scoped_ptr stream = M3D_KERNEL->GetFileServer().CreateFileStream();
    if (!stream->Open(fileName, m3d::fs::IStream::OPEN_READ))
    {
        PrintF(CStr("Could not run file ") + CStr(fileName) + CStr("\n"));
        return;
    }
    CStr cmdLine;
    while (!stream->Eof() && stream->ReadLine(cmdLine))
    {
        char const* p = cmdLine.c_str();
        while (*p == ' ' || *p == '\t')
        {
            ++p;
        }
        if (*p == '#')
        {
            continue;
        }
        for (; *p; ++p)
        {
            ProcessInputChar(static_cast<unsigned char>(*p));
        }
        ProcessInputChar(4);
    }
    stream->Close();
}

void ConsoleImp::executeScriptCommand(CStr const& command)
{
    // RVA 0x8D9BB0 - a line that is not a /command is a script statement.
    m3d::eScriptError const err = M3D_KERNEL->GetScriptServer().execute(command.c_str(), "console_string");
    if (err)
    {
        PrintF(getFormatedScriptErrorDesc(err) + CStr("\n"));
    }
}

void ConsoleImp::Linefeed()
{
    // RVA 0x8D8800 - starts a new, blank line; the view follows if it was at the bottom.
    m_con.x = 0;
    if (m_con.display == m_con.current)
    {
        ++m_con.display;
    }
    ++m_con.current;
    memset(&m_con.text[m_con.linewidth * (m_con.current % m_con.totallines)], ' ', m_con.linewidth);
}

void ConsoleImp::DrawInput()
{
    // RVA 0x94FA30 - the line being edited, scrolled to keep the cursor in view, with a blinking
    // '_' cursor.
    char* line = m_key_lines[m_edit_line];
    for (int i = m_line_len + 1; i < m_con.linewidth; ++i)
    {
        line[i] = ' ';
    }
    if (m_key_linepos >= m_con.linewidth)
    {
        line += m_key_linepos - m_con.linewidth + 1;
    }
    float const y = static_cast<float>(m_con.vislines - 2 * m_FontSizeY);
    Gfx_PrintFixed(static_cast<float>(m_FontSizeX), y, m_FontSizeX, line, 0xFFE0E0E0, m_con.linewidth);
    if ((m3d::g_Kernel->GetTimer().GetCurTimeUnscaled() & 0x100) != 0)
    {
        Gfx_PrintCharFixed(
            static_cast<float>(m_key_linepos + 1) * static_cast<float>(m_FontSizeX), y, m_FontSizeX, '_', 0xFFE0E0E0);
    }
}

void ConsoleImp::CompleteInput()
{
    // RVA 0x8DC010 - completes the command or cvar name at the start of the line: extends it as
    // far as all the names that begin with it agree, and lists them when there are several.
    char const* text = m_key_lines[m_edit_line];
    if (text[0] == '\\' || text[0] == '/')
    {
        ++text;
    }
    m3d::CConsoleParams params(text);
    CStr partial(params.UnsafeStringToken(0, ' '));
    size_t const partialLen = partial.length();

    std::vector<CStr> matches;
    matches.reserve(100);
    for (auto const& cmd : m_lCmds)
    {
        if (!strncmp(partial.c_str(), cmd.name.c_str(), partialLen))
        {
            matches.push_back(cmd.name);
        }
    }
    for (m3d::CVar* var : m_lCVars)
    {
        if (!strncmp(partial.c_str(), var->GetName(), partialLen))
        {
            matches.push_back(CStr(var->GetName()));
        }
    }
    std::sort(
        matches.begin(),
        matches.end(),
        [](CStr const& a, CStr const& b)
        {
            return strcmp(a.c_str(), b.c_str()) < 0;
        });

    unsigned int const listSize = static_cast<unsigned int>(matches.size());
    if (listSize)
    {
        int const minLen = static_cast<int>(matches.front().length());
        for (int prefix = static_cast<int>(partialLen) + 1; prefix <= minLen; ++prefix)
        {
            CStr const newPartial = matches.front().substr(0, prefix);
            bool agree = true;
            for (unsigned int i = 1; i < listSize; ++i)
            {
                if (!(matches[i].substr(0, prefix) == newPartial))
                {
                    agree = false;
                    break;
                }
            }
            if (!agree)
            {
                break;
            }
            partial = newPartial;
        }
        if (listSize > 1)
        {
            PrintF(partial + CStr("\n"));
            for (CStr const& match : matches)
            {
                PrintF(CStr("    ") + match + CStr("\n"));
            }
        }
    }

    // The line becomes "/<name>" plus the rest of what was typed.
    char* line = m_key_lines[m_edit_line];
    line[0] = '/';
    strcpy(line + 1, partial.c_str());
    m_line_len = static_cast<int>(partial.length()) + 1;
    if (partial.length())
    {
        for (int j = 1; j < params.NumOfTokens(' '); ++j)
        {
            line[m_line_len++] = ' ';
            char const* token = params.UnsafeStringToken(j, ' ');
            strcpy(&line[m_line_len], token);
            m_line_len += static_cast<int>(strlen(token));
        }
        if (listSize == 1)
        {
            line[m_line_len] = ' ';
            m_key_linepos = ++m_line_len;
        }
        line[m_line_len] = 0;
    }
    // NOTE: the cursor goes back to the end of the name, even after a unique match's space.
    m_key_linepos = static_cast<int>(partial.length()) + 1;
}

CStr ConsoleImp::getFormatedScriptErrorDesc(m3d::eScriptError err) const
{
    // RVA 0x8D99E0 - "(source @ line): error\ndescription", or nothing for no error.
    CStr desc;
    if (err)
    {
        auto& scriptServer = M3D_KERNEL->GetScriptServer();
        char const* errorDesc = scriptServer.getErrorDescString(err);
        m3d::auxScriptErrorDesc const& last = scriptServer.getLastErrorDesc();
        desc = CStr("(");
        desc += last.sourceString;
        desc += CStr(" @ ");
        desc += CStr(last.lineNumber);
        desc += CStr("): ");
        desc += CStr(errorDesc);
        desc += CStr("\n");
        desc += last.descriptionString;
    }
    return desc;
}

void ConsoleImp::RenderNotify()
{
    // RVA 0x94FB10 - the last m_nNumNotify lines, over the game, while the console is closed.
    int lines = static_cast<int>(static_cast<float>(m_con.height) * m_screensize);
    if (lines > m_FontSizeY * m_nNumNotify)
    {
        lines = m_FontSizeY * m_nNumNotify;
    }
    if (lines <= 0)
    {
        return;
    }
    if (lines > m_con.height)
    {
        lines = m_con.height;
    }
    M3D_RENDERER->PushZbState(m3d::rend::ZB_DISABLE);
    M3D_RENDERER->PushFog(false);
    m3d::Application::g_pApp->SetFont(CStr("Impact"), 9.0f, 0, m3d::Application::g_pApp->m_codePage.CodePage);
    int const rows = lines / m_FontSizeY;
    int display = m_con.display;
    m_con.vislines = lines;
    for (int i = 0; i < rows; ++i)
    {
        if (display < 0 || m_con.current - display >= m_con.totallines)
        {
            break;
        }
        Gfx_PrintFixed(
            3.0f,
            static_cast<float>(lines),
            m_FontSizeX,
            &m_con.text[m_con.linewidth * (display % m_con.totallines)],
            0xFFA0A0A0,
            m_con.linewidth);
        lines -= m_FontSizeY;
        --display;
    }
    M3D_RENDERER->PopZbState();
    M3D_RENDERER->PopFog();
}

void ConsoleImp::CompleteScriptCommand()
{
    // RVA 0x8D8F40 - completes the script identifier around the cursor by asking the script
    // function _GET_MOST_APPROPRIATE_GLOBAL_NAME.
    static char const DELIMITERS[] = " ,.()\t|+-=/*";
    if (!m_line_len)
    {
        return;
    }
    int pos = m_key_linepos;
    if (pos == m_line_len)
    {
        --pos;
    }
    else if (!pos)
    {
        pos = 1;
    }
    std::string const text(m_key_lines[m_edit_line]);
    int const split = pos + 1;
    std::string const cmdPart1 = text.substr(0, split);
    std::string cmdPart2;
    if (split < m_line_len)
    {
        cmdPart2 = text.substr(split);
    }
    std::string token;
    std::string strBegin;
    std::string strEnd;
    size_t lastOf = cmdPart1.find_last_of(DELIMITERS);
    size_t const firstOf = cmdPart2.find_first_of(DELIMITERS);
    if (lastOf == std::string::npos)
    {
        token = cmdPart1;
    }
    else
    {
        // NOTE: a delimiter at the very start stays part of the token.
        if (lastOf)
        {
            ++lastOf;
        }
        token = cmdPart1.substr(lastOf);
        strBegin = cmdPart1.substr(0, lastOf);
    }
    if (firstOf == std::string::npos)
    {
        token += cmdPart2;
    }
    else
    {
        if (firstOf)
        {
            token += cmdPart2.substr(0, firstOf);
        }
        strEnd = cmdPart2.substr(firstOf);
    }

    m3d::sArgStack stack;
    stack.m_InArgs[stack.m_numInArgs++].SetS(token.c_str());
    if (!M3D_KERNEL->GetScriptServer().callScriptFunc("_GET_MOST_APPROPRIATE_GLOBAL_NAME", stack, 1) &&
        stack.m_numOutArgs == 1)
    {
        m3d::sArg* out = stack.popOut();
        if (out->m_type == m3d::sArg::ARGTYPE_STRING)
        {
            char const* name = out->GetS();
            if (!(std::string(name) == token))
            {
                std::string const fullStr = strBegin + name + strEnd;
                strcpy(m_key_lines[m_edit_line], fullStr.c_str());
                m_key_linepos = static_cast<int>(strBegin.size() + strlen(name));
                m_line_len = static_cast<int>(fullStr.size());
            }
        }
    }
}

void ConsoleImp::HandleCommand(int cmdId, m3d::CConsoleParams const& params)
{
    // RVA 0x94DE50 - the console's own commands (see conCommands).
    switch (cmdId)
    {
    case 0:
        // conClear
        Clear();
        break;
    case 1:
        // conDump <file_name>
        if (params.NumOfTokens(' ') == 2)
        {
            char const* file = params.UnsafeStringToken(1, ' ');
            if (DumpToFile(file))
            {
                PrintF(CStr("Dumped console text to file ") + CStr(file) + CStr("\n"));
            }
            else
            {
                PrintF(CStr("Failed write file ") + CStr(file) + CStr("\n"));
            }
        }
        else
        {
            PrintF(CStr("Usage: /conDump <file_name>\n"));
        }
        break;
    case 2:
        // conCVarList
        for (m3d::CVar* var : m_lCVars)
        {
            PrintF(CStr(var->GetName()) + CStr("\n"));
        }
        break;
    case 3:
        // conCmdList
        for (auto const& cmd : m_lCmds)
        {
            PrintF(cmd.name + CStr("\n"));
        }
        break;
    case 4:
        // conDebug on|off - the notify lines over the game.
        if (params.NumOfTokens(' ') == 2)
        {
            char const* arg = params.UnsafeStringToken(1, ' ');
            if (!_stricmp(arg, "on"))
            {
                m_bDrawNotify = true;
            }
            else if (!_stricmp(arg, "off"))
            {
                m_bDrawNotify = false;
            }
            else
            {
                PrintF(CStr("Usage: /conDebug on|off\n"));
            }
        }
        else
        {
            PrintF(CStr("Usage: /conDebug on|off\n"));
        }
        break;
    case 5:
        // conExec <filename>
        if (params.NumOfTokens(' ') == 2)
        {
            executeCmdFile(params.UnsafeStringToken(1, ' '));
        }
        else
        {
            PrintF(CStr("Usage: /conExec <filename>\n"));
        }
        break;
    case 6:
        // conScript <filename>
        if (params.NumOfTokens(' ') == 2)
        {
            if (auto res = m3d::g_Kernel->GetScriptServer().executeScriptFile(params.UnsafeStringToken(1, ' ')))
            {
                PrintF(getFormatedScriptErrorDesc(res) + CStr("\n"));
            }
        }
        else
        {
            PrintF(CStr("Usage: /conScript <filename>\n"));
        }
        break;
    default:
        break;
    }
}

bool ConsoleImp::HandleCVar(m3d::CVar const*, m3d::CConsoleParams const&)
{
    // RVA 0x8DBEB0 - the console handles no cvars of its own.
    return false;
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
            while (1)
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
        char const* v5;  // ecx
        char v6;         // bl
        char const* v7;  // ebp
        int v8;          // ecx
        int v9;          // eax
        int v10;         // eax
        int v11;         // edx
        int v12;         // eax
        CStr v13;        // [esp+Ch] [ebp-24h] BYREF

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
                        this->m_con.text
                            [this->m_con.x + this->m_con.linewidth * (this->m_con.current % this->m_con.totallines)] =
                            v6;
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
