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

    class VehiclePartPrototypeInfo : public PhysicBodyPrototypeInfo
    {
    public:
        CVector const& GetSize() const;
        VehiclePartPrototypeInfo();
        virtual Obj* CreateTargetObject() const;
        virtual void RefreshFromXml(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);
        virtual ~VehiclePartPrototypeInfo();
        virtual bool LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);

    private:
        void _InitModelMeshes(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);
        int m_weaponPrototypeId;
        float m_durabilityCoeffsForDamageTypes[4];
        float m_durability;
        std::set<CStr> m_loadPoints;
        CStr m_blowEffectName;
        bool m_canBeUsedInAutogenerating;
        float m_repairCoef;
        std::vector<TriMesh*> m_modelMeshes;
        std::vector<Box*> m_boundsForMeshes;
        std::vector<void*> m_verts;
        std::vector<int*> m_inds;
        std::vector<int> m_numsTris;
        std::vector<int> m_vertsStride;
        std::vector<float> m_groupHealthes;
    };

    class VehiclePart :  public PhysicBody
    {
    public:
        class BreakData
        {
        public:
            BreakData();

        private:
            CVector point;
            CVector dir;
            CVector normal;
            float damage;
            int decalId;
        };

        class BreakModelData
        {
        public:
            BreakModelData();
        protected:
        private:
            m3d::AnimatedModel* mdl;
            m3d::Configuration* cfg;
            int meshId;
            int groupId;
            CVector pos;
            CVector dir;
            CVector normal;
        };

        class ModelPart
        {
        public:
            ModelPart();

        private:
            float maxHealth;
            float health;
            m3d::SgNode* jadedEffect;
        };

        class LoadDecalData
        {
        private:
            m3d::SgNode* node;
            m3d::DecalData dd;
            int meshNum;
        };

        class JadedEffectsPassageInfo
        {
        private:
            CVector pos;
            CStr name;
            int num;
        };

        class DecalsPassageInfo
        {
        private:
            std::vector<CVector> poses;
            std::vector<CVector> normals;
            std::vector<CVector> tangents;
            std::vector<int> meshNums;
            int id;
            CStr name;
        };

        class PassToAnotherMapData
        {
        private:
            std::vector<JadedEffectsPassageInfo> jadedEffects;
            std::vector<DecalsPassageInfo> decals;
        };

    public:
        virtual void RenderDebugInfo() const ;
        virtual void LoadRuntimeValues(struct m3d::cmn::XmlFile *,struct m3d::cmn::XmlNode const *);
        CStr const & GetPartName() const ;
        virtual void TransferPhysicParamsToSceneGraphNode();
        CompoundVehiclePart * GetOwnerCompoundVehiclePart();
        CompoundVehiclePart const * GetOwnerCompoundVehiclePart() const ;
        float GetDurabilityCoeffForDamageType(DamageType) const ;
        CStr const & GetBlowEffectName() const ;
        unsigned int GetRepairPrice() const ;
        virtual struct m3d::Class * GetClass() const ;
        struct CVector GetSize() const ;
        virtual CStr GetPropertyName(int) const ;
        virtual void DumpPhysicInfo(struct m3d::cmn::XmlFile *,struct m3d::cmn::XmlNode *) const ;
        virtual class VehiclePartPrototypeInfo const * GetPrototypeInfo() const ;
        void SetPartName(CStr const &);
        virtual bool ApplyModifier(Modifier const &);
        void SetOwnerCompoundVehiclePart(CompoundVehiclePart *);
        void DefineSuppressedLPs();
        NumericInRangeRegenerating<float> const & Durability() const ;
        NumericInRangeRegenerating<float> & Durability();
        virtual void GetPropertiesIDs(class std::set<int,struct std::less<int>,class std::allocator<int> > &) const ;
        virtual void GetPropertiesNames(class std::set<CStr,struct std::less<CStr>,class std::allocator<CStr> > &) const ;
        virtual void Update(float,unsigned int);
        virtual void SaveRuntimeValues(struct m3d::cmn::XmlFile *,struct m3d::cmn::XmlNode *) const ;
        virtual void Remove();
        virtual bool SetPropertyById(int,class m3d::AIParam const &);
        static struct m3d::Class * GetBaseClass();
        float GetRepairPriceForOneUnit() const ;
        virtual unsigned int GetPrice(IPriceCoeffProvider const *) const ;
        struct CVector const & GetLastHitPos() const ;
        virtual void SetPassedToAnotherMapStatus();
        static void __fastcall Registration();
        virtual eGObjPropertySaveStatus GetPropertySaveStatus(int) const ;
        virtual int GetPropertyId(char const *) const ;
        void BreakModel(struct BreakData const &);
        VehiclePart(class VehiclePartPrototypeInfo const &);

    protected:
        virtual void _SetAllPropertiesToMax();
        static void __fastcall RegisterProperty(char const *,int,eGObjPropertySaveStatus);
        virtual bool _OnDurabilityValueBeforeApplyModifier(Modifier const &,float &);
        virtual bool _GetPropertyInternal(int,class m3d::AIParam &) const ;
        float _GetModelPartHealth(int) const ;
        virtual void _OnDurabilityValueAfterChange(float);
        virtual void _InternalCreateVisualPart();
        virtual bool _GetPropertyDefaultInternal(int,class m3d::AIParam &) const ;
        virtual ~VehiclePart();

    protected:
        //MemberFunctionOneArg<VehiclePart,float,void>::MemberFunctionOneArg<VehiclePart,float,void>(VehiclePart &,void (*const)(float));
        static class m3d::Object * CreateObject();
        void SaveDecalsRuntime(struct m3d::cmn::XmlFile *,struct m3d::cmn::XmlNode *) const ;
        void _CalcMeshToBreak(struct BreakModelData &);
        void _AddDecal(struct CVector const &,struct CVector const &,struct CVector const &,unsigned int,int);
        virtual class m3d::Object * Clone();
        //MemberFunctionTwoArgsRef<VehiclePart,Modifier,float,bool>::MemberFunctionTwoArgsRef<VehiclePart,Modifier,float,bool>(VehiclePart &,bool (*const)(Modifier const &,float &));
        void _RecalcDecals(unsigned int,unsigned int);
        void LoadDecalsRuntime(struct m3d::cmn::XmlFile *,struct m3d::cmn::XmlNode const *);

    public:
        RT_CLASS_DECLARE(VehiclePart);

    private:
        m3d::SgNode *m_SplashEffect;
        bool m_MakeSplash;
        std::map<int,m3d::SgNode *> m_decals;
        //Numeric<float> m_price;
        //NumericInRangeRegenerating<float> m_durability;
        CStr m_partName;
        float m_durabilityCoeffsForDamageTypes[4];
        CStr m_blowEffectName;
        std::set<int> m_suppressedLPs;
        std::vector<ModelPart> m_modelParts;
        CVector m_lastHitPos;
        PassToAnotherMapData *m_passToAnotherMapData;
        std::vector<LoadDecalData> m_loadDecalsData;
        CompoundVehiclePart *m_ownerCompoundPart;
    };
}

