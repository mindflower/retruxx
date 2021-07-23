#pragma once
#include "i_stream.h"

namespace m3d
{
    namespace fs
    {
        class FileStream : public IStream
        {
        public:
            virtual ~FileStream();
            FileStream();

        private:
            int m_lastError;
            unsigned int m_FileSize;
        };
    }
}
