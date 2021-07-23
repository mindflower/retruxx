#pragma once
#include "stringm3d.h"
#include <set>

namespace m3d
{
    namespace cmn
    {
        class XmlNode;
        class XmlFile;
    }

    class ExportInfo;
    struct Class;

    class RefCountedBase
    {
    public:
        virtual ~RefCountedBase();

        RefCountedBase();
        int IncRef();
        int DecRef();
        int GetRefCount();

    private:
        int m_refCount;
    };

    class Object : public RefCountedBase
    {
    public:
        static Class* GetBaseClass();
        static Object* CreateObject();

    public:
        virtual int SetProperty(unsigned int, void*);
        virtual char const* GetClassNameA() const;
        virtual int GetPropertiesList(std::set<unsigned int>&) const;
        virtual int ReadFromXmlNode(cmn::XmlFile*, cmn::XmlNode*);
        virtual int AddChild(Object*);
        virtual int IncWeakRef();
        virtual Object* Clone();
        virtual int GetProperty(unsigned int, void*) const;
        virtual int ReadFromXmlNodeAfterAdd(cmn::XmlFile*, cmn::XmlNode*);
        virtual int DecWeakRef();
        virtual ~Object();
        virtual Class* GetClass() const;
        virtual int RemoveChild(Object*);
        virtual int WriteToXmlNode(cmn::XmlFile*, cmn::XmlNode*);
        virtual int GetWeakRefCount();

        void SetChildDirty(bool);
        int LinkChildAtTail(Object*);
        char const* GetName() const;
        Object* ChildNodeFromXmlFile(char const*);
        bool GetChildDirty() const;
        int UnlinkChild(Object*);
        Object* GetParent() const;
        bool IsKindOf(Class const*) const;
        bool IsKindOf(char const*) const;
        bool IsChildOf(Object const*) const;
        Object* GetNextSibling() const;
        void SetName(CStr const&);
        Object* GetFirstChild() const;
        int RemoveAllChildren();
        Object* GetChildByName(CStr const&) const;
        void MoveChildToLastPosition(Object*);
        void MoveChildToFirstPosition(Object*);
        void SetPersistance(bool);
        Object* GetLastChild() const;
        bool IsDirectChild(Object const*) const;
        int LinkChildAtHead(Object*);

    protected:
        Object* ChildNodeFromXmlNode(cmn::XmlFile*, cmn::XmlNode*);
        Object();
        Object(Object const&);

    private:
        CStr m_name;
        bool m_persistant;
        bool m_isChildDirty;
        Object* m_parent;
        Object* m_firstChild;
        Object* m_lastChild;
        Object* m_nextSibling;
        Object* m_prevSibling;
        int m_numChildren;
        void* m_scriptHandle;
    };

    struct Class
    {
        const char* m_className;
        int m_classSize;
         Object* (__fastcall* m_fnCreateObject)();
        Class* (__fastcall* m_fnGetBaseClass)();
        int m_index;
        ExportInfo* m_lExports;
        void* m_scriptHandle;

    public:
        bool IsKindOf(char const*) const;
        bool IsKindOf(Class const*) const;
        Object* NewInstance() const;
    };
}
