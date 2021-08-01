#pragma once
#include "ui.h"

namespace m3d
{
    namespace ui
    {
        class EditWnd : public Wnd
        {
        public:
            static Class* __fastcall GetBaseClass();
            static Object* __fastcall CreateObject();

        public:
            int IncrementCursorPosition();
            virtual int ReadFromXmlNode(cmn::XmlFile*, cmn::XmlNode*);
            void DeleteSymbol(int);
            void SetQuickEraseState(bool);
            virtual Object* Clone();
            int GetMaxCursorPos() const;
            virtual int SetText(CStr const&);
            int GetMinCursorPos() const;
            virtual ~EditWnd();
            virtual int WriteToXmlNode(cmn::XmlFile*, cmn::XmlNode*);
            int CursorPos2StrPos(int) const;
            int SetCursorPosition(int);
            int DecrementCursorPosition();
            bool IsInQuickEraseState() const;
            int GetCursorPosition() const;
            virtual int Create(CStr const&, unsigned int, BoundsBase<float> const&, unsigned int);
            CStr GetVisibleText() const;
            void SetCursorColor(unsigned int);
            virtual Class* GetClass() const;
            unsigned int GetCursorColor() const;

        protected:
            virtual int OnActivate(bool);
            virtual int OnMouseButton0(unsigned int, PointBase<float> const&);
            EditWnd();
            EditWnd(EditWnd const&);
            virtual int OnPaint(DrawInfo const&);
            virtual int OnKey(unsigned short, unsigned char, unsigned int);

        private:
            bool m_bQuickEraseState;
            int m_cursorPosition;
            unsigned int m_cursorColor;
            CStr m_strCursorColor;
        };
    }
}
