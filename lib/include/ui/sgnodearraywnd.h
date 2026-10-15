#pragma once
#include "image.h"
#include <math/quaternion.h>

namespace m3d
{
    class SgNode;
}

namespace m3d
{
    namespace ui
    {
        class SgNodeArrayWnd : public m3d::ui::ImageWnd
        {
        protected:
            SgNodeArrayWnd();
            SgNodeArrayWnd(m3d::ui::SgNodeArrayWnd const& rhs);

        public:
            virtual ~SgNodeArrayWnd() override;
            virtual m3d::Object* Clone() override;
            static m3d::Object* CreateObject();
            static m3d::Class* GetBaseClass();
            virtual m3d::Class* GetClass() const override;
            static m3d::Class m_classSgNodeArrayWnd;
            virtual int CreateSgNodeArrayWnd(
                m3d::rend::TexHandle imageTex,
                unsigned int style,
                BoundsBase<float> const& rc,
                unsigned int id,
                m3d::rend::TexHandle targetTex);
            virtual int CreateSgNodeArrayWnd(
                CStr const& imageFile,
                unsigned int style,
                BoundsBase<float> const& rc,
                unsigned int id,
                m3d::rend::TexHandle targetTex);
            int SetTargetTexture(m3d::rend::TexHandle targetTex);
            m3d::rend::TexHandle GetTargetTexture() const;
            virtual int ReadFromXmlNode(m3d::cmn::XmlFile* file, m3d::cmn::XmlNode* node) override;
            virtual int WriteToXmlNode(m3d::cmn::XmlFile* file, m3d::cmn::XmlNode* writeTo) override;
            virtual int OnPaint(m3d::ui::DrawInfo const& di) override;
            void AddSgNode(m3d::SgNode* node, CMatrix const& xForm, bool withChilds);
            void ClearSgNodes();
            void SetClassesToRender(std::vector<m3d::Class*, std::allocator<m3d::Class*>> const& classes);
            CVector& Translation();
            Quaternion& Rotation();
            CVector& Scale();

        protected:
            virtual int CreateImageWnd(BoundsBase<float> const& rc, m3d::rend::TexHandle tex) override;
            virtual int CreateImageWnd(BoundsBase<float> const& rc, CStr const& textureName) override;
            m3d::rend::TexHandle m_renderTexture;
            std::vector<m3d::SgNode*, std::allocator<m3d::SgNode*>> m_nodes;
            std::vector<CMatrix, std::allocator<CMatrix>> m_xForms;
            std::vector<bool, std::allocator<bool>> m_withChilds;
            std::vector<m3d::Class*, std::allocator<m3d::Class*>> m_classesToRender;
            CVector m_Scale;
            CVector m_Translation;
            Quaternion m_Rotation;
        };
    }  // namespace ui
}  // namespace m3d
