#include <stdexcept>
#include <core/ini.h>
#include <core/kernel.h>
#include <core/scoped_ptr.h>
#include <file/fileserver.h>
#include <file/filestream.h>

#include "math/vector.h"

#include <sstream>

namespace m3d
{
    cmn::XmlFile* ReadXmlFile(char const* filename, CStr* errorStr)
    {
        scoped_ptr fileStream = g_Kernel->GetFileServer().CreateFileStream();
        if (fileStream->Open(filename, fs::IStream::OPEN_READ))
        {
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

    int WriteXmlFile(char const* filename, cmn::XmlFile* xmlFile, CStr* errorStr)
    {
        // RVA 0x5AEAC0 - writes to a temp file first and only then replaces the target.
        scoped_ptr fileStream = g_Kernel->GetFileServer().CreateFileStream();
        char path[MAX_PATH];
        if (!GetTempFileNameA(".", "temp", 0, path))
        {
            *errorStr = CStr("WriteXmlFile: Cannot create temp file name, cant save cfg");
            return 0;
        }
        if (!fileStream->Open(path, fs::IStream::OPEN_WRITE))
        {
            *errorStr = CStr("WriteXmlFile: Cannot save config because cannot open temp file ") + CStr(path);
            return 0;
        }
        if (!xmlFile->Write(*fileStream))
        {
            *errorStr = CStr("WriteXmlFile: Error occured while saving config in ") + CStr(path);
            fileStream->Close();
            return 0;
        }
        fileStream->Close();
        if (GetFileAttributesA(filename) != INVALID_FILE_ATTRIBUTES && !DeleteFileA(filename))
        {
            *errorStr = CStr("WriteXmlFile: Could not delete original file ") + CStr(filename);
            return 0;
        }
        if (!MoveFileA(path, filename))
        {
            *errorStr = CStr("WriteXmlFile: Cannot rename temp file ") + CStr(path) + CStr(" to ") + CStr(filename);
            return 0;
        }
        return 1;
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
            clr = std::stoul(str.c_str(), nullptr, 16);
            return true;
        }
        return false;
    }

    bool SafeIntAttrib(int& v, m3d::cmn::XmlNode const* node, char const* attrib)
    {
        if (!node->IsEmpty())
        {
            auto attr = node->GetAttribute(attrib);
            if (attr)
            {
                v = atoi(attr);
                return true;
            }
        }
        return false;
    }

    bool SafeInt64Attrib(long long& v, m3d::cmn::XmlNode const* node, char const* attrib)
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
        if (!node->IsEmpty())
        {
            auto attr = node->GetAttribute(attrib);
            if (attr)
            {
                v = atof(attr);
                return true;
            }
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
        auto trueVal = !stricmp(attrValue, "yes") || !stricmp(attrValue, "yeah") || !stricmp(attrValue, "yep") ||
            !stricmp(attrValue, "1") || !stricmp(attrValue, "true");
        auto falseVal = !stricmp(attrValue, "false") || !stricmp(attrValue, "0") || !stricmp(attrValue, "no") ||
            !stricmp(attrValue, "nope") || !stricmp(attrValue, "none");
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

    bool SafeVectorAttrib(CVector& v, m3d::cmn::XmlNode const* node, char const* attrib)
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
        v = strToVec(val);
        return 1;
    }

    bool SafeQuaternionAttrib(Quaternion& v, m3d::cmn::XmlNode const* node, char const* attrib)
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
        v = strToQuat(val);
        return 1;
    }

    void Tokenize(CStr const& str, retruxx::vector<CStr>& tokens, char const* chars)
    {
        if (!str.empty())
        {
            tokens.clear();
            auto temp = new char[str.length() + 1];
            strncpy(temp, str.c_str(), str.length() + 1);
            for (auto i = strtok(temp, chars); i; i = strtok(nullptr, chars))
            {
                tokens.push_back(i);
            }
            delete[] temp;
        }
    }

