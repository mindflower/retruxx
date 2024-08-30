#include "prototypeinfo.h"

#include <stdexcept>

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
	}

	bool PrototypeInfo::bIsAbstract() const
	{
		throw std::logic_error("Not implemented");
	}

	bool PrototypeInfo::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
	{
		throw std::logic_error("Not implemented");
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
