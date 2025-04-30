/*	SCCS Id: @(#)attrib.c	3.2	96/06/16	*/
/*	Copyright 1988, 1989, 1990, 1992, M. Stephenson		  */
/* NetHack may be freely redistributed.  See license for details. */

/*
**	Japanese version Copyright
**	(c) Issei Numata, Naoki Hamada, Shigehiro Miyashita, 1994-1996
**	changing point is marked `JP' (94/6/7)
**	JNetHack may be freely redistributed.  See license for details. 
*/


/*  attribute modification routines. */

#include "hack.h"
#include "artifact.h"

/* #define DEBUG	/* uncomment for debugging info */

#ifdef OVLB

	/* part of the output on gain or loss of attribute */
static
const char	*plusattr[] = {
/*JP	"strong", "smart", "wise", "agile", "tough", "charismatic"*/
	"強い","賢明だ","賢い","機敏だ","頑丈だ","魅力的だ"
},
		*minusattr[] = {
/*JP	"weak", "stupid", "foolish", "clumsy", "vulnerable", "ugly"*/
	"弱い","愚かだ","間抜けだ","不器用だ","ひ弱だ","醜い"
};

/* STEPHEN WHITE'S NEW CODE */   
	/* maximum and minimum values for the attributes */
struct attribs  class_a = {18,  19, 18, 19, 17, 19},        
		class_b = {120, 11, 10, 18, 20, 10},
		class_c = {120, 10, 13, 18, 20, 15},
		class_d = {118, 18, 18, 18, 18, 18},
		class_e = {68,  19, 16, 21, 13, 19},
		class_f = {68,  19, 17, 19, 16, 18},
		class_g = {118, 15, 15, 20, 18, 13},
		class_h = {14,  19, 19, 19, 19, 18},
		class_i = {68,  19, 17, 19, 16, 18},
		class_j = {98,  19, 16, 19, 19, 20},
		class_k = {119, 17, 19, 13, 18, 19},
		class_l = {120, 13, 13, 18, 14,  7},
		class_m = {10,  20, 20, 18, 20, 18},
		class_n = {12,  20, 20, 19, 16, 18},
		class_p = {118, 17, 20, 15, 18, 18},
		class_r = {118, 17, 17, 20, 17, 16},
		class_s = {118, 15, 15, 20, 18, 13},
		class_t = {68,  19, 19, 15, 18, 19},
		class_u = {118, 17, 17, 18, 19, 18},
		class_v = {120, 12, 17, 16, 20, 16},
		class_w = {12,  20, 20, 19, 16, 18},
		attrmin = {3, 3, 3, 3, 3, 3};

static
const struct innate {
	schar	ulevel;
	long	*ability;
	const char *gainstr, *losestr;
}	a_abil[] = { {	 1, &(Stealth), "", "" },
		     {   1, &(Fast), "", "" },
		     {   7, &(HSee_invisible), "", "" },
/*JP		     {  13, &(Searching), "perceptive", "unaware" },*/
		     {  13, &(Searching), "知覚力を得た", "知覚力を失った" },
		     {	 0, 0, 0, 0 } },

	b_abil[] = { {	 1, &(HPoison_resistance), "", "" },
/*JP		     {   7, &(Stealth), "stealthy", "" },
		     {   11, &(Fast), "quick", "slow" },*/
		     {   7, &(Stealth), "人目を盗む力を得た", "人目を盗む力を失った" },
		     {   11, &(Fast), "素早さを得た", "遅くなった" },
		     {	 0, 0, 0, 0 } },

/*JP	c_abil[] = { {	 7, &(Fast), "quick", "slow" },
		     {	15, &(Warning), "sensitive", "" },*/
	c_abil[] = { {	 7, &(Fast), "素早さを得た", "遅くなった" },
		     {	15, &(Warning), "敏感になった", "鈍感になった" },
		     {	 0, 0, 0, 0 } },

	d_abil[] = { {   1, &(Polymorph), "", "" },
/*JP		     {   9, &(Polymorph_control), "your choices improve", "choiceless" },*/
		     {   9, &(Polymorph_control), "選択力を得た", "選択力を失った" },
		     {   0, 0, 0, 0 } },

	e_abil[] = { {   1, &(HSleep_resistance), "", "" },
		     {	 1, &(HSee_invisible), "", "" },
		     {	 1, &(Searching), "", "" },
/*JP		     {   7, &(Fast), "quick", "slow" },*/
		     {   7, &(Fast), "素早さを得た", "遅くなった" },
		     {   0, 0, 0, 0 } },

	f_abil[] = { {   1, &(HFire_resistance), "", "" },
/*JP		     {  13, &(HCold_resistance), "warm", "cooler" },*/
		     {  13, &(HCold_resistance), "暖かく感じる", "冷たくなった" },
		     {   0, 0, 0, 0 } },

/*JP	g_abil[] = { {   5, &(Stealth), "stealthy", "" },
		     {   9, &(Fast), "quick", "slow" },
		     {   11, &(Searching), "perceptive", "unaware" },*/
	g_abil[] = { {   5, &(Stealth), "人目を盗む力を得た", "人目を盗む力を失った" },
		     {   9, &(Fast), "素早さを得た", "遅くなった" },
		     {   11, &(Searching), "知覚力を得た", "知覚力を失った" },
		     {	 0, 0, 0, 0 } },

	h_abil[] = { {	 1, &(HPoison_resistance), "", "" },
/*JP		     {	15, &(Warning), "sensitive", "" },*/
		     {	15, &(Warning), "敏感になった", "鈍感になった" },
		     {	 0, 0, 0, 0 } },

	i_abil[] = { {   1, &(HCold_resistance), "", "" },
/*JP		     {  13, &(HFire_resistance), "cool", "warmer" },*/
		     {  13, &(HFire_resistance), "冷たく感じる", "暖かくなった" },
		     {   0, 0, 0, 0 } },

