#include "ropeobj.h"
#include "base/prototypemanager.h"

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(RopeObj)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(RopeObj);

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
        return new RopeObj(*this);
    }

    RopeObj::~RopeObj() = default;

    RopeObj::RopeObj(ai::RopeObjPrototypeInfo const& prototypeInfo) : SimplePhysicObj(prototypeInfo)
    {
        m_strech = 1.0f;
        DisablePhysics();
        DisableGeometry(true);
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
        return RT_CLASS_LOCAL(SimplePhysicObj);
    }

    m3d::Class* RopeObj::GetClass() const
    {
        return RT_CLASS_LOCAL(RopeObj);
    }

    ai::RopeObjPrototypeInfo const* RopeObj::GetPrototypeInfo() const
    {
        return RT_DYNCAST(thePrototypeManager->GetPrototypeInfo(GetPrototypeId()), RopeObjPrototypeInfo const);
    }

    void RopeObj::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        Obj::LoadFromXML(xmlFile, xmlNode);
        // TODO: implement RopeObj::LoadFromXML
        // RETRUXX_NOT_IMPLEMENTED;
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
        // TODO: implement RopeObj::SetPosFromTies
        // RETRUXX_NOT_IMPLEMENTED;
    }

    m3d::SgNode* RopeObj::GetTiedSgNodeNearPos(CVector const& tiePos, bool simpleSearch) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void RopeObj::HangToPosts()
    {
        // TODO: implement RopeObj::SetPosFromTies
        // RETRUXX_NOT_IMPLEMENTED;
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
        m_postNode = nullptr;
        m_postObj = nullptr;
        m_postTiePos = ZeroVector;
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
        SimplePhysicObj::_InternalCreateVisualPart();
        SetPosFromTies();
        HangToPosts();
    }
}  // namespace ai
