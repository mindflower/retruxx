#include "relationship.h"
#include <stdexcept>
#include <core/ini.h>
#include <core/kernel.h>
#include <core/ref_ptr.h>
#include <core/log.h>
#include <server/objects/base/objcontainer.h>
#include <server/utils.h>
#include <algorithm>
#include <cmath>

namespace ai
{
    Relationship::auxToleranceInfo::auxToleranceInfo(ai::eTolerance tolerance, CStr name) :
        m_tolerance(tolerance),
        m_name(name)
    {
    }

    void Relationship::LoadDefaultFromXmlFile(char const* fileName)
    {
        // RVA 0x65BF30 - reads the same <relationship> document as LoadFromXmlFile, but keeps
        // the result as the "pristine" table the game compares against later.
        delete[] m_pDefaultTolerance;
        m_pDefaultTolerance = nullptr;

        CStr err;
        ref_ptr file = m3d::ReadXmlFile(fileName, &err);
        if (file)
        {
            ref_ptr node = file->CreateNode();
            file->GetFirstChild(node, "relationship");

            int oldMinId = m_MinID;
            int oldMaxId = m_MaxID;

            // LoadFromXML always builds into m_pTolerance, so the live table is parked here
            // and swapped back once the freshly parsed one has been claimed.
            float* tempTolerance = m_pTolerance;
            m_pTolerance = nullptr;
            LoadFromXML(file, node);

            M3D_ASSERT(m_MaxID == oldMaxId);
            M3D_ASSERT(m_MinID == oldMinId);

            m_pDefaultTolerance = m_pTolerance;
            m_pTolerance = tempTolerance;
        }
        else
        {
            M3D_LOG_ERR("Error: No Relationship file: " + CStr(fileName) + err);
        }
    }

    int Relationship::GetMinBelong() const
    {
        // RVA 0x65A710
        return m_MinID;
    }

    void Relationship::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        delete[] m_pTolerance;
        m_pTolerance = nullptr;

        m3d::SafeIntAttrib(m_MinID, xmlNode, "MinPlayerID");
        m3d::SafeIntAttrib(m_MaxID, xmlNode, "MaxPlayerID");

        CStr tolName(xmlNode->GetAttribute("DefaultTolerance"));
        m_defaultTolerance = GetToleranceByName(tolName);

        if (m_MinID > m_MaxID)
            return;

        // NOTE: the guard overshoots the limit it is testing - a range wider than 1000 ids is
        // cut back to m_MinID + 1001, which is 1002 ids, not 1000.
        if (m_MaxID - m_MinID + 1 > 1000)
            m_MaxID = m_MinID + 1001;

        int const side = m_MaxID - m_MinID + 1;
        m_pTolerance = new float[side * side];
        for (int i = 0; i < side * side; ++i)
        {
            m_pTolerance[i] = (float)m_defaultTolerance;
        }

        int formatType = 0;
        m3d::SafeIntAttrib(formatType, xmlNode, "FormatType");
        if (!formatType)
        {
            _LoadFormat0(xmlFile, xmlNode);
        }
        else if (formatType == 1)
        {
            _LoadFormat1(xmlFile, xmlNode);
        }
        else
        {
            SYS_ERROR("!\"Invalid format type for relationship\"");
        }
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

