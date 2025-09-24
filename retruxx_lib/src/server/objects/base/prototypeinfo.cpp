#include "prototypeinfo.h"

#include <stdexcept>

#include "core/ini.h"
#include "core/kernel.h"
#include "core/log.h"
#include "server/resourcemanager.h"

namespace ai
{
	void PrototypeInfo::PostLoad()
	{
		throw std::logic_error("Not implemented");
	}

	unsigned PrototypeInfo::GetBasePrice() const
	{
		throw std::logic_error("Not implemented");
	}

	PrototypeInfo::PrototypeInfo()
	{
        this->m_prototypeId = -1;
        this->m_resourceId = -1;
        this->m_bIsUpdating = 1;
        this->m_bVisibleInEncyclopedia = 1;
        this->m_bApplyAffixes = 1;
        this->m_price = 0;
        this->m_bIsAbstract = 0;
        this->m_protoClassObject = 0;
	}

	bool PrototypeInfo::bIsAbstract() const
	{
		throw std::logic_error("Not implemented");
	}

	bool PrototypeInfo::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const* xmlNode)
	{
        m3d::SafeStrAttrib(m_prototypeName, xmlNode, "Name");
        m3d::SafeStrAttrib(m_className, xmlNode, "Class");

        this->m_protoClassObject = m3d::g_Kernel->FindClass(m_className.c_str());

        CStr resType;
        m3d::SafeStrAttrib(resType, xmlNode, "ResourceType");
        if (!resType.empty())
        {
            m_resourceId = ai::theResourceManager->GetResourceId(resType);
        }
        m3d::SafeBoolAttrib(m_bIsUpdating, xmlNode, "IsUpdating");
        if (!resType.empty() && m_resourceId == -1)
        {
            M3D_LOG_ERR("Error: No ResourceType '" + resType + "' registered");
        }

        m3d::SafeBoolAttrib(m_bVisibleInEncyclopedia, xmlNode, "VisibleInEncyclopedia");
        m3d::SafeBoolAttrib(m_bApplyAffixes, xmlNode, "ApplyAffixes");
        m3d::SafeUintAttrib(m_price, xmlNode, "Price");

        this->m_bIsAbstract = false;
        m3d::SafeBoolAttrib(m_bIsAbstract, xmlNode, "Abstract");
        m3d::SafeStrAttrib(m_parentPrototypeName, xmlNode, "ParentPrototype");

        return true;
	}

	void PrototypeInfo::CopyFrom(PrototypeInfo const&)
	{
		throw std::logic_error("Not implemented");
	}

	void PrototypeInfo::RefreshFromXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
	{
		throw std::logic_error("Not implemented");
	}

	CStr PrototypeInfo::GetDebugDescription() const
	{
		throw std::logic_error("Not implemented");
	}

	PrototypeInfo::~PrototypeInfo()
	{
	}

	bool PrototypeInfo::IsPrototypeOf(m3d::Class const*) const
	{
		throw std::logic_error("Not implemented");
	}

	CStr const& PrototypeInfo::GetParentPrototypeName() const
	{
		throw std::logic_error("Not implemented");
	}

	void PrototypeInfo::_InternalCopyFrom(PrototypeInfo const&)
	{
		throw std::logic_error("Not implemented");
	}
}
