/*	SCCS Id: @(#)attrib.h	3.2	90/22/02	*/
/* Copyright 1988, Mike Stephenson				  */
/* NetHack may be freely redistributed.  See license for details. */

/*	attrib.h - Header file for character class processing. */

#ifndef ATTRIB_H
#define ATTRIB_H

#define A_STR	0
#define A_INT	1
#define A_WIS	2
#define A_DEX	3
#define A_CON	4
#define A_CHA	5

#define A_MAX	6	/* used in rn2() selection of attrib */

#define ABASE(x)	(u.acurr.a[x])
#define ABON(x)		(u.abon.a[x])
#define AEXE(x)		(u.aexe.a[x])
#define ACURR(x)	(acurr(x))
#define ACURRSTR	(acurrstr())
/* should be: */
/* #define ACURR(x) (ABON(x) + ATEMP(x) + (u.umonnum == -1) ? ABASE(x) : MBASE(x)) */
#define MCURR(x)	(u.macurr.a[x])
#define AMAX(x)		(u.amax.a[x])
#define MMAX(x)		(u.mamax.a[x])

#define ATEMP(x)	(u.atemp.a[x])
#define ATIME(x)	(u.atime.a[x])

struct	attribs {
	schar	a[A_MAX];
};

extern struct attribs class_a, class_b, class_c, class_d, class_e, class_f, 
                      class_g, class_h, class_i, class_j, class_k, class_l,
                      class_m, class_n, class_p, class_r, class_s, class_t,
                      class_u, class_v, class_w, attrmin;

#define CLASS_A(x) (class_a.a[x])
#define CLASS_B(x) (class_b.a[x])
#define CLASS_C(x) (class_c.a[x])
#define CLASS_D(x) (class_d.a[x])
#define CLASS_E(x) (class_e.a[x])
#define CLASS_F(x) (class_f.a[x])
#define CLASS_G(x) (class_g.a[x])
#define CLASS_H(x) (class_h.a[x])
#define CLASS_I(x) (class_i.a[x])
#define CLASS_J(x) (class_j.a[x])
#define CLASS_K(x) (class_k.a[x])
#define CLASS_L(x) (class_l.a[x])
#define CLASS_M(x) (class_m.a[x])
#define CLASS_N(x) (class_n.a[x])
#define CLASS_P(x) (class_p.a[x])
#define CLASS_R(x) (class_r.a[x])
#define CLASS_S(x) (class_s.a[x])
#define CLASS_T(x) (class_t.a[x])
#define CLASS_U(x) (class_u.a[x])
#define CLASS_V(x) (class_v.a[x])
#define CLASS_W(x) (class_w.a[x])
#define ATTRMIN(x) (attrmin.a[x])

#endif /* ATTRIB_H */
