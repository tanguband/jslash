/*	SCCS Id: @(#)objects.c	3.2	96/06/16	*/
/* Copyright (c) Mike Threepoint, 1989.				  */
/* NetHack may be freely redistributed.  See license for details. */

#ifndef OBJECTS_PASS_2_
/* first pass */
struct monst { struct monst *dummy; };	/* lint: struct obj's union */
#include "config.h"
#include "obj.h"
#include "objclass.h"
#include "prop.h"

#else	/* !OBJECTS_PASS_2_ */
/* second pass */
# ifdef TEXTCOLOR
#include "color.h"
#  define COLOR_FIELD(X) X,
# else
#  define COLOR_FIELD(X) /*empty*/
# endif
#endif	/* !OBJECTS_PASS_2_ */

/* objects have symbols: ) [ = " ( % ! ? + / $ * ` 0 _ . */

/*
 *	Note:  OBJ() and BITS() macros are used to avoid exceeding argument
 *	limits imposed by some compilers.  The ctnr field of BITS currently
 *	does not map into struct objclass, and is ignored in the expansion.
 *	The 0 in the expansion corresponds to oc_pre_discovered, which is
 *	set at run-time during role-specific character initialization.
 */

#ifndef OBJECTS_PASS_2_
/* first pass -- object descriptive text */
# define OBJ(name,desc) name,desc
# define OBJECT(obj,bits,prp,sym,prob,dly,wt,cost,sdam,ldam,oc1,oc2,nut,color) \
	{obj}

