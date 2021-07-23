#pragma once
#include "base/obj.h"
#include "base/prototypeinfo.h"
#include <math/quaternion.h>
#include <math/vector.h>

namespace ai
{
    class SgNodeObjPrototypeInfo : public PrototypeInfo
    {
    public:
        virtual bool LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);
        SgNodeObjPrototypeInfo();
        CStr const& GetEngineModelName() const;
        virtual ai::Obj* CreateTargetObject() const;

    private:
        CStr m_engineModelName;
    };

    class SgNodeObj :  public Obj
    {
    public:
        SgNodeObj(SgNodeObjPrototypeInfo const &);
        virtual void Update(float,unsigned int);
        virtual void GetPropertiesNames(std::set<CStr,std::less<CStr>,std::allocator<CStr> > &) const ;
        virtual void SetRotation(Quaternion const &);
        static void __fastcall Registration();
        virtual bool SetPropertyById(int,m3d::AIParam const &);
        void SetSgNode(CStr const &);
        void SetSgNode();
        static m3d::Class * __fastcall GetBaseClass();
        Quaternion GetRotation() const ;
        virtual int GetPropertyId(char const *) const ;
        virtual SgNodeObjPrototypeInfo const * GetPrototypeInfo() const ;
        void DeleteSgNode();
        virtual eGObjPropertySaveStatus GetPropertySaveStatus(int) const ;
        virtual CStr GetPropertyName(int) const ;
        virtual m3d::Class * GetClass() const ;
        virtual void GetPropertiesIDs(std::set<int,std::less<int>,std::allocator<int> > &) const ;
        virtual void SetPosition(CVector const &);
        float GetScale() const ;
        CVector GetPosition() const ;
        virtual void SetScale(float);

    protected:
        virtual bool _GetPropertyDefaultInternal(int,m3d::AIParam &) const ;
        virtual bool _GetPropertyInternal(int,m3d::AIParam &) const ;
        static void __fastcall RegisterProperty(char const *,int,eGObjPropertySaveStatus);
        virtual void _InternalCreateVisualPart();
        virtual ~SgNodeObj();

    private:
        static m3d::Object * __fastcall CreateObject();
        virtual m3d::Object * Clone();

    private:
        m3d::SgNode *m_node;
        CStr m_modelName;
        CVector m_position;
        Quaternion m_rotation;
        float m_scale;
        bool m_needToRelink;
    };
}
