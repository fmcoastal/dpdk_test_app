#ifndef _fs_net_common_h
#define _fs_net_common_h




#define LE     /* ARM, X86 */

#ifdef LE
#include <byteswap.h>

#define SWAP_16(x) ( __bswap_16(x) )
#define SWAP_32(x) ( __bswap_32(x) )
#else
#define SWAP_16(x) (x)
#define SWAP_32(x)  (x)
#endif

// L4 Payload types
#define IPv4_VERSION  0x04    /* constants */
#define UDP_PACKET    0x11    /* constants (17) */
#define TCP_PACKET    0x06    /* constants */
#define ICMP_PACKET   0x01    /* constants */


// L3 Payload Types
#define MAC_HEADER_SIZE  (6+6+2+4)
#define  ARP_PKT    (uint16_t)0x0806
#define  IPV4_PKT   (uint16_t)0x0800
#define  IPV6_PKT   (uint16_t)0x86dd
#define  Q802_PKT   (uint16_t)0x8100
#define  PAUSE_PKT  (uint16_t)0x8088 /* Pause Packet  */
#define  LLDP_PKT   (uint16_t)0x88cc /* IEEE Std 802.1AB - Link Layer Discovery Protocol (LLDP)  */
#define  SPN_TREE   (uint16_t)0x0027 /* Spanning Tree  */



// byteswap is defined and used only in the .c File
//#define BYTE_SWAP(x) ( ((x << 8) & 0xff00) | (( x >> 8)  & 0x00ff))
//#define BYTE_SWAP(x) x

void PrintBuff(uint8_t * buffer, int64_t bufferSize,uint8_t * Address,const char * title);




//MAC address
typedef struct
{
    uint8_t   addr[6];         /* Mac Address */
}MacAddr_t;


typedef struct ip_endpt_struct {
      uint32_t   ip_addr;
      uint16_t   port;
      MacAddr_t  MAC;
 } ip_endpt_t;

void Print_ip_endpt(ip_endpt_t * var,char *var_name);



#define fcrc16

#ifdef fcrc16

#define NEW_CRC16_SEED  ((uint32_t)0xffff)
#define NEW_CRC32_SEED  ((uint32_t)0xffffffff)

/** Reverses bit order. MSB -> LSB and LSB -> MSB. */
extern  uint32_t reverse(uint32_t x);

//   for starting new crc calc, make sure crc=0xffffffff
extern  uint16_t crc16(uint8_t * message, uint32_t msgsize, uint16_t crc);

//   for starting new crc calc, make sure crc=0xffffffff
extern  uint32_t crc32(uint8_t * message, uint32_t msgsize, uint32_t crc);

#endif








//Source Port
//Destination Port
//Sequence Number
//Acknowledgement Number
//Data Offset/reserved flagss
//WindowSize
//Checksum (includes Header & Data)
//Urgent Pointer
//Options (if data offset >5)

typedef struct
{
    uint16_t  SrcPort;
    uint16_t  DstPort;
    uint32_t  SequenceNumber;
    uint32_t  AckNumber;
    uint8_t   DataOffset;             // from DataOffset/Flags
    uint16_t  Flags;
    uint16_t  WindowSize;
    uint16_t  CheckSum ;
    uint16_t  UrgentPtr;
    uint32_t  Options;          //(if data offset >5)
    uint32_t  fSize;            // need these for crc calculation  save the size of the TC{ H    dr & Data packet you just built
    uint32_t  fSrcIPAddr;       // need these for crc calculation
    uint32_t  fDstIPAddr;       // need these for crc calculation

}TCP_Hdr;


/////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////MAC///////////////////////////////////////////

//MAC Header
typedef struct
{
    MacAddr_t  DstMac;     /* destination Mack address */
    MacAddr_t  SrcMac;     /* source Mack  address */
    uint8_t    Q802_3;     /* Q802_3  Number of Vlans 0-2  */
    uint16_t   vlan_0;     /* Q802_3  VLan 0  - if present +4 bytes  0x8100 vlan#  */
    uint16_t   vlan_1;     /* Q802_3  VLan 1  - if present +4 bytes  0x8100 vlan# */
    uint16_t   EtherType;  /* 0x0800  IPV40,  x0806 ARP,  0x8100 Q802_3, 0x86dd  IPV6 */
    uint32_t   CRC32;      /* EVOL --only valid after packet create
                              When a source station assembles a MAC frame, it calculates a CRC
                              checksum on all the bits in the frame from Destination MAC Address (DA)
                              to the Pad fields (that is, all fields except the preamble,
                              start frame delimiter, and frame check sequence). */
    uint32_t  TotPacketLenght;  /* EVOL--only valid after packet create total packet
                                   length with CRC32 a the end (Bytes) */
}MAC_Hdr;


