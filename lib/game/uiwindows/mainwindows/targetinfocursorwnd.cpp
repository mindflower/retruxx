#include "targetinfocursorwnd.h"

#include "config.h"
#include "core/kernel.h"
#include "core/log.h"
#include "m3dapp.h"

#include "game/m3dgame.h"
#include "game/uimanager/truxxuimanager.h"
#include "targetinfownd.h"

#include "server/objects/base/objcontainer.h"
#include "server/objects/base/physicobj.h"
#include "server/objects/player.h"
#include "server/objects/vehicle.h"

// NOTE: this whole file was missing entirely (0 bytes) prior to this pass;
// reconstructed from the decompile below.

RT_CLASS_EXPORTS_BEGIN(TargetInfoCursorWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(TargetInfoCursorWnd);

TargetInfoCursorWnd::AuxInfo::AuxInfo()
{
    // RVA 0x134D20
    m_texName = "CursorInfo";
    m_texSz = PointBase<float>{64.0f, 64.0f};
}

m3d::Object* TargetInfoCursorWnd::Clone()
{
    // RVA 0x134C70
    return new TargetInfoCursorWnd(*this);
}

m3d::Class* TargetInfoCursorWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(TargetCursorWnd);
}

m3d::Class* TargetInfoCursorWnd::GetClass() const
{
    // RVA 0x134CE0
    return RT_CLASS_LOCAL(TargetInfoCursorWnd);
}

m3d::Object* TargetInfoCursorWnd::CreateObject()
{
    // RVA 0x134CF0
    return new TargetInfoCursorWnd;
}

TargetInfoCursorWnd::TargetInfoCursorWnd() = default;

TargetInfoCursorWnd::TargetInfoCursorWnd(TargetInfoCursorWnd const&) : TargetInfoCursorWnd()
{
    // Matches the original: the copy ctor default-constructs the base, the
    // AuxInfo, and an invalid texture handle; nothing is copied from the source.
}

TargetInfoCursorWnd::~TargetInfoCursorWnd()
{
    // RVA 0x134E60 - the AuxInfo CStr and the TargetCursorWnd/SmartCursorWnd
    // bases clean themselves up automatically.
    M3D_RENDERER->ReleaseTexture(m_tex);
}

int TargetInfoCursorWnd::GameDataSetup()
{
    // RVA 0x134EC0
    if ((m_gameDataFlags & 2) == 0)
    {
        m_tex = M3D_APP->m_pInterfaceManager->GetIcoByName(m_aif.m_texName, 0);
        M3D_RENDERER->ReferenceTexture(m_tex);
        m_gameDataFlags |= 1u;
    }
    if ((m_gameDataFlags & 1) != 0)
    {
        return 1;
    }
    M3D_LOG_INFO("TargetInfoCursorWnd: error - fail to init because of a bad resource");
    return 0;
}

int TargetInfoCursorWnd::GetTargetObjId() const
{
    // RVA 0x134F90
    if (!ai::thePlayer)
    {
        return -1;
    }
    ai::Vehicle* vehicle = ai::thePlayer->GetVehicle();
    if (!vehicle)
    {
        return -1;
    }

    int const infoObjId = vehicle->GetInfoObjId();
    if (infoObjId == -1)
    {
        return -1;
    }

    ai::Obj* obj = ai::theObjects->GetEntityByObjId(infoObjId);
    if (obj && obj->IsKindOf(&ai::Obj::m_classObj))
    {
        unsigned const flags = obj->GetFlags();
        if ((flags & 8) == 0 && (flags & 2) == 0 && !obj->GetParentRepository() &&
            TargetInfoWnd::IsObjClassValidForInfo(obj->GetClass()))
        {
            return infoObjId;
        }
    }
    return -1;
}

void TargetInfoCursorWnd::OnNewFrame()
{
    // RVA 0x135030
    ClearDrawInfo();
    if (NeedShow())
    {
        Show(true);
        AccidentValidateBounds();
    }
    else
    {
        Show(false);
    }
    UpdateTex();
}

void TargetInfoCursorWnd::UpdateTex()
{
    // RVA 0x135080
    // NOTE: the shipped build gates this behind a vtable call taking only
    // `this` and returning bool; NeedShow() is the only virtual on this
    // branch of the hierarchy matching that shape.
    if (NeedShow())
    {
        auto* aimDi = new SmartCursorWnd::AuxDrawInfo();
        aimDi->SetTexture(m_tex);
        aimDi->m_sz = m_aif.m_texSz;
        m_drawInfo.push_back(aimDi);
    }
}
