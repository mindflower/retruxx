#pragma once
#include "stringm3d.h"
#include <iface.h>
#include <vector>
#include <engine/tinyxml/tinyxml.h>

namespace m3d
{
    namespace fs
    {
        class IStream;
    }
}

namespace m3d
{
    //TODO: add static functions

    namespace cmn
    {
        class IniFile : public IBase
        {
        public:
        protected:
        private:
        };

        class XmlAttrib : public IBase
        {
        public:
            virtual bool GetNextSibling_(XmlAttrib*) = 0;
            virtual bool IsEmpty() = 0;
            virtual ~XmlAttrib() = default;
            virtual char const* GetValue() = 0;
            virtual char const* GetName() = 0;
        };

        enum XmlNodeType
        {
            XML_NODE_EMPTY = 0x0,
            XML_NODE_DOCUMENT = 0x1,
            XML_NODE_ELEMENT = 0x2,
            XML_NODE_COMMENT = 0x3,
            XML_NODE_UNKNOWN = 0x4,
            XML_NODE_TEXT = 0x5,
            XML_NODE_DECLARATION = 0x6,
        };

        class XmlNode : public IBase
        {
        public:
            virtual ~XmlNode() = default;
            virtual bool HasChildOrAttribute() const = 0;
            virtual bool AddBeforeChild(XmlNode const*, XmlNode*) = 0;
            virtual char const* GetValue() const = 0;
            virtual bool GetFirstAttribute(XmlAttrib*) const = 0;
            virtual bool RemoveAttribute(char const*) = 0;
            virtual bool IsEmpty() const = 0;
            virtual bool GetPrevSibling_(XmlNode*, char const*) const = 0;  //GetPrevSibling
            virtual bool IsOfType(XmlNodeType) const = 0;
            virtual bool AddAfterChild(XmlNode const*, XmlNode*) = 0;
            virtual void SetValue(char const*) = 0;
            virtual bool GetNextSibling_(XmlNode*, char const*) const = 0;  //GetNextSibling
            virtual bool GetParent(XmlNode*) const = 0;
            virtual void GetAttributeMbcsSafe(char const*, char**, int*) const = 0;
            virtual bool AddChild(XmlNode*) = 0;
            virtual bool SetAttribute(char const*, char const*) = 0;
            virtual bool RemoveChild(XmlNode*) = 0;
            virtual XmlAttrib* CreateAttribute() const = 0;
            virtual bool GetFirstChild_(XmlNode*, char const*) const = 0; //GetFirstChild
            virtual char const* GetAttribute(char const*) const = 0;
            virtual bool GetLastChild_(XmlNode*, char const*) const = 0;     //GetLastChild
        };

        class XmlFile : public IBase
        {
        public:
            virtual ~XmlFile() = default;
            virtual bool AddBeforeChild(XmlNode const*, XmlNode*) = 0;
            virtual void SetHeader(char const*, char const*, char const*) = 0;
            virtual bool AddAfterChild(XmlNode const*, XmlNode*) = 0;
            virtual int Write(fs::IStream&) = 0;
            virtual void GetHeader(char**, char**, char**) = 0;
            virtual bool AddChild(XmlNode*) = 0;
            virtual bool RemoveChild(XmlNode*) = 0;
            virtual bool GetFirstChild_(XmlNode*, char const*) const = 0;
            virtual int Read(fs::IStream&) = 0;
            virtual XmlNode* CreateNode(XmlNodeType, char const*) const = 0;
            virtual bool GetLastChild_(XmlNode*, char const*) const = 0;
            virtual char const* GetError() = 0;
        };
    }

    cmn::XmlFile* ReadXmlFile(char const* filename, CStr* errorStr);
    int SafeStrAttrib(CStr&, cmn::XmlNode const*, char const*);
    void Tokenize(CStr const*, std::vector<CStr>&, char const*);
}

