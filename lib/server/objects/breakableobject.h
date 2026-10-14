#pragma once
#include "base/simplephysicobj.h"

struct dxJoint;

namespace ai
{
    class BlastWave;
    class BoxyBody;

    class BreakableObjectPrototypeInfo : public ai::SimplePhysicObjPrototypeInfo
    {
    public:
        int m_destroyable;
        float m_criticalHitEnergy;
        CStr m_effectType;
        CStr m_destroyEffectType;
        CStr m_brokenModelName;
        CStr m_destroyedModelName;
        CStr m_breakEffect;
        int m_BlastWavePrototypeId;
        CStr m_BlastWavePrototypeName;
        BreakableObjectPrototypeInfo();
        virtual bool LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override;
        virtual void PostLoad() override;
        virtual void RefreshFromXml(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override;
        virtual ai::Obj* CreateTargetObject() const override;
    };

    class BreakableObject : public ai::SimplePhysicObj
    {
        friend class BreakableObjectPrototypeInfo;
    protected:
        virtual  ~BreakableObject() override;

    private:
        BreakableObject(const ai::BreakableObjectPrototypeInfo& prototypeInfo);
        BreakableObject(const ai::BreakableObject&);
        virtual m3d::Object* Clone() override;
        static m3d::Object* __fastcall CreateObject();

    public:
        static m3d::Class* __fastcall GetBaseClass();
        virtual m3d::Class* GetClass() const override;
        static m3d::Class m_classBreakableObject;
        virtual const ai::BreakableObjectPrototypeInfo* GetPrototypeInfo() const override;

        enum STATES
        {
            DISABLED = 0,
            ENABLED = 1,
            REMOVED = 2,
        };

    private:
        int m_destroyable;
        float m_criticalHitEnergy;
        ai::BreakableObject::STATES m_state;
        unsigned short m_effectType;
        unsigned short m_destroyEffectType;
        dxJoint* m_jointId;
        CStr m_removingEffectName;
        CVector m_causePos;
        float m_causeForce;
        bool m_initVelocities;
        bool m_bPositioningOnGround;
        retruxx::set<int, retruxx::less<int>, retruxx::allocator<int> > m_connectedRopes;

    public:
        virtual void LoadRuntimeValues(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override;
        virtual void SaveRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const override;
        virtual void SetPosition(const CVector& pos) override;
        virtual void SetRotation(const Quaternion& rot) override;
        virtual void SetPositionSelf(const CVector& pos) override;
        virtual void RenderDebugInfo() const override;
        virtual CVector GetGeometricCenter() const override;
        virtual bool CanChildBeAdded(m3d::Class* pClass) const override;
        virtual void AddChild(ai::Obj* pObj) override;
        virtual bool RemoveChild(ai::Obj* pChild) override;
        void DoTouch();
        void SetState(ai::BreakableObject::STATES state);
        void SetEnabled();
        void SetDestroyed();
        void SetJointAnchor(const CVector& vec);
        ai::BlastWave* CreateBlastWave() const;
        int IsDestroyable();
        float GetCriticalHitEnergy();
        unsigned short GetEffectType();
        unsigned short GetDestroyEffectType();
        ai::BreakableObject::STATES GetState();
        const ai::BoxyBody* _BoxyBody() const;
        ai::BoxyBody* _BoxyBody();
        CVector GetBounds();
        void SetPositioningOnGround(bool);
        const CStr& GetBrokenModelName() const;
        const CStr& GetDestroyedModelName() const;
        bool IsExplosive() const;
        void ConnectWithRope(int ropeId);
        bool DisconnectFromRope(int ropeId);
        const retruxx::set<int, retruxx::less<int>, retruxx::allocator<int> >& GetConnectedRopes();
        void SetRemovingEffectName(const CStr& name);
        void SetCausePos(const CVector& pos);
        void SetCauseForce(float force);
        void SetInitVelocities(bool initVelocities);

    protected:
        virtual void _Construct() override;
        void _BreakOffConnectedRopes(bool createJoint);
        void _StifleChildLights();
    };
}
