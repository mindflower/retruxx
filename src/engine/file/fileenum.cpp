#include <stdexcept>
#include <file/fileenum.h>

#include "core/stringm3d.h"

namespace m3d
{
	namespace fs
	{
		bool CFileEnum::GetNextFile(_finddata_t* pFile)
		{
			if (_findnext(this->m_hEnumFile, pFile) != -1)
				return 1;
			_findclose(this->m_hEnumFile);
			this->m_hEnumFile = 0;
			return 0;
		}

		CFileEnum::CFileEnum()
		{
		}

		CFileEnum::~CFileEnum()
		{
			if (m_hEnumFile)
				_findclose(m_hEnumFile);
		}

		bool CFileEnum::StartEnumeration(char const* const szEnumPath, char const* const szEnumPattern, _finddata_t* pFile)
		{
			CStr path = szEnumPath;
			auto len = path.length();
			if (path[len - 1] != '\\' && path[len - 1] != '/')
			{
				path += "\\";
			}
			path += szEnumPattern;
			return StartEnumeration(path.c_str(), pFile);
		}

		bool CFileEnum::StartEnumeration(char const* const szEnumPathAndPattern, _finddata_t* pFile)
		{
			int v4; // eax

			if (this->m_hEnumFile)
			{
				_findclose(this->m_hEnumFile);
				this->m_hEnumFile = 0;
			}
			v4 = _findfirst(szEnumPathAndPattern, pFile);
			this->m_hEnumFile = v4;
			if (v4 != -1)
				return 1;
			_findclose(-1);
			this->m_hEnumFile = 0;
			return 0;
		}
	}
}
