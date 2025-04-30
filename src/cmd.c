/*	SCCS Id: @(#)cmd.c	3.2	96/08/04	*/
/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 1985. */
/* NetHack may be freely redistributed.  See license for details. */

/*
**	Japanese version Copyright
**	(c) Issei Numata, Naoki Hamada, Shigehiro Miyashita, 1994-1996
**	changing point is marked `JP' (94/6/7)
**	JNetHack may be freely redistributed.  See license for details. 
*/

#include "hack.h"
#include "func_tab.h"
/* #define DEBUG	/* uncomment for debugging */

/*
 * Some systems may have getchar() return EOF for various reasons, and
 * we should not quit before seeing at least NR_OF_EOFS consecutive EOFs.
 */
#if defined(SYSV) || defined(DGUX) || defined(HPUX)
#define NR_OF_EOFS	20
#endif

#ifdef DEBUG
/*
 * only one "wiz_debug_cmd" routine should be available (in whatever
 * module you are trying to debug) or things are going to get rather
 * hard to link :-)
 */
extern void NDECL(wiz_debug_cmd);
#endif

#ifdef DUMB	/* stuff commented out in extern.h, but needed here */
extern int NDECL(doapply); /**/
extern int NDECL(dorub); /**/
extern int NDECL(dojump); /**/
extern int NDECL(doextlist); /**/
extern int NDECL(dodrop); /**/
extern int NDECL(doddrop); /**/
extern int NDECL(dodown); /**/
extern int NDECL(doup); /**/
extern int NDECL(donull); /**/
extern int NDECL(dowipe); /**/
extern int NDECL(do_mname); /**/
extern int NDECL(ddocall); /**/
extern int NDECL(dotakeoff); /**/
extern int NDECL(doremring); /**/
extern int NDECL(dowear); /**/
extern int NDECL(doputon); /**/
extern int NDECL(doddoremarm); /**/
extern int NDECL(dokick); /**/
extern int NDECL(dofire); /**/
extern int NDECL(dothrow); /**/
extern int NDECL(doeat); /**/
extern int NDECL(done2); /**/
extern int NDECL(doengrave); /**/
extern int NDECL(dopickup); /**/
extern int NDECL(ddoinv); /**/
extern int NDECL(dotypeinv); /**/
extern int NDECL(dolook); /**/
extern int NDECL(doprgold); /**/
extern int NDECL(doprwep); /**/
extern int NDECL(doprarm); /**/
extern int NDECL(doprring); /**/
extern int NDECL(dopramulet); /**/
extern int NDECL(doprtool); /**/
extern int NDECL(dosuspend); /**/
extern int NDECL(doforce); /**/
extern int NDECL(doopen); /**/
extern int NDECL(doclose); /**/
extern int NDECL(dosh); /**/
extern int NDECL(dodiscovered); /**/
extern int NDECL(doset); /**/
extern int NDECL(dotogglepickup); /**/
extern int NDECL(dowhatis); /**/
extern int NDECL(doquickwhatis); /**/
extern int NDECL(dowhatdoes); /**/
extern int NDECL(dohelp); /**/
extern int NDECL(dohistory); /**/
extern int NDECL(doloot); /**/
extern int NDECL(dodrink); /**/
extern int NDECL(dodip); /**/
extern int NDECL(dosacrifice); /**/
extern int NDECL(dopray); /**/
extern int NDECL(doturn); /**/
extern int NDECL(doredraw); /**/
extern int NDECL(doread); /**/
extern int NDECL(dosave); /**/
extern int NDECL(dosearch); /**/
extern int NDECL(doidtrap); /**/
extern int NDECL(dopay); /**/
extern int NDECL(dosit); /**/
extern int NDECL(dotalk); /**/
extern int NDECL(docast); /**/
extern int NDECL(dovspell); /**/
extern int NDECL(dotele); /**/
extern int NDECL(dountrap); /**/
extern int NDECL(doversion); /**/
extern int NDECL(doextversion); /**/
extern int NDECL(doswapweapon); /**/
extern int NDECL(dowield); /**/
extern int NDECL(dowieldquiver); /**/
extern int NDECL(dozap); /**/
extern int NDECL(doorganize); /**/
#endif /* DUMB */

#ifdef OVL1
static int NDECL((*timed_occ_fn));
#endif /* OVL1 */

STATIC_PTR int NDECL(doprev_message);
STATIC_PTR int NDECL(timed_occupation);
STATIC_PTR int NDECL(doextcmd);
#ifdef BORG
STATIC_PTR int NDECL(doborgtoggle);
#endif
STATIC_PTR int NDECL(domonability);
STATIC_PTR int NDECL(polyatwill);
STATIC_PTR int NDECL(playersteal);
STATIC_PTR int NDECL(specialpower);
# ifdef WIZARD
STATIC_PTR int NDECL(wiz_wish);
STATIC_PTR int NDECL(wiz_identify);
STATIC_PTR int NDECL(wiz_map);
STATIC_PTR int NDECL(wiz_genesis);
STATIC_PTR int NDECL(wiz_where);
STATIC_PTR int NDECL(wiz_detect);
STATIC_PTR int NDECL(wiz_level_tele);
STATIC_PTR int NDECL(wiz_show_seenv);
STATIC_PTR int NDECL(wiz_show_vision);
STATIC_PTR int NDECL(wiz_show_wmodes);
#ifdef __BORLANDC__
extern void FDECL(show_borlandc_stats, (winid));
#endif
static void FDECL(count_obj, (struct obj *, long *, long *, BOOLEAN_P, BOOLEAN_P));
static void FDECL(obj_chain, (winid, const char *, struct obj *, long *, long *));
static void FDECL(mon_invent_chain, (winid, const char *, struct monst *, long *, long *));
static void FDECL(mon_chain, (winid, const char *, struct monst *, long *, long *));
static void FDECL(contained, (winid, const char *, long *, long *));
STATIC_PTR int NDECL(wiz_show_stats);
# endif
STATIC_PTR int NDECL(enter_explore_mode);
STATIC_PTR int NDECL(wiz_attributes);

static NEARDATA struct rm *maploc;

#ifdef OVLB
static void FDECL(enlght_line, (const char *,const char *,const char *));
#ifdef UNIX
static void NDECL(end_of_input);
#endif
#endif /* OVLB */

STATIC_DCL char *NDECL(parse);

#ifdef BORG
/* in borg.c */
extern char borg_on;
extern char borg_line[80];
char borg_input(void);
#endif
#ifdef OVL1

STATIC_PTR int
doprev_message()
{
    return nh_doprev_message();
}

/* Count down by decrementing multi */
STATIC_PTR int
timed_occupation() {
	(*timed_occ_fn)();
	if (multi > 0)
		multi--;
	return multi > 0;
}

/* If you have moved since initially setting some occupations, they
 * now shouldn't be able to restart.
 *
 * The basic rule is that if you are carrying it, you can continue
 * since it is with you.  If you are acting on something at a distance,
 * your orientation to it must have changed when you moved.
 *
 * The exception to this is taking off items, since they can be taken
 * off in a number of ways in the intervening time, screwing up ordering.
 *
 *	Currently:	Take off all armor.
 *			Picking Locks / Forcing Chests.
 */
void
reset_occupations() {

	reset_remarm();
	reset_pick();
}

/* If a time is given, use it to timeout this function, otherwise the
 * function times out by its own means.
 */
void
set_occupation(fn, txt, xtime)
int NDECL((*fn));
const char *txt;
int xtime;
{
	if (xtime) {
		occupation = timed_occupation;
		timed_occ_fn = fn;
	} else
		occupation = fn;
	occtxt = txt;
	occtime = 0;
	return;
}

#ifdef REDO

static char NDECL(popch);

/* Provide a means to redo the last command.  The flag `in_doagain' is set
 * to true while redoing the command.  This flag is tested in commands that
 * require additional input (like `throw' which requires a thing and a
 * direction), and the input prompt is not shown.  Also, while in_doagain is
 * TRUE, no keystrokes can be saved into the saveq.
 */
#define BSIZE 20
static char pushq[BSIZE], saveq[BSIZE];
static NEARDATA int phead, ptail, shead, stail;

static char
popch() {
	/* If occupied, return '\0', letting tgetch know a character should
	 * be read from the keyboard.  If the character read is not the
	 * ABORT character (as checked in pcmain.c), that character will be
	 * pushed back on the pushq.
	 */
	if (occupation) return '\0';
	if (in_doagain) return(char)((shead != stail) ? saveq[stail++] : '\0');
	else		return(char)((phead != ptail) ? pushq[ptail++] : '\0');
}

char
pgetchar() {		/* curtesy of aeb@cwi.nl */
	register int ch;

	if(!(ch = popch()))
		ch = nhgetch();
	return((char)ch);
}

/* A ch == 0 resets the pushq */
void
pushch(ch)
char ch;
{
	if (!ch)
		phead = ptail = 0;
	if (phead < BSIZE)
		pushq[phead++] = ch;
	return;
}

/* A ch == 0 resets the saveq.	Only save keystrokes when not
 * replaying a previous command.
 */
void
savech(ch)
char ch;
{
	if (!in_doagain) {
		if (!ch)
			phead = ptail = shead = stail = 0;
		else if (shead < BSIZE)
			saveq[shead++] = ch;
	}
	return;
}
#endif /* REDO */

#endif /* OVL1 */
#ifdef OVLB

STATIC_PTR int
doextcmd()	/* here after # - now read a full-word command */
{
	int idx, retval;

	/* keep repeating until we don't run help or quit */
	do {
	    idx = get_ext_cmd();
	    if (idx < 0) return 0;	/* quit */

	    retval = (*extcmdlist[idx].ef_funct)();
	} while (extcmdlist[idx].ef_funct == doextlist);

	return retval;
}

int
doextlist()	/* here after #? - now list all full-word commands */
{
	register const struct ext_func_tab *efp;
	char	 buf[BUFSZ];
	winid datawin;

	datawin = create_nhwindow(NHW_TEXT);
	putstr(datawin, 0, "");
/*JP
	putstr(datawin, 0, "            Extended Commands List");
*/
	putstr(datawin, 0, "            拡張コマンド一覧");
	putstr(datawin, 0, "");
/*JP
	putstr(datawin, 0, "    Press '#', then type:");
*/
	putstr(datawin, 0, "    '#'を押したあとタイプせよ:");
	putstr(datawin, 0, "");

	for(efp = extcmdlist; efp->ef_txt; efp++) {
		Sprintf(buf, "    %-14s  - %s.", efp->ef_txt, efp->ef_desc);
		putstr(datawin, 0, buf);
	}
	display_nhwindow(datawin, FALSE);
	destroy_nhwindow(datawin);
	return 0;
}

