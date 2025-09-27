#pragma once
#include <set>
#include <vector>
#include <core/stringm3d.h>

namespace m3d
{
    namespace cmn
    {
        struct XmlNode;
        class XmlFile;
    }
}

namespace ai
{
    enum eTolerance
    {
        RS_ENEMY = 0x1,
        RS_NEUTRAL = 0x2,
        RS_ALLY = 0x3,
        RS_OWN = 0x4,
        RS_MAX = 0x5,
    };

    class Relationship
    {
    public:
        class auxToleranceInfo
        {
        public:
            auxToleranceInfo(ai::eTolerance tolerance, CStr name);

        public:
            eTolerance m_tolerance;
            CStr m_name;
        };

    public:
        void LoadDefaultFromXmlFile(char const*);
        int GetMinBelong() const;
        void LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);
        bool AmongTolerance(int, int, std::set<eTolerance> const&) const;
        Relationship();
        eTolerance GetToleranceByName(CStr const&) const;
        void LoadFromXmlFile(char const*);
        float GetDefaultTolerance(int, int) const;
        void IncTolerance(int, int, float);
        void SaveToXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const;
        ~Relationship();
        int GetMaxBelong() const;
        eTolerance CheckTolerance(int, int) const;
        eTolerance CheckDefaultTolerance(int, int) const;
        void SetTolerance(int, int, float);
        void SetTolerance(CStr const&, CStr const&, float);
        CStr GetNameByTolerance(eTolerance) const;
        float GetTolerance(int, int) const;

    private:
        void _LoadFormat1(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);
        void _LoadFormat0(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);
        int GetIndex(int) const;
        bool IsValid(int) const;
        void _SaveFormat1(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const;
        void _SaveFormat0(m3d::cmn::XmlFile*, m3d::cmn::XmlNode*) const;

    private:
        int m_MinID = 0;
        int m_MaxID = -1;
        eTolerance m_defaultTolerance = RS_ENEMY;
        std::vector<auxToleranceInfo> m_toleranceList;
        float* m_pTolerance = nullptr;
        float* m_pDefaultTolerance = nullptr;
    };

    inline Relationship* theRelationship = nullptr;
}
