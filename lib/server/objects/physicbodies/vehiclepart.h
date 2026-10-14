#pragma once
#include "physicbody.h"
#include <geomobject.h>
#include <server/components/numeric.h>
#include <server/components/numericinrangeregenerating.h>

namespace m3d
{
    class Configuration;
}

namespace ai
{
    class CompoundVehiclePart;
    class Box;
    class TriMesh;
    enum DamageType;

    class VehiclePartPrototypeInfo : public ai::PhysicBodyPrototypeInfo
    {
    public:
        VehiclePartPrototypeInfo();
        virtual  ~VehiclePartPrototypeInfo() override;
        int m_weaponPrototypeId;
        float m_durabilityCoeffsForDamageTypes[4];
        float m_durability;
        retruxx::set<CStr, retruxx::less<CStr>, retruxx::allocator<CStr> > m_loadPoints;
        CStr m_blowEffectName;
        bool m_canBeUsedInAutogenerating;
        float m_repairCoef;
        retruxx::vector<ai::TriMesh*, retruxx::allocator<ai::TriMesh*> > m_modelMeshes;
        retruxx::vector<ai::Box*, retruxx::allocator<ai::Box*> > m_boundsForMeshes;
        retruxx::vector<void*, retruxx::allocator<void*> > m_verts;
        retruxx::vector<int*, retruxx::allocator<int*> > m_inds;
        retruxx::vector<int, retruxx::allocator<int> > m_numsTris;
        retruxx::vector<int, retruxx::allocator<int> > m_vertsStride;
        retruxx::vector<float, retruxx::allocator<float> > m_groupHealthes;
        virtual bool LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override;
        virtual void RefreshFromXml(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override;
        virtual ai::Obj* CreateTargetObject() const override;
        const CVector& GetSize() const;

    private:
        void _InitModelMeshes(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode);
    };

    class VehiclePart : public ai::PhysicBody
    {
        friend class VehiclePartPrototypeInfo;
        // Vehicle::InflictDamage builds a BreakData directly, as the shipped
        // code does.
        friend class Vehicle;
        // Breaks the hit part the way Vehicle::InflictDamage does.
        friend class Boss03;
        friend class Boss04Station;
        friend class Boss04Drone;
    public:
        using AfterChangeFloatCallback = ai::MemberFunctionOneArg<ai::VehiclePart, float, void>;
        using BeforeApplyModifierFloatCallback = ai::MemberFunctionTwoArgsRef<ai::VehiclePart, ai::Modifier, float, bool>;

    protected:
        virtual  ~VehiclePart() override;

    protected:
        VehiclePart(const ai::VehiclePartPrototypeInfo& prototypeInfo);
        virtual m3d::Object* Clone() override;
        static m3d::Object* __fastcall CreateObject();

    public:
        static m3d::Class* __fastcall GetBaseClass();
        virtual m3d::Class* GetClass() const override;
        static m3d::Class m_classVehiclePart;
        virtual const ai::VehiclePartPrototypeInfo* GetPrototypeInfo() const override;

    protected:
        static void __fastcall RegisterProperty(const char* Name, int id, ai::eGObjPropertySaveStatus saveStatus);

    public:
        virtual ai::eGObjPropertySaveStatus GetPropertySaveStatus(int id) const override;
        virtual void GetPropertiesNames(retruxx::set<CStr, retruxx::less<CStr>, retruxx::allocator<CStr> >& Props) const override;
        virtual void GetPropertiesIDs(retruxx::set<int, retruxx::less<int>, retruxx::allocator<int> >& Props) const override;
        virtual CStr GetPropertyName(int id) const override;
        virtual bool SetPropertyById(int propertyId, const m3d::AIParam& newValue) override;
        virtual int GetPropertyId(const char* PropertyName) const override;

    protected:
        static inline retruxx::map<CStr, int, ai::Obj::LessNoCaseCStr, retruxx::allocator<retruxx::pair<CStr const, int> > > m_propertiesMap;
        static inline retruxx::map<int, enum ai::eGObjPropertySaveStatus, retruxx::less<int>, retruxx::allocator<retruxx::pair<int const, enum ai::eGObjPropertySaveStatus> > > m_propertiesSaveStatesMap;
        virtual bool _GetPropertyDefaultInternal(int propertyId, m3d::AIParam& retVal) const override;
        virtual bool _GetPropertyInternal(int propertyId, m3d::AIParam& retVal) const override;

        struct BreakModelData
        {
            m3d::AnimatedModel* mdl;
            m3d::Configuration* cfg;
            int meshId;
            int groupId;
            CVector pos;
            CVector dir;
            CVector normal;
            BreakModelData(const ai::VehiclePart::BreakModelData&);
            BreakModelData();
        };