#ifdef JFIGHTER
	j_abil[] = { {	 7, &(Fast), "素早さを得た", "遅くなった" },
		     {  15, &(Stealth), "人目を盗む力を得た", "人目を盗む力を失った" },
		     {  17, &(HTeleport_control), "制御力を得た","制御力を失った" },
		     {	 0, 0, 0, 0 } },
#endif

/*JP	k_abil[] = { {	 7, &(Fast), "quick", "slow" },*/
	k_abil[] = { {	 7, &(Fast), "素早さを得た", "遅くなった" },
		     {	 0, 0, 0, 0 } },

	l_abil[] = { {   1, &(HPoison_resistance), "", "" },
		     {   1, &(HRegeneration), "", "" },
/*JP		     {   7, &(Stealth), "stealthy", "" },*/
		     {   7, &(Stealth), "人目を盗む力を得た", "" },
		     {   0, 0, 0, 0 } },

	m_abil[] = { {   1, &(Fast), "", "" },
		     {   1, &(HSleep_resistance), "", "" },
		     {   1, &(HSee_invisible), "", "" },
/*JP		     {   3, &(HPoison_resistance), "healthy", "" },
		     {   5, &(Stealth), "stealthy", "" },
		     {   7, &(Warning), "sensitive", "" },
		     {   9, &(Searching), "perceptive", "unaware" },
		     {  11, &(HFire_resistance), "cool", "warmer" },
		     {  13, &(HCold_resistance), "warm", "cooler" },
		     {  15, &(HShock_resistance), "insulated", "conductive" },
		     {  17, &(HTeleport_control), "controlled","uncontrolled" },*/
		     {   3, &(HPoison_resistance), "免疫力を得た", "" },
		     {   5, &(Stealth), "人目を盗む力を得た", "" },
		     {   7, &(Warning), "敏感になった", "" },
		     {   9, &(Searching), "知覚力を得た", "知覚力を失った" },
		     {  11, &(HFire_resistance), "冷たく感じる", "暖かくなった"},
		     {  13, &(HCold_resistance), "暖かく感じる", "冷たくなった"},
		     {  15, &(HShock_resistance), "絶縁体になった", "伝導体になった" },
		     {  17, &(HTeleport_control), "制御力を得た","制御力を失った" },
		     {   0, 0, 0, 0 } },

	n_abil[] = { {   1, &(HDrain_resistance), "", "" },
		     {   1, &(HSick_resistance), "", "" },
/*JP		     {   3, &(Undead_warning), "sensitive", "" },*/
		     {   3, &(Undead_warning), "敏感になった", "" },
		     {   0, 0, 0, 0 } },

	p_abil[] = { {   1, &(HShock_resistance), "", "" },
/*JP		     {  15, &(Warning), "sensitive", "" },
		     {  20, &(HFire_resistance), "cool", "warmer" },*/
		     {  15, &(Warning), "敏感になった", "鈍感になった" },
		     {  20, &(HFire_resistance), "冷たく感じる", "暖かくなった" },
		     {	 0, 0, 0, 0 } },

	r_abil[] = { {	 1, &(Stealth), "", ""  },
/*JP		     {  10, &(Searching), "perceptive", "unaware" },*/
		     {  10, &(Searching), "知覚力を得た", "知覚力を失った" },
		     {	 0, 0, 0, 0 } },

	s_abil[] = { {	 1, &(Fast), "", "" },
		     {  15, &(Stealth), "人目を盗む力を得た", "人目を盗む力を失った" },
		     {	 0, 0, 0, 0 } },

/*JP	t_abil[] = { {  10, &(Searching), "perceptive", "unaware" },*/
	t_abil[] = { {	10, &(Searching), "知覚力を得た", "知覚力を失った" },
/*JP		     {	20, &(HPoison_resistance), "hardy", "" },*/
		     {	20, &(HPoison_resistance), "免疫力を得た", "免疫力を失った" },
		     {	 0, 0, 0, 0 } },

	u_abil[] = { {   1, &(Stealth), "", "" },
		     {   1, &(HDrain_resistance), "", "" },
		     {   1, &(HSick_resistance), "", "" },
		     {   1, &(Undead_warning), "", "" },
/*JP		     {   7, &(Fast), "quick", "slow" },
		     {   9, &(HPoison_resistance), "hardy", "less healthy" },*/
		     {   7, &(Fast), "素早さを得た", "遅くなった" },
		     {	 9, &(HPoison_resistance), "免疫力を得た", "免疫力を失った" },
		     {   0, 0, 0, 0 } },

	v_abil[] = { {	 1, &(HCold_resistance), "", "" },
		     {	 1, &(Stealth), "", "" },
/*JP		     {   7, &(Fast), "quick", "slow" },*/
		     {   7, &(Fast), "素早さを得た", "遅くなった" },
		     {	 0, 0, 0, 0 } },

/*JP	w_abil[] = { {	15, &(Warning), "sensitive", "" },
		     {  17, &(HTeleport_control), "controlled","uncontrolled" },*/
	w_abil[] = { {	15, &(Warning), "敏感になった", "鈍感になった" },
		     {  17, &(HTeleport_control), "制御力を得た","制御力を失った" },
		     {	 0, 0, 0, 0 } };

