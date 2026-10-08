#pragma once
#include "ode/contact.h"

struct CVector;

namespace m3d
{
	class Object;
}

namespace ai
{
	class PhysicObj;
	struct DamageInfo;

	float CalcHitVelocity(CVector const& deltaVel, CVector const& normal);
	void CalcDamageToVehicles(ai::PhysicObj* obj1, ai::PhysicObj* obj2, dContact* contacts, float& dSpeed,
	                          ai::DamageInfo& damageInfo, CVector const& bodyPos);

	int CollideDummyAndVehiclePart(m3d::Object*, m3d::Object*, dContact*, unsigned int&, bool);
	int CollideVehiclePartAndVehiclePart(m3d::Object*, m3d::Object*, dContact*, unsigned int&, bool);
	int CollideVehicleAndLandscape(m3d::Object*, m3d::Object*, dContact*, unsigned int&, bool);
	int CollideVehicleAndStatics(m3d::Object*, m3d::Object*, dContact*, unsigned int&, bool);
	int CollideVehicleAndRoad(m3d::Object*, m3d::Object*, dContact*, unsigned int&, bool);
	int CollideVehicleAndWater(m3d::Object*, m3d::Object*, dContact*, unsigned int&, bool);
	int CollideVehiclePartAndGeomObj(m3d::Object*, m3d::Object*, dContact*, unsigned int&, bool);
	int CollideVehicleAndPassCell(m3d::Object*, m3d::Object*, dContact*, unsigned int&, bool);
}
