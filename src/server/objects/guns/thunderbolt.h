#pragma once
#include <math/vector.h>
#include <server/components/numericinrangeregenerating.h>
#include <server/objects/base/obj.h>
#include <server/objects/base/prototypeinfo.h>

class Segment;

namespace ai
{
    class ThunderboltPrototypeInfo : public PrototypeInfo
    {
    public:
        virtual void PostLoad();
        virtual bool LoadFromXML(m3d::cmn::XmlFile*, m3d::cmn::XmlNode const*);
        ThunderboltPrototypeInfo();
        virtual Obj* CreateTargetObject() const;

    private:
        float m_flyTime;
        float m_Damage;
        float m_AverageSegmentLength;
        CStr m_EffectName;

    };

    class Thunderbolt : public Obj
    {
    public:
        class ThunderboltPoint
        {
        public:
            int getObjId() const;
            CVector const& getThunderboltPoint();
            ThunderboltPoint(CVector const&);
            ThunderboltPoint(ThunderboltPoint const&);
            ThunderboltPoint();
            void setObjid(int, bool);
            CVector const& getPoint() const;
            void setDependencies(std::vector<ThunderboltPoint, std::allocator<ThunderboltPoint> > const&);
            void setPoint(CVector const&);

        private:
            CVector m_Point;
            int m_ObjId;
            bool m_launcher;
            bool m_dependant;
            std::vector<ThunderboltPoint> m_masterPoints;
        };


        class ThunderboltSegment
        {
        public:
            ~ThunderboltSegment();
            int getStartObjId() const;
            int getFinishObjId() const;
            std::vector<CVector, std::allocator<CVector> > const& getThunderboltPoints();
            ThunderboltSegment(ThunderboltPrototypeInfo const*, ThunderboltPoint const&, ThunderboltPoint const&);
            void Render(ThunderboltPrototypeInfo const*);
            CVector const& getFinishPoint();
            CVector const& getStartPoint();
        protected:
        private:
            std::vector<CVector, std::allocator<CVector> > const& SegmentFinding(Segment const&) const;
            ThunderboltPoint m_Start;
            ThunderboltPoint m_Finish;
            unsigned int m_nSteps;
            bool m_EffectCreated;
            m3d::SgNode* m_EffectNode;
        };

    public:
        void setTargets(std::vector<int, std::allocator<int> > const&);
        void setTargets(std::vector<CVector, std::allocator<CVector> > const&);
        int getSourceId() const;
        Thunderbolt(ThunderboltPrototypeInfo const&);
        static m3d::Class* GetBaseClass();
        std::vector<int, std::allocator<int> > getTargets() const;
        void setSourceId(int, bool);
        virtual void Remove();
        virtual void Update(float, unsigned int);
        virtual m3d::Class* GetClass() const;
        virtual bool CanLookAtTarget() const;
        virtual bool isLookAtPoint(CVector const&, float) const;
        void setOrigin(CVector const&);
        virtual ThunderboltPrototypeInfo const* GetPrototypeInfo() const;
        CVector const& getOrigin() const;
        virtual void RenderDebugInfo() const;

    protected:
        virtual ~Thunderbolt();

    private:
        //getThunderboltSegmentsInt(ThunderboltPoint&, std::vector<ThunderboltPoint>&);
        void calcThunderboltSegments();
        virtual m3d::Object* Clone();
        void Render();
        static m3d::Object* CreateObject();

    private:
        NumericInRangeRegenerating<float> m_lifeTime;
        ThunderboltPoint m_Origin;
        std::vector<ThunderboltPoint> m_Targets;
        std::vector<ThunderboltSegment> m_Segments;
    };
}
