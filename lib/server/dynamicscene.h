#pragma once
#include <core/clazz.h>
#include <deque>
#include <vector>
#include <engine/ode/sources/objects.h>

struct dContact;

namespace ai
{
    class Vehicle;
    class Bullet;
    class PhysicBody;
    class Shell;
    class Obj;
    class PhysicObj;

    class TraceLineCallback
    {
    public:
        virtual ~TraceLineCallback();
        virtual bool CollideId(int) const = 0;
        virtual bool CollidePhysicObj(ai::PhysicObj const*) const = 0;
    };

    class DynamicScene : public m3d::Object
    {
    protected:
        DynamicScene();
        DynamicScene(ai::DynamicScene const& __formal);

    public:
        virtual ~DynamicScene() override;
        virtual m3d::Object* Clone() override;
        static m3d::Object* CreateObject();
        static m3d::Class* GetBaseClass();
        virtual m3d::Class* GetClass() const override;
        static m3d::Class m_classDynamicScene;

        struct SoilProps
        {
            short m_splashType;
            CStr m_splashTypeName;
            CStr m_wheelTraceTextureName;
            float m_friction;
            float m_resistance;
            int m_idx;
            SoilProps();
            void LoadFromXml(m3d::cmn::XmlNode const* xmlNode);
        };

    private:
        retruxx::vector<ai::DynamicScene::SoilProps, retruxx::allocator<ai::DynamicScene::SoilProps>> m_soilProps;
        retruxx::vector<
            retruxx::vector<unsigned short, retruxx::allocator<unsigned short>>,
            retruxx::allocator<retruxx::vector<unsigned short, retruxx::allocator<unsigned short>>>>
            m_soilPropsIdx;
        retruxx::vector<CStr, retruxx::allocator<CStr>> m_wheelTypeNames;
        retruxx::vector<
            retruxx::vector<CStr, retruxx::allocator<CStr>>,
            retruxx::allocator<retruxx::vector<CStr, retruxx::allocator<CStr>>>>
            m_soilEffectNames;
        retruxx::vector<CStr, retruxx::allocator<CStr>> m_roadEffectNames;
        retruxx::vector<CStr, retruxx::allocator<CStr>> m_soilSplashTypeNames;
        retruxx::vector<CStr, retruxx::allocator<CStr>> m_shellTypesNames;
        retruxx::vector<
            retruxx::vector<CStr, retruxx::allocator<CStr>>,
            retruxx::allocator<retruxx::vector<CStr, retruxx::allocator<CStr>>>>
            m_shellsEffectsNames;
        retruxx::vector<CStr, retruxx::allocator<CStr>> m_shellWaterEffectNames;
        retruxx::vector<CStr, retruxx::allocator<CStr>> m_shellsStaticsEffNames;
        retruxx::vector<CStr, retruxx::allocator<CStr>> m_shellsVehiclesEffNames;
        retruxx::vector<CStr, retruxx::allocator<CStr>> m_shellsRoadEffNames;
        retruxx::vector<CStr, retruxx::allocator<CStr>> m_vehicleSoilEffectNames;
        retruxx::vector<CStr, retruxx::allocator<CStr>> m_BoEffectTypeNames;
        retruxx::vector<CStr, retruxx::allocator<CStr>> m_BoVehicleEffectNames;
        retruxx::vector<
            retruxx::vector<CStr, retruxx::allocator<CStr>>,
            retruxx::allocator<retruxx::vector<CStr, retruxx::allocator<CStr>>>>
            m_BoShellEffectNames;
        retruxx::vector<CStr, retruxx::allocator<CStr>> m_decalsNames;
        int m_clashDecalId;
        float m_physicTimeAccumulator;

