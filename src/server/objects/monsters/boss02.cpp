#include "boss02.h"

RT_CLASS_EXPORT_METHOD_DEFINE(Boss02, NextState)
{
	throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORT_METHOD_DEFINE(Boss02, SetJustMovedContainerToBlock)
{
	throw std::logic_error("Not implemented");
}

namespace ai
{
	RT_CLASS_EXPORTS_BEGIN(Boss02)
		RT_CLASS_EXPORT(Boss02, m3d::METHOD, NextState, "", "", "")
		RT_CLASS_EXPORT(Boss02, m3d::METHOD, SetJustMovedContainerToBlock, "", "", "")
	RT_CLASS_EXPORTS_END;
	RT_CLASS_DEFINE(Boss02);

	void Boss02PrototypeInfo::StateInfo::SaveToXML(m3d::cmn::XmlNode*) const
	{
		throw std::logic_error("Not implemented");
	}

	void Boss02PrototypeInfo::StateInfo::PostLoad()
	{
		throw std::logic_error("Not implemented");
	}

	Boss02PrototypeInfo::StateInfo::StateInfo(StateInfo const&)
	{
		throw std::logic_error("Not implemented");
	}

	void Boss02PrototypeInfo::StateInfo::LoadFromXML(m3d::cmn::XmlNode const*)
	{
		throw std::logic_error("Not implemented");
	}

	bool Boss02PrototypeInfo::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
	{
		throw std::logic_error("Not implemented");
	}

	void Boss02PrototypeInfo::PostLoad()
	{
		throw std::logic_error("Not implemented");
	}

	Boss02PrototypeInfo::Boss02PrototypeInfo()
	{
		throw std::logic_error("Not implemented");
	}

	void Boss02PrototypeInfo::RefreshFromXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
	{
		throw std::logic_error("Not implemented");
	}

	Obj* Boss02PrototypeInfo::CreateTargetObject() const
	{
		throw std::logic_error("Not implemented");
	}

	void Boss02::SaveRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
	{
		throw std::logic_error("Not implemented");
	}

	void Boss02::CreateChildren()
	{
		throw std::logic_error("Not implemented");
	}

	void Boss02::SaveToXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
	{
		throw std::logic_error("Not implemented");
	}

	m3d::Class* Boss02::GetClass() const
	{
		throw std::logic_error("Not implemented");
	}

	Boss02::Boss02(Boss02PrototypeInfo const& prototype) : ComplexPhysicObj(prototype)
	{
		throw std::logic_error("Not implemented");
	}

	void Boss02::NextState()
	{
		throw std::logic_error("Not implemented");
	}

	bool Boss02::CanChildBeAdded(m3d::Class*) const
	{
		throw std::logic_error("Not implemented");
	}

	int Boss02::OnEvent(Event const&)
	{
		throw std::logic_error("Not implemented");
	}

	void Boss02::SetJustMovedContainerToBlock()
	{
		throw std::logic_error("Not implemented");
	}

	void Boss02::Update(float, unsigned)
	{
		throw std::logic_error("Not implemented");
	}

	m3d::Class* Boss02::GetBaseClass()
	{
		return RT_CLASS_LOCAL(ComplexPhysicObj);
	}

	Boss02PrototypeInfo const* Boss02::GetPrototypeInfo() const
	{
		throw std::logic_error("Not implemented");
	}

	bool Boss02::RemoveChild(Obj*)
	{
		throw std::logic_error("Not implemented");
	}

	void Boss02::LoadRuntimeValues(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
	{
		throw std::logic_error("Not implemented");
	}

	float Boss02::GetHealth() const
	{
		throw std::logic_error("Not implemented");
	}

	void Boss02::AddChild(Obj*)
	{
		throw std::logic_error("Not implemented");
	}

	void Boss02::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
	{
		throw std::logic_error("Not implemented");
	}

	void Boss02::SetPositionSelf(CVector const&)
	{
		throw std::logic_error("Not implemented");
	}

	float Boss02::GetMaxHealth() const
	{
		throw std::logic_error("Not implemented");
	}

	void Boss02::Die()
	{
		throw std::logic_error("Not implemented");
	}

	void Boss02::_InternalPostLoad()
	{
		throw std::logic_error("Not implemented");
	}

	void Boss02::_InternalCreateVisualPart()
	{
		throw std::logic_error("Not implemented");
	}

	Boss02::~Boss02()
	{
		throw std::logic_error("Not implemented");
	}

	void Boss02::_SetMoveState(MoveState)
	{
		throw std::logic_error("Not implemented");
	}

	void Boss02::_PlaceContainerToInitialPosition()
	{
		throw std::logic_error("Not implemented");
	}

	VehiclePart* Boss02::_GetPlatform()
	{
		throw std::logic_error("Not implemented");
	}

	void Boss02::_OnBossCriticalLoadsExploded(Event const&)
	{
		throw std::logic_error("Not implemented");
	}

	m3d::Object* Boss02::CreateObject()
	{
		throw std::logic_error("Not implemented");
	}

	void Boss02::_SetMovePosition(CVector const&)
	{
		throw std::logic_error("Not implemented");
	}

	m3d::Object* Boss02::Clone()
	{
		throw std::logic_error("Not implemented");
	}

	Boss02Arm* Boss02::_GetArm()
	{
		throw std::logic_error("Not implemented");
	}

	Boss02Arm const* Boss02::_GetArm() const
	{
		throw std::logic_error("Not implemented");
	}

	void Boss02::_SetToDeadPose()
	{
		throw std::logic_error("Not implemented");
	}

	void Boss02::_OnBossArmActionFinished(Event const&)
	{
		throw std::logic_error("Not implemented");
	}
}