#ifdef BORG
STATIC_PTR int 
doborgtoggle()
{
      char    qbuf[QBUFSZ];
      char    c;
/*JP	Strcpy(qbuf,"Really enable cyborg?");*/
	Strcpy(qbuf,"ボーグモードに入りますか？");
      if ((c = yn_function(qbuf, ynqchars, 'n')) == 'y') {
	borg_on = 1;
/*JP	pline("The cyborg is enabled.... Good luck!");*/
	pline("ボーグモードに入った．．．．幸運を祈る！");
      }
     return 0;
}
#endif 
STATIC_PTR int
domonability()
{
	if (can_breathe(uasmon)) return dobreathe();
	else if (attacktype(uasmon, AT_SPIT)) return dospit();
	else if (u.usym == S_NYMPH) return doremove();
	else if (u.usym == S_UMBER) return doconfuse();
	else if (is_were(uasmon)) return dosummon();
	else if (webmaker(uasmon)) return dospinweb();
	else if (is_hider(uasmon)) return dohide();
	else if(u.umonnum == PM_GREMLIN) {
	    if(IS_FOUNTAIN(levl[u.ux][u.uy].typ)) {
		struct monst *mtmp;
		if ((mtmp = cloneu()) != 0) {
			mtmp->mhpmax = (u.mhmax /= 2);
/*JP			You("multiply.");*/
			You("分裂した．");
			dryup(u.ux,u.uy);
		}
/*JP	    } else pline("There is no fountain here.");*/
	    } else pline("ここには泉はない．");
	}
	else if (u.usym == S_UNICORN) {
	    use_unicorn_horn((struct obj *)0);
	    return 1;
	} else if (u.umonnum == PM_MIND_FLAYER) return domindblast();
	else if (uasmon->msound == MS_SHRIEK) {
/*JP	    You("shriek.");*/
	    You("金切り声をあげた．");
	    if(u.uburied)
/*JP		pline("Unfortunately sound does not carry well through rock.");*/
		pline("不幸にも音は岩をうまく伝わらない．");
	    else aggravate();
	} else if (Upolyd)
/*JP		pline("Any special ability you may have is purely reflexive.");*/
		pline("あなたの持っている特殊能力はどれも受動的だ．");
/*JP	else You("don't have a special ability!");*/
	else You("特殊能力を持っていない！");
	return 0;
}

STATIC_PTR int
enter_explore_mode()
{
	if(!discover && !wizard) {
/*JP		pline("Beware!  From explore mode there will be no return to normal game.");*/
		pline("警告！発見モードに入ったら通常モードには戻れない．");
/*JP		if (yn("Do you want to enter explore mode?") == 'y') {*/
		if (yn("発見モードに移りますか？") == 'y') {
			clear_nhwindow(WIN_MESSAGE);
/*JP			You("are now in non-scoring explore mode.");*/
			You("スコアがのらない発見モードに移行した．");
			discover = TRUE;
		}
/* 追加 by Ooi Takefumi */
#ifdef WIZARD
                else if (yn("ヴィザードモードに移りますか？") == 'y') {
                        clear_nhwindow(WIN_MESSAGE);
                        You("スコアがのらないヴィザードモードに移行した．");
                        wizard = TRUE;
                      }
#endif
/* ここ迄 */

		else {
			clear_nhwindow(WIN_MESSAGE);
/*JP			pline("Resuming normal game.");*/
			pline("通常モードを再開．");
		}
	}
	return 0;
}

STATIC_PTR int
polyatwill()      /* Polymorph at will for Doppelganger class */
{
	int mon;
	boolean scales = ((uarm && uarm->otyp == RED_DRAGON_SCALE_MAIL   
				&& Role_is('F')) ||
			  (uarm && uarm->otyp == WHITE_DRAGON_SCALE_MAIL 
				&& Role_is('I')));  

	if (Role_is('D') || Role_is('L')) {
		/* [Tom] Dopplegangers only polymorph when they want... */
		if ((Role_is('D') /* && u.ulevel > 4 */) ||
		    (Role_is('L') && u.ulevel > 2)) {
		    if(!Upolyd) {
/*JP			if (u.uen < 10) pline("You don't have the energy to polymorph!");*/
			if (u.uen < 10) pline("変化するだけのエネルギーを持っていない！");
			else {
			   u.uen -= 10;
			   if(Polymorph) {
			      if (multi >= 0) {
				 if (occupation) stop_occupation();
				 else nomul(0);
			      }
			      polyself();
			   } else if (u.ulycn >= 0) {
			      if(!Upolyd) {
				  if (multi >= 0) {
				     if (occupation) stop_occupation();
				     else nomul(0);
				  }
				  you_were();
			      } else rehumanize();
			   }
			}
		    } else if (!Role_is('D')) {
			if (u.uen > 9) rehumanize();
/*JP			else pline("You don't have the energy to polymorph!");*/
			else pline("変化するだけのエネルギーを持っていない！");
		    } else rehumanize();
/*JP		} else You("can't polymorph at will yet.");*/
		} else You("まだ自分の意思で変化できない．");
	} else if (Role_is('I') || Role_is('F')) {
		if ((u.ulevel > 13 && u.uen > 49) || (scales && u.uen > 34)) {
		    if (scales) u.uen -= 35; else u.uen -= 50;
			if(!Upolyd) {
			   if (Role_is('F')) mon = PM_RED_DRAGON;
			   else mon = PM_WHITE_DRAGON;
			   if (!(mons[mon].geno & G_GENOD) && scales) {
			      /* allow G_EXTINCT */
/*JP			      You("merge with your scaly armor.");*/
			      You("鱗の鎧と一体化した．");
			      uskin = uarm;
			      uarm = (struct obj *)0;
			   }
			   polymon(mon);
			} else rehumanize();
		} else if (u.ulevel > 6 && u.uen > 24) {
		    u.uen -= 25;
		    if(!Upolyd) {        
			if (Role_is('F')) mon = PM_BABY_RED_DRAGON;
			else mon = PM_BABY_WHITE_DRAGON;
			polymon(mon);
		    } else rehumanize();
		} else 
/*JP		    if (u.ulevel < 7) You("can't polymorph at will yet.");*/
		    if (u.ulevel < 7) You("まだ自分の意思で変化できない．");
/*JP		    else You("don't have the energy to polymorph.");*/
		    else You("変化するだけのエネルギーを持っていない．");
	} else {
/*JP		pline("You can't polymorph at will.");*/
		pline("あなたは自分の意思で変化できない．");
		flags.botl = 1;
		return 0;
	}
	flags.botl = 1;
	return 1;
}

STATIC_PTR int
playersteal()
{
	register int x, y;
        int temp, chanch, base, dexadj, statbonus = 0;
	boolean no_steal = FALSE;

	if (nohands(uasmon)) {
/*JP		pline("Could be hard without hands ...");*/
		pline("手を使わずに行なうのはつらい．．．");
		no_steal = TRUE;
	} else
	if (near_capacity() > SLT_ENCUMBER) {
/*JP		Your("load is too heavy to attempt to steal.");*/
		Your("荷物の重量が重過ぎて盗みを行なうことができない．");
		no_steal = TRUE;
	}
	if (no_steal) {
		/* discard direction typeahead, if any */
		display_nhwindow(WIN_MESSAGE, TRUE);    /* --More-- */
		return 0;
	}

	if(!getdir(NULL)) return(0);
	if(!u.dx && !u.dy) return(0);

	x = u.ux + u.dx;
	y = u.uy + u.dy;
	
	if(u.uswallow) {
/*JP		pline("You search around but don't find anything.");*/
		pline("あなたは辺りを捜したが何も見つからなかった.");
		return(1);
	}

	u_wipe_engr(2);

	maploc = &levl[x][y];

	if(MON_AT(x, y)) {
		register struct monst *mdat = m_at(x, y);

		/* calculate chanch of sucess */
		base = 5;
		dexadj = 1;
		if (Role_is('R')) {
			base = 5 + (u.ulevel * 2);
			dexadj = 3;
		}
		if (ACURR(A_DEX) < 10) statbonus = (ACURR(A_DEX) - 10) * dexadj;
		else 
		if (ACURR(A_DEX) > 14) statbonus = (ACURR(A_DEX) - 14) * dexadj;

		chanch = base + statbonus;

		if (uarmg && !uarmg->otyp == GAUNTLETS_OF_DEXTERITY)
				chanch -= 5;
		if (!uarmg)     chanch += 5;
		if (uarms)      chanch -= 10;
		if (uarm && uarm->owt < 75)       chanch += 10;
		else if (uarm && uarm->owt < 125) chanch += 5;
		else if (uarm && uarm->owt < 175) chanch += 0;
		else if (uarm && uarm->owt < 225) chanch -= 5;
		else if (uarm && uarm->owt < 275) chanch -= 10;
		else if (uarm && uarm->owt < 325) chanch -= 15;
		else if (uarm && uarm->owt < 375) chanch -= 20;
		else if (uarm)                    chanch -= 25;
		if (chanch < 5) chanch = 5;
		if (chanch > 95) chanch = 95;
		if (rnd(100) < chanch || mdat->mtame) {

			if (mdat->mgold) {
				temp = (u.ulevel * rn1(25,25));
				if (temp > mdat->mgold) temp = mdat->mgold;
				u.ugold += temp;
				mdat->mgold -= temp;
/*JP				You("steal %d gold.",temp);*/
				You("%dゴールド盗んだ．",temp);
			} else
/*JP				You("don't find anything to steal.");*/
				You("盗む物が見つからなかった．");

			exercise(A_DEX, TRUE);
			return(1);
		} else {
/*JP			You("failed to steal anything.");*/
			You("何も盗むことができなかった．");
			setmangry(mdat);
	       }
	} else {
/*JP		pline("I don't see anybody to rob there!");*/
		pline("盗む相手が見当たりません！");
		return(0);
	}
	return(0);
} 

