#include "relationship.h"
#include <stdexcept>
#include <core/ini.h>
#include <core/ref_ptr.h>
#include <core/log.h>
#include <algorithm>

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
        //RETRUXX_NOT_IMPLEMENTED;
    }

    int Relationship::GetMinBelong() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Relationship::LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        // TODO: implement Relationship::LoadFromXML
        //RETRUXX_NOT_IMPLEMENTED;
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
        RETRUXX_NOT_IMPLEMENTED;
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

    float Relationship::GetDefaultTolerance(int belongId1, int belongId2) const
    {
        if (belongId1 == belongId2)
            return 4.0;

        if (belongId1 < m_MinID)
            return (double)(int)m_defaultTolerance;

        if (belongId1 > m_MaxID || belongId2 < m_MinID || belongId2 > m_MaxID)
            return (double)(int)m_defaultTolerance;
        else
            return m_pDefaultTolerance[belongId1 + (belongId2 - m_MinID) * (m_MaxID - m_MinID + 1) - m_MinID];
    }

    void Relationship::IncTolerance(int belongId1, int belongId2, float increment)
    {
        if (belongId1 != belongId2)
        {
            if (belongId1 >= m_MinID)
            {
                if (belongId1 <= m_MaxID && belongId2 >= m_MinID && belongId2 <= m_MaxID)
                {
                    int v7 = belongId1 - m_MinID;
                    float v8 = m_pTolerance[(belongId2 - m_MinID) * (m_MaxID - m_MinID + 1) + v7] + increment;
                    v8 = std::clamp(v8, 0.0f, 3.0f);

                    m_pTolerance[(belongId2 - m_MinID) * (m_MaxID - m_MinID + 1) + v7] = v8;
                    m_pTolerance[belongId2 + (belongId1 - m_MinID) * (m_MaxID - m_MinID + 1) - m_MinID] = v8;
                }
            }
        }
    }

    void Relationship::SaveToXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    Relationship::~Relationship()
    {
        delete[] m_pTolerance;
        delete[] m_pDefaultTolerance;
    }

    int Relationship::GetMaxBelong() const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    eTolerance Relationship::CheckTolerance(int belongId1, int belongId2) const
    {
        if (belongId1 == belongId2)
            return RS_OWN;

        if (belongId1 < this->m_MinID)
            return this->m_defaultTolerance;

        if (belongId1 > m_MaxID || belongId2 < m_MinID || belongId2 > m_MaxID)
            return this->m_defaultTolerance;

        RETRUXX_NOT_IMPLEMENTED;
    }

    eTolerance Relationship::CheckDefaultTolerance(int belongId1, int belongId2) const
    {
        if (belongId1 == belongId2)
            return RS_OWN;

        if (belongId1 < m_MinID)
            return m_defaultTolerance;

        if (belongId1 > m_MaxID || belongId2 < m_MinID || belongId2 > m_MaxID)
            return m_defaultTolerance;

        auto v10 = GetDefaultTolerance(belongId1, belongId2) - (double)(int)m_defaultTolerance;
        eTolerance NearestTol = m_defaultTolerance;
        unsigned belongId1a = 0;
        unsigned i = 0;
        float Min = fabs(v10);
        for (auto const& tolerance : m_toleranceList)
        {
            if (Min > fabs(GetDefaultTolerance(belongId1, belongId2) - (double)(int)tolerance.m_tolerance))
            {
                auto DefaultTolerance = GetDefaultTolerance(belongId1, belongId2);
                NearestTol = tolerance.m_tolerance;
                Min = fabs(DefaultTolerance - (double)(int)tolerance.m_tolerance);
            }
        }
        return NearestTol;
    }

    void Relationship::SetTolerance(int belongId1, int belongId2, float value)
    {
        if (belongId1 != belongId2)
        {
            if (belongId1 >= this->m_MinID)
            {
                if (belongId1 <= m_MaxID && belongId2 >= m_MinID && belongId2 <= m_MaxID)
                {
                    this->m_pTolerance[belongId1 + (belongId2 - m_MinID) * (m_MaxID - m_MinID + 1) - m_MinID] = value;
                    this->m_pTolerance
                        [belongId2 + (belongId1 - this->m_MinID) * (this->m_MaxID - this->m_MinID + 1) -
                         this->m_MinID] = value;
                }
            }
        }
    }

    void Relationship::SetTolerance(CStr const&, CStr const&, float)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    CStr Relationship::GetNameByTolerance(eTolerance) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    float Relationship::GetTolerance(int belongId1, int belongId2) const
    {
        if (belongId1 == belongId2)
            return 4.0;
        if (belongId1 < this->m_MinID)
            return (double)(int)this->m_defaultTolerance;

        if (belongId1 > m_MaxID || belongId2 < m_MinID || belongId2 > m_MaxID)
            return (double)(int)this->m_defaultTolerance;
        else
            return this->m_pTolerance[belongId1 + (belongId2 - m_MinID) * (m_MaxID - m_MinID + 1) - m_MinID];
    }

    void Relationship::_LoadFormat1(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Relationship::_LoadFormat0(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*)
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    int Relationship::GetIndex(int) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    bool Relationship::IsValid(int) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Relationship::_SaveFormat1(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }

    void Relationship::_SaveFormat0(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const
    {
        RETRUXX_NOT_IMPLEMENTED;
    }
}  // namespace ai
