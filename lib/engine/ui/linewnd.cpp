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

        int LineWnd::WriteToXmlNode(cmn::XmlFile* file, cmn::XmlNode* writeTo)
        {
            // RVA 0x674D30
            if (!Wnd::WriteToXmlNode(file, writeTo))
            {
                return 0;
            }
            writeTo->SetAttribute("lineDirection", CStr(static_cast<int>(m_direction)).c_str());
            return 1;
        }

        void LineWnd::SetDirection(LineWndDirection dir)
        {
            // RVA 0x72A5F0 - turning the line swaps its extents so the thickness
            // stays on the other axis.
            if (m_direction != dir)
            {
                auto const width = m_bounds.width;
                m_bounds.width = m_bounds.height;
                m_bounds.height = width;
            }
            m_direction = dir;
        }

        LineWndDirection LineWnd::GetDirection()
        {
            return m_direction;
        }

        Object* LineWnd::Clone()
        {
            // RVA 0x72A310
            return new LineWnd(*this);
        }

        int LineWnd::ReadFromXmlNode(cmn::XmlFile* xmlFile, cmn::XmlNode* xmlNode)
        {
            auto res = Wnd::ReadFromXmlNode(xmlFile, xmlNode);
            if (!res)
            {
                return res;
            }
            // NOTE: the shipped code copies an uninitialised stack slot into
            // m_direction when the attribute is missing; leaving the current value
            // alone is the only sane reading of that.
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
            // RVA 0x72A510 - the line is always exactly as thick as its pane's bar
            // texture, so the bounds are pinned back on every paint.
            auto pane = GetGfxServer()->GetPane(m_paneName);
            float thickness = 0.0;
            if (pane && pane->m_frame[0])
            {
                thickness = pane->m_frame[0]->m_barTexWidth;
            }
            if (m_direction)
            {
                if (m_bounds.width != thickness)
                {
                    m_bounds.width = thickness;
                }
            }
            else if (m_bounds.height != thickness)
            {
                m_bounds.height = thickness;
            }

            auto const b = GetBounds();
            BoundsBase<float> rect;
            rect.x0 = 0.0;
            rect.y0 = 0.0;
            rect.width = b.width;
            rect.height = b.height;
            // The direction doubles as the background flag: a vertical line asks
            // the pane for its other bar.
            GetGfxServer()->AddFlatAxialPane0(
                di, rect, clr, m_paneFlags, m_paneName, static_cast<PaneFlagBg>(m_direction != LINEWND_HORIZONTAL));
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

        LineWnd::LineWnd(LineWnd const& lw)
            : Wnd(lw)
        {
            // RVA 0x72A280 - the shipped copy constructor builds the Wnd base from
            // rhs and, unlike the default one, leaves m_direction uninitialised and
            // does not set up the "line3" pane.
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