    eTolerance Relationship::GetToleranceByName(CStr const& name) const
    {
        // RVA 0x65B350
        for (auto const& tolerance : m_toleranceList)
        {
            if (tolerance.m_name == name)
                return tolerance.m_tolerance;
        }
        return m_defaultTolerance;
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

    void Relationship::SaveToXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const
    {
        // RVA 0x65CC70
        xmlNode->SetAttribute("MinPlayerID", CStr(m_MinID).c_str());
        xmlNode->SetAttribute("MaxPlayerID", CStr(m_MaxID).c_str());
        xmlNode->SetAttribute("DefaultTolerance", GetNameByTolerance(m_defaultTolerance).c_str());

        if (!theObjects->m_SaveType)
        {
            _SaveFormat0(xmlFile, xmlNode);
        }
        else if (theObjects->m_SaveType == ObjContainer::SAVE_FULL)
        {
            _SaveFormat1(xmlFile, xmlNode);
        }
        else
        {
            // NOTE: SAVE_EDITOR has no writer, so saving from the editor is a fatal error.
            M3D_ASSERT(0);
        }
    }

    Relationship::~Relationship()
    {
        delete[] m_pTolerance;
        delete[] m_pDefaultTolerance;
    }

    int Relationship::GetMaxBelong() const
    {
        // RVA 0x65A720
        return m_MaxID;
    }

    eTolerance Relationship::CheckTolerance(int belongId1, int belongId2) const
    {
        if (belongId1 == belongId2)
            return RS_OWN;

        if (belongId1 < this->m_MinID)
            return this->m_defaultTolerance;

        if (belongId1 > m_MaxID || belongId2 < m_MinID || belongId2 > m_MaxID)
            return this->m_defaultTolerance;

        // The stored tolerance is a float, so report the listed step nearest to it.
        auto v10 = GetTolerance(belongId1, belongId2) - (double)(int)m_defaultTolerance;
        eTolerance NearestTol = m_defaultTolerance;
        float Min = fabs(v10);
        for (auto const& tolerance : m_toleranceList)
        {
            if (Min > fabs(GetTolerance(belongId1, belongId2) - (double)(int)tolerance.m_tolerance))
            {
                auto Tolerance = GetTolerance(belongId1, belongId2);
                NearestTol = tolerance.m_tolerance;
                Min = fabs(Tolerance - (double)(int)tolerance.m_tolerance);
            }
        }
        return NearestTol;
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

    void Relationship::SetTolerance(CStr const& list1, CStr const& list2, float value)
    {
        // RVA 0x65B5A0 - both attributes are space separated id lists, and every pair drawn
        // from them gets the value (the shipped build inlines the int overload here).
        retruxx::vector<int> intList1;
        retruxx::vector<int> intList2;
        StrToIntVector(list1, intList1);
        StrToIntVector(list2, intList2);

        for (auto belongId2 : intList2)
        {
            for (auto belongId1 : intList1)
            {
                SetTolerance(belongId1, belongId2, value);
            }
        }
    }

    CStr Relationship::GetNameByTolerance(eTolerance tolerance) const
    {
        // RVA 0x65B3B0 - falls back to the name of the default tolerance, and only then to a
        // literal when even that is missing from the list.
        for (auto const& info : m_toleranceList)
        {
            if (info.m_tolerance == tolerance)
                return info.m_name;
        }

        for (auto const& info : m_toleranceList)
        {
            if (info.m_tolerance == m_defaultTolerance)
                return info.m_name;
        }
        return CStr("Unknown");
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

    void Relationship::_LoadFormat1(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        // RVA 0x65ABF0 - <Belong id=".."> nested inside one another, the inner one carrying the
        // tolerance. Every pair is written both ways round, so the table stays symmetric.
        ref_ptr belongNode1 = xmlFile->CreateNode();
        for (xmlNode->GetFirstChild(belongNode1, "Belong"); !belongNode1->IsEmpty();
             belongNode1->GetNextSibling(belongNode1, "Belong"))
        {
            int belongId1 = -1;
            if (!m3d::SafeIntAttrib(belongId1, belongNode1, "id") || belongId1 == -1)
                continue;

            ref_ptr belongNode2 = xmlFile->CreateNode();
            for (belongNode1->GetFirstChild(belongNode2, "Belong"); !belongNode2->IsEmpty();
                 belongNode2->GetNextSibling(belongNode2, "Belong"))
            {
                int belongId2 = -1;
                if (!m3d::SafeIntAttrib(belongId2, belongNode2, "id") || belongId2 == -1)
                    continue;

                float tolerance = (float)m_defaultTolerance;
                m3d::SafeFloatAttrib(tolerance, belongNode2, "tolerance");
                SetTolerance(belongId1, belongId2, tolerance);
            }
        }
    }

    void Relationship::_LoadFormat0(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        // RVA 0x65B6D0 - flat <set who=".." forwhom=".." tolerance=".."/> rows, where who and
        // forwhom are id lists and the tolerance is named rather than numeric.
        ref_ptr node = xmlFile->CreateNode();
        for (xmlNode->GetFirstChild(node, "set"); !node->IsEmpty(); node->GetNextSibling(node, "set"))
        {
            CStr name(node->GetAttribute("tolerance"));
            CStr list2(node->GetAttribute("who"));
            CStr list1(node->GetAttribute("forwhom"));
            SetTolerance(list1, list2, (float)GetToleranceByName(name));
        }
    }

    int Relationship::GetIndex(int belongId) const
    {
        // RVA 0x65A750
        return belongId - m_MinID;
    }

    bool Relationship::IsValid(int belongId) const
    {
        // RVA 0x65A730
        return belongId >= m_MinID && belongId <= m_MaxID;
    }

    void Relationship::_SaveFormat1(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const
    {
        // RVA 0x65C8B0 - writes the exact float tolerances, so it round-trips through
        // _LoadFormat1 without snapping to the named steps.
        xmlNode->SetAttribute("FormatType", CStr(1).c_str());

        for (int belongId1 = m_MinID; belongId1 <= m_MaxID; ++belongId1)
        {
            // Only the upper triangle is walked; the table is symmetric and the loader
            // writes both halves back.
            retruxx::vector<int> belongs;
            for (int belongId2 = belongId1 + 1; belongId2 <= m_MaxID; ++belongId2)
            {
                if (fabs(GetTolerance(belongId1, belongId2) - (double)(int)m_defaultTolerance) > 0.001)
                    belongs.push_back(belongId2);
            }

            if (belongs.empty())
                continue;

            ref_ptr belongNode1 = xmlFile->CreateNode(m3d::cmn::XML_NODE_ELEMENT, "Belong");
            belongNode1->SetAttribute("id", CStr(belongId1).c_str());

            for (auto belongId2 : belongs)
            {
                ref_ptr belongNode2 = xmlFile->CreateNode(m3d::cmn::XML_NODE_ELEMENT, "Belong");
                belongNode2->SetAttribute("id", CStr(belongId2).c_str());
                belongNode2->SetAttribute("tolerance", CStr(GetTolerance(belongId1, belongId2)).c_str());
                belongNode1->AddChild(belongNode2);
            }
            xmlNode->AddChild(belongNode1);
        }
    }

    void Relationship::_SaveFormat0(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const
    {
        // RVA 0x65C490 - one <set> row per (belong, tolerance) pair, so the float tolerances
        // are snapped to the nearest named step on the way out.
        xmlNode->SetAttribute("FormatType", CStr(0).c_str());

        for (int belongId1 = m_MinID; belongId1 <= m_MaxID; ++belongId1)
        {
            for (auto const& info : m_toleranceList)
            {
                // The default tolerance is what a missing row means, so it is never written.
                if (info.m_tolerance == m_defaultTolerance)
                    continue;

                // Only the upper triangle is walked; the table is symmetric.
                retruxx::vector<int> belongs;
                for (int belongId2 = belongId1 + 1; belongId2 <= m_MaxID; ++belongId2)
                {
                    if (CheckTolerance(belongId1, belongId2) == info.m_tolerance)
                        belongs.push_back(belongId2);
                }

                if (belongs.empty())
                    continue;

                ref_ptr node = xmlFile->CreateNode(m3d::cmn::XML_NODE_ELEMENT, "set");
                node->SetAttribute("forwhom", CStr(belongId1).c_str());

                CStr who;
                for (unsigned i = 0; i < belongs.size(); ++i)
                {
                    who += CStr(belongs[i]) + CStr(i == belongs.size() - 1 ? "" : " ");
                }

                node->SetAttribute("who", who.c_str());
                node->SetAttribute("tolerance", info.m_name.c_str());
                xmlNode->AddChild(node);
            }
        }
    }
}  // namespace ai
