#include "contextmodelwnd.h"
#include "complexmodelwnd.h"
#include "itemmodelwnd.h"

#include <vector>

#include <core/kernel.h>
#include <core/log.h>
#include <game/m3dgame.h>
#include <ui/image.h>
#include <server/objects/base/obj.h>
#include <server/objects/base/objcontainer.h>

RT_CLASS_EXPORTS_BEGIN(ContextModelWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(ContextModelWnd);

ContextModelWnd::AuxInfo::AuxInfo()
{
    m_wndModelName = "wndModel";
}

int ContextModelWnd::Show2dImage(m3d::rend::TexHandle tex)
{
    if ((m_gameDataFlags & 1) == 0)
    {
        return 0;
    }
    DestroyObjToShow();
    ClearCurrentModelWnd();
    m_wnd2dModel->SetImage(tex);
    return SetCurrentModelWnd(m_wnd2dModel);
}

int ContextModelWnd::Show2dImage(CStr const& fileName)
{
    if ((m_gameDataFlags & 1) == 0)
    {
        return 0;
    }
    DestroyObjToShow();
    ClearCurrentModelWnd();
    m_wnd2dModel->SetImage(fileName);
    return SetCurrentModelWnd(m_wnd2dModel);
}

int ContextModelWnd::Show3dImage(CStr const& modelName, int skin)
{
    DestroyObjToShow();
    ClearCurrentModelWnd();
    return SetupForModel(modelName, skin);
}

int ContextModelWnd::GetObjId() const
{
    return m_objId;
}

int ContextModelWnd::ShowImageByPrototypeId(int prototypeId)
{
    return ShowImageByPrototypeId0(prototypeId, true);
}

m3d::Object* ContextModelWnd::CreateObject()
{
    return new ContextModelWnd;
}

m3d::Class* ContextModelWnd::GetClass() const
{
    return RT_CLASS_LOCAL(ContextModelWnd);
}

int ContextModelWnd::ShowImageByObjId(int objId)
{
    return ShowImageByObjId0(objId, true);
}

m3d::Class* ContextModelWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(Wnd);
}

int ContextModelWnd::GetPrototypeId() const
{
    return m_prototypeId;
}

m3d::Object* ContextModelWnd::Clone()
{
    return new ContextModelWnd(*this);
}

int ContextModelWnd::HideImage()
{
    DestroyObjToShow();
    ClearCurrentModelWnd();
    return 1;
}

int ContextModelWnd::GameDataClear(bool)
{
    DestroyObjToShow();
    ClearCurrentModelWnd();
    return 1;
}

ContextModelWnd::~ContextModelWnd()
{
    DestroyObjToShow();
    ClearCurrentModelWnd();
    // the four ref_ptr model windows and m_aif release themselves; ~Wnd runs.
}

int ContextModelWnd::CreateFromPattern(m3d::ui::Wnd* patternWnd, bool deleteSrc)
{
    if (!patternWnd)
    {
        M3D_LOG_INFO("ContextModelWnd::ContextModelWnd error - invalid patternWnd");
        return 0;
    }

    unsigned const style = patternWnd->GetStyle();
    BoundsBase<float> const rc = patternWnd->GetBounds();
    int const id = patternWnd->GetId();
    m3d::Object* parent = patternWnd->GetParent();
    if (!parent)
    {
        M3D_LOG_INFO("ContextModelWnd::ContextModelWnd error - invalid parent for pattern");
        return 0;
    }

    if (Wnd::Create(CStr(), style, rc, id) == 0)
    {
        M3D_LOG_INFO("ContextModelWnd::CreateFromPattern error - cannot create");
        return 0;
    }

    SetPane(patternWnd->GetPaneName());
    SetPaneFlags(patternWnd->GetPaneFlags());
    parent->AddChild(this);
    parent->MoveChildToFirstPosition(this);
    if (deleteSrc)
    {
        parent->RemoveChild(patternWnd);
        patternWnd->DecRef();
    }

    int res = 1;

    if (ref_ptr<m3d::ui::Wnd> img = CreateModelWnd("ImageWnd");
        img && img->IsKindOf(&m3d::ui::ImageWnd::m_classImageWnd))
    {
        m_wnd2dModel = static_cast<m3d::ui::ImageWnd*>(img.get());
    }
    else
    {
        res = 0;
    }

    if (ref_ptr<m3d::ui::Wnd> item = CreateModelWnd("ItemModelWnd");
        item && item->IsKindOf(&ItemModelWnd::m_classItemModelWnd))
    {
        m_wnd3dSimpleModel = static_cast<ItemModelWnd*>(item.get());
    }
    else
    {
        res = 0;
    }

    if (ref_ptr<m3d::ui::Wnd> complex = CreateModelWnd("ComplexModelWnd");
        complex && complex->IsKindOf(&ComplexModelWnd::m_classComplexModelWnd))
    {
        m_wnd3dComplexModel = static_cast<ComplexModelWnd*>(complex.get());
        if (res)
        {
            m_gameDataFlags |= 1u;
        }
    }

    if ((m_gameDataFlags & 1) == 0)
    {
        M3D_LOG_INFO("ContextModelWnd: error - fail to init because of a bad resource");
        return 0;
    }
    return 1;
}