    void Tokenize(CStr const* str, retruxx::vector<CStr>& tokens, char const* chars)
    {
        if (!str || str->empty())
        {
            return;
        }

        // Clear existing tokens
        tokens.clear();

        // Calculate string length
        auto strLen = str->length();

        // Allocate memory for copy
        char* buffer = new char[strLen + 1];

        // Copy the string
        std::strcpy(buffer, str->c_str());

        // Tokenize the copied string
        char* token = strtok(buffer, chars);
        while (token != nullptr)
        {
            tokens.push_back(token);
            token = strtok(nullptr, chars);
        }

        // Free the buffer
        delete[] buffer;
    }
}  // namespace m3d

namespace
{
    // Copies str into memory from the kernel allocator, so the caller frees it the engine's way.
    char* AllocStringCopy(CStr const& str, unsigned int allocSize)
    {
        auto* copy = static_cast<char*>(M3D_KERNEL->g_mar.AllocMem(allocSize, nullptr, 0));
        std::strcpy(copy, str.c_str());
        return copy;
    }
}  // namespace

bool XmlFileImpl::AddBeforeChild(m3d::cmn::XmlNode const* addBefore, m3d::cmn::XmlNode* child)
{
    // RVA 0x7493D0 - the document takes over the child's node.
    auto* childCasted = static_cast<XmlNodeImpl*>(child);
    childCasted->m_nodeOwned = false;
    return LinkBeforeChild(static_cast<XmlNodeImpl const*>(addBefore)->m_node, childCasted->m_node) != nullptr;
}

void XmlFileImpl::SetHeader(char const* version, char const* encoding, char const* isStandAlone)
{
    // RVA 0x749F50 - adds the default declaration first if the document has none. NOTE: in an
    // empty document the declaration cannot be inserted, and the null declaration is then written
    // through.
    TiXmlNode* declNode = GetDeclarationNode();
    if (!declNode)
    {
        TiXmlDeclaration const decl("1.0", "windows-1251", "yes");
        InsertBeforeChild(FirstChild(), decl);
        declNode = GetDeclarationNode();
    }

    TiXmlDeclaration* decl = declNode->ToDeclaration();
    if (version)
    {
        decl->SetVersion(version);
    }
    if (encoding)
    {
        decl->SetEncoding(encoding);
    }
    if (isStandAlone)
    {
        decl->SetStandalone(isStandAlone);
    }
}

bool XmlFileImpl::AddAfterChild(m3d::cmn::XmlNode const* addAfter, m3d::cmn::XmlNode* child)
{
    // RVA 0x749400 - the document takes over the child's node.
    auto* childCasted = static_cast<XmlNodeImpl*>(child);
    childCasted->m_nodeOwned = false;
    return LinkAfterChild(static_cast<XmlNodeImpl const*>(addAfter)->m_node, childCasted->m_node) != nullptr;
}

int XmlFileImpl::Write(m3d::fs::IStream& out)
{
    // RVA 0x749E30 - every written file gets a declaration. The shipped TinyXML streams straight
    // into the IStream; this one streams into a string that is then written out.
    if (!GetDeclarationNode())
    {
        TiXmlDeclaration const decl("1.0", "windows-1251", "yes");
        InsertBeforeChild(FirstChild(), decl);
    }

    std::ostringstream stream;
    StreamOut(&stream);
    std::string const text = stream.str();
    out.WriteBytes(text.data(), static_cast<unsigned int>(text.size()));
    return !Error();
}

void XmlFileImpl::GetHeader(char** version, char** encoding, char** isStandAlone)
{
    // RVA 0x74AA90 - like SetHeader, adds the default declaration first if there is none. Each
    // requested field is returned as a copy the caller frees; an empty field leaves its output
    // untouched.
    TiXmlNode* declNode = GetDeclarationNode();
    if (!declNode)
    {
        TiXmlDeclaration const decl("1.0", "windows-1251", "yes");
        InsertBeforeChild(FirstChild(), decl);
        declNode = GetDeclarationNode();
    }

    TiXmlDeclaration const* decl = declNode->ToDeclaration();
    if (version && !decl->Version().empty())
    {
        CStr const str(decl->Version().c_str());
        *version = AllocStringCopy(str, str.length() + 1);
    }
    if (encoding && !decl->Encoding().empty())
    {
        CStr const str(decl->Encoding().c_str());
        *encoding = AllocStringCopy(str, str.length() + 1);
    }
    if (isStandAlone && !decl->Standalone().empty())
    {
        CStr const str(decl->Standalone().c_str());
        *isStandAlone = AllocStringCopy(str, str.length() + 1);
    }
}

