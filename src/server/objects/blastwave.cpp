#include "blastwave.h"

#include <stdexcept>

namespace ai
{
	bool BlastWavePrototypeInfo::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
	{
		throw std::logic_error("Not implemented");
	}

	BlastWavePrototypeInfo::BlastWavePrototypeInfo()
	{
		throw std::logic_error("Not implemented");
	}

	Obj* BlastWavePrototypeInfo::CreateTargetObject() const
	{
		throw std::logic_error("Not implemented");
	}

	void BlastWavePrototypeInfo::_InternalCopyFrom(ai::PrototypeInfo const&)
	{
		throw std::logic_error("Not implemented");
	}

	void BlastWave::Update(float, unsigned)
	{
		throw std::logic_error("Not implemented");
	}

	void BlastWave::GetPropertiesNames(std::set<CStr>&) const
	{
		throw std::logic_error("Not implemented");
	}

	m3d::Class* BlastWave::GetBaseClass()
	{
		throw std::logic_error("Not implemented");
	}

	bool BlastWave::SetPropertyById(int, m3d::AIParam const&)
	{
		throw std::logic_error("Not implemented");
	}

	float BlastWave::GetBlastRadius() const
	{
		throw std::logic_error("Not implemented");
	}

	m3d::Class* BlastWave::GetClass() const
	{
		throw std::logic_error("Not implemented");
	}

	int BlastWave::GetEmitterId() const
	{
		throw std::logic_error("Not implemented");
	}

	short BlastWave::GetRocketExplosionType() const
	{
		throw std::logic_error("Not implemented");
	}

	int BlastWave::GetPropertyId(char const*) const
	{
		throw std::logic_error("Not implemented");
	}

	BlastWavePrototypeInfo const* BlastWave::GetPrototypeInfo() const
	{
		throw std::logic_error("Not implemented");
	}

	CStr BlastWave::GetPropertyName(int) const
	{
		throw std::logic_error("Not implemented");
	}

	eGObjPropertySaveStatus BlastWave::GetPropertySaveStatus(int) const
	{
		throw std::logic_error("Not implemented");
	}

	float BlastWave::GetDampedDamageIntensity(float) const
	{
		throw std::logic_error("Not implemented");
	}

	float BlastWave::GetWaveDamageIntensity() const
	{
		throw std::logic_error("Not implemented");
	}

	void BlastWave::GetPropertiesIDs(std::set<int, std::less<int>, std::allocator<int>>&) const
	{
		throw std::logic_error("Not implemented");
	}

	float BlastWave::GetWaveForceIntensity() const
	{
		throw std::logic_error("Not implemented");
	}

	float BlastWave::GetDampedForceIntensity(float) const
	{
		throw std::logic_error("Not implemented");
	}

	void BlastWave::SetEmitterId(int)
	{
		throw std::logic_error("Not implemented");
	}

	void BlastWave::SetRocketExplosionType(short)
	{
		throw std::logic_error("Not implemented");
	}

	void BlastWave::SetRocketId(int)
	{
		throw std::logic_error("Not implemented");
	}

	float BlastWave::GetDampingCoefficeient(float) const
	{
		throw std::logic_error("Not implemented");
	}

	void BlastWave::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
	{
		throw std::logic_error("Not implemented");
	}

	int BlastWave::CollideBlastWaveAndPhysicObj(BlastWave*, Obj*, dContact*, unsigned&, bool)
	{
		throw std::logic_error("Not implemented");
	}

	BlastWave::BlastWave(BlastWavePrototypeInfo const& prototypeInfo) : SimplePhysicObj(prototypeInfo)
	{
		throw std::logic_error("Not implemented");
	}

	bool BlastWave::_GetPropertyDefaultInternal(int, m3d::AIParam&) const
	{
		throw std::logic_error("Not implemented");
	}

	bool BlastWave::_GetPropertyInternal(int, m3d::AIParam&) const
	{
		throw std::logic_error("Not implemented");
	}

	BlastWave::~BlastWave()
	{
		throw std::logic_error("Not implemented");
	}

	m3d::Object* BlastWave::CreateObject()
	{
		throw std::logic_error("Not implemented");
	}

	m3d::Object* BlastWave::Clone()
	{
		throw std::logic_error("Not implemented");
	}
}
