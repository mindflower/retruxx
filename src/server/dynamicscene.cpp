#include "dynamicscene.h"

#include <stdexcept>

namespace ai
{
	DynamicScene* gDynamicScene = nullptr;

	void DynamicScene::SoilProps::LoadFromXml(m3d::cmn::XmlNode const*)
	{
		throw std::logic_error("Not implemented");
	}

	DynamicScene::SoilProps::SoilProps()
	{
		throw std::logic_error("Not implemented");
	}

	short DynamicScene::GetBoEffectTypeByName(CStr const&)
	{
		throw std::logic_error("Not implemented");
	}

	void DynamicScene::DeleteAll()
	{
		throw std::logic_error("Not implemented");
	}

	void DynamicScene::PurgeBodies()
	{
		throw std::logic_error("Not implemented");
	}

	CStr const& DynamicScene::GetBoEffectTypeName(unsigned short)
	{
		throw std::logic_error("Not implemented");
	}

	m3d::Object* DynamicScene::Clone()
	{
		return Object::Clone();
	}

	void DynamicScene::LinkNodesFromBodyToSceneGraph(Obj*)
	{
		throw std::logic_error("Not implemented");
	}

	int DynamicScene::ProcessShellAndBody(Shell*, PhysicBody*, dContact*, unsigned&, bool)
	{
		throw std::logic_error("Not implemented");
	}

	void DynamicScene::InitClashDecalId()
	{
		throw std::logic_error("Not implemented");
	}

	void DynamicScene::ReadSoilProps(char const*)
	{
		throw std::logic_error("Not implemented");
	}

	CStr const& DynamicScene::GetShellWaterEffectName(unsigned short) const
	{
		throw std::logic_error("Not implemented");
	}

	bool DynamicScene::LoadSceneFromXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*, std::vector<m3d::Class*> const&)
	{
		throw std::logic_error("Not implemented");
	}

	void DynamicScene::StepScene(float)
	{
		throw std::logic_error("Not implemented");
	}

	bool DynamicScene::SaveSceneToXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*)
	{
		throw std::logic_error("Not implemented");
	}

	DynamicScene::SoilProps const& DynamicScene::GetSoilProps(unsigned, unsigned) const
	{
		throw std::logic_error("Not implemented");
	}

	CStr const& DynamicScene::GetShellStaticsEffectName(unsigned short) const
	{
		throw std::logic_error("Not implemented");
	}

	CStr const& DynamicScene::GetSoilEffectName(unsigned, unsigned short, bool) const
	{
		throw std::logic_error("Not implemented");
	}

	void DynamicScene::RenderDebugInfo()
	{
		throw std::logic_error("Not implemented");
	}

	m3d::Object* DynamicScene::CreateObject()
	{
		throw std::logic_error("Not implemented");
	}

	CStr const& DynamicScene::GetBoVehicleEffectName(unsigned short) const
	{
		throw std::logic_error("Not implemented");
	}

	void DynamicScene::CollideScene(float)
	{
		throw std::logic_error("Not implemented");
	}

	CStr const& DynamicScene::GetShellVehicleEffectName(unsigned short) const
	{
		throw std::logic_error("Not implemented");
	}

	void DynamicScene::CreateBoShellEffectNames()
	{
		throw std::logic_error("Not implemented");
	}

	CStr const& DynamicScene::GetVehicleSoilEffectName(unsigned short) const
	{
		throw std::logic_error("Not implemented");
	}

	m3d::Class* DynamicScene::GetBaseClass()
	{
		throw std::logic_error("Not implemented");
	}

	CStr const& DynamicScene::GetDecalName(int)
	{
		throw std::logic_error("Not implemented");
	}

	void DynamicScene::ClearOnce()
	{
		throw std::logic_error("Not implemented");
	}

	int DynamicScene::GetNumNearCallbacksLastFrame()
	{
		throw std::logic_error("Not implemented");
	}

	int DynamicScene::AddDecalName(CStr const&)
	{
		throw std::logic_error("Not implemented");
	}

	int DynamicScene::GetClashDecalId()
	{
		throw std::logic_error("Not implemented");
	}

	void DynamicScene::InitOnce()
	{
		throw std::logic_error("Not implemented");
	}

	DynamicScene::~DynamicScene()
	{
		throw std::logic_error("Not implemented");
	}

	unsigned DynamicScene::GetWheelTypeByName(CStr const&)
	{
		throw std::logic_error("Not implemented");
	}

	CStr const& DynamicScene::GetBoShellEffectName(unsigned short, unsigned short)
	{
		throw std::logic_error("Not implemented");
	}

	void DynamicScene::CollideBullet(Bullet const&)
	{
		throw std::logic_error("Not implemented");
	}

	bool DynamicScene::LoadSceneFromFile(char const*, std::vector<m3d::Class*> const&)
	{
		throw std::logic_error("Not implemented");
	}

	int DynamicScene::ReadNewObjectFromXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*,
		std::vector<m3d::Class*> const&)
	{
		throw std::logic_error("Not implemented");
	}

	short DynamicScene::GetExplosionType(CStr const&)
	{
		throw std::logic_error("Not implemented");
	}

	bool DynamicScene::SaveSceneToFile(char const*)
	{
		throw std::logic_error("Not implemented");
	}

	Vehicle* DynamicScene::GetVehicleControlledByPlayer() const
	{
		throw std::logic_error("Not implemented");
	}

	CStr const& DynamicScene::GetRoadEffectName(unsigned, bool) const
	{
		throw std::logic_error("Not implemented");
	}

	CStr const& DynamicScene::GetShellRoadEffectName(unsigned short) const
	{
		throw std::logic_error("Not implemented");
	}

	void DynamicScene::Clear()
	{
		throw std::logic_error("Not implemented");
	}

	m3d::Class* DynamicScene::GetClass() const
	{
		return Object::GetClass();
	}

	CStr const& DynamicScene::GetShellEffectName(unsigned short, unsigned short) const
	{
		throw std::logic_error("Not implemented");
	}

	void DynamicScene::UpdateSceneItems(float)
	{
		throw std::logic_error("Not implemented");
	}

	DynamicScene::DynamicScene(DynamicScene const&)
	{
		throw std::logic_error("Not implemented");
	}

	DynamicScene::DynamicScene()
	{
		throw std::logic_error("Not implemented");
	}

	void DynamicScene::_InitWheelTraces()
	{
		throw std::logic_error("Not implemented");
	}

	void DynamicScene::_RecalcWheelEffectNames()
	{
		throw std::logic_error("Not implemented");
	}

	void DynamicScene::_AddSoilEffectNameForWheelTypeName(CStr const&)
	{
		throw std::logic_error("Not implemented");
	}
}
