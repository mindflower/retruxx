#pragma once

namespace m3d
{
    class RefCountedBase
    {
    public:
        int IncRef();
        RefCountedBase();
        int DecRef();
        virtual ~RefCountedBase();
        int GetRefCount();
    protected:
    private:
        RefCountedBase_vtbl *__vftable /*VFT*/;
        int m_refCount;
    };
}

namespace m3d
{
    class Object :  public RefCountedBase
    {
    public:
        void SetChildDirty(bool);
        int LinkChildAtTail(class Object *);
        virtual int SetProperty(unsigned int,void *);
        virtual char const * GetClassNameA() const ;
        char const * GetName() const ;
        virtual int GetPropertiesList(class std::set<unsigned int,struct std::less<unsigned int>,class std::allocator<unsigned int> > &) const ;
        class Object * ChildNodeFromXmlFile(char const *);
        virtual int ReadFromXmlNode(cmn::XmlFile *,cmn::XmlNode *);
        virtual int AddChild(class Object *);
        virtual int IncWeakRef();
        bool GetChildDirty() const ;
        virtual class Object * Clone();
        int UnlinkChild(class Object *);
        class Object * GetParent() const ;
        bool IsKindOf(Class const *) const ;
        bool IsKindOf(char const *) const ;
        virtual int GetProperty(unsigned int,void *) const ;
        static Class * __fastcall GetBaseClass();
        bool IsChildOf(class Object const *) const ;
        class Object * GetNextSibling() const ;
        void SetName(class CStr const &);
        virtual int ReadFromXmlNodeAfterAdd(cmn::XmlFile *,cmn::XmlNode *);
        class Object * GetFirstChild() const ;
        int RemoveAllChildren();
        class Object * GetChildByName(class CStr const &) const ;
        virtual int DecWeakRef();
        virtual ~Object();
        virtual Class * GetClass() const ;
        void MoveChildToLastPosition(class Object *);
        virtual int RemoveChild(class Object *);
        static class Object * __fastcall CreateObject();
        void MoveChildToFirstPosition(class Object *);
        void SetPersistance(bool);
        class Object * GetLastChild() const ;
        bool IsDirectChild(class Object const *) const ;
        int LinkChildAtHead(class Object *);
        virtual int WriteToXmlNode(cmn::XmlFile *,cmn::XmlNode *);
        virtual int GetWeakRefCount();
    protected:
        class Object * ChildNodeFromXmlNode(cmn::XmlFile *,cmn::XmlNode *);
        Object();
        Object(class Object const &);
    private:
        CStr m_name;
        bool m_persistant;
        bool m_isChildDirty;
        Object *m_parent;
        Object *m_firstChild;
        Object *m_lastChild;
        Object *m_nextSibling;
        Object *m_prevSibling;
        int m_numChildren;
        void *m_scriptHandle;
    };
}
