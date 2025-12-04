#pragma once
#include <core/ref_ptr.h>
#include <ui/ui.h>

namespace ai
{
    class Obj;
}

class ComplexModelWnd;
class ItemModelWnd;

namespace m3d
{
    namespace ui
    {
        class ImageWnd;
    }
}

class ContextModelWnd :  public m3d::ui::Wnd
{
public:
    class AuxInfo
    {
    public:
        AuxInfo();

    private:
        CStr m_wndModelName;
    };

public:
    int Show2dImage(m3d::rend::TexHandle);
    int Show2dImage(CStr const &);
    int Show3dImage(CStr const &,int);
    int GetObjId() const ;
    int ShowImageByPrototypeId(int);
    static m3d::Object * CreateObject();
    virtual m3d::Class * GetClass() const ;
    int ShowImageByObjId(int);
    static m3d::Class * GetBaseClass();
    int GetPrototypeId() const ;
    virtual m3d::Object * Clone();
    int HideImage();
    virtual int GameDataClear(bool);
    virtual ~ContextModelWnd();
    int CreateFromPattern(m3d::ui::Wnd *,bool);

protected:
    virtual int OnAfterRemoveFromWndStation();
    virtual int OnBeforeAddToWndStation();
    void ClearCurrentModelWnd();
    int ShowImageByObjId0(int,bool);
    ref_ptr<m3d::ui::Wnd> CreateModelWnd(CStr const &);
    ai::Obj * GetObjToShow() const ;
    int ShowImageByPrototypeId0(int,bool);
    int SetupForComplexObj(ai::Obj const *);
    int CreateObjToShow(int);
    ContextModelWnd(ContextModelWnd const &);
    ContextModelWnd();
    void DestroyObjToShow();
    int SetupForModel(CStr const &,int);
    int SetCurrentModelWnd(ref_ptr<m3d::ui::ImageWnd>);

public:
    RT_CLASS_DECLARE(ContextModelWnd);

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
