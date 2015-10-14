#include <string.h>   
#include <stdio.h>   
#include <time.h>   
#include "bignum.h" 
#include "md5.h"
#include <iostream>
using namespace std;
#define MAXPRIMECOUNT 1000 // 寻找素数的最大次数   
unsigned int nSmallPrimes[MAXPRIMECOUNT][2]; // 小素数数组   
unsigned int nPrimeCount = 0; // 寻找素数的次数   

// 寻找小素数   
void MakeSmallPrimes()   
{   
	unsigned int n;   
	unsigned int j;   
	nPrimeCount = 3;   
	// 首先定义几个小素数   
	nSmallPrimes[0][0] = 2;   
	nSmallPrimes[1][0] = 3;   
	nSmallPrimes[2][0] = 5;   
	nSmallPrimes[0][1] = 4;   
	nSmallPrimes[1][1] = 9;   
	nSmallPrimes[2][1] = 25;   
	// 开始寻找素数   
	for (n=7; nPrimeCount < MAXPRIMECOUNT; n+=2)   
	{   
		for (j=0; nSmallPrimes[j][1] < n; j++)   
		{   
			if (j>= nPrimeCount) // 如果大于当前素数组标号，则返回   
			{   
				return;   
			}   
			if (n % nSmallPrimes[j][0]==0)  // 如果不是素数，则退出循环   
			{   
				break;   
			}   
		}   
		// 找到素数，设置素数   
		if (nSmallPrimes[j][1] > n)   
		{   
			nSmallPrimes[nPrimeCount][0] = n;   
			nSmallPrimes[nPrimeCount++][1] = n*n;   
		}   
	}   
}   

// 生成随机大数   
CBigNum GenerateBigRandomNumber(unsigned short nBytes)   
{   
	CBigNum Result=0U; // 初始化大数   
	int i;   
	clock_t ctStart;   
	unsigned long ctr=0;   

	// 设置时间间隔   
	clock_t ctInterval = CLOCKS_PER_SEC / 50 + 1;    

	for (i=0; i<nBytes*2; i++)   
	{   
		ctStart = clock();   
		// 等到大于时间间隔再开始   
		while (clock() - ctStart < ctInterval)   
			ctr++;   

		ctr = (ctr % 33) & 0xF;   

		Result <<= 4U; // 大数左移4位   
		Result |= ctr; // 做或运算   
	}   
	putchar('\n');   
	return Result; // 返回大数   
}   

CBigNum FindABigPrime(unsigned short nBytes)   
{   
	CBigNum nBig, nBig2;   
	DWORD j;   
	DWORD nTestCount = 0;   
	DWORD nLehmanCount = 0;    
	clock_t ctStartTime = clock(); // 记录开始时间   
	DWORD nOffset=0;   
	bool bPrime=false; // 素数标志位   

	// 开始寻找对应位数的大素数   
	for (nBig = GenerateBigRandomNumber(nBytes) | 1U; !bPrime; nBig+=2U, nOffset+=2)   
	{   
		nTestCount++;   
		for (j=0; j<nPrimeCount; j++)   
		{   
			// 如果不是大素数，则退出循环   
			if (nBig % nSmallPrimes[j][0] == 0)   
			{   
				break;   
			}   
		}   

		if (j<nPrimeCount)   
			continue;   
		nLehmanCount++;   
		nBig2 = (nBig - 1U) / 2U;   
		// 设置一些随机的素数   
		DWORD arnLehmanPrimes[] = { 89, 5179, 25981, 25439, 25013, 25667, 27397 };   
		// 初始化大数组   
		CBigNum LehmanResults[sizeof(arnLehmanPrimes) / sizeof(arnLehmanPrimes[0])];   
		nBig2 = nBig - 1U;   
		bPrime = true;   
		for (j=0; j<sizeof(arnLehmanPrimes) / sizeof(arnLehmanPrimes[0]); j++)   
		{   
			// 开始生成大素数   
			LehmanResults[j] =    
				CBigNum(arnLehmanPrimes[j]).PowMod(nBig2, nBig, CLOCKS_PER_SEC);   
			if (LehmanResults[j] == nBig2)   
			{   
			}   
			else if (LehmanResults[j] == 1U)   
			{   
			}   
			else // 不是大素数   
			{      
				bPrime = false;   
				break;   
			}   
		}   
		// 找到大素数   
		if (bPrime)    
		{   
			break;   
		}   
	}      
	return nBig; // 返回大素数   
}   

