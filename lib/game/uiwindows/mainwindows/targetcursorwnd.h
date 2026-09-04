#pragma once
#include "smartcursorwnd.h"

class TargetCursorWnd : public SmartCursorWnd
{
public:
    virtual m3d::Class* GetRtClass(void) const;
    static m3d::Class* GetBaseClass(void);
    virtual ~TargetCursorWnd(void);

protected:
    virtual void OnNewFrame(void);
    TargetCursorWnd(void);
    virtual int OnPaint(m3d::ui::DrawInfo const&);
    // NOTE: not declared in the shipped header this was extracted from, but
    // every concrete subclass (TargetCapturedCursorWnd, TargetCapturingCursorWnd,
    // TargetInfoCursorWnd) overrides it and TargetCursorWnd's own NeedShow /
    // UpdatePosition call it virtually - it must be declared here.
    virtual int GetTargetObjId(void) const = 0;
    virtual bool NeedShow(void) const;
    virtual void UpdatePosition(void);
    virtual void Show(bool);
    // NOTE: shipped as accessible to subclasses (every subclass's OnNewFrame
    // calls it), so this cannot be private.
    void AccidentValidateBounds(void);

public:
    RT_CLASS_DECLARE(TargetCursorWnd);
};
