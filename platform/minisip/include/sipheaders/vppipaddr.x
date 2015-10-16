/*******************************************************************************
    Copyright (C), 1988-2006, Huawei Tech. Co., Ltd.

    FileName        : vppipaddr.x

    Author          : zhaolei (ronnie)

    Version         : 1.0

    Date            : 2005-12-27

    Description     : The file defines the interface functions for the
                      IP address encode and decode operations.

    Function List   :
        1. VppIpv4Dec
        2. VppIpv4Enc
        3. VppIpv6Dec
        4. VppIpv6Enc

    History:
        1. Date        : <YYYY-MM-DD>
           Tag         : None
           Author      :
           Modification:
*******************************************************************************/

#ifndef __VPP_IP_ADDR_X__
#define __VPP_IP_ADDR_X__


#include "vpptypes.h"
#include "vppcommon.x"

#ifdef __cplusplus
extern "C"{
#endif /* __cplusplus */

/*******************************************************************************
 Function   : VppIpv4Dec

 Description: Convert IPv4 text string into IPv4 binary address.

 Input      : pcIpv4Str   - IPv4 text string format , like : "255.255.255.255".

 Output     : pucIpv4Addr - IPv4 binary address, this is pointer to an array of
                            size four, the memory for this has to be allocated
                            by the user (4 bytes).

 Return     : VPP_SUCCESS if success, others if failure.
*******************************************************************************/
VPP_SINT32    VppIpv4Dec
(
    IN VPP_CHAR           *pcIpv4Str,
    OUT VPP_UINT8         *pucIpv4Addr
);

/*******************************************************************************
 Function   : VppIpv6Dec

 Description: Convert IPv6 text string into IPv6 binary address.

 Input      : pcIpv6Str   - IPv6 text string format , like : "ABCD::1234"

 Output     : pucIpv6Addr - IPv6 binary address, this is pointer to an array of
                            size sixteen, the memory for this has to be
                            allocated by the user (16 bytes).

 Return     : VPP_SUCCESS if success, others if failure.

 Remarks    : About IPv6 address. We decode in Big Endian ("big end first")
              format as TptD ssktwrap.c will use it.


Left most is always byte zero

"127.0.0.1" will be decode as
    ipv4[0] = 127
    ipv4[1] = 0
    ipv4[2] = 0
    ipv4[3] = 1

 and "1234:5678:9ABC:DEF1::ABCD" will be decode as
    ipv6[0]  = 0x12  , ipv6[1]  = 0x34
    ipv6[2]  = 0x56  , ipv6[3]  = 0x78
    ipv6[4]  = 0x9A  , ipv6[5]  = 0xBC
    ipv6[6]  = 0xDE  , ipv6[7]  = 0xF1
    ipv6[8]  = 0x00  , ipv6[9]  = 0x00
    ipv6[10] = 0x00  , ipv6[11] = 0x00
    ipv6[12] = 0x00  , ipv6[13] = 0x00
    ipv6[14] = 0xAB  , ipv6[15] = 0xCD


---------------------- RFC 3513-----------------------------------BEGIN
2.2 Text Representation of Addresses

   There are three conventional forms for representing IPv6 addresses as
   text strings:

   1. The preferred form is x:x:x:x:x:x:x:x, where the 'x's are the
      hexadecimal values of the eight 16-bit pieces of the address.

      Examples:

         FEDC:BA98:7654:3210:FEDC:BA98:7654:3210

         1080:0:0:0:8:800:200C:417A

      Note that it is not necessary to write the leading zeros in an
      individual field, but there must be at least one numeral in every
      field (except for the case described in 2.).

   2. Due to some methods of allocating certain styles of IPv6
      addresses, it will be common for addresses to contain long strings
      of zero bits.  In order to make writing addresses containing zero
      bits easier a special syntax is available to compress the zeros.



Hinden & Deering            Standards Track                     [Page 4]

RFC 3513              IPv6 Addressing Architecture            April 2003


      The use of "::" indicates one or more groups of 16 bits of zeros.
      The "::" can only appear once in an address.  The "::" can also be
      used to compress leading or trailing zeros in an address.

      For example, the following addresses:

         1080:0:0:0:8:800:200C:417A  a unicast address
         FF01:0:0:0:0:0:0:101        a multicast address
         0:0:0:0:0:0:0:1             the loopback address
         0:0:0:0:0:0:0:0             the unspecified addresses

      may be represented as:

         1080::8:800:200C:417A       a unicast address
         FF01::101                   a multicast address
         ::1                         the loopback address
         ::                          the unspecified addresses

   3. An alternative form that is sometimes more convenient when dealing
      with a mixed environment of IPv4 and IPv6 nodes is
      x:x:x:x:x:x:d.d.d.d, where the 'x's are the hexadecimal values of
      the six high-order 16-bit pieces of the address, and the 'd's are
      the decimal values of the four low-order 8-bit pieces of the
      address (standard IPv4 representation).  Examples:

         0:0:0:0:0:0:13.1.68.3

         0:0:0:0:0:FFFF:129.144.52.38

      or in compressed form:

         ::13.1.68.3

         ::FFFF:129.144.52.38
---------------------- RFC 3513-----------------------------------END
MiniSip also supports other product requirements

    4. Decoding of "2000:0:a:0:0:1:2.70.0.12" , but MiniSip CANNOT encode
       this same as orginal , it encode as "2000:0:A::1:246:C"

Other Notes

a.  If the address from the user is
    ipv6[0]  = 0x00  , ipv6[1]  = 0x00
    ipv6[2]  = 0x00  , ipv6[3]  = 0x00
    ipv6[4]  = 0x00  , ipv6[5]  = 0x00
    ipv6[6]  = 0x00  , ipv6[7]  = 0x00
    ipv6[8]  = 0x00  , ipv6[9]  = 0x00
    ipv6[10] = 0x00  , ipv6[11] = 0x00
    then
    ipv6[12], ipv6[13], ipv6[14], ipv6[15] contains IPv4 address
    and will be encoded in ::d.d.d.d format

b.  If the address from the user is
    ipv6[0]  = 0x00  , ipv6[1]  = 0x00
    ipv6[2]  = 0x00  , ipv6[3]  = 0x00
    ipv6[4]  = 0x00  , ipv6[5]  = 0x00
    ipv6[6]  = 0x00  , ipv6[7]  = 0x00
    ipv6[8]  = 0x00  , ipv6[9]  = 0x00
    ipv6[10] = 0xFF  , ipv6[11] = 0xFF
    then
    ipv6[12], ipv6[13], ipv6[14], ipv6[15] contains IPv4 address
    and will be encoded in ::FFFF:d.d.d.d format
*******************************************************************************/
VPP_SINT32    VppIpv6Dec
(
    IN VPP_CHAR           *pcIpv6Str,
    OUT VPP_UINT8         *pucIpv6Addr
);

/*******************************************************************************
 Function   : VppIpv4Enc

 Description: Convert IPv4 binary address to text IPv4 string.

 Input      : pucIpv4Addr - IPv4 binary address (4 bytes)
              pulIpv4Len  - Total length of IPv4 output buffer(pcIpv4Str).
                            Normally this can be greater than or equal to size
                            of "255.255.255.255".

 Output     : pcIpv4Str   - IPv4 text string format, the memory should be
                            allocated by the user, the size of this memory
                            normally this can be greater than or equal to size
                            of "255.255.255.255".
              pulIpv4Len  - Value is real encoded length

 Return     : VPP_NULL_PTR if failure, IPv4 address string if success same as
              pcIpv4Str.
*******************************************************************************/
VPP_CHAR*     VppIpv4Enc
(
    IN VPP_UINT8          *pucIpv4Addr,
    OUT VPP_CHAR          *pcIpv4Str,
    IO VPP_UINT32         *pulIpv4Len
);

/*******************************************************************************
 Function   : VppIpv6Enc

 Description: Convert IPv6 binary address into presentation (printable)
              format.

 Input      : pucIpv6Addr - IPv6 binary address(16 bytes)
              pulIpv6Len  - Total length of IPv6 output buffer(pcIpv6Str).
                            Normally this can be greater than or equal to size
                            of "ffff:ffff:ffff:ffff:ffff:ffff:255.255.255.255"
                            (length 45). This is the maximum possible length of
                            IPv6 address in text format.

 Output     : pcIpv6Str   - IP64 text string format, the memory should be
                            allocated by the user, the size of this memory
                            normally this can be greater than or equal to size
                            of "ffff:ffff:ffff:ffff:ffff:ffff:255.255.255.255"
                            (length 45). This is the maximum possible length of
                            IPv6 address in text format.
              pulIpv6Len  - Value is real encoded length

 Return     : VPP_NULL_PTR if failure, IPv6 address string if success.
*******************************************************************************/
VPP_CHAR*     VppIpv6Enc
(
    IN VPP_UINT8          *pucIpv6Addr,
    OUT VPP_CHAR          *pcIpv6Str,
    IO VPP_UINT32         *pulIpv6Len
);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __VPP_IP_ADDR_X__ */