STATIC_PTR int
specialpower()      /* Special class abilites [modified by Tom] */
{
	/*
	 * STEPHEN WHITE'S NEW CODE
	 *
	 * For clarification, lastuse (as declared in decl.{c|h}) is the
	 * actual length of time the power is active, nextuse is when you can
	 * next use the ability.
	 */

	if (u.unextuse) {
/*JP	    You("have to wait %s before using your ability again.",
		(u.unextuse > 500) ? "for a while" : "a little longer");*/
	    You("能力を再び使うには%s待たなければならない",
		(u.unextuse > 500) ? "しばらくの間" : "ちょっと");
	    return(0);
	  } else switch (u.role) {
	    case 'A':
	    case 'G':
/*JP		Your("ability, gem identification, is automatic.");*/
		Your("能力『宝石の鑑定』は自動的に行なわれています．");
		break;
	    case 'P':
/*JP		Your("ability, bless and curse detection, is automatic.");*/
		Your("能力『祝福と呪いの判別』は自動的に行なわれています．");
		break;
	    case 'D':
	    case 'L':
/*JP		Your("ability, polymorphing, uses the alt-y key.");*/
		Your("能力『変化』はalt-yキーを使います．");
		break;
	    case 'R':
/*JP		Your("ability, stealing, uses the alt-b key.");*/
		Your("能力『盗む』はalt-bキーを使います．");
		break;
	    case 'M':
/*JP		Your("special ability is unarmed combat, and it is automatic.");*/
		Your("特殊能力は素手で闘うことで，それは自動的に行なわれています．");
		break;
	    case 'C':
	    case 'T':
/*JP		You("don't have a special ability!");*/
		You("特殊能力を持っていません！");
		break;
	    case 'B':
/*JP		You("fly into a berserk rage!");*/
		You("狂暴化した！");
		u.ulastuse = d(2,8) + u.ulevel;
		Fast += u.ulastuse;
		u.unextuse = rn1(1000,500);
		return(0);
		break;
	    case 'F':
	    case 'I':
	    case 'N':
	    case 'W':
		if(Hallucination || Stunned || Confusion) {
/*JP		    You("can't concentrate right now!");*/
		    You("意識を集中することができない！");
		    break;
		} else if((ACURR(A_INT) + ACURR(A_WIS)) < rnd(60)) {
/*JP		    You("try to study but, unfortunately, accomplish nothing...");*/
		    You("調べようとした．しかし不幸なことに何も判らなかった．．．");
		    u.unextuse = rn1(500,500);
		    break;
		} else if(invent) {
		    int ret;
/*JP		    You("start to study.");
		    ret = ggetobj("identify", identify, 1, FALSE);*/
		    You("調べ始めた．");
		    ret = ggetobj("識別する", identify, 1, FALSE);
		    if (ret < 0) break;     /* quit or no eligible items */
		} else {
/*JP		    pline("Identify what?");*/
		    pline("識別する？何を？");
		    break;
		}
		u.unextuse = rn1(500,1500);
		break;
	    case 'E':
	    case 'U':
	    case 'V':
		if(!uwep) {
/*JP		    You("are not wielding a weapon!");*/
		    You("武器を装備していない！");
		    break;
		} else if(uwep->known == TRUE) {
/*JP		    You("already know all about your weapon!");*/
		    You("この武器について既に知り尽くしている！");
		    break;
		} else {
/*JP		    You("study and practice with your weapon.");*/
		    You("この武器の使い方を調べ，練習した．");
		    if (rnd(15) <= ACURR(A_INT)) {
/*JP			You("were able to learn more about your weapon!");*/
			You("この武器について更に学ぶことができる！");
			makeknown(uwep->otyp);
			uwep->known = TRUE;
		    } else
/*JP			pline("Unfortunately, you didn't learn anything new.");*/
			pline("不幸にも，何も新しく学ぶことが無かった．");
		}
		u.unextuse = rn1(250,250);
		break;
	    case 'H':
		if (Hallucination || Stunned || Confusion) {
/*JP		    You("are in no condition to perform surgery!");*/
		    You("診察を行なえる様な状態ではない！");
		    break;
		}
		if (Sick) {
		    if(carrying(SCALPEL)) {
/*JP			pline("Using your scalpel (ow!), you cure your infection!");*/
			pline("メスを使った『うっ！』．あなたは自分の病気を治療した！");
			make_sick(0L,(char *)0, TRUE,SICK_ALL);
			if(u.uhp > 6) u.uhp -= 5;
			else          u.uhp = 1;
			u.unextuse = rn1(500,500);
			break;
/*JP		    } else pline("If only you had a scalpel...");*/
		    } else pline("メスさえ持っていたら．．．");
		}
		if (u.uhp < u.uhpmax) {
		    if(carrying(MEDICAL_KIT)) {
/*JP			pline("Using your medical kit, you bandage your wounds.");*/
			pline("医療道具を使った．あなたは傷口に包帯を巻いた．");
			u.uhp += (u.ulevel * (rnd(2)+1)) + rn1(5,5);
		    } else {
/*JP			pline("You bandage your wounds as best you can.");*/
			pline("あなたは最善を尽くして包帯を傷口に巻いた．");
			u.uhp += (u.ulevel) + rn1(5,5);
		    }
		    u.unextuse = rn1(1000,500);
		    if (u.uhp > u.uhpmax) u.uhp = u.uhpmax;
/*JP		} else pline("You don't need your healing powers!");*/
		} else pline("あなたは治療する必要がない！");
		break;
	    case 'K':
		if (u.uhp < u.uhpmax || Sick) {
/*JP			if (Sick) You("lay your hands on the foul sickness...");
			pline("A warm glow spreads through your body!");*/
			if (Sick) You("調子の悪い所に両手を当てた．．．");
			pline("暖かい光が体中を駆け巡った！");
			if(Sick) make_sick(0L,(char*)0, TRUE, SICK_ALL);
			else     u.uhp += (u.ulevel * 4);
			if (u.uhp > u.uhpmax) u.uhp = u.uhpmax;
			u.unextuse = 3000;
/*JP		} else pline("Nothing happens...");*/
		} else pline("何も起こらなかった．．．");
		break;
	    case 'S':
/*JP		You("scream \"KIIIII!\"");*/
		You("金切り声をあげた\"キーーーーー！\"");
		aggravate();
		u.ulastuse = rnd(2) + 1;
		u.unextuse = rn1(1000,500);
		return(0);
		break;
/* 追加 by Ooi Takefumi */
	    case 'J':
		Invulnerable += (u.ulevel * 4 + rn1(4,8));
		You("星の力を吸収した．");
		Your("体は聖なる光に覆われた！");
		u.unextuse = rn1(500,250);
		Invulnerable |= FROMOUTSIDE;
		break;
/* ここ迄 */
	    default:
		break;
	  }
	  return(0);
}

#ifdef WIZARD
STATIC_PTR int
wiz_wish()	/* Unlimited wishes for debug mode by Paul Polderman */
{
	if (wizard) {
	    boolean save_verbose = flags.verbose;

	    flags.verbose = FALSE;
	    makewish();
	    flags.verbose = save_verbose;
	    (void) encumber_msg();
	} else
/*JP	    pline("Unavailable command '^W'.");*/
	    pline("'^W'コマンドは使えない．");
	return 0;
}

STATIC_PTR int
wiz_identify()
{
	if (wizard)	identify_pack(0);
/*JP	else		pline("Unavailable command '^I'.");*/
	else		pline("'^I'コマンドは使えない．");
	return 0;
}

STATIC_PTR int
wiz_map()
{
	if (wizard)     {
	    do_mapping();
	    trap_detect((struct obj *)0);
	    object_detect((struct obj *)0,0);
	}
/*JP	else		pline("Unavailable command '^F'.");*/
	else		pline("'^F'コマンドは使えない．");
	return 0;
}

STATIC_PTR int
wiz_gain_level()
{
	if (wizard) pluslvl();
/*JP	else            pline("Unavailable command '^J'.");*/
	else		pline("'^J'コマンドは使えない．");
	return 0;
}



STATIC_PTR int
wiz_genesis()
{
	if (wizard)	(void) create_particular();
/*JP	else		pline("Unavailable command '^G'.");*/
	else		pline("'^G'コマンドは使えない．");
	return 0;
}

STATIC_PTR int
wiz_where()
{
	if (wizard) print_dungeon();
/*JP	else	    pline("Unavailable command '^O'.");*/
	else	    pline("'^O'コマンドは使えない．");
	return 0;
}

STATIC_PTR int
wiz_detect()
{
	if(wizard)  (void) findit();
/*JP	else	    pline("Unavailable command '^E'.");*/
	else	    pline("'^E'コマンドは使えない．");
	return 0;
}

STATIC_PTR int
wiz_level_tele()
{
	if (wizard)	level_tele();
/*JP	else		pline("Unavailable command '^V'.");*/
	else		pline("'^V'コマンドは使えない．");
	return 0;
}

STATIC_PTR int
wiz_show_seenv()
{
	winid win;
	int x, y, v, startx, stopx, curx;
	char row[COLNO+1];

	win = create_nhwindow(NHW_TEXT);
	/*
	 * Each seenv description takes up 2 characters, so center
	 * the seenv display around the hero.
	 */
	startx = max(1, u.ux-(COLNO/4));
	stopx = min(startx+(COLNO/2), COLNO);
	/* can't have a line exactly 80 chars long */
	if (stopx - startx == COLNO/2) startx++;

	for (y = 0; y < ROWNO; y++) {
	    for (x = startx, curx = 0; x < stopx; x++, curx += 2) {
		if (x == u.ux && y == u.uy) {
		    row[curx] = row[curx+1] = '@';
		} else {
		    v = levl[x][y].seenv & 0xff;
		    if (v == 0)
			row[curx] = row[curx+1] = ' ';
		    else
			Sprintf(&row[curx], "%02x", v);
		}
	    }
	    /* remove trailing spaces */
	    for (x = curx-1; x >= 0; x--)
		if (row[x] != ' ') break;
	    row[x+1] = '\0';

	    putstr(win, 0, row);
	}
	display_nhwindow(win, TRUE);
	destroy_nhwindow(win);
	return 0;
}

STATIC_PTR int
wiz_show_vision()
{
	winid win;
	int x, y, v;
	char row[COLNO+1];

	win = create_nhwindow(NHW_TEXT);
	Sprintf(row, "Flags: 0x%x could see, 0x%x in sight, 0x%x temp lit",
		COULD_SEE, IN_SIGHT, TEMP_LIT);
	putstr(win, 0, row);
	putstr(win, 0, "");
	for (y = 0; y < ROWNO; y++) {
	    for (x = 1; x < COLNO; x++) {
		if (x == u.ux && y == u.uy)
		    row[x] = '@';
		else {
		    v = viz_array[y][x]; /* data access should be hidden */
		    if (v == 0)
			row[x] = ' ';
		    else
			row[x] = '0' + viz_array[y][x];
		}
	    }
	    /* remove trailing spaces */
	    for (x = COLNO-1; x >= 1; x--)
		if (row[x] != ' ') break;
	    row[x+1] = '\0';

	    putstr(win, 0, &row[1]);
	}
	display_nhwindow(win, TRUE);
	destroy_nhwindow(win);
	return 0;
}