NEARDATA struct objdescr obj_descr[] = {
#else
/* second pass -- object definitions */

# define BITS(nmkn,mrg,uskn,ctnr,mgc,chrg,uniq,nwsh,big,tuf,dir,sub,mtrl) \
	nmkn,mrg,uskn,0,mgc,chrg,uniq,nwsh,big,tuf,dir,sub,mtrl /* SCO ODT 1.1 cpp fodder */
# ifndef lint
#  define HARDGEM(n) (n >= 8)
# else
#  define HARDGEM(n) (0)
# endif
#ifdef TEXTCOLOR
# define OBJECT(obj,bits,prp,sym,prob,dly,wt,cost,sdam,ldam,oc1,oc2,nut,color) \
	{0, 0, (char *)0, bits, prp, sym, dly, COLOR_FIELD(color) \
	 prob, wt, cost, sdam, ldam, oc1, oc2, nut}
#else
# define OBJECT(obj,bits,prp,sym,prob,dly,wt,cost,sdam,ldam,oc1,oc2,nut,color) \
	{0, 0, (char *)0, bits, prp, sym, dly, \
	 prob, wt, cost, sdam, ldam, oc1, oc2, nut}
#endif
NEARDATA struct objclass objects[] = {
#endif
/* dummy object[0] -- description [2nd arg] *must* be NULL */
	OBJECT(OBJ("strange object",(char *)0), BITS(1,0,0,0,0,0,0,0,0,0,0,0,0), 0,
			ILLOBJ_CLASS, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),

/* weapons ... */
#define WEAPON(name,app,kn,mg,bi,prob,wt,cost,sdam,ldam,hitbon,typ,sub,metal,color) \
	OBJECT( \
		OBJ(name,app), BITS(kn,mg,1,0,0,1,0,0,bi,0,typ,sub,metal), 0, \
		WEAPON_CLASS, prob, 0, \
		wt, cost, sdam, ldam, hitbon, 0, wt, color )
#define PROJECTILE(name,app,kn,prob,wt,cost,sdam,ldam,hitbon,metal,prop,color) \
	OBJECT( \
		OBJ(name,app), \
		BITS(kn,1,1,0,0,1,0,0,0,0,PIERCE,WEP_AMMO,metal), 0, \
		WEAPON_CLASS, prob, 0, \
		wt, cost, sdam, ldam, hitbon, prop, wt, color )
#define BOW(name,app,kn,prob,wt,cost,sdam,ldam,hitbon,metal,prop,color) \
	OBJECT( \
		OBJ(name,app), BITS(kn,0,1,0,0,1,0,0,0,0,0,WEP_BOW,metal), 0, \
		WEAPON_CLASS, prob, 0, \
		wt, cost, sdam, ldam, hitbon, -(prop), wt, color )

/* Note: for weapons that don't do an even die of damage (ex. 2-7 or 3-18)
 * the extra damage is added on in weapon.c, not here! */

#define P PIERCE
#define S SLASH
#define B WHACK

/* missiles */
PROJECTILE("arrow", (char *)0,
		1, 40, 1, 2, 6, 6, 0, IRON, WP_BOW, HI_METAL),
PROJECTILE("elven arrow", "runed arrow",
		0, 20, 1, 2, 7, 6, 0, IRON, WP_BOW, HI_METAL),
PROJECTILE("dark elven arrow", "black runed arrow",
		0,  0, 1, 2, 7, 6, 0, IRON, WP_BOW, CLR_BLACK),
PROJECTILE("orcish arrow", "crude arrow",
		0, 13, 1, 2, 5, 6, 0, IRON, WP_BOW, CLR_BLACK),
PROJECTILE("silver arrow", (char *)0,
		1,  8, 1, 2, 6, 6, 0, SILVER, WP_BOW, HI_SILVER),
PROJECTILE("ya", "bamboo arrow",
		0,  5, 1, 4, 7, 7, 1, METAL, WP_BOW, HI_METAL),
PROJECTILE("crossbow bolt", (char *)0,
		1, 40, 1, 2, 4, 6, 0, IRON, WP_CROSSBOW, HI_METAL),

WEAPON("dart", (char *)0,
	1, 1, 0, 50,  1,  2,  3,  2, 0, P,   WEP_MISSILE, IRON, HI_METAL),
WEAPON("shuriken", "throwing star",
	0, 1, 0, 35,  1,  5,  8,  6, 2, P,   WEP_MISSILE, IRON, HI_METAL),
WEAPON("boomerang", (char *)0,
	1, 1, 0, 15,  5, 20,  9,  9, 0, 0,   WEP_MISSILE, WOOD, HI_WOOD),

/* spears */
WEAPON("spear", (char *)0,
	1, 1, 0, 50, 30,  3,  6,  8, 0, P,   WEP_SPEAR, IRON, HI_METAL),
WEAPON("elven spear", "runed spear",
	0, 1, 0, 10, 30,  3,  7,  8, 0, P,   WEP_SPEAR, IRON, HI_METAL),
WEAPON("orcish spear", "crude spear",
	0, 1, 0, 13, 30,  3,  5,  8, 0, P,   WEP_SPEAR, IRON, CLR_BLACK),
WEAPON("dwarvish spear", "stout spear",
	0, 1, 0, 12, 35,  3,  8,  8, 0, P,   WEP_SPEAR, IRON, HI_METAL),
WEAPON("javelin", "throwing spear",
	0, 1, 0, 10, 20,  3,  6,  6, 0, P,   WEP_SPEAR, IRON, HI_METAL),

WEAPON("trident", (char *)0,
	1, 0, 0,  8, 25,  5,  6,  4, 0, P,   0, IRON, HI_METAL),
						/* +1 small, +2d4 large */

/* blades */
WEAPON("dagger", (char *)0,
	1, 1, 0, 25, 10,  4,  4,  3, 2, P,   WEP_BLADE, IRON, HI_METAL),
WEAPON("elven dagger", "runed dagger",
	0, 1, 0,  8, 10,  4,  5,  3, 2, P,   WEP_BLADE, IRON, HI_METAL),
WEAPON("dark elven dagger", "black runed dagger",
	0, 1, 0,  0, 10,  4,  5,  3, 2, P,   WEP_BLADE, IRON, CLR_BLACK),
WEAPON("orcish dagger", "crude dagger",
	0, 1, 0, 10, 10,  4,  3,  3, 2, P,   WEP_BLADE, IRON, CLR_BLACK),
WEAPON("athame", (char *)0,
	1, 1, 0,  0, 10,  4,  4,  3, 2, S,   WEP_BLADE, IRON, HI_METAL),
WEAPON("scalpel", (char *)0,
	1, 1, 0,  0,  5,  4,  3,  3, 2, S,   WEP_BLADE, IRON, HI_METAL),
WEAPON("knife", (char *)0,
	1, 1, 0, 15,  5,  4,  3,  2, 0, P|S, WEP_BLADE, IRON, HI_METAL),
WEAPON("stiletto", (char *)0,
	1, 1, 0,  5,  5,  4,  3,  2, 0, P|S, WEP_BLADE, IRON, HI_METAL),
WEAPON("worm tooth", (char *)0,
	1, 0, 0,  0, 20,  2,  2,  2, 0, 0,   0, 0, CLR_WHITE),
/* [Tom] increased crysknife damage from d10/d10 to d20/d30
	 (otherwise, it's useless to make them...) */
WEAPON("crysknife", (char *)0,
	1, 0, 0,  0, 20,100, 20, 30, 3, P,   WEP_BLADE, MINERAL, CLR_WHITE),
WEAPON("axe", (char *)0,
	1, 0, 0, 35, 60,  8,  6,  4, 0, S,   WEP_BLADE, IRON, HI_METAL),
WEAPON("battle-axe", "double-headed axe",
	0, 0, 1, 10,120, 40,  8,  6, 0, S,   WEP_BLADE, IRON, HI_METAL),
						/* "double-bitted" ? */

/* swords */
WEAPON("short sword", (char *)0,
	1, 0, 0,  8, 30, 10,  6,  8, 0, P,   WEP_SWORD, IRON, HI_METAL),
WEAPON("elven short sword", "runed short sword",
	0, 0, 0,  2, 30, 10,  8,  8, 0, P,   WEP_SWORD, IRON, HI_METAL),
WEAPON("dark elven short sword", "black runed short sword",
	0, 0, 0,  0, 30, 10,  8,  8, 0, P,   WEP_SWORD, IRON, CLR_BLACK),
WEAPON("orcish short sword", "crude short sword",
	0, 0, 0,  3, 30, 10,  5,  8, 0, P,   WEP_SWORD, IRON, CLR_BLACK),
WEAPON("dwarvish short sword", "broad short sword",
	0, 0, 0,  2, 30, 10,  7,  8, 0, P,   WEP_SWORD, IRON, HI_METAL),
WEAPON("scimitar", "curved sword",
	0, 0, 0, 15, 40, 15,  8,  8, 0, S,   WEP_SWORD, IRON, HI_METAL),
WEAPON("silver saber", (char *)0,
	1, 0, 0,  6, 40, 75,  8,  8, 0, S,   WEP_SWORD, SILVER, HI_SILVER),
/* STEPHEN WHITE'S NEW CODE */
WEAPON("silver dagger", (char *)0,
	1, 1, 0,  2, 12, 40,  4,  3, 2, S,   WEP_BLADE, SILVER, HI_SILVER),
WEAPON("silver short sword", (char *)0, 
	1, 0, 0,  2, 36, 50,  6,  8, 0, S,   WEP_SWORD, SILVER, HI_SILVER),
WEAPON("silver long sword", (char *)0, 
	1, 0, 0,  2, 48, 75,  8, 12, 0, S,   WEP_SWORD,SILVER, HI_SILVER),
WEAPON("silver mace", (char *)0,
	1, 0, 0,  2, 36, 65,  6,  6, 0, B, 0, SILVER, HI_SILVER),
WEAPON("silver spear", (char *)0,
	1, 1, 0,  2, 36, 40,  6,  8, 0, P,   WEP_SPEAR, SILVER, HI_SILVER),
WEAPON("broadsword", (char *)0,
	1, 0, 0, 18, 70, 10,  4,  6, 0, S,   WEP_SWORD, IRON, HI_METAL),
						/* +d4 small, +1 large */
WEAPON("elven broadsword", "runed broadsword",
	0, 0, 0,  4, 70, 10,  6,  6, 0, S,   WEP_SWORD, IRON, HI_METAL),
						/* +d4 small, +1 large */
WEAPON("long sword", (char *)0,
	1, 0, 0, 50, 40, 15,  8, 12, 0, S,   WEP_SWORD, IRON, HI_METAL),
WEAPON("two-handed sword", (char *)0,
	1, 0, 1, 20,150, 50, 12,  6, 0, S,   WEP_SWORD, IRON, HI_METAL),
						/* +2d6 large */
WEAPON("katana", "samurai sword",
	0, 0, 0,  4, 40, 80, 10, 12, 1, S,   WEP_SWORD, IRON, HI_METAL),
/* [Tom] adds star wars stuff... (same damage as unicorn horns -- d16/d20, +2 to hit) */
WEAPON("lightsaber", "green shimmering sword", 
	0, 0, 0,  1, 60, 500, 16,  20, 2, S, 0, PLASTIC, CLR_GREEN),
WEAPON("darksaber",  "red shimmering sword", 
	0, 0, 0,  1, 60, 500, 16,  20, 2, S, 0, PLASTIC, CLR_RED),
/* special swords set up for artifacts */
WEAPON("tsurugi", "long samurai sword",
	0, 0, 1,  0, 60,500, 16,  8, 2, S,   WEP_SWORD, METAL, HI_METAL),
						/* +2d6 large */
WEAPON("runesword", "runed broadsword",
	0, 0, 0,  0, 40,300,  4,  6, 0, S,   WEP_SWORD, IRON, CLR_BLACK),
						/* +d4 small, +1 large */
						/* +5d2 +d8 from level drain */
/* STEPHEN WHITE'S NEW CODE */
WEAPON("heavy hammer", (char *)0,
	1, 0, 1,  0, 60,500,  6,  6, 0, B,   0, METAL, HI_METAL),
WEAPON("wooden stake", (char *)0,
	1, 0, 0,  0, 20, 50,  6,  6, 0, B,   0, WOOD, HI_WOOD),
WEAPON("baseball bat", (char *)0,   
	1, 0, 0,  0, 40, 50,  8,  6, 0, B,   0, WOOD, HI_WOOD),
/* for necromancer artifact... */
WEAPON("great dagger", (char *)0,            
	1, 0, 0,  0, 20,500,  6,  7, 2, P,   WEP_BLADE, METAL, CLR_BLACK),
/* for Scalpel */
WEAPON("rapier", (char *)0,
	1, 0, 0,  0, 30, 40,  6,  8,  0, P,  WEP_BLADE, METAL, CLR_BLACK),

/* polearms */
/* spear-type */
WEAPON("partisan", "vulgar polearm",
	0, 0, 1, 10, 80, 10,  6,  6, 0, P,   WEP_POLEARM, IRON, HI_METAL),
						/* +1 large */
WEAPON("ranseur", "hilted polearm",
	0, 0, 1, 10, 50,  6,  4,  4, 0, P,   WEP_POLEARM, IRON, HI_METAL),
						/* +d4 both */
WEAPON("spetum", "forked polearm",
	0, 0, 1, 10, 50,  5,  6,  6, 0, P,   WEP_POLEARM, IRON, HI_METAL),
						/* +1 small, +d6 large */
WEAPON("glaive", "single-edged polearm",
	0, 0, 1, 15, 75,  6,  6, 10, 0, S,   WEP_POLEARM, IRON, HI_METAL),
WEAPON("lance", (char *)0,
	1, 0, 0,  8,180, 10,  6,  8, 0, P,   WEP_POLEARM, IRON, HI_METAL),
/* axe-type */
WEAPON("halberd", "angled poleaxe",
	0, 0, 1, 16,150, 10, 10,  6, 0, P|S, WEP_POLEARM, IRON, HI_METAL),
						/* +1d6 large */
WEAPON("bardiche", "long poleaxe",
	0, 0, 1,  8,120,  7,  4,  4, 0, S,   WEP_POLEARM, IRON, HI_METAL),
						/* +1d4 small, +2d4 large */
WEAPON("voulge", "pole cleaver",
	0, 0, 1,  8,125,  5,  4,  4, 0, S,   WEP_POLEARM, IRON, HI_METAL),
						/* +d4 both */
WEAPON("dwarvish mattock", "broad pick",
	0, 0, 1, 13,120, 50, 12,  8,-1, B,   WEP_POLEARM, IRON, HI_METAL),

/* curved/hooked */
WEAPON("fauchard", "pole sickle",
	0, 0, 1, 11, 60,  5,  6,  8, 0, P|S, WEP_POLEARM, IRON, HI_METAL),
WEAPON("guisarme", "pruning hook",
	0, 0, 1, 11, 80,  5,  4,  8, 0, S,   WEP_POLEARM, IRON, HI_METAL),
						/* +1d4 small */
WEAPON("bill-guisarme", "hooked polearm",
	0, 0, 1,  8,120,  7,  4, 10, 0, P|S, WEP_POLEARM, IRON, HI_METAL),
						/* +1d4 small */
/* other */
WEAPON("lucern hammer", "pronged polearm",
	0, 0, 1, 10,150,  7,  4,  6, 0, B|P, WEP_POLEARM, IRON, HI_METAL),
						/* +1d4 small */
WEAPON("bec de corbin", "beaked polearm",
	0, 0, 1,  8,100,  8,  8,  6, 0, B|P, WEP_POLEARM, IRON, HI_METAL),

/* bludgeons */
WEAPON("mace", (char *)0,
	1, 0, 0, 40, 30,  5,  6,  6, 0, B,   0, IRON, HI_METAL),
						/* +1 small */
WEAPON("morning star", (char *)0,
	1, 0, 0, 12,120, 10,  4,  6, 0, B,   0, IRON, HI_METAL),
						/* +d4 small, +1 large */
WEAPON("war hammer", (char *)0,
	1, 0, 0, 15, 50,  5,  4,  4, 0, B,   0, IRON, HI_METAL),
						/* +1 small */
WEAPON("club", (char *)0,
	1, 0, 0, 12, 30,  3,  6,  3, 0, B,   0, WOOD, HI_WOOD),
#ifdef KOPS
WEAPON("rubber hose", (char *)0,
	1, 0, 0,  0, 20,  3,  4,  3, 0, B,   0, PLASTIC, CLR_BROWN),
#endif
WEAPON("quarterstaff", "staff",
	0, 0, 1, 11, 40,  5,  6,  6, 0, B,   0, WOOD, HI_WOOD),
/* two-piece */
WEAPON("aklys", "thonged club",
	0, 0, 0,  8, 15,  4,  6,  3, 0, B,   0, IRON, HI_METAL),
WEAPON("flail", (char *)0,
	1, 0, 0, 40, 15,  4,  6,  4, 0, B,   0, IRON, HI_METAL),
						/* +1 small, +1d4 large */
/* misc */
WEAPON("bullwhip", (char *)0,
	1, 0, 0,  2, 20,  4,  2,  1, 0, 0,   0, LEATHER, CLR_BROWN),

/* bows */
/* damage values (num field #4) were really big -- 30-35 (except sling)
   weird, and changed */
BOW("bow", (char *)0,           1, 24, 30, 60, 4, 6, 0, WOOD, WP_BOW, HI_WOOD),
BOW("elven bow", "runed bow",   0, 12, 30, 60, 4, 6, 0, WOOD, WP_BOW, HI_WOOD),
BOW("dark elven bow",  "black runed bow",  0,  0, 30, 60, 4, 6, 0, WOOD, WP_BOW, CLR_BLACK),
BOW("orcish bow", "crude bow",  0, 12, 30, 60, 4, 6, 0, WOOD, WP_BOW, CLR_BLACK),
BOW("yumi", "long bow",         0,  0, 30, 60, 5, 6, 0, WOOD, WP_BOW, HI_WOOD),
BOW("sling", (char *)0,         1, 40,  3, 20, 4, 6, 0, WOOD, WP_SLING, HI_WOOD),
BOW("crossbow", (char *)0,      1, 45, 50, 40, 6, 6, 0, WOOD, WP_CROSSBOW, HI_WOOD),

#undef P
#undef S
#undef B

#undef WEAPON
#undef PROJECTILE
#undef BOW

/* armor ... */
/* IRON denotes ferrous metals, including steel.
 * Only IRON weapons and armor can rust.
 * Only COPPER (including brass) corrodes.
 * Some creatures are vulnerable to SILVER.
 */
#define ARMOR(name,desc,kn,mgc,blk,power,prob,delay,wt,cost,ac,can,sub,metal,c) \
	OBJECT( \
		OBJ(name,desc), BITS(kn,0,1,0,mgc,1,0,0,blk,0,0,sub,metal), power, \
		ARMOR_CLASS, prob, delay, wt, cost, \
		0, 0, 10 - ac, can, wt, c )
#define HELM(name,desc,kn,mgc,power,prob,delay,wt,cost,ac,can,metal,c) \
	ARMOR(name,desc,kn,mgc,0,power,prob,delay,wt,cost,ac,can,ARM_HELM,metal,c)
#define CLOAK(name,desc,kn,mgc,power,prob,delay,wt,cost,ac,can,metal,c) \
	ARMOR(name,desc,kn,mgc,0,power,prob,delay,wt,cost,ac,can,ARM_CLOAK,metal,c)
#define SHIELD(name,desc,kn,mgc,blk,power,prob,delay,wt,cost,ac,can,metal,c) \
	ARMOR(name,desc,kn,mgc,blk,power,prob,delay,wt,cost,ac,can,ARM_SHIELD,metal,c)
#define GLOVES(name,desc,kn,mgc,power,prob,delay,wt,cost,ac,can,metal,c) \
	ARMOR(name,desc,kn,mgc,0,power,prob,delay,wt,cost,ac,can,ARM_GLOVES,metal,c)
#define BOOTS(name,desc,kn,mgc,power,prob,delay,wt,cost,ac,can,metal,c) \
	ARMOR(name,desc,kn,mgc,0,power,prob,delay,wt,cost,ac,can,ARM_BOOTS,metal,c)

/* helmets */
HELM("elven leather helm", "leather hat",
		0, 0,  0,       7, 1,  3,   8, 9, 0, LEATHER, HI_LEATHER),
HELM("orcish helm", "iron skull cap",
		0, 0,  0,       7, 1, 30,  10, 9, 0, IRON, CLR_BLACK),
HELM("dwarvish iron helm", "hard hat",
		0, 0,  0,       7, 1, 40,  20, 8, 0, IRON, HI_METAL),
HELM("fire helmet", "red shiny helmet",
		0, 0, FIRE_RES, 2, 1, 50,  50, 9, 0, IRON, CLR_RED),
HELM("fedora", (char *)0,
		1, 0,  0,       2, 0,  3,   1,10, 0, CLOTH, CLR_BROWN),
HELM("cornuthaum", "conical hat",
		0, 1,  CLAIRVOYANT,
				2, 1,  4,  80,10, 2, CLOTH, CLR_BLUE),
HELM("dunce cap", "conical hat",
		0, 1,  0,       2, 1,  4,   1,10, 0, CLOTH, CLR_BLUE),
HELM("dented pot", (char *)0,
		1, 0,  0,	2, 0, 10,   8, 9, 0, IRON, CLR_BLACK),
HELM("helmet", "plumed helmet",
		0, 0,  0,       7, 1, 30,  10, 9, 0, IRON, HI_METAL),
HELM("helm of brilliance", "etched helmet",
		0, 1,  0,       2, 1, 50,  50, 9, 0, IRON, CLR_GREEN),
HELM("helm of opposite alignment", "crested helmet",
		0, 1,  0,       2, 1, 50,  50, 9, 0, IRON, HI_METAL),
HELM("helm of telepathy", "visored helmet",
		0, 1,  TELEPAT, 2, 1, 50,  50, 9, 0, IRON, HI_METAL),

/* suits of armor */
/*
 * There is code in polyself.c that assumes (1) and (2).
 * There is code in objnam.c, mon.c, read.c that assumes (2).
 *
 *	(1) The dragon scale mails and the dragon scales are together.
 *	(2) That the order of the dragon scale mail and dragon scales is the
 *	    the same defined in monst.c.
 */
#define DRGN_ARMR(name,power,cost,ac,color) \
	ARMOR(name,(char *)0,1,0,1,power,0,5,75,cost,ac,0,ARM_SUIT,DRAGON_HIDE,color)
DRGN_ARMR("gray dragon scale mail",   ANTIMAGIC,  1200, 1, CLR_GRAY),
DRGN_ARMR("silver dragon scale mail", REFLECTING, 1200, 1, SILVER),
DRGN_ARMR("shimmering dragon scale mail", DISPLACED, 1200, 1, CLR_CYAN),
DRGN_ARMR("deep dragon scale mail",   DRAIN_RES,  1200, 1, CLR_MAGENTA),
DRGN_ARMR("red dragon scale mail",    FIRE_RES,    900, 1, CLR_RED),
DRGN_ARMR("white dragon scale mail",  COLD_RES,    900, 1, CLR_WHITE),
DRGN_ARMR("orange dragon scale mail", SLEEP_RES,   900, 1, CLR_ORANGE),
DRGN_ARMR("black dragon scale mail",  DISINT_RES, 1200, 1, CLR_BLACK),
DRGN_ARMR("blue dragon scale mail",   SHOCK_RES,   900, 1, CLR_BLUE),
DRGN_ARMR("green dragon scale mail",  POISON_RES,  900, 1, CLR_GREEN),
DRGN_ARMR("yellow dragon scale mail", 0,           900, -4, CLR_YELLOW),

/* For now, only dragons leave these. */
DRGN_ARMR("gray dragon scales",   ANTIMAGIC,  700, 7, CLR_GRAY),
DRGN_ARMR("silver dragon scales", REFLECTING, 700, 7, SILVER),
DRGN_ARMR("shimmering dragon scales", DISPLACED,  700, 7, CLR_CYAN),
DRGN_ARMR("deep dragon scales",   DRAIN_RES,  500, 7, CLR_MAGENTA),
DRGN_ARMR("red dragon scales",    FIRE_RES,   500, 7, CLR_RED),
DRGN_ARMR("white dragon scales",  COLD_RES,   500, 7, CLR_WHITE),
DRGN_ARMR("orange dragon scales", SLEEP_RES,  500, 7, CLR_ORANGE),
DRGN_ARMR("black dragon scales",  DISINT_RES, 700, 7, CLR_BLACK),
DRGN_ARMR("blue dragon scales",   SHOCK_RES,  500, 7, CLR_BLUE),
DRGN_ARMR("green dragon scales",  POISON_RES, 500, 7, CLR_GREEN),
DRGN_ARMR("yellow dragon scales", 0,          500, 2, CLR_YELLOW),
#undef DRGN_ARMR

ARMOR("plate mail", (char *)0,
	1, 0, 1, 0,     20, 5, 450, 600,  3, 2, ARM_SUIT, IRON, HI_METAL),
ARMOR("crystal plate mail", (char *)0,
	1, 0, 1, 0,      3, 5, 450, 820,  3, 2, ARM_SUIT, GLASS, CLR_WHITE),
#ifdef TOURIST
ARMOR("bronze plate mail", (char *)0,
	1, 0, 1, 0,     22, 5, 450, 400,  4, 0, ARM_SUIT, COPPER, HI_COPPER),
#else
ARMOR("bronze plate mail", (char *)0,
	1, 0, 1, 0,     32, 5, 450, 400,  4, 0, ARM_SUIT, COPPER, HI_COPPER),
#endif
ARMOR("splint mail", (char *)0,
	1, 0, 1, 0,     30, 5, 400,  80,  4, 1, ARM_SUIT, IRON, HI_METAL),
ARMOR("banded mail", (char *)0,
	1, 0, 1, 0,     30, 5, 350,  90,  4, 0, ARM_SUIT, IRON, HI_METAL),
ARMOR("dwarvish mithril-coat", (char *)0,
	1, 0, 0, 0,	10, 1, 150, 240,  4, 3, ARM_SUIT, MITHRIL, HI_METAL),
ARMOR("dark elven mithril-coat", (char *)0,
	1, 0, 0, 0,      0, 1, 150, 240,  4, 3, ARM_SUIT, MITHRIL, CLR_BLACK),
ARMOR("elven mithril-coat", (char *)0,
	1, 0, 0, 0,	15, 1, 150, 240,  5, 3, ARM_SUIT, MITHRIL, HI_METAL),
ARMOR("chain mail", (char *)0,
	1, 0, 0, 0,     40, 5, 300,  75,  5, 1, ARM_SUIT, IRON, HI_METAL),
ARMOR("orcish chain mail", "crude chain mail",
	0, 0, 0, 0,     20, 5, 300,  75,  5, 1, ARM_SUIT, IRON, CLR_BLACK),
ARMOR("scale mail", (char *)0,
	1, 0, 0, 0,     70, 5, 250,  45,  6, 0, ARM_SUIT, IRON, HI_METAL),
ARMOR("studded leather armor", (char *)0,
	1, 0, 0, 0,     76, 3, 150,  15,  7, 1, ARM_SUIT, LEATHER, HI_LEATHER),
ARMOR("ring mail", (char *)0,
	1, 0, 0, 0,     70, 5, 250, 100,  7, 0, ARM_SUIT, IRON, HI_METAL),
ARMOR("orcish ring mail", "crude ring mail",
	0, 0, 0, 0,	20, 5, 250,  80,  8, 1, ARM_SUIT, IRON, CLR_BLACK),
#ifdef JFIGHTER
ARMOR("leather armor", (char *)0,
	1, 0, 0, 0,	83, 3, 150,   5,  8, 0, ARM_SUIT, LEATHER, HI_LEATHER),
#else
ARMOR("leather armor", (char *)0,
	1, 0, 0, 0,     95, 3, 100,   5,  8, 0, ARM_SUIT, LEATHER, HI_LEATHER),
#endif
ARMOR("leather jacket", (char *)0,
	1, 0, 0, 0,     10, 0, 30,  10,  9, 0, ARM_SUIT, LEATHER, CLR_BLACK),
ARMOR("asbestos jacket", "silver jacket",
	1, 0, 0, FIRE_RES,  2, 0,  30,  100,  9, 0, ARM_SUIT, MITHRIL, HI_SILVER),
#ifdef JFIGHTER
ARMOR("sailor blouse", (char *)0,
	1, 0, 0, 0,	12, 0,	30, 200,  7, 0, ARM_SUIT, CLOTH, CLR_WHITE),
#endif
/* STEPHEN WHITE'S NEW CODE */
ARMOR("robe", "red robe",
	0, 0, 0, 0,     50, 1,  40,  25,  9, 0, ARM_SUIT, LEATHER, CLR_RED),
ARMOR("robe of protection", "blue robe",
	0, 1, 0, PROTECTION, 10, 1,  40,  50,  5, 0, ARM_SUIT, LEATHER, CLR_BLUE),
ARMOR("robe of power", "orange robe",
	0, 1, 0, 0,     10, 1,  40,  50,  9, 0, ARM_SUIT, LEATHER, CLR_ORANGE),
ARMOR("robe of weakness", "green robe",
	0, 1, 0, 0,     10, 1,  40,  50,  9, 0, ARM_SUIT, LEATHER, CLR_GREEN),

#ifdef TOURIST
/* shirts */
ARMOR("Hawaiian shirt", (char *)0,
	1, 0, 0, 0,	 8, 0,	 5,   3, 10, 0, ARM_SHIRT, CLOTH, CLR_MAGENTA),
ARMOR("T-shirt", (char *)0,
	1, 0, 0, 0,	 2, 0,	 5,   2, 10, 0, ARM_SHIRT, CLOTH, CLR_WHITE),
#endif

/* cloaks */
/*  'cope' is not a spelling mistake... leave it be */
CLOAK("mummy wrapping", (char *)0,
		1, 0,	0,	    0, 0,  3,  2, 10, 1, CLOTH, CLR_GRAY),
CLOAK("elven cloak", "faded pall",
		0, 1,   STEALTH,   20, 0, 10, 60,  10, 3, CLOTH, CLR_BLACK),
CLOAK("orcish cloak", "coarse mantelet",
		0, 0,   0,         22, 0, 10, 40, 10, 2, CLOTH, CLR_BLACK),
CLOAK("dwarvish cloak", "hooded cloak",
		0, 0,   0,          9, 0, 10, 50, 10, 2, CLOTH, HI_CLOTH),
CLOAK("lab coat", "white coat",
		0, 1,   POISON_RES, 3, 0, 10, 60,  9, 3, CLOTH, CLR_WHITE),
CLOAK("oilskin cloak", "slippery cloak",
		0, 0,   0,          9, 0, 10, 50,  10, 3, CLOTH, HI_CLOTH),
CLOAK("cloak of protection", "tattered cape",
		0, 1,   PROTECTION, 4, 0, 10, 50,  7, 3, CLOTH, HI_CLOTH),
CLOAK("cloak of invisibility", "opera cloak",
		0, 1,   INVIS,      5, 0, 10, 60,  9, 2, CLOTH, CLR_BRIGHT_MAGENTA),
CLOAK("cloak of magic resistance", "ornamental cope",
		0, 1,   ANTIMAGIC,  1, 0, 10, 60,  10, 3, CLOTH, CLR_WHITE),
CLOAK("cloak of drain resistance", "gleaming cloak",
		0, 1,   DRAIN_RES,  2, 0, 10, 60, 10, 3, CLOTH, HI_SILVER),
CLOAK("cloak of poisonousness", "sickly cloak",
		0, 1,   0,          1, 0, 10, 40, 10, 3, CLOTH, CLR_GREEN),
CLOAK("cloak of displacement", "piece of cloth",
		0, 1,   DISPLACED,  1, 0, 10, 50,  9, 2, CLOTH, HI_CLOTH),

/* shields */
SHIELD("small shield", (char *)0,
		1, 0, 0, 0,         45, 0, 30,  3,  9, 0, WOOD, HI_WOOD),
SHIELD("elven shield", "blue and green shield",
		0, 0, 0, 0,          5, 0, 50,  7,  8, 0, IRON, CLR_GREEN),
SHIELD("Uruk-hai shield", "white-handed shield",
		0, 0, 0, 0,         10, 0, 50,  7,  9, 0, IRON, HI_METAL),
SHIELD("orcish shield", "red-eyed shield",
		0, 0, 0, 0,         12, 0, 50,  7,  9, 0, IRON, CLR_RED),
SHIELD("large shield", (char *)0,
		1, 0, 1, 0,         25, 0, 75, 10,  8, 0, IRON, HI_METAL),
SHIELD("dwarvish roundshield", "large round shield",
		0, 0, 0, 0,         10, 0,100, 10,  8, 0, IRON, HI_METAL),
SHIELD("shield of reflection", "polished silver shield",
		0, 1, 0, REFLECTING, 1, 0, 50, 50,  8, 0, SILVER, HI_SILVER),

/* gloves */
/* these have their color but not material shuffled, so the IRON must stay
 * CLR_BROWN (== HI_LEATHER)
 */
GLOVES("leather gloves", "old gloves",
		0, 0,  0,         30, 1, 10,  8,  10, 0, LEATHER, HI_LEATHER),
GLOVES("gauntlets of fumbling", "padded gloves",
		0, 1,  FUMBLING,   5, 1, 10, 50,  10, 0, LEATHER, HI_LEATHER),
GLOVES("gauntlets of power", "riding gloves",
		0, 1,  0,          3, 1, 30, 50,  10, 0, IRON, CLR_BROWN),
GLOVES("gauntlets of swimming", "black gloves",
		0, 1,  SWIMMING,   3, 1, 10, 50, 10, 0, LEATHER, CLR_BLACK),
GLOVES("gauntlets of dexterity", "fencing gloves",
		0, 1,  0,          3, 1, 10, 50,  10, 0, LEATHER, HI_LEATHER),

/* boots */
BOOTS("low boots", "walking shoes",
		0, 0,  0,         37, 2, 10,  8,  9, 0, LEATHER, HI_LEATHER),
BOOTS("iron shoes", "hard shoes",
		0, 0,  0,         10, 2, 50, 16,  8, 0, IRON, HI_METAL),
BOOTS("high boots", "jackboots",
		0, 0,  0,         32, 2, 20, 12,  8, 0, LEATHER, HI_LEATHER),
BOOTS("speed boots", "combat boots",
		0, 1,  FAST,       3, 2, 20, 50,  10, 0, LEATHER, HI_LEATHER),
BOOTS("water walking boots", "jungle boots",
		0, 1,  WWALKING,   3, 2, 20, 50,  10, 0, LEATHER, HI_LEATHER),
BOOTS("jumping boots", "hiking boots",
		0, 1,  JUMPING,    3, 2, 20, 50,  10, 0, LEATHER, HI_LEATHER),
BOOTS("elven boots", "mud boots",
		0, 1,  STEALTH,    7, 2, 15,  8,  10, 0, LEATHER, HI_LEATHER),
BOOTS("kicking boots", "steel boots",
		0, 1,  0,          3, 2, 15,  8, 10, 0, IRON, HI_METAL),
BOOTS("fumble boots", "riding boots",
		0, 1,  FUMBLING,   3, 2, 20, 30,  10, 0, LEATHER, HI_LEATHER),
BOOTS("levitation boots", "snow boots",
		0, 1,  LEVITATION, 3, 2, 15, 30,  10, 0, LEATHER, HI_LEATHER),
#undef HELM
#undef CLOAK
#undef SHIELD
#undef GLOVES
#undef BOOTS
#undef ARMOR

/* rings ... */
#define RING(name,power,stone,cost,mgc,spec,mohs,metal,color) OBJECT( \
		OBJ(name,stone), \
		BITS(0,0,spec,0,mgc,spec,0,0,0,HARDGEM(mohs),0,0,metal), \
		power, RING_CLASS, 0, 0, 3, cost, 0, 0, 0, 0, 15, color )
RING("adornment",       ADORNED,"wooden",     100, 0, 1, 2, WOOD,     HI_WOOD),
RING("gain strength", 0, "granite",         150, 1, 1, 7, MINERAL, HI_MINERAL),
RING("gain constitution",
			0,      "opal",       150, 1, 1, 7, MINERAL,  HI_MINERAL),
/* [Tom] looks like there are no probs to change... */
RING("gain intelligence",
			0,      "plain",      150, 1, 1, 7, MINERAL,  HI_MINERAL),
RING("gain wisdom",     0,      "glass",      150, 1, 1, 7, MINERAL,  CLR_CYAN),
RING("gain dexterity",  0,      "obsidian",   150, 1, 1, 7, GEMSTONE, CLR_BLACK),
RING("increase damage", 0, "coral",         150, 1, 1, 4, MINERAL, CLR_ORANGE),
RING("protection",      PROTECTION,
				"black onyx", 100, 1, 1, 7, MINERAL,  CLR_BLACK),
RING("regeneration",    REGENERATION,
				"moonstone",  200, 1, 0, 6, MINERAL,  HI_MINERAL),
RING("searching",       SEARCHING,
				"tiger eye",  200, 1, 0, 6, GEMSTONE, CLR_BROWN),
RING("stealth", STEALTH, "jade",            100, 1, 0, 6, GEMSTONE, CLR_GREEN),
RING("levitation",      LEVITATION,
				"agate",      200, 1, 0, 7, GEMSTONE, CLR_RED),
RING("hunger", HUNGER, "topaz",             100, 1, 0, 8, GEMSTONE, CLR_CYAN),
RING("sleeping",        SLEEPING,
				"wedding",    100, 1, 0, 7, GEMSTONE, CLR_WHITE),
RING("aggravate monster",
			AGGRAVATE_MONSTER,
				"sapphire",   150, 1, 0, 9, GEMSTONE, CLR_BLUE),
RING("drain resistance",DRAIN_RES,
				"black",      250, 1, 0, 7, IRON,     CLR_BLACK),
RING("conflict",        CONFLICT,
				"ruby",       300, 1, 0, 9, GEMSTONE, CLR_RED),
RING("warning", WARNING, "diamond",         100, 1, 0,10, GEMSTONE, CLR_WHITE),
RING("poison resistance",
			POISON_RES,
				"pearl",      150, 1, 0, 4, IRON,     CLR_WHITE),
RING("fire resistance", FIRE_RES,
				"iron",       200, 1, 0, 5, IRON,     HI_METAL),
RING("cold resistance", COLD_RES,
				"brass",      150, 1, 0, 4, COPPER,   HI_COPPER),
RING("shock resistance",SHOCK_RES,
				"copper",     150, 1, 0, 3, COPPER,   HI_COPPER),
RING("teleportation",   TELEPORT,
				"silver",     200, 1, 0, 3, SILVER,   HI_SILVER),
RING("teleport control",TELEPORT_CONTROL,
				"gold",       300, 1, 0, 3, GOLD,     HI_GOLD),
RING("polymorph",       POLYMORPH,
				"ivory",      300, 1, 0, 4, BONE,     CLR_WHITE),
RING("polymorph control",
			POLYMORPH_CONTROL, 
				"emerald",    300, 1, 0, 8, GEMSTONE, CLR_BRIGHT_GREEN),
RING("invisibility", INVIS, "wire",         150, 1, 0, 5, IRON, HI_METAL),
RING("free action",     FREE_ACTION,
				"twisted",    200, 1, 0, 6, IRON, HI_METAL),
RING("see invisible",   SEE_INVIS,
				"engagement", 150, 1, 0, 5, IRON, HI_METAL),
RING("protection from shape changers", 
			PROT_FROM_SHAPE_CHANGERS,
				"shiny",      100, 1, 0, 5, IRON, CLR_BRIGHT_CYAN),
#undef RING

/* amulets ... - THE Amulet comes last because it is special */
#define AMULET(name,desc,power,prob) OBJECT( \
		OBJ(name,desc), BITS(0,0,0,0,1,0,0,0,0,0,0,0,IRON), power, \
		AMULET_CLASS, prob, 0, 20, 150, 0, 0, 0, 0, 20, HI_METAL )

