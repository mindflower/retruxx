#pragma once
#include "rocket.h"

namespace ai
{
    class MinePrototypeInfo : public RocketPrototypeInfo
    {
    public:
        float m_TTL;
        float m_TimeForActivation;
        MinePrototypeInfo();
        virtual Obj* CreateTargetObject() const override;
        virtual bool LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override;
    };

    class Mine : public Rocket
    {
        // MinePrototypeInfo::CreateTargetObject builds a mine from its prototype.
        friend class MinePrototypeInfo;

    protected:
        virtual  ~Mine() override;

    private:
        Mine(const MinePrototypeInfo& prototypeInfo);
        Mine(const Mine&);
        virtual Object* Clone() override;
        static Object* CreateObject();

    public:
        static m3d::Class* GetBaseClass();
        virtual m3d::Class* GetClass() const override;
        static m3d::Class m_classMine;
        virtual const MinePrototypeInfo* GetPrototypeInfo() const override;
        virtual void LoadRuntimeValues(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override;
        virtual void SaveRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const override;
        virtual void Update(float elapsedTime, unsigned int workTime) override;

        enum MineState
        {
            msActivation = 0,
            msWaiting = 1,
        };

    public:
        MineState getState();

    private:
        float m_TL;
        float m_yVelocity;
        MineState m_MineState;
    };
}
