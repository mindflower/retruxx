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
        class SgNodeArrayWnd : public ImageWnd
        {
        public:
            static Object* CreateObject();
            static Class* GetBaseClass();

        public:
            virtual int WriteToXmlNode(cmn::XmlFile*, cmn::XmlNode*);
            CVector& Translation();
            void AddSgNode(SgNode*, CMatrix const&, bool);
            Quaternion& Rotation();
            virtual Class* GetClass() const;
            virtual ~SgNodeArrayWnd();
            void SetClassesToRender(std::vector<Class*> const&);
            void ClearSgNodes();
            virtual int OnPaint(DrawInfo const&);
            virtual Object* Clone();
            rend::TexHandle GetTargetTexture() const;
            virtual int ReadFromXmlNode(cmn::XmlFile*, cmn::XmlNode*);
            virtual int CreateSgNodeArrayWnd(CStr const&, unsigned int, BoundsBase<float> const&, unsigned int, rend::TexHandle);
            virtual int CreateSgNodeArrayWnd(rend::TexHandle, unsigned int, BoundsBase<float> const&, unsigned int, rend::TexHandle);
            int SetTargetTexture(rend::TexHandle);

        protected:
            virtual int CreateImageWnd(BoundsBase<float> const&, rend::TexHandle);
            virtual int CreateImageWnd(BoundsBase<float> const&, CStr const&);
            SgNodeArrayWnd();
            SgNodeArrayWnd(SgNodeArrayWnd const&);

        public:
            RT_CLASS_DECLARE(SgNodeArrayWnd);

        private:
            rend::TexHandle m_renderTexture;
            std::vector<SgNode*> m_nodes;
            std::vector<CMatrix> m_xForms;
            std::vector<bool> m_withChilds;
            std::vector<Class*> m_classesToRender;
            CVector m_Scale;
            CVector m_Translation;
            Quaternion m_Rotation;
        };
    }
}
