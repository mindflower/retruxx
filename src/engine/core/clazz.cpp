#include <cassert>
#include <core/clazz.h>
#include <core/kernel.h>
#include <core/log.h>
#include <core/ref_ptr.h>

namespace m3d
{
    int RefCountedBase::IncRef()
    {
        return ++m_refCount;
    }

    int RefCountedBase::DecRef()
    {
        auto const result = --m_refCount;
        if (result <=0)
        {
            delete this;
        }
        return result;
    }

    int RefCountedBase::GetRefCount() const
    {
        return m_refCount;
    }

    Class* Object::GetBaseClass()
    {
        return nullptr;
    }

    Object* Object::CreateObject()
    {
        return new Object{};
    }

    Object::Object() :
        m_name("Object" + std::to_string(g_Kernel->GetUniqueId()))
    {
    }

    Object::Object(Object const& clazz) :
        m_name(clazz.m_name),
        m_persistant(clazz.m_persistant),
        m_isChildDirty(clazz.m_isChildDirty)
    {
        for (auto* it = GetFirstNestling(); it; it = it->GetNextRelative())
        {
            AddChild(it->Clone());
        }
    }

    bool Class::IsKindOf(char const* className) const
    {
        if (className == nullptr)
        {
            return false;
        }
        auto* clazz = this;
        while (CStr(clazz->m_className) != className)
        {
            clazz = m_fnGetBaseClass();
            if (clazz == nullptr)
            {
                return false;
            }
        }
        return true;
    }

    bool Class::IsKindOf(Class const* clazz) const
    {
        if (clazz == nullptr)
        {
            return false;
        }
        auto* thisClazz = this;
        while (thisClazz != clazz)
        {
            thisClazz = m_fnGetBaseClass();
            if (thisClazz == nullptr)
            {
                return false;
            }
        }
        return true;
    }

    Object* Class::NewInstance() const
    {
        if (m_fnCreateObject != nullptr)
        {
            return m_fnCreateObject();
        }
        //TODO: replace to macro
        g_Kernel->SysError("m_fnCreateObject", "here");
        return nullptr;
    }

    int Object::AddChild(Object* node)
    {
        assert(node != nullptr);
        if (m_lastChild != nullptr)
        {
            node->m_prevSibling = m_lastChild;
            m_lastChild->m_nextSibling = node;
        }
        else
        {
            node->m_prevSibling = nullptr;
            m_firstChild = node;
        }
        m_lastChild = node;
        node->m_parent = this;
        node->m_nextSibling = nullptr;
        ++m_numChildren;
        for (auto* it = m_parent; it; it = it->m_parent)
        {
            if (it->m_isChildDirty)
            {
                break;
            }
            it->m_isChildDirty = true;
        }
        return 1;
    }

    int Object::RemoveChild(Object* node)
    {
        assert(node != nullptr);
        UnlinkChild(node);
        return 1;
    }

    Object* Object::Clone()
    {
        return new Object(*this);
    }

    Class* Object::GetClass() const
    {
        return RT_CLASS_LOCAL(Object);
    }

    char const* Object::GetClassNameA() const
    {
        return GetClass()->m_className;
    }

    int Object::GetProperty(unsigned int propId, void* prop) const
    {
        return 0;
    }

    int Object::GetPropertiesList(std::set<size_t>& properties) const
    {
        return 1;
    }

    int Object::SetProperty(unsigned int propId, void* prop)
    {
        return 0;
    }

    int Object::IncWeakRef()
    {
        return 0;
    }

    int Object::DecWeakRef()
    {
        return 0;
    }

    int Object::GetWeakRefCount()
    {
        return 0;
    }

    int Object::ReadFromXmlNode(cmn::XmlFile* file, cmn::XmlNode* node)
    {
        CStr nameAttr(node->GetAttribute("name"));
        if (m_parent != nullptr)
        {
            auto* child = m_parent->GetChildByName(m_name);
            if (child != nullptr && this != child)
            {
                M3D_LOG_INFO("ReadFromXmlNode: name = " + m_name + " conflicts with another child");
                return 0;
            }
            else
            {
                ref_ptr newNode = file->CreateNode(cmn::XmlNodeType::XML_NODE_EMPTY, nullptr);
                for (node->GetFirstChild_(newNode, "Node"); !newNode->IsEmpty(); newNode->GetNextSibling_(newNode, "Node"))
                {
                    if (!ChildNodeFromXmlNode(file, newNode))
                    {
                        return 0;
                    }
                }
                return 1;
            }
        }
        return 0;
    }

    int Object::ReadFromXmlNodeAfterAdd(cmn::XmlFile* file, cmn::XmlNode* mode)
    {
        return 1;
    }

