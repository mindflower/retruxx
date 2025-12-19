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
        /* 0x0080 */ int m_destroyable;
        /* 0x0084 */ float m_criticalHitEnergy;
        /* 0x0088 */ CStr m_effectType;
        /* 0x0094 */ CStr m_destroyEffectType;
        /* 0x00a0 */ CStr m_brokenModelName;
        /* 0x00ac */ CStr m_destroyedModelName;
        /* 0x00b8 */ CStr m_breakEffect;
        /* 0x00c4 */ int m_BlastWavePrototypeId;
        /* 0x00c8 */ CStr m_BlastWavePrototypeName;
        BreakableObjectPrototypeInfo();
        virtual bool LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override /* 0x04 */;
        virtual void PostLoad() override /* 0x00 */;
        virtual void RefreshFromXml(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override /* 0x00 */;
        virtual ai::Obj* CreateTargetObject() const override /* 0x00 */;
    }; /* size: 0x00d4 */

    static_assert(sizeof(BreakableObjectPrototypeInfo) == 0x00d4);

    class BreakableObject : public ai::SimplePhysicObj
    {
        friend class BreakableObjectPrototypeInfo;
    protected:
        virtual  ~BreakableObject() override /* 0x00 */;

    private:
        BreakableObject(const ai::BreakableObjectPrototypeInfo& prototypeInfo);
        BreakableObject(const ai::BreakableObject&);
        virtual m3d::Object* Clone() override /* 0x00 */;
        static m3d::Object* __fastcall CreateObject();

    public:
        static m3d::Class* __fastcall GetBaseClass();
        virtual m3d::Class* GetClass() const override /* 0x00 */;
        static m3d::Class m_classBreakableObject;
        virtual const ai::BreakableObjectPrototypeInfo* GetPrototypeInfo() const override /* 0x00 */;

        enum STATES
        {
            DISABLED = 0,
            ENABLED = 1,
            REMOVED = 2,
        };

    private:
        /* 0x0144 */ int m_destroyable;
        /* 0x0148 */ float m_criticalHitEnergy;
        /* 0x014c */ ai::BreakableObject::STATES m_state;
        /* 0x0150 */ unsigned short m_effectType;
        /* 0x0152 */ unsigned short m_destroyEffectType;
        /* 0x0154 */ dxJoint* m_jointId;
        /* 0x0158 */ CStr m_removingEffectName;
        /* 0x0164 */ CVector m_causePos;
        /* 0x0170 */ float m_causeForce;
        /* 0x0174 */ bool m_initVelocities;
        /* 0x0175 */ bool m_bPositioningOnGround;
        /* 0x0176 */ char Padding_329[2];
        /* 0x0178 */ retruxx::set<int, retruxx::less<int>, retruxx::allocator<int> > m_connectedRopes;

    public:
        virtual void LoadRuntimeValues(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override /* 0x00 */;
        virtual void SaveRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const override /* 0x00 */;
        virtual void SetPosition(const CVector& pos) override /* 0x00 */;
        virtual void SetRotation(const Quaternion& rot) override /* 0x00 */;
        virtual void SetPositionSelf(const CVector& pos) override /* 0x00 */;
        virtual void RenderDebugInfo() const override /* 0x00 */;
        virtual CVector GetGeometricCenter() const override /* 0x00 */;
        virtual bool CanChildBeAdded(m3d::Class* pClass) const override /* 0x00 */;
        virtual void AddChild(ai::Obj* pObj) override /* 0x00 */;
        virtual bool RemoveChild(ai::Obj* pChild) override /* 0x00 */;
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
        virtual void _Construct() override /* 0x1b0 */;
        void _BreakOffConnectedRopes(bool createJoint);
        void _StifleChildLights();
    }; /* size: 0x0184 */

    static_assert(sizeof(BreakableObject) == 0x0184);
}
