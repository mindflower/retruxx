#include "chest.h"

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(Chest)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(Chest);

    ChestPrototypeInfo::ChestPrototypeInfo() = default;

    bool ChestPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        bool const result = SimplePhysicObjPrototypeInfo::LoadFromXML(xmlFile, xmlNode);
        if (result)
        {
            _SetGeomType(GEOM_TYPE_BOX);
            m_LifeTime = -1.0f;
            m3d::SafeFloatAttrib(m_LifeTime, xmlNode, "LifeTime");
            m_WithLifeTime = m_LifeTime > 0.0;
        }
        return result;
    }

    ai::Obj* ChestPrototypeInfo::CreateTargetObject() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Chest::~Chest()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Chest::Chest(ai::ChestPrototypeInfo const& prototypeInfo) : SimplePhysicObj(prototypeInfo)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* Chest::Clone()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* Chest::CreateObject()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* Chest::GetBaseClass()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* Chest::GetClass() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    ai::ChestPrototypeInfo const* Chest::GetPrototypeInfo() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Chest::Update(float elapsedTime, unsigned int workTime)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Chest::LoadRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Chest::SaveRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    ai::GeomRepository* Chest::GetRepository()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Chest::FillFromGroundRepository(ai::GeomRepository* repository, bool takeDroppedObjects)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Chest::SetPositionSelf(CVector const& pos)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Chest::RenderDebugInfo() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Chest::CanChildBeAdded(m3d::Class* pClass) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Chest::AddChild(ai::Obj* pObj)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Chest::RemoveChild(ai::Obj* pObj)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Chest::RemoveChildUnsafe(ai::Obj* pObj)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
}  // namespace ai