    int Object::WriteToXmlNode(cmn::XmlFile* file, cmn::XmlNode* writeTo)
    {
        if (m_name.empty())
        {
            M3D_LOG_INFO("Warning: node have no name, added default");
            m_name = "Node";
            m_name += g_Kernel->GetUniqueId();
        }
        writeTo->SetAttribute("name", m_name.c_str());
        writeTo->SetAttribute("class", GetClassNameA());
        for (auto* it = GetFirstNestling(); it != nullptr; it = it->GetNextRelative())
        {
            //TODO: magic number
            it->GetProperty(4360, &writeTo);
            if (it->m_persistant && writeTo != reinterpret_cast<cmn::XmlNode*>(-1))
            {
                ref_ptr node = file->CreateNode(cmn::XmlNodeType::XML_NODE_ELEMENT, "Node");
                it->WriteToXmlNode(file, node);
            }
        }
        return true;
    }

    Object* Object::GetFirstNestling() const
    {
        return m_firstChild;
    }

    Object* Object::GetLastNestling() const
    {
        return m_lastChild;
    }

    Object* Object::GetChildByName(CStr const& str) const
    {
        if (m_firstChild == nullptr)
        {
            return nullptr;
        }
        //TODO: check correctness
        for (auto* it = m_firstChild; it; it = it->m_nextSibling)
        {
            if (it->m_name == str)
            {
                return it;
            }
        }
        return nullptr;
    }

    Object* Object::GetNextRelative() const
    {
        return m_nextSibling;
    }

    Object* Object::GetParent() const
    {
        return m_parent;
    }

    int Object::LinkChildAtHead(Object* node)
    {
        if (m_firstChild != nullptr)
        {
            node->m_nextSibling = m_firstChild;
            m_firstChild->m_prevSibling = node;
        }
        else
        {
            node->m_nextSibling = nullptr;
            m_lastChild = node;
        }
        m_firstChild = node;
        node->m_prevSibling = nullptr;
        node->m_parent = this;
        return ++m_numChildren;
    }

    Object* Object::ChildNodeFromXmlNode(cmn::XmlFile* xmlFile, cmn::XmlNode* xmlNode)
    {
        CStr const name(xmlNode->GetAttribute("name"));
        for (auto* it = GetFirstNestling(); it; it = GetNextRelative())
        {
            if (it->GetName() == name)
            {
                M3D_LOG_INFO("ChildNodeFromXmlNode: node w/ same name already exists as a child: ",);
                return nullptr;
            }
        }

        Object* res = nullptr;
        auto const* externAttr = xmlNode->GetAttribute("extern");
        if (externAttr != nullptr)
        {
            res = ChildNodeFromXmlFile(externAttr);
            if (res == nullptr)
            {
                return nullptr;
            }
        }

        bool isNewObject = false;
        if (res == nullptr)
        {
            auto const* classAttr = xmlNode->GetAttribute("class");
            res = g_Kernel->New(classAttr);
            if (res == nullptr)
            {
                M3D_LOG_INFO(CStr("ChildNodeFromXmlNode: Cannot load node: name = ") + classAttr);
                return nullptr;
            }
            isNewObject = true;
        }
        if(res->ReadFromXmlNode(xmlFile, xmlNode))
        {
            if (isNewObject)
            {
                AddChild(res);
            }
            res->ReadFromXmlNodeAfterAdd(xmlFile, xmlNode);
        }
        else if (isNewObject)
        {
            delete res;
            res = nullptr;
        }
        if (res != nullptr)
        {
            return res;
        }
        M3D_LOG_INFO(CStr("ChildNodeFromXmlNode: Cannot load node: name = ") + classAttr);
        return nullptr;
    }

    int Object::LinkChildAtTail(Object* node)
    {
        if (m_lastChild != nullptr)
        {
            node->m_prevSibling = m_lastChild;
            m_lastChild->m_nextSibling = node;
        }
        else
        {
            node->m_prevSibling = nullptr;
            m_firstChild = node;
        }
        m_lastChild = node;
        node->m_nextSibling = nullptr;
        node->m_parent = this;
        return ++m_numChildren;
    }

    int Object::UnlinkChild(Object* node)
    {
        if (m_firstChild != node)
        {
            if (m_lastChild == node)
            {
                m_prevSibling->m_nextSibling = nullptr;
                m_lastChild = m_lastChild->m_prevSibling;
            }
            else
            {
                node->m_nextSibling->m_prevSibling = node->m_prevSibling;
                node->m_prevSibling->m_nextSibling = node->m_nextSibling;
            }
            node->m_parent = nullptr;
            node->m_nextSibling = nullptr;
            node->m_prevSibling = nullptr;
            return --m_numChildren;
        }
        if (m_lastChild == node)
        {
            m_firstChild = nullptr;
            m_lastChild = nullptr;
            node->m_parent = nullptr;
            node->m_nextSibling = nullptr;
            node->m_prevSibling = nullptr;
            return --m_numChildren;
        }
        m_nextSibling->m_prevSibling = nullptr;
        m_firstChild = m_firstChild->m_nextSibling;
        node->m_parent = nullptr;
        node->m_nextSibling = nullptr;
        node->m_prevSibling = nullptr;
        return --m_numChildren;
    }