AMULET("amulet of ESP",           "circular",   TELEPAT,     80),
AMULET("amulet of life saving",   "spherical",  LIFESAVED,   20),
AMULET("amulet of strangulation", "oval",       STRANGLED,  180),
AMULET("amulet of restful sleep", "triangular", SLEEPING,   100),
AMULET("amulet versus poison",    "pyramidal",  POISON_RES, 170),
AMULET("amulet of drain resistance","warped",   DRAIN_RES,   60),
AMULET("amulet of regeneration",  "twisted",    REGENERATION,30),
AMULET("amulet of conflict",      "concave",    CONFLICT,    20),
AMULET("amulet of change",        "square",     0,          120),
/* [Tom] added amulet of polymorph, life saving% used to be 80, reflection
	 used to be 80, change used to be 140, vs. poison used to be 170,
	 restful sleep used to be 140 */
/* [Tom] later made ESP, reflect, and life saving more rare. */
AMULET("amulet of polymorph",     "lunate",     0,          120),
						/* POLYMORPH */
AMULET("amulet of reflection",    "hexagonal",  REFLECTING,  30),
AMULET("amulet of magical breathing", "octagonal",      MAGICAL_BREATHING, 70),
OBJECT(OBJ("cheap plastic imitation of the Amulet of Yendor",
	"Amulet of Yendor"), BITS(0,0,1,0,0,0,0,0,0,0,0,0,PLASTIC), 0,
	AMULET_CLASS, 0, 0, 20,    0, 0, 0, 0, 0,  1, HI_METAL),
