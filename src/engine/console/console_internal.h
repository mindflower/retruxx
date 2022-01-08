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

    enum eConsoleState
    {
        CONSOLE_ERROR = 0x0,
        CONSOLE_DOWN = 0x1,
        CONSOLE_UP = 0x2,
        CONSOLE_CLOSED = 0x3,
        CONSOLE_OPENED = 0x4,
    };

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

    class auxConsoleCmd
    {
    public:
        auxConsoleCmd(char const*, int, IConHandler*);
        auxConsoleCmd(auxConsoleCmd const&);
        ~auxConsoleCmd();

    private:
        int id;
        char* name;
        IConHandler* handler;
    };

    class CVarLoadedValue
    {
    public:
        CStr m_name;
        CStr m_stringValue;
    };

public:
    virtual void Render();
    virtual int InputLine(CStr const &);
    virtual ~ConsoleImp();
    virtual Object * Clone();
    virtual void UnregisterCVar(m3d::CVar *);
    virtual int HandleEvent(m3d::Event const &);
    virtual int Save(CStr const &);
    virtual bool isActive() const ;
    virtual void RegisterCommand(char const *,int,m3d::IConHandler *);
    static m3d::Object * CreateObject();
    virtual void executeCommand(CStr const &);
    static m3d::Class * GetBaseClass();
    virtual void ScrollDown(int);
    virtual void Init(int,int);
    virtual int Load(CStr const &);
    virtual void SetScreenSize(float);
    virtual void PrintF(CStr const &);
    virtual void ScrollUp(int);
    virtual void CheckResize(int,int);
    virtual void RegisterCVar(m3d::CVar *,IConHandler *);
    virtual void NotifyMode(bool);
    virtual void ForceRender();
    virtual void setNumNotifyLines(int);
    virtual bool DumpToFile(char const *) const ;
    virtual m3d::Class * GetClass() const ;
    void ProcessInputChar(unsigned short);
    virtual bool Toggle(bool);
    virtual bool Toggle();
    virtual void Clear();

protected:
    ConsoleImp() = default;

private:
    void RegisterConsoleCommands();
    void executeCmdFile(char const *);
    void executeScriptCommand(CStr const &);
    void Linefeed();
    void DrawInput();
    void CompleteInput();
    CStr getFormatedScriptErrorDesc(m3d::eScriptError) const ;
    void RenderNotify();
    void CompleteScriptCommand();
    virtual void HandleCommand(int,m3d::CConsoleParams const &);
    virtual bool HandleCVar(m3d::CVar const *,m3d::CConsoleParams const &);
    void Print(char const *);

private:
    eConsoleState m_csCurState = eConsoleState::CONSOLE_ERROR;
    auxConsole m_con;
    int m_FontSizeX;
    int m_FontSizeY;
    float m_screensize;
    bool m_bDrawNotify = false;
    int m_nNumNotify = 5;
    char m_key_lines[32][256] = {0};
    int m_key_linepos = 0;
    int m_line_len = 0;
    int m_edit_line = 0;
    int m_history_line = 0;
    std::list<auxConsoleCmd> m_lCmds;
    std::list<m3d::CVar *> m_lCVars;
    std::vector<CVarLoadedValue> m_loadedValues;
};
