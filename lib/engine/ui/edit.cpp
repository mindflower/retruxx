#include <core/ini.h>
#include <core/kernel.h>
#include <core/timer.h>
#include <i_event.h>
#include <ui/edit.h>
#include <ui/ui_srv.h>

namespace m3d
{
    namespace ui
    {
        namespace
        {
            // Characters the caption markup reserves: "|" line break, "@" colour
            // marker, "$"/"&" formatting, "#" escape.
            char const* const SERVICE_SYMBOLS = "|@$#&";

            // A colour marker is "@" plus eight hex digits.
            int const COLOR_MARKER_LEN = 8;

            // The caret blinks once a second, visible for the second half.
            unsigned const CARET_BLINK_PERIOD = 1000;
            unsigned const CARET_BLINK_ON_AFTER = 500;
        }  // namespace

        RT_CLASS_EXPORTS_BEGIN(EditWnd)
        RT_CLASS_EXPORTS_END;

        RT_CLASS_DEFINE(EditWnd);

        EditWnd::EditWnd()
        {
            // RVA 0x724210
            m_style = 276992;
            m_paneName = "wnd3";
            m_paneFlags = 7;
            m_bQuickEraseState = false;
            m_cursorPosition = 0;
            SetCursorColor(1);
        }

        EditWnd::EditWnd(EditWnd const& ew)
            : Wnd(ew)
        {
            // RVA 0x723A10 - the shipped copy constructor forwards to the Wnd one
            // and, unlike the default constructor, leaves the quick-erase flag,
            // the cursor position and the cursor colour uninitialised.
        }

        EditWnd::~EditWnd()
        {
            // RVA 0x723A50 - only m_strCursorColor and the Wnd base need unwinding.
        }

        Object* EditWnd::Clone()
        {
            // RVA 0x723C30
            return new EditWnd(*this);
        }

        Object* EditWnd::CreateObject()
        {
            return new EditWnd;
        }

        Class* EditWnd::GetBaseClass()
        {
            return RT_CLASS_LOCAL(Wnd);
        }

        Class* EditWnd::GetClass() const
        {
            return RT_CLASS_LOCAL(EditWnd);
        }

        int EditWnd::Create(CStr const& caption, unsigned style, BoundsBase<float> const& rc, unsigned id)
        {
            // RVA 0x724570
            if (!CreateWnd(caption, style, rc, id))
            {
                return 0;
            }
            m_style |= WS_SEND_NOTIFY_MESSAGES;
            m_cursorPosition = GetMaxCursorPos();
            return 1;
        }

        void EditWnd::SetQuickEraseState(bool bQuickErase)
        {
            // RVA 0x723950
            m_bQuickEraseState = bQuickErase;
        }

        bool EditWnd::IsInQuickEraseState() const
        {
            // RVA 0x723960
            return m_bQuickEraseState;
        }

        int EditWnd::SetText(CStr const& caption)
        {
            // RVA 0x724630 - the caret lands at the end of the new text.
            auto const res = Wnd::SetText(caption);
            m_cursorPosition = GetVisibleText().length();
            return res;
        }

        unsigned EditWnd::GetCursorColor() const
        {
            // RVA 0x7239A0
            return m_cursorColor;
        }

        void EditWnd::SetCursorColor(unsigned color)
        {
            // RVA 0x723CE0 - the caret is drawn as ordinary text, so its colour is
            // kept as a ready-made "@rrggbbaa" markup prefix.
            m_cursorColor = color;
            char tmp[128] = {0};
            sprintf(tmp, "%08x", GetGfxServer()->GetColor(color));
            m_strCursorColor = CStr("@") + tmp;
        }

        int EditWnd::GetCursorPosition() const
        {
            // RVA 0x7239B0
            return m_cursorPosition;
        }

