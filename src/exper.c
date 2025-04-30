/*	SCCS Id: @(#)exper.c	3.2	96/06/16	*/
/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 1985. */
/* NetHack may be freely redistributed.  See license for details. */

/*
**	Japanese version Copyright
**	(c) Issei Numata, Naoki Hamada, Shigehiro Miyashita, 1994-1996
**	changing point is marked `JP' (94/6/7)
**	JNetHack may be freely redistributed.  See license for details. 
*/

#include "hack.h"

/*static*/ long FDECL(newuexp, (int));
static int FDECL(enermod, (int));

/*static*/ long
newuexp(lev)
int lev;
{
	/*              Old XP routine */
	/* if (lev < 10) return (10L * (1L << lev));            */
	/* if (lev < 20) return (10000L * (1L << (lev - 10)));  */
	/* return (10000000L * ((long)(lev - 19)));             */
/*      if (lev == 1)  return (75L);
	if (lev == 2)  return (150L);
	if (lev == 3)  return (300L);
	if (lev == 4)  return (600L);
	if (lev == 5)  return (1200L); */
	if (lev == 1)  return (50L);     /* need 50           */
	if (lev == 2)  return (100L);    /* need 50           */
	if (lev == 3)  return (200L);    /* need 100          */
	if (lev == 4)  return (500L);    /* need 300          */
	if (lev == 5)  return (1000L);   /* need 500          */
	if (lev == 6)  return (1750L);   /* need 750          */
	if (lev == 7)  return (2750L);   /* need 1000         */
	if (lev == 8)  return (4250L);   /* need 1500         */
	if (lev == 9)  return (6250L);   /* need 2000         */
	if (lev == 10) return (8750L);   /* need 2500         */
	if (lev == 11) return (11750L);  /* need 3000         */
	if (lev == 12) return (15500L);  /* need 3750         */
	if (lev == 13) return (20000L);  /* need 4500         */
	if (lev == 14) return (25000L);  /* need 5000         */
	if (lev == 15) return (31000L);  /* need 6000         */
	if (lev == 16) return (38500L);  /* need 7500         */
	if (lev == 17) return (48000L);  /* need 9500         */
	if (lev == 18) return (60000L);  /* need 12000        */
	if (lev == 19) return (76000L);  /* need 16000        */
	if (lev == 20) return (97000L);  /* need 21000        */
	if (lev == 21) return (125000L); /* need 28000   +7   */
	if (lev == 22) return (163000L); /* need 38000   +10  */
	if (lev == 23) return (213000L); /* need 50000   +12  */
	if (lev == 24) return (279000L); /* need 66000  +16   */
	if (lev == 25) return (365000L); /* need 86000 + 20   */
	if (lev == 26) return (476000L); /* need 111000 + 25  */
	if (lev == 27) return (617000L); /* need 141000+ 30   */
	if (lev == 28) return (798000L); /* need 181000 + 40  */
	if (lev == 29) return (1034000L); /* need 236000 + 55 */
	return (1750000L);
}

static int
enermod(en)
int en;
{
	if(Role_is('W') || Role_is('P')) return(2 * en);
	else if(Role_is('H') || Role_is('K')) return((3 * en) / 2);
	else if(Role_is('B') || Role_is('V')) return((3 * en) / 4);
	else return(en);
}

int
experience(mtmp, nk)	/* return # of exp points for mtmp after nk killed */
	register struct	monst *mtmp;
	register int	nk;
