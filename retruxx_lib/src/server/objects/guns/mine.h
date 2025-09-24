#pragma once
#include "rocket.h"

namespace ai
{
    class MinePrototypeInfo : public RocketPrototypeInfo
    {
    public:
        /* 0x00a0 */ float m_TTL;
        /* 0x00a4 */ float m_TimeForActivation;
        MinePrototypeInfo();
        virtual Obj* CreateTargetObject() const override /* 0x00 */;
        virtual bool LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override /* 0x04 */;
    }; /* size: 0x00a8 */

    class Mine : public Rocket
    {
    protected:
        virtual  ~Mine() override /* 0x00 */;

    private:
        Mine(const MinePrototypeInfo& prototypeInfo);
        Mine(const Mine&);
        virtual Object* Clone() override /* 0x00 */;
        static Object* CreateObject();

    public:
        static m3d::Class* GetBaseClass();
        virtual m3d::Class* GetClass() const override /* 0x00 */;
        static m3d::Class m_classMine;
        virtual const MinePrototypeInfo* GetPrototypeInfo() const override /* 0x00 */;
        virtual void LoadRuntimeValues(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override /* 0x00 */;
        virtual void SaveRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const override /* 0x00 */;
        virtual void Update(float elapsedTime, unsigned int workTime) override /* 0x00 */;

        enum MineState;

    public:
        MineState getState();

    private:
        /* 0x032c */ float m_TL;
        /* 0x0330 */ float m_yVelocity;
        /* 0x0334 */ MineState m_MineState;
    }; /* size: 0x0338 */
}