STATIC_PTR int
wiz_show_wmodes()
{
	winid win;
	int x,y;
	char row[COLNO+1];
	struct rm *lev;

	win = create_nhwindow(NHW_TEXT);
	for (y = 0; y < ROWNO; y++) {
	    for (x = 0; x < COLNO; x++) {
		lev = &levl[x][y];
		if (x == u.ux && y == u.uy)
		    row[x] = '@';
		if (IS_WALL(lev->typ) || lev->typ == SDOOR)
		    row[x] = '0' + (lev->wall_info & WM_MASK);
		else if (lev->typ == CORR)
		    row[x] = '#';
		else if (IS_ROOM(lev->typ) || IS_DOOR(lev->typ))
		    row[x] = '.';
		else
		    row[x] = 'x';
	    }
	    row[COLNO] = '\0';
	    putstr(win, 0, row);
	}
	display_nhwindow(win, TRUE);
	destroy_nhwindow(win);
	return 0;
}

#endif /* WIZARD */

/* -enlightenment- */
static winid en_win;
static const char
/*JP	*You_ = "You ",
	*are  = "are ",  *were  = "were ",
	*have = "have ", *had   = "had ",
	*can  = "can ",  *could = "could ";*/
	*You_ = "あなたは",
	*are  = "である",  *were  = "であった",
	*have = "をもっている", *had   = "をもっていた",
	*can  = "できる",  *could = "できた";

#define enl_msg(prefix,present,past,suffix) \
			enlght_line(prefix, suffix, final ? past : present)
#define you_are(attr)	enl_msg(You_,are,were,attr)
#define you_have(attr)	enl_msg(You_,have,had,attr)
#define you_can(attr)	enl_msg(You_,can,could,attr)

static void
enlght_line(start, middle, end)
const char *start, *middle, *end;
{
	char buf[BUFSZ];

	Sprintf(buf, "%s%s%s．", start, middle, end);
	putstr(en_win, 0, buf);
}

