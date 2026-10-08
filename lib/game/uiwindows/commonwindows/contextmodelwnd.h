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
    virtual int GameDataClear(bool beforeContinuousLevel) override /* 0x108 */;

    struct AuxInfo
    {
        /* 0x0000 */ CStr m_wndModelName;
        AuxInfo(ContextModelWnd::AuxInfo const&);
        AuxInfo();
    }; /* size: 0x000c */

protected:
    virtual int OnBeforeAddToWndStation() override /* 0x68 */;
    virtual int OnAfterRemoveFromWndStation() override /* 0x74 */;
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

    /* 0x0220 */ ref_ptr<m3d::ui::ImageWnd> m_wndCurrentModel;
    /* 0x0224 */ ref_ptr<m3d::ui::ImageWnd> m_wnd2dModel;
    /* 0x0228 */ ref_ptr<ItemModelWnd> m_wnd3dSimpleModel;
    /* 0x022c */ ref_ptr<ComplexModelWnd> m_wnd3dComplexModel;
    /* 0x0230 */ ContextModelWnd::AuxInfo m_aif;
    /* 0x023c */ int m_objId;
    /* 0x0240 */ int m_prototypeId;
    /* 0x0244 */ int m_objToShowId;

    ContextModelWnd();
    ContextModelWnd(ContextModelWnd const& rhs);

public:
    virtual ~ContextModelWnd() override /* 0x00 */;
    virtual m3d::Object* Clone() override /* 0x04 */;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override /* 0x34 */;
    static m3d::Class m_classContextModelWnd;
}; /* size: 0x0248 */
