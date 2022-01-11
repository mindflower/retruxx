#include <sstream>
#include <stdexcept>
#include <core/ini.h>
#include <core/kernel.h>
#include <core/scoped_ptr.h>
#include <file/fileserver.h>
#include <file/filestream.h>

namespace m3d
{
    cmn::XmlFile* ReadXmlFile(char const* filename, CStr* errorStr)
    {
        scoped_ptr fileStream = g_Kernel->GetFileServer().CreateFileStream();
        if (fileStream->Open(filename, fs::IStream::OPEN_READ))
        {
            //TODO: ref_ptr?
            auto* xmlFile = g_Kernel->CreateXmlFile();
            xmlFile->Read(*fileStream);
            fileStream->Close();
            if (xmlFile->GetError() != nullptr)
            {
                xmlFile->DecRef();
                xmlFile = nullptr;
                if (errorStr != nullptr)
                {
                    *errorStr = CStr("ReadXmlFile: Cannot parse file ") + filename;
                }
            }
            return xmlFile;
        }
        if (errorStr != nullptr)
        {
            *errorStr = CStr("ReadXmlFile: Cannot open file ") + filename;
        }
        return nullptr;
    }

    int SafeStrAttrib(CStr& v, cmn::XmlNode const* node, char const* attrName)
    {
        if (node->IsEmpty())
        {
            return 0;
        }
        auto const* attr = node->GetAttribute(attrName);
        if (attr == nullptr)
        {
            return 0;
        }
        v = attr;
        return 1;
    }

    void Tokenize(CStr const* str, std::vector<CStr>& tokens, char const* chars)
    {
        //TODO: check this
        if (str && !str->empty())
        {
            tokens.clear();
            size_t start;
            size_t end = 0;
            std::string_view view(str->c_str());
            while ((start = view.find_first_not_of(chars, end)) != CStr::npos)
            {
                end = view.find(chars, start);
                tokens.push_back(str->substr(start, end - start));
            }
        }
    }
}

bool XmlFileImpl::AddBeforeChild(m3d::cmn::XmlNode const*, m3d::cmn::XmlNode*)
{
    throw std::logic_error("Not implemented");
}

void XmlFileImpl::SetHeader(char const*, char const*, char const*)
{
    throw std::logic_error("Not implemented");
}

bool XmlFileImpl::AddAfterChild(m3d::cmn::XmlNode const*, m3d::cmn::XmlNode*)
{
    throw std::logic_error("Not implemented");
}

int XmlFileImpl::Write(m3d::fs::IStream&)
{
    throw std::logic_error("Not implemented");
}

void XmlFileImpl::GetHeader(char**, char**, char**)
{
    throw std::logic_error("Not implemented");
}

bool XmlFileImpl::AddChild(m3d::cmn::XmlNode*)
{
    throw std::logic_error("Not implemented");
}

bool XmlFileImpl::RemoveChild(m3d::cmn::XmlNode*)
{
    throw std::logic_error("Not implemented");
}

char const* XmlFileImpl::GetError()
{
    if (Error())
    {
        return ErrorDesc();
    }
    return nullptr;
}

bool XmlFileImpl::GetFirstChild_(m3d::cmn::XmlNode* writeTo, char const* wantValue) const
{
    auto child = wantValue ? FirstChild(wantValue) : FirstChild();
    auto writeToCasted = dynamic_cast<XmlNodeImpl*>(writeTo);
    *writeToCasted = XmlNodeImpl(const_cast<TiXmlNode*>(child));
    return child != nullptr;
}

XmlFileImpl::~XmlFileImpl()
{
}

int XmlFileImpl::Read(m3d::fs::IStream& in)
{
    auto const size = in.GetSize();
    std::vector<char> buffer(size + 1, 0);
    buffer[in.ReadBytes(buffer.data(), size)] = '\0';
    Parse(buffer.data());
    //TODO: check this;
    return !Error();
}

m3d::cmn::XmlNode* XmlFileImpl::CreateNode(m3d::cmn::XmlNodeType type, char const* nodeName) const
{
    return new XmlNodeImpl(type, nodeName);
}

bool XmlFileImpl::GetLastChild_(m3d::cmn::XmlNode* node, char const* nodeName)  const
{
    throw std::logic_error("Not implemented");
}

int XmlFileImpl::IncRef()
{
    if (m_parent)
    {
        m_parent->IncRef();
    }
    return ++m_refCount;
}

void* XmlFileImpl::QueryIface(char const*)
{
    throw std::logic_error("Not implemented");
}

int XmlFileImpl::DecRef()
{
    --m_refCount;
    auto const ret = m_refCount;
    if (m_parent)
    {
        m_parent->DecRef();
    }
    if (m_refCount <= 0)
    {
        XmlFileImpl::~XmlFileImpl();
    }
    return ret;
}

TiXmlNode* XmlFileImpl::GetDeclarationNode()
{
    throw std::logic_error("Not implemented");
}

bool XmlNodeImpl::HasChildOrAttribute() const
{
    throw std::logic_error("Not implemented");
}

bool XmlNodeImpl::AddBeforeChild(m3d::cmn::XmlNode const*, m3d::cmn::XmlNode*)
{
    throw std::logic_error("Not implemented");
}

char const* XmlNodeImpl::GetValue() const
{
    throw std::logic_error("Not implemented");
}

bool XmlNodeImpl::GetFirstAttribute(m3d::cmn::XmlAttrib* writeTo) const
{
    if (m_node && m_node->Type() == TiXmlNode::ELEMENT)
    {
        auto element = dynamic_cast<TiXmlElement*>(m_node);
        auto writeToCasted = dynamic_cast<XmlAttribImpl*>(writeTo);
        auto attr = element->FirstAttribute();
        *writeToCasted = XmlAttribImpl(attr);
        return attr != nullptr;
    }
    return false;
}