/* STEPHEN WHITE'S NEW CODE */   
static
const struct clattr {
	struct	attribs	base, cldist;
	align	align;
	schar	shp, hd, xlev, ndx;
/* According to AD&D, HD for some classes (ex. Wizard) should be smaller
 * (4-sided for wizards).  But this is not AD&D, and using the AD&D
 * rule here produces an unplayable character.  Thus I have used a minimum
 * of an 10-sided hit die for everything.  Another AD&D change: wizards get
 * a minimum strength of 4 since without one you can't teleport or cast
 * spells. --KAA
 */
	const struct	innate *abil;
}       a_attr = { {	10, 10, 10, 10, 10, 12 },  /* Archeologist */
		   {  	 5,  8,  5,  5,  6,  5 },
		   { A_LAWFUL,  10 },  13,  8, 14,  2, a_abil },

	b_attr = { {    15,  4,  4, 10, 15, 6  },  /* Barbarian */        
		   {     5,  5,  5,  5,  3, 5  },
		    { A_CHAOTIC, 10 }, 16, 8,  10,  3, b_abil },
  
	c_attr = { {    15,  5,  8, 10, 15,  6 },  /* Caveman (fighter) */
		   {     5,  6,  5,  5,  3,  7 },
		    { A_LAWFUL, 0 },   16, 8 , 10,  3, c_abil },
  
	d_attr = { {    10, 10, 10, 10, 10, 10 },  /* Doppelganger */
		   {     5,  5,  5,  5,  5,  5 },
		    { A_NEUTRAL, 10 },  12, 8, 11,  2, d_abil },
  
	e_attr = { {     7, 12,  7, 16,  6, 12 },  /* Elf (ranger) */
		   {     8,  6, 11,  5,  8,  7 },
		    { A_LAWFUL, 10 },   15, 8, 11,  2, e_abil },
  
	f_attr = { {     6, 14, 10, 10,  6,  6 },  /* Flame Mage */
		   {     4,  5,  5,  8, 12, 12 },
		    { A_NEUTRAL, 10 },  14, 8, 11,  2, f_abil },
  
	g_attr = { {    10, 10, 10, 10, 10, 10 },  /* Gnome */
		   {     5,  5,  5,  5,  5,  5 },
		    { A_NEUTRAL, 0 },   14, 8, 10,  3, g_abil },
  
	h_attr = { {     6, 12, 14, 12, 10, 10 },  /* Healer (druid) */
		   {     6,  8,  6,  6,  5,  5 },
		    { A_NEUTRAL, 10 },  13, 8, 20,  2, h_abil },
  
	i_attr = { {     6, 14, 10, 10,  6,  6 },  /* Ice Mage */
		   {     4,  5,  5,  8, 12, 12 },
		    { A_NEUTRAL, 10 },  14, 8, 11,  2, i_abil },
  
#ifdef JFIGHTER 
	j_attr = { {    15,  5, 13, 10, 15, 17 },  /**/
		   {     5, 13,  5,  5,  3,  2 },
		    { A_LAWFUL, 10 },   16, 8, 10,  3, j_abil },
#endif

	k_attr = { {    15,  5, 13, 10, 15, 17 },  /* Knight (paladin) */
		   {     5, 13,  5,  5,  3,  2 },
		    { A_LAWFUL, 10 },   16, 8, 10,  3, k_abil },
  
	l_attr = { {    12,  8,  5, 10,  8,  3 },  /* Lycanthrope */
		   {     7,  5, 10,  8,  5,  5 },
		    { A_CHAOTIC, 10 },  15, 8, 11,  2, l_abil },

	m_attr = { {     6, 10, 13, 16,  8,  8 },  /* Monk (cleric) */        
		   {    12,  8,  5,  4, 10, 10 },
		    { A_NEUTRAL, 0 },   14, 8, 10,  2, m_abil },
  
	n_attr = { {     6, 14, 10, 10,  6,  4 },  /* Necromancer */
		   {     4,  5,  5,  8, 12,  8 },
		    { A_CHAOTIC, 0 },   12, 8, 12,  1, n_abil },
  
	p_attr = { {    10, 10, 14,  7, 10, 12 },  /* Priest (cleric) */
		   {     8,  8,  5, 11,  5,  6 },
		    { A_NEUTRAL, 0 },   14, 8, 10,  2, p_abil },
  
	r_attr = { {    10, 10,  5, 16,  7,  6 },  /* Rogue (thief) */
		   {     8,  5, 12,  4, 10,  6 },
		    { A_CHAOTIC, 10 },  12, 8, 11,  2, r_abil },
  
	s_attr = { {    12,  5,  5, 16, 13,  6 },  /* Samurai (fighter/thief) */
		   {     6, 10, 10,  3,  5,  5 },
		    { A_LAWFUL, 10 },   15, 8, 11,  2, s_abil },

#ifdef TOURIST
	t_attr = { {     8,  5,  5,  5,  5,  5 },  /* Tourist */
		   {    10, 10, 10, 10, 10, 10 },
		    { A_NEUTRAL, 0 },   10, 8, 14,  1, t_abil },
