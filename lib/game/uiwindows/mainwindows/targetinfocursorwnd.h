#pragma once
// NOTE: missing from the header this class was extracted from - required for
// TargetCursorWnd to be a complete type (TargetInfoCursorWnd derives from it).
#include "targetcursorwnd.h"

class TargetInfoCursorWnd :  public TargetCursorWnd
{
public:
    // NOTE: not present in the header this class was extracted from.
    struct AuxInfo
    {
        AuxInfo();
        /* 0x0000 */ CStr m_texName;
        /* 0x000c */ PointBase<float> m_texSz;
    }; /* size: 0x0014 */

    static struct m3d::Class * GetBaseClass();
    virtual class m3d::Object * Clone();
    static class m3d::Object * CreateObject();
    virtual struct m3d::Class * GetClass() const ;
    virtual ~TargetInfoCursorWnd();
protected:
    virtual int GetTargetObjId() const ;
    virtual int GameDataSetup();
    void UpdateTex();
    virtual void OnNewFrame();
    TargetInfoCursorWnd();
    TargetInfoCursorWnd(class TargetInfoCursorWnd const &);
private:
    TargetInfoCursorWnd::AuxInfo m_aif;
    m3d::rend::TexHandle m_tex;

    // NOTE: missing from the header this class was extracted from - without
    // it, m_classTargetInfoCursorWnd (needed by RT_CLASS_DEFINE / GetClass)
    // does not exist.
public:
    RT_CLASS_DECLARE(TargetInfoCursorWnd);
};
