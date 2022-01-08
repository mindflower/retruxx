#pragma once
#include <vector>
#include <core/clazz.h>
#include <math/point2d.h>

namespace ai
{
    class Static2DPath :  public m3d::Object
    {
    public:
        void Remove(int);
        static m3d::Object * CreateObject();
        static m3d::Class * GetBaseClass();
        virtual m3d::Object * Clone();
        void Push(float,float);
        int GetCount() const ;
        virtual m3d::Class * GetClass() const ;
        float GetX(int) const ;
        float GetY(int) const ;
        virtual ~Static2DPath();
        std::vector<CVector,std::allocator<CVector> > asVector();

    protected:
        Static2DPath();
        Static2DPath(Static2DPath const &);

    private:
        std::vector<PointBase<float>> m_Points;
    };
}
