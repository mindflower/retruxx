#pragma once
#include "base/simplephysicobj.h"

struct dxJoint;

namespace ai
{
    class BlastWave;
    class BoxyBody;

    class BreakableObjectPrototypeInfo : public SimplePhysicObjPrototypeInfo
    {
    public:
        virtual Obj* CreateTargetObject() const;
        BreakableObjectPrototypeInfo();
        virtual void RefreshFromXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);
        virtual void PostLoad();
        virtual bool LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);

    private:
        int m_destroyable;
        float m_criticalHitEnergy;
        CStr m_effectType;
        CStr m_destroyEffectType;
        CStr m_brokenModelName;
        CStr m_destroyedModelName;
        CStr m_breakEffect;
        int m_BlastWavePrototypeId;
        CStr m_BlastWavePrototypeName;
    };

    class BreakableObject :  public SimplePhysicObj
    {
    public:
        enum STATES
        {
            DISABLED = 0x0,
            ENABLED = 0x1,
            REMOVED = 0x2,
        };

    public:
        BreakableObject(BreakableObjectPrototypeInfo const &);
        virtual bool CanChildBeAdded(m3d::Class *) const ;
        int IsDestroyable();
        virtual void RenderDebugInfo() const ;
        void SetRemovingEffectName(CStr const &);
        virtual void LoadRuntimeValues(m3d::cmn::XmlFile *,m3d::cmn::XmlNode const *);
        void SetCauseForce(float);
        BoxyBody * _BoxyBody();
        void SetEnabled();
        virtual bool RemoveChild(Obj *);
        virtual CVector GetGeometricCenter() const ;
        bool DisconnectFromRope(int);
        virtual BreakableObjectPrototypeInfo const * GetPrototypeInfo() const ;
        void SetCausePos(CVector const &);
        float GetCriticalHitEnergy();
        virtual void SetPosition(CVector const &);
        void DoTouch();
        virtual m3d::Class * GetClass() const ;
        enum STATES GetState();
        unsigned short GetDestroyEffectType();
        void SetJointAnchor(CVector const &);
        BlastWave * CreateBlastWave() const ;
        CVector GetBounds();
        static m3d::Class * GetBaseClass();
        unsigned short GetEffectType();
        virtual void AddChild(Obj *);
        virtual void SaveRuntimeValues(m3d::cmn::XmlFile *,m3d::cmn::XmlNode *) const ;
        virtual void SetPositionSelf(CVector const &);
        void SetState(enum STATES);
        void SetInitVelocities(bool);
        void ConnectWithRope(int);
        bool IsExplosive() const ;
        void SetDestroyed();
        virtual void SetRotation(Quaternion const &);

    protected:
        void _BreakOffConnectedRopes(bool);
        void _StifleChildLights();
        virtual void _Construct();
        virtual ~BreakableObject();

    private:
        static m3d::Object * CreateObject();
        virtual m3d::Object * Clone();

    public:
        RT_CLASS_INLINE_DECLARE(BreakableObject);

    private:
        int m_destroyable;
        float m_criticalHitEnergy;
        STATES m_state;
        unsigned __int16 m_effectType;
        unsigned __int16 m_destroyEffectType;
        dxJoint *m_jointId;
        CStr m_removingEffectName;
        CVector m_causePos;
        float m_causeForce;
        bool m_initVelocities;
        bool m_bPositioningOnGround;
        std::set<int> m_connectedRopes;
    };
}