#if defined(applec)
# pragma unused(nk)
#endif
{
	register struct permonst *ptr = mtmp->data;
	int	i, tmp, tmp2;

	tmp = 1 + mtmp->m_lev * mtmp->m_lev;

/*	For higher ac values, give extra experience */
	if((i = find_mac(mtmp)) < 3) tmp += (7 - i) * (i < 0) ? 2 : 1;

/*	For very fast monsters, give extra experience */
	if(ptr->mmove >= 12) tmp += (ptr->mmove >= 18) ? 5 : 3;

/*	For each "special" attack type give extra experience */
	for(i = 0; i < NATTK; i++) {

	    tmp2 = ptr->mattk[i].aatyp;
	    if(tmp2 > AT_BUTT) {

		if(tmp2 == AT_WEAP) tmp += 5;
		else if(tmp2 == AT_MAGC) tmp += 10;
		else tmp += 3;
	    }
	}

/*	For each "special" damage type give extra experience */
	for(i = 0; i < NATTK; i++) {

	    tmp2 = ptr->mattk[i].adtyp;
	    if(tmp2 > AD_PHYS && tmp2 < AD_BLND) tmp += 2*mtmp->m_lev;
	    else if((tmp2 == AD_DRLI) || (tmp2 == AD_STON) || (tmp2 == AD_SLIM)) tmp += 50;
	    else if(tmp != AD_PHYS) tmp += mtmp->m_lev;
		/* extra heavy damage bonus */
	    if((int)(ptr->mattk[i].damd * ptr->mattk[i].damn) > 23)
		tmp += mtmp->m_lev;
	}

/*	For certain "extra nasty" monsters, give even more */
	if (extra_nasty(ptr)) tmp += (7 * mtmp->m_lev);
	if (ptr->mlet == S_EEL && !Amphibious) tmp *= 2;

/*	For higher level monsters, an additional bonus is given */
	if(mtmp->m_lev > 8) tmp += 50;

#ifdef MAIL
	/* Mail daemons put up no fight. */
	if(mtmp->data == &mons[PM_MAIL_DAEMON]) tmp = 1;
#endif

	return(tmp);
}

void
more_experienced(exp, rexp)
	register int exp, rexp;
{
	u.uexp += exp;
	u.urexp += 4*exp + rexp;
	if(exp
#ifdef SCORE_ON_BOTL
	   || flags.showscore
#endif
	   ) flags.botl = 1;
	if (u.urexp >= (Role_is('W') ? 1000 : 2000))
		flags.beginner = 0;
}

void
losexp()		/* hit by drain life attack */
{
	register int num;

	if (resists_drli(&youmonst)) return;
	if (Antimagic && rn2(2)) {
/*JP		pline("You feel weaker for a moment, but it passes.");*/
		pline("一瞬弱くなったような気がしたが，すぐに過ぎさった．");
		return;
	}
	if (rn2(2)) {
/*JP		pline("You feel weaker for a moment, but it passes.");*/
		pline("一瞬弱くなったような気がしたが，すぐに過ぎさった．");
		return;
		}
	if(u.ulevel > 1) {
/*JP		pline("Goodbye level %d.", u.ulevel--);*/
		pline("さようならレベル%d．", u.ulevel--);
		/* remove intrinsic abilities */
		adjabil(u.ulevel + 1, u.ulevel);
		reset_rndmonst(NON_PM);	/* new monster selection */
	} else
/* STEPHEN WHITE'S NEW CODE */                                
		u.uhp = -1;
	num = newhp();
	u.uhp -= num;
	u.uhpmax -= num;
	u.uhpbase -= num;
	if (Role_is('A')) num = rnd(4) + 1;
	else if (Role_is('B')) num = rnd(2);
	else if (Role_is('C')) num = rnd(2);
	else if (Role_is('D')) num = rnd(5) + 1;
	else if (Role_is('E')) num = rnd(5) + 1;
	else if (Role_is('F')) num = rnd(6) + 2;
	else if (Role_is('G')) num = rnd(3);
	else if (Role_is('H')) num = rnd(6) + 2;
	else if (Role_is('I')) num = rnd(6) + 2;
#ifdef JFIGHTER
        else if (Role_is('J')) num = rnd(4) + 1;
#endif
	else if (Role_is('K')) num = rnd(3);
	else if (Role_is('L')) num = rnd(5) + 1;
	else if (Role_is('M')) num = rnd(5) + 1;
	else if (Role_is('N')) num = rnd(6) + 2;
	else if (Role_is('P')) num = rnd(6) + 2;
	else if (Role_is('R')) num = rnd(4) + 1;
	else if (Role_is('S')) num = rnd(2);
#ifdef TOURIST        
	else if (Role_is('T')) num = rnd(4) + 1;
#endif        
	else if (Role_is('U')) num = rnd(3);
	else if (Role_is('V')) num = rnd(2);
	else if (Role_is('W')) num = rnd(6) + 2;
	else num = rnd(2) + 1;
/*      num = enermod(rn1(u.ulevel/2 + 1, 2));*/        /* M. Stephenson */
	u.uen -= num;
	if (u.uen < 0)		u.uen = 0;
	u.uenmax -= num;
	if (u.uenbase < 0)      u.uenbase = 0;
	u.uexp = newuexp(u.ulevel) - 1;
	flags.botl = 1;
}

