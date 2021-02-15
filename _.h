//! better c, public domain, courtesy of kparc

#pragma once
#ifndef ATWC_H
#define ATWC_H

#include<unistd.h>  //!< size_t

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wvariadic-macros"
#pragma GCC diagnostic ignored "-Wpedantic"                      //<! allow braced groups

typedef void _;typedef unsigned char*S,G;typedef unsigned short H;typedef int I;typedef unsigned long long UJ,K;typedef long long J;typedef unsigned int UI;

#define  R                return                                 //!< usually K.
#define  W(a...)          while(({a;}))                          //!< while is fine.
#define  N(n,a...)        {I _n=(n),i=-1;W(++i<_n){a;}}          //!< no stinking loops.
#define Nj(n,a...)        {I _n=(n),j=-1;W(++j<_n){a;}}          //!< staple inner loop i,j
#define _N(n,a...)        {I i=(n);W(i--){a;}}                   //!< walk something backwards.
#define N1(n,a...)        {I i=0,_n=(n);do{a;}while(++i<_n);}    //!< do at least one iteration.
#define  P(b,a...)        if(b)R({a;});                          //!< "bail early" predicate aka panic.
#define  Z(b,a...)        if(b){a;}else                          //!< be sure to terminate dangling else.
#define  C(i,a...)        case i:{a;}break;                      //!< case statement without a forgotten break.
#define SW(i,c,a...)      switch(i){c default:a;}                //!< default case is separated by a single comma.
#define SZ                sizeof

//! avoid explicit ops as much as possible:
#define MN(x,y)           ((y)>(x)?(x):(y))
#define MX(x,y)           ((y)>(x)?(y):(x))
#define IN(l,x,r)         ((l)<=(x)&&(x)<=(r))
#define CNT(x)            ((SZ(x)/SZ(0[x]))/((size_t)(!(SZ(x)%SZ(0[x])))))

//! limit scope as much as you can:
#define ZG                static G
#define ZH                static G
#define ZI                static I
#define ZS                static S
#define ZJ                static J
#define ZK                static K
#define Z_                static _
#define ZZ                static inline

//! math
#if (__x86_64__||i386)&&(!__TINYC__)
#define clz(n)            (__builtin_clz((UI)n))    //!< count leading zeros
#define clzl(n)           (__builtin_clzl((UJ)n))
#else
#define __(n)             i|=i>>n;
#define npw2(n)         __(1)__(2)__(4)__(8)__(16)  //!< round n up to the next power of 2
ZI dbj[32] =              {0,9,1,10,13,21,2,29,11,14,16,18,22,25,3,30,8,12,20,28,15,17,24,7,19,27,23,6,26,5,4,31};
static inline             I __lg2(UI i){npw2(i);R dbj[i*0x07C4ACDDU>>27];}
#define clz(i)            (31-__lg2((UI)i))
#define clzl(i)           (63-__lg2((UI)i))
#endif
static UI p10[] =         {1,10,100,1000,10000,100000,1000000,10000000,100000000,1000000000};
static inline             I lg10(UI i){I t=(32-clz(i)+1)*1233>>12;R t-(i<p10[t]);}

#define ABS(e)            ((0>(e))?(-(e)):(e))

#if NOLIBC
ZS mcp(S d,S s,I n)       {W(n--)*d++=*s++;R d;}
ZS memmove(S d,S s,I n)   {P(s<d&&d<s+n,d+=n,s+=n;W(n--)*--d=*--s;d)R mcp(d,s,n);}
ZS memset(S s,I c,I n)    {N(n,s[n]=c);R s;}
#define mmv memmove
#define mset memset
#endif

//#pragma GCC diagnostic pop
#endif//ATWC_H
//:~