void
enlightenment(final)
int final;	/* 0 => still in progress; 1 => over, survived; 2 => dead */
{
	int ltmp;
	char buf[BUFSZ];

	en_win = create_nhwindow(NHW_MENU);
/*JP	putstr(en_win, 0, final ? "Final Attributes:" : "Current Attributes:");*/
	putstr(en_win, 0, final ? "最終属性：" : "現在の属性：");
	putstr(en_win, 0, "");

#ifdef CROWN
	if (u.uevent.uhand_of_elbereth) {
	    static const char *hofe_titles[3] = {
/*JP				"the Hand of Elbereth",
				"the Envoy of Balance",
				"the Glory of Arioch"*/
				"エルベレスの御手",
				"調和の使者",
				"アリオッチの名誉"
	    };
/*JP*/
	    if(u.uevent.uhand_of_elbereth == 2) {
	      you_are(hofe_titles[u.uevent.uhand_of_elbereth - 1]);
	    } else {
	      you_have(hofe_titles[u.uevent.uhand_of_elbereth - 1]);
	    }
	}
#endif

	/* note: piousness 20 matches MIN_QUEST_ALIGN (quest.h) */
#if 0 /*JP*/
	if (u.ualign.record >= 20)	you_are("piously aligned");
	else if (u.ualign.record > 13)	you_are("devoutly aligned");
	else if (u.ualign.record > 8)	you_are("fervently aligned");
	else if (u.ualign.record > 3)	you_are("stridently aligned");
	else if (u.ualign.record == 3)	you_are("aligned");
	else if (u.ualign.record > 0)	you_are("haltingly aligned");
	else if (u.ualign.record == 0)	you_are("nominally aligned");
	else if (u.ualign.record >= -3)	you_have("strayed");
	else if (u.ualign.record >= -8)	you_have("sinned");
	else you_have("transgressed");
#endif /*JP*/
	if (u.ualign.record >= 20)	you_are("敬虔な人間");
	else if (u.ualign.record > 13)	you_are("信心深い人間");
	else if (u.ualign.record > 8)	you_are("熱烈な人間");
	else if (u.ualign.record > 3)	you_are("声のかん高い人間");
	else if (u.ualign.record == 3)	you_are("普通の人間");
	else if (u.ualign.record > 0)	you_are("どもりの人間");
	else if (u.ualign.record == 0)	you_are("有名無実の人間");
	else if (u.ualign.record >= -3)	you_are("迷惑な人間");
	else if (u.ualign.record >= -8)	you_are("許しがたい罪を負った人間");
	else you_are("逸脱した人間");
#ifdef WIZARD
	if (wizard) {
		Sprintf(buf, " %d", u.ualign.record);
/*JP		enl_msg("Your alignment ", "is", "was", buf);*/
		enl_msg("あなたの属性値は", "である", "であった",buf);
	}
#endif
	if (Invulnerable) enl_msg(You_, "である", "であった", "不死身");
	if (Telepat) you_have("テレパシー能力");
	if (Searching) you_have("探査能力");
	if (Teleportation) you_can("瞬間移動");
	if (Teleport_control) you_have("瞬間移動の制御能力");
	if (See_invisible) enl_msg(You_, "見れる", "見れた", "見えないものを");
	if (Invisible) you_are("透明");
	else if (Invis) you_are("他人に対して透明");
	/* ordinarily "visible" is redundant; this is a special case for
	   the situation when invisibility would be an expected attribute */
	else if ((HInvis & I_BLOCKED) != 0L &&
		 ((HInvis & ~I_BLOCKED) != 0L || pm_invisible(uasmon)))
	    you_are("まる見え");
	if (Fast) you_have((Fast & ~INTRINSIC) ? "とても素早く行動する能力" : 
						"素早く行動する能力");
	if (Stealth) you_have("人目を盗む能力");
	if (Regeneration) you_have("再生能力");
	if (Hunger) enl_msg("あなたはすぐに", "腹が減る", "腹が減った", "");
	if (Conflict) enl_msg("あなたは", "引き起こしている",
				"引き起こしていた","闘争を");
	if (Aggravate_monster) enl_msg("あなたは","反感をかっている",
				"反感をかっていた","");
	if (Poison_resistance) you_have("毒への耐性");
	if (Fire_resistance) you_have("火への耐性");
	if (Cold_resistance) you_have("寒さへの耐性");
	if (Shock_resistance) you_have("ショックへの耐性");
	if (Sleep_resistance) you_have("眠りへの耐性");
	if (Disint_resistance) you_have("粉砕への耐性");
	if (HDrain_resistance) you_have("レベルドレインへの耐性");
	if (HSick_resistance) you_have("病気への免疫");
	if (Free_action) you_have("自由行動能力");
	if (Protection_from_shape_changers)
		you_have("変化怪物への耐性");
	if (Polymorph) enl_msg("あなたは","変化している","変化していた","");
	if (Polymorph_control) you_have("変化の制御能力");
	if (HHalluc_resistance)
		you_have("幻覚への耐性");
	if (final) {
		if (Hallucination) you_are("幻覚状態");
		if (Stunned) you_are("くらくら状態");
		if (Confusion) you_are("混乱状態");
		if (Sick) {
			if (u.usick_type & SICK_VOMITABLE)
				enl_msg("あなたは食中毒で気分が", "悪い",
					"悪かった", "");
			if (u.usick_type & SICK_NONVOMITABLE)
				enl_msg("あなたは病気で気分が", "悪い",
					"悪かった", "");
		}
		if (Blinded) you_are("盲目");
	}
	if (Wounded_legs) {
		Sprintf(buf, "あなたは%sを負傷して", makeplural(body_part(LEG)));
		enl_msg(buf,"いる","いた","");
	}
	if (Glib) {
		Sprintf(buf, "%sがぬるぬるして", makeplural(body_part(FINGER)));
		enl_msg(buf,"いる","いた","");
	}
	if (Strangled) {
		Sprintf(buf, "あなたは%s", (u.uburied) ? "窒息して" :
							 "首を絞められて");
		enl_msg(buf,"いる","いた","");
	}
	if (Stoned) you_are("石化状態");
	if (Slimed) enl_msg("あなたの体はスライムへと","変化している",
				"変化していた","");
	if (Lifesaved)
		enl_msg("あなたの生命は","保存されている","保存されていた","");
	if (Adornment) you_have("装飾品");
	if (Undead_warning) enl_msg("あなたは不死の怪物に対して",
					"警戒能力を持っている",
					"警戒能力を持っていた","");
	if (Warning) enl_msg("あなたは","警戒能力を持っている",
				"警戒能力を持っていた","");
	if (Protection) enl_msg("あなたは","守られている","守られていた","");
	if (Reflecting) you_have("反射能力");
	if ((HLevitation & (I_SPECIAL|W_ARTI)) != 0L &&
	    (HLevitation & ~(I_SPECIAL|W_ARTI|TIMEOUT)) == 0L &&
	     !is_floater(uasmon)) enl_msg("あなたは自分の意志で浮いて", 
						"いる", "いた", "");
	else if (Levitation) you_are("浮遊状態");	/* without control */
	else if (is_flyer(uasmon)) you_can("飛行");
	if (Fumbling) enl_msg("あなたはよく物を","落している","落した","");
	if (Jumping) you_can("跳躍");
	if (Wwalking) you_can("水上歩行");
	if (Swimming) you_can("泳ぐことが");        
	if (passes_walls(uasmon)) you_can("壁を通りぬけることが");
	if (Breathless) you_can("空気なしで生存することが");
	else if (Amphibious) you_can("水中呼吸");
	if (Antimagic) you_have("魔法防御能力");
	if (Displaced) you_have("幻影能力");
	if (Clairvoyant) you_have("千里眼能力");
	if (u.uelf_drow) you_are("デゥロウ");
	if (u.ulycn >= LOW_PM) {
		Strcpy(buf, jtrns_mon(mons[u.ulycn].mname, flags.female));
		you_are(buf);
	}
	if (Upolyd) {
/*JP	    if (u.ulycn >= LOW_PM) Strcpy(buf, "in beast form");*/
	    if (u.ulycn >= LOW_PM) Strcpy(buf, "あなたは獣の姿をして");
/*JP	    else Sprintf(buf, "polymorphed into %s", an(uasmon->mname));*/
	    else Sprintf(buf, "あなたは%sに変化して", jtrns_mon(uasmon->mname, flags.female));
#ifdef WIZARD
 	    if (wizard) Sprintf(eos(buf), " (%d)", Upolyd);
#endif
	    enl_msg(buf, "いる", "いた", "");
	}
	if (Luck) {
	    ltmp = abs((int)Luck);
	    Sprintf(buf, "%s%s",
		    ltmp >= 10 ? "猛烈に" : ltmp >= 5 ? "とても" : "",
		    Luck < 0 ? "不幸" : "幸福");
#ifdef WIZARD
	    if (wizard) Sprintf(eos(buf), " (%d)", Luck);
#endif
	    you_are(buf);
	}
#ifdef WIZARD
	 else if (wizard) enl_msg("あなたの運はゼロ", "である", 
					"であった", "");
#endif
 	if (u.moreluck > 0) you_have("さらなる幸運");
	else if (u.moreluck < 0) you_have("さらなる悪運");
	if (carrying(LUCKSTONE) || stone_luck(TRUE)) {
	    ltmp = stone_luck(FALSE);
	    if (ltmp <= 0)
		enl_msg("悪運は", "去っていない", "去っていなかった", "");
	    if (ltmp >= 0)
		enl_msg("幸運は", "去っていない", "去っていなかった", "");
	}

	if (u.ugangr) {
	    Sprintf(buf, "%sは%s怒って", u_gname(),
		    u.ugangr > 6 ? "猛烈に" : u.ugangr > 3 ? "とても" : "");
	    if(final)
	      Strcat(buf, "いた");
	    else
	      Strcat(buf, "いる");
#ifdef WIZARD
	    if (wizard) Sprintf(eos(buf), " (%d)", u.ugangr);
#endif
	    enl_msg("", buf, buf, "");
	} else
	    /*
	     * We need to suppress this when the game is over, because death
	     * can change the value calculated by can_pray(), potentially
	     * resulting in a false claim that you could have prayed safely.
	     */
	  if (!final) {
	    if(can_pray(FALSE))
	      Sprintf(buf, "できる");
	    else
	      Sprintf(buf, "できない");
#ifdef WIZARD
	    if (wizard) Sprintf(eos(buf), " (%d)", u.ublesscnt);
#endif
	    enl_msg("あなたは", buf, buf, "安全に祈ることが");
	    if(!u.unextuse)
	      Sprintf(buf, "できる");
	    else
	      Sprintf(buf, "できない");
#ifdef WIZARD
	    if (wizard) Sprintf(eos(buf), " (%d)", u.unextuse);
#endif
	    enl_msg("あなたは", buf, buf, "特殊能力を使うことが");
	}

    {
	const char *p;

	buf[0] = '\0';
	if (final < 2) {    /* still in progress, or quit/escaped/ascended */
	    p = "死んだ後復活してた";
	    switch (u.umortality) {
	    case 0:  p = !final ? (char *)0 : "生き延びていた";  break;
	    case 1:  Strcpy(buf, "一度");  break;
	    case 2:  Strcpy(buf, "二度");  break;
	    case 3:  Strcpy(buf, "三度");  break;
	    default: Sprintf(buf, "%d回", u.umortality);
		     break;
	    }
	} else {		/* game ended in character's death */
	    p = "死んでいる";
	    switch (u.umortality) {
	    case 0:  impossible("dead without dying?");
	    case 1:  break;			/* just "are dead" */
	    default: Sprintf(buf, " (%d回！)", u.umortality);
		     break;
	    }
	}
	if (p) enl_msg(You_, "死んでいる", p, buf);
    }

	display_nhwindow(en_win, TRUE);
	destroy_nhwindow(en_win);
	return;

#if 0 /*JP*/
	if (Invulnerable) you_are("invulnerable");
	if (Telepat) you_are("telepathic");
	if (Searching) you_have("automatic searching");
	if (Teleportation) you_can("teleport");
	if (Teleport_control) you_have("teleport control");
	if (See_invisible) enl_msg(You_, "see", "saw", " invisible");
	if (Invisible) you_are("invisible");
	else if (Invis) you_are("invisible to others");
	/* ordinarily "visible" is redundant; this is a special case for
	   the situation when invisibility would be an expected attribute */
	else if ((HInvis & I_BLOCKED) != 0L &&
		 ((HInvis & ~I_BLOCKED) != 0L || pm_invisible(uasmon)))
	    you_are("visible");
	if (Fast) you_are((Fast & ~INTRINSIC) ? "very fast" : "fast");
	if (Stealth) you_are("stealthy");
	if (Regeneration) enl_msg("You regenerate", "", "d", "");
	if (Hunger) enl_msg("You hunger", "", "ed", " rapidly");
	if (Conflict) enl_msg("You cause", "", "d", " conflict");
	if (Aggravate_monster) enl_msg("You aggravate", "", "d", " monsters");
	if (Poison_resistance) you_are("poison resistant");
	if (Fire_resistance) you_are("fire resistant");
	if (Cold_resistance) you_are("cold resistant");
	if (Shock_resistance) you_are("shock resistant");
	if (Sleep_resistance) you_are("sleep resistant");
	if (Disint_resistance) you_are("disintegration-resistant");
	if (HDrain_resistance) you_are("level-drain resistant");
	if (HSick_resistance) you_are("immune to sickness");
	if (Free_action) you_have("free action");
	if (Protection_from_shape_changers)
		you_are("protected from shape changers");
	if (Polymorph) you_are("polymorphing");
	if (Polymorph_control) you_have("polymorph control");
	if (HHalluc_resistance)
		enl_msg("You resist", "", "ed", " hallucinations");
	if (final) {
		if (Hallucination) you_are("hallucinating");
		if (Stunned) you_are("stunned");
		if (Confusion) you_are("confused");
		if (Sick) {
			if (u.usick_type & SICK_VOMITABLE)
				you_are("sick from food poisoning");
			if (u.usick_type & SICK_NONVOMITABLE)
				you_are("sick from illness");
		}
		if (Blinded) you_are("blinded");
	}
	if (Wounded_legs) {
		Sprintf(buf, "wounded %s", makeplural(body_part(LEG)));
		you_have(buf);
	}
	if (Glib) {
		Sprintf(buf, "slippery %s", makeplural(body_part(FINGER)));
		you_have(buf);
	}
	if (Strangled) you_are((u.uburied) ? "buried" : "being strangled");
	if (Stoned) you_are("turning to stone");
	if (Slimed) you_are("turning into slime");
	if (Lifesaved)
		enl_msg("Your life ", "will be", "would have been", " saved");
	if (Adornment) you_are("adorned");
	if (Undead_warning) you_are("warned of undead");
	if (Warning) you_are("warned");
	if (Protection) you_are("protected");
	if (Reflecting) you_have("reflection");
	if ((HLevitation & (I_SPECIAL|W_ARTI)) != 0L &&
	    (HLevitation & ~(I_SPECIAL|W_ARTI|TIMEOUT)) == 0L &&
	    !is_floater(uasmon)) you_are("levitating, at will");
	else if (Levitation) you_are("levitating");	/* without control */
	else if (is_flyer(uasmon)) you_can("fly");
	if (Fumbling) enl_msg("You fumble", "", "d", "");
	if (Jumping) you_can("jump");
	if (Wwalking) you_can("walk on water");
	if (Swimming) you_can("swim");        
	if (passes_walls(uasmon)) you_can("walk through walls");
	if (Breathless) you_can("survive without air");
	else if (Amphibious) you_can("breathe water");
	if (Antimagic) you_are("magic-protected");
	if (Displaced) you_are("displaced");
	if (Clairvoyant) you_are("clairvoyant");
	if (u.uelf_drow) you_are("a Drow");
	if (u.ulycn >= LOW_PM) {
		Strcpy(buf, an(mons[u.ulycn].mname));
		you_are(buf);
	}
	if (Upolyd) {
	    if (u.ulycn >= LOW_PM) Strcpy(buf, "in beast form");
	    else Sprintf(buf, "polymorphed into %s", an(uasmon->mname));
#ifdef WIZARD
 	    if (wizard) Sprintf(eos(buf), " (%d)", Upolyd);
#endif
	    you_are(buf);
	}
	if (Luck) {
	    ltmp = abs((int)Luck);
	    Sprintf(buf, "%s%slucky",
		    ltmp >= 10 ? "extremely " : ltmp >= 5 ? "very " : "",
		    Luck < 0 ? "un" : "");
#ifdef WIZARD
	    if (wizard) Sprintf(eos(buf), " (%d)", Luck);
#endif
	    you_are(buf);
	}
#ifdef WIZARD
	 else if (wizard) enl_msg("Your luck ", "is", "was", " zero");
#endif
	if (u.moreluck > 0) you_have("extra luck");
	else if (u.moreluck < 0) you_have("reduced luck");
	if (carrying(LUCKSTONE) || stone_luck(TRUE)) {
	    ltmp = stone_luck(FALSE);
	    if (ltmp <= 0)
		enl_msg("Bad luck ", "does", "did", " not time out for you");
	    if (ltmp >= 0)
		enl_msg("Good luck ", "does", "did", " not time out for you");
	}

	if (u.ugangr) {
	    Sprintf(buf, " %sangry with you",
		    u.ugangr > 6 ? "extremely " : u.ugangr > 3 ? "very " : "");
#ifdef WIZARD
	    if (wizard) Sprintf(eos(buf), " (%d)", u.ugangr);
#endif
	    enl_msg(u_gname(), " is", " was", buf);
	} else
	    /*
	     * We need to suppress this when the game is over, because death
	     * can change the value calculated by can_pray(), potentially
	     * resulting in a false claim that you could have prayed safely.
	     */
	  if (!final) {
#if 0
	    /* "can [not] safely pray" vs "could [not] have safely prayed" */
	    Sprintf(buf, "%s%ssafely pray%s", can_pray(FALSE) ? "" : "not ",
		    final ? "have " : "", final ? "ed" : "");
#else
	    Sprintf(buf, "%ssafely pray", can_pray(FALSE) ? "" : "not ");
#endif
#ifdef WIZARD
	    if (wizard) Sprintf(eos(buf), " (%d)", u.ublesscnt);
#endif
	    you_can(buf);
#if 0
	    Sprintf(buf, "%s%suse%s your special", !u.unextuse ? "" : "not ",
		    final ? "have " : "", final ? "d" : "");
#else
	    Sprintf(buf, "%suse your special", !u.unextuse ? "" : "not ");
#endif
#ifdef WIZARD
	    if (wizard) Sprintf(eos(buf), " (%d)", u.unextuse);
#endif
	    you_can(buf);
	}

    {
	const char *p;

	buf[0] = '\0';
	if (final < 2) {    /* still in progress, or quit/escaped/ascended */
	    p = "survived after being killed ";
	    switch (u.umortality) {
	    case 0:  p = !final ? (char *)0 : "survived";  break;
	    case 1:  Strcpy(buf, "once");  break;
	    case 2:  Strcpy(buf, "twice");  break;
	    case 3:  Strcpy(buf, "thrice");  break;
	    default: Sprintf(buf, "%d times", u.umortality);
		     break;
	    }
	} else {		/* game ended in character's death */
	    p = "are dead";
	    switch (u.umortality) {
	    case 0:  impossible("dead without dying?");
	    case 1:  break;			/* just "are dead" */
	    default: Sprintf(buf, " (%d%s time!)", u.umortality,
			     ordin(u.umortality));
		     break;
	    }
	}
	if (p) enl_msg(You_, "have been killed ", p, buf);
    }

	display_nhwindow(en_win, TRUE);
	destroy_nhwindow(en_win);
	return;
#endif /*JP*/
}

