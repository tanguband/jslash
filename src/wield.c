/*	SCCS Id: @(#)wield.c	3.2	96/07/04	*/
/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 1985. */
/* NetHack may be freely redistributed.  See license for details. */

/*
**	Japanese version Copyright
**	(c) Issei Numata, Naoki Hamada, Shigehiro Miyashita, 1994-1996
**	changing point is marked `JP' (94/6/7)
**	JNetHack may be freely redistributed.  See license for details. 
*/

#include "hack.h"

static int FDECL(ready_weapon, (struct obj *));

/* elven weapons vibrate warningly when enchanted beyond a limit */
#define is_elven_weapon(optr)	((optr)->otyp == ELVEN_ARROW\
				|| (optr)->otyp == ELVEN_SPEAR\
				|| (optr)->otyp == ELVEN_DAGGER\
				|| (optr)->otyp == ELVEN_SHORT_SWORD\
				|| (optr)->otyp == ELVEN_BROADSWORD\
				|| (optr)->otyp == ELVEN_BOW)

/* used by erode_weapon() and will_weld() */
#define erodeable_wep(optr)	((optr)->oclass == WEAPON_CLASS \
				|| is_weptool(optr) \
				|| (optr)->otyp == HEAVY_IRON_BALL \
				|| (optr)->otyp == IRON_CHAIN)

/* used by welded(), and also while wielding */
#define will_weld(optr)		((optr)->cursed \
				&& (erodeable_wep(optr) \
				   || (optr)->otyp == TIN_OPENER))

/* Note: setuwep() with a null obj, and uwepgone(), are NOT the same!
 * Sometimes unwielding a weapon can kill you, and lifesaving will then
 * put it back into your hand.  If lifesaving is permitted to do this,
 * use setwuep((struct obj *)0); otherwise use uwepgone().
 */
void
setuwep(obj)
register struct obj *obj;
{
	if (obj == uwep) return; /* necessary to not set unweapon */
	setworn(obj, W_WEP);
	/* Note: Explicitly wielding a pick-axe will not give a "bashing"
	 * message.  Wielding one via 'a'pplying it will.
	 * 3.2.2:  Wielding arbitrary objects will give bashing message too.
	 */
	if (obj) {
		int wepcat = objects[obj->otyp].oc_wepcat;
		unweapon = (obj->oclass == WEAPON_CLASS) ?
				(wepcat == WEP_BOW ||
				 wepcat == WEP_AMMO ||
				 wepcat == WEP_MISSILE) :
			   !is_weptool(obj);
	} else
		unweapon = TRUE;	/* for "bare hands" message */
	update_inventory();
}

/* [Tom]'s quiver */
void
setuqwep(obj)
register struct obj *obj;
{
	setworn(obj, W_QUIVER);
}

void
setuswapwep(obj)
register struct obj *obj;
{
	setworn(obj, W_SWAPWEP);
}

static void
setutempwep(obj)
register struct obj *obj;
{
	setworn(obj, W_TEMPWEP);
}

void
uwepgone()
{
	if (uwep) {
		setworn((struct obj *)0, W_WEP);
		unweapon = TRUE;
		update_inventory();
	}
}

void
uswapwepgone()
{
	if (uswapwep) {
		setworn((struct obj *)0, W_SWAPWEP);
		update_inventory();
	}
}

void
uqwepgone()
{
	if (uquiver) {
		setworn((struct obj *)0, W_QUIVER);
		update_inventory();
	}
}

static NEARDATA const char wield_objs[] =
	{ ALL_CLASSES, ALLOW_NONE, WEAPON_CLASS, TOOL_CLASS, 0 };



