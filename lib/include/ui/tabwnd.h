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

        struct TabItemInfo
        {
            /* 0x0000 */ CStr m_caption;
            /* 0x000c */ CStr m_diz;
            /* 0x0018 */ rend::TexHandle m_image;
            /* 0x001c */ rend::TexHandle m_imageDisabled;
            /* 0x0020 */ int m_data;
            /* 0x0024 */ bool m_bCenterX;
            /* 0x0025 */ bool m_bCenterY;
            TabItemInfo(TabItemInfo const& rhs);
            TabItemInfo(CStr const& caption, CStr const& diz, rend::TexHandle image, rend::TexHandle imageDisabled,
                        int data);
            TabItemInfo();
            TabItemInfo& operator=(TabItemInfo const& rhs);
            void Set(CStr const& caption, CStr const& diz, rend::TexHandle image, rend::TexHandle imageDisabled,
                     int data);
        }; /* size: 0x0028 */

        class TabItem
        {
        public:
            TabItem(TabItem const& rhs);
            TabItem(ref_ptr<Wnd> workWnd, TabItemInfo const& info, int idx);
            ~TabItem();
            ref_ptr<Wnd> GetWorkWnd() const;
            void SetWorkWnd(ref_ptr<Wnd> workWnd);
            TabItemInfo const& GetInfo() const;
            void SetInfo(TabItemInfo const& info);

        protected:
            /* 0x0000 */ int m_idx;
            /* 0x0004 */ ref_ptr<Wnd> m_workWnd;
            /* 0x0008 */ TabItemInfo m_info;
        }; /* size: 0x0030 */

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

            struct AuxInfo
            {
                /* 0x0000 */ float m_dizSpace;
                /* 0x0004 */ float m_clientEdge;
                /* 0x0008 */ float m_btnClientEdge;
                AuxInfo();
            }; /* size: 0x000c */

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