    public:
        static void Clear();
        static void InitOnce();
        static void ClearOnce();
        static int ProcessShellAndBody(
            ai::Shell* shell,
            ai::PhysicBody* body,
            dContact* contact,
            unsigned int& numContacts,
            bool reverse);
        static void CollideBullet(ai::Bullet const& bullet);
        void DeleteAll();
        void PurgeBodies();
        ai::Vehicle* GetVehicleControlledByPlayer() const;
        bool LoadSceneFromXml(
            m3d::cmn::XmlFile* xmlFile,
            m3d::cmn::XmlNode const* rootNode,
            retruxx::vector<m3d::Class*, retruxx::allocator<m3d::Class*>> const& allowedClasses);
        bool SaveSceneToXml(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode* sceneNode);
        bool LoadSceneFromFile(
            char const* fileName,
            retruxx::vector<m3d::Class*, retruxx::allocator<m3d::Class*>> const& allowedClasses);
        bool SaveSceneToFile(char const* fileName);
        int ReadNewObjectFromXml(
            m3d::cmn::XmlFile* xmlFile,
            m3d::cmn::XmlNode const* xmlNode,
            retruxx::vector<m3d::Class*, retruxx::allocator<m3d::Class*>> const& allowedClasses);
        void ReadSoilProps(char const* fileName);
        ai::DynamicScene::SoilProps const& GetSoilProps(unsigned int x, unsigned int z) const;
        unsigned int GetWheelTypeByName(CStr const& wheelTypeName);
        CStr const& GetSoilEffectName(unsigned int wheelType, unsigned short soilType, bool bVehicleIsBraking) const;
        CStr const& GetRoadEffectName(unsigned int wheelType, bool bVehicleIsBraking) const;
        short GetExplosionType(CStr const& shellTypeName);
        CStr const& GetShellEffectName(unsigned short shellType, unsigned short soilType) const;
        CStr const& GetShellWaterEffectName(unsigned short shellType) const;
        CStr const& GetShellRoadEffectName(unsigned short shellType) const;
        CStr const& GetShellStaticsEffectName(unsigned short shellType) const;
        CStr const& GetShellVehicleEffectName(unsigned short shellType) const;
        CStr const& GetVehicleSoilEffectName(unsigned short splashType) const;
        short GetBoEffectTypeByName(CStr const& BreakableObjectEffectName);
        CStr const& GetBoEffectTypeName(unsigned short BoEffectType);
        void CreateBoShellEffectNames();
        CStr const& GetBoShellEffectName(unsigned short BoEffectType, unsigned short ShellEffectType);
        CStr const& GetBoVehicleEffectName(unsigned short BoEffectType) const;
        int AddDecalName(CStr const& name);
        CStr const& GetDecalName(int id);
        void InitClashDecalId();
        int GetClashDecalId();
        void LinkNodesFromBodyToSceneGraph(ai::Obj* pObj);
        void CollideScene(float elapsedTime);
        void StepScene(float elapsedTime);
        void UpdateSceneItems(float elapsedTime);
        void RenderDebugInfo();
        int GetNumNearCallbacksLastFrame();

    private:
        retruxx::deque<int, retruxx::allocator<int>> m_timefilterValues;
        void _InitWheelTraces();
        void _RecalcWheelEffectNames();
        void _AddSoilEffectNameForWheelTypeName(CStr const& wheelTypeName);
    };

    // ODE's broadphase hands every candidate pair here; defined in dynamicscene.cpp.
    void NearCallback(void* data, dxGeom* geom1, dxGeom* geom2);

    inline dxSpace* gGlobalSpace = nullptr;
    inline dxSpace* gTempSpace = nullptr;
    inline dxSpace* gBulletSpace = nullptr;
    inline dxSpace* gIntersectionSpace = nullptr;
    inline dxSpace* gSpaceForAllPhysicParticles = nullptr;
    inline dxWorld* gGlobalWorld = nullptr;
    inline DynamicScene* gDynamicScene = nullptr;

    class ObjIdExceptionalTraceLineCallback : public ai::TraceLineCallback
    {
    private:
        std::vector<int, std::allocator<int>> m_Exceptions;

    public:
        ObjIdExceptionalTraceLineCallback(ai::ObjIdExceptionalTraceLineCallback const&);
        ObjIdExceptionalTraceLineCallback(std::vector<int, std::allocator<int>> const& Exceptions);
        virtual bool CollideId(int objId) const override;
        virtual bool CollidePhysicObj(ai::PhysicObj const* physicObj) const override;
    };
}  // namespace ai