STATIC_PTR int
wiz_attributes()
{
	if (wizard || discover)	enlightenment(0);
/*JP	else pline("Unavailable command '^X'.");*/
	else pline("'^X'コマンドは使えない．");
	return 0;
}

#endif /* OVLB */
#ifdef OVL1

#ifndef M
# ifndef NHSTDC
#  define M(c)		(0x80 | (c))
# else
#  define M(c)		((c) - 128)
# endif /* NHSTDC */
#endif
#ifndef C
#define C(c)		(0x1f & (c))
#endif

static const struct func_tab cmdlist[] = {
	{C('d'), FALSE, dokick}, /* "D" is for door!...?  Msg is in dokick.c */
#ifdef WIZARD
	{C('b'), FALSE, playersteal},
	{C('e'), TRUE, wiz_detect},
	{C('f'), TRUE, wiz_map},
	{C('g'), TRUE, wiz_genesis},
	{C('i'), TRUE, wiz_identify},
	{C('j'), TRUE, wiz_gain_level},
#endif
	{C('l'), TRUE, doredraw}, /* if number_pad is set */
#ifdef WIZARD
	{C('o'), TRUE, wiz_where},
#endif
	{C('p'), TRUE, doprev_message},
	{C('r'), TRUE, doredraw},
	{C('s'), FALSE, specialpower},
	{C('t'), TRUE, dotele},
#ifdef WIZARD
	{C('v'), TRUE, wiz_level_tele},
	{C('w'), TRUE, wiz_wish},
	{C('x'), TRUE, wiz_attributes},
#endif
	{C('y'), TRUE, polyatwill},
#ifdef SUSPEND
	{C('z'), TRUE, dosuspend},
#endif
	{'a', FALSE, doapply},
	{'A', FALSE, doddoremarm},
	{M('a'), TRUE, doorganize},
/*	'b', 'B' : go sw */
#ifdef BORG
	{'B', TRUE, doborgtoggle}, /* [Tom] */
#endif
	{M('b'), FALSE, playersteal},   /* jla */
	{M('b'), FALSE, specialpower},   /* jla */
	{'c', FALSE, doclose},
	{'C', TRUE, do_mname},
	{M('c'), TRUE, dotalk},
	{'d', FALSE, dodrop},
	{'D', FALSE, doddrop},
	{M('d'), FALSE, dodip},
	{'e', FALSE, doeat},
	{'E', FALSE, doengrave},
	{M('e'), FALSE, specialpower},        
/* Soon to be
	{'f', FALSE, dofight, "fighting"},
	{'F', FALSE, doFight, "fighting"},
 */
	{'f', FALSE, dofire},        
	{M('f'), FALSE, doforce},
/*	'g', 'G' : multiple go */
/*	'h', 'H' : go west */
	{'h', TRUE, dohelp}, /* if number_pad is set */
	{'i', TRUE, ddoinv},
	{'I', TRUE, dotypeinv},		/* Robert Viduya */
	{M('i'), TRUE, doinvoke},
/*	'j', 'J', 'k', 'K', 'l', 'L', 'm', 'M', 'n', 'N' : move commands */
	{'j', FALSE, dojump}, /* if number_pad is on */
	{M('j'), FALSE, dojump},
	{'k', FALSE, dokick}, /* if number_pad is on */
#ifdef WEAPON_SKILLS
	{M('k'), TRUE, enhance_weapon_skill},
#endif /* WEAPON_SKILLS */
	{'l', FALSE, doloot}, /* if number_pad is on */
	{M('l'), FALSE, doloot},
/*	'n' prefixes a count if number_pad is on */
	{M('m'), TRUE, domonability},
	{'N', TRUE, ddocall}, /* if number_pad is on */
	{M('n'), TRUE, ddocall},
	{'o', FALSE, doopen},
	{'O', TRUE, doset},
	{M('o'), FALSE, dosacrifice},
	{'p', FALSE, dopay},
	{'P', FALSE, doputon},
	{M('p'), TRUE, dopray},
	{'q', FALSE, dodrink},
	{'Q', FALSE, dowieldquiver},        
	{M('q'), TRUE, done2},
	{'r', FALSE, doread},
	{'R', FALSE, doremring},
	{M('r'), FALSE, dorub},
/*JP	{'s', TRUE, dosearch, "searching"},*/
	{'s', TRUE, dosearch, "捜す"},
	{'S', TRUE, dosave},
	{M('s'), FALSE, dosit},
	{'t', FALSE, dothrow},
	{'T', FALSE, dotakeoff},
	{M('t'), TRUE, doturn},
/*	'u', 'U' : go ne */
	{'u', FALSE, dountrap}, /* if number_pad is on */
	{M('u'), FALSE, dountrap},
	{'v', TRUE, doversion},
	{'V', TRUE, dohistory},
	{M('v'), TRUE, doextversion},
	{'w', FALSE, dowield},
	{'W', FALSE, dowear},
	{M('w'), FALSE, dowipe},
	{'x', FALSE, doswapweapon},                    /* [Tom] */        
	{'X', TRUE, dovspell},                  /* Mike Stephenson */
        {M('x'), TRUE, enter_explore_mode},
/*	'y', 'Y' : go nw */
	{M('y'), FALSE, polyatwill},  /* jla */    
	{'z', FALSE, dozap},
	{'Z', TRUE, docast},
	{'<', FALSE, doup},
	{'>', FALSE, dodown},
	{'/', TRUE, dowhatis},
	{'&', TRUE, dowhatdoes},
	{'?', TRUE, dohelp},
	{M('?'), TRUE, doextlist},
#ifdef SHELL
	{'!', TRUE, dosh},
#endif
/*JP	{'.', TRUE, donull, "waiting"},
	{' ', TRUE, donull, "waiting"},*/
	{'.', TRUE, donull, "休憩する"},
	{' ', TRUE, donull, "休憩する"},
	{',', FALSE, dopickup},
	{':', TRUE, dolook},
	{';', TRUE, doquickwhatis},
	{'^', TRUE, doidtrap},
	{'\\', TRUE, dodiscovered},		/* Robert Viduya */
	{'@', TRUE, dotogglepickup},
/*JP*/
	{'`', TRUE, dotogglelang},
	{WEAPON_SYM,  TRUE, doprwep},
	{ARMOR_SYM,  TRUE, doprarm},
	{RING_SYM,  TRUE, doprring},
	{AMULET_SYM, TRUE, dopramulet},
	{TOOL_SYM, TRUE, doprtool},
	{GOLD_SYM, TRUE, doprgold},
	{SPBOOK_SYM, TRUE, dovspell},			/* Mike Stephenson */
	{'#', TRUE, doextcmd},
	{0,0,0,0}
};

struct ext_func_tab extcmdlist[] = {
#if 0 /*JP*/
	{"adjust", "adjust inventory letters", doorganize, TRUE},
	{"bsteal", "steal from monsters", playersteal, FALSE},  /* jla */        
	{"chat", "talk to someone", dotalk, TRUE},	/* converse? */
	{"dip", "dip an object into something", dodip, FALSE},
	{"effect", "use a special class effect", specialpower, FALSE},
#ifdef WEAPON_SKILLS
	{"enhance", "advance or check weapons skills", enhance_weapon_skill,
							TRUE},
#endif /* WEAPON_SKILLS */
	{"force", "force a lock", doforce, FALSE},
	{"invoke", "invoke an object's powers", doinvoke, TRUE},
	{"jump", "jump to a location", dojump, FALSE},
	{"loot", "loot a box on the floor", doloot, FALSE},
	{"monster", "use a monster's special ability", domonability, TRUE},
	{"name", "name an item or type of object", ddocall, TRUE},
	{"offer", "offer a sacrifice to the gods", dosacrifice, FALSE},
	{"pray", "pray to the gods for help", dopray, TRUE},
	{"quit", "quit the game", done2},        
	{"rub", "rub a lamp", dorub, FALSE},
	{"sit", "sit down", dosit, FALSE},
	{"turn", "turn undead", doturn, TRUE},
	{"untrap", "untrap something", dountrap, FALSE},
	{"version", "list compile time options for this version of NetHack",
		doextversion, TRUE},
	{"wipe", "wipe off your face", dowipe, FALSE},
	{"youpoly", "polymorph at will", polyatwill, FALSE},  /* jla */        
	{"?", "get this list of extended commands", doextlist, TRUE},
#endif /*JP*/

#ifdef WEAPON_SKILLS
	{"2weapon", "片手持ちと二刀流の切換え", dotwoweapon, FALSE },
#endif
	{"adjust", "持ち物一覧の調整", doorganize, TRUE},
	{"bsteal", "モンスターから盗む", playersteal, FALSE},  /* jla */
	{"chat", "誰かと話す", dotalk, TRUE},	/* converse? */
	{"dip", "何かに物を浸す", dodip, FALSE},
	{"effect", "各職業の特別能力を使う", specialpower, FALSE},
#ifdef WEAPON_SKILLS
	{"enhance", "武器熟練度を高める", enhance_weapon_skill, TRUE},
#endif /* WEAPON_SKILLS */
#ifdef WIZARD
	{"explore", "ウィザードモード", enter_explore_mode, FALSE},
#endif
	{"force", "鍵をこじあける", doforce, FALSE},
	{"invoke", "物の特別な力を使う", doinvoke, TRUE},
	{"jump", "他の位置に飛びうつる", dojump, FALSE},
	{"loot", "床の上の箱を開ける", doloot, TRUE},
	{"monster", "怪物の特別能力を使う", domonability, TRUE},
	{"name", "アイテムや物に名前をつける", ddocall, TRUE},
	{"offer", "神に供物を捧げる", dosacrifice, FALSE},
	{"pray", "神に祈る", dopray, TRUE},
	{"quit", "ゲームを放棄する", done2},
	{"rub", "ランプをこする", dorub, FALSE},
	{"sit", "座る", dosit, FALSE},
	{"turn", "アンデットを土に返す", doturn, TRUE},
	{"untrap", "罠をはずす", dountrap, FALSE},
	{"version", "コンパイル時のオプションを表示する",
		doextversion, TRUE},
	{"wipe", "顔を拭う", dowipe, FALSE},
	{"youpoly", "変化する", polyatwill, FALSE},  /* jla */
	{"?", "この拡張コマンド一覧を表示する", doextlist, TRUE},
#if defined(WIZARD)
	/*
	 * There must be a blank entry here for every entry in the table
	 * below.
	 */
	{(char *)0, (char *)0, donull, TRUE},
	{(char *)0, (char *)0, donull, TRUE},
	{(char *)0, (char *)0, donull, TRUE},
	{(char *)0, (char *)0, donull, TRUE},
	{(char *)0, (char *)0, donull, TRUE},
#ifdef DEBUG
	{(char *)0, (char *)0, donull, TRUE},
#endif
	{(char *)0, (char *)0, donull, TRUE},
#endif
	{(char *)0, (char *)0, donull, TRUE}
};

