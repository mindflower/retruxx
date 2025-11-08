// cstdio standard header
#pragma once
#include "yvals.hpp"

#ifdef _STD_USING
 #undef _STD_USING
  #include <stdio.h>
 #define _STD_USING

#else /* _STD_USING */
 #include <stdio.h>

 #if _GLOBAL_USING
_OLDSTD_BEGIN
using ::size_t; using ::fpos_t; using ::FILE;
using ::clearerr; using ::fclose; using ::feof;
using ::ferror; using ::fflush; using ::fgetc;
using ::fgetpos; using ::fgets; using ::fopen;
using ::fprintf; using ::fputc; using ::fputs;
using ::fread; using ::freopen; using ::fscanf;
using ::fseek; using ::fsetpos; using ::ftell;
using ::fwrite; using ::getc; using ::getchar;
using ::gets; using ::perror;
using ::putc; using ::putchar;
using ::printf; using ::puts; using ::remove;
using ::rename; using ::rewind; using ::scanf;
using ::setbuf; using ::setvbuf; using ::sprintf;
using ::sscanf; using ::tmpfile; using ::tmpnam;
using ::ungetc; using ::vfprintf; using ::vprintf;
using ::vsprintf;
_OLDSTD_END
 #endif /* _GLOBAL_USING */

#endif /* _STD_USING */

/*
 * Copyright (c) 1992-2002 by P.J. Plauger.  ALL RIGHTS RESERVED.
 * Consult your license regarding permissions and restrictions.
 V3.13:0009 */
