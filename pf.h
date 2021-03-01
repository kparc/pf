//! nolibc printf(3), copyright (c) 2020 regents of kparc, bsd-2-clause

#pragma once
#ifndef PF_H
#define PF_H

#define PFV1 2021       //!< major
#define PFV2 02         //!< minor
#define PFV3 14         //!< patch
#define PFL "(c) 2020 kparc / bsd"

#ifndef NOPF
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wint-conversion"
#pragma GCC diagnostic ignored "-Wpedantic"

#define printf pf   //!< use pf(f,a..)

#if defined(__TINYC__)
#ifndef PFMX
#define PFMX 8   //!< tcc max argcount, up to 16
#pragma message("PFMX is mandatory for tcc, defaulting to 8")
#endif
#define PFSZ PFMX
#else
#define PFMX 0      //!< for gcc/clang, argcount is unlimited
#define PFSZ
#endif

#ifndef PFCH
#define PFCH 0      //!< if f<128, treat it as char (transmit and return 1)     (!posix)
#endif

#include"_.h"
#include<unistd.h> //!< write(2)

#if NOLC            //!< satisfy minimal freestanding environment
//#include<string.h>
#define strlen slen
_*memset(_*d, I c, size_t n)        {S s=(S)d;W(n--)*s++=c;R d;}
_*memcpy(_*d, const _*s, size_t n)  {S x=(S)d,y=(S)s;W(n--)*x++=*y++;R d;}
_ exit(I);
#endif

//! modify below this line at your own risk.

typedef union{UJ uj;}arg;typedef arg args[PFSZ];                                    //!< use union to avoid gcc bug
#define PU(fn,t) ZZ S fn(S s,t i){t j;do*--s='0'+i-10*(j=i/10);W(i=j);R s;}         //!< parse signed/unsigned
#define TU(fn,t,u) ZZ I fn(t x,I f,I p,I l){I n;S r=jS(x,&n,u);R txp(r,n,PLR,PCH);} //!< tramsmit signed/unsigned
#define FG(f)  (1u<<f)               //!< flag bit
#define fC(fb) ((f&FG(fb))==FG(fb))  //!< check flag bit in f
#define PCH    (" 0"[fC(2)&&!fC(1)]) //!< zero padding (nop in rpad mode)
#define PLR    (fC(1)?-p:p)          //!< pad l/r

//! strlen [u]ltoa ato[u]l hex
ZG xb[26];ZI slen(char*s){I r=0;W(*s++)r++;R r;}ZS ng(S s){R*--s='-',s;}PU(pj,J)PU(pu,UJ);
ZS jS(UJ i,I*n,G u){S r=(!u&&0>(J)i)?ng(jS(-i,n,u)):u?pu(xb+25,i):pj(xb+25,i);R*n=25+(xb-r),r;}
UI sI(char*a,I*n){G c;UI i=0,r=*n=0;W((c=*a++)&&IN('0',c,'9'))i++,r=r*10u+((UI)c-'0');R*n=i,r;}ZS hh(S s,G c);
ZS hh(S s,G c){N(2,G a=i?c&15:c>>4;s[i]="0W"[9<a]+a)R s;}ZI jX(UJ j){S s=xb+25;UJ k=j;do hh(s-=2,k);W(k>>=8);R 25-(s-xb);}

//! (tx)byte (txn)bytes (txN)times (b)yte (i)nt he(x) (s)tr (p)ad  (f)lags (p)ad/width (l)ength/prec
ZI tx(G c){R write(1,&c,1);}ZI txn(S x,I n){P(!n,n)N(n,tx(x[i]));R n;}ZI txN(G c,I n){N(n,tx(c))R n;}
ZI txp(S x,I n,I p,G pc){R txN(pc,MX(0,p-n))+txn(x,n)+txN(pc,ABS(MN(0,p+n)));}ZI txb(G c,I f,I p,I l){R txp(&c,1,PLR,PCH);}
ZI txx(UJ j,I f,I p,I l){I n=jX(j)+2;S b=xb+25-n;*b='0',b[1]='x';R txp(b,n,PLR,PCH);}TU(txj,J,0)TU(txu,UJ,1)
ZI txs(char*x,I f,I p,I l){R txp((S)x,l?l:slen(x),PLR,PCH);}

#ifdef __TINYC__
#define nrg(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,N,...) N
#define NRG(...) nrg(__VA_ARGS__, 16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1)
#define pf(f,a...) ({args pfa={a};txpf(f,pfa,NRG(a));})    //!< coerce arguments of pf() to a stack array of unsigned long longs
#else
#define pf(f,a...) ({args pfa={a};txpf(f,pfa,CNT(pfa));})  //!< coerce arguments of pf() to a stack array of unsigned long longs
#endif
#define vnx(r)     r=a[i++].uj                             //!< retrieve next positional arg
#if PFMX
#define varg(r)    P(PFMX==i,n)vnx(r)                      //!< check PFMX overflow
#else
#define varg(r)    vnx(r)
#endif
#define vtx(f,a)   n+=f(a,flg,flw,prc);
#define va(c,t,f)  C(c,{t _a;varg(_a);vtx(f,_a)})  //!< call f((type)nextarg, options)
#define Cf(c,fl)   C(c,flg|=FG(fl),f++)            //!< set format bit 1|2|3 (0pad,lpad,#alt)
#define echo       n+=tx(c);continue               //!< tx single byte and advance
#define dbg(s,i)   (txs(s,0,0,0),txj(i,0,0,0))

//! %[%-][09..][.09..*]dcups
I txpf(char*f,args a,I ac){P(!f,f)             //!< (f)ormat string (aka tape), (a)rguments, (a)rg(c)ount
 P(PFCH&&(char*)128>f,tx(*(G*)&f))             //!< optional char check for f, see PFCH
 G c;I j,i=0,n=0;                              //!< total le(n)gth, arg(i)ndex, curr(c)har
 UI flg,flw,prc;                               //!< fmt flags, field width, precision
 W(c=*f++){                                    //!< while more chars left on tape:
  Z('%'-c,echo)Z('%'==*f,f++,echo)             //!< echo c until first %, %% is literal %, otherwise:
  flg=prc=0,j=1;                               //!< reset state, then:
  W(j)SW(c=*f,Cf('-',1)Cf('0',2)Cf('#',3),j=0) //!< scan format flags (%[-0#])
  flw=sI(f,&j),f+=j,c=*f;                      //!< scan field width (%flw)
  Z('.'==c,prc=sI(++f,&j);f+=j;c=*f;           //!< scan precision (%.prc)
   Z(!j,Z('*'-c,f++;continue)                  //!< invalid precision is empty field
    c=*++f;varg(prc)))c=*f;                    //!< scan positional precision (%.*)
  W('l'==c||'h'==c)c=*++f;                     //!< skip [lh..]
  Z(ac==i,vtx(txs,"(null)"))                   //!< print (null) on argc overflow, otherwise:
  //! flush  (G)byte   (J)longint   (U)nsignedlongint   he(x)   (s)tring
  SW(c,va('c',G,txb)va('d',J,txj)va('u',UJ,txu)va('p',UJ,txx)va('s',char*,txs)) 
  f++;}R n;}

#pragma GCC diagnostic pop
#else
#include<stdio.h>                              //!< stock printf(3)
#include<stdlib.h>			       //!< exit(2)
#endif//NOPF

#endif//PF_H

//:~