OBJECT(OBJ("Amulet of Yendor",(char *)0), BITS(1,0,1,0,1,0,1,1,0,0,0,0,MITHRIL), 0,
	AMULET_CLASS, 0, 0, 20, 3500, 0, 0, 0, 0, 20, HI_METAL),
#undef AMULET

/* tools ... */
/* tools with weapon characteristics come last */
#define TOOL(name,desc,kn,mrg,mgc,chg,prob,wt,cost,mat,color) \
	OBJECT( OBJ(name,desc), \
		BITS(kn,mrg,chg,0,mgc,chg,0,0,0,0,0,0,mat), \
		0, TOOL_CLASS, prob, 0, \
		wt, cost, 0, 0, 0, 0, wt, color )
#define CONTAINER(name,desc,kn,mgc,chg,prob,wt,cost,mat,color) \
	OBJECT( OBJ(name,desc), \
		BITS(kn,0,chg,1,mgc,chg,0,0,0,0,0,0,mat), \
		0, TOOL_CLASS, prob, 0, \
		wt, cost, 0, 0, 0, 0, wt, color )
#define WEPTOOL(name,desc,kn,mgc,bi,prob,wt,cost,sdam,ldam,hitbon,sub,mat,clr) \
	OBJECT( OBJ(name,desc), \
		BITS(kn,0,1,0,mgc,1,0,0,bi,0,hitbon,sub,mat), \
		0, TOOL_CLASS, prob, 0, \
		wt, cost, sdam, ldam, hitbon, 0, wt, clr )
