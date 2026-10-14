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
            virtual ~EditWnd() override;
            virtual Object* Clone() override;
            static Object* __fastcall CreateObject();
            static Class* __fastcall GetBaseClass();
            virtual Class* GetClass() const override;
            RT_CLASS_DECLARE(EditWnd);

            virtual int Create(CStr const& caption, unsigned int style, BoundsBase<float> const& rc, unsigned int id) override;
            void SetQuickEraseState(bool bQuickErase);
            bool IsInQuickEraseState() const;
            virtual int SetText(CStr const& caption) override;
            unsigned int GetCursorColor() const;
            void SetCursorColor(unsigned int color);
            int GetCursorPosition() const;
            int SetCursorPosition(int pos);
            int GetMaxCursorPos() const;
            int GetMinCursorPos() const;
            int CursorPos2StrPos(int cursorPos) const;
            void DeleteSymbol(int cursorPos);
            CStr GetVisibleText() const;
            virtual int WriteToXmlNode(cmn::XmlFile* file, cmn::XmlNode* writeTo) override;
            virtual int ReadFromXmlNode(cmn::XmlFile* file, cmn::XmlNode* node) override;
            int IncrementCursorPosition();
            int DecrementCursorPosition();

        protected:
            virtual int OnPaint(DrawInfo const& di) override;
            virtual int OnKey(unsigned short key, unsigned char scanCode, unsigned int state) override;
            virtual int OnActivate(bool on) override;
            virtual int OnMouseButton0(unsigned int state, PointBase<float> const& at) override;

            bool m_bQuickEraseState = false;
            int m_cursorPosition = 0;
            unsigned int m_cursorColor = 0;
            CStr m_strCursorColor;
        };
    }
}
