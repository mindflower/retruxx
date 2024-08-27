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
    virtual bool NeedShow(void) const;
    virtual void UpdatePosition(void);
    virtual void Show(bool);

private:
    void AccidentValidateBounds(void);

public:
    RT_CLASS_DECLARE(TargetCursorWnd);
};