/* containers */
CONTAINER("large box", (char *)0,       1, 0, 0,  30,350,   8, WOOD, HI_WOOD),
CONTAINER("chest", (char *)0,           1, 0, 0,  25,600,  16, WOOD, HI_WOOD),
CONTAINER("ice box", (char *)0,         1, 0, 0,   5,900,  42, PLASTIC, CLR_WHITE),
CONTAINER("sack", "bag",                0, 0, 0,  45, 15,   2, CLOTH, HI_CLOTH),
CONTAINER("oilskin sack", "bag",        0, 0, 0,  15, 15, 100, CLOTH, HI_CLOTH),
CONTAINER("bag of holding", "bag",      0, 1, 0,  10, 15, 100, CLOTH, HI_CLOTH),
CONTAINER("bag of tricks", "bag",       0, 1, 1,  10, 15, 100, CLOTH, HI_CLOTH),
#undef CONTAINER

/* lock opening tools */
TOOL("skeleton key", "key",     0, 0, 0, 0,  40,  3,  10, IRON, HI_METAL),
#ifdef TOURIST
TOOL("lock pick", (char *)0,    1, 0, 0, 0,  30,  4,  20, IRON, HI_METAL),
TOOL("credit card", (char *)0,  1, 0, 0, 0,  15,  1,  10, PLASTIC, CLR_WHITE),
#else
TOOL("lock pick", (char *)0,    1, 0, 0, 0,  45,  4,  20, IRON, HI_METAL),
#endif
/* light sources */
/* [Tom] made cheaper & more common */
TOOL("tallow candle", "candle", 0, 1, 0, 0,  110,  2,  1, WAX, CLR_WHITE),
TOOL("wax candle", "candle",    0, 1, 0, 0,   80,  2,  2, WAX, CLR_WHITE),
/* STEPHEN WHITE'S NEW CODE */
TOOL("magic candle",  "candle", 0, 1, 0, 0,   5,  2,1000, WAX, CLR_WHITE),
TOOL("brass lantern", (char *)0,1, 0, 0, 0,  25, 100,  12, COPPER, CLR_YELLOW),
TOOL("oil lamp", "lamp",        0, 0, 0, 0,  35, 80,  10, COPPER, CLR_YELLOW),
TOOL("magic lamp", "lamp",      0, 0, 1, 0,   5, 80,  2000, COPPER, CLR_YELLOW),
/* other tools */
#ifdef TOURIST
TOOL("expensive camera", (char *)0,
				1, 0, 0, 0,  15, 30, 200, PLASTIC, CLR_BLACK),
TOOL("mirror", "looking glass", 0, 0, 0, 0,  45, 13,  10, GLASS, HI_SILVER),
#else
TOOL("mirror", "looking glass", 0, 0, 0, 0,  60, 13,  10, GLASS, HI_SILVER),
#endif
TOOL("crystal ball", "glass orb",
				0, 0, 1, 1,  15,150,  60, GLASS, HI_GLASS),
/* STEPHEN WHITE'S NEW CODE */
TOOL("orb of enchantment", "glass orb",
				0, 0, 1, 1,   5, 75, 750, GLASS, HI_GLASS),
TOOL("orb of charging", "glass orb",
				0, 0, 1, 1,   5, 75, 750, GLASS, HI_GLASS),
TOOL("orb of destruction", "glass orb",
				0, 0, 0, 0,   5, 75, 750, GLASS, HI_GLASS),

TOOL("blindfold", (char *)0,    1, 0, 0, 0,  55,  2,  20, CLOTH, CLR_BLACK),
TOOL("towel", (char *)0,        1, 0, 0, 0,  50,  2,  50, CLOTH, CLR_MAGENTA),
TOOL("leash", (char *)0,        1, 0, 0, 0,  65, 12,  20, LEATHER, HI_LEATHER),
TOOL("stethoscope", (char *)0,  1, 0, 0, 0,  25,  4,  75, IRON, HI_METAL),
TOOL("tinning kit", (char *)0,  1, 0, 0, 1,  15, 75,  30, IRON, HI_METAL),
TOOL("medical kit", "leather bag",
				0, 0, 0, 1,  10, 25, 500, LEATHER, HI_LEATHER),
TOOL("tin opener", (char *)0,   1, 0, 0, 0,  15,  4,  30, IRON, HI_METAL),
TOOL("can of grease", (char *)0,1, 0, 0, 1,  15, 15,  20, IRON, HI_METAL),
TOOL("figurine", (char *)0,     1, 0, 1, 0,  35, 50,  150, MINERAL, HI_MINERAL),
TOOL("magic marker", (char *)0, 1, 0, 1, 1,  15,  2,  50, PLASTIC, CLR_RED),
/* traps */
TOOL("land mine",(char *)0,     1, 0, 0, 0,   0,300, 180, IRON, CLR_RED),
TOOL("beartrap", (char *)0,     1, 0, 0, 0,   0,200,  60, IRON, HI_METAL),
/* instruments */
TOOL("tin whistle", "whistle",  0, 0, 0, 0,  55,  3,  10, METAL, HI_METAL),
TOOL("magic whistle", "whistle",0, 0, 1, 0,   5,  3,  500, METAL, HI_METAL),
/* "If tin whistles are made out of tin, what do they make foghorns out of?" */
TOOL("wooden flute", "flute",   0, 0, 0, 0,   4,  5,  12, WOOD, HI_WOOD),
TOOL("magic flute", "flute",    0, 0, 1, 1,   2,  5,  600, WOOD, HI_WOOD),
TOOL("tooled horn", "horn",     0, 0, 0, 0,   5, 18,  15, BONE, CLR_WHITE),
TOOL("frost horn", "horn",      0, 0, 1, 1,   2, 18,  300, BONE, CLR_WHITE),
TOOL("fire horn", "horn",       0, 0, 1, 1,   2, 18,  300, BONE, CLR_WHITE),
TOOL("horn of plenty", "horn",  0, 0, 1, 1,   2, 18,  300, BONE, CLR_WHITE),
TOOL("wooden harp", "harp",     0, 0, 1, 0,   4, 30,  50, WOOD, HI_WOOD),
TOOL("magic harp", "harp",      0, 0, 1, 1,   2, 30,  600, WOOD, HI_WOOD),
TOOL("bell", (char *)0,         1, 0, 0, 0,   2, 30,  50, COPPER, HI_COPPER),
TOOL("bugle", (char *)0,        1, 0, 0, 0,   4, 10,  15, COPPER, HI_COPPER),
TOOL("leather drum", "drum",    0, 0, 0, 0,   4, 25,  25, LEATHER, HI_LEATHER),
TOOL("pan pipe of summoning", "set of pipes",
				0, 0, 1, 1,   3,  5, 100, WOOD, HI_WOOD),
