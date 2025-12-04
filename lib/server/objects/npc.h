#pragma once
#include "base/obj.h"
#include "base/prototypeinfo.h"

namespace ai
{
    class NpcPrototypeInfo : public ai::PrototypeInfo
    {
    public:
        NpcPrototypeInfo();
        virtual bool LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override /* 0x04 */;
        virtual ai::Obj* CreateTargetObject() const override /* 0x10 */;
    }; /* size: 0x0040 */

    static_assert(sizeof(NpcPrototypeInfo) == 0x0040);

    class Npc : public ai::Obj
    {
    protected:
        virtual  ~Npc() override /* 0x00 */;

    private:
        Npc(const ai::NpcPrototypeInfo& prototypeInfo);
        Npc(const ai::Npc&);
        virtual m3d::Object* Clone() override /* 0x00 */;
        static m3d::Object* __fastcall CreateObject();

    public:
        static m3d::Class* __fastcall GetBaseClass();
        virtual m3d::Class* GetClass() const override /* 0x00 */;
        static m3d::Class m_classNpc;
        virtual const ai::NpcPrototypeInfo* GetPrototypeInfo() const override /* 0x4c */;

    protected:
        static void __fastcall RegisterProperty(const char* Name, int id, ai::eGObjPropertySaveStatus saveStatus);

    public:
        virtual ai::eGObjPropertySaveStatus GetPropertySaveStatus(int id) const override /* 0x58 */;
        virtual void GetPropertiesNames(retruxx::set<CStr, retruxx::less<CStr>, retruxx::allocator<CStr> >& Props) const override /* 0x5c */;
        virtual void GetPropertiesIDs(retruxx::set<int, retruxx::less<int>, retruxx::allocator<int> >& Props) const override /* 0x60 */;
        virtual CStr GetPropertyName(int id) const override /* 0x78 */;
        virtual bool SetPropertyById(int propertyId, const m3d::AIParam& newValue) override /* 0x7c */;
        virtual int GetPropertyId(const char* PropertyName) const override /* 0x74 */;

    protected:
        static inline retruxx::map<CStr, int, ai::Obj::LessNoCaseCStr, retruxx::allocator<retruxx::pair<CStr const, int> > > m_propertiesMap;
        static inline retruxx::map<int, enum ai::eGObjPropertySaveStatus, retruxx::less<int>, retruxx::allocator<retruxx::pair<int const, enum ai::eGObjPropertySaveStatus> > > m_propertiesSaveStatesMap;
        virtual bool _GetPropertyDefaultInternal(int propertyId, m3d::AIParam& retVal) const override /* 0x10c */;
        virtual bool _GetPropertyInternal(int propertyId, m3d::AIParam& retVal) const override /* 0x108 */;

        enum NpcType;

    public:
        const retruxx::vector<CStr, retruxx::allocator<CStr> >& GetHelloReplyNames() const;
        void SetHelloReplyNames(const retruxx::vector<CStr, retruxx::allocator<CStr> >& replyNames);
        bool HasDynamicQuests() const;
        bool HasNotTakenDynamicQuests() const;
        ai::Npc::NpcType GetNpcType() const;
        const CStr& GetModelName() const;
        unsigned int GetSkinNumber() const;
        unsigned int GetCfgNumber() const;
        virtual void LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override /* 0xac */;
        virtual void LoadRuntimeValues(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override /* 0xb0 */;
        virtual void SaveToXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const override /* 0xb4 */;
        virtual void SaveRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const override /* 0xb8 */;
        int GetSpokenCount() const;
        void SetSpokenCount(int spokenCount);

    protected:
        virtual void _InternalPostLoad() override /* 0xfc */;

    private:
        /* 0x00c0 */ ai::Npc::NpcType m_npcType;
        /* 0x00c4 */ CStr m_ModelName;
        /* 0x00d0 */ unsigned int m_SkinNumber;
        /* 0x00d4 */ unsigned int m_CfgNumber;
        /* 0x00d8 */ retruxx::vector<CStr, retruxx::allocator<CStr> > m_helloReplyNames;
        /* 0x00e8 */ int m_spokenCount;
        static ai::Npc* theCurrentNpc;

    public:
        static void __fastcall SetCurrentNpc(ai::Npc* npc);
        static ai::Npc* __fastcall GetCurrentNpc();
        static void __fastcall Registration();
    }; /* size: 0x00ec */

    static_assert(sizeof(Npc) == 0x00ec);
}
