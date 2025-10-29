#include "ropeobj.h"

namespace ai
{
    RopeObjPrototypeInfo::RopeObjPrototypeInfo()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool RopeObjPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    ai::Obj* RopeObjPrototypeInfo::CreateTargetObject() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    RopeObj::~RopeObj()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    RopeObj::RopeObj(const ai::RopeObjPrototypeInfo& prototypeInfo) : SimplePhysicObj(prototypeInfo)
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

    const ai::RopeObjPrototypeInfo* RopeObj::GetPrototypeInfo() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void RopeObj::LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode)
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

    m3d::SgNode* RopeObj::GetTiedSgNodeNearPos(const CVector& tiePos, bool simpleSearch) const
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

    RopeObj::Post::Post(const ai::RopeObj::Post& __that)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    RopeObj::Post::Post()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    const ai::RopeObj::Post& RopeObj::GetPost(ai::RopeObj::HangPoints)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void RopeObj::SetPost(ai::RopeObj::HangPoints hp, const ai::RopeObj::Post& post)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void RopeObj::_InternalCreateVisualPart()
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
}
