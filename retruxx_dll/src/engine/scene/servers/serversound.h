#pragma once

namespace Sound3DServer
{
    namespace m3d
    {
        class SoundItem
        {
        public:
            SoundItem();
        protected:
        private:
            SoundTypeEnum type;
            int soundIds[3];
        };
    }
}
