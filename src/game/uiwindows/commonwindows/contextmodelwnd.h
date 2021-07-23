#pragma once

class ContextModelWnd :  public m3d::ui::Wnd
{
public:
    int Show2dImage(class m3d::rend::TexHandle);
    int Show2dImage(class CStr const &);
    int Show3dImage(class CStr const &,int);
    int GetObjId() const ;
    int ShowImageByPrototypeId(int);
    static class m3d::Object * __fastcall CreateObject();
    virtual struct m3d::Class * GetClass() const ;
    int ShowImageByObjId(int);
    static struct m3d::Class * __fastcall GetBaseClass();
    int GetPrototypeId() const ;
    virtual class m3d::Object * Clone();
    int HideImage();
    virtual int GameDataClear(bool);
    virtual ~ContextModelWnd();
    int CreateFromPattern(class m3d::ui::Wnd *,bool);
protected:
    virtual int OnAfterRemoveFromWndStation();
    virtual int OnBeforeAddToWndStation();
    void ClearCurrentModelWnd();
    int ShowImageByObjId0(int,bool);
    class ref_ptr<class m3d::ui::Wnd> CreateModelWnd(class CStr const &);
    class ai::Obj * GetObjToShow() const ;
    int ShowImageByPrototypeId0(int,bool);
    int SetupForComplexObj(class ai::Obj const *);
    int CreateObjToShow(int);
    ContextModelWnd(class ContextModelWnd const &);
    ContextModelWnd();
    void DestroyObjToShow();
    int SetupForModel(class CStr const &,int);
    int SetCurrentModelWnd(class ref_ptr<class m3d::ui::ImageWnd>);
private:
    ref_ptr<m3d::ui::ImageWnd> m_wndCurrentModel;
    ref_ptr<m3d::ui::ImageWnd> m_wnd2dModel;
    ref_ptr<ItemModelWnd> m_wnd3dSimpleModel;
    ref_ptr<ComplexModelWnd> m_wnd3dComplexModel;
    ContextModelWnd::AuxInfo m_aif;
    int m_objId;
    int m_prototypeId;
    int m_objToShowId;
};
