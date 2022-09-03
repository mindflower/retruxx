#pragma once
#include "stringm3d.h"
#include <set>

#define RT_CLASS_LOCAL(cl) &cl::m_class##cl
#define RT_CLASS_DECLARE(cl) static m3d::Class m_class##cl
#define RT_CLASS_DEFINE(cl) m3d::Class cl::m_class##cl {#cl, sizeof(cl), CreateObject, GetBaseClass, -1, _exports_##cl}
//#define RT_CLASS_INLINE_DECLARE(cl) static inline m3d::Class m_class##cl {#cl}

#define RT_CLASS_EXPORTS_BEGIN(cl) m3d::ExportInfo _exports_##cl[] {
#define RT_CLASS_EXPORTS_END m3d::ExportInfo{}}
#define RT_CLASS_EXPORT(cl, type, funcName, retVal, params, desc) m3d::ExportInfo{#funcName, type, _export_##cl##_##funcName, nullptr, retVal, params, desc},
#define RT_CLASS_EXPORT_METHOD_DEFINE(cl, funcName) int _export_##cl##_##funcName##(m3d::Context* context)

namespace m3d
{
    namespace cmn
    {
        class XmlNode;
        class XmlFile;
    }

    struct ExportInfo;
    class Object;
    class AIParam;

    class Context
    {
    public:
        virtual ~Context() = default;
        virtual void pushObject(Object*) = 0;
        virtual void pushAIParam(AIParam const&) = 0;
        virtual int asInt(int) = 0;
        virtual bool asBool(int) = 0;
        virtual Object* asObject(int, char const*) = 0;
        virtual int countArgs() = 0;
        virtual float asFloat(int) = 0;
        virtual AIParam& asAIParam(int) = 0;
        virtual void pushBool(bool) = 0;
        virtual char const* asString(int) = 0;
        virtual void pushQuaternion(Quaternion const&) = 0;
        virtual void pushInt(int) = 0;
        virtual void pushFloat(float) = 0;
        virtual Quaternion& asQuaternion(int) = 0;
        virtual void pushString(char const*) = 0;
        virtual CVector& asVector(int) = 0;
        virtual void pushVector(CVector const&) = 0;
    };

    //IMPORTANT: fields and members order is strict
    struct Class
    {
        const char* m_className = nullptr;
        int m_classSize =0 ;
        Object* (* m_fnCreateObject)() = nullptr;
        Class* (* m_fnGetBaseClass)() = nullptr;
        int m_index = 0;
        ExportInfo* m_lExports = nullptr;
        void* m_scriptHandle = nullptr;

    public:
        bool IsKindOf(char const*) const;
        bool IsKindOf(Class const*) const;
        Object* NewInstance() const;
    };

    //IMPORTANT: fields and members order is strict
    class RefCountedBase
    {
    public:
        RefCountedBase();
        virtual ~RefCountedBase() = default;
        int IncRef();
        int DecRef();
        int GetRefCount() const;

    private:
        int m_refCount = 0;
    };

    //IMPORTANT: fields and members order is strict c
    class Object : public RefCountedBase
    {
    public:
        virtual Object* Clone();
        virtual int ReadFromXmlNode(cmn::XmlFile*, cmn::XmlNode*);
        virtual int ReadFromXmlNodeAfterAdd(cmn::XmlFile*, cmn::XmlNode*);
        virtual int WriteToXmlNode(cmn::XmlFile*, cmn::XmlNode*);
        void SetPersistance(bool);
        bool GetPersistance() const;
        virtual int SetProperty(unsigned int propId, void* prop);
        virtual int GetProperty(unsigned int propId, void* prop) const;
        virtual int GetPropertiesList(std::set<size_t>&) const;
        Object* GetParent() const;
        Object* GetFirstChild_() const;
        Object* GetLastChild_() const;
        Object* GetNextSibling_() const;
        Object* GetPrevSibling_() const;
        int GetNumChildren() const;
        Object* GetChildByName(CStr const&) const;
        bool IsDirectChild(Object const*) const;
        bool IsChildOf(Object const*) const;
        virtual int AddChild(Object*);
        int LinkChildAtHead(Object*);
        int LinkChildAtTail(Object*);
        int UnlinkChild(Object*);
        virtual int RemoveChild(Object*);
        int RemoveAllChildren();
        void MoveChildToFirstPosition(Object*);
        void MoveChildToLastPosition(Object*);
        char const* GetName() const;
        void SetName(CStr const&);
        void SetChildDirty(bool);
        bool GetChildDirty() const;

    protected:
        CStr m_name;
        bool m_persistant = true;
        bool m_isChildDirty = false;

    private:
        Object* m_parent = nullptr;
        Object* m_firstChild = nullptr;
        Object* m_lastChild = nullptr;
        Object* m_nextSibling = nullptr;
        Object* m_prevSibling = nullptr;
        int m_numChildren = 0;

    protected:
        Object* ChildNodeFromXmlNode(cmn::XmlFile*, cmn::XmlNode*);

    public:
        Object* ChildNodeFromXmlFile(char const*);
        virtual int IncWeakRef();
        virtual int DecWeakRef();
        virtual int GetWeakRefCount();

    public:
        void* m_scriptHandle = nullptr;
        RT_CLASS_DECLARE(Object);

    public:
        virtual Class* GetClass() const;
        virtual char const* GetClassNameA() const;
        bool IsKindOf(char const*) const;
        bool IsKindOf(Class const*) const;
        static Class* GetBaseClass();
        static Object* CreateObject();

    protected:
        Object(Object const&);
        Object();

    public:
        virtual ~Object() = default;
    };

    enum eExportType
    {
        METHOD = 0x0,
        NATIVE_METHOD = 0x1,
    };

    struct ExportInfo
    {
        const char* name = nullptr;
        eExportType type = METHOD;
        void* addr1 = nullptr;
        void* addr2 = nullptr;
        const char* returns = nullptr;
        const char* params = nullptr;
        const char* desc = nullptr;
    };

}
