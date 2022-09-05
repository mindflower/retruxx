#pragma once
#include <server/components/numericinrangeregenerating.h>
#include <server/objects/base/shell.h>

namespace ai
{
    class PlasmaBunchPrototypeInfo : public ShellPrototypeInfo
    {
    public:
        PlasmaBunchPrototypeInfo();
        virtual void PostLoad();
        virtual bool LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);
        virtual Obj* CreateTargetObject() const;

    private:
        float m_velocity;
        float m_acceleration;
        float m_flyTime;
        int m_BlastWavePrototypeId;
        CStr m_BlastWavePrototypeName;

    };
    class PlasmaBunch : public Shell
    {
    public:
        virtual void SaveRuntimeValues(m3d::cmn::XmlFile *,m3d::cmn::XmlNode *) const ;
        PlasmaBunch(PlasmaBunchPrototypeInfo const &);
        virtual void Remove();
        static m3d::Class * GetBaseClass();
        void SetVelocity(float);
        virtual void RenderDebugInfo() const ;
        virtual void TransferPhysicParamsToSceneGraphNode();
        virtual void LoadRuntimeValues(m3d::cmn::XmlFile *,m3d::cmn::XmlNode const *);
        virtual m3d::Class * GetClass() const ;
        virtual PlasmaBunchPrototypeInfo const * GetPrototypeInfo() const ;
        virtual void Update(float,unsigned int);

    protected:
        virtual ~PlasmaBunch();

    private:
        virtual m3d::Object * Clone();
        static m3d::Object * CreateObject();

    public:
        RT_CLASS_DECLARE(PlasmaBunch);

    private:
        //NumericInRangeRegenerating<float> m_velocity;
        //NumericInRangeRegenerating<float> m_lifeTime;
    };
}
