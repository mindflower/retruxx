#pragma once
#include "location.h"

namespace ai
{
    class TemporaryLocationPrototypeInfo : public LocationPrototypeInfo
    {
    public:
        TemporaryLocationPrototypeInfo();
        virtual bool LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override /* 0x04 */;
        virtual ai::Obj* CreateTargetObject() const override /* 0x00 */;
        /* 0x0080 */ float m_TTL;
        /* 0x0084 */ float m_TimeForActivation;
        CStr m_EffectName;
    }; /* size: 0x0094 */

    class TemporaryLocation : public ai::Location
    {
    protected:
        virtual  ~TemporaryLocation() override = 0 /* 0x00 */;

        TemporaryLocation(const ai::TemporaryLocationPrototypeInfo& prototypeInfo);
        virtual m3d::Object* Clone() override /* 0x00 */;
        static m3d::Object* __fastcall CreateObject();

    public:
        static m3d::Class* __fastcall GetBaseClass();
        virtual m3d::Class* GetClass() const override /* 0x00 */;
        static m3d::Class m_classTemporaryLocation;
        virtual const ai::TemporaryLocationPrototypeInfo* GetPrototypeInfo() const override /* 0x00 */;

        enum TemporaryLocationState
        {
            tlsActivation = 0,
            tlsWaiting = 1,
        };

    public:
        virtual void Update(float elapsedTime, unsigned int workTime) override /* 0x00 */;
        virtual void Remove() override /* 0x00 */;
        virtual void LoadRuntimeValues(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override /* 0x00 */;
        virtual void SaveRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const override /* 0x00 */;
        void CreateEffectNode();

    private:
        virtual void OnActivate() /* 0x1bc */;
        /* 0x027c */ float m_TL;
        /* 0x0280 */ ai::TemporaryLocation::TemporaryLocationState m_TemporaryLocationState;
    }; /* size: 0x0284 */
}