int doswapweapon()
{
     static struct obj *swapped;

     if (!uwep && !uswapwep) return 0;

     if (uwep && !uswapwep) {
	if (welded(uwep)) {
           weldmsg(uwep);
	   return 0;
	}
	swapped = uwep;
	setuwep((struct obj *) 0);
	swapped->owornmask |= W_SWAPWEP;
	swapped->owornmask = W_SWAPWEP;
	setuswapwep(swapped);
/*JP	You("are empty %s.", body_part(HANDED));*/
	You("%sを空けた．", body_part(HAND));
	return 0;
     } else if (uwep && uswapwep) {
/* 修正 by Ooi Takefumi */
	long dummy;
	if (uarms && bimanual(uswapwep)){
	  pline("盾を装備しているので両手持ちの%sに持ち替えることができない．",
		is_sword(uswapwep) ? "剣" :
		uswapwep->otyp == BATTLE_AXE ? "斧" : "武器");
	  return 0;
	}
	dummy = uwep->owornmask;
/* ここ迄 */
/*	long dummy = uwep->owornmask;*/
	if (welded(uwep)) {
           weldmsg(uwep);
	   return 0;
	}
	swapped = uwep;
	setuwep((struct obj *) 0);
	swapped->owornmask |= W_TEMPWEP;
	swapped->owornmask = W_TEMPWEP;
	setutempwep(swapped);
	/* now wield secondary weapon */
	dummy = uswapwep->owornmask;
	swapped = uswapwep;
	setuswapwep((struct obj *) 0);
	swapped->owornmask |= W_WEP;
	swapped->owornmask = W_WEP;
	ready_weapon(swapped);
	/* now put first weapon into secondary slot... */
	dummy = utempwep->owornmask;
	swapped = utempwep;
	setutempwep((struct obj *) 0);
	swapped->owornmask |= W_TEMPWEP;
	swapped->owornmask = W_TEMPWEP;
	setuswapwep(swapped);
#ifdef WEAPON_SKILLS
	prinv((char *)0, swapped, 0L);
#endif
     } else {
/* 修正 by Ooi Takefumi */
       if (uarms && bimanual(uswapwep)){
	 pline("盾を装備しているので両手持ちの%sに持ち替えることができない．",
	       is_sword(uswapwep) ? "剣" :
	       uswapwep->otyp == BATTLE_AXE ? "斧" : "武器");
	 return 0;
       }
/* ここ迄 */
	swapped = uswapwep;
	setuswapwep((struct obj *) 0);
	swapped->owornmask |= W_WEP;
	swapped->owornmask = W_WEP;
	ready_weapon(swapped);
	return 0;
     }
     return 0;
}

int
dowield()
{
	register struct obj *wep;
/*
	register int res = 0;
 */
	int result;

	multi = 0;
	if (cantwield(uasmon)) {
/*JP	    pline("Don't be ridiculous!");*/
	    pline("ばかばかしい！");
	    return(0);
	}
/*JP	if (!(wep = getobj(wield_objs, "wield"))) /* nothing */;
/*JP 修正 by OoiTakefumi */
	if (!(wep = getobj(wield_objs, "装備する"))) return(0);
/*ここ迄*/

	if(!(wep == (struct obj *) 0)) {
	/* [Tom] separated function so swapping works easily */
#ifdef WEAPON_SKILLS
	if (u.twoweap && !can_twoweapon())
		untwoweapon();
#endif
	result = ready_weapon(wep);
	return (result);
	}
        return 0;
}
 