// returns a string of a human readable formatted Mac String.
extern  char * formatMACAddr(MacAddr_t* mac);

// on success (0), update the *mac,
// on failure (-1),  leaves *mac unchanged.
extern  int StringToMac(char * string,MacAddr_t * mac);

extern  void printMAC(MacAddr_t* mac,const char * string);

extern  void SetMAC(MacAddr_t* Dst, MacAddr_t* Src);

// returns 0 if the mac addresses are Identical
extern  int  CompareMAC(MacAddr_t* mac0, MacAddr_t* mac1);

extern  void printMAC_Hdr(MAC_Hdr *hdr);

// copies the MAC header data to the structure pointed to by MAC_Hdr
extern   int GetMacData(uint8_t * pMACStart, MAC_Hdr *ExtractedData);

// Returns the number of bytes to the "Next Packet Type"
//   does not count the 4bytes of crc at the end.
extern  int GetMacHeaderSize(uint8_t * pMACStart);

// assume the string is at least
char *   formatIpAddr(uint32_t ip);
uint32_t StringToIp(char * string);  // string must be a.b.c.d

// assume the string is at least 32 chars
void macString(char mac[],char * string);





/////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////IPv4//////////////////////////////////////////
//IP Header
typedef struct
{
    uint8_t   Version;         /* Version 4  Length 4 */
    uint8_t   HeaderLength;    /*            Length 4 */

    uint8_t   DiffSrv;         /* DiffServ6  Congestion notification 2 */
    uint8_t   Congestion;         /* DiffServ6  Congestion notification 2 */

    uint16_t  TotalLength;     /* lengh of the header and the IP Payload */
    uint16_t  Identification;

    uint8_t   Flags;            /* Flg3 Frag Offset13*/
    uint16_t  FragOffset;       /* Flg3 Frag Offset13*/

    uint8_t   TimeToLive;
    uint8_t   Protocol;
    uint16_t  CheckSum;       /* header only */
    uint32_t  SrcIPAddr;
    uint32_t  DstIPAddr;
    uint32_t  Options;
}IPv4_Hdr;

// no options included in size below !!!!!!!!!!
#define IP_HDR_STRT_SZ  (sizeof(uint8_t)*4  + sizeof(uint16_t)*4 +  sizeof(uint32_t)*2 )

#define IPv4_CHECKSUM_OFFSET 0x0A

extern  char * formatIPAddr(uint32_t IpAddr);

extern  void printIPv4_Hdr(IPv4_Hdr *hdr);

// copies the MAC header data to the structure pointed to by MAC_Hdr
extern  int GetIPv4Data(uint8_t * pIPv4Start, IPv4_Hdr *ExtractedData);

// Returns the number of bytes to the "Next Packet Type"
extern int GetIPv4HeaderSize(uint8_t * pIPv4Start);




//////////////////////////////////////////////////////////////////////////
//////////////////////////// ARP /////////////////////////////////////////

typedef struct __attribute__((__packed__)) {
    uint16_t HwType;
    uint16_t Protocol;
    uint8_t  HwAddLen;
    uint8_t  ProtocolAddLen;
    uint16_t OpCode;
    MacAddr_t  SrcHwAddr;
    uint32_t  SrcProtocolAddr;
    MacAddr_t  TarHwAddr;
    uint32_t  TarProtocolAddr;
}ArpPktData_t;




/////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////UDP///////////////////////////////////////////

//Source Port
//Destination Port
//Length                (Includes Header & Data)
//Checksum         (Includes header & Data)
typedef struct
{
    uint16_t  SrcPort;
    uint16_t  DstPort;
    uint16_t  Length;
    uint16_t  CheckSum;
}UDP_Hdr;


extern void printUDP_Hdr(UDP_Hdr *hdr);

// copies the MAC header data to the structure pointed to by MAC_Hdr
extern  int GetUDPData(uint8_t * pUDPStart, UDP_Hdr *ExtractedData);

// Returns the number of bytes to the "Next Packet Type"
extern  int GetUDPHeaderSize(uint8_t * pUDPStart);

