#include <string.h>   
#include <stdio.h>   
#include <time.h>   
#include "bignum.h" 
#include "md5.h"
#include <iostream>
using namespace std;
#define MAXPRIMECOUNT 1000 // Ѱ��������������   
unsigned int nSmallPrimes[MAXPRIMECOUNT][2]; // С��������   
unsigned int nPrimeCount = 0; // Ѱ�������Ĵ���   

// Ѱ��С����   
void MakeSmallPrimes()   
{   
	unsigned int n;   
	unsigned int j;   
	nPrimeCount = 3;   
	// ���ȶ��弸��С����   
	nSmallPrimes[0][0] = 2;   
	nSmallPrimes[1][0] = 3;   
	nSmallPrimes[2][0] = 5;   
	nSmallPrimes[0][1] = 4;   
	nSmallPrimes[1][1] = 9;   
	nSmallPrimes[2][1] = 25;   
	// ��ʼѰ������   
	for (n=7; nPrimeCount < MAXPRIMECOUNT; n+=2)   
	{   
		for (j=0; nSmallPrimes[j][1] < n; j++)   
		{   
			if (j>= nPrimeCount) // ������ڵ�ǰ�������ţ��򷵻�   
			{   
				return;   
			}   
			if (n % nSmallPrimes[j][0]==0)  // ����������������˳�ѭ��   
			{   
				break;   
			}   
		}   
		// �ҵ���������������   
		if (nSmallPrimes[j][1] > n)   
		{   
			nSmallPrimes[nPrimeCount][0] = n;   
			nSmallPrimes[nPrimeCount++][1] = n*n;   
		}   
	}   
}   

// �����������   
CBigNum GenerateBigRandomNumber(unsigned short nBytes)   
{   
	CBigNum Result=0U; // ��ʼ������   
	int i;   
	clock_t ctStart;   
	unsigned long ctr=0;   

	// ����ʱ����   
	clock_t ctInterval = CLOCKS_PER_SEC / 50 + 1;    

	for (i=0; i<nBytes*2; i++)   
	{   
		ctStart = clock();   
		// �ȵ�����ʱ�����ٿ�ʼ   
		while (clock() - ctStart < ctInterval)   
			ctr++;   

		ctr = (ctr % 33) & 0xF;   

		Result <<= 4U; // ��������4λ   
		Result |= ctr; // ��������   
	}   
	putchar('\n');   
	return Result; // ���ش���   
}   

CBigNum FindABigPrime(unsigned short nBytes)   
{   
	CBigNum nBig, nBig2;   
	DWORD j;   
	DWORD nTestCount = 0;   
	DWORD nLehmanCount = 0;    
	clock_t ctStartTime = clock(); // ��¼��ʼʱ��   
	DWORD nOffset=0;   
	bool bPrime=false; // ������־λ   

	// ��ʼѰ�Ҷ�Ӧλ���Ĵ�����   
	for (nBig = GenerateBigRandomNumber(nBytes) | 1U; !bPrime; nBig+=2U, nOffset+=2)   
	{   
		nTestCount++;   
		for (j=0; j<nPrimeCount; j++)   
		{   
			// ������Ǵ����������˳�ѭ��   
			if (nBig % nSmallPrimes[j][0] == 0)   
			{   
				break;   
			}   
		}   

		if (j<nPrimeCount)   
			continue;   
		nLehmanCount++;   
		nBig2 = (nBig - 1U) / 2U;   
		// ����һЩ���������   
		DWORD arnLehmanPrimes[] = { 89, 5179, 25981, 25439, 25013, 25667, 27397 };   
		// ��ʼ��������   
		CBigNum LehmanResults[sizeof(arnLehmanPrimes) / sizeof(arnLehmanPrimes[0])];   
		nBig2 = nBig - 1U;   
		bPrime = true;   
		for (j=0; j<sizeof(arnLehmanPrimes) / sizeof(arnLehmanPrimes[0]); j++)   
		{   
			// ��ʼ���ɴ�����   
			LehmanResults[j] =    
				CBigNum(arnLehmanPrimes[j]).PowMod(nBig2, nBig, CLOCKS_PER_SEC);   
			if (LehmanResults[j] == nBig2)   
			{   
			}   
			else if (LehmanResults[j] == 1U)   
			{   
			}   
			else // ���Ǵ�����   
			{      
				bPrime = false;   
				break;   
			}   
		}   
		// �ҵ�������   
		if (bPrime)    
		{   
			break;   
		}   
	}      
	return nBig; // ���ش�����   
}   

