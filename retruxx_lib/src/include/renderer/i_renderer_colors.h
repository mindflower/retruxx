#pragma once

namespace m3d
{
    namespace rend
    {
        class Colorf
        {
        public:
            void init(float, float, float, float);
            void init(unsigned int);
            Colorf();
            Colorf(unsigned int);
            Colorf(float, float, float, float);

        private:
            float r;
            float g;
            float b;
            float a;
        };
    }
}