int ContextModelWnd::OnAfterRemoveFromWndStation()
{
    int const result = m3d::ui::Wnd::OnAfterRemoveFromWndStation();
    if (m_objToShowId != -1)
    {
        DestroyObjToShow();
    }
    return result;
}

int ContextModelWnd::OnBeforeAddToWndStation()
{
    if (m_prototypeId != -1)
    {
        ShowImageByPrototypeId0(m_prototypeId, true);
    }
    return m3d::ui::Wnd::OnBeforeAddToWndStation();
}

void ContextModelWnd::ClearCurrentModelWnd()
{
    if (m_wndCurrentModel.get() && IsDirectChild(m_wndCurrentModel.get()))
    {
        RemoveChild(m_wndCurrentModel.get());
        m_wndCurrentModel = nullptr;
    }
    m_objId = -1;
    m_prototypeId = -1;
}

int ContextModelWnd::ShowImageByObjId0(int, bool)
{
    // TODO: dispatch on the object's class (vehicle/gun -> SetupForComplexObj,
    // cabin/basket -> SetupForModel, else -> ShowImageByPrototypeId0).
    RETRUXX_NOT_IMPLEMENTED;
}

ref_ptr<m3d::ui::Wnd> ContextModelWnd::CreateModelWnd(CStr const& className)
{
    auto* wnd = static_cast<m3d::ui::Wnd*>(M3D_KERNEL->New(className.c_str()));
    if (!wnd)
    {
        return nullptr;
    }
    ref_ptr<m3d::ui::Wnd> modelWnd(wnd);

    BoundsBase<float> const bounds = GetBounds();
    BoundsBase<float> rc;
    rc.x0 = 0.0f;
    rc.y0 = 0.0f;
    rc.width = bounds.width;
    rc.height = bounds.height;

    std::vector<int> events;
    if (wnd->IsKindOf(&ItemModelWnd::m_classItemModelWnd))
    {
        events.push_back(89);
    }
    else if (wnd->IsKindOf(&ComplexModelWnd::m_classComplexModelWnd))
    {
        events.push_back(89);
        events.push_back(65);
    }
    else if (wnd->IsKindOf(&m3d::ui::ImageWnd::m_classImageWnd))
    {
        events.push_back(89);
    }
    else
    {
        return nullptr;
    }

    if (wnd->Create(CStr(), m3d::ui::WS_IS_VISIBLE, rc, static_cast<unsigned>(-1)) == 0)
    {
        return nullptr;
    }

    int guiId = -1;
    ref_ptr<m3d::ui::Wnd> registeredCopy = modelWnd;
    if (!M3D_APP->m_pInterfaceManager->AddWindow(registeredCopy, guiId, true, false))
    {
        return nullptr;
    }
    M3D_APP->m_pInterfaceManager->SetEventsForWindow(guiId, events);
    return modelWnd;
}

ai::Obj* ContextModelWnd::GetObjToShow() const
{
    if (m_objToShowId < 0)
    {
        return nullptr;
    }
    return ai::theObjects->GetEntityByObjId(m_objToShowId);
}

int ContextModelWnd::ShowImageByPrototypeId0(int, bool)
{
    // TODO: dispatch on the prototype's class as ShowImageByObjId0 does.
    RETRUXX_NOT_IMPLEMENTED;
}

int ContextModelWnd::SetupForComplexObj(ai::Obj const*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ContextModelWnd::CreateObjToShow(int prototypeId)
{
    DestroyObjToShow();
    if (prototypeId == -1)
    {
        return 0;
    }
    m_objToShowId = ai::theObjects->CreateNewObject(prototypeId, "TempObj", -1, -1);
    return m_objToShowId != -1;
}

ContextModelWnd::ContextModelWnd(ContextModelWnd const&) : ContextModelWnd()
{
}

ContextModelWnd::ContextModelWnd()
{
    m_objId = -1;
    m_prototypeId = -1;
    m_objToShowId = -1;
}

void ContextModelWnd::DestroyObjToShow()
{
    if (m_objToShowId == -1)
    {
        return;
    }
    if (ai::Obj* obj = ai::theObjects->GetEntityByObjId(m_objToShowId))
    {
        obj->Remove();
    }
    m_objToShowId = -1;
    m_objId = -1;
}

int ContextModelWnd::SetupForModel(CStr const&, int)
{
    // TODO: needs ItemModelWnd::SetModelByName plus write access to ModelWnd::m_SkinNum.
    RETRUXX_NOT_IMPLEMENTED;
}

int ContextModelWnd::SetCurrentModelWnd(ref_ptr<m3d::ui::ImageWnd> modelWnd)
{
    if (!modelWnd)
    {
        return 0;
    }
    if (IsDirectChild(modelWnd.get()))
    {
        return 0;
    }
    AddChild(modelWnd.get());
    m_wndCurrentModel = modelWnd;
    return 1;
}
