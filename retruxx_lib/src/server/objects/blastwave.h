#pragma once
#include "base/simplephysicobj.h"
#include <ode/contact.h>

namespace ai
{
    class BlastWavePrototypeInfo : public ai::SimplePhysicObjPrototypeInfo
    {
    protected:
        virtual void _InternalCopyFrom(const ai::PrototypeInfo& rhs) override /* 0x00 */;

    public:
        BlastWavePrototypeInfo();
        /* 0x0080 */ float m_WaveForceIntensity;
        /* 0x0084 */ float m_WaveDamageIntensity;
        /* 0x0088 */ CStr m_EffectName;
        virtual bool LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override /* 0x04 */;
        virtual ai::Obj* CreateTargetObject() const override /* 0x00 */;
    }; /* size: 0x0094 */
    static_assert(sizeof(BlastWavePrototypeInfo) == 0x0094);


    class BlastWave : public ai::SimplePhysicObj
    {
    protected:
        virtual  ~BlastWave() override /* 0x00 */;

    private:
        BlastWave(const ai::BlastWavePrototypeInfo& prototypeInfo);
        BlastWave(const ai::BlastWave&);
        virtual m3d::Object* Clone() override /* 0x00 */;
        static m3d::Object* __fastcall CreateObject();

    public:
        static m3d::Class* __fastcall GetBaseClass();
        virtual m3d::Class* GetClass() const override /* 0x00 */;
        static m3d::Class m_classBlastWave;
        virtual const ai::BlastWavePrototypeInfo* GetPrototypeInfo() const override /* 0x00 */;

    protected:
        static void __fastcall RegisterProperty(const char*, int, ai::eGObjPropertySaveStatus);

    public:
        virtual ai::eGObjPropertySaveStatus GetPropertySaveStatus(int id) const override /* 0x00 */;
        virtual void GetPropertiesNames(retruxx::set<CStr, retruxx::less<CStr>, retruxx::allocator<CStr> >& Props) const override /* 0x00 */;
        virtual void GetPropertiesIDs(retruxx::set<int, retruxx::less<int>, retruxx::allocator<int> >& Props) const override /* 0x00 */;
        virtual CStr GetPropertyName(int id) const override /* 0x00 */;
        virtual bool SetPropertyById(int propertyId, const m3d::AIParam& newValue) override /* 0x00 */;
        virtual int GetPropertyId(const char* PropertyName) const override /* 0x00 */;

    protected:
        static retruxx::map<CStr, int, ai::Obj::LessNoCaseCStr, retruxx::allocator<retruxx::pair<CStr const, int> > > m_propertiesMap;
        static retruxx::map<int, enum ai::eGObjPropertySaveStatus, retruxx::less<int>, retruxx::allocator<retruxx::pair<int const, enum ai::eGObjPropertySaveStatus> > > m_propertiesSaveStatesMap;
        virtual bool _GetPropertyDefaultInternal(int propertyId, m3d::AIParam& retVal) const override /* 0x00 */;
        virtual bool _GetPropertyInternal(int propertyId, m3d::AIParam& retVal) const override /* 0x00 */;

    public:
        virtual void LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override /* 0x00 */;
        virtual void Update(float elapsedTime, unsigned int workTime) override /* 0x00 */;
        static int __fastcall CollideBlastWaveAndPhysicObj(ai::BlastWave* blastWave, ai::Obj* obj, dContact* contact, unsigned int& numContacts, bool reverse);
        float GetWaveForceIntensity() const;
        float GetWaveDamageIntensity() const;
        short GetRocketExplosionType() const;
        void SetRocketExplosionType(short explosionType);
        float GetBlastRadius() const;
        float GetDampingCoefficeient(float dist) const;
        float GetDampedDamageIntensity(float dist) const;
        float GetDampedForceIntensity(float dist) const;
        void SetRocketId(int rocketId);
        int GetEmitterId() const;
        void SetEmitterId(int emitterId);

    private:
        /* 0x0144 */ float m_WaveForceIntensity;
        /* 0x0148 */ float m_WaveDamageIntensity;
        /* 0x014c */ int m_frameWhenCreated;
        /* 0x0150 */ unsigned int m_Frame;
        /* 0x0154 */ short m_rocketExplosionType;
        /* 0x0156 */ char Padding_200[2];
        /* 0x0158 */ m3d::SgNode* m_EffectNode;
        /* 0x015c */ int m_emitterId;
        /* 0x0160 */ bool m_bCollided;
    }; /* size: 0x0164 */

    static_assert(sizeof(BlastWave) == 0x0164);
}
