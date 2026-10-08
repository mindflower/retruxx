#include "targetcursorwnd.h"

#include "config.h"
#include "core/kernel.h"
#include "m3dapp.h"

#include "game/uimisc/guihelper.h"

#include "server/objects/base/objcontainer.h"
#include "server/objects/base/physicobj.h"

RT_CLASS_EXPORTS_BEGIN(TargetCursorWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(TargetCursorWnd);

// NOTE: this is a genuinely separate virtual from m3d::Object::GetClass() (the
// name differs, so it cannot override that slot) - TargetCursorWnd itself is
// never instantiated directly, and every concrete subclass overrides GetClass()
// on its own, so this shipped naming quirk has no observable effect.
m3d::Class* TargetCursorWnd::GetRtClass() const
{
    // RVA 0x134910
    return RT_CLASS_LOCAL(TargetCursorWnd);
}

m3d::Class* TargetCursorWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(SmartCursorWnd);
}

TargetCursorWnd::~TargetCursorWnd()
{
    // RVA 0x134980 - SmartCursorWnd::~SmartCursorWnd() runs automatically.
}

void TargetCursorWnd::OnNewFrame()
{
    // RVA 0x134990
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
}

TargetCursorWnd::TargetCursorWnd()
{
    // RVA 0x134920 - m_drawInfo is default-constructed empty.
}

int TargetCursorWnd::OnPaint(m3d::ui::DrawInfo const& di)
{
    // RVA 0x1349D0
    UpdatePosition();

    m3d::ui::DrawInfo newDi = di;
    newDi.m_clientClippedRect = m_bounds;
    newDi.m_clientRect = m_bounds;
    newDi.m_clippedRect = m_bounds;
    newDi.m_originalRect = m_bounds;
    return SmartCursorWnd::OnPaint(newDi);
}

bool TargetCursorWnd::NeedShow() const
{
    // RVA 0x134B70
    return GetTargetObjId() != -1;
}

void TargetCursorWnd::UpdatePosition()
{
    // RVA 0x134A80
    int const targetObjId = GetTargetObjId();
    if (targetObjId < 0)
    {
        return;
    }

    ai::Obj* obj = ai::theObjects->GetEntityByObjId(targetObjId);
    if (!obj || !obj->IsKindOf(&ai::PhysicObj::m_classPhysicObj))
    {
        return;
    }
    auto* physicObj = static_cast<ai::PhysicObj*>(obj);

    unsigned const flags = physicObj->GetFlags();
    if ((flags & 8) != 0 || (flags & 2) != 0 || physicObj->GetParentRepository())
    {
        return;
    }

    PointBase<float> const screenPos = help::GetRelScreenPtByWorldPos(physicObj->GetPosition());
    m_bounds.x0 = screenPos.x - (m_bounds.width * 0.5f);
    m_bounds.y0 = screenPos.y - (m_bounds.height * 0.5f);
}

void TargetCursorWnd::Show(bool bShow)
{
    // RVA 0x134B90
    ShowWindow(bShow);
}

void TargetCursorWnd::AccidentValidateBounds()
{
    // RVA 0x134BA0 - clamps the cursor rect into the [0,1024]x[0,768] design
    // resolution and resets the origin to (0,0) if the clamped rect collapses
    // to nothing (an edge case the shipped build guards against explicitly).
    float clampedWidth;
    float clampedHeight;

    float const x0 = m_bounds.x0;
    float const rightEdge = m_bounds.width + x0;
    float const y0 = m_bounds.y0;
    float const bottomEdge = m_bounds.height + y0;

    if (x0 > 1024.0f || rightEdge < 0.0f || y0 > 768.0f || bottomEdge < 0.0f)
    {
        clampedWidth = 0.0f;
        clampedHeight = 0.0f;
    }
    else
    {
        float const left = (x0 <= 0.0f) ? 0.0f : x0;
        float const right = (rightEdge >= 1024.0f) ? 1024.0f : rightEdge;
        float const top = (y0 <= 0.0f) ? 0.0f : y0;
        float const bottom = (bottomEdge >= 768.0f) ? 768.0f : bottomEdge;
        clampedWidth = right - left;
        clampedHeight = bottom - top;
    }

    if (clampedWidth == 0.0f && clampedHeight == 0.0f)
    {
        m_bounds.x0 = 0.0f;
        m_bounds.y0 = 0.0f;
    }
}