TOOL("pan pipe of the sewers", "set of pipes",
				0, 0, 1, 1,   2,  5, 200, WOOD, HI_WOOD),
TOOL("pan pipe", "set of pipes",
				0, 0, 1, 0,   5,  5,  20, WOOD, HI_WOOD),
TOOL("drum of earthquake", "drum",
				0, 0, 1, 1,   2, 25,  25, LEATHER, HI_LEATHER),
/* tools useful as weapons */
WEPTOOL("pick-axe", (char *)0,
	1, 0, 1, 20, 80,  50,  6,  3, WHACK,  WEP_BLADE, IRON, HI_METAL),
/* [Tom] increase unicorn horn damage from d12/d12 to d16/d20 */
WEPTOOL("unicorn horn", (char *)0,
	1, 1, 1,  0,  20, 100, 16, 20, PIERCE, WEP_SPEAR, BONE, CLR_WHITE),

/* two special unique artifact "tools" */
OBJECT(OBJ("Candelabrum of Invocation", "candelabrum"),
		BITS(0,0,1,0,1,0,1,1,0,0,0,0,GOLD), 0,
		TOOL_CLASS, 0, 0,10, 3000, 0, 0, 0, 0, 200, HI_GOLD),
OBJECT(OBJ("Bell of Opening", "silver bell"),
		BITS(0,0,1,0,1,1,1,1,0,0,0,0,SILVER), 0,
		TOOL_CLASS, 0, 0,10, 1000, 0, 0, 0, 0, 50, HI_SILVER),
#undef TOOL
#undef WEPTOOL

/* comestibles ... */
#define FOOD(name,prob,delay,wt,unk,tin,nutrition,color) OBJECT( \
		OBJ(name,(char *)0), BITS(1,1,unk,0,0,0,0,0,0,0,0,0,tin), 0, \
		FOOD_CLASS, prob, delay, \
		wt, nutrition/20 + 5, 0, 0, 0, 0, nutrition, color )
/* all types of food (except tins & corpses) must have a delay of at least 1. */
/* delay on corpses is computed and is weight dependant */
/* dog eats foods 0-4 but prefers tripe rations above all others */
/* fortune cookies can be read */
/* carrots improve your vision */
/* +0 tins contain monster meat */
/* +1 tins (of spinach) make you stronger (like Popeye) */
/* food CORPSE is a cadaver of some type */

/* meat */
FOOD("tripe ration",       142, 2, 10, 0, FLESH, 200, CLR_BROWN),
FOOD("corpse",               0, 1,  0, 0, FLESH,   0, CLR_BROWN),
FOOD("egg",                 80, 1,  1, 1, FLESH,  80, CLR_WHITE),

/* fruits & veggies */
FOOD("apple",               13, 1,  2, 0, VEGGY,  50, CLR_RED),
FOOD("orange",              10, 1,  2, 0, VEGGY,  80, CLR_ORANGE),
FOOD("pear",                 9, 1,  2, 0, VEGGY,  50, CLR_BRIGHT_GREEN),

FOOD("asian pear",           1, 1,  2, 0, VEGGY,  75, CLR_BRIGHT_GREEN),
FOOD("mushroom",             1, 1,  5, 0, VEGGY,  90, CLR_BLACK),

FOOD("melon",                9, 1,  5, 0, VEGGY, 100, CLR_BRIGHT_GREEN),
FOOD("banana",              10, 1,  2, 0, VEGGY,  80, CLR_YELLOW),
FOOD("carrot",              15, 1,  2, 0, VEGGY,  50, CLR_ORANGE),

FOOD("sprig of wolfsbane",   7, 1,  1, 0, VEGGY,  40, CLR_GREEN),
FOOD("clove of garlic",      7, 1,  1, 0, VEGGY,  40, CLR_WHITE),
FOOD("holy wafer",           7, 1,  1, 0, VEGGY, 150, CLR_WHITE),

/* body parts.... eeeww */
FOOD("eyeball",              0, 1,  0, 0, FLESH,  10, CLR_WHITE),
FOOD("severed hand",         0, 1,  0, 0, FLESH,  40, CLR_BROWN),

FOOD("slime mold",          75, 1,  5, 0, VEGGY, 250, HI_ORGANIC),
/*
FOOD("slice of pizza",      75, 1,  3, 0, VEGGY, 250, CLR_RED),
*/

/* people food */
FOOD("lump of royal jelly",  1, 1,  2, 0, VEGGY, 200, CLR_YELLOW),
FOOD("cream pie",           14, 1, 10, 0, VEGGY, 100, CLR_WHITE),
FOOD("sandwich",            10, 1, 10, 0, VEGGY, 100, CLR_WHITE),
FOOD("candy bar",           13, 1,  2, 0, VEGGY, 100, CLR_BROWN),
FOOD("fortune cookie",      55, 1,  1, 0, VEGGY,  40, CLR_YELLOW),
FOOD("pancake",             14, 2,  2, 0, VEGGY, 200, CLR_YELLOW),
FOOD("tortilla",             1, 2,  2, 0, VEGGY,  80, CLR_WHITE),

/* [Tom] more food.... taken off pancake (25) */
FOOD("cheese",              10, 2,  2, 0, FLESH, 250, CLR_YELLOW),
FOOD("pill",                 1, 1,  1, 0, VEGGY,   0, CLR_BRIGHT_MAGENTA),

FOOD("lembas wafer",        20, 2,  5, 0, VEGGY, 800, CLR_WHITE),
FOOD("cram ration",         20, 3, 15, 0, VEGGY, 600, HI_ORGANIC),
FOOD("food ration",        380, 5, 20, 0, VEGGY, 800, HI_ORGANIC),

FOOD("K-ration",             0, 1, 10, 0, VEGGY, 400, HI_ORGANIC),
FOOD("C-ration",             0, 1, 10, 0, VEGGY, 300, HI_ORGANIC),

FOOD("tin",                 75, 0, 10, 1, METAL,   0, HI_METAL),
#undef FOOD

/* potions ... */
#define POTION(name,desc,mgc,power,prob,cost,color) OBJECT( \
		OBJ(name,desc), BITS(0,1,0,0,mgc,0,0,0,0,0,0,0,GLASS), power, \
		POTION_CLASS, prob, 0, 20, cost, 0, 0, 0, 0, 10, color )
POTION("gain ability", "ruby",          1, 0,          35, 300, CLR_RED),
POTION("restore ability", "pink",       1, 0,          45, 30, CLR_BRIGHT_MAGENTA),
POTION("confusion", "orange",           1, CONFUSION,  45, 30, CLR_ORANGE),
POTION("blindness", "yellow",           1, BLINDED,    35, 50, CLR_YELLOW),
POTION("paralysis", "emerald",          1, 0,          25, 100, CLR_BRIGHT_GREEN),
POTION("sleeping",  "murky",            1, 0,          20, 100, CLR_GRAY),
POTION("speed", "dark green",           1, FAST,       35, 60, CLR_GREEN),
POTION("levitation", "cyan",            1, LEVITATION, 25, 60, CLR_CYAN),
POTION("hallucination", "sky blue",     1, HALLUC,     45, 30, CLR_CYAN),
POTION("invisibility", "brilliant blue",1, INVIS,      45, 50, CLR_BRIGHT_BLUE),
POTION("see invisible", "magenta",      1, SEE_INVIS,  45,  25, CLR_MAGENTA),
POTION("healing", "purple-red",         1, 0,          65, 25, CLR_MAGENTA),
POTION("extra healing", "puce",         1, 0,          50, 75, CLR_RED),
POTION("full healing",  "black",        1, 0,          20, 200, CLR_BLACK),
/* [Tom] added full healing, took probs from gain ability 45, and
	 gain level 20 */
/* [Tom] added potion of polymorph.
	 Old probabilities I changed:
		paralysis was 45, levitation was 45
*/
POTION("polymorph",     "golden",       1, 0,          30, 200, CLR_YELLOW),
POTION("gain level", "milky",           1, 0,          10, 300, CLR_WHITE),
POTION("enlightenment", "swirly",       1, 0,          20, 200, CLR_BROWN),
POTION("monster detection", "bubbly",   1, 0,          35, 50, CLR_WHITE),
POTION("object detection", "smoky",     1, 0,          35, 50, CLR_GRAY),
POTION("clairvoyance","luminescent",    1, 0,          10,  75, CLR_WHITE),
POTION("ESP",         "muddy",          1, TELEPAT,    10, 150, CLR_BROWN),
POTION("fire resistance","maroon",      1, 0,          10,  75, CLR_RED),
POTION("invulnerability", "icy",        1, 0,          10, 300, CLR_BRIGHT_BLUE),
POTION("gain energy", "cloudy",         1, 0,          35, 100, CLR_WHITE),
POTION("booze", "brown",                0, 0,          45,  15, CLR_BROWN),
POTION("sickness", "fizzy",             0, 0,          45,  50, CLR_CYAN),
POTION("fruit juice", "dark",           0, 0,          45,  15, CLR_BLACK),
POTION("oil", "viscous",                0, 0,          20, 150, CLR_BROWN),
POTION("water", "clear",                0, 0,         105, 100, CLR_CYAN),
#undef POTION

