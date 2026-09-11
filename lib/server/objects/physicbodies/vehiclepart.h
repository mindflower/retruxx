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
        virtual  ~VehiclePartPrototypeInfo() override /* 0x00 */;
        /* 0x0068 */ int m_weaponPrototypeId;
        /* 0x006c */ float m_durabilityCoeffsForDamageTypes[4];
        /* 0x007c */ float m_durability;
        /* 0x0080 */ retruxx::set<CStr, retruxx::less<CStr>, retruxx::allocator<CStr> > m_loadPoints;
        /* 0x008c */ CStr m_blowEffectName;
        /* 0x0098 */ bool m_canBeUsedInAutogenerating;
        /* 0x0099 */ char Padding_127[3];
        /* 0x009c */ float m_repairCoef;
        /* 0x00a0 */ retruxx::vector<ai::TriMesh*, retruxx::allocator<ai::TriMesh*> > m_modelMeshes;
        /* 0x00b0 */ retruxx::vector<ai::Box*, retruxx::allocator<ai::Box*> > m_boundsForMeshes;
        /* 0x00c0 */ retruxx::vector<void*, retruxx::allocator<void*> > m_verts;
        /* 0x00d0 */ retruxx::vector<int*, retruxx::allocator<int*> > m_inds;
        /* 0x00e0 */ retruxx::vector<int, retruxx::allocator<int> > m_numsTris;
        /* 0x00f0 */ retruxx::vector<int, retruxx::allocator<int> > m_vertsStride;
        /* 0x0100 */ retruxx::vector<float, retruxx::allocator<float> > m_groupHealthes;
        virtual bool LoadFromXML(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override /* 0x04 */;
        virtual void RefreshFromXml(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override /* 0x0c */;
        virtual ai::Obj* CreateTargetObject() const override /* 0x00 */;
        const CVector& GetSize() const;

    private:
        void _InitModelMeshes(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode);
    }; /* size: 0x0110 */

    static_assert(sizeof(VehiclePartPrototypeInfo) == 0x0110);

    class VehiclePart : public ai::PhysicBody
    {
        friend class VehiclePartPrototypeInfo;
        // Vehicle::InflictDamage builds a BreakData directly, as the shipped
        // code does.
        friend class Vehicle;
    public:
        using AfterChangeFloatCallback = ai::MemberFunctionOneArg<ai::VehiclePart, float, void>;
        using BeforeApplyModifierFloatCallback = ai::MemberFunctionTwoArgsRef<ai::VehiclePart, ai::Modifier, float, bool>;

    protected:
        virtual  ~VehiclePart() override /* 0x00 */;

    protected:
        VehiclePart(const ai::VehiclePartPrototypeInfo& prototypeInfo);
        virtual m3d::Object* Clone() override /* 0x00 */;
        static m3d::Object* __fastcall CreateObject();

    public:
        static m3d::Class* __fastcall GetBaseClass();
        virtual m3d::Class* GetClass() const override /* 0x00 */;
        static m3d::Class m_classVehiclePart;
        virtual const ai::VehiclePartPrototypeInfo* GetPrototypeInfo() const override /* 0x00 */;

    protected:
        static void __fastcall RegisterProperty(const char* Name, int id, ai::eGObjPropertySaveStatus saveStatus);

    public:
        virtual ai::eGObjPropertySaveStatus GetPropertySaveStatus(int id) const override /* 0x00 */;
        virtual void GetPropertiesNames(retruxx::set<CStr, retruxx::less<CStr>, retruxx::allocator<CStr> >& Props) const override /* 0x00 */;
        virtual void GetPropertiesIDs(retruxx::set<int, retruxx::less<int>, retruxx::allocator<int> >& Props) const override /* 0x00 */;
        virtual CStr GetPropertyName(int id) const override /* 0x00 */;
        virtual bool SetPropertyById(int propertyId, const m3d::AIParam& newValue) override /* 0x00 */;
        virtual int GetPropertyId(const char* PropertyName) const override /* 0x00 */;

    protected:
        static inline retruxx::map<CStr, int, ai::Obj::LessNoCaseCStr, retruxx::allocator<retruxx::pair<CStr const, int> > > m_propertiesMap;
        static inline retruxx::map<int, enum ai::eGObjPropertySaveStatus, retruxx::less<int>, retruxx::allocator<retruxx::pair<int const, enum ai::eGObjPropertySaveStatus> > > m_propertiesSaveStatesMap;
        virtual bool _GetPropertyDefaultInternal(int propertyId, m3d::AIParam& retVal) const override /* 0x00 */;
        virtual bool _GetPropertyInternal(int propertyId, m3d::AIParam& retVal) const override /* 0x00 */;

        struct BreakModelData
        {
            /* 0x0000 */ m3d::AnimatedModel* mdl;
            /* 0x0004 */ m3d::Configuration* cfg;
            /* 0x0008 */ int meshId;
            /* 0x000c */ int groupId;
            /* 0x0010 */ CVector pos;
            /* 0x001c */ CVector dir;
            /* 0x0028 */ CVector normal;
            BreakModelData(const ai::VehiclePart::BreakModelData&);
            BreakModelData();
        }; /* size: 0x0034 */

        struct DecalsPassageInfo
        {
            /* 0x0000 */ std::vector<CVector, std::allocator<CVector> > poses;
            /* 0x0010 */ std::vector<CVector, std::allocator<CVector> > normals;
            /* 0x0020 */ std::vector<CVector, std::allocator<CVector> > tangents;
            /* 0x0030 */ std::vector<int, std::allocator<int> > meshNums;
            /* 0x0040 */ int id;
            /* 0x0044 */ CStr name;
        }; /* size: 0x0050 */

        struct JadedEffectsPassageInfo
        {
            /* 0x0000 */ CVector pos;
            /* 0x000c */ CStr name;
            /* 0x0018 */ int num;
        }; /* size: 0x001c */

        struct PassToAnotherMapData
        {
            /* 0x0000 */ std::vector<ai::VehiclePart::JadedEffectsPassageInfo, std::allocator<ai::VehiclePart::JadedEffectsPassageInfo> > jadedEffects;
            /* 0x0010 */ std::vector<ai::VehiclePart::DecalsPassageInfo, std::allocator<ai::VehiclePart::DecalsPassageInfo> > decals;
        }; /* size: 0x0020 */

        struct LoadDecalData
        {
            /* 0x0000 */ m3d::SgNode* node;
            /* 0x0004 */ m3d::DecalData dd;
            /* 0x0044 */ int meshNum;
        }; /* size: 0x0048 */

        struct ModelPart
        {
            /* 0x0000 */ float maxHealth;
            /* 0x0004 */ float health;
            /* 0x0008 */ m3d::SgNode* jadedEffect;
            ModelPart();
        }; /* size: 0x000c */

        struct BreakData
        {
            /* 0x0000 */ CVector point;
            /* 0x000c */ CVector dir;
            /* 0x0018 */ CVector normal;
            /* 0x0024 */ float damage;
            /* 0x0028 */ int decalId;
            BreakData(const ai::VehiclePart::BreakData&);
            BreakData();
        }; /* size: 0x002c */

    public:
        /* 0x0158 */ m3d::SgNode* m_SplashEffect;
        /* 0x015c */ bool m_MakeSplash;
        /* 0x015d */ char Padding_34[3];
        /* 0x0160 */ retruxx::map<int, m3d::SgNode*, retruxx::less<int>, retruxx::allocator<retruxx::pair<int const, m3d::SgNode*> > > m_decals;
        virtual void LoadRuntimeValues(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode) override /* 0x00 */;
        virtual void SaveRuntimeValues(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const override /* 0x00 */;
        virtual void Update(float elapsedTime, unsigned int workTime) override /* 0x00 */;
        virtual void Remove() override /* 0x00 */;
        virtual bool ApplyModifier(const ai::Modifier& modifier) override /* 0x00 */;
        virtual void SetPassedToAnotherMapStatus() override /* 0x00 */;
        const CStr& GetPartName() const;
        void SetPartName(const CStr& newName);
        virtual void TransferPhysicParamsToSceneGraphNode() override /* 0x00 */;
        CVector GetSize() const;
        const CStr& GetBlowEffectName() const;
        void BreakModel(const ai::VehiclePart::BreakData& breakData);
        void DefineSuppressedLPs();
        virtual void RenderDebugInfo() const override /* 0x00 */;
        const ai::NumericInRangeRegenerating<float>& Durability() const;
        ai::NumericInRangeRegenerating<float>& Durability();
        float GetDurabilityCoeffForDamageType(ai::DamageType damageType) const;
        virtual unsigned int GetPrice(const ai::IPriceCoeffProvider* priceCoeffProvider) const override /* 0x00 */;
        unsigned int GetRepairPrice() const;
        float GetRepairPriceForOneUnit() const;
        const CVector& GetLastHitPos() const;
        const ai::CompoundVehiclePart* GetOwnerCompoundVehiclePart() const;
        ai::CompoundVehiclePart* GetOwnerCompoundVehiclePart();
        void SetOwnerCompoundVehiclePart(ai::CompoundVehiclePart* compoundVehiclePart);

    protected:
        /* 0x016c */ ai::Numeric<float> m_price;
        virtual void _SetAllPropertiesToMax() override /* 0x00 */;
        virtual void _InternalCreateVisualPart() override /* 0x00 */;
        float _GetModelPartHealth(int groupId) const;
        virtual bool _OnDurabilityValueBeforeApplyModifier(const ai::Modifier& modifier, float& newDurabilityValue) /* 0x160 */;
        virtual void _OnDurabilityValueAfterChange(float oldDurabilityValue) /* 0x164 */;

    private:
        /* 0x0188 */ ai::NumericInRangeRegenerating<float> m_durability;
        /* 0x0260 */ CStr m_partName;
        /* 0x026c */ float m_durabilityCoeffsForDamageTypes[4];
        /* 0x027c */ CStr m_blowEffectName;
        /* 0x0288 */ retruxx::set<int, retruxx::less<int>, retruxx::allocator<int> > m_suppressedLPs;
        /* 0x0294 */ retruxx::vector<ai::VehiclePart::ModelPart, retruxx::allocator<ai::VehiclePart::ModelPart> > m_modelParts;
        /* 0x02a4 */ CVector m_lastHitPos;
        /* 0x02b0 */ ai::VehiclePart::PassToAnotherMapData* m_passToAnotherMapData;
        /* 0x02b4 */ retruxx::vector<ai::VehiclePart::LoadDecalData, retruxx::allocator<ai::VehiclePart::LoadDecalData> > m_loadDecalsData;
        /* 0x02c4 */ ai::CompoundVehiclePart* m_ownerCompoundPart;
        void _CalcMeshToBreak(ai::VehiclePart::BreakModelData& modelData);
        void _AddDecal(const CVector& pos, const CVector& normal, const CVector& tangent, unsigned int meshId, int decalId);
        void _RecalcDecals(unsigned int oldMeshId, unsigned int newMeshId);
        void SaveDecalsRuntime(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const;
        void LoadDecalsRuntime(m3d::cmn::XmlFile* xmlFile, const m3d::cmn::XmlNode* xmlNode);

    public:
        static void __fastcall Registration();
        virtual void DumpPhysicInfo(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* xmlNode) const override /* 0x00 */;
    }; /* size: 0x02c8 */

    static_assert(sizeof(VehiclePart) == 0x02c8);
}

