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
        virtual bool LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode) override;
        virtual void PostLoad() override;
        virtual void RefreshFromXml(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode) override;
        virtual ai::Obj* CreateTargetObject() const override;
    };

    class BreakableObject : public ai::SimplePhysicObj
    {
        friend class BreakableObjectPrototypeInfo;

    protected:
        virtual ~BreakableObject() override;

    private:
        BreakableObject(ai::BreakableObjectPrototypeInfo const& prototypeInfo);
        BreakableObject(ai::BreakableObject const&);
        virtual m3d::Object* Clone() override;
        static m3d::Object* CreateObject();

    public:
        static m3d::Class* GetBaseClass();
        virtual m3d::Class* GetClass() const override;
        static m3d::Class m_classBreakableObject;
        virtual ai::BreakableObjectPrototypeInfo const* GetPrototypeInfo() const override;

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
        retruxx::set<int, retruxx::less<int>, retruxx::allocator<int>> m_connectedRopes;

    public:
        virtual void LoadRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode) override;
        virtual void SaveRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const override;
        virtual void SetPosition(CVector const& pos) override;
        virtual void SetRotation(Quaternion const& rot) override;
        virtual void SetPositionSelf(CVector const& pos) override;
        virtual void RenderDebugInfo() const override;
        virtual CVector GetGeometricCenter() const override;
        virtual bool CanChildBeAdded(m3d::Class* pClass) const override;
        virtual void AddChild(ai::Obj* pObj) override;
        virtual bool RemoveChild(ai::Obj* pChild) override;
        void DoTouch();
        void SetState(ai::BreakableObject::STATES state);
        void SetEnabled();
        void SetDestroyed();
        void SetJointAnchor(CVector const& vec);
        ai::BlastWave* CreateBlastWave() const;
        int IsDestroyable();
        float GetCriticalHitEnergy();
        unsigned short GetEffectType();
        unsigned short GetDestroyEffectType();
        ai::BreakableObject::STATES GetState();
        ai::BoxyBody const* _BoxyBody() const;
        ai::BoxyBody* _BoxyBody();
        CVector GetBounds();
        void SetPositioningOnGround(bool);
        CStr const& GetBrokenModelName() const;
        CStr const& GetDestroyedModelName() const;
        bool IsExplosive() const;
        void ConnectWithRope(int ropeId);
        bool DisconnectFromRope(int ropeId);
        retruxx::set<int, retruxx::less<int>, retruxx::allocator<int>> const& GetConnectedRopes();
        void SetRemovingEffectName(CStr const& name);
        void SetCausePos(CVector const& pos);
        void SetCauseForce(float force);
        void SetInitVelocities(bool initVelocities);

    protected:
        virtual void _Construct() override;
        void _BreakOffConnectedRopes(bool createJoint);
        void _StifleChildLights();
    };
}  // namespace ai
