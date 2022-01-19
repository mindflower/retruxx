#pragma once
#include "base/obj.h"
#include "base/prototypeinfo.h"

namespace ai
{
    class NpcPrototypeInfo : public PrototypeInfo
    {
    public:
        NpcPrototypeInfo();
        virtual Obj* CreateTargetObject() const;
        virtual bool LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);

    };

    class Npc :  public Obj
    {
    public:
        enum NpcType
        {
            NPC_BARMAN = 0x0,
            NPC_CLIENT = 0x1,
        };

    public:
        bool HasDynamicQuests() const ;
        virtual void LoadRuntimeValues(m3d::cmn::XmlFile *,m3d::cmn::XmlNode const *);
        unsigned int GetCfgNumber() const ;
        unsigned int GetSkinNumber() const ;
        static void __fastcall SetCurrentNpc(Npc *);
        virtual void LoadFromXML(m3d::cmn::XmlFile *,m3d::cmn::XmlNode const *);
        void SetSpokenCount(int);
        virtual NpcPrototypeInfo const * GetPrototypeInfo() const ;
        virtual m3d::Class * GetClass() const ;
        virtual CStr GetPropertyName(int) const ;
        virtual void GetPropertiesIDs(std::set<int,std::less<int>,std::allocator<int> > &) const ;
        virtual void GetPropertiesNames(std::set<CStr,std::less<CStr>,std::allocator<CStr> > &) const ;
        static Npc * __fastcall GetCurrentNpc();
        int GetSpokenCount() const ;
        virtual bool SetPropertyById(int,m3d::AIParam const &);
        void SetHelloReplyNames(std::vector<CStr,std::allocator<CStr> > const &);
        virtual void SaveRuntimeValues(m3d::cmn::XmlFile *,m3d::cmn::XmlNode *) const ;
        static m3d::Class * GetBaseClass();
        CStr const & GetModelName() const ;
        virtual int GetPropertyId(char const *) const ;
        bool HasNotTakenDynamicQuests() const ;
        std::vector<CStr,std::allocator<CStr> > const & GetHelloReplyNames() const ;
        virtual void SaveToXML(m3d::cmn::XmlFile *,m3d::cmn::XmlNode *) const ;
        static void __fastcall Registration();
        virtual eGObjPropertySaveStatus GetPropertySaveStatus(int) const ;
        NpcType GetNpcType() const ;
        Npc(NpcPrototypeInfo const &);

    protected:
        static void __fastcall RegisterProperty(char const *,int,eGObjPropertySaveStatus);
        virtual bool _GetPropertyInternal(int,m3d::AIParam &) const ;
        virtual bool _GetPropertyDefaultInternal(int,m3d::AIParam &) const ;
        virtual ~Npc();
        virtual void _InternalPostLoad();

    private:
        static m3d::Object * CreateObject();
        virtual m3d::Object * Clone();

    public:
        RT_CLASS_INLINE_DECLARE(Npc);

    private:
        NpcType m_npcType;
        CStr m_ModelName;
        unsigned int m_SkinNumber;
        unsigned int m_CfgNumber;
        std::vector<CStr> m_helloReplyNames;
        int m_spokenCount;
    };
}
