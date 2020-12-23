#define SIMPLE_TEST(x) int main(){ x; return 0; }

#ifdef STDC_HEADERS
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <float.h>
int main() { return 0; }
#endif /* STDC_HEADERS */

#ifdef HAVE_ATTRIBUTE

#if 0
static void test int __attribute((unused)) var)
{
  int __attribute__((unused)) x = var;
}

int main(void)
{
  test(19);
}

#else
int
main ()
{
int __attribute__((unused)) x
  ;
  return 0;
}
#endif


#endif /* HAVE_ATTRIBUTE */

#ifdef HAVE_OFF64_T
#include <sys/types.h>
int main()
{
  off64_t n = 0;
  return (int)n;
}
#endif

#ifdef HAVE_STAT64_STRUCT
#include <sys/types.h>
#include <sys/stat.h>],
struct stat64 sb;
int main()
{
  return 0;
}
#endif

#ifdef TEST_LFS_WORKS
/* Return 0 when LFS is available and 1 otherwise.  */
#define _LARGEFILE_SOURCE
#define _LARGEFILE64_SOURCE
#define _LARGE_FILES
#define _FILE_OFFSET_BITS 64
#include <sys/types.h>
#include <sys/stat.h>
#include <assert.h>
#include <stdio.h>

int main(int argc, char **argv)
{
  /* check that off_t can hold 2^63 - 1 and perform basic operations... */
#define OFF_T_64 (((off_t) 1 << 62) - 1 + ((off_t) 1 << 62))
  if (OFF_T_64 % 2147483647 != 1)
    return 1;

  /* stat breaks on SCO OpenServer */
  struct stat buf;
  stat( argv[0], &buf );
  if (!S_ISREG(buf.st_mode))
    return 2;

  FILE *file = fopen( argv[0], "r" );
  off_t offset = ftello( file );
  fseek( file, offset, SEEK_CUR );
  fclose( file );
  return 0;
}
#endif