#if defined(WIZARD)
static const struct ext_func_tab debug_extcmdlist[] = {
	{"light sources", "show mobile light sources", wiz_light_sources, TRUE},
	{"seenv", "show seen vectors", wiz_show_seenv, TRUE},
	{"stats", "show memory statistics", wiz_show_stats, TRUE},
	{"timeout", "look at timeout queue", wiz_timeout_queue, TRUE},
	{"vision", "show vision array", wiz_show_vision, TRUE},
#ifdef DEBUG
	{"wizdebug", "wizard debug command", wiz_debug_cmd, TRUE},
#endif
	{"wmode", "show wall modes", wiz_show_wmodes, TRUE},
	{(char *)0, (char *)0, donull, TRUE}
};

/*
 * Insert debug commands into the extended command list.  This function
 * assumes that the last entry will be the help entry.
 *
 * You must add entries in ext_func_tab every time you add one to the
 * debug_extcmdlist().
 */
void
add_debug_extended_commands()
{
	int i, j, k, n;

	/* count the # of help entries */
	for (n = 0; extcmdlist[n].ef_txt[0] != '?'; n++)
	    ;

	for (i = 0; debug_extcmdlist[i].ef_txt; i++) {
	    for (j = 0; j < n; j++)
		if (strcmp(debug_extcmdlist[i].ef_txt, extcmdlist[j].ef_txt) < 0) break;

	    /* insert i'th debug entry into extcmdlist[j], pushing down  */
	    for (k = n; k >= j; --k)
		extcmdlist[k+1] = extcmdlist[k];
	    extcmdlist[j] = debug_extcmdlist[i];
	    n++;	/* now an extra entry */
	}
}


static const char *template = "%-18s %4ld  %6ld";
static const char *count_str = "                   count  bytes";
static const char *separator = "------------------ -----  ------";

static void
count_obj(chain, total_count, total_size, top, recurse)
	struct obj *chain;
	long *total_count;
	long *total_size;
	boolean top;
	boolean recurse;
{
	long count, size;
	struct obj *obj;

	for (count = size = 0, obj = chain; obj; obj = obj->nobj) {
	    if (top) {
		count++;
		size += sizeof(struct obj) + obj->oxlth + obj->onamelth;
	    }
	    if (recurse && obj->cobj)
		count_obj(obj->cobj, total_count, total_size, TRUE, TRUE);
	}
	*total_count += count;
	*total_size += size;
}

static void
obj_chain(win, src, chain, total_count, total_size)
	winid win;
	const char *src;
	struct obj *chain;
	long *total_count;
	long *total_size;
{
	char buf[BUFSZ];
	long count = 0, size = 0;

	count_obj(chain, &count, &size, TRUE, FALSE);
	*total_count += count;
	*total_size += size;
	Sprintf(buf, template, src, count, size);
	putstr(win, 0, buf);
}

static void
mon_invent_chain(win, src, chain, total_count, total_size)
	winid win;
	const char *src;
	struct monst *chain;
	long *total_count;
	long *total_size;
{
	char buf[BUFSZ];
	long count = 0, size = 0;
	struct monst *mon;

	for (mon = chain; mon; mon = mon->nmon)
	    count_obj(mon->minvent, &count, &size, TRUE, FALSE);
	*total_count += count;
	*total_size += size;
	Sprintf(buf, template, src, count, size);
	putstr(win, 0, buf);
}

static void
contained(win, src, total_count, total_size)
	winid win;
	const char *src;
	long *total_count;
	long *total_size;
{
	char buf[BUFSZ];
	long count = 0, size = 0;
	struct monst *mon;

	count_obj(invent, &count, &size, FALSE, TRUE);
	count_obj(fobj, &count, &size, FALSE, TRUE);
	count_obj(level.buriedobjlist, &count, &size, FALSE, TRUE);
	count_obj(migrating_objs, &count, &size, FALSE, TRUE);
	for (mon = fmon; mon; mon = mon->nmon)
	    count_obj(mon->minvent, &count, &size, FALSE, TRUE);
	for (mon = migrating_mons; mon; mon = mon->nmon)
	    count_obj(mon->minvent, &count, &size, FALSE, TRUE);

	*total_count += count; *total_size += size;

	Sprintf(buf, template, src, count, size);
	putstr(win, 0, buf);
}

static void
mon_chain(win, src, chain, total_count, total_size)
	winid win;
	const char *src;
	struct monst *chain;
	long *total_count;
	long *total_size;
{
	char buf[BUFSZ];
	long count, size;
	struct monst *mon;

	for (count = size = 0, mon = chain; mon; mon = mon->nmon) {
	    count++;
	    size += sizeof(struct monst) + mon->mxlth + mon->mnamelth;
	}
	*total_count += count;
	*total_size += size;
	Sprintf(buf, template, src, count, size);
	putstr(win, 0, buf);
}

/*
 * Display memory usage of all monsters and objects on the level.
 */
static int
wiz_show_stats()
{
	char buf[BUFSZ];
	winid win;
	long total_obj_size = 0, total_obj_count = 0;
	long total_mon_size = 0, total_mon_count = 0;

	win = create_nhwindow(NHW_TEXT);
	putstr(win, 0, "Current memory statistics:");
	putstr(win, 0, "");
	Sprintf(buf, "Objects, size %d", (int) sizeof(struct obj));
	putstr(win, 0, buf);
	putstr(win, 0, "");
	putstr(win, 0, count_str);

	obj_chain(win, "invent", invent, &total_obj_count, &total_obj_size);
	obj_chain(win, "fobj", fobj, &total_obj_count, &total_obj_size);
	obj_chain(win, "buried", level.buriedobjlist,
				&total_obj_count, &total_obj_size);
	obj_chain(win, "migrating obj", migrating_objs,
				&total_obj_count, &total_obj_size);
	mon_invent_chain(win, "minvent", fmon,
				&total_obj_count,&total_obj_size);
	mon_invent_chain(win, "migrating minvent", migrating_mons,
				&total_obj_count, &total_obj_size);

	contained(win, "contained",
				&total_obj_count, &total_obj_size);

	putstr(win, 0, separator);
	Sprintf(buf, template, "Total", total_obj_count, total_obj_size);
	putstr(win, 0, buf);

	putstr(win, 0, "");
	putstr(win, 0, "");
	Sprintf(buf, "Monsters, size %d", (int) sizeof(struct monst));
	putstr(win, 0, buf);
	putstr(win, 0, "");

	mon_chain(win, "fmon", fmon,
				&total_mon_count, &total_mon_size);
	mon_chain(win, "migrating", migrating_mons,
				&total_mon_count, &total_mon_size);

	putstr(win, 0, separator);
	Sprintf(buf, template, "Total", total_mon_count, total_mon_size);
	putstr(win, 0, buf);

#ifdef __BORLANDC__
	show_borlandc_stats(win);
#endif

	display_nhwindow(win, FALSE);
	destroy_nhwindow(win);
	return 0;
}

void
sanity_check()
{
	obj_sanity_check();
	timer_sanity_check();
}

#endif /* WIZARD */

#define unctrl(c)	((c) <= C('z') ? (0x60 | (c)) : (c))
#define unmeta(c)	(0x7f & (c))


