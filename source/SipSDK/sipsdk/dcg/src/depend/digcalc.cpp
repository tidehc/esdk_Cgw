#include "md5.h"
#include "string.h"
#include "digcalc.h"

#ifndef WIN32
#define stricmp strcasecmp
#endif


void CvtHex(
    IN HASH Bin,
    OUT HASHHEX Hex
    )
{
    unsigned short i;
    unsigned char j;

    for (i = 0; i < HASHLEN; i++) {
        j = (Bin[i] >> 4) & 0xf;//lint !e702
        if (j <= 9)
            Hex[i*2] = (char)(j + '0');
         else
            Hex[i*2] = (char)(j + 'a' - 10);
        j = Bin[i] & 0xf;
        if (j <= 9)
            Hex[i*2+1] = (char)(j + '0');
         else
            Hex[i*2+1] = (char)(j + 'a' - 10);
    };
    Hex[HASHHEXLEN] = '\0';
}//lint !e818 

/* calculate H(A1) as per spec */
void DigestCalcHA1(
    IN char * pszAlg,
    IN char * pszUserName,
    IN char * pszRealm,
    IN char * pszPassword,
    IN char * pszNonce,
    IN char * pszCNonce,
    OUT HASHHEX SessionKey
    )
{
      MD5_CTX Md5Ctx;
	  HASH HA1 = {0};

      MD5Init(&Md5Ctx);
      MD5Update(&Md5Ctx, (unsigned char*)pszUserName, strlen(pszUserName));
      MD5Update(&Md5Ctx, (unsigned char*)":", 1);//lint !e1773
      MD5Update(&Md5Ctx, (unsigned char*)pszRealm, strlen(pszRealm));
      MD5Update(&Md5Ctx, (unsigned char*)":", 1);//lint !e1773
      MD5Update(&Md5Ctx, (unsigned char*)pszPassword, strlen(pszPassword));
      MD5Final((unsigned char*)HA1, &Md5Ctx);
      if (stricmp(pszAlg, "md5-sess") == 0) {
            MD5Init(&Md5Ctx);
            MD5Update(&Md5Ctx, (unsigned char*)HA1, HASHLEN);
            MD5Update(&Md5Ctx, (unsigned char*)":", 1);//lint !e1773
            MD5Update(&Md5Ctx, (unsigned char*)pszNonce, strlen(pszNonce));
            MD5Update(&Md5Ctx, (unsigned char*)":", 1);//lint !e1773
            MD5Update(&Md5Ctx, (unsigned char*)pszCNonce, strlen(pszCNonce));
            MD5Final((unsigned char*)HA1, &Md5Ctx);
      };
      CvtHex(HA1, SessionKey);
}//lint !e818

/* calculate request-digest/response-digest as per HTTP Digest spec */
void DigestCalcResponse(
    IN HASHHEX HA1,           /* H(A1) */
    IN char * pszNonce,       /* nonce from server */
    IN char * pszNonceCount,  /* 8 hex digits */
    IN char * pszCNonce,      /* client nonce */
    IN char * pszQop,         /* qop-value: "", "auth", "auth-int" */
    IN char * pszMethod,      /* method from the request */
    IN char * pszDigestUri,   /* requested URL */
    IN HASHHEX HEntity,       /* H(entity body) if qop="auth-int" */
    OUT HASHHEX Response      /* request-digest or response-digest */
    )
{
      MD5_CTX Md5Ctx;
	  HASH HA2 = {0};
	  HASH RespHash = {0};
      HASHHEX HA2Hex;

      // calculate H(A2)
      MD5Init(&Md5Ctx);
      MD5Update(&Md5Ctx, (unsigned char*)pszMethod, strlen(pszMethod));
      MD5Update(&Md5Ctx, (unsigned char*)":", 1);//lint !e1773
      MD5Update(&Md5Ctx, (unsigned char*)pszDigestUri, strlen(pszDigestUri));
      if (stricmp(pszQop, "auth-int") == 0) {
            MD5Update(&Md5Ctx, (unsigned char*)":", 1);//lint !e1773
            MD5Update(&Md5Ctx, (unsigned char*)HEntity, HASHHEXLEN);
      };
      MD5Final((unsigned char*)HA2, &Md5Ctx);
       CvtHex(HA2, HA2Hex);

      // calculate response
      MD5Init(&Md5Ctx);
      MD5Update(&Md5Ctx, (unsigned char*)HA1, HASHHEXLEN);
      MD5Update(&Md5Ctx, (unsigned char*)":", 1);//lint !e1773
      MD5Update(&Md5Ctx, (unsigned char*)pszNonce, strlen(pszNonce));
      MD5Update(&Md5Ctx, (unsigned char*)":", 1);//lint !e1773
      if (*pszQop) {
          MD5Update(&Md5Ctx, (unsigned char*)pszNonceCount, strlen(pszNonceCount));
          MD5Update(&Md5Ctx, (unsigned char*)":", 1);//lint !e1773
          MD5Update(&Md5Ctx, (unsigned char*)pszCNonce, strlen(pszCNonce));
          MD5Update(&Md5Ctx, (unsigned char*)":", 1);//lint !e1773
          MD5Update(&Md5Ctx, (unsigned char*)pszQop, strlen(pszQop));
          MD5Update(&Md5Ctx, (unsigned char*)":", 1);//lint !e1773
      };
      MD5Update(&Md5Ctx, (unsigned char*)HA2Hex, HASHHEXLEN);
      MD5Final((unsigned char*)RespHash, &Md5Ctx);
      CvtHex(RespHash, Response);
}

//计算原始MD5值
void Md5
(
    IN  char*   pszIn,
    OUT HASH    szOut
)
{
    MD5_CTX Md5Ctx;

    MD5Init(&Md5Ctx);
    MD5Update(&Md5Ctx, (unsigned char*)pszIn, strlen(pszIn));
    MD5Final((unsigned char*)szOut, &Md5Ctx);
}

//计算出十六进制的MD5值
void Md5Hex
(
    IN  char*   pszIn,
    OUT HASHHEX szOut
)
{
	HASH Hash = {0};
    Md5(pszIn, Hash);
    CvtHex(Hash, szOut);
}