int
ready_weapon(struct obj *wep)
{
	register int res = 0;
 
	if (uwep == wep) {
/*JP	    You("are already wielding that!");*/
	    You("もうそれを%sにしている！", body_part(HAND));
	    if (is_weptool(wep)) unweapon = FALSE;	/* [see setuwep()] */
	} else if (uquiver == wep) {       
/*JP	    You("are already using that as ammunition!");*/
	    You("もうそれを攻撃手段として使っている！");
	} else if (uswapwep == wep) {
/*JP	    You("are using that as a secondary weapon!");*/
	    You("もうそれを予備の武器として使っている！");
	} else if (welded(uwep)) {
	    weldmsg(uwep);
	} else if (wep == &zeroobj) {
/*JP	    if (uwep == 0) You("are already empty %s.", body_part(HANDED));*/
	    if (uwep == 0) You("何も%sにしていない！", body_part(HAND));
	    else  {
/*JP		You("are empty %s.", body_part(HANDED));*/
	        You("%sを空けた．", body_part(HAND));
		setuwep((struct obj *) 0);
		res++;
	    }
	    /* Prevent wielding cockatrice when not wearing gloves --KAA */
	} else if (!uarmg && !resists_ston(&youmonst) &&
		      (wep->otyp == CORPSE &&                   
			  TURNSTONE(wep->corpsenm))) {
/*JP	    You("wield the corpse in your bare %s.",makeplural(body_part(HAND)));*/
	    You("死体を%sにした．",makeplural(body_part(HAND)));
/*JP	    instapetrify("petrifying corpse");*/
	    instapetrify("危険な死体に触れて");
	} else if (uarms && bimanual(wep))
/*JP	    You("cannot wield a two-handed %s while wearing a shield.",
		is_sword(wep) ? "sword" :
		    wep->otyp == BATTLE_AXE ? "axe" : "weapon");*/
	    pline("盾を装備しているときに両手持ちの%sを装備できない．",
		is_sword(wep) ? "剣" :
		    wep->otyp == BATTLE_AXE ? "斧" : "武器");
	else if (wep->owornmask & (W_ARMOR | W_RING | W_AMUL | W_TOOL))
/*JP	    You("cannot wield that!");*/
	    You("それを装備できない！");
	else if (!wep->oartifact || touch_artifact(wep, &youmonst)) {
	    res++;	/* takes a turn even though it doesn't get wielded */
	    if (wep->otyp == LIGHTSABER) {        
/*JP	    	You("feel the power of the Force flowing through you.");*/
	    	You("理力があなたの体に注ぎ込まれたように感じた．");
	    	if (u.ualign.type != A_LAWFUL) {
/*JP	       		pline("It overwhelms you!");*/
	       		pline("その力に圧倒された！");
/*JP	       		losehp(rnd(15), "the Force", KILLED_BY);*/
	       		losehp(rnd(15), "理力に耐えられず", KILLED_BY);
	       		exercise(A_WIS, FALSE);
	    		}
	    } else if (wep->otyp == DARKSABER) {
/*JP	    	You("feel the power of the Dark Side of the Force.");*/
	    	You("暗黒の力を感じた．");
	    	if (u.ualign.type != A_CHAOTIC) {
/*JP	       		pline("You shudder...");*/
	       		pline("あなたは身震いした．．．");
/*JP	       		losehp(rnd(15), "the Dark Side of the Force", KILLED_BY);*/
	       		losehp(rnd(15), "暗黒の力を浴びて", KILLED_BY);
	       		exercise(A_WIS, FALSE);
	    	}
	   }
	    if (will_weld(wep)) {
		const char *tmp = xname(wep), *thestr = "The ";
		if (strncmp(tmp, thestr, 4) && !strncmp(The(tmp),thestr,4))
		    tmp = thestr; else tmp = "";
/*JP		pline("%s%s %s to your %s!",tmp, aobjnam(wep, "weld"),
		      (wep->quan == 1L) ? "itself" : "themselves", /* a3 *
		      body_part(HAND));*/
	        pline("%sは勝手にあなたの%sに装備された．",
		      xname(wep), 
  		      body_part(HAND));
		wep->bknown = TRUE;
	    } else {
		/* The message must be printed before setuwep (since
		 * you might die and be revived from changing weapons),
		 * and the message must be before the death message and
		 * Lifesaved rewielding.  Yet we want the message to
		 * say "weapon in hand", thus this kludge.
		 */
		long dummy = wep->owornmask;
		wep->owornmask |= W_WEP;
		prinv((char *)0, wep, 0L);
		wep->owornmask = dummy;
	    }
	    setuwep(wep);
	    if (wep->unpaid) {
		struct monst *this_shkp;

		if ((this_shkp = shop_keeper(inside_shop(u.ux, u.uy))) !=
		    (struct monst *)0) {
/*JP		    pline("%s says \"You be careful with my %s!\"",*/
		    pline("%sは述べた「%sの扱いは気をつけてくれよ！」",
			  shkname(this_shkp),
			  xname(wep));
		}
	    }
	}
	if (rnd(20) > ACURR(A_DEX)) return(res);
	else                        return(0);
}

