#pragma once
#include <core/console/console.h>
#include <script/scriptserver.h>
#include <list>

class ConsoleImp :
    public m3d::IConsole,
    public m3d::IConHandler
{
public:
    friend m3d::IConsole* m3d::ConsoleFactory();

protected:
    ConsoleImp();
    ConsoleImp(ConsoleImp const&);

public:
    virtual ~ConsoleImp();
    virtual Object* Clone();
    static m3d::Object* CreateObject();
    static m3d::Class* GetBaseClass();
    virtual m3d::Class* GetClass() const;
    RT_CLASS_DECLARE(ConsoleImp);
    virtual void RegisterCommand(char const*, int, m3d::IConHandler*);
    virtual void RegisterCVar(m3d::CVar*, IConHandler*);
    virtual void UnregisterCVar(m3d::CVar*);
    virtual int HandleEvent(m3d::Event const&);
    virtual void Init(int, int);
    virtual void Clear();
    virtual void CheckResize(int, int);
    virtual void PrintF(CStr const&);
    virtual void ForceRender();
    virtual void Render();
    virtual void NotifyMode(bool);
    virtual void setNumNotifyLines(int);
    virtual bool DumpToFile(char const*) const;
    virtual void ScrollUp(int);
    virtual void ScrollDown(int);
    virtual bool isActive() const;
    virtual bool Toggle(bool);
    virtual bool Toggle();
    void ProcessInputChar(unsigned short);
    virtual void SetScreenSize(float);
    virtual int InputLine(CStr const&);
    virtual int Load(CStr const&);
    virtual int Save(CStr const&);
    virtual void executeCommand(CStr const&);

    enum eConsoleState
    {
        CONSOLE_ERROR = 0x0,
        CONSOLE_DOWN = 0x1,
        CONSOLE_UP = 0x2,
        CONSOLE_CLOSED = 0x3,
        CONSOLE_OPENED = 0x4,
    };

private:
    eConsoleState m_csCurState = eConsoleState::CONSOLE_ERROR;

public:
    struct auxConsole
    {
        char text[65536];
        int current;
        int x;
        int display;
        int linewidth;
        int totallines;
        int vislines;
        int width;
        int height;
    };

private:
    auxConsole m_con;

public:
    class auxConsoleCmd
    {
    public:
        auxConsoleCmd(char const*, int, IConHandler*);

    private:
        int id;
        CStr name;
        IConHandler* handler;
    };

private:
    int m_FontSizeX;
    int m_FontSizeY;
    float m_screensize;
    bool m_bDrawNotify = false;
    int m_nNumNotify = 5;

private:
    void Linefeed();
    void Print(char const*);
    void DrawInput();

private:
    char m_key_lines[32][256] = { 0 };
    int m_key_linepos = 0;
    int m_line_len = 0;
    int m_edit_line = 0;
    int m_history_line = 0;

public:
    using CmdList = std::list<auxConsoleCmd>;;
    using CVarList = std::list<m3d::CVar*>;

private:
    std::list<auxConsoleCmd> m_lCmds;
    std::list<m3d::CVar*> m_lCVars;

private:
    void PrintConsoleCommandChoices();
    void CompleteInput();
    void CompleteScriptCommand();
    void RenderNotify();
    virtual void HandleCommand(int, m3d::CConsoleParams const&);
    virtual bool HandleCVar(m3d::CVar const*, m3d::CConsoleParams const&);
    void executeCmdFile(char const *);
    void executeScriptCommand(CStr const &);
    CStr getFormatedScriptErrorDesc(m3d::eScriptError) const ;

public:
    class CVarLoadedValue
    {
    public:
        CStr m_name;
        CStr m_stringValue;
    };

private:
    std::vector<CVarLoadedValue> m_loadedValues;

private:
    void RegisterConsoleCommands();
};
