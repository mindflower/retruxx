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

    bool SafeClrAttrib(unsigned& clr, m3d::cmn::XmlNode const* node, char const* attrib)
    {
        CStr str;
        if (SafeStrAttrib(str, node, attrib) && !str.empty())
        {
            clr = std::stoul(str.c_str(), nullptr, 16);;
            return true;;
        }
        return false;
    }

    bool SafeIntAttrib(int& v, m3d::cmn::XmlNode const* node, char const* attrib)
    {
        CStr str;
        if (SafeStrAttrib(str, node, attrib) && !str.empty())
        {
            std::istringstream iss(str.c_str());
            iss >> v;
            return true;
        }
        return false;
    }

    bool SafeUintAttrib(unsigned& v, m3d::cmn::XmlNode const* node, char const* attrib)
    {
        CStr str;
        if (SafeStrAttrib(str, node, attrib) && !str.empty())
        {
            std::istringstream iss(str.c_str());
            iss >> v;
            return true;
        }
        return false;
    }

    bool SafeFloatAttrib(float& v, m3d::cmn::XmlNode const* node, char const* attrib)
    {
        CStr str;
        if (SafeStrAttrib(str, node, attrib) && !str.empty())
        {
            std::istringstream iss(str.c_str());
            iss >> v;
            return true;
        }
        return false;
    }

    bool SafeBoolAttrib(bool& v, m3d::cmn::XmlNode const* node, char const* attrib)
    {
        if (node->IsEmpty())
        {
            return 0;
        }
        auto attrValue = node->GetAttribute(attrib);
        if (!attrValue)
        {
            return 0;
        }
        auto trueVal =
            !stricmp(attrValue, "yes") ||
            !stricmp(attrValue, "yeah") ||
            !stricmp(attrValue, "yep") ||
            !stricmp(attrValue, "1") ||
            !stricmp(attrValue, "true");
        auto falseVal =
            !stricmp(attrValue, "false") ||
            !stricmp(attrValue, "0") ||
            !stricmp(attrValue, "no") ||
            !stricmp(attrValue, "nope") ||
            !stricmp(attrValue, "none");
        if (!trueVal && !falseVal)
        {
            return 0;
        }
        v = trueVal;
        return 1;
    }

    bool SafeVector2Attrib(CVector2& v, m3d::cmn::XmlNode const* node, char const* attrib)
    {
        if (node->IsEmpty())
        {
            return 0;
        }
        auto val = node->GetAttribute(attrib);
        if (!val)
        {
            return 0;
        }
        v = strToVec2(val);
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
            while ((start = view.find_first_not_of(chars, end)) != CStr_npos)
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
        return ErrorDesc().c_str();
    }
    return nullptr;
}

