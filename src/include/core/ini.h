#pragma once
#include "stringm3d.h"
#include <iface.h>
#include <vector>
#include <engine/tinyxml/tinyxml.h>
#include <sstream>

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
            virtual ~IniFile() = default;
            virtual int Read(m3d::fs::IStream&) = 0;
            virtual int Write(m3d::fs::IStream&) = 0;
            virtual char const* GetError() = 0;
            virtual char const* GetString(CStr const&, CStr const&) = 0;
            virtual int GetInteger(CStr const&, CStr const&) = 0;
            virtual unsigned int GetHex(CStr const&, CStr const&) = 0;
            virtual float GetFloat(CStr const&, CStr const&) = 0;
            virtual void SetString(CStr const&, CStr const&, CStr const&) = 0;
            virtual void SetInteger(CStr const&, CStr const&, int) = 0;
            virtual void SetFloat(CStr const&, CStr const&, float) = 0;
        };

        //IMPORTANT: fields and members order is strict
        class XmlAttrib : public IBase
        {
        public:
            virtual ~XmlAttrib() = default;
            virtual int DecRef() = 0;
            virtual int IncRef() = 0;
            virtual void* QueryIface(const char*) = 0;
            virtual char const* GetName() = 0;
            virtual char const* GetValue() = 0;
            virtual bool GetNextSibling_(XmlAttrib*) = 0;
            virtual bool IsEmpty() = 0;
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

        //IMPORTANT: fields and members order is strict
        class XmlNode : public IBase
        {
        public:
            virtual ~XmlNode() = default;
            virtual int DecRef() = 0;
            virtual int IncRef() = 0;
            virtual void* QueryIface(const char*) = 0;
            virtual bool IsOfType(XmlNodeType) const = 0;
            virtual bool IsEmpty() const = 0;
            virtual bool HasChildOrAttribute() const = 0;
            virtual bool GetParent(XmlNode*) const = 0;
            virtual bool GetNextSibling_(XmlNode*, char const*) const = 0;  //GetNextSibling
            virtual bool GetPrevSibling_(XmlNode*, char const*) const = 0;  //GetPrevSibling
            virtual bool GetFirstChild_(XmlNode*, char const*) const = 0; //GetFirstChild
            virtual bool GetLastChild_(XmlNode*, char const*) const = 0;     //GetLastChild
            virtual bool AddChild(XmlNode*) = 0;
            virtual bool AddBeforeChild(XmlNode const*, XmlNode*) = 0;
            virtual bool AddAfterChild(XmlNode const*, XmlNode*) = 0;
            virtual bool RemoveChild(XmlNode*) = 0;
            virtual char const* GetValue() const = 0;
            virtual void SetValue(char const*) = 0;
            virtual char const* GetAttribute(char const*) const = 0;
            virtual void GetAttributeMbcsSafe(char const*, char**, int*) const = 0;
            virtual bool SetAttribute(char const*, char const*) = 0;
            virtual bool RemoveAttribute(char const*) = 0;
            virtual XmlAttrib* CreateAttribute() const = 0;
            virtual bool GetFirstAttribute(XmlAttrib*) const = 0;
        };

        //IMPORTANT: fields and members order is strict
        class XmlFile : public IBase
        {
        protected:
            virtual ~XmlFile() = default;

        public:
            virtual int Read(fs::IStream&) = 0;
            virtual int Write(fs::IStream&) = 0;
            virtual char const* GetError() = 0;
            virtual XmlNode* CreateNode(XmlNodeType, char const*) const = 0;
            virtual bool GetFirstChild_(XmlNode*, char const*) const = 0;
            virtual bool GetLastChild_(XmlNode*, char const*) const = 0;
            virtual bool AddChild(XmlNode*) = 0;
            virtual bool AddBeforeChild(XmlNode const*, XmlNode*) = 0;
            virtual bool AddAfterChild(XmlNode const*, XmlNode*) = 0;
            virtual bool RemoveChild(XmlNode*) = 0;
            virtual void GetHeader(char**, char**, char**) = 0;
            virtual void SetHeader(char const*, char const*, char const*) = 0;
        };
    }

    cmn::XmlFile* ReadXmlFile(char const* filename, CStr* errorStr);
    int SafeStrAttrib(CStr&, cmn::XmlNode const*, char const*);
    bool SafeClrAttrib(unsigned int&, m3d::cmn::XmlNode const*, char const*);
    bool SafeIntAttrib(int&, m3d::cmn::XmlNode const*, char const*);
    bool SafeUintAttrib(unsigned&, m3d::cmn::XmlNode const*, char const*);
    bool SafeFloatAttrib(float&, m3d::cmn::XmlNode const*, char const*);

    template<class T>
    bool SafeEnumAttrib(T& v, m3d::cmn::XmlNode const* node, char const* attrib)
    {
        CStr str;
        if (SafeStrAttrib(str, node, attrib) && !str.empty())
        {
            int temp = 0;
            std::istringstream iss(str.c_str());
            iss >> temp;
            v = static_cast<T>(temp);
            return true;
        }
        return false;
    }

    void Tokenize(CStr const*, std::vector<CStr>&, char const*);
}

