#pragma once
#include <server/components/numericinrangeregenerating.h>
#include <server/objects/base/shell.h>

namespace ai
{
    class PlasmaBunchPrototypeInfo : public ai::ShellPrototypeInfo
    {
    public:
        /* 0x0080 */ float m_velocity;
        /* 0x0084 */ float m_acceleration;
        /* 0x0088 */ float m_flyTime;
        /* 0x008c */ int m_BlastWavePrototypeId;
        PlasmaBunchPrototypeInfo();
        virtual bool LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override /* 0x04 */;
        virtual void PostLoad() override /* 0x00 */;
        virtual ai::Obj* CreateTargetObject() const override /* 0x00 */;

    private:
        /* 0x0090 */ CStr m_BlastWavePrototypeName;
    }; /* size: 0x009c */

    static_assert(sizeof(PlasmaBunchPrototypeInfo) == 0x009c);

    class PlasmaBunch : public ai::Shell
    {
        friend class PlasmaBunchPrototypeInfo;

    protected:
        virtual ~PlasmaBunch() override /* 0x00 */;

    private:
        PlasmaBunch(const ai::PlasmaBunchPrototypeInfo& prototypeInfo);
        PlasmaBunch(const ai::PlasmaBunch&);
        virtual m3d::Object* Clone() override /* 0x00 */;
        static m3d::Object* __fastcall CreateObject();

    public:
        static m3d::Class* __fastcall GetBaseClass();
        virtual m3d::Class* GetClass() const override /* 0x00 */;
        RT_CLASS_DECLARE(PlasmaBunch);
        virtual const ai::PlasmaBunchPrototypeInfo* GetPrototypeInfo() const override /* 0x00 */;
        float GetVelocity() const;
        void SetVelocity(float value);
        virtual void Update(float elapsedTime, unsigned int workTime) override /* 0x00 */;
        virtual void Remove() override /* 0x00 */;
        virtual void TransferPhysicParamsToSceneGraphNode() override /* 0x00 */;
        virtual void RenderDebugInfo() const override /* 0x00 */;
        virtual void LoadRuntimeValues(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override /* 0x00 */;
        virtual void SaveRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const override /* 0x00 */;

    private:
        /* 0x0150 */ ai::NumericInRangeRegenerating<float> m_velocity;
        /* 0x0228 */ ai::NumericInRangeRegenerating<float> m_lifeTime;
    }; /* size: 0x0300 */

    static_assert(sizeof(PlasmaBunch) == 0x0300);
}