/* scrolls ... */
#define SCROLL(name,text,mgc,prob,cost) OBJECT( \
		OBJ(name,text), BITS(0,1,0,0,mgc,0,0,0,0,0,0,0,PAPER), 0, \
		SCROLL_CLASS, prob, 0, 5, cost, 0, 0, 0, 0, 6, HI_PAPER )
	SCROLL("enchant armor",         "ZELGO MER",            1,  40,  500),
	SCROLL("destroy armor",         "JUYED AWK YACC",       1,  30, 100),
	SCROLL("confuse monster",       "NR 9",                 1,  40, 100),
	SCROLL("scare monster",         "XIXAXA XOXAXA XUXAXA", 1,  40, 100),
	SCROLL("remove curse",          "PRATYAVAYAH",          1, 100,  80),
	SCROLL("enchant weapon",        "DAIYEN FOOELS",        1,  40,  500),
	SCROLL("create monster",        "LEP GEX VEN ZEA",      1,  50, 200),
	SCROLL("taming",                "PRIRUTSENIE",          1,  10, 900),
	SCROLL("genocide",              "ELBIB YLOH",           1,   5, 1000),
	SCROLL("light",                 "VERR YED HORRE",       1, 100,  25),
	SCROLL("teleportation",         "VENZAR BORGAVVE",      1,  70, 100),
	SCROLL("gold detection",        "THARR",                1,  50, 100),
	SCROLL("food detection",        "YUM YUM",              1,  50, 100),
	SCROLL("identify",              "KERNOD WEL",           1, 220,  10),
	SCROLL("magic mapping",         "ELAM EBOW",            1,  30, 100),
	SCROLL("amnesia",               "DUAM XNAHT",           1,  15, 200),
	SCROLL("fire",                  "ANDOVA BEGARIN",       1,  20, 100),
	SCROLL("punishment",            "VE FORBRYDERNE",       1,  15, 300),
	SCROLL("trap detection",        "SKCUS LAVUY",          1,  30, 100),
	SCROLL("charging",              "HACKEM MUCHE",         1,  20, 500),
	SCROLL((char *)0,		"VELOX NEB",            1,   0, 100),
	SCROLL((char *)0,		"FOOBIE BLETCH",        1,   0, 100),
	SCROLL((char *)0,		"TEMOV",                1,   0, 100),
	SCROLL((char *)0,		"GARVEN DEH",           1,   0, 100),
	SCROLL((char *)0,		"READ ME",              1,   0, 100),
	SCROLL((char *)0,		"KIRJE",                1,   0, 100),
	/* these must come last because they have special descriptions */
#ifdef MAIL
	SCROLL("mail",                  "stamped",          0,   0,   0),
#endif
	SCROLL("blank paper",           "unlabeled",        0,  25,  60),
#undef SCROLL

/* spell books ... */
#define SPELL(name,desc,prob,delay,level,mgc,dir,color) OBJECT( \
		OBJ(name,desc), BITS(0,0,0,0,mgc,0,0,0,0,0,dir,0,PAPER), 0, \
		SPBOOK_CLASS, prob, delay, \
		50, level*100, 0, 0, 0, level, 20, color )
/* STEPHEN WHITE'S NEW CODE - enchant weapon & armor, endure heat,  
 *                            endure cold, resist poison, resist sleep,
 *                            insolate, enlighten  
 */
SPELL("dig",             "parchment",   14,  6, 5, 1, RAY,       HI_PAPER),
SPELL("magic missile",   "vellum",      28,  3, 2, 1, RAY,       HI_PAPER),
SPELL("fireball",        "ragged",      19,  5, 4, 1, RAY,       HI_PAPER),
SPELL("cone of cold",    "dog eared",   14,  6, 5, 1, RAY,       HI_PAPER),
SPELL("sleep",           "mottled",     35,  2, 1, 1, RAY,       HI_PAPER),
SPELL("finger of death", "stained",      5, 8, 7, 1, RAY,       HI_PAPER),
SPELL("light",           "cloth",       35,  2, 1, 1, NODIR,     HI_CLOTH),
SPELL("detect monsters", "leather",     35,  2, 1, 1, NODIR,     HI_LEATHER),
SPELL("knock",           "pink",        35,  2, 1, 1, IMMEDIATE, CLR_BRIGHT_MAGENTA),
SPELL("healing",         "white",       35,  2, 1, 1, IMMEDIATE, CLR_WHITE),
SPELL("force bolt",      "red",         35,  2, 1, 1, IMMEDIATE, CLR_RED),
SPELL("resist poison",   "big",         35,  2, 1, 1, NODIR,     CLR_GRAY),
SPELL("resist sleep",    "fuzzy",       35,  2, 1, 1, NODIR,     CLR_BROWN),
SPELL("detect food",     "cyan",        35,  2, 1, 1, NODIR,     CLR_CYAN),
SPELL("wizard lock",     "dark green",  35,  2, 1, 1, IMMEDIATE, CLR_GREEN),
SPELL("confuse monster", "orange",      29,  3, 2, 1, IMMEDIATE, CLR_ORANGE),
SPELL("cure blindness",  "yellow",      29,  3, 2, 1, IMMEDIATE, CLR_YELLOW),
SPELL("slow monster",    "light green", 29,  3, 2, 1, IMMEDIATE, CLR_BRIGHT_GREEN),
SPELL("create monster",  "turquoise",   29,  3, 2, 1, NODIR,     CLR_BRIGHT_CYAN),
SPELL("endure heat",     "wide",        29,  3, 2, 1, NODIR,     HI_PAPER),
SPELL("endure cold",     "deep",        29,  3, 2, 1, NODIR,     HI_PAPER),
SPELL("insulate",        "long",        29,  3, 2, 1, NODIR,     HI_PAPER),
SPELL("cause fear",      "light blue",  23,  4, 3, 1, NODIR,     CLR_BRIGHT_BLUE),
SPELL("clairvoyance",    "dark blue",   23,  4, 3, 1, NODIR,     CLR_BLUE),
SPELL("cure sickness",   "indigo",      23,  4, 3, 1, NODIR,     CLR_BLUE),
SPELL("charm monster",   "magenta",     23,  4, 3, 1, IMMEDIATE, CLR_MAGENTA),
SPELL("haste self",      "purple",      23,  4, 3, 1, NODIR,     CLR_MAGENTA),
SPELL("detect unseen",   "violet",      23,  4, 3, 1, NODIR,     CLR_MAGENTA),
SPELL("identify",        "bronze",      23,  4, 3, 1, NODIR,     HI_COPPER),
SPELL("extra healing",   "plaid",       23,  4, 3, 1, IMMEDIATE, CLR_GREEN),
SPELL("levitation",      "tan",         19,  5, 4, 1, NODIR,     CLR_BROWN),
SPELL("restore ability", "light brown", 19,  5, 4, 1, NODIR,     CLR_BROWN),
SPELL("invisibility",    "dark brown",  19,  5, 4, 1, NODIR,     CLR_BROWN),
SPELL("detect treasure", "gray",        19,  5, 4, 1, NODIR,     CLR_GRAY),
SPELL("enlighten",       "faded",       19,  5, 4, 1, NODIR,     CLR_GRAY),
SPELL("remove curse",    "wrinkled",    14,  6, 5, 1, NODIR,     HI_PAPER),
SPELL("magic mapping",   "dusty",       14,  6, 5, 1, NODIR,     HI_PAPER),
SPELL("turn undead",     "copper",       7,  7, 6, 1, IMMEDIATE, HI_COPPER),
/* necromancer books... */
SPELL("summon undead",   "black",        2,  7, 5, 1, IMMEDIATE, CLR_BLACK),
SPELL("command undead",  "dark",         6,  7, 5, 1, IMMEDIATE, CLR_BLACK),
SPELL("polymorph",       "silver",       7,  7, 6, 1, IMMEDIATE, HI_SILVER),
SPELL("teleport away",   "gold",         7,  7, 6, 1, IMMEDIATE, HI_GOLD),
SPELL("create familiar", "glittering",   7,  7, 6, 1, NODIR,     CLR_WHITE),
SPELL("cancellation",    "shining",      5,  8, 7, 1, IMMEDIATE, CLR_WHITE),
SPELL("enchant weapon",  "dull",         5,  8, 7, 1, NODIR,     CLR_WHITE),
SPELL("enchant armor",   "thin",         5,  8, 7, 1, NODIR,     CLR_WHITE),
SPELL((char *)0,	 "thick",        0,  0, 0, 1, 0,         HI_PAPER),
/* blank spellbook must come last because it retains its description */
SPELL("blank paper",     "plain",        9,  0, 0, 0, 0,         HI_PAPER),
/* a special, one of a kind, spellbook */
OBJECT(OBJ("Book of the Dead", "papyrus"), BITS(0,0,1,0,1,0,1,1,0,0,0,0,PAPER), 0,
	SPBOOK_CLASS, 0, 0,20, 3500, 0, 0, 0, 7, 20, HI_PAPER),
#undef SPELL

/* wands ... */
#define WAND(name,typ,prob,cost,mgc,dir,metal,color) OBJECT( \
		OBJ(name,typ), BITS(0,0,1,0,mgc,1,0,0,0,0,dir,0,metal), 0, \
		WAND_CLASS, prob, 0, 7, cost, 0, 0, 0, 0, 30, color )
WAND("light",          "glass",    69, 100, 1, NODIR,     GLASS,    HI_GLASS),
WAND("enlightenment",   "crystal", 30, 150, 1, NODIR,     GLASS,    HI_GLASS),
WAND("secret door detection", "balsa",
				   30, 150, 1, NODIR,     WOOD,     HI_WOOD),