//IMPORTANT: fields and members order is strict
class XmlFileImpl :
    public TiXmlDocument,
    public m3d::cmn::XmlFile
{
private:
    int m_refCount = 0;
    IBase* m_parent = nullptr;

private:
    virtual int DecRef();
    virtual int IncRef();
    virtual void* QueryIface(char const*);

public:
    XmlFileImpl();
    virtual ~XmlFileImpl();
    virtual int Read(m3d::fs::IStream&);
    virtual int Write(m3d::fs::IStream&);
    virtual char const* GetError();
    virtual m3d::cmn::XmlNode* CreateNode(m3d::cmn::XmlNodeType, char const*) const;
    virtual bool GetFirstChild_(m3d::cmn::XmlNode*, char const*) const;
    virtual bool GetLastChild_(m3d::cmn::XmlNode*, char const*) const;
    virtual bool AddChild(m3d::cmn::XmlNode*);
    virtual bool AddBeforeChild(m3d::cmn::XmlNode const*, m3d::cmn::XmlNode*);
    virtual bool AddAfterChild(m3d::cmn::XmlNode const*, m3d::cmn::XmlNode*);
    virtual bool RemoveChild(m3d::cmn::XmlNode*);
    virtual void GetHeader(char**, char**, char**);
    virtual void SetHeader(char const*, char const*, char const*);

private:
    TiXmlNode* GetDeclarationNode();

};

class IniFileImpl : public m3d::cmn::IniFile
{
public:
    IniFileImpl();
    virtual int Write(m3d::fs::IStream&);
    virtual char const* GetError();
    virtual unsigned int GetHex(CStr const&, CStr const&);
    virtual void SetString(CStr const&, CStr const&, CStr const&);
    virtual int GetInteger(CStr const&, CStr const&);
    virtual float GetFloat(CStr const&, CStr const&);
    virtual ~IniFileImpl();
    virtual char const* GetString(CStr const&, CStr const&);
    virtual int Read(m3d::fs::IStream&);
    virtual void SetInteger(CStr const&, CStr const&, int);
    virtual void SetFloat(CStr const&, CStr const&, float);

private:
    virtual int IncRef();
    bool FindSection(m3d::cmn::XmlNode*, CStr const&);
    bool AddKey(m3d::cmn::XmlNode*, CStr const&, CStr const&, CStr const&);
    virtual int DecRef();
    virtual void* QueryIface(char const*);
    bool FindKey(m3d::cmn::XmlNode*, CStr const&, CStr const&);

private:
    int m_refCount = 0;
    IBase* m_parent = 0;
    m3d::cmn::XmlFile* m_file = nullptr;
};

//IMPORTANT: fields and members order is strict
class XmlNodeImpl : public m3d::cmn::XmlNode
{
    int m_refCount = 0;
    IBase* m_parent = nullptr;

private:
    virtual int DecRef();
    virtual int IncRef();
    virtual void* QueryIface(char const*);

public:
    XmlNodeImpl(XmlNodeImpl const&);
    XmlNodeImpl(TiXmlNode*);
    XmlNodeImpl(m3d::cmn::XmlNodeType, char const*);
    XmlNodeImpl();

private:
    TiXmlNode* m_node = nullptr;
    bool m_nodeOwned = false;

public:
    virtual ~XmlNodeImpl();
    virtual bool IsOfType(m3d::cmn::XmlNodeType) const;
    virtual bool IsEmpty() const;
    virtual bool HasChildOrAttribute() const;
    virtual bool GetParent(m3d::cmn::XmlNode*) const;
    virtual bool GetNextSibling_(m3d::cmn::XmlNode*, char const*) const;
    virtual bool GetPrevSibling_(m3d::cmn::XmlNode*, char const*) const;
    virtual bool GetFirstChild_(m3d::cmn::XmlNode*, char const*) const;
    virtual bool GetLastChild_(m3d::cmn::XmlNode*, char const*) const;
    virtual bool AddChild(m3d::cmn::XmlNode*);
    virtual bool AddBeforeChild(m3d::cmn::XmlNode const*, m3d::cmn::XmlNode*);
    virtual bool AddAfterChild(m3d::cmn::XmlNode const*, m3d::cmn::XmlNode*);
    virtual bool RemoveChild(m3d::cmn::XmlNode*);
    virtual char const* GetValue() const;
    virtual void SetValue(char const*);
    virtual char const* GetAttribute(char const*) const;
    virtual void GetAttributeMbcsSafe(char const*, char**, int*) const;
    virtual bool SetAttribute(char const*, char const*);
    virtual bool RemoveAttribute(char const*);
    virtual m3d::cmn::XmlAttrib* CreateAttribute() const;
    virtual bool GetFirstAttribute(m3d::cmn::XmlAttrib*) const;
};

//IMPORTANT: fields and members order is strict
class XmlAttribImpl : public m3d::cmn::XmlAttrib
{
    friend class XmlNodeImpl;
    int m_refCount = 0;
    IBase* m_parent = nullptr;

private:
    virtual int DecRef();
    virtual int IncRef();
    virtual void* QueryIface(char const*);

private:
    TiXmlAttribute* m_attrib = nullptr;
    bool m_attribOwned = false;

public:
    XmlAttribImpl(XmlAttribImpl const&);
    XmlAttribImpl();
    virtual ~XmlAttribImpl();
    virtual char const* GetName();
    virtual char const* GetValue();
    virtual bool GetNextSibling_(m3d::cmn::XmlAttrib*);
    virtual bool IsEmpty();
};

