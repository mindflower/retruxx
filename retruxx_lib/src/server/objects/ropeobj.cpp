#include "ropeobj.h"

namespace ai
{
    RopeObjPrototypeInfo::RopeObjPrototypeInfo()
    {
        throw std::logic_error("Not implemented");
    }

    bool RopeObjPrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode)
    {
        throw std::logic_error("Not implemented");
    }

    ai::Obj* RopeObjPrototypeInfo::CreateTargetObject() const
    {
        throw std::logic_error("Not implemented");
    }

    RopeObj::~RopeObj()
    {
        throw std::logic_error("Not implemented");
    }

    RopeObj::RopeObj(const ai::RopeObjPrototypeInfo& prototypeInfo) : SimplePhysicObj(prototypeInfo)
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* RopeObj::Clone()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Object* RopeObj::CreateObject()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* RopeObj::GetBaseClass()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::Class* RopeObj::GetClass() const
    {
        throw std::logic_error("Not implemented");
    }

    const ai::RopeObjPrototypeInfo* RopeObj::GetPrototypeInfo() const
    {
        throw std::logic_error("Not implemented");
    }

    void RopeObj::LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode)
    {
        throw std::logic_error("Not implemented");
    }

    void RopeObj::SaveToXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const
    {
        throw std::logic_error("Not implemented");
    }

    CVector RopeObj::GetGeometricCenter() const
    {
        throw std::logic_error("Not implemented");
    }

    void RopeObj::SetPosFromTies()
    {
        throw std::logic_error("Not implemented");
    }

    m3d::SgNode* RopeObj::GetTiedSgNodeNearPos(const CVector& tiePos, bool simpleSearch) const
    {
        throw std::logic_error("Not implemented");
    }

    void RopeObj::HangToPosts()
    {
        throw std::logic_error("Not implemented");
    }

    void RopeObj::BreakOff(int breakObj, bool createJointForBreakObj)
    {
        throw std::logic_error("Not implemented");
    }

    RopeObj::Post::Post(const ai::RopeObj::Post& __that)
    {
        throw std::logic_error("Not implemented");
    }

    RopeObj::Post::Post()
    {
        throw std::logic_error("Not implemented");
    }

    const ai::RopeObj::Post& RopeObj::GetPost(ai::RopeObj::HangPoints)
    {
        throw std::logic_error("Not implemented");
    }

    void RopeObj::SetPost(ai::RopeObj::HangPoints hp, const ai::RopeObj::Post& post)
    {
        throw std::logic_error("Not implemented");
    }

    void RopeObj::_InternalCreateVisualPart()
    {
        throw std::logic_error("Not implemented");
    }
}
