#pragma once
#include "image.h"
#include <skelmodel.h>

class ContextModelWnd;

namespace m3d
{
    namespace ui
    {
        class ModelWnd : public m3d::ui::ImageWnd
        {
            // ContextModelWnd owns a ModelWnd and re-stamps its skin number
            // directly after SetModelByName, exactly as the shipped code does.
            friend class ::ContextModelWnd;

        protected:
            ModelWnd();
            ModelWnd(m3d::ui::ModelWnd const& rhs);

        public:
            virtual ~ModelWnd() override;
            virtual m3d::Object* Clone() override;
            static m3d::Object* CreateObject();
            static m3d::Class* GetBaseClass();
            virtual m3d::Class* GetClass() const override;
            static m3d::Class m_classModelWnd;
            virtual int CreateModelWnd(
                m3d::rend::TexHandle imageTex,
                unsigned int style,
                BoundsBase<float> const& rc,
                unsigned int id,
                m3d::rend::TexHandle targetTex);
            virtual int CreateModelWnd(
                CStr const& imageFile,
                unsigned int style,
                BoundsBase<float> const& rc,
                unsigned int id,
                m3d::rend::TexHandle targetTex);
            static PointBase<int> GetFitTargetTextureSize(BoundsBase<float> const& wndBounds);
            static m3d::rend::TexHandle CreateTextureAsRenderTarget(PointBase<int> const& texSz);
            int SetTargetTexture(m3d::rend::TexHandle targetTex);
            m3d::rend::TexHandle GetTargetTexture() const;
            virtual int ReadFromXmlNode(m3d::cmn::XmlFile* file, m3d::cmn::XmlNode* node) override;
            virtual int WriteToXmlNode(m3d::cmn::XmlFile* file, m3d::cmn::XmlNode* writeTo) override;
            virtual int OnPaint(m3d::ui::DrawInfo const& di) override;
            int SetModel(m3d::AnimatedModel* Mdl);
            unsigned int GetCfgNum();
            void SetCfgNum(unsigned int cfgNum);
            unsigned int& SkinNum();
            m3d::AnimatedModel* GetModel();
            CVector& Translation();
            Quaternion& Rotation();
            CVector& Scale();
            m3d::AnimInfo* Animation();

        protected:
            virtual int CreateImageWnd(BoundsBase<float> const& rc, m3d::rend::TexHandle tex) override;
            virtual int CreateImageWnd(BoundsBase<float> const& rc, CStr const& textureName) override;
            m3d::rend::TexHandle m_renderTexture;
            m3d::AnimatedModel* m_Model;
            m3d::AnimInfo* m_Animation;
            m3d::Configuration m_cfg;
            unsigned int m_SkinNum;
            CVector m_Scale;
            CVector m_Translation;
            Quaternion m_Rotation;
            unsigned int m_LastTimeCalled;
        };
    }  // namespace ui
}  // namespace m3d