        int EditWnd::SetCursorPosition(int pos)
        {
            // RVA 0x7246A0 - an out-of-range position snaps to the end and reports
            // failure.
            auto const maxPos = static_cast<int>(GetVisibleText().length());
            if (pos < 0 || pos > maxPos)
            {
                m_cursorPosition = maxPos;
                return 0;
            }
            m_cursorPosition = pos;
            return 1;
        }

        int EditWnd::GetMaxCursorPos() const
        {
            // RVA 0x724070
            return GetVisibleText().length();
        }

        int EditWnd::GetMinCursorPos() const
        {
            // RVA 0x7239C0
            return 0;
        }

        int EditWnd::IncrementCursorPosition()
        {
            // RVA 0x7245C0
            ++m_cursorPosition;
            if (m_cursorPosition <= static_cast<int>(GetVisibleText().length()))
            {
                return 1;
            }
            --m_cursorPosition;
            return 0;
        }

        int EditWnd::DecrementCursorPosition()
        {
            // RVA 0x723AB0
            if (--m_cursorPosition >= 0)
            {
                return 1;
            }
            ++m_cursorPosition;
            return 0;
        }

        int EditWnd::CursorPos2StrPos(int cursorPos) const
        {
            // RVA 0x723AE0 - maps a caret position, counted in visible characters,
            // onto an index into the raw caption with all its markup.
            auto const len = static_cast<int>(m_caption.length());
            if (len == 0)
            {
                return 0;
            }
            if (cursorPos >= len)
            {
                return len;
            }

            CStr const serviceSymbols = SERVICE_SYMBOLS;
            auto strPos = 0;
            if (cursorPos > -1)
            {
                auto i = 0;
                auto visible = -1;
                auto bEsc = false;
                auto markerEnd = COLOR_MARKER_LEN;
                do
                {
                    strPos = i;
                    if (i >= len)
                    {
                        break;
                    }
                    auto const c = m_caption[i];
                    if (serviceSymbols.find(c) == CStr_npos)
                    {
                        ++visible;
                        bEsc = false;
                    }
                    else if (c == '#')
                    {
                        if (bEsc)
                        {
                            ++visible;
                            bEsc = false;
                        }
                        else
                        {
                            bEsc = true;
                        }
                    }
                    else if (bEsc)
                    {
                        ++visible;
                        bEsc = false;
                    }
                    else if (c == '@' && markerEnd < len)
                    {
                        i += COLOR_MARKER_LEN;
                        markerEnd += COLOR_MARKER_LEN;
                    }
                    ++i;
                    ++markerEnd;
                } while (visible < cursorPos);

                if (strPos < 0)
                {
                    strPos = 0;
                }
            }
            if (strPos > len)
            {
                strPos = 0;
            }
            return strPos;
        }

        void EditWnd::DeleteSymbol(int cursorPos)
        {
            // RVA 0x7240C0
            auto const len = static_cast<int>(m_caption.length());
            auto const strPos = CursorPos2StrPos(cursorPos);
            if (strPos < 0 || strPos >= len)
            {
                return;
            }

            CStr const serviceSymbols = SERVICE_SYMBOLS;
            if (serviceSymbols.find(m_caption[strPos]) == CStr_npos || strPos - 1 < 0 || m_caption[strPos - 1] != '#')
            {
                m_caption.del(strPos, 1);
            }
            else
            {
                // An escaped service symbol is two characters wide, so the "#" in
                // front of it goes with it.
                CStr tail;
                if (strPos + 1 < len)
                {
                    tail = m_caption.substr(strPos + 1, CStr_npos);
                }
                m_caption = m_caption.substr(0, strPos - 1) + tail;
            }
        }