int
dowieldquiver()
{
	register struct obj *wep;
	register int res = 0;

	multi = 0;
	if (cantwield(uasmon)) {
/*JP		pline("Don't be ridiculous!");*/
		pline("ばかばかしい！");
		return(0);
	}

/*JP	if (!(wep = getobj(wield_objs, "wield"))) /* nothing */;
	if (!(wep = getobj(wield_objs, "装填する"))) /* nothing */;
	else if (uquiver == wep) {
/*JP		pline("That ammunition is already readied!");*/
		pline("それは既に装填されている！");
		return(0);
	}
	else if (wep->oclass == ARMOR_CLASS) {
/*JP		pline("You cannot put armor in the quiver!");*/
		pline("矢筒の中に防具はいれれない！");
	}
	else if (uswapwep == wep) {
/*JP		You("are using that as a secondary weapon!");*/
		You("それはもう武器として使っている！");
		return 0;
	}
	else if (uwep == wep) {
/*JP		pline("That ammunition is being used as a weapon!");*/
		pline("それはもう武器として使っている！");
		return(0);
	}
	else if (wep == &zeroobj) {
	    if (uwep == (struct obj *) 0)
/*JP		pline("You already have no ammunition readied!");*/
	        pline("あなたは今攻撃する武器を全く装備していません！");
	    else  {
/*JP		pline("You now have no ammunition readied.");*/
		pline("もう矢筒の中には何も無い．");
		setuqwep((struct obj *) 0);
		res++;
	    }
	}
	else {
			/* The message must be printed before setuwep (since
			 * you might die and be revived from changing weapons),
			 * and the message must be before the death message and
			 * Lifesaved rewielding.  Yet we want the message to
			 * say "weapon in hand", thus this kludge.
			 */
		long dummy = wep->owornmask;
		wep->owornmask |= W_QUIVER;
		prinv(NULL, wep, 0L);
		wep->owornmask = dummy;
		setuqwep(wep);
	}
	if (rnd(20) > ACURR(A_DEX)) return(res);
	else                        return(0);
}

#ifdef WEAPON_SKILLS

int can_twoweapon ()
{
	if (Upolyd)
		pline("変化している時は二刀流はできない．");
	else if (uwep && bimanual(uwep))
		pline("片手持ちの武器を装備していない．");
	else if (!uswapwep || (uswapwep && bimanual(uswapwep)))
		pline("片手持ちの武器を予備装備として持っていない．");
        /* WAC:  FIXME:  secondary weapon artifacts are problematic ... */
#ifndef DEBUG
	else if (uswapwep && uswapwep->oartifact)
			pline("その予備装備で二刀流はできない．");
#endif
	else if (uarms)
		pline("盾を装備しているので二刀流はできない．");
	else if (!uarmg &&
		  (uswapwep && uswapwep->otyp == CORPSE &&                   
                  (TURNSTONE(uswapwep->corpsenm)))) {
		char kbuf[BUFSZ];

		You("%sの死体を素%sで装備した．",
		    jtrns_mon(mons[uswapwep->corpsenm].mname,-1),
		    body_part(HAND));
		Sprintf(kbuf, "%sの死体", jtrns_mon(mons[uswapwep->corpsenm].mname,-1));            
		instapetrify(kbuf);
	} else if (Glib || (uswapwep && uswapwep->cursed)) {
		struct obj *obj = uswapwep;

		Your("%sはあなたの%sから滑り落ちた！",  xname(obj),
				makeplural(body_part(HAND)));
		if (!Glib)
			obj->bknown = TRUE;
		setuswapwep((struct obj *) 0);
		dropx(obj);
	} else
		return (TRUE);
	return (FALSE);
}

int
dotwoweapon()
{
	/* You can always toggle it off */
	if (u.twoweap) {
		You("片手持ちに切りかえた．");
		u.twoweap = 0;
		return (0);
	}

	/* May we use two weapons? */
	/* Touch the (possibly artifact) swapwep here */
	if (can_twoweapon() && uwep && uswapwep && 
	    (!uswapwep || !uswapwep->oartifact
	      || touch_artifact(uswapwep, &youmonst))) {
		/* Success! */
		You("二刀流を始めた．");

		u.twoweap = 1;

		prinv((char *)0, uwep, 0L);
		prinv((char *)0, uswapwep, 0L);
		return (rnd(20) > ACURR(A_DEX));
	}
	return (0);
}

void
untwoweapon()
{
	if (u.twoweap) {
		You("もう二つの武器を一度に使うことができない．");
		u.twoweap = FALSE;
/*		if (uwep) {
			long dummy = uwep->owornmask;
			uwep->owornmask |= W_WEP;
			prinv((char *)0, uwep, 0L);
			uwep->owornmask = dummy;
		} else
			You("are empty %s.", body_part(HANDED));*/
	}
	return;
}
#endif	/* WEAPON_SKILLS */