// ���ɹ�Կ��˽Կ   
void GenKeyPair(CBigNum &PublicMod, CBigNum &PublicKey, CBigNum &PrivateKey, CBigNum &P, CBigNum &Q, unsigned int nByteCount = 32)   
{   
	if (0U==(P | Q))   
	{   
		P=FindABigPrime(nByteCount); // ����nByteCountλ�Ĵ�����   
		Q=FindABigPrime(nByteCount); // ����nByteCountλ�Ĵ�����   
		PublicKey=GenerateBigRandomNumber(nByteCount) | 1U;   
	} else {   
		PublicKey |= 1U;   
	}   
	PrivateKey = (P-1U) * (Q-1U); // ��ʼ��˽Կ   
	while (PublicKey > PrivateKey)   
		PublicKey=GenerateBigRandomNumber(nByteCount-1) | 1U;   
	while(CBigNum::gcd(PublicKey,PrivateKey) != 1U) // ���ɹ�Կ   
		PublicKey+=2; // �ۼ�ֱ������   
	PrivateKey = PublicKey.Inverse(PrivateKey); // ����˽Կ   

	PublicMod = P*Q; // ��Կn������   
}   





// �����ض���ʽ�Ĺ�Կ��˽Կ   
void GenerateKeys(CBigNumString &PublicMod, CBigNumString &PublicKey, CBigNumString &PrivateKey, unsigned short nBytes)   
{   
	CBigNum PubMod, PubKey, PriKey, PriP, PriQ;   
	MakeSmallPrimes();   
	GenKeyPair(PubMod, PubKey, PriKey, PriP, PriQ, nBytes); // ���ɹ�Կ��˽Կ   
	// ����Կ��˽Կת����16������ʽ   
	PublicMod = PubMod.ToHexString();    
	PublicKey = PubKey.ToHexString();   
	PrivateKey = PriKey.ToHexString();   
}   

//CBigNumString strMod, strPubKey, strPriKey; // ��ʼ�������ַ�����   

// RSA���ܺ���   
void RSAEncrypt(char *publickey,char *publicmod, char *output, unsigned int *outputlen, char *input, unsigned int inputlen)   
{   
	CBigNum Transform;   
	CBigNum PubMod, PubKey;   
	CBigNumString strTransform;   
	// ����Կת���ɴ���   
	PubMod = CBigNum::FromHexString(publicmod);   
	PubKey = CBigNum::FromHexString(publickey);    

	// ת�����������   
	Transform = Transform.FromByteString(input);   
	// ʹ��RSA��������   
	Transform = Transform.PowMod(PubKey,PubMod);   
	// ������ת����16���Ƶ��ַ�   
	strTransform = Transform.ToHexString();   

	// ������ĳ���   
	*outputlen = strlen((const char*)strTransform)+1;   
	// �������   
	memcpy(output,(const char*)strTransform,(*outputlen)+1);   
}   

// RSA����   
void RSADecrypt(char *publickey,char *publicmod,char *output, unsigned int *outputlen, char *input, unsigned int inputlen)   
{   
	CBigNum Transform;   
	CBigNum PubMod,PriKey;   
	CBigNumString strTransform;   
	// ��˽Կת���ɴ���   
	PubMod = CBigNum::FromHexString((const char*)publicmod);   
	PriKey = CBigNum::FromHexString((const char*)publickey);   
	// ת�����������   
	Transform = Transform.FromHexString(input);   
	// ʹ��RSA�����Ľ��н���   
	Transform = Transform.PowMod(PriKey,PubMod);   
	// ��������ת�����ֽ��ַ���   
	strTransform = Transform.ToByteString();   

	// ��������ĳ���   
	*outputlen = strlen((const char*)strTransform)+1;   
	// ���������   
	memcpy(output,(const char*)strTransform,(*outputlen)+1);   


}  