#include <ui/edit.h>

namespace m3d
{
    namespace ui
    {
        Class* EditWnd::GetBaseClass()
        {
            throw std::logic_error("Not implemented");
        }

        Object* EditWnd::CreateObject()
        {
            throw std::logic_error("Not implemented");
        }

        int EditWnd::IncrementCursorPosition()
        {
            throw std::logic_error("Not implemented");
        }

        int EditWnd::ReadFromXmlNode(cmn::XmlFile*, cmn::XmlNode*)
        {
            throw std::logic_error("Not implemented");
        }

        void EditWnd::DeleteSymbol(int)
        {
            throw std::logic_error("Not implemented");
        }

        void EditWnd::SetQuickEraseState(bool)
        {
            throw std::logic_error("Not implemented");
        }

        Object* EditWnd::Clone()
        {
            throw std::logic_error("Not implemented");
        }

        int EditWnd::GetMaxCursorPos() const
        {
            throw std::logic_error("Not implemented");
        }

        int EditWnd::SetText(CStr const&)
        {
            throw std::logic_error("Not implemented");
        }

        int EditWnd::GetMinCursorPos() const
        {
            throw std::logic_error("Not implemented");
        }

        EditWnd::~EditWnd()
        {
            throw std::logic_error("Not implemented");
        }

        int EditWnd::WriteToXmlNode(cmn::XmlFile*, cmn::XmlNode*)
        {
            throw std::logic_error("Not implemented");
        }

        int EditWnd::CursorPos2StrPos(int) const
        {
            throw std::logic_error("Not implemented");
        }

        int EditWnd::SetCursorPosition(int)
        {
            throw std::logic_error("Not implemented");
        }

        int EditWnd::DecrementCursorPosition()
        {
            throw std::logic_error("Not implemented");
        }

        bool EditWnd::IsInQuickEraseState() const
        {
            throw std::logic_error("Not implemented");
        }

        int EditWnd::GetCursorPosition() const
        {
            throw std::logic_error("Not implemented");
        }

        int EditWnd::Create(CStr const&, unsigned, BoundsBase<float> const&, unsigned)
        {
            throw std::logic_error("Not implemented");
        }

        CStr EditWnd::GetVisibleText() const
        {
            throw std::logic_error("Not implemented");
        }

        void EditWnd::SetCursorColor(unsigned)
        {
            throw std::logic_error("Not implemented");
        }

        Class* EditWnd::GetClass() const
        {
            throw std::logic_error("Not implemented");
        }

        unsigned EditWnd::GetCursorColor() const
        {
            throw std::logic_error("Not implemented");
        }

        int EditWnd::OnActivate(bool)
        {
            throw std::logic_error("Not implemented");
        }

        int EditWnd::OnMouseButton0(unsigned, PointBase<float> const&)
        {
            throw std::logic_error("Not implemented");
        }

        EditWnd::EditWnd()
        {
            throw std::logic_error("Not implemented");
        }

        EditWnd::EditWnd(EditWnd const&)
        {
            throw std::logic_error("Not implemented");
        }

        int EditWnd::OnPaint(DrawInfo const&)
        {
            throw std::logic_error("Not implemented");
        }

        int EditWnd::OnKey(unsigned short, unsigned char, unsigned)
        {
            throw std::logic_error("Not implemented");
        }
    }
}
