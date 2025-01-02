#pragma once

#include "stringm3d.h"
#include <iface.h>
#include <vector>
#include <engine/tinyxml/tinyxml.h>
#include <sstream>
#include "thirdparty/stl/vector.hpp"

#undef GetFirstChild
#undef GetFirstSibling
#undef GetLastSibling
#undef GetNextSibling
#undef GetPrevSibling

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
            virtual bool GetNextSibling(XmlAttrib*) = 0;
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
        struct XmlNode : public IBase
        {
            virtual bool IsOfType(m3d::cmn::XmlNodeType) const = 0 /* 0x10 */;
            virtual bool IsEmpty() const = 0 /* 0x14 */;
            virtual bool HasChildOrAttribute() const = 0 /* 0x18 */;
            virtual bool GetParent(m3d::cmn::XmlNode*) const = 0 /* 0x1c */;
            virtual bool GetNextSibling(m3d::cmn::XmlNode*, const char*) const = 0 /* 0x20 */;
            virtual bool GetPrevSibling(m3d::cmn::XmlNode*, const char*) const = 0 /* 0x24 */;
            virtual bool GetFirstChild(m3d::cmn::XmlNode*, const char*) const = 0 /* 0x28 */;
            virtual bool GetLastChild(m3d::cmn::XmlNode*, const char*) const = 0 /* 0x2c */;
            virtual bool AddChild(m3d::cmn::XmlNode*) = 0 /* 0x30 */;
            virtual bool AddBeforeChild(const m3d::cmn::XmlNode*, m3d::cmn::XmlNode*) = 0 /* 0x34 */;
            virtual bool AddAfterChild(const m3d::cmn::XmlNode*, m3d::cmn::XmlNode*) = 0 /* 0x38 */;
            virtual bool RemoveChild(m3d::cmn::XmlNode*) = 0 /* 0x3c */;
            virtual const char* GetValue() const = 0 /* 0x40 */;
            virtual void SetValue(const char*) = 0 /* 0x44 */;
            virtual const char* GetAttribute(const char*) const = 0 /* 0x48 */;
            virtual void GetAttributeMbcsSafe(const char*, char**, int*) const = 0 /* 0x4c */;
            virtual bool SetAttribute(const char*, const char*) = 0 /* 0x50 */;
            virtual bool RemoveAttribute(const char*) = 0 /* 0x54 */;
            virtual m3d::cmn::XmlAttrib* CreateAttribute() const = 0 /* 0x58 */;
            virtual bool GetFirstAttribute(m3d::cmn::XmlAttrib*) const = 0 /* 0x5c */;
        }; /* size: 0x0004 */

        //IMPORTANT: fields and members order is strict
        class XmlFile : public IBase
        {
        protected:
            virtual ~XmlFile() = default;

        public:
            virtual int Read(fs::IStream&) = 0;
            virtual int Write(fs::IStream&) = 0;
            virtual char const* GetError() = 0;
            virtual XmlNode* CreateNode(XmlNodeType = XML_NODE_EMPTY, char const* = nullptr) const = 0;
            virtual bool GetFirstChild(XmlNode*, char const*) const = 0;
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
    bool SafeInt64Attrib(__int64&, m3d::cmn::XmlNode const*, char const*);
    bool SafeUintAttrib(unsigned&, m3d::cmn::XmlNode const*, char const*);
    bool SafeFloatAttrib(float&, m3d::cmn::XmlNode const*, char const*);
    bool SafeBoolAttrib(bool&, m3d::cmn::XmlNode const*, char const*);
    bool SafeVector2Attrib(CVector2&, m3d::cmn::XmlNode const*, char const*);
    bool SafeVectorAttrib(CVector&, m3d::cmn::XmlNode const*, char const*);
    bool SafeQuaternionAttrib(Quaternion&, m3d::cmn::XmlNode const*, char const*);
    void Tokenize(CStr const&, oldstd::vector<CStr>&, char const*);

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

    void Tokenize(CStr const*, oldstd::vector<CStr>&, char const*);
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
    virtual bool GetFirstChild(m3d::cmn::XmlNode*, char const*) const;
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
    friend class XmlFileImpl;
private:
    /* 0x0004 */ int m_refCount;
    /* 0x0008 */ IBase* m_parent;
    virtual int DecRef() override /* 0x04 */;
    virtual int IncRef() override /* 0x08 */;
    virtual void* QueryIface(const char* ifaceName) override /* 0x0c */;
    XmlNodeImpl(const XmlNodeImpl&);
    XmlNodeImpl(TiXmlNode* fromNode);
    XmlNodeImpl(m3d::cmn::XmlNodeType tt, const char* nodeName);
    XmlNodeImpl();
    /* 0x000c */ TiXmlNode* m_node;
    /* 0x0010 */ bool m_nodeOwned;

public:
    virtual  ~XmlNodeImpl() override /* 0x00 */;
    virtual bool IsOfType(m3d::cmn::XmlNodeType castTo) const override /* 0x10 */;
    virtual bool IsEmpty() const override /* 0x14 */;
    virtual bool HasChildOrAttribute() const override /* 0x18 */;
    virtual bool GetParent(m3d::cmn::XmlNode* writeTo) const override /* 0x1c */;
    virtual bool GetNextSibling(m3d::cmn::XmlNode* writeTo, const char* wantValue) const override /* 0x20 */;
    virtual bool GetPrevSibling(m3d::cmn::XmlNode* writeTo, const char* wantValue) const override /* 0x24 */;
    virtual bool GetFirstChild(m3d::cmn::XmlNode* writeTo, const char* wantValue) const override /* 0x28 */;
    virtual bool GetLastChild(m3d::cmn::XmlNode* writeTo, const char* wantValue) const override /* 0x2c */;
    virtual bool AddChild(m3d::cmn::XmlNode* child) override /* 0x30 */;
    virtual bool AddBeforeChild(const m3d::cmn::XmlNode* addBefore, m3d::cmn::XmlNode* child) override /* 0x34 */;
    virtual bool AddAfterChild(const m3d::cmn::XmlNode* addAfter, m3d::cmn::XmlNode* child) override /* 0x38 */;
    virtual bool RemoveChild(m3d::cmn::XmlNode* child) override /* 0x3c */;
    virtual const char* GetValue() const override /* 0x40 */;
    virtual void SetValue(const char* v) override /* 0x44 */;
    virtual const char* GetAttribute(const char* name) const override /* 0x48 */;
    virtual void GetAttributeMbcsSafe(const char* name, char** str, int* strSz) const override /* 0x4c */;
    virtual bool SetAttribute(const char* name, const char* value) override /* 0x50 */;
    virtual bool RemoveAttribute(const char* name) override /* 0x54 */;
    virtual m3d::cmn::XmlAttrib* CreateAttribute() const override /* 0x58 */;
    virtual bool GetFirstAttribute(m3d::cmn::XmlAttrib* writeTo) const override /* 0x5c */;
}; /* size: 0x0014 */

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
    virtual bool GetNextSibling(m3d::cmn::XmlAttrib*);
    virtual bool IsEmpty();
};