class XmlFileImpl :
    public TiXmlDocument,
    public m3d::cmn::XmlFile
{
public:
    virtual bool AddBeforeChild(m3d::cmn::XmlNode const*, m3d::cmn::XmlNode*);
    virtual void SetHeader(char const*, char const*, char const*);
    virtual bool AddAfterChild(m3d::cmn::XmlNode const*, m3d::cmn::XmlNode*);
    virtual int Write(m3d::fs::IStream&);
    virtual void GetHeader(char**, char**, char**);
    virtual bool AddChild(m3d::cmn::XmlNode*);
    virtual bool RemoveChild(m3d::cmn::XmlNode*);
    virtual char const* GetError();
    virtual bool GetFirstChild_(m3d::cmn::XmlNode*, char const*) const;
    virtual ~XmlFileImpl();
    virtual int Read(m3d::fs::IStream&);
    virtual m3d::cmn::XmlNode* CreateNode(m3d::cmn::XmlNodeType, char const*) const;
    virtual bool GetLastChild_(m3d::cmn::XmlNode*, char const*) const;

private:
    virtual int IncRef();
    virtual void* QueryIface(char const*);
    virtual int DecRef();
    TiXmlNode* GetDeclarationNode();

private:
    int m_refCount = 0;
    IBase* m_parent = nullptr;
};

class XmlNodeImpl : public m3d::cmn::XmlNode
{
public:
    virtual bool HasChildOrAttribute() const;
    virtual bool AddBeforeChild(m3d::cmn::XmlNode const*, m3d::cmn::XmlNode*);
    virtual char const* GetValue() const;
    virtual bool GetFirstAttribute(m3d::cmn::XmlAttrib*) const;
    virtual bool RemoveAttribute(char const*);
    virtual bool IsEmpty() const;
    XmlNodeImpl(m3d::cmn::XmlNodeType, char const*);
    XmlNodeImpl(TiXmlNode*);
    virtual bool GetPrevSibling_(m3d::cmn::XmlNode*, char const*) const;
    virtual bool IsOfType(m3d::cmn::XmlNodeType) const;
    virtual bool AddAfterChild(m3d::cmn::XmlNode const*, m3d::cmn::XmlNode*);
    virtual void SetValue(char const*);
    virtual bool GetNextSibling_(m3d::cmn::XmlNode*, char const*) const;
    virtual bool GetParent(m3d::cmn::XmlNode*) const;
    virtual void GetAttributeMbcsSafe(char const*, char**, int*) const;
    virtual bool AddChild(m3d::cmn::XmlNode*);
    virtual bool SetAttribute(char const*, char const*);
    virtual bool RemoveChild(m3d::cmn::XmlNode*);
    virtual m3d::cmn::XmlAttrib* CreateAttribute() const;
    virtual bool GetFirstChild_(m3d::cmn::XmlNode*, char const*) const;
    virtual ~XmlNodeImpl();
    virtual char const* GetAttribute(char const*) const;
    virtual bool GetLastChild_(m3d::cmn::XmlNode*, char const*) const;

private:
    virtual int IncRef();
    XmlNodeImpl();
    virtual void* QueryIface(char const*);
    virtual int DecRef();

private:
    int m_refCount = 0;
    IBase* m_parent = nullptr;
    TiXmlNode* m_node = nullptr;
    bool m_nodeOwned = false;
};

class XmlAttribImpl : public m3d::cmn::XmlAttrib
{
public:
    virtual bool GetNextSibling_(m3d::cmn::XmlAttrib*);
    virtual bool IsEmpty();
    virtual ~XmlAttribImpl();
    XmlAttribImpl() = default;
    XmlAttribImpl(TiXmlAttribute*);
    virtual char const* GetValue();
    virtual char const* GetName();

private:
    virtual int IncRef();
    virtual int DecRef();
    virtual void* QueryIface(char const*);

private:
    int m_refCount = 0;
    IBase* m_parent = nullptr;
    TiXmlAttribute* m_attrib = nullptr;
    bool m_attribOwned = false;
};