#endif

	u_attr = { {    15,  5, 13, 10, 15, 10 },  /* Undead Slayer */
		   {     5, 13,  5,  5,  3,  5},
		    { A_LAWFUL, 10 },   14, 8, 14,  1, u_abil },
 
	v_attr = { {    15,  5,  5, 10, 15, 10 },  /* Valkyrie (fighter) */
		   {     5, 10, 10,  5,  3,  5 },
		    { A_NEUTRAL, 0 },   16, 8, 10,  3, v_abil },

	w_attr = { {     6, 14, 10, 10,  6,  6 },  /* Wizard (magic-user) */
		   {     4,  5,  5,  8, 12, 12 },
		    { A_NEUTRAL, 0 },   12, 8, 12,  1, w_abil },

	X_attr = { {    10, 10, 10, 10, 10, 10 },
		   {    20, 15, 15, 15, 15, 20 },
		    { A_NEUTRAL, 0 },   12, 8, 14,  1,  0 };

static long next_check = 3000L;  /* arbitrary first setting */
static const struct clattr NEARDATA *NDECL(clx);
static void NDECL(init_align);
static void NDECL(exerper);

/* adjust an attribute; return TRUE if change is made, FALSE otherwise */
boolean
adjattrib(ndx, incr, msgflg)
	int	ndx, incr;
	int	msgflg;	    /* positive => no message, zero => message, and */
{			    /* negative => conditional (msg if change made) */
  /* STEPHEN WHITE'S NEW CODE */                
	int     temp;        
  
	switch  (u.role) {
	    case 'A':   temp = CLASS_A(ndx); break;
	    case 'B':   temp = CLASS_B(ndx); break;
	    case 'C':   temp = CLASS_C(ndx); break;
	    case 'D':   temp = CLASS_D(ndx); break;
	    case 'E':   temp = CLASS_E(ndx); break;
	    case 'F':   temp = CLASS_F(ndx); break;
	    case 'G':   temp = CLASS_G(ndx); break;
	    case 'H':   temp = CLASS_H(ndx); break;
	    case 'I':   temp = CLASS_I(ndx); break;
#ifdef JFIGHTER 
	    case 'J':   temp = CLASS_J(ndx); break;
#endif
	    case 'K':   temp = CLASS_K(ndx); break;
	    case 'L':   temp = CLASS_L(ndx); break;
	    case 'M':   temp = CLASS_M(ndx); break;
	    case 'N':   temp = CLASS_N(ndx); break;
	    case 'P':   temp = CLASS_P(ndx); break;
	    case 'R':   temp = CLASS_R(ndx); break;
	    case 'S':   temp = CLASS_S(ndx); break;
#ifdef TOURIST
	    case 'T':   temp = CLASS_T(ndx); break;
#endif
	    case 'U':   temp = CLASS_U(ndx); break;
	    case 'V':   temp = CLASS_V(ndx); break;
	    case 'W':   temp = CLASS_W(ndx); break;
	    default:    /* unknown type */
			temp = CLASS_A(ndx); break;
	}
		
  
	if (!incr) return FALSE;

	if ((ndx == A_INT || ndx == A_WIS)
				&& uarmh && uarmh->otyp == DUNCE_CAP) {
		if (msgflg == 0)
/*JP		    Your("cap constricts briefly, then relaxes again.");*/
		    Your("帽子がキュっと締めつけた．ふー，またリラックスできる．");
		return FALSE;
	}

	if (incr > 0) {
	    if ((AMAX(ndx) >= temp) && (ACURR(ndx) >= AMAX(ndx))) {
		if (msgflg == 0 && flags.verbose)
/*JP		    pline("You're already as %s as you can get.",*/
		    You("もう十分に%s．",
			  plusattr[ndx]);
		ABASE(ndx) = AMAX(ndx) = temp; /* just in case */
		return FALSE;
	    }

	    ABASE(ndx) += incr;
	    if(ABASE(ndx) > AMAX(ndx)) {
		incr = ABASE(ndx) - AMAX(ndx);
		AMAX(ndx) += incr;
		if(AMAX(ndx) > temp) AMAX(ndx) = temp;
		ABASE(ndx) = AMAX(ndx);
	    }
	} else {
	    if (ABASE(ndx) <= ATTRMIN(ndx)) {
		if (msgflg == 0 && flags.verbose)
/*JP		    pline("You're already as %s as you can get.",*/
		    You("もう十分%s．",
			  minusattr[ndx]);
		ABASE(ndx) = ATTRMIN(ndx); /* just in case */
		return FALSE;
	    }

	    ABASE(ndx) += incr;
	    if(ABASE(ndx) < ATTRMIN(ndx)) {
		incr = ABASE(ndx) - ATTRMIN(ndx);
		ABASE(ndx) = ATTRMIN(ndx);
		AMAX(ndx) += incr;
		if(AMAX(ndx) < ATTRMIN(ndx)) AMAX(ndx) = ATTRMIN(ndx);
	    }
	}
	if (msgflg <= 0)
/*JP	    You_feel("%s%s!",
		  (incr > 1 || incr < -1) ? "very ": "",
		  (incr > 0) ? plusattr[ndx] : minusattr[ndx]);*/
	    You("%s%sなったような気がした！",
		  (incr > 1 || incr < -1) ? "とても ": "",
		  jconj_adj((incr > 0) ? plusattr[ndx] : minusattr[ndx]));
	flags.botl = 1;
	if (moves > 0 && (ndx == A_STR || ndx == A_CON))
		(void)encumber_msg();
	return TRUE;
}

