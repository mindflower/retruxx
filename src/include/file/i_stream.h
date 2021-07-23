#pragma once

namespace m3d
{
    namespace fs
    {
        class IStream
        {
        public:
            enum OpenFlags
            {
                OPEN_WRITE = 0x0,
                OPEN_READ = 0x1,
            };

        public:
            IStream();
            virtual ~IStream();

        private:
            //m3d::fs::IStream_vtbl* __vftable /*VFT*/;
        };
    }
}