/* maybe rust weapon, or corrode it if acid damage is called for */
void
erode_weapon(acid_dmg)
boolean acid_dmg;
{
	if (!uwep || !erodeable_wep(uwep))
		return;

	if (uwep->greased) {
		grease_protect(uwep,(char *)0,FALSE);
	} else if (uwep->oerodeproof ||
		    (acid_dmg ? !is_corrodeable(uwep) : !is_rustprone(uwep))) {
		if (flags.verbose || !(uwep->oerodeproof && uwep->rknown))
/*JP		    Your("%s not affected.", aobjnam(uwep, "are"));*/
		    Your("%sは影響を受けない．", xname(uwep));
		if (uwep->oerodeproof) uwep->rknown = TRUE;
	} else if (uwep->oeroded < MAX_ERODE) {
/*JP		Your("%s%s!", aobjnam(uwep, acid_dmg ? "corrode" : "rust"),
		     uwep->oeroded+1 == MAX_ERODE ? " completely" :
		     uwep->oeroded ? " further" : "");*/
		Your("%sは%s%s！", xname(uwep),
		     uwep->oeroded+1 == MAX_ERODE ? "完全に" :
		     uwep->oeroded ? "さらに" : "",
		     acid_dmg ? "腐食した" : "錆びた");
		uwep->oeroded++;
	} else {
		if (flags.verbose)
/*JP		    Your("%s completely %s.",
			 aobjnam(uwep, Blind ? "feel" : "look"),
			 acid_dmg ? "corroded" : "rusty");*/
		    Your("%sは完全に%s%s．",
			 xname(uwep),
			 acid_dmg ? "腐食した" : "錆びた",
			 Blind ? "ようだ" : "ように見える");
	}
}

void
erode_swapweapon(acid_dmg)
boolean acid_dmg;
{
	if (!uswapwep || !erodeable_wep(uswapwep))
		return;

	if (uswapwep->greased) {
		grease_protect(uswapwep,(char *)0,FALSE);
	} else if (uswapwep->oerodeproof ||
		    (acid_dmg ? !is_corrodeable(uswapwep) : !is_rustprone(uswapwep))) {
		if (flags.verbose || !(uswapwep->oerodeproof && uswapwep->rknown))
/*JP		    Your("%s not affected.", aobjnam(uswapwep, "are"));*/
		    Your("%sは影響を受けない．", xname(uswapwep));
		if (uswapwep->oerodeproof) uswapwep->rknown = TRUE;
	} else if (uswapwep->oeroded < MAX_ERODE) {
/*JP		Your("%s%s!", aobjnam(uswapwep, acid_dmg ? "corrode" : "rust"),
		     uswapwep->oeroded+1 == MAX_ERODE ? " completely" :
		     uswapwep->oeroded ? " further" : "");*/
		Your("%sは%s%s！", xname(uswapwep),
		     uswapwep->oeroded+1 == MAX_ERODE ? "完全に" :
		     uswapwep->oeroded ? "さらに" : "",
		     acid_dmg ? "腐食した" : "錆びた");
		uswapwep->oeroded++;
	} else {
		if (flags.verbose)
/*JP		    Your("%s completely %s.",
			 aobjnam(uswapwep, Blind ? "feel" : "look"),
			 acid_dmg ? "corroded" : "rusty");*/
		    Your("%sは完全に%s%s．",
			 xname(uswapwep),
			 acid_dmg ? "腐食した" : "錆びた",
			 Blind ? "ようだ" : "ように見える");
	}
}