bool XmlNodeImpl::RemoveAttribute(char const*)
{
    throw std::logic_error("Not implemented");
}

bool XmlNodeImpl::IsEmpty() const
{
    return m_node == nullptr;
}

XmlNodeImpl::XmlNodeImpl(m3d::cmn::XmlNodeType type, char const* nodeName)
{
    //TODO: check this
    switch(type)
    {
    case m3d::cmn::XML_NODE_ELEMENT:
    {
        m_node = new TiXmlElement(nodeName);
        m_nodeOwned = true;
        break;
    }
    case m3d::cmn::XML_NODE_COMMENT:
    {
        m_node = new TiXmlComment;
        m_nodeOwned = true;
        break;
    }
    case m3d::cmn::XML_NODE_UNKNOWN:
    {
        m_node = new TiXmlUnknown;
        m_nodeOwned = true;
        break;
    }
    case m3d::cmn::XML_NODE_TEXT:
    {
        m_node = new TiXmlText(nodeName);
        m_nodeOwned = true;
        break;
    }
    default:
    {
        m_nodeOwned = true;
        break;
    }
    }
}

XmlNodeImpl::XmlNodeImpl(TiXmlNode* fromNode) :
    m_node(fromNode)
{
}

bool XmlNodeImpl::GetPrevSibling_(m3d::cmn::XmlNode*, char const*) const
{
    throw std::logic_error("Not implemented");
}

bool XmlNodeImpl::IsOfType(m3d::cmn::XmlNodeType) const
{
    throw std::logic_error("Not implemented");
}

bool XmlNodeImpl::AddAfterChild(m3d::cmn::XmlNode const*, m3d::cmn::XmlNode*)
{
    throw std::logic_error("Not implemented");
}

void XmlNodeImpl::SetValue(char const*)
{
    throw std::logic_error("Not implemented");
}

bool XmlNodeImpl::GetNextSibling_(m3d::cmn::XmlNode*, char const*) const
{
    throw std::logic_error("Not implemented");
}

bool XmlNodeImpl::GetParent(m3d::cmn::XmlNode*) const
{
    throw std::logic_error("Not implemented");
}

void XmlNodeImpl::GetAttributeMbcsSafe(char const*, char**, int*) const
{
    throw std::logic_error("Not implemented");
}

bool XmlNodeImpl::AddChild(m3d::cmn::XmlNode*)
{
    throw std::logic_error("Not implemented");
}

bool XmlNodeImpl::SetAttribute(char const*, char const*)
{
    throw std::logic_error("Not implemented");
}

bool XmlNodeImpl::RemoveChild(m3d::cmn::XmlNode*)
{
    throw std::logic_error("Not implemented");
}

m3d::cmn::XmlAttrib* XmlNodeImpl::CreateAttribute() const
{
    return new XmlAttribImpl;
}

bool XmlNodeImpl::GetFirstChild_(m3d::cmn::XmlNode*, char const*) const
{
    throw std::logic_error("Not implemented");
}

XmlNodeImpl::~XmlNodeImpl()
{
    if (m_nodeOwned)
    {
        delete m_node;
    }
}

char const* XmlNodeImpl::GetAttribute(char const*) const
{
    throw std::logic_error("Not implemented");
}

bool XmlNodeImpl::GetLastChild_(m3d::cmn::XmlNode*, char const*) const
{
    throw std::logic_error("Not implemented");
}

int XmlNodeImpl::IncRef()
{
    if (m_parent)
    {
        m_parent->IncRef();
    }
    return ++m_refCount;
}

XmlNodeImpl::XmlNodeImpl()
{
    throw std::logic_error("Not implemented");
}

void* XmlNodeImpl::QueryIface(char const*)
{
    throw std::logic_error("Not implemented");
}

int XmlNodeImpl::DecRef()
{
    --m_refCount;
    auto const ret = m_refCount;
    if (m_parent)
    {
        m_parent->DecRef();
    }
    if (m_refCount <= 0)
    {
        XmlNodeImpl::~XmlNodeImpl();
    }
    return ret;
}

bool XmlAttribImpl::GetNextSibling_(m3d::cmn::XmlAttrib* writeTo)
{
    auto writeToCasted = dynamic_cast<XmlAttribImpl*>(writeTo);
    auto attr = m_attrib->Next();
    *writeToCasted = XmlAttribImpl(attr);
    return attr != nullptr;
}

bool XmlAttribImpl::IsEmpty()
{
    return m_attrib == nullptr;
}

XmlAttribImpl::~XmlAttribImpl()
{
    if (m_attribOwned)
    {
        delete m_attrib;
    }
}

XmlAttribImpl::XmlAttribImpl(TiXmlAttribute* attr) :
    m_attrib(attr)
{
}

char const* XmlAttribImpl::GetValue()
{
    return m_attrib->Value();
}

char const* XmlAttribImpl::GetName()
{
    return m_attrib->Name();
}

int XmlAttribImpl::IncRef()
{
    if (m_parent)
    {
        m_parent->IncRef();
    }
    return ++m_refCount;
}

int XmlAttribImpl::DecRef()
{
    --m_refCount;
    auto const ret = m_refCount;
    if (m_parent)
    {
        m_parent->DecRef();
    }
    if (m_refCount <= 0)
    {
        XmlAttribImpl::~XmlAttribImpl();
    }
    return ret;
}

void* XmlAttribImpl::QueryIface(char const*)
{
    throw std::logic_error("Not implemented");
}
