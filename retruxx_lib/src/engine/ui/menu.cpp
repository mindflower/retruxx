#include <ui/menu.h>

namespace m3d
{
    namespace ui
    {
        RT_CLASS_EXPORTS_BEGIN(MenuWnd)
        RT_CLASS_EXPORTS_END;
        RT_CLASS_DEFINE(MenuWnd);

        Object* MenuWnd::CreateObject()
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        Class* MenuWnd::GetBaseClass()
        {
            return RT_CLASS_LOCAL(ModalWnd);
        }

        Class* MenuWnd::GetClass() const
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        void MenuWnd::SetMenu(Item*)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        int MenuWnd::OnMouseMove(PointBase<float> const&, PointBase<float> const&)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        int MenuWnd::OnInitModal()
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        MenuWnd::~MenuWnd()
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        int MenuWnd::OnPaint(DrawInfo const&)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        int MenuWnd::OnMouseOut()
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        int MenuWnd::OnMouseButton0(unsigned, PointBase<float> const&)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        Object* MenuWnd::Clone()
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        int MenuWnd::CanClose()
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        void MenuWnd::UpdateItems(Item*)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        void MenuWnd::RecalcLayout()
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        MenuWnd::MenuWnd(MenuWnd const&)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        MenuWnd::MenuWnd()
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        int MenuWnd::StorePathToItem(std::list<Item*>&, Item*, Item*)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }
    }
}