////////////////////////////////////////////////////
// assumes the pointer has a UDP header at the beginning.
//  We clear the CheckSum
//  Calculate a new checksum
//  place the checksum in the header
//  Per https://tools.ietf.org/html/rfc768 the checksum can be zero.  If an actual checksum i    s wanted
//     call this function after the IP header is attached.
//     remember if the checksum calculation needs to be all ones if it calculates to zero,
//  And the Checksum is done on uint16 boundries.
//static uint16_t CalculateUDPCkSum(uint8_t * ip_packet);

// Builds a TCP packet,
//   Calculates Length and Checksum
uint64_t build_tcp_packet(TCP_Hdr *TcpHdr , uint8_t * pData, uint16_t DataSz,uint8_t *pBuf, uint16_t BufSz );

// Builds a UDP packet,
//   Calculates Length and Checksum
uint64_t build_udp_packet(UDP_Hdr *UdpHdr ,uint8_t * pData, uint16_t DataSz,uint8_t *pBuf, uint16_t BufSz );



/* This Calculates the IPv4 checksum. It does not put the results in header field,
it simply returns the value

IP HEADER CHECKSUM:: This means that if we divide the IP header is 16 bit words and
sum each of them up and then finally do a ones compliment of the sum then the value
generated out of this operation would be the checksum.

Now, the above is done at the source side which is sending the data packet.  At the
destination side which receives the data packet replaces the checksum value in the
header with all zeros and then calculates the checksum based on the same algorithm
as mentioned above. After a checksum value is obtained then this value is compared
with the value that came in the header. This comparison decides whether the IP
header is fine or corrupted. */

// static uint16_t CalculateIPv4CkSum(uint8_t * buf );

//   Calculates Length and Checksum
uint64_t build_IPv4_packet(IPv4_Hdr *IPv4Hdr ,uint8_t * pData, uint16_t DataSz,uint8_t *pBuf,     uint16_t BufSz );

// build_mac_packet
uint64_t build_MAC_packet(MAC_Hdr *MACHdr ,uint8_t * pData, uint16_t DataSz,uint8_t *pBuf, uint16_t BufSz );


/*
static  uint8_t  g_PacketPayload[]={'D' ,'E' ,'A' ,'D' ,'B' ,'E' ,'E' ,'F' ,   'F' ,'E' ,'E' ,'D' ,'C' ,'A' ,'F' ,'E' ,
 0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,   0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f,
 'c' ,'a' ,'1' ,'1' ,'f' ,'0' ,'0' ,'d' ,   'b' ,'a' ,'b' ,'e' ,'-' ,'E' ,'0' ,'B' };

static uint64_t  g_PayloadSz = 16*3;
*/

//#define BYTE_SWAP(x) ( ((x << 8) & 0xff00) | (( x >> 8)  & 0x00ff))
#define BYTE_SWAP(x) x

/* This Calculates the IPv4 checksum. It does not put the results in header field, it simply returns the value
IP HEADER CHECKSUM:: This means that if we divide the IP header is 16 bit words and
sum each of them up and then finally do a one~Rs compliment of the sum then the value
generated out of this operation would be the checksum.

Now, the above is done at the source side which is sending the data packet.  At the
destination side which receives the data packet replaces the checksum value in the
header with all zeros and then calculates the checksum based on the same algorithm
as mentioned above. After a checksum value is obtained then this value is compared
with the value that came in the header. This comparison decides whether the IP
header is fine or corrupted. */

extern uint16_t CalculateIPv4CkSum(uint8_t * buf );



/**
 * Debug routine to dump the packet structure to the console
 *
 * @param work   Work queue entry containing the packet to dump
 * @return
 */

// Returns true (0) if we Recognize the packet type.
//  Goal is to consolidate the THree functons above into 1 function.
int ParsePacketData( uint8_t *buf , int64_t bufsz,
                     MacAddr_t *DstMAC, MacAddr_t *SrcMAC,
                     uint32_t *DstIpAddr,uint32_t * SrcIpAddr,
                     uint16_t *DstPort,uint16_t *SrcPort,
                     uint16_t *L3Type, uint16_t *L4Type,
                     uint8_t* *pktbuf, int64_t *pktsz,
                     uint8_t* *payloadbuf, int64_t *payloadsz,
                     uint64_t *FPABuff );



extern int  FBuildUdpPacket( ip_endpt_t *dst,ip_endpt_t *src ,uint8_t * data, int64_t datasz ,uint8_t     **PktPtr,int64_t * PktSz );





#endif