// 生成公钥和私钥   
void GenKeyPair(CBigNum &PublicMod, CBigNum &PublicKey, CBigNum &PrivateKey, CBigNum &P, CBigNum &Q, unsigned int nByteCount = 32)   
{   
	if (0U==(P | Q))   
	{   
		P=FindABigPrime(nByteCount); // 生成nByteCount位的大素数   
		Q=FindABigPrime(nByteCount); // 生成nByteCount位的大素数   
		PublicKey=GenerateBigRandomNumber(nByteCount) | 1U;   
	} else {   
		PublicKey |= 1U;   
	}   
	PrivateKey = (P-1U) * (Q-1U); // 初始化私钥   
	while (PublicKey > PrivateKey)   
		PublicKey=GenerateBigRandomNumber(nByteCount-1) | 1U;   
	while(CBigNum::gcd(PublicKey,PrivateKey) != 1U) // 生成公钥   
		PublicKey+=2; // 累加直至互素   
	PrivateKey = PublicKey.Inverse(PrivateKey); // 生成私钥   

	PublicMod = P*Q; // 公钥n的生成   
}   





// 生成特定格式的公钥和私钥   
void GenerateKeys(CBigNumString &PublicMod, CBigNumString &PublicKey, CBigNumString &PrivateKey, unsigned short nBytes)   
{   
	CBigNum PubMod, PubKey, PriKey, PriP, PriQ;   
	MakeSmallPrimes();   
	GenKeyPair(PubMod, PubKey, PriKey, PriP, PriQ, nBytes); // 生成公钥和私钥   
	// 将公钥和私钥转换成16进制形式   
	PublicMod = PubMod.ToHexString();    
	PublicKey = PubKey.ToHexString();   
	PrivateKey = PriKey.ToHexString();   
}   

//CBigNumString strMod, strPubKey, strPriKey; // 初始化大数字符变量   

// RSA加密函数   
void RSAEncrypt(char *publickey,char *publicmod, char *output, unsigned int *outputlen, char *input, unsigned int inputlen)   
{   
	CBigNum Transform;   
	CBigNum PubMod, PubKey;   
	CBigNumString strTransform;   
	// 将公钥转换成大数   
	PubMod = CBigNum::FromHexString(publicmod);   
	PubKey = CBigNum::FromHexString(publickey);    

	// 转换输入的明文   
	Transform = Transform.FromByteString(input);   
	// 使用RSA加密明文   
	Transform = Transform.PowMod(PubKey,PubMod);   
	// 将密文转换成16进制的字符   
	strTransform = Transform.ToHexString();   

	// 输出密文长度   
	*outputlen = strlen((const char*)strTransform)+1;   
	// 输出密文   
	memcpy(output,(const char*)strTransform,(*outputlen)+1);   
}   

// RSA解密   
void RSADecrypt(char *publickey,char *publicmod,char *output, unsigned int *outputlen, char *input, unsigned int inputlen)   
{   
	CBigNum Transform;   
	CBigNum PubMod,PriKey;   
	CBigNumString strTransform;   
	// 将私钥转换成大数   
	PubMod = CBigNum::FromHexString((const char*)publicmod);   
	PriKey = CBigNum::FromHexString((const char*)publickey);   
	// 转换输入的密文   
	Transform = Transform.FromHexString(input);   
	// 使用RSA对密文进行解密   
	Transform = Transform.PowMod(PriKey,PubMod);   
	// 将解密文转换成字节字符串   
	strTransform = Transform.ToByteString();   

	// 输出解密文长度   
	*outputlen = strlen((const char*)strTransform)+1;   
	// 输出解密文   
	memcpy(output,(const char*)strTransform,(*outputlen)+1);   


}  