void
rhack(cmd)
register char *cmd;
{
	boolean do_walk, do_rush, prefix_seen, bad_command,
		firsttime = (cmd == 0);

	if (firsttime) {
		flags.nopick = 0;
		cmd = parse();
	}
	if (*cmd == '\033') {
		flags.move = FALSE;
		return;
	}
#ifdef REDO
	if (*cmd == DOAGAIN && !in_doagain && saveq[0]) {
		in_doagain = TRUE;
		stail = 0;
		rhack((char *)0);	/* read and execute command */
		in_doagain = FALSE;
		return;
	}
	/* Special case of *cmd == ' ' handled better below */
	if(!*cmd || *cmd == (char)0377) {
#else
	if(!*cmd || *cmd == (char)0377 ||
	   (!flags.rest_on_space && *cmd == ' ')) {
#endif
		nhbell();
		flags.move = FALSE;
		return;		/* probably we just had an interrupt */
	}

	/* handle most movement commands */
	do_walk = do_rush = prefix_seen = FALSE;
	switch (*cmd) {
	 case 'g':  if (movecmd(cmd[1])) {
			flags.run = 2;
			do_rush = TRUE;
		    } else
			prefix_seen = TRUE;
		    break;
	 case '5':  if (!iflags.num_pad) break;	/* else FALLTHRU */
	 case 'G':  if (movecmd(lowc(cmd[1]))) {
			flags.run = 3;
			do_rush = TRUE;
		    } else
			prefix_seen = TRUE;
		    break;
	 case '-':  if (!iflags.num_pad) break;	/* else FALLTHRU */
	 case 'm':  if (movecmd(cmd[1]) || u.dz) {
			flags.run = 0;
			flags.nopick = 1;
			if (!u.dz) do_walk = TRUE;
			else cmd[0] = cmd[1];	/* "m<" or "m>" */
		    } else
			prefix_seen = TRUE;
		    break;
	 case 'M':  if (movecmd(lowc(cmd[1]))) {
			flags.run = 1;
			flags.nopick = 1;
			do_rush = TRUE;
		    } else
			prefix_seen = TRUE;
		    break;
	 case '0':  if (!iflags.num_pad) break;
		    (void)ddoinv(); /* a convenience borrowed from the PC */
		    flags.move = FALSE;
		    multi = 0;
		    return;
	 default:   if (movecmd(*cmd)) {	/* ordinary movement */
			do_walk = TRUE;
		    } else if (movecmd(iflags.num_pad ?
				       unmeta(*cmd) : lowc(*cmd))) {
			flags.run = 1;
			do_rush = TRUE;
		    } else if (movecmd(unctrl(*cmd))) {
			flags.run = 3;
			do_rush = TRUE;
		    }
		    break;
	}
	if (do_walk) {
	    if (multi) flags.mv = TRUE;
	    domove();
	    return;
	} else if (do_rush) {
	    if (firsttime) {
		if (!multi) multi = max(COLNO,ROWNO);
		u.last_str_turn = 0;
	    }
	    flags.mv = TRUE;
	    domove();
	    return;
	} else if (prefix_seen && cmd[1] == '\033') {	/* <prefix><escape> */
	    /* don't report "unknown command" for change of heart... */
	    bad_command = FALSE;
	} else if (*cmd == ' ' && !flags.rest_on_space) {
	    bad_command = TRUE;		/* skip cmdlist[] loop */
	/* handle all other commands */
	} else {
	    register const struct func_tab *tlist;
	    int res, NDECL((*func));
	    for (tlist = cmdlist; tlist->f_char; tlist++) {
		if ((*cmd & 0xff) != (tlist->f_char & 0xff)) continue;

		if (u.uburied && !tlist->can_if_buried) {
/*JP		    You_cant("do that while you are buried!");*/
		    You("埋まっている時にそんなことはできない！");
		    res = 0;
		} else {
		    /* we discard 'const' because some compilers seem to have
		       trouble with the pointer passed to set_occupation() */
		    func = ((struct func_tab *)tlist)->f_funct;
		    if (tlist->f_text && !occupation && multi)
			set_occupation(func, tlist->f_text, multi);
		    res = (*func)();		/* perform the command */
		}
		if (!res) {
		    flags.move = FALSE;
		    multi = 0;
		}
		return;
	    }
	    /* if we reach here, cmd wasn't found in cmdlist[] */
	    bad_command = TRUE;
	}
	if (bad_command) {
	    char expcmd[10];
	    register char *cp = expcmd;

	    while (*cmd && (int)(cp - expcmd) < (int)(sizeof expcmd - 3)) {
		if (*cmd >= 040 && *cmd < 0177) {
		    *cp++ = *cmd++;
		} else if (*cmd & 0200) {
		    *cp++ = 'M';
		    *cp++ = '-';
		    *cp++ = *cmd++ &= ~0200;
		} else {
		    *cp++ = '^';
		    *cp++ = *cmd++ ^ 0100;
		}
	    }
	    *cp = '\0';
/*JP	    Norep("Unknown command '%s'.", expcmd);*/
	    Norep("'%s'コマンド？", expcmd);
	}
	/* didn't move */
	flags.move = FALSE;
	multi = 0;
	return;
}

int
xytod(x, y)	/* convert an x,y pair into a direction code */
schar x, y;
{
	register int dd;

	for(dd = 0; dd < 8; dd++)
	    if(x == xdir[dd] && y == ydir[dd]) return dd;

	return -1;
}

void
dtoxy(cc,dd)	/* convert a direction code into an x,y pair */
coord *cc;
register int dd;
{
	cc->x = xdir[dd];
	cc->y = ydir[dd];
	return;
}

int
movecmd(sym)	/* also sets u.dz, but returns false for <> */
char sym;
{
	register const char *dp;
	register const char *sdp;
	if(iflags.num_pad) sdp = ndir; else sdp = sdir;	/* DICE workaround */

	u.dz = 0;
	if(!(dp = index(sdp, sym))) return 0;
	u.dx = xdir[dp-sdp];
	u.dy = ydir[dp-sdp];
	u.dz = zdir[dp-sdp];
	if (u.dx && u.dy && u.umonnum == PM_GRID_BUG) {
		u.dx = u.dy = 0;
		return 0;
	}
	return !u.dz;
}

int
getdir(s)
const char *s;
{
	char dirsym;

#ifdef REDO	
	if(in_doagain)
	    dirsym = readchar();
	else
#endif
/*JP	    dirsym = yn_function (s ? s : "In what direction?",*/
	    dirsym = yn_function (s ? s : "どの方向？",
					(char *)0, '\0');
#ifdef REDO
	savech(dirsym);
#endif
	if(dirsym == '.' || dirsym == 's')
		u.dx = u.dy = u.dz = 0;
	else if(!movecmd(dirsym) && !u.dz) {
		if(!index(quitchars, dirsym))
/*JP			pline("What a strange direction!");*/
			pline("ずいぶんと奇妙な方向だ！");
		return 0;
	}
	if(!u.dz && (Stunned || (Confusion && !rn2(5)))) confdir();
	return 1;
}

#endif /* OVL1 */
#ifdef OVLB

void
confdir()
{
	register int x = (u.umonnum == PM_GRID_BUG) ? 2*rn2(4) : rn2(8);
	u.dx = xdir[x];
	u.dy = ydir[x];
	return;
}

#endif /* OVLB */
#ifdef OVL0

int
isok(x,y)
register int x, y;
{
	/* x corresponds to curx, so x==1 is the first column. Ach. %% */
	return x >= 1 && x <= COLNO-1 && y >= 0 && y <= ROWNO-1;
}

static NEARDATA int last_multi;

/*
 * convert a MAP window position into a movecmd
 */
int
click_to_cmd(x, y, mod)
    int x, y, mod;
{
    x -= u.ux;
    y -= u.uy;
    /* convert without using floating point, allowing sloppy clicking */
    if(x > 2*abs(y))
	x = 1, y = 0;
    else if(y > 2*abs(x))
	x = 0, y = 1;
    else if(x < -2*abs(y))
	x = -1, y = 0;
    else if(y < -2*abs(x))
	x = 0, y = -1;
    else
	x = sgn(x), y = sgn(y);

    if(x == 0 && y == 0)	/* map click on player to "rest" command */
	return '.';

    x = xytod(x, y);
    if(mod == CLICK_1) {
	return (iflags.num_pad ? ndir[x] : sdir[x]);
    } else {
	return (iflags.num_pad ? M(ndir[x]) :
		(sdir[x] - 'a' + 'A')); /* run command */
    }
}

STATIC_OVL char *
parse()
{
#ifdef LINT	/* static char in_line[COLNO]; */
	char in_line[COLNO];
#else
	static char in_line[COLNO];
#endif
	register int foo;
	char junk_char;
	static char repeat_char;
	boolean prezero = FALSE;

	multi = 0;
	flags.move = 1;
	flush_screen(1); /* Flush screen buffer. Put the cursor on the hero. */

#ifdef BORG
	if (borg_on) {
	   if (!kbhit()) {
	       borg_input();
	       return(borg_line);
	   } else {
		 junk_char = readchar();
		 pline("Cyborg terminated.");
		 borg_on = 0;
	   }

	/* [Tom] for those who occasionally go insane... */
	} else if (repeat_hit) {
#else
       if (repeat_hit) {
#endif
		repeat_hit--;
		in_line[0] = repeat_char;
		in_line[1] = 0;
		return (in_line);
	}

	if (!iflags.num_pad || (foo = readchar()) == 'n')
	    for (;;) {
		foo = readchar();
		if (foo >= '0' && foo <= '9') {
		    multi = 10 * multi + foo - '0';
		    if (multi < 0 || multi >= LARGEST_INT) multi = LARGEST_INT;
		    if (multi > 9) {
			clear_nhwindow(WIN_MESSAGE);
/*JP			Sprintf(in_line, "Count: %d", multi);*/
			Sprintf(in_line, "数: %d", multi);
			pline(in_line);
			mark_synch();
		    }
		    last_multi = multi;
		    if (!multi && foo == '0') prezero = TRUE;
		} else break;	/* not a digit */
	    }

	if (foo == '\033') {   /* esc cancels count (TH) */
	    clear_nhwindow(WIN_MESSAGE);
	    multi = last_multi = 0;
# ifdef REDO
	} else if (foo == DOAGAIN || in_doagain) {
	    multi = last_multi;
	} else {
	    last_multi = multi;
	    savech(0);	/* reset input queue */
	    savech((char)foo);
# endif
	}

	if (multi) {
	    multi--;
	    save_cm = in_line;
	} else {
	    save_cm = (char *)0;
	}
	in_line[0] = foo;
	in_line[1] = '\0';
	if (foo == 'g' || foo == 'G' || (iflags.num_pad && foo == '5') ||
	    foo == 'm' || foo == 'M') {
	    foo = readchar();
#ifdef REDO
	    savech((char)foo);
#endif
	    in_line[1] = foo;
	    in_line[2] = 0;
	}
	clear_nhwindow(WIN_MESSAGE);
	if (prezero) in_line[0] = '\033';
	repeat_char = in_line[0];
	return(in_line);
}

#endif /* OVL0 */
#ifdef OVLB

#ifdef UNIX
static
void
end_of_input()
{
	exit_nhwindows("End of input?");
#ifndef NOSAVEONHANGUP
	if (!program_state.done_hup++)
	    (void) dosave0();
#endif
	clearlocks();
	terminate(EXIT_SUCCESS);
}
#endif

#endif /* OVLB */
#ifdef OVL0

char
readchar()
{
	register int sym;
	int x, y, mod;

#ifdef REDO
	sym = in_doagain ? Getchar() : nh_poskey(&x, &y, &mod);
#else
	sym = Getchar();
#endif

#ifdef UNIX
# ifdef NR_OF_EOFS
	if (sym == EOF) {
	    register int cnt = NR_OF_EOFS;
	  /*
	   * Some SYSV systems seem to return EOFs for various reasons
	   * (?like when one hits break or for interrupted systemcalls?),
	   * and we must see several before we quit.
	   */
	    do {
		clearerr(stdin);	/* omit if clearerr is undefined */
		sym = Getchar();
	    } while (--cnt && sym == EOF);
	}
# endif /* NR_OF_EOFS */
	if (sym == EOF)
	    end_of_input();
#endif /* UNIX */

	if(sym == 0) /* click event */
	    sym = click_to_cmd(x, y, mod);
	return((char) sym);
}
#endif /* OVL0 */

/*cmd.c*/