bool XmlFileImpl::AddChild(m3d::cmn::XmlNode* child)
{
    // RVA 0x7493B0 - the document takes over the child's node.
    auto* childCasted = static_cast<XmlNodeImpl*>(child);
    childCasted->m_nodeOwned = false;
    return LinkEndChild(childCasted->m_node) != nullptr;
}

bool XmlFileImpl::RemoveChild(m3d::cmn::XmlNode* child)
{
    // RVA 0x749430 - NOTE: TinyXML deletes the removed node, yet the wrapper is marked as its
    // owner again, so releasing the wrapper deletes it a second time.
    auto* childCasted = static_cast<XmlNodeImpl*>(child);
    childCasted->m_nodeOwned = true;
    return TiXmlDocument::RemoveChild(childCasted->m_node);
}

char const* XmlFileImpl::GetError()
{
    if (Error())
    {
        return ErrorDesc().c_str();
    }
    return nullptr;
}

bool XmlFileImpl::GetFirstChild(m3d::cmn::XmlNode* writeTo, char const* wantValue) const
{
    auto child = wantValue ? FirstChild(wantValue) : FirstChild();
    auto writeToCasted = dynamic_cast<XmlNodeImpl*>(writeTo);
    writeToCasted->m_node = child;
    return child != nullptr;
}

XmlFileImpl::~XmlFileImpl()
{
}

int XmlFileImpl::Read(m3d::fs::IStream& in)
{
    auto const size = in.GetSize();
    retruxx::vector<char> buffer(size + 1, 0);
    buffer[in.ReadBytes(&buffer[0], size)] = '\0';
    // RVA 0x749140
    Parse(&buffer[0]);
    return !Error();
}

m3d::cmn::XmlNode* XmlFileImpl::CreateNode(m3d::cmn::XmlNodeType type, char const* nodeName) const
{
    // RVA 0x74AA20 - an empty node is only a cursor for walking the tree and never owns what it
    // points at; element, comment, unknown and text nodes are created owning a new node. Documents
    // and declarations cannot be created.
    if (type == m3d::cmn::XML_NODE_EMPTY)
    {
        return new XmlNodeImpl();
    }
    if (type > m3d::cmn::XML_NODE_DOCUMENT && type <= m3d::cmn::XML_NODE_TEXT)
    {
        return new XmlNodeImpl(type, nodeName);
    }
    return nullptr;
}

bool XmlFileImpl::GetLastChild_(m3d::cmn::XmlNode* node, char const* nodeName) const
{
    // RVA 0x74A820
    TiXmlNode* child = nodeName ? LastChild(nodeName) : LastChild();
    static_cast<XmlNodeImpl*>(node)->m_node = child;
    return child != nullptr;
}

int XmlFileImpl::IncRef()
{
    if (m_parent)
    {
        m_parent->IncRef();
    }
    return ++m_refCount;
}

void* XmlFileImpl::QueryIface(char const* ifaceName)
{
    // RVA 0x748FC0
    return m_parent ? m_parent->QueryIface(ifaceName) : nullptr;
}

XmlFileImpl::XmlFileImpl()
{
    this->m_parent = 0;
    this->m_refCount = 0;
    SetCondenseWhiteSpace(0);
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
        delete this;
    }
    return ret;
}

TiXmlNode* XmlFileImpl::GetDeclarationNode()
{
    // RVA 0x7491A0
    TiXmlNode* node = FirstChild();
    while (node && node->Type() != TiXmlNode::DECLARATION)
    {
        node = node->NextSibling();
    }
    return node;
}

