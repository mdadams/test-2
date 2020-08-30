/*
 * Copyright (c) 1999-2000 Image Power, Inc. and the University of
 *   British Columbia.
 * Copyright (c) 2001-2002 Michael David Adams.
 * All rights reserved.
 */

/* __START_OF_JASPER_LICENSE__
 * 
 * JasPer License Version 2.0
 * 
 * Copyright (c) 2001-2006 Michael David Adams
 * Copyright (c) 1999-2000 Image Power, Inc.
 * Copyright (c) 1999-2000 The University of British Columbia
 * 
 * All rights reserved.
 * 
 * Permission is hereby granted, free of charge, to any person (the
 * "User") obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, and/or sell copies of the Software, and to permit
 * persons to whom the Software is furnished to do so, subject to the
 * following conditions:
 * 
 * 1.  The above copyright notices and this permission notice (which
 * includes the disclaimer below) shall be included in all copies or
 * substantial portions of the Software.
 * 
 * 2.  The name of a copyright holder shall not be used to endorse or
 * promote products derived from the Software without specific prior
 * written permission.
 * 
 * THIS DISCLAIMER OF WARRANTY CONSTITUTES AN ESSENTIAL PART OF THIS
 * LICENSE.  NO USE OF THE SOFTWARE IS AUTHORIZED HEREUNDER EXCEPT UNDER
 * THIS DISCLAIMER.  THE SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS
 * "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
 * BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE AND NONINFRINGEMENT OF THIRD PARTY RIGHTS.  IN NO
 * EVENT SHALL THE COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, OR ANY SPECIAL
 * INDIRECT OR CONSEQUENTIAL DAMAGES, OR ANY DAMAGES WHATSOEVER RESULTING
 * FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT,
 * NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION
 * WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.  NO ASSURANCES ARE
 * PROVIDED BY THE COPYRIGHT HOLDERS THAT THE SOFTWARE DOES NOT INFRINGE
 * THE PATENT OR OTHER INTELLECTUAL PROPERTY RIGHTS OF ANY OTHER ENTITY.
 * EACH COPYRIGHT HOLDER DISCLAIMS ANY LIABILITY TO THE USER FOR CLAIMS
 * BROUGHT BY ANY OTHER ENTITY BASED ON INFRINGEMENT OF INTELLECTUAL
 * PROPERTY RIGHTS OR OTHERWISE.  AS A CONDITION TO EXERCISING THE RIGHTS
 * GRANTED HEREUNDER, EACH USER HEREBY ASSUMES SOLE RESPONSIBILITY TO SECURE
 * ANY OTHER INTELLECTUAL PROPERTY RIGHTS NEEDED, IF ANY.  THE SOFTWARE
 * IS NOT FAULT-TOLERANT AND IS NOT INTENDED FOR USE IN MISSION-CRITICAL
 * SYSTEMS, SUCH AS THOSE USED IN THE OPERATION OF NUCLEAR FACILITIES,
 * AIRCRAFT NAVIGATION OR COMMUNICATION SYSTEMS, AIR TRAFFIC CONTROL
 * SYSTEMS, DIRECT LIFE SUPPORT MACHINES, OR WEAPONS SYSTEMS, IN WHICH
 * THE FAILURE OF THE SOFTWARE OR SYSTEM COULD LEAD DIRECTLY TO DEATH,
 * PERSONAL INJURY, OR SEVERE PHYSICAL OR ENVIRONMENTAL DAMAGE ("HIGH
 * RISK ACTIVITIES").  THE COPYRIGHT HOLDERS SPECIFICALLY DISCLAIM ANY
 * EXPRESS OR IMPLIED WARRANTY OF FITNESS FOR HIGH RISK ACTIVITIES.
 * 
 * __END_OF_JASPER_LICENSE__
 */

/*
 * $Id$
 */

#ifndef JPC_T1COD_H
#define JPC_T1COD_H

/******************************************************************************\
* Includes.
\******************************************************************************/

#include "jpc_fix.h"
#include "jpc_mqcod.h"
#include "jpc_tsfb.h"
#include "jasper/jas_math.h"

/******************************************************************************\
* Constants.
\******************************************************************************/

/* The number of bits used to index into various lookup tables. */
#define JPC_NMSEDEC_BITS	7
#define JPC_NMSEDEC_FRACBITS	(JPC_NMSEDEC_BITS - 1)

/*
 * Segment types.
 */

enum jpc_segtype {
	/** Invalid. */
	JPC_SEG_INVALID,

	/* MQ. */
	JPC_SEG_MQ,

	/* Raw. */
	JPC_SEG_RAW,
};

/* The nominal word size. */
#define	JPC_PREC	32

