#include "dummyobject.h"

#include <stdexcept>
#include <ode/objects.h>

#include "skelmodel.h"
#include "core/ini.h"
#include "base/prototypemanager.h"
#include "ode/odecpp.h"

RT_CLASS_EXPORT_METHOD_DEFINE(DummyObject, SetModelName)
{
	auto* obj = dynamic_cast<ai::DummyObject*>(context->asObject(0, "DummyObject"));
	auto name = context->asString(1);
	obj->SetModelName(name);
	return 1;
}

namespace ai
{
	RT_CLASS_EXPORTS_BEGIN(DummyObject)
		RT_CLASS_EXPORT(DummyObject, m3d::METHOD, SetModelName, "", "", "")
	RT_CLASS_EXPORTS_END;
	RT_CLASS_DEFINE(DummyObject);

	DummyObjectPrototypeInfo::DummyObjectPrototypeInfo()
	{
        this->m_DisablePhysics = 0;
        this->m_DisableGeometry = 0;
        this->m_bIsUpdating = 0;
	}

	bool DummyObjectPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
	{
        auto result = ai::SimplePhysicObjPrototypeInfo::LoadFromXML(xmlFile, xmlNode);
        if (result)
        {
            ai::SimplePhysicObjPrototypeInfo::_SetGeomType(GEOM_TYPE_FROM_MODEL);
            m3d::SafeBoolAttrib(this->m_DisablePhysics, xmlNode, "DisablePhysics");
            m3d::SafeBoolAttrib(this->m_DisableGeometry, xmlNode, "DisableGeometry");
            this->m_bCollisionTrimeshAllowed = this->m_DisablePhysics;
            return 1;
        }
        return result;
	}

	Obj* DummyObjectPrototypeInfo::CreateTargetObject() const
	{
        return new DummyObject(*this);
	}

	void DummyObject::Registration()
	{
		m_propertiesMap["ModelName"] = 44;
	}

	eGObjPropertySaveStatus DummyObject::GetPropertySaveStatus(int) const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	int DummyObject::GetPropertyId(char const*) const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	void DummyObject::SetModelName(char const* modelName)
	{
		m_modelName = modelName;
		m_physicBody->SetModelName(modelName);

        auto model = m_physicBody->GetModel();
		auto x = model->m_box.m_box[3] - model->m_box.m_box[0];
		auto y = model->m_box.m_box[4] - model->m_box.m_box[1];
		auto z = model->m_box.m_box[5] - model->m_box.m_box[2];
        auto radius = sqrt(x * x + y * y + z * z) * 0.5f;
		_SetBoundSphereRadius(radius);
		_UpdateCollisionInfoFromPhysicBody();
		if (!dBodyIsEnabled(GetBody()->id()))
		{
			m_physicBody->UnlinkGeomsFromBody();
        }
		SetMass(GetMass());
		SetScale(m_scale, true);
	}

	bool DummyObject::SetPropertyById(int, m3d::AIParam const&)
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	m3d::Class* DummyObject::GetBaseClass()
	{
		return RT_CLASS_LOCAL(SimplePhysicObj);
	}

	m3d::Class* DummyObject::GetClass() const
	{
        return RT_CLASS_LOCAL(DummyObject);
	}

	CStr DummyObject::GetPropertyName(int) const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	DummyObjectPrototypeInfo const* DummyObject::GetPrototypeInfo() const
	{
		return RT_DYNCAST(thePrototypeManager->GetPrototypeInfo(GetPrototypeId()), const DummyObjectPrototypeInfo);
	}

	void DummyObject::SetModelNameUnsafe(CStr const&)
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	void DummyObject::SetSgNodeAndCollision(m3d::SgNode*, CollisionInfo const*)
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	void DummyObject::GetPropertiesIDs(retruxx::set<int, retruxx::less<int>, retruxx::allocator<int>>&) const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	void DummyObject::GetPropertiesNames(retruxx::set<CStr, retruxx::less<CStr>, retruxx::allocator<CStr>>&) const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	DummyObject::DummyObject(DummyObjectPrototypeInfo const& prototypeInfo) : SimplePhysicObj(prototypeInfo)
	{
		if (prototypeInfo.m_DisablePhysics)
			ai::PhysicObj::DisablePhysics();

		if (prototypeInfo.m_DisableGeometry)
			ai::SimplePhysicObj::DisableGeometry(1);

		if (m_physicBody)
		{
			if (m_modelName != m_physicBody->m_modelname)
			    m_modelName = m_physicBody->m_modelname;
		}
	}

	bool DummyObject::CanChildBeAdded(m3d::Class*) const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	bool DummyObject::_GetPropertyDefaultInternal(int, m3d::AIParam&) const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	bool DummyObject::_GetPropertyInternal(int, m3d::AIParam&) const
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	void DummyObject::RegisterProperty(char const*, int, eGObjPropertySaveStatus)
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	DummyObject::~DummyObject() = default;

	m3d::Object* DummyObject::Clone()
	{
		RETRUXX_NOT_IMPLEMENTED;
	}

	m3d::Object* DummyObject::CreateObject()
	{
		RETRUXX_NOT_IMPLEMENTED;
	}
}
