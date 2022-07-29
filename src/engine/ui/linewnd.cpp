#include <core/ini.h>
#include <ui/frame.h>
#include <ui/linewnd.h>
#include <ui/ui_srv.h>

namespace m3d
{
    namespace ui
    {
        RT_CLASS_EXPORTS_BEGIN(LineWnd)
    	RT_CLASS_EXPORTS_END;

        RT_CLASS_DEFINE(LineWnd);

        Object* LineWnd::CreateObject()
        {
            return new LineWnd;
        }

        Class* LineWnd::GetBaseClass()
        {
            return RT_CLASS_LOCAL(Wnd);
        }

        Class* LineWnd::GetClass() const
        {
            return RT_CLASS_LOCAL(LineWnd);
        }

        int LineWnd::Create(CStr const& caption, unsigned style, BoundsBase<float> const& rc, unsigned id)
        {
            return Create(rc, style, LINEWND_HORIZONTAL);
        }

        int LineWnd::Create(BoundsBase<float> const& rc, unsigned style, LineWndDirection dir)
        {
            m_direction = dir;
            auto rect = rc;
            auto pane = GetGfxServer()->GetPane(m_paneName);
            float size = 0.0;
            if (pane && pane->m_frame[0])
            {
                size = pane->m_frame[0]->m_barTexWidth;
            }
            if (dir != LINEWND_HORIZONTAL)
            {
                rect.width = size;
            }
            else
            {
                rect.height = size;
            }
            if (!style)
            {
                style = 768;
            }
            return Wnd::CreateWnd({}, style, rect, 0) != 0;
        }

        LineWnd::~LineWnd()
        {
            throw std::logic_error("Not implemented");
        }

        int LineWnd::WriteToXmlNode(cmn::XmlFile*, cmn::XmlNode*)
        {
            throw std::logic_error("Not implemented");
        }

        void LineWnd::SetDirection(LineWndDirection)
        {
            throw std::logic_error("Not implemented");
        }

        Object* LineWnd::Clone()
        {
            throw std::logic_error("Not implemented");
        }

        int LineWnd::ReadFromXmlNode(cmn::XmlFile* xmlFile, cmn::XmlNode* xmlNode)
        {
            auto res = Wnd::ReadFromXmlNode(xmlFile, xmlNode);
            if (!res)
            {
                return res;
            }
            SafeEnumAttrib(m_direction, xmlNode, "lineDirection");
            m_bounds = strToBounds(xmlNode->GetAttribute("org"));
            return 1;
        }

        void LineWnd::SetPaneFlags(int flags)
        {
            m_paneFlags = 1;
        }

        void LineWnd::OnNcPaint(DrawInfo const&, unsigned)
        {
            throw std::logic_error("Not implemented");
        }

        int LineWnd::OnPaint(DrawInfo const&)
        {
            throw std::logic_error("Not implemented");
        }

        LineWnd::LineWnd(LineWnd const&)
        {
            throw std::logic_error("Not implemented");
        }

        LineWnd::LineWnd()
        {
            m_paneName = "line3";
            m_paneFlags = 1;
            auto pane = GetGfxServer()->GetPane(m_paneName);
            if (pane && pane->m_frame[0])
            {
                m_bounds.height = pane->m_frame[0]->m_barTexWidth;
            }
        }
    }
}
