#pragma once
#include "stringm3d.h"
#include "retruxx/common.h"

#define RT_CLASS_LOCAL(cl)   &cl::m_class##cl
#define RT_CLASS_DECLARE(cl) static m3d::Class m_class##cl
#define RT_CLASS_DEFINE(cl)                                            \
    m3d::Class cl::m_class##cl                                         \
    {                                                                  \
        #cl, sizeof(cl), CreateObject, GetBaseClass, -1, _exports_##cl \
    }
#define RT_CLASS_INLINE_DECLARE(cl)      \
    static inline m3d::Class m_class##cl \
    {                                    \
        #cl                              \
    }

#define RT_CLASS_EXPORTS_BEGIN(cl)  \
    m3d::ExportInfo _exports_##cl[] \
    {
#define RT_CLASS_EXPORTS_END \
    m3d::ExportInfo          \
    {                        \
    }                        \
    }
#define RT_CLASS_EXPORT(cl, type, funcName, retVal, params, desc) \
    m3d::ExportInfo{#funcName, type, _export_##cl##_##funcName, nullptr, retVal, params, desc},
#define RT_CLASS_EXPORT_METHOD_DEFINE(cl, funcName) int _export_##cl##_##funcName##(m3d::Context * context)

#define IS_KIND_OF(obj, className) ((obj) != nullptr && (obj)->IsKindOf(RT_CLASS_LOCAL(className)))
#define RT_DYNCAST(obj, className) (dynamic_cast<className*>(obj))

#undef GetFirstChild
#undef GetFirstSibling
#undef GetLastSibling
#undef GetNextSibling
#undef GetPrevSibling

namespace m3d
{
    namespace cmn
    {
        struct XmlNode;
        class XmlFile;
    }  // namespace cmn

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
        char const* m_className = nullptr;
        int m_classSize = 0;
        Object* (*m_fnCreateObject)() = nullptr;
        Class* (*m_fnGetBaseClass)() = nullptr;
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
        virtual m3d::Object* Clone();
        virtual int ReadFromXmlNode(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* node);
        virtual int ReadFromXmlNodeAfterAdd(m3d::cmn::XmlFile* file, m3d::cmn::XmlNode* node);
        virtual int WriteToXmlNode(m3d::cmn::XmlFile* file, m3d::cmn::XmlNode* writeTo);
        bool GetPersistance() const;
        void SetPersistance(bool per);
        virtual int SetProperty(unsigned int propId, void* prop);
        virtual int GetProperty(unsigned int propId, void* prop) const;
        virtual int GetPropertiesList(retruxx::set<unsigned int>& properties) const;
        m3d::Object* GetParent() const;
        m3d::Object* GetFirstChild() const;
        m3d::Object* GetLastChild() const;
        m3d::Object* GetNextSibling() const;
        m3d::Object* GetPrevSibling() const;
        int GetNumChildren() const;
        m3d::Object* GetChildByName(CStr const& str) const;
        bool IsDirectChild(m3d::Object const* w) const;
        bool IsChildOf(m3d::Object const* wnd) const;
        virtual int AddChild(m3d::Object* node);
        int LinkChildAtHead(m3d::Object* node);
        int LinkChildAtTail(m3d::Object* node);
        int UnlinkChild(m3d::Object* node);
        virtual int RemoveChild(m3d::Object* node);
        int RemoveAllChildren();
        void MoveChildToFirstPosition(m3d::Object* wnd);
        void MoveChildToLastPosition(m3d::Object* wnd);
        char const* GetName() const;
        void SetName(CStr const& str);
        void SetChildDirty(bool d);
        bool GetChildDirty() const;

    protected:
        CStr m_name;
        bool m_persistant = true;
        bool m_isChildDirty = false;

    private:
        m3d::Object* m_parent = nullptr;
        m3d::Object* m_firstChild = nullptr;
        m3d::Object* m_lastChild = nullptr;
        m3d::Object* m_nextSibling = nullptr;
        m3d::Object* m_prevSibling = nullptr;
        int m_numChildren = 0;

    protected:
        m3d::Object* ChildNodeFromXmlNode(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode);

    public:
        m3d::Object* ChildNodeFromXmlFile(char const* extStr);
        virtual int IncWeakRef();
        virtual int DecWeakRef();
        virtual int GetWeakRefCount();
        void* m_scriptHandle = nullptr;

        static m3d::Class m_classObject;
        virtual m3d::Class* GetClass() const;
        virtual char const* GetClassNameA() const;
        bool IsKindOf(char const* className) const;
        bool IsKindOf(m3d::Class const* object) const;
        static m3d::Class* GetBaseClass();
        static m3d::Object* CreateObject();

    protected:
        Object(m3d::Object const& clazz);
        Object();
    };

    enum eExportType
    {
        METHOD = 0x0,
        NATIVE_METHOD = 0x1,
    };

    struct ExportInfo
    {
        char const* name = nullptr;
        eExportType type = METHOD;
        void* addr1 = nullptr;
        void* addr2 = nullptr;
        char const* returns = nullptr;
        char const* params = nullptr;
        char const* desc = nullptr;
    };

}  // namespace m3d
