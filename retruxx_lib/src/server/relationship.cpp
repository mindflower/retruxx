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

    bool Relationship::AmongTolerance(int belongId1, int belongId2, std::set<eTolerance> const& toleranceSet) const
    {
        auto checkTolerance = CheckTolerance(belongId1, belongId2);
        return toleranceSet.find(checkTolerance) != toleranceSet.end();
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

    eTolerance Relationship::CheckTolerance(int belongId1, int belongId2) const
    {
        if (belongId1 == belongId2)
            return RS_OWN;

        if (belongId1 < this->m_MinID)
            return this->m_defaultTolerance;

        if (belongId1 > m_MaxID || belongId2 < m_MinID || belongId2 > m_MaxID)
            return this->m_defaultTolerance;

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
