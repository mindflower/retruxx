#pragma once
#include <corecrt_io.h>

namespace m3d
{
    namespace fs
    {
        class CFileEnum
        {
        public:
            bool GetNextFile(_finddata_t*);
            CFileEnum();
            virtual ~CFileEnum();
            bool StartEnumeration(char const* const, char const* const, _finddata_t*);
            bool StartEnumeration(char const* const, _finddata_t*);

        private:
            intptr_t m_hEnumFile = 0;
        };
    }
}