void
gainstr(otmp, incr)
	register struct obj *otmp;
	register int incr;
{
	int num = 1;

	if(incr) num = incr;
	else {
	    if(ABASE(A_STR) < 18) num = (rn2(4) ? 1 : rnd(6) );
	    else if (ABASE(A_STR) < 103) num = rnd(10);
	}
	(void) adjattrib(A_STR, (otmp && otmp->cursed) ? -num : num, TRUE);
}

void
losestr(num)	/* may kill you; cause may be poison or monster like 'a' */
	register int num;
{
	int ustr = ABASE(A_STR) - num;
/* STEPHEN WHITE'S NEW CODE */
	while(ustr < 3) {
		ustr++;
		num--;
		u.uhp -= 4;
		u.uhpmax -= 4;
		u.uhpbase -= 4;
	}
	(void) adjattrib(A_STR, -num, TRUE);
}

void
change_luck(n)
	register schar n;
{
	u.uluck += n;
	if (u.uluck < 0 && u.uluck < LUCKMIN)	u.uluck = LUCKMIN;
	if (u.uluck > 0 && u.uluck > LUCKMAX)	u.uluck = LUCKMAX;
}

int
stone_luck(parameter)
boolean parameter; /* So I can't think up of a good name.  So sue me. --KAA */
{
	register struct obj *otmp;
	register long bonchance = 0;

	for(otmp = invent; otmp; otmp=otmp->nobj)
	    if (otmp->otyp == LUCKSTONE
		|| (otmp->oartifact && spec_ability(otmp, SPFX_LUCK))) {
		if (otmp->cursed) bonchance -= otmp->quan;
		else if (otmp->blessed) bonchance += otmp->quan;
		else if (parameter) bonchance += otmp->quan;
	    }

	/* STEPHEN WHITE'S NEW CODE */
	if (uarmh && uarmh->otyp == FEDORA && !uarmh->cursed) bonchance += 2;
	
	return sgn((int)bonchance);
}

/* there has just been an inventory change affecting a luck-granting item */
void
set_moreluck()
{
	int luckbon = stone_luck(TRUE);

	if (!luckbon && !carrying(LUCKSTONE)) u.moreluck = 0;
	else if (luckbon >= 0) u.moreluck = LUCKADD;
	else u.moreluck = -LUCKADD;
}

#endif /* OVLB */
#ifdef OVL1

void
restore_attrib()
{
	int	i;

	for(i = 0; i < A_MAX; i++) {	/* all temporary losses/gains */

	   if(ATEMP(i) && ATIME(i)) {
		if(!(--(ATIME(i)))) { /* countdown for change */
		    ATEMP(i) += ATEMP(i) > 0 ? -1 : 1;

		    if(ATEMP(i)) /* reset timer */
			ATIME(i) = 100 / ACURR(A_CON);
		}
	    }
	}
	(void)encumber_msg();
}

#endif /* OVL1 */
#ifdef OVLB

#define AVAL	50		/* tune value for exercise gains */

void
exercise(i, inc_or_dec)
int	i;
boolean	inc_or_dec;
{
#ifdef DEBUG
	pline("Exercise:");
#endif
	if (i == A_INT || i == A_CHA) return;	/* can't exercise these */

	/* no physical exercise while polymorphed; the body's temporary */
	if (u.umonnum >= LOW_PM && i != A_WIS) return;

	if(abs(AEXE(i)) < AVAL) {
		/*
		 *	Law of diminishing returns (Part I):
		 *
		 *	Gain is harder at higher attribute values.
		 *	79% at "3" --> 0% at "18"
		 *	Loss is even at all levels (50%).
		 *
		 *	Note: *YES* ACURR is the right one to use.
		 */
		AEXE(i) += (inc_or_dec) ? (rn2(19) > ACURR(i)) : -rn2(2);
#ifdef DEBUG
		pline("%s, %s AEXE = %d",
			(i == A_STR) ? "Str" : (i == A_WIS) ? "Wis" :
			(i == A_DEX) ? "Dex" : "Con",
			(inc_or_dec) ? "inc" : "dec", AEXE(i));
#endif
	}
	if (moves > 0 && (i == A_STR || i == A_CON)) (void)encumber_msg();
}

/* hunger values - from eat.c */
#define SATIATED	0
#define NOT_HUNGRY	1
#define HUNGRY		2
#define WEAK		3
#define FAINTING	4
#define FAINTED		5
#define STARVED		6

static void
exerper()
{
	if(!(moves % 10)) {
		/* Hunger Checks */

		int hs = (u.uhunger > 1000) ? SATIATED :
			 (u.uhunger > 150) ? NOT_HUNGRY :
			 (u.uhunger > 50) ? HUNGRY :
			 (u.uhunger > 0) ? WEAK : FAINTING;

#ifdef DEBUG
		pline("exerper: Hunger checks");
#endif
		switch (hs) {
		    case SATIATED:	exercise(A_DEX, FALSE); break;
		    case NOT_HUNGRY:	exercise(A_CON, TRUE); break;
		    case WEAK:		exercise(A_STR, FALSE); break;
		    case FAINTING:
		    case FAINTED:	exercise(A_CON, FALSE); break;
		}

		/* Encumberance Checks */
#ifdef DEBUG
		pline("exerper: Encumber checks");
#endif
		switch (near_capacity()) {
		    case MOD_ENCUMBER:	exercise(A_STR, TRUE); break;
		    case HVY_ENCUMBER:	exercise(A_STR, TRUE);
					exercise(A_DEX, FALSE); break;
		    case EXT_ENCUMBER:	exercise(A_DEX, FALSE);
					exercise(A_CON, FALSE); break;
		}

	}

	/* status checks */
	if(!(moves % 5)) {
#ifdef DEBUG
		pline("exerper: Status checks");
#endif
		if ((HClairvoyant & (INTRINSIC|TIMEOUT)) &&
			!(HClairvoyant & I_BLOCKED))	exercise(A_WIS, TRUE);
		if (HRegeneration)			exercise(A_STR, TRUE);

		if(Sick || Vomiting)			exercise(A_CON, FALSE);
		if(Confusion || Hallucination)		exercise(A_WIS, FALSE);
		if(Wounded_legs || Fumbling || HStun)	exercise(A_DEX, FALSE);
	}
}

