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
        }

        int LineWnd::WriteToXmlNode(cmn::XmlFile*, cmn::XmlNode*)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        void LineWnd::SetDirection(LineWndDirection)
        {
            RETRUXX_NOT_IMPLEMENTED;
        }

        Object* LineWnd::Clone()
        {
            RETRUXX_NOT_IMPLEMENTED;
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

        void LineWnd::OnNcPaint(DrawInfo const& di, unsigned clr)
        {
            //TODO: check this
            auto pane = GetGfxServer()->GetPane(m_paneName);
            float width = 0.0;
            if (pane && pane->m_frame[0])
            {
                width = pane->m_frame[0]->m_barTexWidth;
            }
            if (m_direction)
            {
                if (m_bounds.width != width)
                {
                    m_bounds.width = width;
                }
            }
            else
            {
                m_bounds.height = width;
            }
            BoundsBase<float> rect;
            rect.x0 = 0.0;
            rect.y0 = 0.0;
            rect.width = GetBounds().width;
            rect.height = GetBounds().height;
            GetGfxServer()->AddFlatAxialPane0(di, rect, clr, m_paneFlags, m_paneName, m_bgFlags);
        }

        int LineWnd::OnPaint(DrawInfo const& di)
        {
            if ((m_style & 0x40) != 0)
                return 1;
            if ((m_style & 2) == 0 && (m_style & 0x80000) == 0)
            {
                DrawNonClient(di, m_curClr);
                return 1;
            }
            DrawNonClient(di, 3u);
            return 1;
        }

        LineWnd::LineWnd(LineWnd const&)
        {
            RETRUXX_NOT_IMPLEMENTED;
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
