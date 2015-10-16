/*
------------------------------------------------------------------------------
rand.c: By Bob Jenkins.  My random number generator, ISAAC.  Public Domain.
MODIFIED:
  960327: Creation (addition of randinit, really)
  970719: use context, not global variables, for internal state
  980324: added main (ifdef'ed out), also rearranged randinit()
  010626: Note that this is public domain
------------------------------------------------------------------------------
*/

#include "rand.h"

#ifdef __cplusplus
extern "C" {
#endif

#define ind(mm, x)  \
    (*(SS_UINT *)((SS_UCHAR *)(mm) + ((x) & ((RANDSIZ - 1) << 2))))

#define rngstep(mix, a, b, mm, m, m2, r, x) \
{ \
  x = *m;  \
  a = (a ^ (mix)) + *(m2++); \
  *(m++) = y = ind(mm, x) + a + b; \
  *(r++) = b = ind(mm, y >> RANDSIZL) + x; \
}

/*******************************************************************************
 Function   : sipisaac

 Description: random function

 Input      : ctx - random context

 Output     : None

 Return     : None
*******************************************************************************/
SS_VOID sipisaac
(
    randctx *ctx
)
{
   register SS_UINT a, b, x, y, *m, *mm, *m2, *r, *mend;
   mm = ctx->randmem; r = ctx->randrsl;
   a = ctx->randa; b = ctx->randb + (++ctx->randc);
   /*lint -e826 */
   for (m = mm, mend = m2 = m + (RANDSIZ / 2); m < mend; )
   {
      rngstep( a << 13, a, b, mm, m, m2, r, x);
      rngstep( a >> 6 , a, b, mm, m, m2, r, x);
      rngstep( a << 2 , a, b, mm, m, m2, r, x);
      rngstep( a >> 16, a, b, mm, m, m2, r, x);
   }
   for (m2 = mm; m2<mend; )
   {
      rngstep( a << 13, a, b, mm, m, m2, r, x);
      rngstep( a >> 6 , a, b, mm, m, m2, r, x);
      rngstep( a << 2 , a, b, mm, m, m2, r, x);
      rngstep( a >> 16, a, b, mm, m, m2, r, x);
   }
   /*lint +e826 */
   ctx->randb = b; ctx->randa = a;
}


#define rand_mix(a, b, c, d, e, f, g, h) \
{ \
   a ^= b << 11; d += a; b += c; \
   b ^= c >> 2;  e += b; c += d; \
   c ^= d << 8;  f += c; d += e; \
   d ^= e >> 16; g += d; e += f; \
   e ^= f << 10; h += e; f += g; \
   f ^= g >> 4;  a += f; g += h; \
   g ^= h << 8;  b += g; h += a; \
   h ^= a >> 9;  c += h; a += b; \
}

/* if (flag==TRUE), then use the contents of randrsl[] to initialize mm[]. */
/*******************************************************************************
 Function   : siprandinit

 Description: random init function

 Input      : ctx - random context
              flag - random flag

 Output     : None

 Return     : None
*******************************************************************************/
SS_VOID siprandinit
(
    randctx    *ctx,
    SS_UINT16   flag
)
{
   SS_UINT16 i;
   SS_UINT a, b, c, d, e, f, g, h;
   SS_UINT *m,*r;

   ctx->randa = ctx->randb = ctx->randc = 0;
   m = ctx->randmem;
   r = ctx->randrsl;
   a = b = c = d = e = f = g = h = 0x9e3779b9;  /* the golden ratio */

   for (i = 0; i < 4; ++i)          /* scramble it */
   {
     rand_mix(a, b, c, d, e, f, g, h);
   }

   if (flag)
   {
     /* initialize using the contents of r[] as the seed */
     for (i =0 ; i < RANDSIZ - 7; i += 8)
     {
       a += r[i]; b += r[i + 1]; c += r[i + 2]; d += r[i + 3];
       e += r[i + 4]; f += r[i + 5]; g += r[i + 6]; h += r[i + 7];
       rand_mix(a, b, c, d, e, f, g, h);
       m[i] = a; m[i + 1] = b; m[i + 2] = c; m[i + 3] = d;
       m[i + 4] = e; m[i + 5] = f; m[i + 6] = g; m[i + 7] = h;
     }
     /* do a second pass to make all of the seed affect all of m */
     for (i = 0; i < RANDSIZ - 7; i += 8)
     {
       a += m[i]; b += m[i + 1]; c += m[i + 2]; d += m[i + 3];
       e += m[i + 4]; f += m[i + 5]; g += m[i + 6]; h += m[i + 7];
       rand_mix(a, b, c, d, e, f, g, h);
       m[i] = a; m[i + 1] = b; m[i + 2] = c; m[i + 3] = d;
       m[i + 4] = e; m[i + 5] = f; m[i + 6] = g; m[i + 7] = h;
     }
   }
   else
   {
     /* fill in mm[] with messy stuff */
     for (i = 0; i < RANDSIZ - 7; i += 8)
     {
       rand_mix(a, b, c, d, e, f, g, h);
       m[i] = a; m[i + 1] = b; m[i + 2] = c; m[i + 3] = d;
       m[i + 4] = e; m[i + 5] = f; m[i + 6] = g; m[i + 7] = h;
     }
   }

   sipisaac(ctx);            /* fill in the first set of results */
   ctx->randcnt = RANDSIZ;  /* prepare to use the first set of results */
}

#ifdef __cplusplus
}
#endif