        CStr EditWnd::GetVisibleText() const
        {
            // RVA 0x723E00 - the caption with its markup stripped out, which is
            // what the caret positions are counted in.
            CStr res;
            CStr const serviceSymbols = SERVICE_SYMBOLS;
            auto const len = static_cast<int>(m_caption.length());
            auto bEsc = false;
            auto markerEnd = COLOR_MARKER_LEN;

            for (auto i = 0; i < len; ++i, ++markerEnd)
            {
                auto const c = m_caption[i];
                if (serviceSymbols.find(c) == CStr_npos)
                {
                    res += CStr(c);
                    bEsc = false;
                }
                else if (c == '#')
                {
                    if (bEsc)
                    {
                        res += CStr(c);
                        bEsc = false;
                    }
                    else
                    {
                        bEsc = true;
                    }
                }
                else if (bEsc)
                {
                    res += CStr(c);
                    bEsc = false;
                }
                else if (c == '@' && markerEnd < len)
                {
                    // Skip the eight hex digits of a colour marker; a truncated one
                    // at the very end of the caption is left alone.
                    i += COLOR_MARKER_LEN;
                    markerEnd += COLOR_MARKER_LEN;
                }
            }
            return res;
        }

        int EditWnd::WriteToXmlNode(cmn::XmlFile* file, cmn::XmlNode* writeTo)
        {
            // RVA 0x6750A0
            if (!Wnd::WriteToXmlNode(file, writeTo))
            {
                return 0;
            }
            CStr cursorClr;
            cursorClr.format("%08x", m_cursorColor);
            writeTo->SetAttribute("cursorColor", cursorClr.c_str());
            return 1;
        }

        int EditWnd::ReadFromXmlNode(cmn::XmlFile* xmlFile, cmn::XmlNode* xmlNode)
        {
            // RVA 0x676300
            if (!Wnd::ReadFromXmlNode(xmlFile, xmlNode))
            {
                return 0;
            }
            unsigned clr = 1;
            SafeClrAttrib(clr, xmlNode, "cursorColor");
            SetCursorColor(clr);
            return 1;
        }

        int EditWnd::OnPaint(DrawInfo const& di)
        {
            // RVA 0x7242B0
            if ((m_style & WS_NOFRAME) == 0)
            {
                auto clr = m_curClr;
                if ((m_style & WS_DISABLE) != 0 || (m_style & WS_GRAYED) != 0)
                {
                    clr = 3;
                }
                DrawNonClient(di, clr);
            }

            auto sz = GetGfxServer()->MeasureText(GetText(), m_defFont, TW_NOWRAP, 0.0);
            if (m_gotFocus && m_bQuickEraseState)
            {
                // The whole text is shown selected until the first keystroke.
                auto selectRect = GetClientBounds();
                selectRect.width = sz.x;
                GetGfxServer()->AddFlatAxialQuad(di, selectRect, 1);
            }
            DrawWndText(di);

            if (!m_gotFocus)
            {
                return 1;
            }

            auto const maxPos = GetMaxCursorPos();
            if (m_cursorPosition < 0 || m_cursorPosition > maxPos)
            {
                m_cursorPosition = maxPos;
            }

            auto const strPos = CursorPos2StrPos(m_cursorPosition);
            PointBase<float> origin;
            origin.x = GetGfxServer()->MeasureText(m_caption.substr(0, strPos), m_defFont, TW_NOWRAP, 0.0).x;
            origin.y = 0.0;
            if ((m_style & WS_TEXT_CENTERED_Y) != 0)
            {
                if (m_caption.empty())
                {
                    // An empty box still needs a line height to centre against.
                    sz.y = GetGfxServer()->MeasureText("A", m_defFont, TW_NOWRAP, 0.0).y;
                }
                auto const y = (di.m_clientRect.height - sz.y) * 0.5f;
                origin.y = y;
                if (y < 0.0)
                {
                    origin.y = 0.0;
                }
                else if (y > di.m_clientRect.height)
                {
                    origin.y = di.m_clientRect.height;
                }
            }

            if (g_Kernel->GetTimer().GetFrameStartTimeUnscaled() % CARET_BLINK_PERIOD > CARET_BLINK_ON_AFTER)
            {
                GetGfxServer()->AddText(di, origin, m_strCursorColor + "_", m_defFont, TW_NOWRAP, TF_LEFT);
            }
            return 1;
        }

