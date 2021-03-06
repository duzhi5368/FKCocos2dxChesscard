#include "FKEncrpty.h"
//------------------------------------------------------------
#define XOR_ENCRYPT_KEY_LEN					5		
//------------------------------------------------------------
std::string	FKEncrypt::FKEncodeString(const char* Data, unsigned int DataByte)
{
	// encode table
	const char EncodeTable[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	
	std::string strEncode;
	unsigned char Tmp[4] = { 0 };
	int LineLength = 0;
	for (unsigned int i = 0; i<(unsigned int)(DataByte / 3); i++)
	{
		Tmp[1] = *Data++;
		Tmp[2] = *Data++;
		Tmp[3] = *Data++;
		strEncode += EncodeTable[Tmp[1] >> 2];
		strEncode += EncodeTable[((Tmp[1] << 4) | (Tmp[2] >> 4)) & 0x3F];
		strEncode += EncodeTable[((Tmp[2] << 2) | (Tmp[3] >> 6)) & 0x3F];
		strEncode += EncodeTable[Tmp[3] & 0x3F];
		if (LineLength += 4, LineLength == 76) 
		{ 
			strEncode += "\r\n"; 
			LineLength = 0; 
		}
	}
	// encode the left datas
	int Mod = DataByte % 3;
	if (Mod == 1)
	{
		Tmp[1] = *Data++;
		strEncode += EncodeTable[(Tmp[1] & 0xFC) >> 2];
		strEncode += EncodeTable[((Tmp[1] & 0x03) << 4)];
		strEncode += "==";
	}
	else if (Mod == 2)
	{
		Tmp[1] = *Data++;
		Tmp[2] = *Data++;
		strEncode += EncodeTable[(Tmp[1] & 0xFC) >> 2];
		strEncode += EncodeTable[((Tmp[1] & 0x03) << 4) | ((Tmp[2] & 0xF0) >> 4)];
		strEncode += EncodeTable[((Tmp[2] & 0x0F) << 2)];
		strEncode += "=";
	}

	return strEncode;
}
//------------------------------------------------------------
std::string FKEncrypt::FKDecodeString(const char* Data, 
	unsigned int DataByte,unsigned int& OutByte)
{
	// decode table
	const char DecodeTable[] =
	{
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		62, // '+'
		0, 0, 0,
		63, // '/'
		52, 53, 54, 55, 56, 57, 58, 59, 60, 61, // '0'-'9'
		0, 0, 0, 0, 0, 0, 0,
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
		13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, // 'A'-'Z'
		0, 0, 0, 0, 0, 0,
		26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38,
		39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, // 'a'-'z'
	};

	std::string strDecode;
	int nValue;
	unsigned int i = 0;
	while (i < DataByte)
	{
		if (*Data != '\r' && *Data != '\n')
		{
			nValue = DecodeTable[*Data++] << 18;
			nValue += DecodeTable[*Data++] << 12;
			strDecode += (nValue & 0x00FF0000) >> 16;
			OutByte++;
			if (*Data != '=')
			{
				nValue += DecodeTable[*Data++] << 6;
				strDecode += (nValue & 0x0000FF00) >> 8;
				OutByte++;
				if (*Data != '=')
				{
					nValue += DecodeTable[*Data++];
					strDecode += nValue & 0x000000FF;
					OutByte++;
				}
			}
			i += 4;
		}
		else 
		{
			// if the character is \n\r, skip it.
			Data++;
			i++;
		}
	}
	return strDecode;
}
//------------------------------------------------------------
WORD FKEncrypt::FKXOREncryptData(const char* pszSrcData, 
	char* pszEncrypData, WORD wSize)
{
	// check params
	CC_ASSERT(wSize>0);
	CC_ASSERT(pszSrcData!="");
	CC_ASSERT(pszEncrypData!="");

	// init
	pszEncrypData[0]=0;

	// generate key
	std::string strSrcData = pszSrcData;
	unsigned char szRandKey[XOR_ENCRYPT_KEY_LEN];
	szRandKey[0]=strSrcData.length();
	int nRandKeyLen = sizeof(szRandKey) / sizeof(szRandKey[0]);
	for (WORD i=1;i<nRandKeyLen;i++) 
	{
		szRandKey[i]=rand()%255;
	}

	// genarate encrypt string
	BYTE bTempCode=0;
	WORD wTimes=(szRandKey[0]+XOR_ENCRYPT_KEY_LEN-1)/XOR_ENCRYPT_KEY_LEN*XOR_ENCRYPT_KEY_LEN;
	if ((wTimes*4+1)>wSize) return wTimes*4+1;
	for (WORD i=0;i<wTimes;i++)
	{
		if (i<szRandKey[0]) bTempCode=pszSrcData[i]^szRandKey[i%XOR_ENCRYPT_KEY_LEN];
		else bTempCode=szRandKey[i%XOR_ENCRYPT_KEY_LEN]^(rand()%255);
		snprintf(pszEncrypData+i*4,5,"%02x%02x",szRandKey[i%XOR_ENCRYPT_KEY_LEN],bTempCode);
	}

	return wTimes*4;
}
//------------------------------------------------------------
WORD FKEncrypt::FKXORDecryptData(const char* pszEncrypData, 
	char* pszSrcData, WORD wSize)
{
	// check params
	CC_ASSERT(wSize>0);
	CC_ASSERT(pszSrcData!="");
	CC_ASSERT(pszEncrypData!="");

	// init params
	pszSrcData[0]=0;

	// check length
	std::string strEncrypData = pszEncrypData;
	WORD wEncrypPassLen=strEncrypData.length();
	if (wEncrypPassLen<XOR_ENCRYPT_KEY_LEN*4) 
		return 0;

	// check length again
	char * pEnd=NULL;
	char szTempBuffer[3]={pszEncrypData[0],pszEncrypData[1],0};
	WORD wSoureLength=(WORD)strtol(szTempBuffer,&pEnd,16);
	if (wEncrypPassLen!=((wSoureLength+XOR_ENCRYPT_KEY_LEN-1)/XOR_ENCRYPT_KEY_LEN*XOR_ENCRYPT_KEY_LEN*4)) 
		return 0;
	if (wSoureLength>(wSize-1)) 
		return wSoureLength;

	// encrypt
	unsigned char szKeyBuffer[3];
	for (int i=0;i<wSoureLength;i++)
	{
		szKeyBuffer[0]=pszEncrypData[i*4];
		szKeyBuffer[1]=pszEncrypData[i*4+1];
		szKeyBuffer[2]=0;
		szTempBuffer[0]=pszEncrypData[i*4+2];
		szTempBuffer[1]=pszEncrypData[i*4+3];
		szTempBuffer[2]=0;

		UINT uKey=strtol((char*)szKeyBuffer,&pEnd,16);
		UINT uEncrypt=strtol(szTempBuffer,&pEnd,16);
		pszSrcData[i]=(char)((BYTE)uKey^(BYTE)uEncrypt);
	}
	pszSrcData[wSoureLength]=0;

	return wSoureLength;
}
//------------------------------------------------------------
#define S11		7
#define S12		12
#define S13		17
#define S14		22
#define S21		5
#define S22		9
#define S23 	14
#define S24 	20
#define S31 	4
#define S32 	11
#define S33 	16
#define S34 	23
#define S41 	6
#define S42 	10
#define S43 	15
#define S44 	21

#define F(x,y,z) (((x)&(y))|((~x)&(z)))
#define G(x,y,z) (((x)&(z))|((y)&(~z)))
#define H(x,y,z) ((x)^(y)^(z))
#define I(x,y,z) ((y)^((x)|(~z)))

#define ROTATE_LEFT(x,n) (((x)<<(n))|((x)>>(32-(n))))

#define FF(a,b,c,d,x,s,ac)													\
{																			\
	(a)+=F((b),(c),(d))+(x)+(unsigned long int)(ac);						\
	(a)=ROTATE_LEFT((a),(s));												\
	(a)+=(b);																\
}

#define GG(a,b,c,d,x,s,ac)													\
{																			\
	(a)+=G((b),(c),(d))+(x)+(unsigned long int)(ac);						\
	(a)=ROTATE_LEFT ((a),(s));												\
	(a)+=(b);																\
}

#define HH(a,b,c,d,x,s,ac)													\
{																			\
	(a)+=H((b),(c),(d))+(x)+(unsigned long int)(ac);						\
	(a)=ROTATE_LEFT((a),(s));												\
	(a)+=(b);																\
}

#define II(a,b,c,d,x,s,ac)													\
{																			\
	(a)+=I((b),(c),(d))+(x)+(unsigned long int)(ac);						\
	(a)=ROTATE_LEFT((a),(s));												\
	(a)+=(b);																\
}
//------------------------------------------------------------
class CMD5
{
private:
	unsigned long int				state[4];
	unsigned long int				count[2];
	unsigned char					buffer[64];
	unsigned char					PADDING[64];

public:
	CMD5() { MD5Init(); }

public:
	void MD5Final(unsigned char digest[16]);
	void MD5Update(unsigned char * input, unsigned int inputLen);

private:
	void MD5Init();
	void MD5_memset(unsigned char * output, int value, unsigned int len);
	void MD5_memcpy(unsigned char * output, unsigned char * input, unsigned int len);
	void MD5Transform(unsigned long int state[4], unsigned char block[64]);
	void Encode(unsigned char * output, unsigned long int * input, unsigned int len);
	void Decode(unsigned long int *output, unsigned char * input, unsigned int len);
};
//------------------------------------------------------------
void CMD5::MD5Init()
{
	count[0]=0;
	count[1]=0;
	state[0]=0x67452301;
	state[1]=0xefcdab89;
	state[2]=0x98badcfe;
	state[3]=0x10325476;
	MD5_memset(PADDING,0,sizeof(PADDING));
	*PADDING=0x80;
	return;
}
//------------------------------------------------------------
void CMD5::MD5Update (unsigned char * input, unsigned int inputLen)
{
	unsigned int i,index,partLen;
	index=(unsigned int)((this->count[0]>>3)&0x3F);
	if ((count[0]+=((unsigned long int)inputLen<<3))<((unsigned long int)inputLen<<3)) count[1]++;
	count[1]+=((unsigned long int)inputLen>>29);
	partLen=64-index;
	if (inputLen>=partLen) 
	{
		MD5_memcpy((unsigned char*)&buffer[index],(unsigned char *)input,partLen);
		MD5Transform(state,buffer);
		for (i=partLen;i+63<inputLen;i+=64) MD5Transform(state,&input[i]);
		index=0;
	}
	else i=0;
	MD5_memcpy((unsigned char*)&buffer[index],(unsigned char *)&input[i],inputLen-i);
	return;
}
//------------------------------------------------------------
void CMD5::MD5Final(unsigned char digest[16])
{
	unsigned char bits[8];
	unsigned int index,padLen;
	Encode(bits,count,8);
	index=(unsigned int)((count[0]>>3)&0x3f);
	padLen=(index<56)?(56-index):(120-index);
	MD5Update( PADDING,padLen);
	MD5Update(bits,8);
	Encode(digest,state,16);
	MD5_memset((unsigned char*)this,0,sizeof (*this));
	MD5Init();
	return;
}
//------------------------------------------------------------
void CMD5::MD5Transform(unsigned long int state[4], unsigned char block[64])
{
	unsigned long int a=state[0],b=state[1],c=state[2],d=state[3],x[16];
	Decode(x,block,64);

	FF(a,b,c,d,x[ 0],S11,0xd76aa478); /* 1 */
	FF(d,a,b,c,x[ 1],S12,0xe8c7b756); /* 2 */
	FF(c,d,a,b,x[ 2],S13,0x242070db); /* 3 */
	FF(b,c,d,a,x[ 3],S14,0xc1bdceee); /* 4 */
	FF(a,b,c,d,x[ 4],S11,0xf57c0faf); /* 5 */
	FF(d,a,b,c,x[ 5],S12,0x4787c62a); /* 6 */
	FF(c,d,a,b,x[ 6],S13,0xa8304613); /* 7 */
	FF(b,c,d,a,x[ 7],S14,0xfd469501); /* 8 */
	FF(a,b,c,d,x[ 8],S11,0x698098d8); /* 9 */
	FF(d,a,b,c,x[ 9],S12,0x8b44f7af); /* 10 */
	FF(c,d,a,b,x[10],S13,0xffff5bb1); /* 11 */
	FF(b,c,d,a,x[11],S14,0x895cd7be); /* 12 */
	FF(a,b,c,d,x[12],S11,0x6b901122); /* 13 */
	FF(d,a,b,c,x[13],S12,0xfd987193); /* 14 */
	FF(c,d,a,b,x[14],S13,0xa679438e); /* 15 */
	FF(b,c,d,a,x[15],S14,0x49b40821); /* 16 */

	GG(a,b,c,d,x[ 1],S21,0xf61e2562); /* 17 */
	GG(d,a,b,c,x[ 6],S22,0xc040b340); /* 18 */
	GG(c,d,a,b,x[11],S23,0x265e5a51); /* 19 */
	GG(b,c,d,a,x[ 0],S24,0xe9b6c7aa); /* 20 */
	GG(a,b,c,d,x[ 5],S21,0xd62f105d); /* 21 */
	GG(d,a,b,c,x[10],S22,0x2441453);  /* 22 */
	GG(c,d,a,b,x[15],S23,0xd8a1e681); /* 23 */
	GG(b,c,d,a,x[ 4],S24,0xe7d3fbc8); /* 24 */
	GG(a,b,c,d,x[ 9],S21,0x21e1cde6); /* 25 */
	GG(d,a,b,c,x[14],S22,0xc33707d6); /* 26 */
	GG(c,d,a,b,x[ 3],S23,0xf4d50d87); /* 27 */
	GG(b,c,d,a,x[ 8],S24,0x455a14ed); /* 28 */
	GG(a,b,c,d,x[13],S21,0xa9e3e905); /* 29 */
	GG(d,a,b,c,x[ 2],S22,0xfcefa3f8); /* 30 */
	GG(c,d,a,b,x[ 7],S23,0x676f02d9); /* 31 */
	GG(b,c,d,a,x[12],S24,0x8d2a4c8a); /* 32 */

	HH(a,b,c,d,x[ 5],S31,0xfffa3942); /* 33 */
	HH(d,a,b,c,x[ 8],S32,0x8771f681); /* 34 */
	HH(c,d,a,b,x[11],S33,0x6d9d6122); /* 35 */
	HH(b,c,d,a,x[14],S34,0xfde5380c); /* 36 */
	HH(a,b,c,d,x[ 1],S31,0xa4beea44); /* 37 */
	HH(d,a,b,c,x[ 4],S32,0x4bdecfa9); /* 38 */
	HH(c,d,a,b,x[ 7],S33,0xf6bb4b60); /* 39 */
	HH(b,c,d,a,x[10],S34,0xbebfbc70); /* 40 */
	HH(a,b,c,d,x[13],S31,0x289b7ec6); /* 41 */
	HH(d,a,b,c,x[ 0],S32,0xeaa127fa); /* 42 */
	HH(c,d,a,b,x[ 3],S33,0xd4ef3085); /* 43 */
	HH(b,c,d,a,x[ 6],S34,0x4881d05);  /* 44 */
	HH(a,b,c,d,x[ 9],S31,0xd9d4d039); /* 45 */
	HH(d,a,b,c,x[12],S32,0xe6db99e5); /* 46 */
	HH(c,d,a,b,x[15],S33,0x1fa27cf8); /* 47 */
	HH(b,c,d,a,x[ 2],S34,0xc4ac5665); /* 48 */

	II(a,b,c,d,x[ 0],S41,0xf4292244); /* 49 */
	II(d,a,b,c,x[ 7],S42,0x432aff97); /* 50 */
	II(c,d,a,b,x[14],S43,0xab9423a7); /* 51 */
	II(b,c,d,a,x[ 5],S44,0xfc93a039); /* 52 */
	II(a,b,c,d,x[12],S41,0x655b59c3); /* 53 */
	II(d,a,b,c,x[ 3],S42,0x8f0ccc92); /* 54 */
	II(c,d,a,b,x[10],S43,0xffeff47d); /* 55 */
	II(b,c,d,a,x[ 1],S44,0x85845dd1); /* 56 */
	II(a,b,c,d,x[ 8],S41,0x6fa87e4f); /* 57 */
	II(d,a,b,c,x[15],S42,0xfe2ce6e0); /* 58 */
	II(c,d,a,b,x[ 6],S43,0xa3014314); /* 59 */
	II(b,c,d,a,x[13],S44,0x4e0811a1); /* 60 */
	II(a,b,c,d,x[ 4],S41,0xf7537e82); /* 61 */
	II(d,a,b,c,x[11],S42,0xbd3af235); /* 62 */
	II(c,d,a,b,x[ 2],S43,0x2ad7d2bb); /* 63 */
	II(b,c,d,a,x[ 9],S44,0xeb86d391); /* 64 */

	state[0]+=a;
	state[1]+=b;
	state[2]+=c;
	state[3]+=d;

	MD5_memset((unsigned char *)x,0,sizeof(x));

	return;
}
//------------------------------------------------------------
void CMD5::Encode(unsigned char * output, unsigned long int * input,unsigned int len)
{
	unsigned int i, j;
	for (i=0,j=0;j<len;i++,j+=4)
	{
		output[j]=(unsigned char)(input[i]&0xff);
		output[j+1]=(unsigned char)((input[i]>>8)&0xff);
		output[j+2]=(unsigned char)((input[i]>>16)&0xff);
		output[j+3]=(unsigned char)((input[i]>>24)&0xff);
	}
	return;
}
//------------------------------------------------------------
void CMD5::Decode(unsigned long int *output, unsigned char *input, unsigned int len)
{
	unsigned int i,j;
	for (i=0,j=0;j<len;i++,j+=4)
	{
		output[i]=((unsigned long int)input[j])|(((unsigned long int)input[j+1])<<8)|
			(((unsigned long int)input[j+2])<<16)|(((unsigned long int)input[j+3])<< 24);
	}
	return;
}
//------------------------------------------------------------
void CMD5::MD5_memcpy(unsigned char * output, unsigned char * input,unsigned int len)
{
	for (unsigned int i=0;i<len;i++) output[i]=input[i];
}
//------------------------------------------------------------
void CMD5::MD5_memset (unsigned char * output, int value, unsigned int len)
{
	for (unsigned int i=0;i<len;i++) ((char *)output)[i]=(char)value;
}
//------------------------------------------------------------
#define DEF_MAX_MD5_PSW_LEN 64
//------------------------------------------------------------
void FKEncrypt::FKMD5EncryptData(std::string strSrcData, char szMD5Result[DEFAULT_MD5_LEN])
{ 
	CMD5 MD5Encrypt;
	unsigned char szResult[DEFAULT_MD5_LEN/2] = {0}; 
	unsigned char pass[DEF_MAX_MD5_PSW_LEN] = {0};
	int len = strSrcData.length();
	CC_ASSERT(len < DEF_MAX_MD5_PSW_LEN);
	memcpy(pass, strSrcData.c_str(), len);

	MD5Encrypt.MD5Update(pass, len);
	MD5Encrypt.MD5Final(szResult);

	szMD5Result[0]=0;
	for (int i=0;i<DEFAULT_MD5_LEN/2;i++)
	{
		sprintf(&szMD5Result[i*2],"%02x",szResult[i]);
	}

	return;
}
//------------------------------------------------------------