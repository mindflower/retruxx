#include "contextmodelwnd.h"
#include "complexmodelwnd.h"
#include "itemmodelwnd.h"

RT_CLASS_EXPORTS_BEGIN(ContextModelWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(ContextModelWnd);

ContextModelWnd::AuxInfo::AuxInfo()
{
    throw std::logic_error("Not implemented");
}

int ContextModelWnd::Show2dImage(m3d::rend::TexHandle)
{
    throw std::logic_error("Not implemented");
}

int ContextModelWnd::Show2dImage(CStr const&)
{
    throw std::logic_error("Not implemented");
}

int ContextModelWnd::Show3dImage(CStr const&, int)
{
    throw std::logic_error("Not implemented");
}

int ContextModelWnd::GetObjId() const
{
    throw std::logic_error("Not implemented");
}

int ContextModelWnd::ShowImageByPrototypeId(int)
{
    throw std::logic_error("Not implemented");
}

m3d::Object* ContextModelWnd::CreateObject()
{
    throw std::logic_error("Not implemented");
}

m3d::Class* ContextModelWnd::GetClass() const
{
    throw std::logic_error("Not implemented");
}

int ContextModelWnd::ShowImageByObjId(int)
{
    throw std::logic_error("Not implemented");
}

m3d::Class* ContextModelWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(Wnd);
}

int ContextModelWnd::GetPrototypeId() const
{
    throw std::logic_error("Not implemented");
}

m3d::Object* ContextModelWnd::Clone()
{
    throw std::logic_error("Not implemented");
}

int ContextModelWnd::HideImage()
{
    throw std::logic_error("Not implemented");
}

int ContextModelWnd::GameDataClear(bool)
{
    throw std::logic_error("Not implemented");
}

ContextModelWnd::~ContextModelWnd()
{
    throw std::logic_error("Not implemented");
}

int ContextModelWnd::CreateFromPattern(m3d::ui::Wnd*, bool)
{
    throw std::logic_error("Not implemented");
}

int ContextModelWnd::OnAfterRemoveFromWndStation()
{
    throw std::logic_error("Not implemented");
}

int ContextModelWnd::OnBeforeAddToWndStation()
{
    throw std::logic_error("Not implemented");
}

void ContextModelWnd::ClearCurrentModelWnd()
{
    throw std::logic_error("Not implemented");
}

int ContextModelWnd::ShowImageByObjId0(int, bool)
{
    throw std::logic_error("Not implemented");
}

ref_ptr<m3d::ui::Wnd> ContextModelWnd::CreateModelWnd(CStr const&)
{
    throw std::logic_error("Not implemented");
}

ai::Obj* ContextModelWnd::GetObjToShow() const
{
    throw std::logic_error("Not implemented");
}

int ContextModelWnd::ShowImageByPrototypeId0(int, bool)
{
    throw std::logic_error("Not implemented");
}

int ContextModelWnd::SetupForComplexObj(ai::Obj const*)
{
    throw std::logic_error("Not implemented");
}

int ContextModelWnd::CreateObjToShow(int)
{
    throw std::logic_error("Not implemented");
}

ContextModelWnd::ContextModelWnd(ContextModelWnd const&)
{
    throw std::logic_error("Not implemented");
}

ContextModelWnd::ContextModelWnd()
{
    throw std::logic_error("Not implemented");
}

void ContextModelWnd::DestroyObjToShow()
{
    throw std::logic_error("Not implemented");
}

int ContextModelWnd::SetupForModel(CStr const&, int)
{
    throw std::logic_error("Not implemented");
}

int ContextModelWnd::SetCurrentModelWnd(ref_ptr<m3d::ui::ImageWnd>)
{
    throw std::logic_error("Not implemented");
}
