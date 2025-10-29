#include "contextmodelwnd.h"
#include "complexmodelwnd.h"
#include "itemmodelwnd.h"

RT_CLASS_EXPORTS_BEGIN(ContextModelWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(ContextModelWnd);

ContextModelWnd::AuxInfo::AuxInfo()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ContextModelWnd::Show2dImage(m3d::rend::TexHandle)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ContextModelWnd::Show2dImage(CStr const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ContextModelWnd::Show3dImage(CStr const&, int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ContextModelWnd::GetObjId() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ContextModelWnd::ShowImageByPrototypeId(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* ContextModelWnd::CreateObject()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* ContextModelWnd::GetClass() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ContextModelWnd::ShowImageByObjId(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* ContextModelWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(Wnd);
}

int ContextModelWnd::GetPrototypeId() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* ContextModelWnd::Clone()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ContextModelWnd::HideImage()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ContextModelWnd::GameDataClear(bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

ContextModelWnd::~ContextModelWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ContextModelWnd::CreateFromPattern(m3d::ui::Wnd*, bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ContextModelWnd::OnAfterRemoveFromWndStation()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ContextModelWnd::OnBeforeAddToWndStation()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void ContextModelWnd::ClearCurrentModelWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ContextModelWnd::ShowImageByObjId0(int, bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

ref_ptr<m3d::ui::Wnd> ContextModelWnd::CreateModelWnd(CStr const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

ai::Obj* ContextModelWnd::GetObjToShow() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ContextModelWnd::ShowImageByPrototypeId0(int, bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ContextModelWnd::SetupForComplexObj(ai::Obj const*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ContextModelWnd::CreateObjToShow(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

ContextModelWnd::ContextModelWnd(ContextModelWnd const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

ContextModelWnd::ContextModelWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void ContextModelWnd::DestroyObjToShow()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ContextModelWnd::SetupForModel(CStr const&, int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ContextModelWnd::SetCurrentModelWnd(ref_ptr<m3d::ui::ImageWnd>)
{
    RETRUXX_NOT_IMPLEMENTED;
}
