/*	SCCS Id: @(#)artilist.h	3.2	96/05/19	*/
/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 1985. */
/* NetHack may be freely redistributed.  See license for details. */

#ifdef MAKEDEFS_C
/* in makedefs.c, all we care about is the list of names */

#define A(nam,typ,s1,s2,mt,atk,dfn,cry,inv,al,cl) nam

static const char *artifact_names[] = {
#else
/* in artifact.c, set up the actual artifact list structure */

#define A(nam,typ,s1,s2,mt,atk,dfn,cry,inv,al,cl) \
 { typ, nam, s1, s2, mt, atk, dfn, cry, inv, al, cl }

#define     NO_ATTK	{0,0,0,0}		/* no attack */
#define     NO_DFNS	{0,0,0,0}		/* no defense */
#define     NO_CARY	{0,0,0,0}		/* no carry effects */
#define     DFNS(c)	{0,c,0,0}
#define     CARY(c)	{0,c,0,0}
#define     PHYS(a,b)	{0,AD_PHYS,a,b}		/* physical */
#define     DRLI(a,b)	{0,AD_DRLI,a,b}		/* life drain */
#define     COLD(a,b)	{0,AD_COLD,a,b}
#define     FIRE(a,b)	{0,AD_FIRE,a,b}
#define     ELEC(a,b)	{0,AD_ELEC,a,b}		/* electrical shock */
#define     STUN(a,b)	{0,AD_STUN,a,b}		/* magical attack */

STATIC_OVL NEARDATA struct artifact artilist[] = {
#endif	/* MAKEDEFS_C */

/*  dummy element #0, so that all interesting indices are non-zero */
A("",				STRANGE_OBJECT,
	0, 0, 0, NO_ATTK, NO_DFNS, NO_CARY, 0, A_NONE, 0 ),

/* [Tom] rearranged by alignment, so when people ask... */
A("Excalibur",			LONG_SWORD,
	(SPFX_NOGEN|SPFX_RESTR|SPFX_SEEK|SPFX_DEFN|SPFX_INTEL|SPFX_SEARCH),0,0,
	PHYS(5,6),      DRLI(0,0),      NO_CARY,        0, A_LAWFUL,    'K' ),

A("Sunsword",                   LONG_SWORD,
       (SPFX_RESTR|SPFX_INTEL|SPFX_DFLAG2|SPFX_DEFN), 0, M2_UNDEAD,
       PHYS(5,12),     DRLI(0,0),     NO_CARY, LIGHT_AREA, A_LAWFUL,    'P' ),
                    /*DFNS(AD_BLND)*/
A("Snickersnee",                KATANA,
	SPFX_RESTR, 0, 0,
	PHYS(5,8),      NO_DFNS,        NO_CARY,        0, A_LAWFUL,    'S' ),

/* STEPHEN WHITE'S NEW CODE */
A("Holy Sword of Law",                 LONG_SWORD,
	(SPFX_RESTR|SPFX_DALIGN), 0, 0,
	PHYS(5,12),     NO_DFNS,        NO_CARY,        0, A_LAWFUL,     0  ),

A("Demonbane",                  LONG_SWORD,
	(SPFX_RESTR|SPFX_DFLAG2), 0, M2_DEMON,
	PHYS(5,30),     NO_DFNS,        NO_CARY,        0, A_LAWFUL,     0  ),

A("Werebane",                   SILVER_SABER,
	(SPFX_RESTR|SPFX_DFLAG2), 0, M2_WERE,
	PHYS(5,20),     NO_DFNS,        NO_CARY,        0, A_LAWFUL,     0  ),

A("Trollsbane",                 MORNING_STAR,
	(SPFX_RESTR|SPFX_DCLAS), 0, S_TROLL,
	PHYS(5,30),     NO_DFNS,        NO_CARY,        0, A_LAWFUL,     0  ),

A("Ogresmasher",                WAR_HAMMER,
	(SPFX_RESTR|SPFX_DCLAS), 0, S_OGRE,
	PHYS(5,30),     NO_DFNS,        NO_CARY,        0, A_LAWFUL,     'G'  ),

A("Orcrist",			ELVEN_BROADSWORD,
	SPFX_DFLAG2, 0, M2_ORC,
	PHYS(5,15),     NO_DFNS,        NO_CARY,        0, A_LAWFUL,     0  ),

A("Sting",			ELVEN_DAGGER,
	(SPFX_WARN|SPFX_DCLAS), 0, S_SPIDER,
	PHYS(5,7),      NO_DFNS,        NO_CARY,        0, A_LAWFUL,     0  ),

A("Holy Spear of Light",             SILVER_SPEAR,
       (SPFX_RESTR|SPFX_INTEL|SPFX_DFLAG2), 0, M2_UNDEAD,
       PHYS(5,10),      NO_DFNS,  NO_CARY,     LIGHT_AREA, A_LAWFUL,    'U' ),

A("Skullcrusher",               CLUB,
	SPFX_RESTR, 0, 0,
	PHYS(3,10),     NO_DFNS,        NO_CARY,        0, A_LAWFUL,     0  ),

A("Quick Blade",                ELVEN_SHORT_SWORD,
	SPFX_RESTR, 0, 0,
	PHYS(9,2),      NO_DFNS,        NO_CARY,        0, A_LAWFUL,     0  ),

A("Fire Dagger",                DAGGER,
	(SPFX_RESTR|SPFX_ATTK|SPFX_DEFN), 0, 0,
	FIRE(4,4),      FIRE(0,0),      NO_CARY,        0, A_LAWFUL,     0  ),

A("Grayswandir",                SILVER_SABER,
	(SPFX_RESTR|SPFX_HALRES), 0, 0,
	PHYS(7,7),      NO_DFNS,        NO_CARY,        0, A_LAWFUL,     0  ),

A("Firebiter",                 AXE,
	(SPFX_RESTR|SPFX_ATTK|SPFX_DEFN), 0, 0,
	FIRE(5,5),      FIRE(0,0),      NO_CARY,        0, A_LAWFUL,    0 ),

A("Sword of Balance",           SILVER_SHORT_SWORD,
	SPFX_DALIGN, 0, 0,
	PHYS(2,5),      NO_DFNS,        NO_CARY,        0, A_NEUTRAL,    0  ),

A("Giantkiller",                DWARVISH_MATTOCK,
	(SPFX_RESTR|SPFX_DFLAG2), 0, M2_GIANT,
	PHYS(2,30),      NO_DFNS,        NO_CARY,       0, A_NEUTRAL,    0  ),

A("Vorpal Blade",               LONG_SWORD,
	(SPFX_RESTR|SPFX_BEHEAD), 0, 0,
	PHYS(5,1),      NO_DFNS,        NO_CARY,        0, A_NEUTRAL,    0  ),

A("Luckblade",                  SHORT_SWORD,
	(SPFX_RESTR|SPFX_LUCK), 0, 0,
	PHYS(5,5),      NO_DFNS,        NO_CARY,        0, A_NEUTRAL,    0  ),
/*
 *	Mjollnir will return to the hand of the wielder when thrown
 *	if the wielder is wearing Gauntlets of Power.
 */
A("Mjollnir",                   HEAVY_HAMMER,           /* Mjo:llnir */
	(SPFX_RESTR|SPFX_ATTK),  0, 0,
	ELEC(5,16),     NO_DFNS,        NO_CARY,        0, A_NEUTRAL,   'V' ),

A("Magicbane",			ATHAME,
	(SPFX_RESTR|SPFX_ATTK|SPFX_DEFN), 0, 0,
	STUN(3,4),	DFNS(AD_MAGM),	NO_CARY,	0, A_NEUTRAL,	'W' ),

A("Frost Brand",		LONG_SWORD,
	(SPFX_RESTR|SPFX_ATTK|SPFX_DEFN), 0, 0,
	COLD(5,5),      COLD(0,0),      NO_CARY,        0, A_NEUTRAL,   'I' ),

A("Fire Brand",			LONG_SWORD,
	(SPFX_RESTR|SPFX_ATTK|SPFX_DEFN), 0, 0,
	FIRE(5,5),      FIRE(0,0),      NO_CARY,        0, A_NEUTRAL,   'F' ),

A("Dragonbane",			BROADSWORD,
	(SPFX_RESTR|SPFX_DCLAS), 0, S_DRAGON,
	PHYS(5,30),     NO_DFNS,        NO_CARY,        0, A_NEUTRAL,    0  ),

A("Disrupter",                  MACE,
	(SPFX_RESTR|SPFX_DFLAG2), 0, M2_UNDEAD,
	PHYS(5,30),     NO_DFNS,        NO_CARY,        0, A_NEUTRAL,   'P' ),

A("Mirrorbright",               SHIELD_OF_REFLECTION,
	(SPFX_RESTR|SPFX_HALRES), 0, 0,
	NO_ATTK,      NO_DFNS,        NO_CARY,        0, A_NEUTRAL,     0  ),

A("Deluder",               CLOAK_OF_DISPLACEMENT,
	(SPFX_RESTR|SPFX_STLTH|SPFX_LUCK), 0, 0,
	NO_ATTK,      NO_DFNS,        NO_CARY,        0, A_NEUTRAL,     0  ),

A("Whisperfeet",               SPEED_BOOTS,
	(SPFX_RESTR|SPFX_STLTH|SPFX_LUCK), 0, 0,
	NO_ATTK,      NO_DFNS,        NO_CARY,        0, A_NEUTRAL,     0  ),

/* [Tom] added artifacts of Nehwon...
 * they're a little more studly in Nethack then they were in the Leiber
 * books, but oh well... :)  Both Fafhrd and the Gray Mouser were typically
 * Neutral. */

/* Fafhrd's swords */

/* Fafhrd was from the snowy north, so we'll make his main weapon like
   a frost brand, but it doesn't protect you from fire, and does a little
   more damage, being a two-handed sword */
A("Graywand",                TWO_HANDED_SWORD,
	(SPFX_RESTR|SPFX_ATTK|SPFX_DEFN), 0, 0,
	COLD(3,6),      NO_DFNS,      NO_CARY,        0, A_NEUTRAL,    0 ),

/* His secondary weapon has a special attack in artifact.h, fitting its
   name. */
A("Heartseeker",             SHORT_SWORD,
	(SPFX_RESTR), 0, 0,
	PHYS(3,3),      NO_DFNS,      NO_CARY,        0, A_NEUTRAL,    0 ),

/* The Gray Mouser's weapons */

/* His rapier is supposedly really fast, so we'll make it hit a lot, and
   put some special stuff in artifact.h */
A("Scalpel",             RAPIER,
	(SPFX_RESTR), 0, 0,
	PHYS(8,2),      NO_DFNS,      NO_CARY,        0, A_NEUTRAL,    0 ),

/* Because of his adventures below Lankhmar, and the names ("Mouser" and
  "Cat's Claw", we'll make his dagger specially suited for rats. */
A("Cat's Claw",                      DAGGER,
	(SPFX_RESTR|SPFX_WARN|SPFX_DCLAS), 0, S_RODENT,
	PHYS(5,7),      NO_DFNS,        NO_CARY,        0, A_NEUTRAL,     0  ),

/* back to our program... */

A("Cleaver",                    BATTLE_AXE,
	SPFX_RESTR, 0, 0,
	PHYS(3,10),     NO_DFNS,        NO_CARY,        0, A_CHAOTIC,   'B' ),



A("Serpent's Tongue",            DAGGER,
	SPFX_RESTR, 0, 0,
        PHYS(2,0),      NO_DFNS,        NO_CARY,        0, A_CHAOTIC,   'N'  ),
	/* See artifact.c for special poison damage */

A("Grimtooth",                  ORCISH_DAGGER,
	SPFX_RESTR, 0, 0,
	PHYS(2,6),      NO_DFNS,        NO_CARY,        0, A_CHAOTIC,    0  ),

A("Doomblade",                  ORCISH_SHORT_SWORD,
	SPFX_RESTR, 0, 0,
	PHYS(0,10),     NO_DFNS,        NO_CARY,        0, A_CHAOTIC,    0  ),

A("Elfrist",                    ORCISH_SPEAR,
	SPFX_DFLAG2, 0, M2_ELF,
	PHYS(5,15),     NO_DFNS,        NO_CARY,        0, A_CHAOTIC,    0  ),
/*
 *	Stormbringer only has a 2 because it can drain a level,
 *	providing 8 more.
 */
A("Stormbringer",               RUNESWORD,
	(SPFX_RESTR|SPFX_ATTK|SPFX_DEFN|SPFX_INTEL|SPFX_DRLI), 0, 0,
	DRLI(5,2),      DRLI(0,0),      NO_CARY,        0, A_CHAOTIC,    0  ),

#ifdef BLACKMARKET
A("Thiefbane",                  LONG_SWORD,
	(SPFX_NOGEN|SPFX_RESTR|SPFX_ATTK|SPFX_DRLI|SPFX_BEHEAD|SPFX_DCLAS), 0, S_HUMAN,
	DRLI(5,1),      NO_DFNS,        NO_CARY,        0, A_CHAOTIC,    0  ),
#endif /* BLACKMARKET */

A("Icebiter",                AXE,
	(SPFX_RESTR|SPFX_ATTK|SPFX_DEFN), 0, 0,
	COLD(5,5),      COLD(0,0),      NO_CARY,        0, A_CHAOTIC,    0  ),

A("Deathsword",                   TWO_HANDED_SWORD,
	(SPFX_RESTR|SPFX_DFLAG2), 0, S_HUMAN,
	PHYS(5,14),      NO_DFNS,        NO_CARY,        0, A_CHAOTIC,    0 ),

A("Bat from Hell",                BASEBALL_BAT,
	(SPFX_RESTR), 0, 0,
	PHYS(3,20),      NO_DFNS,        NO_CARY,        0, A_CHAOTIC,     0  ),

/* Special Artifacts */

/* to do: Eye/Hand of Vecna  (add severed eye/hand in objects.c)*/
/* don't use hand/eye for anything else! */
A("The Eye of Vecna",       EYEBALL,
	(SPFX_RESTR), (SPFX_ESP|SPFX_HSPDAM), 0,
	NO_ATTK,        NO_DFNS,        CARY(AD_COLD),
	DEATH_GAZE,      A_CHAOTIC,       0 ),

A("The Hand of Vecna",       SEVERED_HAND,
	(SPFX_RESTR), (SPFX_REGEN|SPFX_HPHDAM), 0,
	NO_ATTK,        DRLI(0,0),      CARY(AD_COLD),
	SUMMON_UNDEAD,          A_CHAOTIC,       0 ),

/* STEPHEN WHITE'S NEW CODE */
A("Gauntlets of Defense",    GAUNTLETS_OF_DEXTERITY,
	SPFX_RESTR, SPFX_HPHDAM, 0,
	NO_ATTK,        NO_DFNS,        NO_CARY,    INVIS, A_NEUTRAL,   'M' ),

/*
 *	The artifacts for the quest dungeon, all self-willed.
 */

A("The Orb of Detection",	CRYSTAL_BALL,
	(SPFX_NOGEN|SPFX_RESTR|SPFX_INTEL), (SPFX_ESP|SPFX_HSPDAM), 0,
	NO_ATTK,	NO_DFNS,	CARY(AD_MAGM),
	INVIS,		A_LAWFUL,	'A' ),

A("The Heart of Ahriman",	LUCKSTONE,
	(SPFX_NOGEN|SPFX_RESTR|SPFX_INTEL), SPFX_STLTH, 0,
	NO_ATTK,	NO_DFNS,	NO_CARY,
	LEVITATION,     A_CHAOTIC,      'B' ),

A("The Sceptre of Might",       QUARTERSTAFF,
	(SPFX_NOGEN|SPFX_RESTR|SPFX_INTEL|SPFX_DALIGN), 0, 0,
	PHYS(3,5),      NO_DFNS,        CARY(AD_MAGM),
	CONFLICT,	A_LAWFUL,	'C' ),

/* STEPHEN WHITE'S NEW CODE */
A("The Medallion of Shifters",  AMULET_OF_ESP,
	(SPFX_NOGEN|SPFX_RESTR|SPFX_INTEL), 0, 0,
	NO_ATTK,        NO_DFNS,        NO_CARY,
	PROT_POLY,      A_NEUTRAL,      'D' ),

A("The Palantir of Westernesse",	CRYSTAL_BALL,
	(SPFX_NOGEN|SPFX_RESTR|SPFX_INTEL),
		(SPFX_ESP|SPFX_REGEN|SPFX_HSPDAM), 0,
	NO_ATTK,	NO_DFNS,	NO_CARY,
	TAMING,         A_LAWFUL,      'E' ),

/* STEPHEN WHITE'S NEW CODE */
A("The Candle of Eternal Flame",        MAGIC_CANDLE,
	(SPFX_NOGEN|SPFX_RESTR), (SPFX_WARN|SPFX_TCTRL), 0,
	NO_ATTK,        NO_DFNS,        CARY(AD_COLD),
	SUMMON_FIRE_ELEMENTAL,         A_NEUTRAL,      'F' ),

A("The Pick of Flandal Steelskin",       PICK_AXE,
	(SPFX_NOGEN|SPFX_RESTR|SPFX_INTEL|SPFX_DALIGN), (SPFX_WARN), 0,
	FIRE(5,20),     NO_DFNS,        CARY(AD_FIRE),
	CONFLICT,       A_NEUTRAL,       'G' ),

#ifdef JFIGHTER
A("The Silver Crystal",			DILITHIUM_CRYSTAL,
	(SPFX_NOGEN|SPFX_RESTR|SPFX_INTEL),
		(SPFX_ESP|SPFX_REGEN|SPFX_DBONUS|SPFX_PCTRL), 0,
	NO_ATTK,	NO_DFNS,	CARY(AD_MAGM),
	HEALING,	A_LAWFUL,	'J' ),
#endif


A("The Staff of Aesculapius",	QUARTERSTAFF,
	(SPFX_NOGEN|SPFX_RESTR|SPFX_ATTK|SPFX_INTEL|SPFX_DRLI|SPFX_REGEN), 0,0,
	DRLI(3,0),      NO_DFNS,        NO_CARY,
	HEALING,	A_NEUTRAL,	'H' ),

/* STEPHEN WHITE'S NEW CODE */
A("The Storm Whistle",          MAGIC_WHISTLE,
	(SPFX_NOGEN|SPFX_RESTR), (SPFX_WARN|SPFX_TCTRL), 0,
	NO_ATTK,        NO_DFNS,        CARY(AD_FIRE),
	SUMMON_WATER_ELEMENTAL,         A_LAWFUL,       'I' ),

A("The Magic Mirror of Merlin",	MIRROR,
	(SPFX_NOGEN|SPFX_RESTR|SPFX_INTEL|SPFX_SPEAK), SPFX_ESP, 0,
	NO_ATTK,	NO_DFNS,	CARY(AD_MAGM),
	0,		A_LAWFUL,	'K' ),

/* STEPHEN WHITE'S NEW CODE */
A("The Staff of Withering",       QUARTERSTAFF,
	(SPFX_NOGEN|SPFX_RESTR|SPFX_ATTK|SPFX_INTEL|SPFX_DRLI|SPFX_DALIGN), 0, 0,
	DRLI(3,5),      NO_DFNS,        CARY(AD_COLD),
	ENERGY_BOOST,   A_CHAOTIC,      'L' ),

/* STEPHEN WHITE'S NEW CODE */
A("The Mantle of Knowledge",      HELM_OF_BRILLIANCE,
	(SPFX_NOGEN|SPFX_RESTR|SPFX_INTEL), SPFX_ESP, 0,
	NO_ATTK,        NO_DFNS,        CARY(AD_MAGM),
	ENERGY_BOOST,   A_NEUTRAL,       'M' ),

A("The Great Dagger of Glaurgnaa",       GREAT_DAGGER,
	(SPFX_NOGEN|SPFX_RESTR|SPFX_ATTK|SPFX_INTEL|SPFX_DRLI|SPFX_DALIGN), 0, 0,
	DRLI(8,4),      NO_DFNS,        CARY(AD_MAGM),
	ENERGY_BOOST,   A_CHAOTIC,      'N' ),

A("The Mitre of Holiness",	HELM_OF_BRILLIANCE,
	(SPFX_NOGEN|SPFX_RESTR|SPFX_DCLAS|SPFX_INTEL), 0, M2_UNDEAD,
	NO_ATTK,	NO_DFNS,	CARY(AD_FIRE),
	ENERGY_BOOST,	A_LAWFUL,	'P' ),

A("The Master Key of Thievery", SKELETON_KEY,
	(SPFX_NOGEN|SPFX_RESTR|SPFX_INTEL|SPFX_SPEAK),
		(SPFX_WARN|SPFX_TCTRL|SPFX_HPHDAM), 0,
	NO_ATTK,	NO_DFNS,	NO_CARY,
	UNTRAP,		A_CHAOTIC,	'R' ),

A("The Tsurugi of Muramasa",	TSURUGI,
	(SPFX_NOGEN|SPFX_RESTR|SPFX_INTEL|SPFX_BEHEAD|SPFX_LUCK), 0, 0,
	NO_ATTK,        NO_DFNS,        NO_CARY,
	0,		A_LAWFUL,	'S' ),

#ifdef TOURIST
A("The Platinum Yendorian Express Card", CREDIT_CARD,
	(SPFX_NOGEN|SPFX_RESTR|SPFX_INTEL|SPFX_DEFN),
		(SPFX_ESP|SPFX_HSPDAM), 0,
	NO_ATTK,	NO_DFNS,	CARY(AD_MAGM),
	CHARGE_OBJ,	A_NEUTRAL,	'T' ),
#endif

A("The Stake of Van Helsing",        WOODEN_STAKE,
	(SPFX_NOGEN|SPFX_RESTR|SPFX_INTEL|SPFX_DCLAS), 0, S_VAMPIRE,
	PHYS(10,20),    NO_DFNS,        CARY(AD_MAGM),
	0,              A_LAWFUL,       'U' ),

A("The Orb of Fate",		CRYSTAL_BALL,
	(SPFX_NOGEN|SPFX_RESTR|SPFX_INTEL|SPFX_LUCK),
		(SPFX_WARN|SPFX_HSPDAM|SPFX_HPHDAM), 0,
	NO_ATTK,	NO_DFNS,	NO_CARY,
	LEV_TELE,	A_NEUTRAL,	'V' ),

A("The Eye of the Aethiopica",	AMULET_OF_ESP,
	(SPFX_NOGEN|SPFX_RESTR|SPFX_INTEL), (SPFX_EREGEN|SPFX_HSPDAM), 0,
	NO_ATTK,	NO_DFNS,	CARY(AD_MAGM),
	CREATE_PORTAL,	A_NEUTRAL,	'W' ),

A("The Tentacle Staff",  QUARTERSTAFF,
	(SPFX_NOGEN|SPFX_RESTR|SPFX_INTEL|SPFX_ATTK|SPFX_DRLI), (SPFX_EREGEN|SPFX_HSPDAM), 0,
	DRLI(8,5),        NO_DFNS,        CARY(AD_MAGM),
	LEV_TELE,         A_CHAOTIC,      'E' ),
/*
 *  terminator; otyp must be zero
 */
A(0, 0, 0, 0, 0, NO_ATTK, NO_DFNS, NO_CARY, 0, A_NONE, 0 )

};	/* artilist[] (or artifact_names[]) */

#undef	A

#ifndef MAKEDEFS_C
#undef	NO_ATTK
#undef	NO_DFNS
#undef	DFNS
#undef	PHYS
#undef	DRLI
#undef	COLD
#undef	FIRE
#undef	ELEC
#undef	STUN
#endif

/*artilist.h*/
