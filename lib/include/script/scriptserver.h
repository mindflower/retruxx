#pragma once
#include "funcstack.h"
extern "C"
{
#include "lua/lua.h"
}
#include <core/clazz.h>
#include <map>

enum ext_InternalTags
{
	tag_Unknown = 0x0,
	tag_luaAIParam = 0x3E8,
	tag_luaVector = 0x3E9,
	tag_instance = 0x3EA,
	tag_luaQuaternion = 0x3EB,
};

ext_InternalTags ext_getTag(lua_State*, int);
bool ext_checkTag(lua_State*, int, ext_InternalTags);

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
		friend class ScriptServer;
	public:
		static inline ScriptServer* g_scriptServer = nullptr;

	public:
		~Scriptlet()
		{
			if (m_data)
			{
				delete[] m_data;
				m_data = nullptr;
			}
		}

		eScriptError compile();
		eScriptError loadFromFile(char const* nameAs);
		eScriptError execute(char const*, bool);

	private:
		Scriptlet() : m_compiledDataLen(0), m_dataLen(0)
		{ }

		bool m_bCompiled = false;
		unsigned int m_dataLen;
		char* m_data = nullptr;
		unsigned __int8* m_compiledData = nullptr;
		unsigned int m_compiledDataLen;
		bool m_bLoaded = false;
	};

	class auxScriptErrorDesc
	{
	public:
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
		public:
			CStr returnValue;
			CStr params;
			CStr shortDesc;
		};

	public:
		static Class* __fastcall GetBaseClass();
        static Object* __fastcall CreateObject();
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
		retruxx::map<CStr, auxFuncDesc> const& getRegisteredFunctionsDesc() const;
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
		retruxx::map<CStr, Scriptlet*> m_scripts;
		retruxx::map<CStr, auxFuncDesc> m_funcDescs;
		CStr m_lastScriptExecuted;
		bool m_bInitialized = false;
	};
}
