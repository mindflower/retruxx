#pragma once
#include "image.h"
#include <math/quaternion.h>

namespace m3d
{
    class AnimatedModel;
    class AnimInfo;
    class Configuration;
}

namespace m3d
{
    namespace ui
    {
        class ModelArrayWnd : public ImageWnd
        {
        public:
            RT_CLASS_DECLARE(ModelArrayWnd);

        public:
            static Object* CreateObject();
            static Class* GetBaseClass();

        public:
            rend::TexHandle GetTargetTexture() const;
            int SetTargetTexture(rend::TexHandle);
            virtual Object* Clone();
            virtual int OnPaint(DrawInfo const&);
            virtual int CreateModelArrayWnd(rend::TexHandle, unsigned int, BoundsBase<float> const&, unsigned int, rend::TexHandle);
            virtual int CreateModelArrayWnd(CStr const&, unsigned int, BoundsBase<float> const&, unsigned int, rend::TexHandle);
            void ClearModels();
            virtual ~ModelArrayWnd();
            int GetSkinNum(int) const;
            void SetSkinNum(int, int);
            void AddModel(AnimatedModel*, CMatrix const&);
            virtual int ReadFromXmlNode(cmn::XmlFile*, cmn::XmlNode*);
            void SetCfgNum(unsigned int, unsigned int);
            virtual Class* GetClass() const;
            virtual int WriteToXmlNode(cmn::XmlFile*, cmn::XmlNode*);

        protected:
            ModelArrayWnd();
            ModelArrayWnd(ModelArrayWnd const&);
            virtual int CreateImageWnd(BoundsBase<float> const&, rend::TexHandle);
            virtual int CreateImageWnd(BoundsBase<float> const&, CStr const&);

        private:
            rend::TexHandle m_renderTexture;
            std::vector<AnimatedModel*> m_models;
            std::vector<AnimInfo*> m_animations;
            std::vector<CMatrix> m_xForms;
            std::vector<Configuration> m_cfgs;
            std::vector<unsigned int> m_SkinNums;
            CVector m_Scale;
            CVector m_Translation;
            Quaternion m_Rotation;
        };
    }
}
