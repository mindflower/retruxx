#pragma once
#include <core/stringm3d.h>
#include <core/ref_ptr.h>
#include <renderer/i_renderer.h>
#include "ui.h"
#include <vector>
#include <math/bounds2d.h>

namespace m3d {
    class AIParam;
}

namespace m3d
{
    namespace ui
    {
        class GlyphButton;

        class TabItemInfo
        {
        public:
            TabItemInfo();
            TabItemInfo(CStr const&, CStr const&, rend::TexHandle, rend::TexHandle, int);
            void Set(CStr const&, CStr const&, rend::TexHandle, rend::TexHandle, int);

        private:
            CStr m_caption;
            CStr m_diz;
            rend::TexHandle m_image;
            rend::TexHandle m_imageDisabled;
            int m_data;
            bool m_bCenterX;
            bool m_bCenterY;
        };

        class TabItem
        {
        public:
            void SetInfo(TabItemInfo const&);
            void SetWorkWnd(ref_ptr<Wnd>);
            ref_ptr<Wnd> GetWorkWnd() const;
            TabItemInfo const& GetInfo() const;
            ~TabItem();
            TabItem(ref_ptr<Wnd>, TabItemInfo const&, int);

        private:
            int m_idx;
            ref_ptr<Wnd> m_workWnd;
            TabItemInfo m_info;
        };

        class TabButtonInfo
        {
        public:
            enum Alignment
            {
                ALIGNMENT_LEFT = 0x0,
                ALIGNMENT_RIGHT = 0x1,
            };

            /* 153 */
            enum DrawStyle
            {
                DRAWSTYLE_NORMAL = 0x0,
                DRAWSTYLE_IZVRAT = 0x1,
            };

        public:
            TabButtonInfo();
            void SetIzvrat(float, float, float, Alignment);
            void SetNormal(float, float, float, float, Alignment, float);

        private:
            float m_minWidth;
            float m_maxWidth;
            float m_height;
            float m_spaceBetweenButtons;
            float m_glyphSz;
            DrawStyle m_drawStyle;
            Alignment m_alignment;
        };

        class TabWnd : public Wnd
        {
        public:
            RT_CLASS_DECLARE(TabWnd);

        public:
            enum SelectionType
            {
                BY_PROGRAM = 0x0,
                BY_MOUSE = 0x1,
                BY_KB = 0x2,
            };

            class AuxInfo
            {
            public:
                AuxInfo();

            private:
                float m_dizSpace;
                float m_clientEdge;
                float m_btnClientEdge;
            };

        public:
            static Object* CreateObject();
            int SetCurSel(int);
            virtual ~TabWnd();
            virtual Class* GetClass() const;
            int AddItem(ref_ptr<Wnd>, TabItemInfo const&);
            TabItem const* GetItem(int) const;
            int GetCurSel() const;
            virtual int Create(CStr const&, unsigned int, BoundsBase<float> const&, unsigned int);
            TabButtonInfo const& GetTabButtonInfo() const;
            void RemoveAllItems();
            static Class* GetBaseClass();
            virtual void SetClientEdges(std::vector<float, std::allocator<float> > const&);
            virtual void SetClientEdges(float, float, float, float);
            void SetTabButtonInfo(TabButtonInfo const&);
            virtual Object* Clone();
            int GetItemsCount() const;
            virtual void AdjustWorkWindowRect(Wnd*, TabItemInfo const&) const;
        protected:
            int _SetCurSel(int, enum SelectionType);
            PointBase<float> RecalcButtonSize(int);
            virtual void DrawWndText(DrawInfo const&);
            std::vector<float, std::allocator<float> > _GetClientEdges() const;
            virtual void OnNcPaint(DrawInfo const&, unsigned int);
            virtual int OnWndNotify(Wnd*, unsigned int, unsigned int, AIParam const&);
            void OnAfterChangeCurSel(int, enum SelectionType);
            int AddButton(int);
            bool IsItemIdValid(int) const;
            PointBase<float> RecalcButtonPos(int);
            virtual int OnKey(unsigned short, unsigned char, unsigned int);
            void OnBeforeChangeCurSel(int, enum SelectionType);
            TabWnd();
            TabWnd(TabWnd const&);
            float _GetTopEdgeDiff() const;
        private:
            std::vector<TabItem*> m_items;
            std::vector<GlyphButton*> m_buttons;
            int m_curSel;
            TabButtonInfo m_buttonInfo;
            AuxInfo m_aif;
        };
    }
}