void
exerchk()
{
	int	i, mod_val;

	/*	Check out the periodic accumulations */
	exerper();

#ifdef DEBUG
	if(moves >= next_check)
		pline("exerchk: ready to test. multi = %d.", multi);
#endif
	/*	Are we ready for a test?	*/
	if(moves >= next_check && !multi) {
#ifdef DEBUG
	    pline("exerchk: testing.");
#endif
	    /*
	     *	Law of diminishing returns (Part II):
	     *
	     *	The effects of "exercise" and "abuse" wear
	     *	off over time.  Even if you *don't* get an
	     *	increase/decrease, you lose some of the
	     *	accumulated effects.
	     */
	    for(i = 0; i < A_MAX; AEXE(i++) /= 2) {

		if(ABASE(i) >= 18 || !AEXE(i)) continue;
		if(i == A_INT || i == A_CHA) continue;/* can't exercise these */

#ifdef DEBUG
		pline("exerchk: testing %s (%d).",
			(i == A_STR) ? "Str" : (i == A_WIS) ? "Wis" :
			(i == A_DEX) ? "Dex" : "Con", AEXE(i));
#endif
		/*
		 *	Law of diminishing returns (Part III):
		 *
		 *	You don't *always* gain by exercising.
		 *	[MRS 92/10/28 - Treat Wisdom specially for balance.]
		 */
		if(rn2(AVAL) > ((i != A_WIS) ? abs(AEXE(i)*2/3) : abs(AEXE(i))))
		    continue;
		mod_val = sgn(AEXE(i));

#ifdef DEBUG
		pline("exerchk: changing %d.", i);
#endif
		if(adjattrib(i, mod_val, -1)) {
#ifdef DEBUG
		    pline("exerchk: changed %d.", i);
#endif
		    /* if you actually changed an attrib - zero accumulation */
		    AEXE(i) = 0;
		    /* then print an explanation */
		    switch(i) {
		    case A_STR: You((mod_val >0) ?
/*JP				    "must have been exercising." :
				    "must have been abusing your body.");*/
				    "運動したに違いない．" :
				    "体を酷使したに違いない．");
				break;
		    case A_WIS: You((mod_val >0) ?
/*JP				    "must have been very observant." :
				    "haven't been paying attention.");*/
				    "慎重に行動してたに違いない．" :
				    "注意不足だったに違いない．");
				break;
		    case A_DEX: You((mod_val >0) ?
/*JP				    "must have been working on your reflexes." :
				    "haven't been working on reflexes lately.");*/
				    "反射神経を使ったに違いない．" :
				    "最近反射神経を使ってなかったに違いない．");
				break;
		    case A_CON: You((mod_val >0) ?
/*JP				    "must be leading a healthy life-style." :
				    "haven't been watching your health.");*/
				    "健康的な生活をしていたに違いない．" :
				    "健康管理を怠っていたに違いない．");
				break;
		    }
		}
	    }
	    next_check += rn1(2000,2000);
#ifdef DEBUG
	    pline("exerchk: next check at %ld.", next_check);
#endif
	}
}

/* next_check will otherwise have its initial 600L after a game restore */
void
reset_attribute_clock()
{
	if (moves > 600L) next_check = moves + rn1(800,800);
}

static const struct	clattr *
clx()
{
	register const struct	clattr	*attr;

	switch (u.role) {
	    case 'A':	attr = &a_attr;
			break;
	    case 'B':	attr = &b_attr;
			break;
	    case 'C':	attr = &c_attr;
			break;
	    case 'D':   attr = &d_attr;            
			break;
	    case 'E':	attr = &e_attr;
			break;
	    case 'F':   attr = &f_attr;            
			break;
	    case 'G':   attr = &g_attr;
			break;
	    case 'H':	attr = &h_attr;
			break;
	    case 'I':   attr = &i_attr;            
			break;
#ifdef JFIGHTER
	    case 'J':	attr = &j_attr;
			break;
#endif
	    case 'K':	attr = &k_attr;
			break;
	    case 'L':   attr = &l_attr;            
			break;
	    case 'M':   attr = &m_attr;
			break;
	    case 'N':   attr = &n_attr;
			break;
	    case 'P':	attr = &p_attr;
			break;
	    case 'R':	attr = &r_attr;
			break;
	    case 'S':	attr = &s_attr;
			break;
#ifdef TOURIST
	    case 'T':	attr = &t_attr;
			break;
#endif
	    case 'U':   attr = &u_attr;            
			break;
	    case 'V':	attr = &v_attr;
			break;
	    case 'W':	attr = &w_attr;
			break;
	    default:	/* unknown type */
			attr = &X_attr;
			break;
	}
	return(attr);
}

