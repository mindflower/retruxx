#include "mine.h"

#include "server/objects/base/prototypemanager.h"
#include "server/utils.h"

#include <core/kernel.h>
#include <math/vector.h>
#include <stdexcept>

namespace ai
{
    RT_CLASS_EXPORTS_BEGIN(Mine)
    RT_CLASS_EXPORTS_END;
    RT_CLASS_DEFINE(Mine);

    MinePrototypeInfo::MinePrototypeInfo()
    {
        m_TTL = 100.0f;
        m_TimeForActivation = 0.0f;
    }

    Obj* MinePrototypeInfo::CreateTargetObject() const
    {
        // RVA 0x847CC0
        return new Mine(*this);
    }

    bool MinePrototypeInfo::LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        bool const result = RocketPrototypeInfo::LoadFromXML(xmlFile, xmlNode);
        if (result)
        {
            m3d::SafeFloatAttrib(m_TTL, xmlNode, "TTL");
            m3d::SafeFloatAttrib(m_TimeForActivation, xmlNode, "TimeForActivation");
        }
        return result;
    }

    Mine::~Mine()
    {
        // RVA 0x847C70 - nothing of its own to release.
    }

    Mine::Mine(MinePrototypeInfo const& prototypeInfo) : Rocket(prototypeInfo)
    {
        // RVA 0x847C20 - a mine starts its arming countdown and, unlike other shells, has its
        // geometry live from the moment it is created so that it can be driven over.
        m_TL = 0.0f;
        m_yVelocity = 0.0f;
        m_MineState = msActivation;
        EnableGeometry(true);
    }

    Mine::Mine(Mine const& mine) : Rocket(mine)
    {
        // NOTE: the header declares this but the shipped build never emitted it - a mine is
        // only ever created from its prototype - so unlike the other constructor its body does
        // not come from the binary. A memberwise copy is the only thing it can have been.
        m_TL = mine.m_TL;
        m_yVelocity = mine.m_yVelocity;
        m_MineState = mine.m_MineState;
    }

    m3d::Object* Mine::Clone()
    {
        // RVA 0x847FC0
        SYS_ERROR("!\"Object cannot be cloned\"");
        return nullptr;
    }

    m3d::Object* Mine::CreateObject()
    {
        // RVA 0x848180
        SYS_ERROR("!\"Object cannot be created directly\"");
        return nullptr;
    }

    m3d::Class* Mine::GetBaseClass()
    {
        return RT_CLASS_LOCAL(Rocket);
    }

    m3d::Class* Mine::GetClass() const
    {
        // RVA 0x847C10
        return RT_CLASS_LOCAL(Mine);
    }

    MinePrototypeInfo const* Mine::GetPrototypeInfo() const
    {
        // RVA 0x848470
        return RT_DYNCAST(thePrototypeManager->GetPrototypeInfo(GetPrototypeId()), MinePrototypeInfo const);
    }

    void Mine::LoadRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode)
    {
        // RVA 0x847DD0
        Rocket::LoadRuntimeValues(xmlFile, xmlNode);

        m3d::SafeFloatAttrib(m_yVelocity, xmlNode, "yVelocity");

        int mineState = m_MineState;
        m3d::SafeIntAttrib(mineState, xmlNode, "MineState");
        m_MineState = static_cast<MineState>(mineState);

        m3d::SafeFloatAttrib(m_TL, xmlNode, "TL");
    }

    void Mine::SaveRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const
    {
        // RVA 0x8483A0
        Rocket::SaveRuntimeValues(xmlFile, xmlNode);

        xmlNode->SetAttribute("yVelocity", CStr(m_yVelocity).c_str());
        xmlNode->SetAttribute("MineState", CStr(static_cast<int>(m_MineState)).c_str());
        xmlNode->SetAttribute("TL", CStr(m_TL).c_str());
    }

    void Mine::Update(float elapsedTime, unsigned int workTime)
    {
        // RVA 0x847E80 - a mine arms itself, waits out its life, then blows up. It is not
        // simulated by the physics engine: it falls under its own gravity until it meets the
        // ground, which is what makes a mine dropped on a slope settle onto it.
        SimplePhysicObj::Update(elapsedTime, workTime);

        m_TL += elapsedTime;
        MinePrototypeInfo const* const prototypeInfo = GetPrototypeInfo();
        if (m_MineState == msActivation)
        {
            if (m_TL > prototypeInfo->m_TimeForActivation)
            {
                m_TL = 0.0f;
                m_MineState = msWaiting;
            }
        }
        else if (m_MineState == msWaiting && m_TL > prototypeInfo->m_TTL)
        {
            CreateBlastWave();
            Remove();
        }

        m_yVelocity -= elapsedTime * 9.8100004f;
        CVector pos = GetPosition();
        pos.y += m_yVelocity * elapsedTime;

        // NOTE: the resting height is half the prototype's size.z, the model's length along its
        // forward axis, not size.y - the vertical axis every other line here works in.
        float const sizeZ = GetPrototypeInfo()->GetSize().z;
        float const restY = sizeZ * 0.5f + GetGroundPos(pos, true, false).y;
        if (restY > pos.y)
        {
            pos.y = restY;
            m_yVelocity = 0.0f;
        }
        SetPosition(pos);
    }

    Mine::MineState Mine::getState()
    {
        // RVA 0x847C60
        return m_MineState;
    }
}  // namespace ai
