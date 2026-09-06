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
#include <server/objects/base/prototypemanager.h>
#include <server/objects/basket.h>
#include <server/objects/cabin.h>
#include <server/objects/gadget.h>
#include <server/objects/guns/compoundgun.h>
#include <server/objects/guns/gun.h>
#include <server/objects/physicbodies/physicbody.h>
#include <server/objects/vehicle.h>
#include <server/objects/ware.h>
#include <server/quest.h>
#include <server/objects/player.h>

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

int ContextModelWnd::ShowImageByObjId0(int objId, bool bDestroyObjToShow)
{
    // RVA 0x513B30
    if (bDestroyObjToShow)
    {
        DestroyObjToShow();
    }
    else if (m_objToShowId == -1 || m_objToShowId != objId)
    {
        // Showing something else now, so the object we spawned for the previous
        // preview is no longer needed.
        DestroyObjToShow();
    }
    ClearCurrentModelWnd();

    ai::Obj* obj = ai::theObjects->GetEntityByObjId(objId);
    if (!obj)
    {
        // An explicit "show nothing" counts as success; a bad id does not.
        return objId == -1;
    }

    if (obj->IsKindOf(&ai::Vehicle::m_classVehicle) || obj->IsKindOf(&ai::Gun::m_classGun) ||
        obj->IsKindOf(&ai::CompoundGun::m_classCompoundGun))
    {
        int const res = SetupForComplexObj(obj);
        m_objId = objId;
        return res;
    }

    if (obj->IsKindOf(&ai::Cabin::m_classCabin) || obj->IsKindOf(&ai::Basket::m_classBasket))
    {
        // A cabin or basket is drawn as a plain model, painted to match whatever
        // vehicle it is mounted on (or the player's, when it is loose).
        m3d::Object* parent = obj->GetParent();
        ai::Vehicle* vehicle = (parent && parent->IsKindOf(&ai::Vehicle::m_classVehicle))
                                   ? static_cast<ai::Vehicle*>(parent)
                                   : (ai::thePlayer ? ai::thePlayer->GetVehicle() : nullptr);
        auto const* prototype = static_cast<ai::PhysicBodyPrototypeInfo const*>(obj->GetPrototypeInfo());
        if (!prototype)
        {
            return 0;
        }
        int const res = SetupForModel(prototype->m_engineModelName, vehicle ? vehicle->m_skinNumber : -1);
        m_objId = objId;
        return res;
    }

    int const res = ShowImageByPrototypeId0(obj->GetPrototypeId(), bDestroyObjToShow);
    m_objId = objId;
    return res;
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

int ContextModelWnd::ShowImageByPrototypeId0(int prototypeId, bool bDestroyObjToShow)
{
    // RVA 0x513CD0
    if (bDestroyObjToShow)
    {
        DestroyObjToShow();
        ClearCurrentModelWnd();
    }
    if (prototypeId == -1)
    {
        return 1;
    }

    ai::PrototypeInfo const* prototype = ai::thePrototypeManager->GetPrototypeInfo(prototypeId);
    if (!prototype)
    {
        return 0;
    }

    if (prototype->IsPrototypeOf(&ai::Cabin::m_classCabin) || prototype->IsPrototypeOf(&ai::Basket::m_classBasket))
    {
        int const res = SetupForModel(static_cast<ai::PhysicBodyPrototypeInfo const*>(prototype)->m_engineModelName, 0);
        m_prototypeId = prototypeId;
        return res;
    }

    if (prototype->IsPrototypeOf(&ai::Vehicle::m_classVehicle) || prototype->IsPrototypeOf(&ai::Gun::m_classGun) ||
        prototype->IsPrototypeOf(&ai::CompoundGun::m_classCompoundGun))
    {
        // These are assembled from parts, so an actual object has to be spawned
        // before it can be posed and drawn.
        if (!CreateObjToShow(prototypeId) || !GetObjToShow())
        {
            return 0;
        }
        int const res = ShowImageByObjId0(m_objToShowId, false);
        m_prototypeId = prototypeId;
        return res;
    }

    if (prototype->IsPrototypeOf(&ai::Gadget::m_classGadget))
    {
        auto const* gadget = static_cast<ai::GadgetPrototypeInfo const*>(prototype);
        int const res = SetupForModel(gadget->GetModelName(), gadget->GetSkinNum());
        m_prototypeId = prototypeId;
        return res;
    }

    if (prototype->IsPrototypeOf(&ai::Ware::m_classWare))
    {
        int const res = SetupForModel(static_cast<ai::WarePrototypeInfo const*>(prototype)->GetModelName(), 0);
        m_prototypeId = prototypeId;
        return res;
    }

    // QuestItem has no class object to test against, so it is matched by name.
    if (CStr::my_strcmp(prototype->m_className.c_str(), "QuestItem") == 0)
    {
        int const res = SetupForModel(static_cast<ai::QuestItemPrototypeInfo const*>(prototype)->GetModelName(), 0);
        m_prototypeId = prototypeId;
        return res;
    }
    return 0;
}

int ContextModelWnd::SetupForComplexObj(ai::Obj const* o)
{
    // RVA 0x513EF0
    if ((m_gameDataFlags & 1) == 0 || !o)
    {
        return 0;
    }
    if (!o->IsKindOf(&ai::Vehicle::m_classVehicle) && !o->IsKindOf(&ai::Gun::m_classGun) &&
        !o->IsKindOf(&ai::CompoundGun::m_classCompoundGun))
    {
        return 0;
    }

    // SetObjId is exactly the "store the id, and rebuild the node set now if we
    // are already on screen" pair the shipped code inlines here.
    m_wnd3dComplexModel->SetObjId(o->GetId());
    return SetCurrentModelWnd(ref_ptr<m3d::ui::ImageWnd>(m_wnd3dComplexModel.get())) != 0;
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

int ContextModelWnd::SetupForModel(CStr const& modelName, int skin)
{
    // RVA 0x513FD0
    if ((m_gameDataFlags & 1) == 0 || modelName.empty())
    {
        return 0;
    }
    if (skin == -1)
    {
        skin = 0;
    }
    if (!SetCurrentModelWnd(ref_ptr<m3d::ui::ImageWnd>(m_wnd3dSimpleModel.get())))
    {
        return 0;
    }
    // NOTE: the shipped code loads the model with skin 0 and then stamps the real
    // skin straight onto m_SkinNum, rather than passing it to SetModelByName.
    m_wnd3dSimpleModel->SetModelByName(modelName, 0, 0);
    m_wnd3dSimpleModel->m_SkinNum = skin;
    return 1;
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
