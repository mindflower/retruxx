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
}  // namespace m3d

class VideoWnd;

class ContextModelWnd : public m3d::ui::Wnd
{
    // QuestItemsWnd drives the preview directly - clearing it and pushing a
    // prototype at it as the selected quest item changes, exactly as the shipped
    // code does.
    friend class QuestItemsWnd;

    friend class VideoWnd;

public:
    int ShowImageByObjId(int objId);
    int ShowImageByPrototypeId(int prototypeId);
    int Show2dImage(m3d::rend::TexHandle tex);
    int Show2dImage(CStr const& fileName);
    int Show3dImage(CStr const& modelName, int skin);
    int HideImage();
    int GetObjId() const;
    int GetPrototypeId() const;
    int CreateFromPattern(m3d::ui::Wnd* patternWnd, bool deleteSrc);
    virtual int GameDataClear(bool beforeContinuousLevel) override;

    struct AuxInfo
    {
        CStr m_wndModelName;
        AuxInfo(ContextModelWnd::AuxInfo const&);
        AuxInfo();
    };

protected:
    virtual int OnBeforeAddToWndStation() override;
    virtual int OnAfterRemoveFromWndStation() override;
    int SetupForComplexObj(ai::Obj const* o);
    int SetupForModel(CStr const& modelName, int skin);
    ref_ptr<m3d::ui::Wnd> CreateModelWnd(CStr const& className);
    int SetCurrentModelWnd(ref_ptr<m3d::ui::ImageWnd> modelWnd);
    void ClearCurrentModelWnd();
    int CreateObjToShow(int prototypeId);
    void DestroyObjToShow();
    int ShowImageByObjId0(int objId, bool bDestroyObjToShow);
    int ShowImageByPrototypeId0(int prototypeId, bool bDestroyObjToShow);
    ai::Obj* GetObjToShow() const;

    ref_ptr<m3d::ui::ImageWnd> m_wndCurrentModel;
    ref_ptr<m3d::ui::ImageWnd> m_wnd2dModel;
    ref_ptr<ItemModelWnd> m_wnd3dSimpleModel;
    ref_ptr<ComplexModelWnd> m_wnd3dComplexModel;
    ContextModelWnd::AuxInfo m_aif;
    int m_objId;
    int m_prototypeId;
    int m_objToShowId;

    ContextModelWnd();
    ContextModelWnd(ContextModelWnd const& rhs);

public:
    virtual ~ContextModelWnd() override;
    virtual m3d::Object* Clone() override;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override;
    static m3d::Class m_classContextModelWnd;
};
