#pragma once
#include <vector>
#include <math/point2d.h>
#include <renderer/i_renderer.h>

namespace m3d
{
    namespace cmn
    {
        class XmlNode;
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

        private:
            CStr m_name;
            rend::TexHandle m_textures[12];
            int m_barRepeat;
            int m_barTexWidth;
            int m_barUsedWidth;
            int m_cornerSize;
            int m_cornerRoundSize;
        };

        class BackGround
        {
        public:
            ~BackGround();
            int ReadFromXmlNode(cmn::XmlNode*);
            BackGround();

        private:
            CStr m_name;
            int m_repeatU;
            int m_repeatV;
            rend::TexHandle m_texture;
        };


        class Pane
        {
        public:
            ~Pane();
            Pane();
            int ReadFromXmlNode(cmn::XmlNode*, std::vector<BackGround*> const&, std::vector<Frame*> const&);

        private:
            CStr m_name;
            Frame* m_frame[4];
            BackGround* m_bg[4];
            bool m_bInnerBg;
        };

        class ScrollPane
        {
        public:
            float GetWidth() const;
            int ReadFromXmlNode(m3d::cmn::XmlNode*);
            ~ScrollPane();
            ScrollPane();

        private:
            CStr m_name;
            rend::TexHandle m_thumbTex;
            CStr m_barPaneName;
            CStr m_prevButtonPaneName;
            CStr m_nextButtonPaneName;
            float m_barWidth;
            PointBase<float> m_thumbSize;
            PointBase<float> m_btnSize;
            float m_space;
        };
    }
}
