#pragma once
#include "ui.h"

namespace m3d
{
    namespace ui
    {
        class EditWnd : public Wnd
        {
        protected:
            EditWnd();
            EditWnd(m3d::ui::EditWnd const& ew);

        public:
            virtual ~EditWnd() override /* 0x00 */;
            virtual Object* Clone() override /* 0x04 */;
            static Object* __fastcall CreateObject();
            static Class* __fastcall GetBaseClass();
            virtual Class* GetClass() const override /* 0x34 */;
            RT_CLASS_DECLARE(EditWnd);

            virtual int Create(CStr const& caption, unsigned int style, BoundsBase<float> const& rc, unsigned int id) override /* 0x44 */;
            void SetQuickEraseState(bool bQuickErase);
            bool IsInQuickEraseState() const;
            virtual int SetText(CStr const& caption) override /* 0x48 */;
            unsigned int GetCursorColor() const;
            void SetCursorColor(unsigned int color);
            int GetCursorPosition() const;
            int SetCursorPosition(int pos);
            int GetMaxCursorPos() const;
            int GetMinCursorPos() const;
            int CursorPos2StrPos(int cursorPos) const;
            void DeleteSymbol(int cursorPos);
            CStr GetVisibleText() const;
            virtual int WriteToXmlNode(cmn::XmlFile* file, cmn::XmlNode* writeTo) override /* 0x10 */;
            virtual int ReadFromXmlNode(cmn::XmlFile* file, cmn::XmlNode* node) override /* 0x08 */;
            int IncrementCursorPosition();
            int DecrementCursorPosition();

        protected:
            virtual int OnPaint(DrawInfo const& di) override /* 0x88 */;
            virtual int OnKey(unsigned short key, unsigned char scanCode, unsigned int state) override /* 0xbc */;
            virtual int OnActivate(bool on) override /* 0xcc */;
            virtual int OnMouseButton0(unsigned int state, PointBase<float> const& at) override /* 0xa4 */;

            /* 0x0220 */ bool m_bQuickEraseState = false;
            /* 0x0224 */ int m_cursorPosition = 0;
            /* 0x0228 */ unsigned int m_cursorColor = 0;
            /* 0x022c */ CStr m_strCursorColor;
        }; /* size: 0x0238 */
    }
}
