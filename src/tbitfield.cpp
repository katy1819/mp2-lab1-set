// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

#define BITS_IN_ONE_MEM (sizeof(TELEM) * 8)

TBitField::TBitField(int len)
{
  if (len < 0)
    throw "negative length";
  BitLen = len;
  MemLen = BitLen/BITS_IN_ONE_MEM;
  if(BitLen%BITS_IN_ONE_MEM!=0)
	  MemLen+=1;
  pMem = new TELEM[MemLen]; 
  // обнуление
  for (int i = 0; i < MemLen; i++)
	  pMem[i] = 0;
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
	BitLen = bf.BitLen;
	MemLen = bf.MemLen;
	pMem = new TELEM[MemLen];
	for( int i=0; i<MemLen; i++)
		pMem[i]=bf.pMem[i];
}

TBitField::~TBitField()
{
	delete [] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
	int index = n/BITS_IN_ONE_MEM;
	return index;
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
	TELEM mask = 1<<(n%BITS_IN_ONE_MEM);
  return mask;
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
	return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
	if ((n < 0) || (n>BitLen)) 
    throw "negative";
	pMem[GetMemIndex(n)]|= GetMemMask(n);
}

void TBitField::ClrBit(const int n) // очистить бит
{
	if ((n < 0) || (n>BitLen))
    throw "negative";
	TELEM mask=GetMemMask(n);
	int index = GetMemIndex(n);
	pMem[index]&=~mask;
}

int TBitField::GetBit(const int n) const // получить значение бита
{
	if ((n < 0) || (n>BitLen))
    throw "negative";
	int res = pMem[GetMemIndex(n)]&GetMemMask(n);
	if(res>0)
		return 1;
	else
		return 0;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
	if (this==&bf)
		return *this;
	if (MemLen != bf.MemLen)
	{
		delete [] pMem;
		pMem = new TELEM [bf.MemLen];
	}
	MemLen = bf.MemLen;
	BitLen = bf.BitLen;
	for(int i=0; i<MemLen; i++)
		pMem[i]=bf.pMem[i];
  return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
	if(MemLen != bf.MemLen)
		return 0;
	else if(BitLen != bf.BitLen)
		return 0;
	else
	{
		for( int i=0; i<MemLen; i++)
			if(pMem[i] != bf.pMem[i])
				return 0;
		  return 1;
	}
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
	int res = (*this == bf);
	return !res;
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
	int max=(BitLen<bf.BitLen)? bf.BitLen: BitLen;
	int min = (MemLen>bf.MemLen)? bf.MemLen: MemLen;
	TBitField res(max);
	for(int i=0; i < min; i++)
		res.pMem[i]=pMem[i]|bf.pMem[i];

	if (max == bf.BitLen)
		for(int i= min; i<res.MemLen; i++)
			res.pMem[i] = bf.pMem[i];
	else
		for(int i= min; i<res.MemLen; i++)
			res.pMem[i] = pMem[i];
	return res;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
	int min=(BitLen>bf.BitLen)? bf.BitLen: BitLen;
	TBitField res(min);
	for(int i=0; i<res.MemLen; i++)
		res.pMem[i]=pMem[i]&bf.pMem[i];
	/* for(int i=(res.MemLen-1)*BITS_IN_ONE_MEM; i<res.BitLen; i++)
	{
		int a = GetBit(i);
		int b = bf.GetBit(i);
		if((a+b)==2)
			res.SetBit(i);
	} */
  return res;
}

TBitField TBitField::operator~(void) // отрицание
{
	TBitField res(BitLen);
	for (int i=0; i<res.MemLen-1; i++)
		res.pMem[i] = ~pMem[i];
	for(int i=(res.MemLen-1)*BITS_IN_ONE_MEM; i<res.BitLen; i++)
	{
		int a = GetBit(i);
		if(a==0)
			res.SetBit(i);
		else
			res.ClrBit(i);
	}
  return res; 
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
	int i=0;
	char ch;
	do
	{
		istr>>ch;
	}
	while (ch!=' ');
	while (1)
	{
		istr>>ch;
		if(ch=='0')
			bf.ClrBit(i++);
		else 
			if(ch=='1')
				bf.SetBit(i++);
			else
				break;
	}
	return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
	int len = bf.GetLength();
	for(int i=0; i<len; i++)
		if(bf.GetBit(i))
			ostr<<'1';
		else ostr << '1';
  return ostr;
}