/* Tier-1 coding pass types. */
enum jpc_passtype {
	JPC_SIGPASS, /*< significance */
	JPC_REFPASS, /*< refinement */
	JPC_CLNPASS, /*< cleanup */
};

/*
 * Per-sample state information for tier-1 coding.
 */

/* The northeast neighbour has been found to be significant. */
#define	JPC_NESIG	0x0001
/* The southeast neighbour has been found to be significant. */
#define	JPC_SESIG	0x0002
/* The southwest neighbour has been found to be significant. */
#define	JPC_SWSIG	0x0004
/* The northwest neighbour has been found to be significant. */
#define	JPC_NWSIG	0x0008
/* The north neighbour has been found to be significant. */
#define	JPC_NSIG	0x0010
/* The east neighbour has been found to be significant. */
#define	JPC_ESIG	0x0020
/* The south neighbour has been found to be significant. */
#define	JPC_SSIG	0x0040
/* The west neighbour has been found to be significant. */
#define	JPC_WSIG	0x0080
/* The significance mask for 8-connected neighbours. */
#define	JPC_OTHSIGMSK \
	(JPC_NSIG | JPC_NESIG | JPC_ESIG | JPC_SESIG | JPC_SSIG | JPC_SWSIG | JPC_WSIG | JPC_NWSIG)
/* The significance mask for 4-connected neighbours. */
#define	JPC_PRIMSIGMSK	(JPC_NSIG | JPC_ESIG | JPC_SSIG | JPC_WSIG)

/* The north neighbour is negative in value. */
#define	JPC_NSGN	0x0100
/* The east neighbour is negative in value. */
#define	JPC_ESGN	0x0200
/* The south neighbour is negative in value. */
#define	JPC_SSGN	0x0400
/* The west neighbour is negative in value. */
#define	JPC_WSGN	0x0800
/* The sign mask for 4-connected neighbours. */
#define	JPC_SGNMSK	(JPC_NSGN | JPC_ESGN | JPC_SSGN | JPC_WSGN)

/* This sample has been found to be significant. */
#define JPC_SIG		0x1000
/* The sample has been refined. */
#define	JPC_REFINE	0x2000
/* This sample has been processed during the significance pass. */
#define	JPC_VISIT	0x4000

/* The number of aggregation contexts. */
#define	JPC_NUMAGGCTXS	1
/* The number of zero coding contexts. */
#define	JPC_NUMZCCTXS	9
/* The number of magnitude contexts. */
#define	JPC_NUMMAGCTXS	3
/* The number of sign coding contexts. */
#define	JPC_NUMSCCTXS	5
/* The number of uniform contexts. */
#define	JPC_NUMUCTXS	1

/* The context ID for the first aggregation context. */
#define	JPC_AGGCTXNO	0
/* The context ID for the first zero coding context. */
#define	JPC_ZCCTXNO		(JPC_AGGCTXNO + JPC_NUMAGGCTXS)
/* The context ID for the first magnitude context. */
#define	JPC_MAGCTXNO	(JPC_ZCCTXNO + JPC_NUMZCCTXS)
/* The context ID for the first sign coding context. */
#define	JPC_SCCTXNO		(JPC_MAGCTXNO + JPC_NUMMAGCTXS)
/* The context ID for the first uniform context. */
#define	JPC_UCTXNO		(JPC_SCCTXNO + JPC_NUMSCCTXS)
/* The total number of contexts. */
#define	JPC_NUMCTXS		(JPC_UCTXNO + JPC_NUMUCTXS)

/******************************************************************************\
* External data.
\******************************************************************************/

/* These lookup tables are used by various macros/functions. */
/* Do not access these lookup tables directly. */
extern uint_least8_t jpc_zcctxnolut[];
extern bool jpc_spblut[];
extern uint_least8_t jpc_scctxnolut[];
extern uint_least8_t jpc_magctxnolut[];
extern jpc_fix_t jpc_refnmsedec[];
extern jpc_fix_t jpc_signmsedec[];
extern jpc_fix_t jpc_refnmsedec0[];
extern jpc_fix_t jpc_signmsedec0[];

/* The initial settings for the MQ contexts. */
extern jpc_mqctx_t jpc_mqctxs[];

/******************************************************************************\
* Functions and macros.
\******************************************************************************/

/* Arithmetic shift right (with ability to shift left also). */
JAS_ATTRIBUTE_CONST
static inline jpc_fix_t JPC_ASR(jpc_fix_t x, int n)
{
	return n >= 0
		? x >> n
		: x << -n;
}

/* Get the zero coding context. */
JAS_ATTRIBUTE_CONST
static inline uint_least8_t JPC_GETZCCTXNO(unsigned f, enum jpc_tsfb_orient orient)
{
	return jpc_zcctxnolut[((unsigned)orient << 8) | (f & JPC_OTHSIGMSK)];
}

