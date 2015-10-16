#ifndef IVSCBB_SS_RAND_H
#define IVSCBB_SS_RAND_H
/*
------------------------------------------------------------------------------
rand.h: definitions for a random number generator
By Bob Jenkins, 1996, Public Domain
MODIFIED:
  960327: Creation (addition of randinit, really)
  970719: use context, not global variables, for internal state
  980324: renamed seed to flag
  980605: recommend RANDSIZL=4 for noncryptography.
  010626: note this is public domain
------------------------------------------------------------------------------
*/

//#include "ssdatatype.h"
#include "MiniSip.h"

#ifdef __cplusplus
extern "C" {
#endif

#define RAND
#define RANDSIZL   (8)  /* I recommend 8 for crypto, 4 for simulations */
#define RANDSIZ    (1<<RANDSIZL)

/* context of random number generator */
struct _randctx
{
  SS_UINT randcnt;
  SS_UINT randrsl[RANDSIZ];
  SS_UINT randmem[RANDSIZ];
  SS_UINT randa;
  SS_UINT randb;
  SS_UINT randc;
  SS_UINT posn;
};
typedef  struct _randctx  randctx;

/*
------------------------------------------------------------------------------
 If (flag==TRUE), then use the contents of randrsl[0..RANDSIZ-1] as the seed.
------------------------------------------------------------------------------
*/

SS_VOID siprandinit( randctx *r, SS_UINT16 flag );


SS_VOID sipisaac( randctx *r );


/*
------------------------------------------------------------------------------
 Call rand(/o_ randctx *r _o/) to retrieve a single 32-bit random value
------------------------------------------------------------------------------
*/
/* start SymbianMerge */
#if 0
#define rand(r) \
   (!(r)->randcnt-- ? \
     (isaac(r), (r)->randcnt=RANDSIZ-1, (r)->randrsl[(r)->randcnt]) : \
     (r)->randrsl[(r)->randcnt])
#endif
/* end SymbianMerge */

#ifdef __cplusplus
}
#endif

#endif  /* RAND */


