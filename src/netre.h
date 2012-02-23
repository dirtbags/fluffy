#ifndef __NETRE_H__
#define __NETRE_H__
#include "config.h"

#define WHICHOUT(x) (x ? stderr : stdout)

#ifdef DEBUGSTMTS
/*@notfunction@*/
#   define DDUMPf(fmt, args...) fprintf(stderr, "[@%s:%s():%d] " fmt "\n", __FILE__, __FUNCTION__, __LINE__, ##args)
#else
/*@notfunction@*/
#   define DDUMPf(fmt, args...)
#endif

#define DDUMP(v)   DDUMPf("%s", v)
#define DDUMP_d(v) DDUMPf("%s = %d",   #v, v)
#define DDUMP_u(v) DDUMPf("%s = %u",   #v, v)
#define DDUMP_x(v) DDUMPf("%s = 0x%x", #v, v)
#define DDUMP_s(v) DDUMPf("%s = %s",   #v, v)
#define DDUMP_c(v) DDUMPf("%s = %c",   #v, v)
#define DDUMP_p(v) DDUMPf("%s = %p",   #v, v)

#endif /* __NETRE_H__ */
