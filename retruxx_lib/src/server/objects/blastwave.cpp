#include "blastwave.h"

#include <stdexcept>

namespace ai
{
	RT_CLASS_EXPORTS_BEGIN(BlastWave)
	RT_CLASS_EXPORTS_END;
	RT_CLASS_DEFINE(BlastWave);

	bool BlastWavePrototypeInfo::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
	{
		throw retruxx::logic_error("Not implemented");
	}

	BlastWavePrototypeInfo::BlastWavePrototypeInfo()
	{
		throw retruxx::logic_error("Not implemented");
	}

	Obj* BlastWavePrototypeInfo::CreateTargetObject() const
	{
		throw retruxx::logic_error("Not implemented");
	}

	void BlastWavePrototypeInfo::_InternalCopyFrom(ai::PrototypeInfo const&)
	{
		throw retruxx::logic_error("Not implemented");
	}

	void BlastWave::Update(float, unsigned)
	{
		throw retruxx::logic_error("Not implemented");
	}

	void BlastWave::GetPropertiesNames(retruxx::set<CStr>&) const
	{
		throw retruxx::logic_error("Not implemented");
	}

	m3d::Class* BlastWave::GetBaseClass()
	{
		return RT_CLASS_LOCAL(SimplePhysicObj);
	}

	bool BlastWave::SetPropertyById(int, m3d::AIParam const&)
	{
		throw retruxx::logic_error("Not implemented");
	}

	float BlastWave::GetBlastRadius() const
	{
		throw retruxx::logic_error("Not implemented");
	}

	m3d::Class* BlastWave::GetClass() const
	{
		throw retruxx::logic_error("Not implemented");
	}

	int BlastWave::GetEmitterId() const
	{
		throw retruxx::logic_error("Not implemented");
	}

	short BlastWave::GetRocketExplosionType() const
	{
		throw retruxx::logic_error("Not implemented");
	}

	int BlastWave::GetPropertyId(char const*) const
	{
		throw retruxx::logic_error("Not implemented");
	}

	BlastWavePrototypeInfo const* BlastWave::GetPrototypeInfo() const
	{
		throw retruxx::logic_error("Not implemented");
	}

	CStr BlastWave::GetPropertyName(int) const
	{
		throw retruxx::logic_error("Not implemented");
	}

	eGObjPropertySaveStatus BlastWave::GetPropertySaveStatus(int) const
	{
		throw retruxx::logic_error("Not implemented");
	}

	float BlastWave::GetDampedDamageIntensity(float) const
	{
		throw retruxx::logic_error("Not implemented");
	}

	float BlastWave::GetWaveDamageIntensity() const
	{
		throw retruxx::logic_error("Not implemented");
	}

	void BlastWave::GetPropertiesIDs(retruxx::set<int, retruxx::less<int>, retruxx::allocator<int>>&) const
	{
		throw retruxx::logic_error("Not implemented");
	}

	float BlastWave::GetWaveForceIntensity() const
	{
		throw retruxx::logic_error("Not implemented");
	}

	float BlastWave::GetDampedForceIntensity(float) const
	{
		throw retruxx::logic_error("Not implemented");
	}

	void BlastWave::SetEmitterId(int)
	{
		throw retruxx::logic_error("Not implemented");
	}

	void BlastWave::SetRocketExplosionType(short)
	{
		throw retruxx::logic_error("Not implemented");
	}

	void BlastWave::SetRocketId(int)
	{
		throw retruxx::logic_error("Not implemented");
	}

	float BlastWave::GetDampingCoefficeient(float) const
	{
		throw retruxx::logic_error("Not implemented");
	}

	void BlastWave::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
	{
		throw retruxx::logic_error("Not implemented");
	}

	int BlastWave::CollideBlastWaveAndPhysicObj(BlastWave*, Obj*, dContact*, unsigned&, bool)
	{
		throw retruxx::logic_error("Not implemented");
	}

	BlastWave::BlastWave(BlastWavePrototypeInfo const& prototypeInfo) : SimplePhysicObj(prototypeInfo)
	{
		throw retruxx::logic_error("Not implemented");
	}

	bool BlastWave::_GetPropertyDefaultInternal(int, m3d::AIParam&) const
	{
		throw retruxx::logic_error("Not implemented");
	}

	bool BlastWave::_GetPropertyInternal(int, m3d::AIParam&) const
	{
		throw retruxx::logic_error("Not implemented");
	}

	BlastWave::~BlastWave()
	{
		throw retruxx::logic_error("Not implemented");
	}

	m3d::Object* BlastWave::CreateObject()
	{
		throw retruxx::logic_error("Not implemented");
	}

	m3d::Object* BlastWave::Clone()
	{
		throw retruxx::logic_error("Not implemented");
	}
}
