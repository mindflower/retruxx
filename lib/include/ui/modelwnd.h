#pragma once
#include "image.h"
#include <skelmodel.h>

namespace m3d
{
    namespace ui
    {
        class ModelWnd : public m3d::ui::ImageWnd
        {
        protected:
            ModelWnd();
            ModelWnd(m3d::ui::ModelWnd const& rhs);

        public:
            virtual ~ModelWnd() override /* 0x00 */;
            virtual m3d::Object* Clone() override /* 0x04 */;
            static m3d::Object* __fastcall CreateObject();
            static m3d::Class* __fastcall GetBaseClass();
            virtual m3d::Class* GetClass() const override /* 0x34 */;
            static m3d::Class m_classModelWnd;
            virtual int CreateModelWnd(
                m3d::rend::TexHandle imageTex,
                unsigned int style,
                BoundsBase<float> const& rc,
                unsigned int id,
                m3d::rend::TexHandle targetTex) /* 0x128 */;
            virtual int CreateModelWnd(
                CStr const& imageFile,
                unsigned int style,
                BoundsBase<float> const& rc,
                unsigned int id,
                m3d::rend::TexHandle targetTex) /* 0x128 */;
            static PointBase<int> __fastcall GetFitTargetTextureSize(BoundsBase<float> const& wndBounds);
            static m3d::rend::TexHandle __fastcall CreateTextureAsRenderTarget(PointBase<int> const& texSz);
            int SetTargetTexture(m3d::rend::TexHandle targetTex);
            m3d::rend::TexHandle GetTargetTexture() const;
            virtual int ReadFromXmlNode(m3d::cmn::XmlFile* file, m3d::cmn::XmlNode* node) override /* 0x08 */;
            virtual int WriteToXmlNode(m3d::cmn::XmlFile* file, m3d::cmn::XmlNode* writeTo) override /* 0x10 */;
            virtual int OnPaint(m3d::ui::DrawInfo const& di) override /* 0x88 */;
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
            virtual int CreateImageWnd(BoundsBase<float> const& rc, m3d::rend::TexHandle tex) override /* 0x120 */;
            virtual int CreateImageWnd(BoundsBase<float> const& rc, CStr const& textureName) override /* 0x120 */;
            /* 0x0234 */ m3d::rend::TexHandle m_renderTexture;
            /* 0x0238 */ m3d::AnimatedModel* m_Model;
            /* 0x023c */ m3d::AnimInfo* m_Animation;
            /* 0x0240 */ m3d::Configuration m_cfg;
            /* 0x0264 */ unsigned int m_SkinNum;
            /* 0x0268 */ CVector m_Scale;
            /* 0x0274 */ CVector m_Translation;
            /* 0x0280 */ Quaternion m_Rotation;
            /* 0x0290 */ unsigned int m_LastTimeCalled;
        }; /* size: 0x0294 */
    }  // namespace ui
}  // namespace m3d
