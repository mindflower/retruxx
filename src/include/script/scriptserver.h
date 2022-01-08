#pragma once
#include "funcstack.h"
extern "C"
{
#include "lua/lua.h"
}
#include <core/clazz.h>
#include <map>

namespace m3d
{
    class ScriptServer;

    enum eScriptError
    {
        SUCCESS = 0x0,
        NOT_INITIALIZED = 0x1,
        FILE_NOT_FOUND = 0x2,
        RUNTIME_ERROR = 0x3,
        SYNTAX_ERROR = 0x4,
        MEMORY_ERROR = 0x5,
        OTHER_ERROR = 0x6,
        NO_SUCH_FUNCTION = 0x7,
        ALREADY_REGISTERED = 0x8,
    };

    class Scriptlet
    {
    public:
        static inline ScriptServer* g_scriptServer = nullptr;

    public:
        eScriptError compile();
        ~Scriptlet();
        eScriptError loadFromFile(char const*);
        eScriptError execute(char const*, bool);

    private:
        Scriptlet();
        bool m_bCompiled;
        unsigned int m_dataLen;
        char* m_data;
        unsigned __int8* m_compiledData;
        unsigned int m_compiledDataLen;
        bool m_bLoaded;
    };

    class auxScriptErrorDesc
    {
    private:
        CStr descriptionString;
        CStr sourceString;
        int lineNumber;
        int defLineNumber;
        CStr whatString;
        CStr nameString;
        CStr nameWhatString;
    };

    class ScriptServer : public Object
    {
    public:
        class auxFuncDesc
        {
        private:
            CStr returnValue;
            CStr params;
            CStr shortDesc;
        };

    public:
        static Class* GetBaseClass();
        static Object* CreateObject();
        static int _getScriptObject(Object*);

    public:
        static Class m_classScriptServer;
        static inline lua_State* L = nullptr;
        static inline int m_metatable_ClassMethod = 0;
        static inline int m_metatable_ClassNativeMethod = 0;

    public:
        eScriptError reloadScript(char const*);
        eScriptError callScriptFunc(char const*, sArgStack&, int);
        eScriptError done();
        char const* getNameOfLastScript() const;
        std::map<CStr, auxFuncDesc> const& getRegisteredFunctionsDesc() const;
        eScriptError addScript(char const*);
        eScriptError execute(char const*, char const*);
        eScriptError executeBuffer(void*, unsigned int, char const*);
        virtual ~ScriptServer();
        lua_State* getGlobalEnvironment();
        eScriptError reloadAllScripts();
        CStr getFormatedScriptErrorDesc(eScriptError) const;
        auxScriptErrorDesc const& getLastErrorDesc() const;
        virtual Class* GetClass() const;
        char const* getErrorDescString(eScriptError) const;
        void dumpStack();
        eScriptError executeScriptFile(char const*);
        eScriptError init();
        virtual Object* Clone();
        eScriptError registerGlobalFunction(int(*)(sArgStack&), char const* = "", char const* = "", char const* = "", char const* = "");

    protected:
        ScriptServer() = default;

    private:
        std::map<CStr, Scriptlet*> m_scripts;
        std::map<CStr, auxFuncDesc> m_funcDescs;
        CStr m_lastScriptExecuted;
        bool m_bInitialized = false;
    };
}