IniFileImpl::IniFileImpl() : m_file(m3d::g_Kernel->CreateXmlFile())
{
}

int IniFileImpl::Write(m3d::fs::IStream& out)
{
    // RVA 0x749460
    return m_file->Write(out);
}

char const* IniFileImpl::GetError()
{
    return m_file->GetError();
}

unsigned IniFileImpl::GetHex(CStr const& section, CStr const& key)
{
    // RVA 0x7490E0 - NOTE: the original scans into the stack slot of its key argument, so a value
    // that does not parse returns the key's address; here it returns 0.
    unsigned int value = 0;
    sscanf(GetString(section, key), "%x", &value);
    return value;
}

void IniFileImpl::SetString(CStr const& section, CStr const& key, CStr const& value)
{
    // RVA 0x749C10 - replaces the key's text, or adds the key (and its section) if missing.
    ref_ptr node = m_file->CreateNode(m3d::cmn::XML_NODE_EMPTY, nullptr);
    if (FindKey(node, section, key))
    {
        node->SetValue(value.c_str());
    }
    else
    {
        ref_ptr newKey = m_file->CreateNode(m3d::cmn::XML_NODE_EMPTY, nullptr);
        AddKey(newKey, section, key, value);
    }
}

int IniFileImpl::GetInteger(CStr const& section, CStr const& key)
{
    return atoi(GetString(section, key));
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

void IniFileImpl::SetInteger(CStr const& section, CStr const& key, int value)
{
    // RVA 0x749CF0
    SetString(section, key, CStr(value));
}

void IniFileImpl::SetFloat(CStr const& section, CStr const& key, float value)
{
    // RVA 0x749D40
    SetString(section, key, CStr(value));
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
    if (m_file->GetFirstChild(node, "Ini"))
    {
        node->GetFirstChild(writeTo, "Section");
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
            writeTo->GetNextSibling(writeTo, "Section");
        }
    }
    return !writeTo->IsEmpty();
}

bool IniFileImpl::AddKey(m3d::cmn::XmlNode* writeTo, CStr const& section, CStr const& key, CStr const& value)
{
    // RVA 0x749870 - builds <Ini><Section name=""><Key name="">value</Key></Section></Ini> as far
    // as it is missing; writeTo ends up on the new key.
    ref_ptr iniNode = m_file->CreateNode(m3d::cmn::XML_NODE_EMPTY, nullptr);
    if (!m_file->GetFirstChild(iniNode, "Ini"))
    {
        ref_ptr newIniNode = m_file->CreateNode(m3d::cmn::XML_NODE_ELEMENT, "Ini");
        m_file->AddChild(newIniNode);
        m_file->GetLastChild_(iniNode, nullptr);
    }

    if (!FindSection(writeTo, section))
    {
        ref_ptr sectionNode = m_file->CreateNode(m3d::cmn::XML_NODE_ELEMENT, "Section");
        sectionNode->SetAttribute("name", section.c_str());
        iniNode->AddChild(sectionNode);
        iniNode->GetLastChild(writeTo, nullptr);
    }

    ref_ptr keyNode = m_file->CreateNode(m3d::cmn::XML_NODE_ELEMENT, "Key");
    keyNode->SetAttribute("name", key.c_str());
    writeTo->AddChild(keyNode);
    writeTo->GetLastChild(writeTo, nullptr);

    ref_ptr valueNode = m_file->CreateNode(m3d::cmn::XML_NODE_TEXT, value.c_str());
    writeTo->AddChild(valueNode);
    return !writeTo->IsEmpty();
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
        delete this;
    }
    return ret;
}

void* IniFileImpl::QueryIface(char const* ifaceName)
{
    // RVA 0x748F50
    return m_parent ? m_parent->QueryIface(ifaceName) : nullptr;
}

