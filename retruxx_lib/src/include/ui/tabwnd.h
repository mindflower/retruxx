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

        struct TabButtonInfo
        {
            enum DrawStyle
            {
                DRAWSTYLE_NORMAL = 0,
                DRAWSTYLE_IZVRAT = 1,
            };

            enum Alignment
            {
                ALIGNMENT_LEFT = 0,
                ALIGNMENT_RIGHT = 1,
            };

            /* 0x0000 */ float m_minWidth;
            /* 0x0004 */ float m_maxWidth;
            /* 0x0008 */ float m_height;
            /* 0x000c */ float m_spaceBetweenButtons;
            /* 0x0010 */ float m_glyphSz;
            /* 0x0014 */ m3d::ui::TabButtonInfo::DrawStyle m_drawStyle;
            /* 0x0018 */ m3d::ui::TabButtonInfo::Alignment m_alignment;
            TabButtonInfo();
            void SetNormal(float minW, float maxW, float h, float space, m3d::ui::TabButtonInfo::Alignment alignment, float glyphSz);
            void SetIzvrat(float w, float h, float space, m3d::ui::TabButtonInfo::Alignment alignment);
        }; /* size: 0x001c */

        class TabWnd : public Wnd
        {
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

        public:
            RT_CLASS_DECLARE(TabWnd);

        private:
            std::vector<TabItem*> m_items;
            std::vector<GlyphButton*> m_buttons;
            int m_curSel;
            TabButtonInfo m_buttonInfo;
            AuxInfo m_aif;
        };
    }
}
