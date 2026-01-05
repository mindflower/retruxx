#pragma once
#include "image.h"
#include <skelmodel.h>

namespace m3d
{
    namespace ui
    {
        class ModelWnd : public ImageWnd
        {
        public:
            static Class* GetBaseClass();
            static Object* CreateObject();
            static rend::TexHandle CreateTextureAsRenderTarget(PointBase<int> const&);
            static PointBase<int> GetFitTargetTextureSize(BoundsBase<float> const&);

        public:
            virtual int ReadFromXmlNode(cmn::XmlFile*, cmn::XmlNode*);
            int SetTargetTexture(rend::TexHandle);
            AnimInfo* Animation();
            CVector& Scale();
            rend::TexHandle GetTargetTexture() const;
            virtual int WriteToXmlNode(cmn::XmlFile*, cmn::XmlNode*);
            virtual int OnPaint(DrawInfo const&);
            Quaternion& Rotation();
            virtual Class* GetClass() const;
            int SetModel(AnimatedModel*);
            virtual ~ModelWnd();
            void SetCfgNum(unsigned int);
            virtual Object* Clone();
            unsigned int& SkinNum();
            AnimatedModel* GetModel();
            CVector& Translation();
            virtual int CreateModelWnd(rend::TexHandle, unsigned int, BoundsBase<float> const&, unsigned int, rend::TexHandle);
            virtual int CreateModelWnd(CStr const&, unsigned int, BoundsBase<float> const&, unsigned int, rend::TexHandle);

        protected:
            virtual int CreateImageWnd(BoundsBase<float> const&, CStr const&);
            virtual int CreateImageWnd(BoundsBase<float> const&, rend::TexHandle);
            ModelWnd(ModelWnd const&);
            ModelWnd();

        public:
            RT_CLASS_DECLARE(ModelWnd);

        protected:
            rend::TexHandle m_renderTexture;
            AnimatedModel* m_Model = nullptr;
            AnimInfo* m_Animation = nullptr;
            Configuration m_cfg;
            unsigned int m_SkinNum = 0;
            CVector m_Scale{1.0, 1.0, 1.0};
            CVector m_Translation{0.0, 0.0, 0.0};
            Quaternion m_Rotation{0.0, 0.0, 0.0, 1.0};
            unsigned int m_LastTimeCalled;
        };
    }
}