bool IniFileImpl::FindKey(m3d::cmn::XmlNode* writeTo, CStr const& section, CStr const& key)
{
    if (!FindSection(writeTo, section))
    {
        return false;
    }
    writeTo->GetFirstChild(writeTo, "Key");
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
        writeTo->GetNextSibling(writeTo, "Key");
    }
    if (writeTo->IsEmpty())
    {
        return false;
    }
    writeTo->GetFirstChild(writeTo, nullptr);
    while (!writeTo->IsEmpty())
    {
        if (writeTo->IsOfType(m3d::cmn::XML_NODE_TEXT))
        {
            break;
        }
        writeTo->GetNextSibling(writeTo, nullptr);
    }
    return !writeTo->IsEmpty();
}

bool XmlNodeImpl::HasChildOrAttribute() const
{
    // RVA 0x749360 - NOTE: the attributes are read from the node as an element without checking
    // that it is one, so a non-element node is dereferenced as a null element.
    if (IsEmpty())
    {
        return false;
    }
    TiXmlElement const* element = m_node->ToElement();
    return element->FirstAttribute() != nullptr || m_node->FirstChild() != nullptr;
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

bool XmlNodeImpl::RemoveAttribute(char const* name)
{
    // RVA 0x74A740 - succeeds on any element, whether or not it had the attribute.
    if (!IsOfType(m3d::cmn::XML_NODE_ELEMENT))
    {
        return false;
    }
    m_node->ToElement()->RemoveAttribute(name);
    return true;
}

bool XmlNodeImpl::IsEmpty() const
{
    return m_node == nullptr;
}

XmlNodeImpl::XmlNodeImpl(m3d::cmn::XmlNodeType type, char const* nodeName)
{
    // RVA 0x74A170
    switch (type)
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
        // NOTE: the original marks the (missing) node as owned here too.
        m_nodeOwned = true;
        break;
    }
    }
}

XmlNodeImpl::XmlNodeImpl(TiXmlNode* fromNode) : m_node(fromNode)
{
}

bool XmlNodeImpl::GetPrevSibling(m3d::cmn::XmlNode* writeTo, char const* wantValue) const
{
    // RVA 0x74A3B0 - only the target's node pointer changes.
    auto sibling = wantValue ? m_node->PreviousSibling(wantValue) : m_node->PreviousSibling();
    auto writeToCasted = dynamic_cast<XmlNodeImpl*>(writeTo);
    writeToCasted->m_node = sibling;
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
    case m3d::cmn::XML_NODE_DOCUMENT:
        return m_node->Type() == TiXmlNode::DOCUMENT;
    case m3d::cmn::XML_NODE_ELEMENT:
        return m_node->Type() == TiXmlNode::ELEMENT;
    case m3d::cmn::XML_NODE_COMMENT:
        return m_node->Type() == TiXmlNode::COMMENT;
    case m3d::cmn::XML_NODE_UNKNOWN:
        return m_node->Type() == TiXmlNode::UNKNOWN;
    case m3d::cmn::XML_NODE_TEXT:
        return m_node->Type() == TiXmlNode::TEXT;
    case m3d::cmn::XML_NODE_DECLARATION:
        return m_node->Type() == TiXmlNode::DECLARATION;
    default:
        return m_node != nullptr;
    }
}

bool XmlNodeImpl::AddAfterChild(m3d::cmn::XmlNode const* addAfter, m3d::cmn::XmlNode* child)
{
    // RVA 0x749300 - the tree takes over the child's node.
    auto* childCasted = static_cast<XmlNodeImpl*>(child);
    childCasted->m_nodeOwned = false;
    return m_node->LinkAfterChild(static_cast<XmlNodeImpl const*>(addAfter)->m_node, childCasted->m_node) != nullptr;
}

void XmlNodeImpl::SetValue(char const* v)
{
    // RVA 0x74A510
    m_node->SetValue(v);
}

bool XmlNodeImpl::GetNextSibling(m3d::cmn::XmlNode* writeTo, char const* wantValue) const
{
    auto sibling = wantValue ? m_node->NextSibling(wantValue) : m_node->NextSibling();
    auto writeToCasted = dynamic_cast<XmlNodeImpl*>(writeTo);
    writeToCasted->m_node = sibling;
    return sibling != nullptr;
}