static void
init_align()	/* called from newhp if u.ulevel is 0 */
{
	register const struct	clattr	*attr = clx();

	u.ualign = attr->align;
	/* there should be priests of every stripe */
	if (Role_is('P'))
	     u.ualign.type = (rn2(2)) ? attr->align.type : (rn2(2)) ? 1 : -1;
	else u.ualign.type = attr->align.type;
}

void
init_attr(np)
	register int	np;
{
/* STEPHEN WHITE'S NEW CODE */                
	register int    i, x, tryct, temp;
	register const struct	clattr	*attr = clx();

	for(i = 0; i < A_MAX; i++) {

	    ABASE(i) = AMAX(i) = attr->base.a[i];
	    ATEMP(i) = ATIME(i) = 0;
	    np -= attr->base.a[i];
	}

	tryct = 0;
	while(np > 0 && tryct < 100) {

	    x = rn2(100);
	    for (i = 0; (i < A_MAX) && ((x -= attr->cldist.a[i]) > 0); i++) ;
	    if(i >= A_MAX) continue; /* impossible */

	switch  (u.role) {
	    case 'A':   temp = CLASS_A(i); break;
	    case 'B':   temp = CLASS_B(i); break;
	    case 'C':   temp = CLASS_C(i); break;
	    case 'D':   temp = CLASS_D(i); break;
	    case 'E':   temp = CLASS_E(i); break;
	    case 'F':   temp = CLASS_F(i); break;
	    case 'G':   temp = CLASS_G(i); break;
	    case 'H':   temp = CLASS_H(i); break;
	    case 'I':   temp = CLASS_I(i); break;
#ifdef JFIGHTER
	    case 'J':   temp = CLASS_J(i); break;
#endif
	    case 'K':   temp = CLASS_K(i); break;
	    case 'L':   temp = CLASS_L(i); break;
	    case 'M':   temp = CLASS_M(i); break;
	    case 'N':   temp = CLASS_N(i); break;
	    case 'P':   temp = CLASS_P(i); break;
	    case 'R':   temp = CLASS_R(i); break;
	    case 'S':   temp = CLASS_S(i); break;
#ifdef TOURIST
	    case 'T':   temp = CLASS_T(i); break;
#endif
	    case 'U':   temp = CLASS_U(i); break;
	    case 'V':   temp = CLASS_V(i); break;
	    case 'W':   temp = CLASS_W(i); break;
	    default:    /* unknown type */
			temp = CLASS_A(i); break;
	}


	    if(ABASE(i) >= temp) {

		tryct++;
		continue;
	    }
	    tryct = 0;
	    ABASE(i)++;
	    AMAX(i)++;
	    np--;
	}

	tryct = 0;
	while(np < 0 && tryct < 100) {		/* for redistribution */

	    x = rn2(100);
	    for (i = 0; (i < A_MAX) && ((x -= attr->cldist.a[i]) > 0); i++) ;
	    if(i >= A_MAX) continue; /* impossible */

	    if(ABASE(i) <= ATTRMIN(i)) {

		tryct++;
		continue;
	    }
	    tryct = 0;
	    ABASE(i)--;
	    AMAX(i)--;
	    np++;
	}
}

void
redist_attr()
{
	register int i,temp,tmp;

	for(i = 0; i < A_MAX; i++) {
	switch  (u.role) {
	    case 'A':   temp = CLASS_A(i); break;
	    case 'B':   temp = CLASS_B(i); break;
	    case 'C':   temp = CLASS_C(i); break;
	    case 'D':   temp = CLASS_D(i); break;
	    case 'E':   temp = CLASS_E(i); break;
	    case 'F':   temp = CLASS_F(i); break;
	    case 'G':   temp = CLASS_G(i); break;
	    case 'H':   temp = CLASS_H(i); break;
	    case 'I':   temp = CLASS_I(i); break;
#ifdef JFIGHTER
	    case 'J':   temp = CLASS_J(i); break;
#endif
	    case 'K':   temp = CLASS_K(i); break;
	    case 'L':   temp = CLASS_L(i); break;
	    case 'M':   temp = CLASS_M(i); break;
	    case 'N':   temp = CLASS_N(i); break;
	    case 'P':   temp = CLASS_P(i); break;
	    case 'R':   temp = CLASS_R(i); break;
	    case 'S':   temp = CLASS_S(i); break;
#ifdef TOURIST
	    case 'T':   temp = CLASS_T(i); break;
#endif
	    case 'U':   temp = CLASS_U(i); break;
	    case 'V':   temp = CLASS_V(i); break;
	    case 'W':   temp = CLASS_W(i); break;
	    default:    /* unknown type */
			temp = CLASS_A(i); break;
	}
	    
	    if (i==A_INT || i==A_WIS) continue;
		/* Polymorphing doesn't change your mind */
	    tmp = AMAX(i);
	    AMAX(i) += (rn2(5)-2);
	    if (AMAX(i) > temp) AMAX(i) = temp;
	    if (AMAX(i) < ATTRMIN(i)) AMAX(i) = ATTRMIN(i);
	    ABASE(i) = ABASE(i) * AMAX(i) / tmp;
	    /* ABASE(i) > temp is impossible */
	    if (ABASE(i) < ATTRMIN(i)) ABASE(i) = ATTRMIN(i);
	}
	(void)encumber_msg();
}