    void Object::MoveChildToFirstPosition(Object* wnd)
    {
        assert(wnd != nullptr);
        assert(wnd->GetParent() != nullptr);
        assert(IsDirectChild(wnd));
        if (m_firstChild != wnd)
        {
            UnlinkChild(wnd);
            if (m_firstChild)
            {
                wnd->m_nextSibling = m_firstChild;
                m_firstChild->m_prevSibling = wnd;
            }
            else
            {
                wnd->m_nextSibling = nullptr;
                m_lastChild = wnd;
            }
            m_firstChild = wnd;
            wnd->m_prevSibling = nullptr;
            wnd->m_parent = this;
            ++m_numChildren;
        }
    }

    void Object::MoveChildToLastPosition(Object* wnd)
    {
        assert(wnd != nullptr);
        assert(wnd->GetParent() != nullptr);
        assert(IsDirectChild(wnd));
        if (this->m_lastChild != wnd)
        {
            UnlinkChild(wnd);
            if (m_lastChild)
            {
                wnd->m_prevSibling = m_lastChild;
                m_lastChild->m_nextSibling = wnd;
            }
            else
            {
                wnd->m_prevSibling = nullptr;
                m_firstChild = wnd;
            }
            m_lastChild = wnd;
            wnd->m_nextSibling = nullptr;
            wnd->m_parent = this;
            ++m_numChildren;
        }
    }

    int Object::RemoveAllChildren()
    {
        while (auto* child = m_firstChild)
        {
            RemoveChild(child);
            child->DecRef();
        }
        m_firstChild = nullptr;
        return 1;
    }

    bool Object::GetChildDirty() const
    {
        return m_isChildDirty;
    }

    char const* Object::GetName() const
    {
        return m_name.c_str();
    }

    Object* Object::ChildNodeFromXmlFile(char const* extStr)
    {
        CStr const ext(extStr);
        auto const pos = ext.find(':');
        if (pos == CStr::npos)
        {
            M3D_LOG_INFO("ChildNodeFromXmlFile: invalid 'extern' format : " + ext);
            return nullptr;
        }

        auto const filename = ext.substr(0, pos);
        auto const internalName = ext.substr(pos + 1);

        CStr error;
        ref_ptr xmlFile = ReadXmlFile(filename.c_str(), &error);
        if (!xmlFile)
        {
            M3D_LOG_INFO("ChildNodeFromXmlFile: " + error);
            return nullptr;
        }

        ref_ptr xmlNode = xmlFile->CreateNode(cmn::XML_NODE_EMPTY, nullptr);
        xmlFile->GetFirstChild_(xmlNode, "Prefabs");
        if (xmlNode->IsEmpty())
        {
            M3D_LOG_INFO("ChildNodeFromXmlFile: Prefabs node should be root node in " + ext);
            return nullptr;
        }
        xmlNode->GetFirstChild_(xmlNode, "Node");
        while (!xmlNode->IsEmpty())
        {
            if (xmlNode->GetAttribute("name") == internalName)
            {
                break;
            }
            xmlNode->GetNextSibling_(xmlNode, "Node");
        }
        if (xmlNode->IsEmpty())
        {
            M3D_LOG_INFO("ChildNodeFromXmlFile: cannot find " + internalName);
            return nullptr;
        }
        return ChildNodeFromXmlNode(xmlFile, xmlNode);
    }

    void Object::SetChildDirty(bool d)
    {
        m_isChildDirty = d;
    }

    void Object::SetName(CStr const& str)
    {
        m_name = str;
    }

    void Object::SetPersistance(bool per)
    {
        m_persistant = per;
    }

    bool Object::IsChildOf(Object const* wnd) const
    {
        assert(wnd);
        for (auto* it = m_parent; it != nullptr; it = it->m_parent)
        {
            if (it == wnd)
            {
                return true;
            }
        }
        return false;
    }

    bool Object::IsDirectChild(Object const* wnd) const
    {
        assert(wnd);
        for (auto* it = this->m_firstChild; it != nullptr; it = it->m_nextSibling)
        {
            if (it == wnd)
            {
                return true;
            }
        }
        return false;
    }

    bool Object::IsKindOf(char const* className) const
    {
        return GetClass()->IsKindOf(className);
    }

    bool Object::IsKindOf(Class const* object) const
    {
        return GetClass()->IsKindOf(object);
    }
}