bool XmlNodeImpl::GetParent(m3d::cmn::XmlNode* writeTo) const
{
    // RVA 0x749250
    TiXmlNode* parent = m_node->Parent();
    static_cast<XmlNodeImpl*>(writeTo)->m_node = parent;
    return parent != nullptr;
}

void XmlNodeImpl::GetAttributeMbcsSafe(char const* name, char** str, int* strSz) const
{
    // RVA 0x74A5E0 - returns a copy of the attribute the caller frees. NOTE: the original allocates
    // only strlen bytes and then writes the terminator one past the end; the allocation here has
    // room for it, while the reported size is still strlen.
    CStr const cstr(GetAttribute(name));
    *strSz = cstr.length();
    *str = AllocStringCopy(cstr, cstr.length() + 1);
}

bool XmlNodeImpl::AddChild(m3d::cmn::XmlNode* child)
{
    // RVA 0x7492B0 - the tree takes over the child's node.
    auto* childCasted = static_cast<XmlNodeImpl*>(child);
    childCasted->m_nodeOwned = false;
    return m_node->LinkEndChild(childCasted->m_node) != nullptr;
}

bool XmlNodeImpl::SetAttribute(char const* name, char const* value)
{
    // RVA 0x74A660 - succeeds when the attribute is there afterwards.
    if (!IsOfType(m3d::cmn::XML_NODE_ELEMENT))
    {
        return false;
    }
    TiXmlElement* element = m_node->ToElement();
    element->SetAttribute(name, value);
    return element->Attribute(name) != nullptr;
}

bool XmlNodeImpl::RemoveChild(m3d::cmn::XmlNode* child)
{
    // RVA 0x749330 - NOTE: TinyXML deletes the removed node, yet the wrapper is marked as its owner
    // again, so releasing the wrapper deletes it a second time.
    auto* childCasted = static_cast<XmlNodeImpl*>(child);
    childCasted->m_nodeOwned = true;
    return m_node->RemoveChild(childCasted->m_node);
}

m3d::cmn::XmlAttrib* XmlNodeImpl::CreateAttribute() const
{
    return new XmlAttribImpl;
}

bool XmlNodeImpl::GetFirstChild(m3d::cmn::XmlNode* writeTo, char const* wantValue) const
{
    auto child = wantValue ? m_node->FirstChild(wantValue) : m_node->FirstChild();
    auto writeToCasted = dynamic_cast<XmlNodeImpl*>(writeTo);
    writeToCasted->m_node = child;
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

bool XmlNodeImpl::GetLastChild(m3d::cmn::XmlNode* writeTo, char const* wantValue) const
{
    // RVA 0x74A490
    TiXmlNode* child = wantValue ? m_node->LastChild(wantValue) : m_node->LastChild();
    static_cast<XmlNodeImpl*>(writeTo)->m_node = child;
    return child != nullptr;
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
    // RVA 0x74ADB0
}

void* XmlNodeImpl::QueryIface(char const* ifaceName)
{
    // RVA 0x749030
    return m_parent ? m_parent->QueryIface(ifaceName) : nullptr;
}

XmlNodeImpl::XmlNodeImpl(XmlNodeImpl const& other) : m_node(other.m_node), m_nodeOwned(false)
{
    // NOTE: declared in the PDB but never instantiated in the shipped binary, so this body does not
    // come from it. The copy refers to the same node without owning it.
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
        delete this;
    }
    return ret;
}

bool XmlAttribImpl::GetNextSibling(m3d::cmn::XmlAttrib* writeTo)
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

XmlAttribImpl::XmlAttribImpl(XmlAttribImpl const& other) : m_attrib(other.m_attrib), m_attribOwned(false)
{
    // NOTE: declared in the PDB but never instantiated in the shipped binary, so this body does not
    // come from it. The copy refers to the same attribute without owning it.
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
        delete this;
    }
    return ret;
}

void* XmlAttribImpl::QueryIface(char const* ifaceName)
{
    // RVA 0x7490A0
    return m_parent ? m_parent->QueryIface(ifaceName) : nullptr;
}
