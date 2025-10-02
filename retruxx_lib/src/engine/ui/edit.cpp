#include <core/ini.h>
#include <ui/edit.h>
#include <ui/ui_srv.h>

namespace m3d
{
    namespace ui
    {
        RT_CLASS_EXPORTS_BEGIN(EditWnd)
    	RT_CLASS_EXPORTS_END;

        RT_CLASS_DEFINE(EditWnd);

        Class* EditWnd::GetBaseClass()
        {
            return RT_CLASS_LOCAL(Wnd);
        }

        Object* EditWnd::CreateObject()
        {
            return new EditWnd;
        }

        int EditWnd::IncrementCursorPosition()
        {
            throw std::logic_error("Not implemented");
        }

        int EditWnd::ReadFromXmlNode(cmn::XmlFile* xmlFile, cmn::XmlNode* xmlNode)
        {
            auto res = Wnd::ReadFromXmlNode(xmlFile, xmlNode);
            if (!res)
            {
                return res;
            }
            unsigned clr = 1;
            SafeClrAttrib(clr, xmlNode, "cursorColor");
            SetCursorColor(clr);
            return 1;
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
            return GetVisibleText().length();
        }

        int EditWnd::SetText(CStr const& caption)
        {
            auto res = Wnd::SetText(caption);
            auto visibleText = GetVisibleText();
            m_cursorPosition = visibleText.length();
            return res;
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

        int EditWnd::Create(CStr const& caption, unsigned style, BoundsBase<float> const& rc, unsigned id)
        {
            auto res = Wnd::Create(caption, style, rc, id);
            if (!res)
            {
                return res;
            }
            m_style |= 4;
            m_cursorPosition = GetMaxCursorPos();
            return 1;
        }

        CStr EditWnd::GetVisibleText() const
        {
            CStr res;
            CStr serviceSymbols = "|@$#&";
            bool bEsc = false;

            for (int i = 0; i < m_caption.length(); ++i)
            {
                auto pos = serviceSymbols.find(m_caption[i]);
                if (pos != CStr_npos)
                {
                    if (serviceSymbols[pos] == '#')
                    {
                        if (bEsc)
                        {
                            res += m_caption[i];
                            bEsc = false;
                        }
                        else
                        {
                            bEsc = true;
                        }
                    }
                    else if (bEsc)
                    {
                        res += m_caption[i];
                        bEsc = false;
                    }
                    else if (serviceSymbols[pos] == '@')
                    {
                        i += 8;
                    }
                }
                else
                {
                    res += m_caption[i];
                    bEsc = false;
                }
            }
            return res;
        }

        void EditWnd::SetCursorColor(unsigned color)
        {
            m_cursorColor = color;
            char tmp[128] = { 0 };
            sprintf(tmp , "%08x", GetGfxServer()->GetColor(color));
            m_strCursorColor = CStr("@") + tmp;
        }

        Class* EditWnd::GetClass() const
        {
            return RT_CLASS_LOCAL(EditWnd);
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
            m_style = 276992;
            m_paneName = "wnd3";
            m_paneFlags = 7;
            SetCursorColor(1);
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
