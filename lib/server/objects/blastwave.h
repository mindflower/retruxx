#pragma once
#include "base/simplephysicobj.h"
#include <ode/contact.h>

namespace ai
{
    class BlastWavePrototypeInfo : public ai::SimplePhysicObjPrototypeInfo
    {
    protected:
        virtual void _InternalCopyFrom(ai::PrototypeInfo const& rhs) override;

    public:
        BlastWavePrototypeInfo();
        float m_WaveForceIntensity;
        float m_WaveDamageIntensity;
        CStr m_EffectName;
        virtual bool LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode) override;
        virtual ai::Obj* CreateTargetObject() const override;
    };

    class BlastWave : public ai::SimplePhysicObj
    {
        // BlastWavePrototypeInfo::CreateTargetObject constructs the BlastWave.
        friend class BlastWavePrototypeInfo;

    protected:
        virtual ~BlastWave() override;

    private:
        BlastWave(ai::BlastWavePrototypeInfo const& prototypeInfo);
        BlastWave(ai::BlastWave const&);
        virtual m3d::Object* Clone() override;
        static m3d::Object* CreateObject();

    public:
        static m3d::Class* GetBaseClass();
        virtual m3d::Class* GetClass() const override;
        static m3d::Class m_classBlastWave;
        virtual ai::BlastWavePrototypeInfo const* GetPrototypeInfo() const override;

    protected:
        static void RegisterProperty(char const*, int, ai::eGObjPropertySaveStatus);

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
        virtual void LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode) override;
        virtual void Update(float elapsedTime, unsigned int workTime) override;
        static int CollideBlastWaveAndPhysicObj(
            ai::BlastWave* blastWave,
            ai::Obj* obj,
            dContact* contact,
            unsigned int& numContacts,
            bool reverse);
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
        float m_WaveForceIntensity;
        float m_WaveDamageIntensity;
        int m_frameWhenCreated;
        unsigned int m_Frame;
        short m_rocketExplosionType;
        m3d::SgNode* m_EffectNode;
        int m_emitterId;
        bool m_bCollided;
    };
}  // namespace ai
