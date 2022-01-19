#pragma once
#include "stringm3d.h"
#include <set>

#define RT_CLASS_LOCAL(cl) &cl::m_class##cl
#define RT_CLASS_DECLARE(cl) static m3d::Class m_class##cl
#define RT_CLASS_DEFINE(cl) m3d::Class cl::m_class##cl {#cl, sizeof(cl), CreateObject, GetBaseClass}
#define RT_CLASS_INLINE_DECLARE(cl) static inline m3d::Class m_class##cl {#cl}

namespace m3d
{
    namespace cmn
    {
        class XmlNode;
        class XmlFile;
    }

    class ExportInfo;
    class Object;

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
}
