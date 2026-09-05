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
            virtual ~SgNodeArrayWnd() override /* 0x00 */;
            virtual m3d::Object* Clone() override /* 0x04 */;
            static m3d::Object* __fastcall CreateObject();
            static m3d::Class* __fastcall GetBaseClass();
            virtual m3d::Class* GetClass() const override /* 0x34 */;
            static m3d::Class m_classSgNodeArrayWnd;
            virtual int CreateSgNodeArrayWnd(
                m3d::rend::TexHandle imageTex,
                unsigned int style,
                BoundsBase<float> const& rc,
                unsigned int id,
                m3d::rend::TexHandle targetTex) /* 0x128 */;
            virtual int CreateSgNodeArrayWnd(
                CStr const& imageFile,
                unsigned int style,
                BoundsBase<float> const& rc,
                unsigned int id,
                m3d::rend::TexHandle targetTex) /* 0x128 */;
            int SetTargetTexture(m3d::rend::TexHandle targetTex);
            m3d::rend::TexHandle GetTargetTexture() const;
            virtual int ReadFromXmlNode(m3d::cmn::XmlFile* file, m3d::cmn::XmlNode* node) override /* 0x08 */;
            virtual int WriteToXmlNode(m3d::cmn::XmlFile* file, m3d::cmn::XmlNode* writeTo) override /* 0x10 */;
            virtual int OnPaint(m3d::ui::DrawInfo const& di) override /* 0x88 */;
            void AddSgNode(m3d::SgNode* node, CMatrix const& xForm, bool withChilds);
            void ClearSgNodes();
            void SetClassesToRender(std::vector<m3d::Class*, std::allocator<m3d::Class*>> const& classes);
            CVector& Translation();
            Quaternion& Rotation();
            CVector& Scale();

        protected:
            virtual int CreateImageWnd(BoundsBase<float> const& rc, m3d::rend::TexHandle tex) override /* 0x120 */;
            virtual int CreateImageWnd(BoundsBase<float> const& rc, CStr const& textureName) override /* 0x120 */;
            /* 0x0234 */ m3d::rend::TexHandle m_renderTexture;
            /* 0x0238 */ std::vector<m3d::SgNode*, std::allocator<m3d::SgNode*>> m_nodes;
            /* 0x0248 */ std::vector<CMatrix, std::allocator<CMatrix>> m_xForms;
            /* 0x0258 */ std::vector<bool, std::allocator<bool>> m_withChilds;
            /* 0x026c */ std::vector<m3d::Class*, std::allocator<m3d::Class*>> m_classesToRender;
            /* 0x027c */ CVector m_Scale;
            /* 0x0288 */ CVector m_Translation;
            /* 0x0294 */ Quaternion m_Rotation;
        }; /* size: 0x02a4 */
    }  // namespace ui
}  // namespace m3d
