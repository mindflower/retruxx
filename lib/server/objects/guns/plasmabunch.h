#pragma once
#include <server/components/numericinrangeregenerating.h>
#include <server/objects/base/shell.h>

namespace ai
{
    class PlasmaBunchPrototypeInfo : public ai::ShellPrototypeInfo
    {
    public:
        float m_velocity;
        float m_acceleration;
        float m_flyTime;
        int m_BlastWavePrototypeId;
        PlasmaBunchPrototypeInfo();
        virtual bool LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override;
        virtual void PostLoad() override;
        virtual ai::Obj* CreateTargetObject() const override;

    private:
        CStr m_BlastWavePrototypeName;
    };

    class PlasmaBunch : public ai::Shell
    {
        friend class PlasmaBunchPrototypeInfo;

    protected:
        virtual ~PlasmaBunch() override;

    private:
        PlasmaBunch(const ai::PlasmaBunchPrototypeInfo& prototypeInfo);
        PlasmaBunch(const ai::PlasmaBunch&);
        virtual m3d::Object* Clone() override;
        static m3d::Object* __fastcall CreateObject();

    public:
        static m3d::Class* __fastcall GetBaseClass();
        virtual m3d::Class* GetClass() const override;
        RT_CLASS_DECLARE(PlasmaBunch);
        virtual const ai::PlasmaBunchPrototypeInfo* GetPrototypeInfo() const override;
        float GetVelocity() const;
        void SetVelocity(float value);
        virtual void Update(float elapsedTime, unsigned int workTime) override;
        virtual void Remove() override;
        virtual void TransferPhysicParamsToSceneGraphNode() override;
        virtual void RenderDebugInfo() const override;
        virtual void LoadRuntimeValues(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override;
        virtual void SaveRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const override;

    private:
        ai::NumericInRangeRegenerating<float> m_velocity;
        ai::NumericInRangeRegenerating<float> m_lifeTime;
    };
}
