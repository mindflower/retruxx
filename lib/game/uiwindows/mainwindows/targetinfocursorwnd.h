#pragma once

class TargetInfoCursorWnd :  public TargetCursorWnd
{
public:
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
};
