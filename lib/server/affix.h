#pragma once
#include <core/stringm3d.h>
#include <vector>

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
    class Obj;
    class AffixGroup;

    enum AffixType
    {
        AFFIXTYPE_PREFIX = 0x0,
        AFFIXTYPE_SUFFIX = 0x1,
    };

    class Affix
    {
    public:
        class ModificationInfo
        {
        public:
            bool ApplyToObj(Affix const*, Obj*) const;
            ModificationInfo(CStr const&);

        private:
            CStr m_propertyName;
            float m_lowCoeff;
            float m_highCoeff;
        };

    public:
        bool ApplyToObj(Obj*) const;
        void PostLoad();
        Affix(AffixGroup*);
        bool LoadFromXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);
        AffixGroup const* GetAffixGroup() const;
        int GetId() const;
        CStr const& GetName() const;
        AffixType GetAffixType() const;
        CStr GetLocalizedName(unsigned int) const;

        // AffixGeneratorPrototypeInfo::GenerateAffixesForObj reads the group of an affix it has
        // just applied directly, to strike that group's other affixes off its candidate list.
        friend class AffixGeneratorPrototypeInfo;
        // The manager hands out affix ids and matches affixes by name; the group creates them.
        friend class AffixManager;
        friend class AffixGroup;

    private:
        int m_affixId;
        AffixGroup* m_pAffixGroup;
        CStr m_name;
        std::vector<ModificationInfo> m_modifications;
    };

    class AffixManager
    {
    public:
        int GetAffixIdByNameAndResource(CStr const&, int) const;
        bool LoadFromXmlFile(char const*);
        ~AffixManager();
        AffixManager();
        AffixGroup* GetAffixGroupById(int) const;
        void GetAffixGroupsByResourceId(int, class std::vector<int>&) const;
        int GetNumAffixes() const;
        void AddAffix(Affix*);
        int GetAffixGroupIdByName(CStr const&) const;
        Affix* GetAffixById(int) const;

    private:
        std::vector<AffixGroup*> m_affixGroups;
        std::vector<Affix*> m_affixes;
    };

    class AffixGroup
    {
    public:
        int GetId() const;
        AffixGroup(AffixManager*);
        AffixType GetAffixType() const;
        int GetRandomAffixId() const;
        std::vector<int> const& GetAffixIds() const;
        CStr const& GetName() const;
        ~AffixGroup();
        bool LoadFromXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);
        int GetTargetResourceId() const;

        friend class AffixGeneratorPrototypeInfo;
        // The manager stamps a group with its id, resource and type after loading it; an affix
        // reads its own type from the group it belongs to.
        friend class AffixManager;
        friend class Affix;

    private:
        AffixManager* m_pAffixManager;
        int m_affixGroupId;
        CStr m_name;
        int m_order;
        int m_targetResourceId;
        std::vector<int> m_affixIds;
        AffixType m_affixType;
    };
}