void
adjabil(oldlevel,newlevel)
int oldlevel, newlevel;
{
	register const struct clattr	*attr = clx();
#ifdef GCC_WARN
	/* this is the "right" definition */
	register const struct innate	*abil = attr->abil;
#else
	/* this one satisfies more compilers */
	register struct innate	*abil = (struct innate *)attr->abil;
#endif

	if(abil) {
	    for(; abil->ability; abil++) {
		if(oldlevel < abil->ulevel && newlevel >= abil->ulevel) {
			/* Abilities gained at level 1 can never be lost
			 * via level loss, only via means that remove _any_
			 * sort of ability.  A "gain" of such an ability from
			 * an outside source is devoid of meaning, so we set
			 * FROMOUTSIDE to avoid such gains.
			 */
			if (abil->ulevel == 1)
				*(abil->ability) |= (FROMEXPER|FROMOUTSIDE);
			else
				*(abil->ability) |= FROMEXPER;
			if(!(*(abil->ability) & FROMOUTSIDE)) {
			    if(*(abil->gainstr))
/*JP				You_feel("%s!", abil->gainstr);*/
				You("%sような気がした！", abil->gainstr);
			}
		} else if (oldlevel >= abil->ulevel && newlevel < abil->ulevel) {
			*(abil->ability) &= ~FROMEXPER;
			if((*(abil->ability) & INTRINSIC)) {
			    if(*(abil->losestr))
/*JP				You_feel("%s!", abil->losestr);*/
				You("%sような気がした！", abil->losestr);
/*
**	この条件は満さないはず．
*/
			    else if(*(abil->gainstr))
				You_feel("less %s!", abil->gainstr);
			}
		}
	    }
	}

#ifdef WEAPON_SKILLS
	if (oldlevel > 0) {
	    if (newlevel > oldlevel)
		add_weapon_skill(newlevel - oldlevel);
	    else
		lose_weapon_skill(oldlevel - newlevel);
	}
#endif /* WEAPON_SKILLS */
}

/* STEPHEN WHITE'S NEW CODE */   
int
newhp()
{
	register const struct clattr	*attr = clx();
	int     hp, bonushp;

	if(u.ulevel == 0) {

		hp = attr->shp;
		init_align();	/* initialize alignment stuff */
		return hp;
	} else {

	    if(u.ulevel < attr->xlev)
		hp = rnd(attr->hd);
	    else
		hp = attr->ndx;
	
		switch  (u.role) {

		case 'A':   
		case 'D':
#ifdef TOURIST
		case 'T':
#endif
		case 'N':
		case 'W':
		case 'H':
				bonushp = 1;
				break;
		case 'E':
		case 'F':
		case 'I':
		case 'L':
		case 'M':
		case 'P':
		case 'R':
#ifdef JFIGHTER
		case 'J':
#endif
				bonushp = 2;            
				break;
		case 'B':   
		case 'C':
		case 'G':
		case 'K':
		case 'S':
		case 'U':
		case 'V':
				bonushp = 3;            
				break;
		default: 
				bonushp = 1;            
				break;
	}

	}

	if (u.ulevel < 4) hp += rnd(2);        
	return((hp <= 0) ? 1 : hp + bonushp);
}

#endif /* OVLB */
#ifdef OVL0

/* STEPHEN WHITE'S NEW CODE */   
schar
acurr(x)
int x;
{
	register int tmp = (u.abon.a[x] + u.atemp.a[x] + u.acurr.a[x]);

	if (x == A_STR) {
		if (uarmg && uarmg->otyp == GAUNTLETS_OF_POWER) {
			if(uarmg->spe > 7) return(125);
			else return(118 + uarmg->spe);
			}        
		else if (uarm && uarm->otyp == ROBE_OF_WEAKNESS) return(3);
		else return((tmp >= 125) ? 125 : (tmp <= 3) ? 3 : tmp);
	} else if (x == A_CHA) {
		if (tmp < 18 && (u.usym == S_NYMPH ||
		    u.umonnum==PM_SUCCUBUS || u.umonnum == PM_INCUBUS))
		    tmp = 18;
		if (uarmh && uarmh->otyp == FEDORA) tmp += 1;        
		return((tmp >= 25) ? 25 : (tmp <= 3) ? 3 : tmp);
	} else if (x == A_INT || x == A_WIS) {
		/* yes, this may raise int/wis if player is sufficiently
		 * stupid.  there are lower levels of cognition than "dunce".
		 */
		if (uarmh && uarmh->otyp == DUNCE_CAP) return(6);
	}
#ifdef WIN32_BUG
	return(x=((tmp >= 25) ? 25 : (tmp <= 3) ? 3 : tmp));
#else
	return((schar)((tmp >= 25) ? 25 : (tmp <= 3) ? 3 : tmp));
#endif
}

/* condense clumsy ACURR(A_STR) value into value that fits into game formulas
 */
schar
acurrstr()
{
	register int str = ACURR(A_STR);

	if (str <= 18) return str;
	if (str <= 121) return (19 + str / 50); /* map to 19-21 */
	else return str - 100;
}

#endif /* OVL0 */
#ifdef OVL2

/* avoid possible problems with alignment overflow, and provide a centralized
 * location for any future alignment limits
 */
void
adjalign(n)
register int n;
{
	register int newalign = u.ualign.record + n;

	if(n < 0) {
		if(newalign < u.ualign.record)
			u.ualign.record = newalign;
	} else
		if(newalign > u.ualign.record) {
			u.ualign.record = newalign;
			if(u.ualign.record > ALIGNLIM)
				u.ualign.record = ALIGNLIM;
		}
}

#endif /* OVL2 */

/*attrib.c*/
