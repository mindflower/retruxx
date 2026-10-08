#pragma once
#include "ui.h"
#include <renderer/i_renderer.h>

namespace m3d
{
    namespace ui
    {
        class ProgressBarWnd : public Wnd
        {
        public:
            enum Orientation
            {
                ORIENTATION_TOP_TO_BOTTOM = 0x0,
                ORIENTATION_BOTTOM_TO_TOP = 0x1,
                ORIENTATION_LEFT_TO_RIGHT = 0x2,
                ORIENTATION_RIGHT_TO_LEFT = 0x3,
            };

            enum TextStyle
            {
                TEXT_NONE = 0x0,
                TEXT_INTEGER = 0x1,
                TEXT_FLOAT = 0x2,
            };

            enum TextureStyle
            {
                TEXTURE_STRETCH = 0x0,
                TEXTURE_CLAMP = 0x1,
                TEXTURE_REPEAT = 0x2,
            };
        public:

            unsigned int GetBarColor() const;
            float GetMaxValue() const;
            void SetCurValue(float);
            rend::TexHandle GetBarTexture() const;
            void SetTextStyle(enum TextStyle);
            void SetOrientation(enum Orientation);
            virtual int ReadFromXmlNode(cmn::XmlFile*, cmn::XmlNode*);
            int GetNumOfSteps() const;
            void SetBarColor(unsigned int);
            void SetMaxValue(float);
            virtual Object* Clone();
            float GetCurValue() const;
            void SetBarTexture(rend::TexHandle);
            void SetBarTexture(CStr const&);
            enum TextStyle GetTextStyle() const;
            static Class* GetBaseClass();
            enum Orientation GetOrientation() const;
            void SetNumOfSteps(int);
            virtual Class* GetClass() const;
            enum TextureStyle GetTextureStyle() const;
            static Object* CreateObject();
            float GetMinValue() const;
            virtual ~ProgressBarWnd();
            void SetTextureStyle(enum TextureStyle);
            virtual int WriteToXmlNode(cmn::XmlFile*, cmn::XmlNode*);
            void SetMinValue(float);

        protected:
            float GetSizeOfStepInPixel() const;
            int GetCurNumOfSteps() const;
            virtual CStr GetStringValue() const;
            virtual BoundsBase<float> GetBarRect() const;
            virtual int OnPaint(DrawInfo const&);
            bool IsInversed() const;
            void CalcTexCoordinates(float&, float&, float&, float&) const;
            float GetMaxValueInPixel() const;
            bool IsHorizontal() const;
            float GetValueInPixel() const;
            ProgressBarWnd(ProgressBarWnd const&);
            ProgressBarWnd();
            virtual BoundsBase<float> GetMaxBarRect() const;

        public:
            RT_CLASS_DECLARE(ProgressBarWnd);

        protected:
            /* 0x0220 */ float m_curValue = 0.0;
            /* 0x0224 */ float m_minValue = 0.0;
            /* 0x0228 */ float m_maxValue = 0.0;
            /* 0x022c */ unsigned int m_barColor = 0;
            /* 0x0230 */ rend::TexHandle m_barTexture;
            /* 0x0234 */ Orientation m_orientation = ORIENTATION_LEFT_TO_RIGHT;
            /* 0x0238 */ TextStyle m_textStyle = TEXT_NONE;
            /* 0x023c */ TextureStyle m_textureStyle = TEXTURE_STRETCH;

        private:
            /* 0x0240 */ int m_numOfSteps = 1;
        };
    }
}