WAND("create monster", "maple",    35, 200, 1, NODIR,     WOOD,     HI_WOOD),
WAND("create horde",   "black",     5, 250, 1, NODIR,     PLASTIC,  CLR_BLACK),
WAND("wishing",        "pine",      1, 500, 1, NODIR,     WOOD,     HI_WOOD),
WAND("nothing",        "oak",      20, 100, 0, IMMEDIATE, WOOD,     HI_WOOD),
WAND("striking",       "ebony",    65, 150, 1, IMMEDIATE, WOOD,     HI_WOOD),
WAND("make invisible", "marble",   45, 150, 1, IMMEDIATE, MINERAL,  HI_MINERAL),
/* [Tom] old probabilities:
	 cancellation was 45
	 lightning, fire, and cold were 40
	 undead turning was 55
	 light was 95
	 secret door detection was 50
	 create monster was 45
*/
WAND("fear",           "rusty",    25, 200, 1, IMMEDIATE, IRON,     CLR_RED),
WAND("healing",        "bamboo",   60, 150, 1, IMMEDIATE, WOOD,     CLR_YELLOW),
WAND("extra healing",  "bronze",   30, 250, 1, IMMEDIATE, COPPER,   CLR_YELLOW),
WAND("slow monster",   "tin",      45, 150, 1, IMMEDIATE, METAL,    HI_METAL),
WAND("speed monster",  "brass",    45, 150, 1, IMMEDIATE, COPPER,   HI_COPPER),
WAND("undead turning", "copper",   45, 150, 1, IMMEDIATE, COPPER,   HI_COPPER),
WAND("polymorph",      "silver",   45, 200, 1, IMMEDIATE, SILVER,   HI_SILVER),
WAND("cancellation",   "platinum", 45, 200, 1, IMMEDIATE, PLATINUM, CLR_WHITE),
WAND("teleportation",  "iridium",  45, 200, 1, IMMEDIATE, METAL,    CLR_BRIGHT_CYAN),
WAND("opening",        "zinc",     25, 150, 1, IMMEDIATE, METAL,    HI_METAL),
WAND("locking",        "aluminum", 25, 150, 1, IMMEDIATE, METAL,    HI_METAL),
WAND("probing",        "uranium",  30, 150, 1, IMMEDIATE, METAL,    HI_METAL),
WAND("digging",        "iron",     50, 150, 1, RAY,       IRON,     HI_METAL),
WAND("magic missile",  "steel",    50, 150, 1, RAY,       IRON,     HI_METAL),
WAND("fire",           "hexagonal",25, 175, 1, RAY,       IRON,     HI_METAL),
WAND("cold",           "short",    30, 175, 1, RAY,       IRON,     HI_METAL),
WAND("sleep",          "runed",    50, 175, 1, RAY,       IRON,     HI_METAL),
WAND("death",          "long",      5, 500, 1, RAY,       IRON,     HI_METAL),
WAND("lightning",      "curved",   20, 175, 1, RAY,       IRON,     HI_METAL),
WAND("fireball",       "octagonal", 5, 275, 1, RAY,       IRON,     HI_METAL),
WAND((char *)0,        "forked",    0, 150, 1, 0,         WOOD,     HI_WOOD),
WAND((char *)0,        "spiked",    0, 150, 1, 0,         IRON,     HI_METAL),
WAND((char *)0,        "jeweled",   0, 150, 1, 0,         IRON,     HI_MINERAL),
#undef WAND

/* coins ... - so far, gold is all there is */
#define COIN(name,prob,metal) OBJECT( \
		OBJ(name,(char *)0), BITS(0,1,0,0,0,0,0,0,0,0,0,0,metal), 0, \
		GOLD_CLASS, prob, 0, 1, 0, 0, 0, 0, 0, 0, HI_GOLD )
	COIN("gold piece",      1000, GOLD),
#undef COIN

/* gems ... - includes stones and rocks but not boulders */
#define GEM(name,desc,prob,wt,gval,nutr,mohs,glass,color) OBJECT( \
	    OBJ(name,desc), \
	    BITS(0,1,0,0,0,0,0,0,0,HARDGEM(mohs),0,WEP_AMMO,glass), 0, \
	    GEM_CLASS, prob, 0, 1, gval, 3, 3, 0, WP_SLING, nutr, color )
#define ROCK(name,desc,kn,prob,wt,gval,sdam,ldam,mgc,nutr,mohs,glass,color) OBJECT( \
	    OBJ(name,desc), \
	    BITS(kn,1,0,0,mgc,0,0,0,0,HARDGEM(mohs),0,WEP_AMMO,glass), 0, \
	    GEM_CLASS, prob, 0, wt, gval, sdam, ldam, 0, WP_SLING, nutr, color )
GEM("dilithium crystal", "white",      2,  1, 4500, 15,  5, GEMSTONE, CLR_WHITE),
GEM("diamond", "white",                3,  1, 4000, 15, 10, GEMSTONE, CLR_WHITE),
GEM("ruby", "red",                     4,  1, 3500, 15,  9, GEMSTONE, CLR_RED),
GEM("jacinth", "orange",               3,  1, 3250, 15,  9, GEMSTONE, CLR_ORANGE),
GEM("sapphire", "blue",                4,  1, 3000, 15,  9, GEMSTONE, CLR_BLUE),
GEM("black opal", "black",             4,  1, 2500, 15,  8, GEMSTONE, CLR_BLACK),
GEM("emerald", "green",                5,  1, 2500, 15,  8, GEMSTONE, CLR_GREEN),
GEM("turquoise", "green",              6,  1, 2000, 15,  6, GEMSTONE, CLR_GREEN),
GEM("citrine", "yellow",               5,  1, 1500, 15,  6, GEMSTONE, CLR_YELLOW),
GEM("aquamarine", "green",             7,  1, 1500, 15,  8, GEMSTONE, CLR_GREEN),
GEM("amber", "yellowish brown",        9,  1, 1000, 15,  2, GEMSTONE, CLR_BROWN),
GEM("topaz", "yellowish brown",       10,  1,  900, 15,  8, GEMSTONE, CLR_BROWN),
GEM("jet", "black",                    7,  1,  850, 15,  7, GEMSTONE, CLR_BLACK),
GEM("opal", "white",                  13,  1,  800, 15,  6, GEMSTONE, CLR_WHITE),
GEM("chrysoberyl", "yellow",           9,  1,  700, 15,  5, GEMSTONE, CLR_YELLOW),
GEM("garnet", "red",                  13,  1,  700, 15,  7, GEMSTONE, CLR_RED),
GEM("amethyst", "violet",             14,  1,  600, 15,  7, GEMSTONE, CLR_MAGENTA),
GEM("jasper", "red",                  16,  1,  500, 15,  7, GEMSTONE, CLR_RED),
GEM("fluorite", "violet",             16,  1,  400, 15,  4, GEMSTONE, CLR_MAGENTA),
GEM("obsidian", "black",              10,  1,  200, 15,  6, GEMSTONE, CLR_BLACK),
GEM("agate", "orange",                13,  1,  200, 15,  6, GEMSTONE, CLR_ORANGE),
GEM("jade", "green",                  10,  1,  300, 15,  6, GEMSTONE, CLR_GREEN),
GEM("worthless piece of white glass", "white",   76, 1, 0, 6, 5, GLASS, CLR_WHITE),
GEM("worthless piece of blue glass", "blue",     76, 1, 0, 6, 5, GLASS, CLR_BLUE),
GEM("worthless piece of red glass", "red",       76, 1, 0, 6, 5, GLASS, CLR_RED),
GEM("worthless piece of yellowish brown glass", "yellowish brown",
						 77, 1, 0, 6, 5, GLASS, CLR_BROWN),
GEM("worthless piece of orange glass", "orange", 76, 1, 0, 6, 5, GLASS, CLR_ORANGE),
GEM("worthless piece of yellow glass", "yellow", 77, 1, 0, 6, 5, GLASS, CLR_YELLOW),
GEM("worthless piece of black glass",  "black",  76, 1, 0, 6, 5, GLASS, CLR_BLACK),
GEM("worthless piece of green glass", "green",   77, 1, 0, 6, 5, GLASS, CLR_GREEN),
GEM("worthless piece of violet glass", "violet", 76, 1, 0, 6, 5, GLASS, CLR_MAGENTA),

ROCK("luckstone", "gray",       0,  5,  10, 60, 3, 3, 1, 10, 7, MINERAL, CLR_GRAY),
ROCK("loadstone", "gray",       0,  5, 500,  1, 3, 3, 1, 10, 6, MINERAL, CLR_GRAY),
ROCK("stone of health", "gray", 0,  5,  10,100, 3, 3,1, 10, 7, MINERAL, CLR_GRAY),
ROCK("stone of rotting", "gray",0,  5,  10,  5, 3, 3,1, 10, 6, MINERAL, CLR_GRAY),
ROCK("flint", "gray",           0, 10,  10,  1, 6, 6, 0, 10, 7, MINERAL, CLR_GRAY),
ROCK("rock", (char *)0,		1,100,  10,  0, 3, 3, 0, 10, 7, MINERAL, CLR_GRAY),
#undef GEM
#undef ROCK

/* miscellaneous ... */
/* Note: boulders and rocks are not normally created at random; the
 * probabilities only come into effect when you try to polymorph them.
 * Boulders weigh more than MAX_CARR_CAP; statues use corpsenm to take
 * on a specific type and may act as containers (both affect weight).
 */
OBJECT(OBJ("boulder",(char *)0), BITS(1,0,0,0,0,0,0,0,1,0,0,0,MINERAL), 0,
		ROCK_CLASS,   100, 0, 6000,  0, 20, 20, 0, 0, 2000, HI_MINERAL),
OBJECT(OBJ("statue", (char *)0), BITS(1,0,0,1,0,0,0,0,0,0,0,0,MINERAL), 0,
		ROCK_CLASS,   900, 0, 2500,  0, 20, 20, 0, 0, 2500, CLR_WHITE),

OBJECT(OBJ("heavy iron ball", (char *)0), BITS(1,0,0,0,0,0,0,0,0,0,WHACK,0,IRON), 0,
		BALL_CLASS,  1000, 0,  480, 10,  0,  0, 0, 0,  200, HI_METAL),
						/* +d4 when "very heavy" */
OBJECT(OBJ("iron chain", (char *)0), BITS(1,0,0,0,0,0,0,0,0,0,WHACK,0,IRON), 0,
                CHAIN_CLASS, 1000, 0,  120,  0,  0,  0, 0, 0,  200, HI_METAL),
						/* +1 both l & s */
OBJECT(OBJ("blinding venom", "splash of venom"),
		BITS(0,1,0,0,0,0,0,1,0,0,0,0,LIQUID), 0,
		VENOM_CLASS,  500, 0,	 1,  0,  0,  0, 0, 0,	 0, HI_ORGANIC),
OBJECT(OBJ("acid venom", "splash of venom"),
		BITS(0,1,0,0,0,0,0,1,0,0,0,0,LIQUID), 0,
		VENOM_CLASS,  500, 0,	 1,  0,  6,  6, 0, 0,	 0, HI_ORGANIC),
		/* +d6 small or large */

/* fencepost, the deadly Array Terminator -- name [1st arg] *must* be NULL */
	OBJECT(OBJ((char *)0,(char *)0), BITS(0,0,0,0,0,0,0,0,0,0,0,0,0), 0,
		ILLOBJ_CLASS, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
}; /* objects[] */

#ifndef OBJECTS_PASS_2_

/* perform recursive compilation for second structure */
#  undef OBJ
#  undef OBJECT
#  define OBJECTS_PASS_2_
#include "objects.c"

void NDECL(objects_init);

/* dummy routine used to force linkage */
void
objects_init()
{
    return;
}

#endif	/* !OBJECTS_PASS_2_ */

/*objects.c*/
