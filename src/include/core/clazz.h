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
    class Object;

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

    class RefCountedBase
    {
    public:
        virtual ~RefCountedBase() = default;
        int IncRef();
        int DecRef();
        int GetRefCount() const;

    private:
        int m_refCount = 0;
    };

    class Object : public RefCountedBase
    {
    public:
        static inline Class m_classObject;

    public:
        static Class* GetBaseClass();
        static Object* CreateObject();

    public:
        virtual ~Object() = default;
        virtual int SetProperty(unsigned int propId, void* prop);
        virtual char const* GetClassNameA() const;
        virtual int GetPropertiesList(std::set<unsigned int>&) const;
        virtual int ReadFromXmlNode(cmn::XmlFile*, cmn::XmlNode*);
        virtual int AddChild(Object*);
        virtual int IncWeakRef();
        virtual Object* Clone();
        virtual int GetProperty(unsigned int propId, void* prop) const;
        virtual int ReadFromXmlNodeAfterAdd(cmn::XmlFile*, cmn::XmlNode*);
        virtual int DecWeakRef();
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
        Object* GetNextRelative() const;
        void SetName(CStr const&);
        Object* GetFirstNestling() const;
        int RemoveAllChildren();
        Object* GetChildByName(CStr const&) const;
        void MoveChildToLastPosition(Object*);
        void MoveChildToFirstPosition(Object*);
        void SetPersistance(bool);
        Object* GetLastNestling() const;
        bool IsDirectChild(Object const*) const;
        int LinkChildAtHead(Object*);

    protected:
        Object();
        Object(Object const&);
        Object* ChildNodeFromXmlNode(cmn::XmlFile*, cmn::XmlNode*);

    private:
        CStr m_name;
        bool m_persistant = true;
        bool m_isChildDirty = false;
        Object* m_parent = nullptr;
        Object* m_firstChild = nullptr;
        Object* m_lastChild = nullptr;
        Object* m_nextSibling = nullptr;
        Object* m_prevSibling = nullptr;
        int m_numChildren = 0;
        void* m_scriptHandle = nullptr;
    };
}
