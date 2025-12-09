#pragma once
#include <math/point2d.h>
#include <renderer/i_renderer.h>
#include "retruxx/common.h"

namespace m3d
{
    namespace cmn
    {
        struct XmlNode;
    }
}

namespace m3d
{
    namespace ui
    {
        class Frame
        {
        public:
            ~Frame();
            void ReleaseTextures();
            Frame();
            int ReadFromXmlNode(cmn::XmlNode*);

        public:
            CStr m_name;
            rend::TexHandle m_textures[12];
            int m_barRepeat = 0;
            int m_barTexWidth = 0;
            int m_barUsedWidth = 0;
            int m_cornerSize = 0;
            int m_cornerRoundSize = 0;
        };

        class BackGround
        {
        public:
            ~BackGround();
            int ReadFromXmlNode(cmn::XmlNode*);
            BackGround();

        public:
            CStr m_name;
            int m_repeatU = 0;
            int m_repeatV = 0;
            rend::TexHandle m_texture;
        };


        class Pane
        {
        public:
            ~Pane();
            Pane();
            int ReadFromXmlNode(cmn::XmlNode*, retruxx::vector<BackGround*> const&, retruxx::vector<Frame*> const&);

        public:
            CStr m_name;
            Frame* m_frame[4] = {0};
            BackGround* m_bg[4] = {0};
            bool m_bInnerBg = false;
        };

        class ScrollPane
        {
        public:
            float GetWidth() const;
            int ReadFromXmlNode(m3d::cmn::XmlNode*);
            ~ScrollPane();
            ScrollPane();

        public:
            CStr m_name;
            rend::TexHandle m_thumbTex;
            CStr m_barPaneName = "PaneScrollBarDefault";
            CStr m_prevButtonPaneName = "PaneScrollBtnPrevDefault";
            CStr m_nextButtonPaneName = "PaneScrollBtnNextDefault";
            float m_barWidth = 20.0;
            PointBase<float> m_thumbSize{20.0, 20.0};
            PointBase<float> m_btnSize{20.0, 20.0};
            float m_space = 0.0;
        };
    }
}