bool XmlFileImpl::GetFirstChild_(m3d::cmn::XmlNode* writeTo, char const* wantValue) const
{
    auto child = wantValue ? FirstChild(wantValue) : FirstChild();
    auto writeToCasted = dynamic_cast<XmlNodeImpl*>(writeTo);
    *writeToCasted = XmlNodeImpl(child);
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

XmlFileImpl::XmlFileImpl()
{
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

IniFileImpl::IniFileImpl() : m_file(m3d::g_Kernel->CreateXmlFile())
{
}

int IniFileImpl::Write(m3d::fs::IStream&)
{
    throw std::logic_error("Not implemented");
}

char const* IniFileImpl::GetError()
{
    throw std::logic_error("Not implemented");
}

unsigned IniFileImpl::GetHex(CStr const&, CStr const&)
{
    throw std::logic_error("Not implemented");
}

void IniFileImpl::SetString(CStr const&, CStr const&, CStr const&)
{
    throw std::logic_error("Not implemented");
}

int IniFileImpl::GetInteger(CStr const&, CStr const&)
{
    throw std::logic_error("Not implemented");
}

float IniFileImpl::GetFloat(CStr const& section, CStr const& key)
{
    return strtof(GetString(section, key), nullptr);
}

IniFileImpl::~IniFileImpl()
{
    if (m_file)
    {
        m_file->DecRef();
    }
}

char const* IniFileImpl::GetString(CStr const& section, CStr const& key)
{
    ref_ptr node = m_file->CreateNode(m3d::cmn::XML_NODE_EMPTY, nullptr);
    FindKey(node, section, key);
    if (node->IsEmpty())
    {
        return "";
    }
    return node->GetValue();

}

int IniFileImpl::Read(m3d::fs::IStream& in)
{
    return m_file->Read(in);
}

void IniFileImpl::SetInteger(CStr const&, CStr const&, int)
{
    throw std::logic_error("Not implemented");
}

void IniFileImpl::SetFloat(CStr const&, CStr const&, float)
{
    throw std::logic_error("Not implemented");
}

int IniFileImpl::IncRef()
{
    if (m_parent)
    {
        m_parent->IncRef();
    }
    return ++m_refCount;
}

bool IniFileImpl::FindSection(m3d::cmn::XmlNode* writeTo, CStr const& section)
{
    ref_ptr node = m_file->CreateNode(m3d::cmn::XML_NODE_EMPTY, nullptr);
    if (m_file->GetFirstChild_(node, "Ini"))
    {
        node->GetFirstChild_(writeTo, "Section");
        while (!writeTo->IsEmpty())
        {
            if (writeTo->IsOfType(m3d::cmn::XML_NODE_ELEMENT))
            {
                auto attr = writeTo->GetAttribute("name");
                if (attr)
                {
                    if (section == attr)
                    {
                        break;
                    }
                }
            }
            writeTo->GetNextSibling_(writeTo, "Section");
        }
    }
    return !writeTo->IsEmpty();
}

bool IniFileImpl::AddKey(m3d::cmn::XmlNode*, CStr const&, CStr const&, CStr const&)
{
    throw std::logic_error("Not implemented");
}

int IniFileImpl::DecRef()
{
    --m_refCount;
    auto const ret = m_refCount;
    if (m_parent)
    {
        m_parent->DecRef();
    }
    if (m_refCount <= 0)
    {
        IniFileImpl::~IniFileImpl();
    }
    return ret;
}

void* IniFileImpl::QueryIface(char const*)
{
    throw std::logic_error("Not implemented");
}

bool IniFileImpl::FindKey(m3d::cmn::XmlNode* writeTo, CStr const& section, CStr const& key)
{
    if (!FindSection(writeTo, section))
    {
        return false;
    }
    writeTo->GetFirstChild_(writeTo, "Key");
    while (!writeTo->IsEmpty())
    {
        if (writeTo->IsOfType(m3d::cmn::XML_NODE_ELEMENT))
        {
            auto attr = writeTo->GetAttribute("name");
            if (attr)
            {
                if (attr == key)
                {
                    break;
                }
            }
        }
        writeTo->GetNextSibling_(writeTo, "Key");
    }
    if (writeTo->IsEmpty())
    {
        return false;
    }
    writeTo->GetFirstChild_(writeTo, nullptr);
    while (!writeTo->IsEmpty())
    {
        if (writeTo->IsOfType(m3d::cmn::XML_NODE_TEXT))
        {
            break;
        }
        writeTo->GetNextSibling_(writeTo, nullptr);
    }
    return !writeTo->IsEmpty();
}

bool XmlNodeImpl::HasChildOrAttribute() const
{
    throw std::logic_error("Not implemented");
}

bool XmlNodeImpl::AddBeforeChild(m3d::cmn::XmlNode const* addBefore, m3d::cmn::XmlNode* child)
{
    auto addBeforeCasted = dynamic_cast<XmlNodeImpl const*>(addBefore);
    auto childCasted = dynamic_cast<XmlNodeImpl*>(child);
    return m_node->LinkBeforeChild(addBeforeCasted->m_node, childCasted->m_node) != 0;
}

char const* XmlNodeImpl::GetValue() const
{
    return m_node->Value().c_str();
}

bool XmlNodeImpl::GetFirstAttribute(m3d::cmn::XmlAttrib* writeTo) const
{
    if (m_node && m_node->Type() == TiXmlNode::ELEMENT)
    {
        auto element = dynamic_cast<TiXmlElement*>(m_node);
        auto writeToCasted = dynamic_cast<XmlAttribImpl*>(writeTo);
        auto attr = element->FirstAttribute();
        writeToCasted->m_attrib = attr;
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

bool XmlNodeImpl::GetPrevSibling_(m3d::cmn::XmlNode* writeTo, char const* wantValue) const
{
    auto sibling = wantValue ? m_node->PreviousSibling(wantValue) : m_node->PreviousSibling();
    auto writeToCasted = dynamic_cast<XmlNodeImpl*>(writeTo);
    *writeToCasted = XmlNodeImpl(sibling);
    return sibling != nullptr;
}

bool XmlNodeImpl::IsOfType(m3d::cmn::XmlNodeType castTo) const
{
    if (m_node == nullptr)
    {
        return m_node != nullptr;
    }
    switch (castTo)
    {
    case m3d::cmn::XML_NODE_DOCUMENT: return m_node->Type() == TiXmlNode::DOCUMENT;
    case m3d::cmn::XML_NODE_ELEMENT: return m_node->Type() == TiXmlNode::ELEMENT;
    case m3d::cmn::XML_NODE_COMMENT: return m_node->Type() == TiXmlNode::COMMENT;
    case m3d::cmn::XML_NODE_UNKNOWN: return m_node->Type() == TiXmlNode::UNKNOWN;
    case m3d::cmn::XML_NODE_TEXT: return m_node->Type() == TiXmlNode::TEXT;
    case m3d::cmn::XML_NODE_DECLARATION: return m_node->Type() == TiXmlNode::DECLARATION;
    default: return m_node != nullptr;
    }
}

bool XmlNodeImpl::AddAfterChild(m3d::cmn::XmlNode const*, m3d::cmn::XmlNode*)
{
    throw std::logic_error("Not implemented");
}

void XmlNodeImpl::SetValue(char const*)
{
    throw std::logic_error("Not implemented");
}

bool XmlNodeImpl::GetNextSibling_(m3d::cmn::XmlNode* writeTo, char const* wantValue) const
{
    auto sibling = wantValue ? m_node->NextSibling(wantValue) : m_node->NextSibling();
    auto writeToCasted = dynamic_cast<XmlNodeImpl*>(writeTo);
    *writeToCasted = XmlNodeImpl(sibling);
    return sibling != nullptr;
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

bool XmlNodeImpl::GetFirstChild_(m3d::cmn::XmlNode* writeTo, char const* wantValue) const
{
    auto child = wantValue ? m_node->FirstChild(wantValue) : m_node->FirstChild();
    auto writeToCasted = dynamic_cast<XmlNodeImpl*>(writeTo);
    *writeToCasted = XmlNodeImpl(child);
    return child != nullptr;
}

XmlNodeImpl::~XmlNodeImpl()
{
    if (m_nodeOwned)
    {
        delete m_node;
    }
}

char const* XmlNodeImpl::GetAttribute(char const* name) const
{
    if (!IsOfType(m3d::cmn::XML_NODE_ELEMENT))
    {
        return nullptr;
    }
    auto element = dynamic_cast<TiXmlElement*>(m_node);
    auto attr = element->Attribute(name);
    return attr ? attr->c_str() : nullptr;
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

XmlNodeImpl::XmlNodeImpl(XmlNodeImpl const&)
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
    writeToCasted->m_attrib = attr;
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

XmlAttribImpl::XmlAttribImpl(XmlAttribImpl const&)
{
    throw std::logic_error("Not implemented");
}

XmlAttribImpl::XmlAttribImpl()
{
}

char const* XmlAttribImpl::GetValue()
{
    return m_attrib->Value().c_str();
}

char const* XmlAttribImpl::GetName()
{
    return m_attrib->Name().c_str();
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