/* Get the sign prediction bit. */
JAS_ATTRIBUTE_CONST
static inline bool JPC_GETSPB(unsigned f)
{
	return jpc_spblut[(f & (JPC_PRIMSIGMSK | JPC_SGNMSK)) >> 4];
}

/* Get the sign coding context. */
JAS_ATTRIBUTE_CONST
static inline uint_least8_t JPC_GETSCCTXNO(unsigned f)
{
	return jpc_scctxnolut[(f & (JPC_PRIMSIGMSK | JPC_SGNMSK)) >> 4];
}

/* Get the magnitude context. */
JAS_ATTRIBUTE_CONST
static inline uint_least8_t JPC_GETMAGCTXNO(unsigned f)
{
	return jpc_magctxnolut[(f & JPC_OTHSIGMSK) | (((f & JPC_REFINE) != 0) << 11)];
}

/* Get the normalized MSE reduction for significance passes. */
JAS_ATTRIBUTE_CONST
static inline jpc_fix_t JPC_GETSIGNMSEDEC(jpc_fix_t x, int bitpos)
{
	return bitpos > JPC_NMSEDEC_FRACBITS
		? jpc_signmsedec[JPC_ASR(x, bitpos - JPC_NMSEDEC_FRACBITS) & JAS_ONES(JPC_NMSEDEC_BITS)]
		: jpc_signmsedec0[JPC_ASR(x, bitpos - JPC_NMSEDEC_FRACBITS) & JAS_ONES(JPC_NMSEDEC_BITS)];
}

/* Get the normalized MSE reduction for refinement passes. */
JAS_ATTRIBUTE_CONST
static inline jpc_fix_t JPC_GETREFNMSEDEC(jpc_fix_t x, int bitpos)
{
	return bitpos > JPC_NMSEDEC_FRACBITS
		? jpc_refnmsedec[JPC_ASR(x, bitpos - JPC_NMSEDEC_FRACBITS) & JAS_ONES(JPC_NMSEDEC_BITS)]
		: jpc_refnmsedec0[JPC_ASR(x, bitpos - JPC_NMSEDEC_FRACBITS) & JAS_ONES(JPC_NMSEDEC_BITS)];
}

/* Update the per-sample state information. */
static inline void JPC_UPDATEFLAGS4(jpc_fix_t *fp, unsigned rowstep, bool s, bool vcausalflag)
{
	jpc_fix_t *np = fp - rowstep;
	jpc_fix_t *sp = fp + rowstep;
	if (vcausalflag) {
		sp[-1] |= JPC_NESIG;
		sp[1] |= JPC_NWSIG;
		if (s) {
			*sp |= JPC_NSIG | JPC_NSGN;
			fp[-1] |= JPC_ESIG | JPC_ESGN;
			fp[1] |= JPC_WSIG | JPC_WSGN;
		} else {
			*sp |= JPC_NSIG;
			fp[-1] |= JPC_ESIG;
			fp[1] |= JPC_WSIG;
		}
	} else {
		np[-1] |= JPC_SESIG;
		np[1] |= JPC_SWSIG;
		sp[-1] |= JPC_NESIG;
		sp[1] |= JPC_NWSIG;
		if (s) {
			*np |= JPC_SSIG | JPC_SSGN;
			*sp |= JPC_NSIG | JPC_NSGN;
			fp[-1] |= JPC_ESIG | JPC_ESGN;
			fp[1] |= JPC_WSIG | JPC_WSGN;
		} else {
			*np |= JPC_SSIG;
			*sp |= JPC_NSIG;
			fp[-1] |= JPC_ESIG;
			fp[1] |= JPC_WSIG;
		}
	}
}

/* Initialize the lookup tables used by the codec. */
void jpc_initluts(void);

/* Get the nominal gain associated with a particular band. */
JAS_ATTRIBUTE_CONST
unsigned JPC_NOMINALGAIN(unsigned qmfbid, unsigned numlvls, unsigned lvlno, enum jpc_tsfb_orient orient);

/* Get the coding pass type. */
JAS_ATTRIBUTE_CONST
enum jpc_passtype JPC_PASSTYPE(unsigned passno);

/* Get the segment type. */
JAS_ATTRIBUTE_CONST
enum jpc_segtype JPC_SEGTYPE(unsigned passno, unsigned firstpassno, bool bypass);

/* Get the number of coding passess in the segment. */
JAS_ATTRIBUTE_CONST
unsigned JPC_SEGPASSCNT(unsigned passno, unsigned firstpassno, unsigned numpasses, bool bypass,
  bool termall);

/* Is the coding pass terminated? */
JAS_ATTRIBUTE_CONST
bool JPC_ISTERMINATED(unsigned passno, unsigned firstpassno, unsigned numpasses, bool termall,
  bool lazy);

#endif