        int EditWnd::OnKey(unsigned short key, unsigned char scanCode, unsigned state)
        {
            // RVA 0x724740
            if (!state)
            {
                return (m_style & WS_REFLECT_MS_AND_KEYS_TO_PARENT) ? 0 : 1;
            }

            CStr const save = m_caption;
            switch (static_cast<char>(key))
            {
            case KBD_BKSPACE:
                m_bQuickEraseState = false;
                if (m_cursorPosition > 0 && m_cursorPosition - 1 < GetMaxCursorPos())
                {
                    DeleteSymbol(m_cursorPosition - 1);
                    DecrementCursorPosition();
                }
                break;
            case KBD_HOME:
                m_bQuickEraseState = false;
                m_cursorPosition = 0;
                break;
            case KBD_END:
                m_bQuickEraseState = false;
                m_cursorPosition = GetMaxCursorPos();
                break;
            case KBD_LEFT:
                m_bQuickEraseState = false;
                DecrementCursorPosition();
                break;
            case KBD_RIGHT:
                m_bQuickEraseState = false;
                IncrementCursorPosition();
                break;
            case KBD_DELETE:
                if (m_bQuickEraseState)
                {
                    // Delete on a freshly activated box wipes the whole text.
                    m_caption = CStr();
                    m_bQuickEraseState = false;
                    m_cursorPosition = 0;
                }
                else if (m_cursorPosition >= 0 && m_cursorPosition < GetMaxCursorPos())
                {
                    DeleteSymbol(m_cursorPosition);
                }
                break;
            default:
                if ((key & 0xff) >= 0x20)
                {
                    if (m_bQuickEraseState)
                    {
                        m_caption = CStr();
                        m_bQuickEraseState = false;
                        m_cursorPosition = 0;
                    }
                    if ((m_style & WS_EDIT_NUMERIC) == 0 || (key >= '0' && key <= '9'))
                    {
                        auto const maxPos = GetMaxCursorPos();
                        if (m_cursorPosition >= 0 && m_cursorPosition <= maxPos)
                        {
                            CStr const serviceSymbols = SERVICE_SYMBOLS;
                            auto const c = static_cast<char>(key);
                            // A typed service symbol is stored escaped so it does
                            // not turn into markup.
                            CStr const strToAdd =
                                serviceSymbols.find(c) == CStr_npos ? CStr(c) : CStr('#') + CStr(c);

                            if (m_cursorPosition == maxPos)
                            {
                                m_caption += strToAdd;
                            }
                            else
                            {
                                auto const strPos = CursorPos2StrPos(m_cursorPosition);
                                m_caption = m_caption.substr(0, strPos) + strToAdd +
                                            m_caption.substr(strPos, CStr_npos);
                            }
                            IncrementCursorPosition();
                        }
                    }
                }
                break;
            }

            if (m_cursorPosition < 0 || m_cursorPosition > GetMaxCursorPos())
            {
                m_cursorPosition = 0;
            }
            if (save != m_caption)
            {
                CallParentNotify(5u, {}, false);
            }
            return (m_style & WS_REFLECT_MS_AND_KEYS_TO_PARENT) ? 0 : 1;
        }

        int EditWnd::OnActivate(bool on)
        {
            // RVA 0x723970
            auto const res = Wnd::OnActivate(on);
            if (on && (m_style & WS_EDIT_QUICK_ERASE_ON_ACTIVATION) != 0)
            {
                m_bQuickEraseState = true;
            }
            return res;
        }

        int EditWnd::OnMouseButton0(unsigned state, PointBase<float> const& at)
        {
            // RVA 0x7239D0 - clicking into the box cancels the select-all state.
            if (state && m_bQuickEraseState)
            {
                m_bQuickEraseState = false;
            }
            return Wnd::OnMouseButton0(state, at);
        }
    }  // namespace ui
}  // namespace m3d