        struct DecalsPassageInfo
        {
            std::vector<CVector, std::allocator<CVector> > poses;
            std::vector<CVector, std::allocator<CVector> > normals;
            std::vector<CVector, std::allocator<CVector> > tangents;
            std::vector<int, std::allocator<int> > meshNums;
            int id;
            CStr name;
        };

        struct JadedEffectsPassageInfo
        {
            CVector pos;
            CStr name;
            int num;
        };

        struct PassToAnotherMapData
        {
            std::vector<ai::VehiclePart::JadedEffectsPassageInfo, std::allocator<ai::VehiclePart::JadedEffectsPassageInfo> > jadedEffects;
            std::vector<ai::VehiclePart::DecalsPassageInfo, std::allocator<ai::VehiclePart::DecalsPassageInfo> > decals;
        };

        struct LoadDecalData
        {
            m3d::SgNode* node;
            m3d::DecalData dd;
            int meshNum;
        };

        struct ModelPart
        {
            float maxHealth;
            float health;
            m3d::SgNode* jadedEffect;
            ModelPart();
        };

        struct BreakData
        {
            CVector point;
            CVector dir;
            CVector normal;
            float damage;
            int decalId;
            BreakData(const ai::VehiclePart::BreakData&);
            BreakData();
        };

    public:
        m3d::SgNode* m_SplashEffect;
        bool m_MakeSplash;
        retruxx::map<int, m3d::SgNode*, retruxx::less<int>, retruxx::allocator<retruxx::pair<int const, m3d::SgNode*> > > m_decals;
        virtual void LoadRuntimeValues(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override;
        virtual void SaveRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const override;
        virtual void Update(float elapsedTime, unsigned int workTime) override;
        virtual void Remove() override;
        virtual bool ApplyModifier(const ai::Modifier& modifier) override;
        virtual void SetPassedToAnotherMapStatus() override;
        const CStr& GetPartName() const;
        void SetPartName(const CStr& newName);
        virtual void TransferPhysicParamsToSceneGraphNode() override;
        CVector GetSize() const;
        const CStr& GetBlowEffectName() const;
        void BreakModel(const ai::VehiclePart::BreakData& breakData);
        void DefineSuppressedLPs();
        virtual void RenderDebugInfo() const override;
        const ai::NumericInRangeRegenerating<float>& Durability() const;
        ai::NumericInRangeRegenerating<float>& Durability();
        float GetDurabilityCoeffForDamageType(ai::DamageType damageType) const;
        virtual unsigned int GetPrice(const ai::IPriceCoeffProvider* priceCoeffProvider) const override;
        unsigned int GetRepairPrice() const;
        float GetRepairPriceForOneUnit() const;
        const CVector& GetLastHitPos() const;
        const ai::CompoundVehiclePart* GetOwnerCompoundVehiclePart() const;
        ai::CompoundVehiclePart* GetOwnerCompoundVehiclePart();
        void SetOwnerCompoundVehiclePart(ai::CompoundVehiclePart* compoundVehiclePart);

    protected:
        ai::Numeric<float> m_price;
        virtual void _SetAllPropertiesToMax() override;
        virtual void _InternalCreateVisualPart() override;
        float _GetModelPartHealth(int groupId) const;
        virtual bool _OnDurabilityValueBeforeApplyModifier(const ai::Modifier& modifier, float& newDurabilityValue);
        virtual void _OnDurabilityValueAfterChange(float oldDurabilityValue);

    private:
        ai::NumericInRangeRegenerating<float> m_durability;
        CStr m_partName;
        float m_durabilityCoeffsForDamageTypes[4];
        CStr m_blowEffectName;
        retruxx::set<int, retruxx::less<int>, retruxx::allocator<int> > m_suppressedLPs;
        retruxx::vector<ai::VehiclePart::ModelPart, retruxx::allocator<ai::VehiclePart::ModelPart> > m_modelParts;
        CVector m_lastHitPos;
        ai::VehiclePart::PassToAnotherMapData* m_passToAnotherMapData;
        retruxx::vector<ai::VehiclePart::LoadDecalData, retruxx::allocator<ai::VehiclePart::LoadDecalData> > m_loadDecalsData;
        ai::CompoundVehiclePart* m_ownerCompoundPart;
        void _CalcMeshToBreak(ai::VehiclePart::BreakModelData& modelData);
        void _AddDecal(const CVector& pos, const CVector& normal, const CVector& tangent, unsigned int meshId, int decalId);
        void _RecalcDecals(unsigned int oldMeshId, unsigned int newMeshId);
        void SaveDecalsRuntime(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const;
        void LoadDecalsRuntime(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode);

    public:
        static void __fastcall Registration();
        virtual void DumpPhysicInfo(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const override;
    };
}

