#include "relationship.h"
#include <stdexcept>
#include <core/ini.h>
#include <core/ref_ptr.h>
#include <core/log.h>

namespace ai
{
    Relationship::auxToleranceInfo::auxToleranceInfo(ai::eTolerance tolerance, CStr name) :
        m_tolerance(tolerance),
        m_name(name)
    {
    }

    void Relationship::LoadDefaultFromXmlFile(char const*)
    {
        // TODO: implement Relationship::LoadDefaultFromXmlFile
        //throw std::logic_error("Not implemented");
    }

    int Relationship::GetMinBelong() const
    {
        throw std::logic_error("Not implemented");
    }

    void Relationship::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        // TODO: implement Relationship::LoadFromXML
        //throw std::logic_error("Not implemented");
    }

    bool Relationship::AmongTolerance(int, int, std::set<eTolerance> const&) const
    {
        throw std::logic_error("Not implemented");
    }

    Relationship::Relationship() :
        m_toleranceList{{RS_ENEMY, "enemy"}, {RS_NEUTRAL, "neutral"}, {RS_ALLY, "ally"}, {RS_OWN, "own"}}
    {
    }

    eTolerance Relationship::GetToleranceByName(CStr const&) const
    {
        throw std::logic_error("Not implemented");
    }

    void Relationship::LoadFromXmlFile(char const* fileName)
    {
        CStr err;
        ref_ptr file = m3d::ReadXmlFile(fileName, &err);

        M3D_LOG_INFO("\t\t File readed");
        if (file)
        {
            ref_ptr node = file->CreateNode();
            file->GetFirstChild(node, "relationship");
            LoadFromXML(file, node);
        }
        else
        {
            M3D_LOG_ERR("Error: No Relationship file: " + CStr(fileName));
        }
    }

    float Relationship::GetDefaultTolerance(int, int) const
    {
        throw std::logic_error("Not implemented");
    }

    void Relationship::IncTolerance(int, int, float)
    {
        throw std::logic_error("Not implemented");
    }

    void Relationship::SaveToXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        throw std::logic_error("Not implemented");
    }

    Relationship::~Relationship()
    {
        throw std::logic_error("Not implemented");
    }

    int Relationship::GetMaxBelong() const
    {
        throw std::logic_error("Not implemented");
    }

    eTolerance Relationship::CheckTolerance(int, int) const
    {
        throw std::logic_error("Not implemented");
    }

    eTolerance Relationship::CheckDefaultTolerance(int, int) const
    {
        throw std::logic_error("Not implemented");
    }

    void Relationship::SetTolerance(int, int, float)
    {
        throw std::logic_error("Not implemented");
    }

    void Relationship::SetTolerance(CStr const&, CStr const&, float)
    {
        throw std::logic_error("Not implemented");
    }

    CStr Relationship::GetNameByTolerance(eTolerance) const
    {
        throw std::logic_error("Not implemented");
    }

    float Relationship::GetTolerance(int, int) const
    {
        throw std::logic_error("Not implemented");
    }

    void Relationship::_LoadFormat1(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw std::logic_error("Not implemented");
    }

    void Relationship::_LoadFormat0(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        throw std::logic_error("Not implemented");
    }

    int Relationship::GetIndex(int) const
    {
        throw std::logic_error("Not implemented");
    }

    bool Relationship::IsValid(int) const
    {
        throw std::logic_error("Not implemented");
    }

    void Relationship::_SaveFormat1(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        throw std::logic_error("Not implemented");
    }

    void Relationship::_SaveFormat0(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        throw std::logic_error("Not implemented");
    }
}
