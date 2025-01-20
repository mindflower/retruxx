#include <cassert>
#include <stdexcept>
#include <core/clazz.h>
#include "core/ini.h"
#include <core/kernel.h>
#include <core/log.h>
#include <core/ref_ptr.h>

#undef GetClassName

RT_CLASS_EXPORT_METHOD_DEFINE(Object, GetName)
{
    auto obj = context->asObject(0, "Object");
    context->pushString(obj->GetName());
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Object, GetRefCount)
{
    auto obj = context->asObject(0, "Object");
    context->pushInt(obj->GetRefCount());
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Object, GetClassName)
{
    auto obj = context->asObject(0, "Object");
    context->pushString(obj->GetClassNameA());
    return 1;
}

RT_CLASS_EXPORT_METHOD_DEFINE(Object, IsKindOf)
{
    auto obj = context->asObject(0, "Object");
    auto clsName = context->asString(1);
    auto cls = obj->GetClass();
    auto res = cls->IsKindOf(clsName);
    context->pushBool(res);
    return 1;
}

namespace m3d
{
    RT_CLASS_EXPORTS_BEGIN(Object)
        RT_CLASS_EXPORT(Object, METHOD, GetName, "const char*", "", "")
        RT_CLASS_EXPORT(Object, METHOD, GetRefCount, "int", "", "return the number of references held against this instance")
        RT_CLASS_EXPORT(Object, METHOD, GetClassName, "void", "", "get object's class name")
        RT_CLASS_EXPORT(Object, METHOD, IsKindOf, "bool", "const char*", "finds out if objects is of given class")
	RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(Object);
}

namespace m3d
{
    RETRUXX_DLL_INJECT_CTOR(0x00616270, RefCountedBase)
    RefCountedBase::RefCountedBase() :
        m_refCount(0)
    {
    }

    RETRUXX_DLL_INJECT_FUNCTION(0x00437010, RefCountedBase::IncRef)
    int RefCountedBase::IncRef()
    {
        return ++m_refCount;
    }

    RETRUXX_DLL_INJECT_FUNCTION(0x004047B0, RefCountedBase::DecRef)
    int RefCountedBase::DecRef()
    {
        const auto result = --m_refCount;
        if (result <=0)
        {
            delete this;
        }
        return result;
    }

    RETRUXX_DLL_INJECT_FUNCTION(0x00435580, RefCountedBase::GetRefCount)
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
        m_name("Object" + CStr(M3D_KERNEL->GetUniqueId()))
    {
    }

    Object::Object(Object const& clazz) :
        m_name(clazz.m_name),
        m_persistant(clazz.m_persistant),
        m_isChildDirty(clazz.m_isChildDirty)
    {
        for (auto* it = GetFirstChild(); it; it = it->GetNextSibling())
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
            clazz = clazz->m_fnGetBaseClass();
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
            thisClazz = thisClazz->m_fnGetBaseClass();
            if (thisClazz == nullptr)
            {
                return false;
            }
        }
        return true;
    }

    Object* Class::NewInstance() const
    {
        M3D_ASSERT(m_fnCreateObject);
        return m_fnCreateObject();
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

    int Object::GetPropertiesList(retruxx::set<size_t>& properties) const
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
        m_name = node->GetAttribute("name");
        if (m_parent != nullptr)
        {
            auto* child = m_parent->GetChildByName(m_name);
            if (child != nullptr && this != child)
            {
                M3D_LOG_INFO("ReadFromXmlNode: name = " + m_name + " conflicts with another child");
                return 0;
            }
        }
        ref_ptr newNode = file->CreateNode(cmn::XmlNodeType::XML_NODE_EMPTY, nullptr);
        for (node->GetFirstChild(newNode, "Node"); !newNode->IsEmpty(); newNode->GetNextSibling(newNode, "Node"))
        {
            if (!ChildNodeFromXmlNode(file, newNode))
            {
                return 0;
            }
        }
        return 1;
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
            m_name += CStr(g_Kernel->GetUniqueId());
        }
        writeTo->SetAttribute("name", m_name.c_str());
        writeTo->SetAttribute("class", GetClassNameA());
        for (auto* it = GetFirstChild(); it != nullptr; it = it->GetNextSibling())
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

    Object* Object::GetFirstChild() const
    {
        return m_firstChild;
    }

    Object* Object::GetLastChild() const
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

    Object* Object::GetNextSibling() const
    {
        return m_nextSibling;
    }

    Object* Object::GetPrevSibling() const
    {
        throw std::logic_error("Not implemented");
    }

    int Object::GetNumChildren() const
    {
        throw std::logic_error("Not implemented");
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
        for (auto* it = GetFirstChild(); it; it = GetNextSibling())
        {
            if (it->GetName() == name)
            {
                M3D_LOG_INFO("ChildNodeFromXmlNode: node w/ same name already exists as a child: ");
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
        M3D_LOG_INFO(CStr("ChildNodeFromXmlNode: Cannot load node: name = ") + name);
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
        //TODO: check this and refactor!!!!!!!!!!!
        m3d::Object* v2; // edx
        int result; // eax
        m3d::Object* v4; // edx

        v2 = this->m_firstChild;
        if (v2 != node)
        {
            v4 = this->m_lastChild;
            if (v4 == node)
            {
                v4->m_prevSibling->m_nextSibling = 0;
                this->m_lastChild = this->m_lastChild->m_prevSibling;
            }
            else
            {
                node->m_nextSibling->m_prevSibling = node->m_prevSibling;
                node->m_prevSibling->m_nextSibling = node->m_nextSibling;
            }
            node->m_parent = 0;
            node->m_nextSibling = 0;
            node->m_prevSibling = 0;
            result = --this->m_numChildren;
            return result;
        }
        if (this->m_lastChild == node)
        {
            this->m_firstChild = 0;
            this->m_lastChild = 0;
            node->m_parent = 0;
            node->m_nextSibling = 0;
            node->m_prevSibling = 0;
            return --this->m_numChildren;
        }
        if (v2 == node)
        {
            v2->m_nextSibling->m_prevSibling = 0;
            this->m_firstChild = this->m_firstChild->m_nextSibling;
            node->m_parent = 0;
            node->m_nextSibling = 0;
            node->m_prevSibling = 0;
            result = --this->m_numChildren;
        }
        else
        {
            v4 = this->m_lastChild;
            if (v4 == node)
            {
                v4->m_prevSibling->m_nextSibling = 0;
                this->m_lastChild = this->m_lastChild->m_prevSibling;
            }
            else
            {
                node->m_nextSibling->m_prevSibling = node->m_prevSibling;
                node->m_prevSibling->m_nextSibling = node->m_nextSibling;
            }
            node->m_parent = 0;
            node->m_nextSibling = 0;
            node->m_prevSibling = 0;
            result = --this->m_numChildren;
        }
        return result;
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
        if (pos == CStr_npos)
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
        xmlFile->GetFirstChild(xmlNode, "Prefabs");
        if (xmlNode->IsEmpty())
        {
            M3D_LOG_INFO("ChildNodeFromXmlFile: Prefabs node should be root node in " + ext);
            return nullptr;
        }
        xmlNode->GetFirstChild(xmlNode, "Node");
        while (!xmlNode->IsEmpty())
        {
            if (xmlNode->GetAttribute("name") == internalName)
            {
                break;
            }
            xmlNode->GetNextSibling(xmlNode, "Node");
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

    bool Object::GetPersistance() const
    {
        throw std::logic_error("Not implemented");
    }

    RETRUXX_DLL_INJECT_FUNCTION(0X0041DD60, Object::IsChildOf)
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