int
chwepon(otmp, amount)
register struct obj *otmp;
register int amount;
{
	register const char *color = hcolor((amount < 0) ? Black : blue);
	register const char *xtime;

	if(!uwep || (uwep->oclass != WEAPON_CLASS && !is_weptool(uwep))) {
		char buf[BUFSZ];

/*JP		Sprintf(buf, "Your %s %s.", makeplural(body_part(HAND)),
			(amount >= 0) ? "twitch" : "itch");*/
		Sprintf(buf, "あなたの%sは%s．", makeplural(body_part(HAND)),
			(amount >= 0) ? "ひきつった" : "ムズムズした");
		strange_feeling(otmp, buf);
		exercise(A_DEX, (boolean) (amount >= 0));
		return(0);
	}

	if(uwep->otyp == WORM_TOOTH && amount >= 0) {
		uwep->otyp = CRYSKNIFE;
/*JP		Your("weapon seems sharper now.");*/
		Your("武器はより鋭さを増したようだ．");
		uwep->cursed = 0;
		return(1);
	}

	if(uwep->otyp == CRYSKNIFE && amount < 0) {
		uwep->otyp = WORM_TOOTH;
/*JP		Your("weapon seems duller now.");*/
		Your("武器はより鈍くなったようだ．");
		return(1);
	}

	if (amount < 0 && uwep->oartifact && restrict_name(uwep, ONAME(uwep))) {
	    if (!Blind)
/*JP		Your("%s %s.", aobjnam(uwep, "faintly glow"), color);*/
		Your("%sはわずかに%s輝いた．", xname(uwep),jconj_adj(color));
	    return(1);
	}
	/* there is a (soft) upper and lower limit to uwep->spe */
	if(((uwep->spe > 5 && amount >= 0) || (uwep->spe < -5 && amount < 0))
								&& rn2(3)) {
	    if (!Blind)
/*JP	    Your("%s %s for a while and then evaporate%s.",
		 aobjnam(uwep, "violently glow"), color,
		 uwep->quan == 1L ? "s" : "");*/
	    Your("%sはしばらく激しく%s輝き，蒸発した．",
		 xname(uwep), jconj_adj(color));
	    else
/*JP		Your("%s.", aobjnam(uwep, "evaporate"));*/
		Your("%sは蒸発した", xname(uwep));

	    while(uwep)		/* let all of them disappear */
				/* note: uwep->quan = 1 is nogood if unpaid */
		useup(uwep);
	    return(1);
	}
	if (!Blind) {
/*JP	    xtime = (amount*amount == 1) ? "moment" : "while";*/
	    xtime = (amount*amount == 1) ? "一瞬" : "しばらくの間";
/*JP	    Your("%s %s for a %s.",
		 aobjnam(uwep, amount == 0 ? "violently glow" : "glow"),
		 color, xtime);*/
	    Your("%sは%s%s%s輝いた．",
		 xname(uwep), xtime, jconj_adj(color), 
		 amount == 0 ? "激しく" : "");
	}
	uwep->spe += amount;
	if(amount > 0) uwep->cursed = 0;

	/*
	 * Enchantment, which normally improves a weapon, has an
	 * addition adverse reaction on Magicbane whose effects are
	 * spe dependent.  Give an obscure clue here.
	 */
	if (uwep->oartifact == ART_MAGICBANE && uwep->spe >= 0) {
/*JP		Your("right %s %sches!",
			body_part(HAND),
			(((amount > 1) && (uwep->spe > 1)) ? "flin" : "it"));*/
		Your("右%sは%s！",
  			body_part(HAND),
			(((amount > 1) && (uwep->spe > 1)) ?
					"ひりひりした" : "ムズムズした"));
	}

	/* an elven magic clue, cookie@keebler */
	if ((uwep->spe > 3)
		&& (is_elven_weapon(uwep) || uwep->oartifact || !rn2(7)))
/*JP	    Your("%s unexpectedly.",
		aobjnam(uwep, "suddenly vibrate"));*/
	    Your("%sは突然震えだした．",
		xname(uwep));

	return(1);
}

int
welded(obj)
register struct obj *obj;
{
	if (obj && obj == uwep && will_weld(obj)) {
		obj->bknown = TRUE;
		return 1;
	}
	return 0;
}

void
weldmsg(obj)
register struct obj *obj;
{
	long savewornmask;

	savewornmask = obj->owornmask;
/*JP	Your("%s %s welded to your %s!",
		xname(obj), (obj->quan == 1L) ? "is" : "are",
		bimanual(obj) ? (const char *)makeplural(body_part(HAND))
				: body_part(HAND));*/
	You("%sを%sに構えた！", xname(obj), body_part(HAND));
	obj->owornmask = savewornmask;
}

/*wield.c*/
