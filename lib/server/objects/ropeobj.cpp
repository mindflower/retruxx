#include "ropeobj.h"

namespace ai
{
    RopeObjPrototypeInfo::RopeObjPrototypeInfo()
    {
        m_bIsUpdating = false;
    }

    bool RopeObjPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        bool const result = SimplePhysicObjPrototypeInfo::LoadFromXML(xmlFile, xmlNode);
        if (result)
        {
            _SetGeomType(GEOM_TYPE_BOX);
            m3d::SafeStrAttrib(m_brokenModel, xmlNode, "BrokenModel");
        }
        return result;
    }

    ai::Obj* RopeObjPrototypeInfo::CreateTargetObject() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    RopeObj::~RopeObj()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    RopeObj::RopeObj(ai::RopeObjPrototypeInfo const& prototypeInfo) : SimplePhysicObj(prototypeInfo)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* RopeObj::Clone()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Object* RopeObj::CreateObject()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* RopeObj::GetBaseClass()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::Class* RopeObj::GetClass() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    ai::RopeObjPrototypeInfo const* RopeObj::GetPrototypeInfo() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void RopeObj::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void RopeObj::SaveToXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    CVector RopeObj::GetGeometricCenter() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void RopeObj::SetPosFromTies()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::SgNode* RopeObj::GetTiedSgNodeNearPos(CVector const& tiePos, bool simpleSearch) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void RopeObj::HangToPosts()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void RopeObj::BreakOff(int breakObj, bool createJointForBreakObj)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    RopeObj::Post::Post(ai::RopeObj::Post const& __that)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    RopeObj::Post::Post()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    ai::RopeObj::Post const& RopeObj::GetPost(ai::RopeObj::HangPoints)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void RopeObj::SetPost(ai::RopeObj::HangPoints hp, ai::RopeObj::Post const& post)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void RopeObj::_InternalCreateVisualPart()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
}  // namespace ai
