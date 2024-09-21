// cstring standard header
#pragma once
#include "yvals.hpp"

#ifdef _STD_USING
 #undef _STD_USING
  #include "string.h"
 #define _STD_USING

#else /* _STD_USING */
 #include "string.h"

 #if _GLOBAL_USING
_OLDSTD_BEGIN
using ::size_t; using ::memchr; using ::memcmp;
using ::memcpy; using ::memmove; using ::memset;
using ::strcat; using ::strchr; using ::strcmp;
using ::strcoll; using ::strcpy; using ::strcspn;
using ::strerror; using ::strlen; using ::strncat;
using ::strncmp; using ::strncpy; using ::strpbrk;
using ::strrchr; using ::strspn; using ::strstr;
using ::strtok; using ::strxfrm;
_OLDSTD_END
 #endif /* _GLOBAL_USING */

#endif /* _STD_USING */

/*
 * Copyright (c) 1992-2002 by P.J. Plauger.  ALL RIGHTS RESERVED.
 * Consult your license regarding permissions and restrictions.
 V3.13:0009 */
