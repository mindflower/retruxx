#pragma once
#include "base/obj.h"
#include "base/prototypeinfo.h"

namespace ai
{
    class NpcPrototypeInfo : public ai::PrototypeInfo
    {
    public:
        NpcPrototypeInfo();
        virtual bool LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode) override;
        virtual ai::Obj* CreateTargetObject() const override;
    };

    class Npc : public ai::Obj
    {
        friend class NpcPrototypeInfo;
        // Bar::CreateBarman stamps the npc type onto the barman it has just created.
        friend class Bar;

    protected:
        virtual ~Npc() override;

    private:
        Npc(ai::NpcPrototypeInfo const& prototypeInfo);
        Npc(ai::Npc const&);
        virtual m3d::Object* Clone() override;
        static m3d::Object* __fastcall CreateObject();

    public:
        static m3d::Class* __fastcall GetBaseClass();
        virtual m3d::Class* GetClass() const override;
        static m3d::Class m_classNpc;
        virtual ai::NpcPrototypeInfo const* GetPrototypeInfo() const override;

    protected:
        static void __fastcall RegisterProperty(char const* Name, int id, ai::eGObjPropertySaveStatus saveStatus);

    public:
        virtual ai::eGObjPropertySaveStatus GetPropertySaveStatus(int id) const override;
        virtual void GetPropertiesNames(
            retruxx::set<CStr, retruxx::less<CStr>, retruxx::allocator<CStr>>& Props) const override;
        virtual void GetPropertiesIDs(
            retruxx::set<int, retruxx::less<int>, retruxx::allocator<int>>& Props) const override;
        virtual CStr GetPropertyName(int id) const override;
        virtual bool SetPropertyById(int propertyId, m3d::AIParam const& newValue) override;
        virtual int GetPropertyId(char const* PropertyName) const override;

    protected:
        static inline retruxx::
            map<CStr, int, ai::Obj::LessNoCaseCStr, retruxx::allocator<retruxx::pair<CStr const, int>>>
                m_propertiesMap;
        static inline retruxx::map<
            int,
            enum ai::eGObjPropertySaveStatus,
            retruxx::less<int>,
            retruxx::allocator<retruxx::pair<int const, enum ai::eGObjPropertySaveStatus>>>
            m_propertiesSaveStatesMap;
        virtual bool _GetPropertyDefaultInternal(int propertyId, m3d::AIParam& retVal) const override;
        virtual bool _GetPropertyInternal(int propertyId, m3d::AIParam& retVal) const override;

    public:
        enum NpcType
        {
            NPC_BARMAN = 0,
            NPC_CLIENT = 1,
        };

    public:
        retruxx::vector<CStr, retruxx::allocator<CStr>> const& GetHelloReplyNames() const;
        void SetHelloReplyNames(retruxx::vector<CStr, retruxx::allocator<CStr>> const& replyNames);
        bool HasDynamicQuests() const;
        bool HasNotTakenDynamicQuests() const;
        ai::Npc::NpcType GetNpcType() const;
        CStr const& GetModelName() const;
        unsigned int GetSkinNumber() const;
        unsigned int GetCfgNumber() const;
        virtual void LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode) override;
        virtual void LoadRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode) override;
        virtual void SaveToXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const override;
        virtual void SaveRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const override;
        int GetSpokenCount() const;
        void SetSpokenCount(int spokenCount);

    protected:
        virtual void _InternalPostLoad() override;

    private:
        ai::Npc::NpcType m_npcType;
        CStr m_ModelName;
        unsigned int m_SkinNumber;
        unsigned int m_CfgNumber;
        retruxx::vector<CStr, retruxx::allocator<CStr>> m_helloReplyNames;
        int m_spokenCount;
        static inline ai::Npc* theCurrentNpc = nullptr;

    public:
        static void __fastcall SetCurrentNpc(ai::Npc* npc);
        static ai::Npc* __fastcall GetCurrentNpc();
        static void __fastcall Registration();
    };
}  // namespace ai
