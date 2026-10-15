#pragma once
#include <math/vector.h>
#include <server/components/numericinrangeregenerating.h>
#include <server/objects/base/obj.h>
#include <server/objects/base/prototypeinfo.h>

class Segment;

namespace ai
{
    class ThunderboltPrototypeInfo : public ai::PrototypeInfo
    {
    public:
        float m_flyTime;
        float m_Damage;
        float m_AverageSegmentLength;
        CStr m_EffectName;
        ThunderboltPrototypeInfo();
        virtual bool LoadFromXML(m3d::cmn::XmlFile* xmlFile, m3d::cmn::XmlNode const* xmlNode) override;
        virtual void PostLoad() override;
        virtual ai::Obj* CreateTargetObject() const override;
    };

    // A branching lightning bolt from a ThunderboltLauncher to a set of targets. It lives for
    // m_flyTime seconds, damaging every target a branch touches.
    class Thunderbolt : public ai::Obj
    {
        friend class ThunderboltPrototypeInfo;

    protected:
        virtual ~Thunderbolt() override;

    private:
        Thunderbolt(ai::ThunderboltPrototypeInfo const& prototypeInfo);
        virtual m3d::Object* Clone() override;
        static m3d::Object* CreateObject();

    public:
        static m3d::Class* GetBaseClass();
        virtual m3d::Class* GetClass() const override;
        static m3d::Class m_classThunderbolt;
        virtual ai::ThunderboltPrototypeInfo const* GetPrototypeInfo() const override;

        using Points = retruxx::vector<CVector, retruxx::allocator<CVector>>;
        using Segments = retruxx::vector<Segment, retruxx::allocator<Segment>>;

        class ThunderboltPoint
        {
        private:
            CVector m_Point;
            int m_ObjId;
            bool m_launcher;
            bool m_dependant;
            retruxx::vector<ai::Thunderbolt::ThunderboltPoint, retruxx::allocator<ai::Thunderbolt::ThunderboltPoint>>
                m_masterPoints;

        public:
            ThunderboltPoint(ai::Thunderbolt::ThunderboltPoint const& __that);
            ThunderboltPoint(CVector const& point);
            ThunderboltPoint(int const& objId, bool launcher);
            ThunderboltPoint();
            int getObjId() const;
            void setObjid(int objId, bool launcher);
            CVector const& getPoint() const;
            void setPoint(CVector const& point);
            void setDependencies(
                retruxx::vector<
                    ai::Thunderbolt::ThunderboltPoint,
                    retruxx::allocator<ai::Thunderbolt::ThunderboltPoint>> const& dependencies);
            CVector const& getThunderboltPoint();
        };

        using ThunderboltPoints =
            retruxx::vector<ai::Thunderbolt::ThunderboltPoint, retruxx::allocator<ai::Thunderbolt::ThunderboltPoint>>;

        class ThunderboltSegment
        {
        private:
            ai::Thunderbolt::ThunderboltPoint m_Start;
            ai::Thunderbolt::ThunderboltPoint m_Finish;
            unsigned int m_nSteps;
            bool m_EffectCreated;
            m3d::SgNode* m_EffectNode;
            retruxx::vector<CVector, retruxx::allocator<CVector>> const& SegmentFinding(Segment const& seg) const;

        public:
            ThunderboltSegment(ai::Thunderbolt::ThunderboltSegment const& __that);
            ThunderboltSegment(
                ai::ThunderboltPrototypeInfo const* thunderboltPrototype,
                ai::Thunderbolt::ThunderboltPoint const& Start,
                ai::Thunderbolt::ThunderboltPoint const& Finish);
            ~ThunderboltSegment();
            retruxx::vector<CVector, retruxx::allocator<CVector>> const& getThunderboltPoints();
            void Render(ai::ThunderboltPrototypeInfo const* thunderboltPrototype);
            CVector const& getStartPoint();
            CVector const& getFinishPoint();
            int getStartObjId() const;
            int getFinishObjId() const;
        };

        using ThunderboltSegments = retruxx::
            vector<ai::Thunderbolt::ThunderboltSegment, retruxx::allocator<ai::Thunderbolt::ThunderboltSegment>>;

    public:
        virtual void Update(float elapsedTime, unsigned int workTime) override;
        virtual void Remove() override;
        virtual void RenderDebugInfo() const override;
        void setTargets(retruxx::vector<CVector, retruxx::allocator<CVector>> const& targets);
        void setTargets(retruxx::vector<int, retruxx::allocator<int>> const& targets);
        retruxx::vector<int, retruxx::allocator<int>> getTargets() const;
        void setOrigin(CVector const& Origin);
        CVector const& getOrigin() const;
        void setSourceId(int S, bool launcher);
        int getSourceId() const;
        virtual bool isLookAtPoint(CVector const& lookAt, float eps) const;
        virtual bool CanLookAtTarget() const;

    private:
        ai::NumericInRangeRegenerating<float> m_lifeTime;
        ai::Thunderbolt::ThunderboltPoint m_Origin;
        retruxx::vector<ai::Thunderbolt::ThunderboltPoint, retruxx::allocator<ai::Thunderbolt::ThunderboltPoint>>
            m_Targets;
        retruxx::vector<ai::Thunderbolt::ThunderboltSegment, retruxx::allocator<ai::Thunderbolt::ThunderboltSegment>>
            m_Segments;
        retruxx::vector<ai::Thunderbolt::ThunderboltSegment, retruxx::allocator<ai::Thunderbolt::ThunderboltSegment>>
            getThunderboltSegmentsInt(
                ai::Thunderbolt::ThunderboltPoint& org,
                retruxx::vector<
                    ai::Thunderbolt::ThunderboltPoint,
                    retruxx::allocator<ai::Thunderbolt::ThunderboltPoint>>& targets);
        void calcThunderboltSegments();
        void Render();
    };
}  // namespace ai