/*
 * Make experience gaining similar to AD&D(tm), whereby you can at most go
 * up by one level at a time, extra expr possibly helping you along.
 * After all, how much real experience does one get shooting a wand of death
 * at a dragon created with a wand of polymorph??
 */
void
newexplevel()
{
	register int tmp;

	if(u.ulevel < MAXULEV && u.uexp >= newuexp(u.ulevel)) {

		u.ulevel++;
		if (u.uexp >= newuexp(u.ulevel)) u.uexp = newuexp(u.ulevel) - 1;
/*JP		pline("Welcome to experience level %d.", u.ulevel);*/
		pline("レベル%dにようこそ．", u.ulevel);
		/* give new intrinsics */
		adjabil(u.ulevel - 1, u.ulevel);
		reset_rndmonst(NON_PM);	/* new monster selection */
/* STEPHEN WHITE'S NEW CODE */                
		tmp = newhp();
		u.uhpbase += tmp;                
		u.uhpmax += tmp;
		u.uhp += tmp;
                if (Role_is('A')) u.uenbase += rnd(4) + 1;
                else if (Role_is('B')) u.uenbase += rnd(2);
                else if (Role_is('C')) u.uenbase += rnd(2);
                else if (Role_is('D')) u.uenbase += rnd(5) + 1;
                else if (Role_is('E')) u.uenbase += rnd(5) + 1;
                else if (Role_is('F')) u.uenbase += rnd(6) + 2;
                else if (Role_is('G')) u.uenbase += rnd(3);
                else if (Role_is('H')) u.uenbase += rnd(6) + 2;
                else if (Role_is('I')) u.uenbase += rnd(6) + 2;
#ifdef JFIGHTER
                else if (Role_is('J')) u.uenbase += rnd(4) + 1;
#endif
                else if (Role_is('K')) u.uenbase += rnd(3);
                else if (Role_is('L')) u.uenbase += rnd(5) + 1;
                else if (Role_is('M')) u.uenbase += rnd(5) + 1;
                else if (Role_is('N')) u.uenbase += rnd(6) + 2;
                else if (Role_is('P')) u.uenbase += rnd(6) + 2;
                else if (Role_is('R')) u.uenbase += rnd(4) + 1;
                else if (Role_is('S')) u.uenbase += rnd(2);
#ifdef TOURIST
                else if (Role_is('T')) u.uenbase += rnd(4) + 1;
#endif
                else if (Role_is('U')) u.uenbase += rnd(3);
                else if (Role_is('V')) u.uenbase += rnd(2);
                else if (Role_is('W')) u.uenbase += rnd(6) + 2;
		else u.uenbase += rnd(2) + 1;
		flags.botl = 1;
	}
}

void
pluslvl()
{
	register int num;

/*JP	You_feel("more experienced.");*/
	You("より経験をつんだような気がした．");
	num = newhp();
	u.uhpbase += num;
	u.uhpmax += num;
	u.uhp += num;
	num = enermod(rn1((int)ACURR(A_WIS)/2+1, 2));	/* M. Stephenson */
	u.uenbase += num;
	u.uenmax += num;
	u.uen += num;
	if(u.ulevel < MAXULEV) {
		u.uexp = newuexp(u.ulevel);
/*JP		pline("Welcome to experience level %d.", ++u.ulevel);*/
		pline("レベル%dにようこそ．", ++u.ulevel);
		adjabil(u.ulevel - 1, u.ulevel);
		reset_rndmonst(NON_PM);	/* new monster selection */
	}
	flags.botl = 1;
}

long
rndexp()
{
	register long minexp,maxexp;

	if(u.ulevel == 1)
		return rn2((int)newuexp(1));
	else {
		minexp = newuexp(u.ulevel - 1);
		maxexp = newuexp(u.ulevel);
		return(minexp + rn2((int)(maxexp - minexp)));
	}
}

/*exper.c*/
