/*      SCCS Id: @(#)botl.c     3.2     95/07/15        */
/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 1985. */
/* NetHack may be freely redistributed.  See license for details. */

/*
**	Japanese version Copyright
**	(c) Issei Numata, Naoki Hamada, Shigehiro Miyashita, 1994-1996
**	changing point is marked `JP' (94/6/7)
**	JNetHack may be freely redistributed.  See license for details. 
*/

#include "hack.h"

#ifdef OVL0
extern const char *hu_stat[];	/* defined in eat.c */

const char *enc_stat[] = {
/*JP	"",
	"Burdened",
	"Stressed",
	"Strained",
	"Overtaxed",
	"Overloaded"*/
	"",
	"¤è¤í¤á¤­",
	"°µÇ÷",
	"¸Â³¦",
	"²Ù½Å",
	"Ä¶²á"
};

static void NDECL(bot1);
static void NDECL(bot2);
static void NDECL(bot3);
#endif /* OVL0 */

/* MAXCO must hold longest uncompressed status line, and must be larger
 * than COLNO
 *
 * longest practical second status line at the moment is
 *	Astral Plane $:12345 HP:700(700) Pw:111(111) AC:-127 Xp:30/123456789
 *	T:123456 Satiated Conf FoodPois Ill Blind Stun Hallu Overloaded
 * -- or somewhat over 130 characters
 */
#if COLNO <= 140
#define MAXCO 160
#else
#define MAXCO (COLNO+20)
#endif

#ifndef OVLB
STATIC_DCL int mrank_sz;
#else /* OVLB */
STATIC_OVL NEARDATA int mrank_sz = 0; /* loaded by max_rank_sz (from u_init) */
#endif /* OVLB */

struct rank_title {
#ifdef _DCC
    const char *m;		/* male title */
    const char *f;		/* female title, or 0 if same as male */
#else
    char const * const	m;	/* male title */
    char const * const	f;	/* female title, or 0 if same as male */
#endif
};
struct class_ranks {
    char		plclass, fill_;
    short		mplayer_class;
    struct rank_title	titles[9];
};

STATIC_DCL const struct rank_title *FDECL(rank_array, (CHAR_P));
STATIC_DCL const char *NDECL(rank);

#ifdef OVL1

/* 9 pairs of ranks for each class */

static const
struct class_ranks all_classes[] = {
  {					'A',0,	PM_ARCHEOLOGIST, {
/*JP*/
#if 0
	{"Digger",	0},
	{"Field Worker",0},
	{"Investigator",0},
	{"Exhumer",	0},
	{"Excavator",	0},
	{"Spelunker",	0},
	{"Speleologist",0},
	{"Collector",	0},
	{"Curator",	0}
#endif
	{"¹Û°÷",	0},
	{"Ï«Æ¯¼Ô",      0},
	{"Ä´ºº¼Ô",      0},
	{"È¯·¡¼Ô",	0},
	{"·¡ºï¼Ô",	0},
	{"Ãµ¸¡¼Ô",	0},
	{"Æ¶·¢³Ø¼Ô",    0},
	{"Èþ½Ñ¼ý½¸¼Ô",	0},
	{"´ÛÄ¹",	0}
  } },
  {					'B',0,	PM_BARBARIAN, {
/*JP*/
#if 0
	{"Plunderer",	"Plunderess"},
	{"Pillager",	0},
	{"Bandit",	0},
	{"Brigand",	0},
	{"Raider",	0},
	{"Reaver",	0},
	{"Slayer",	0},
	{"Chieftain",	"Chieftainess"},
	{"Conqueror",	"Conqueress"}
#endif
	{"ÅðÂ±",	"½÷ÅðÂ±"},
	{"Î¬Ã¥¼Ô",	0},
	{"°­´Á",	0},
	{"»³Â±",	0},
	{"¿¯Î¬¼Ô",	0},
	{"¶¯Åð",	0},
	{"»¦Ù¤¼Ô",	0},
	{"¼óÎÎ",	"½÷¼óÎÎ"},
	{"À¬Éþ¼Ô",	0}
  } },
  {					'C',0,	PM_CAVEMAN, {
/*JP*/
#if 0
	{"Troglodyte",	0},
	{"Aborigine",	0},
	{"Wanderer",	0},
	{"Vagrant",	0},
	{"Wayfarer",	0},
	{"Roamer",	0},
	{"Nomad",	0},
	{"Rover",	0},
	{"Pioneer",	0}
#endif
	{"·êµï¿Í",	0},
	{"¸¶½»Ì±",	0},
	{"ÊüÏ²¼Ô",	0},
	{"ÉâÏ²¼Ô",	0},
	{"Î¹¹Ô¼Ô",	0},
	{"ÊüÍ·¼Ô",	0},
	{"Í·ËÒÌ±",	0},
	{"Î®Ï²¼Ô",	0},
	{"Àè¶î¼Ô",	0}
  } },
  {                                     'D',0,  PM_DOPPELGANGER, {
/*JP*/
#if 0
	{"Copier",         0},
	{"Copier",         0},
	{"Copier",         0},
	{"Shifter",        0},
	{"Shifter",        0},
	{"Changer",        0},
	{"Changer",        0},
	{"Adept Changer",  0},
	{"Master Changer", 0}
#endif
	{"ÌÏÊï¼Ô",         0},
	{"ÌÏÊï¼Ô",         0},
	{"ÌÏÊï¼Ô",         0},
	{"Å¾ÊÑ¼Ô",        0},
	{"Å¾ÊÑ¼Ô",        0},
	{"ÊÑ²½»Õ",        0},
	{"ÊÑ²½»Õ",        0},
	{"Ì¾ÊÑ²½»Õ",  0},
	{"°ÎÂç¤ÊÊÑ²½»Õ", 0}
  } },
  {					'E',0,	PM_ELF, {
/*JP*/
#if 0
	{"Edhel",	"Elleth"},
	{"Edhel",	"Elleth"},	/* elf-maid */
	{"Ohtar",	"Ohtie"},	/* warrior */
	{"Kano",			/* commander (Q.) ['a] */
			"Kanie"}, /* educated guess, until further research- SAC */
	{"Arandur",		  /* king's servant, minister (Q.) - guess */
			"Aranduriel"},	/* educated guess */
	{"Hir",		"Hiril"},	/* lord, lady (S.) ['ir] */
	{"Aredhel",	"Arwen"},	/* noble elf, maiden (S.) */
	{"Ernil",	"Elentariel"},	/* prince (S.), elf-maiden (Q.) */
	{"Elentar",	"Elentari"}	/* Star-king, -queen (Q.) */
#endif
	{"¥¨¥ë¥Õ",	"¥¨¥ì¥¹"},
	{"¥¨¥ë¥Õ",	"¥¨¥ì¥¹"},	/* elf-maid */
	{"¥¨¥ë¥Õ¤ÎÀï»Î",	"¥¨¥ì¥¹¤Î½÷Àï»Î"},	/* warrior */
	{"¥¨¥ë¥Õ¤Î»Ø´ø¼Ô",			/* commander (Q.) ['a] */
			"¥¨¥ì¥¹¤Î»Ø´ø¼Ô"}, /* educated guess, until further research- SAC */
	{"²¦¤Î½¾¼Ô",		  /* king's servant, minister (Q.) - guess */
			"²¦¤Î»ø½÷"},	/* educated guess */
	{"¥¨¥ë¥Õ¤Î·¯¼ç",	"¥¨¥ì¥¹¤ÎÉ±"},	/* lord, lady (S.) ['ir] */
	{"¹âµ®¤Ê¥¨¥ë¥Õ",	"¹âµ®¤Ê¥¨¥ì¥¹"},	/* noble elf, maiden (S.) */
	{"¥¨¥ë¥Õ¤Î²¦»Ò",	"¥¨¥ì¥¹¤Î²¦½÷"},	/* prince (S.), elf-maiden (Q.) */
	{"À±¤Î²¦",	"À±¤ÎÈÞ"}	/* Star-king, -queen (Q.) */
  } },
  {                                     'F',0,  PM_FLAME_MAGE, {
/*JP*/
#if 0
	{"Igniter",       0},
	{"Igniter",       0},
	{"Igniter",       0},
	{"Torcher",       0},
	{"Torcher",       0},
	{"Torcher",       0},
	{"Incinerator",   0},
	{"Incinerator",   0},
	{"Flame-Master",  0}
#endif
	{"ÅÀ²Ð¼Ô",       0},
	{"ÅÀ²Ð¼Ô",       0},
	{"ÅÀ²Ð¼Ô",       0},
	{"È¯±ê¼Ô",       0},
	{"È¯±ê¼Ô",       0},
	{"È¯±ê¼Ô",       0},
	{"¾ÆµÑ¼Ô",   0},
	{"¾ÆµÑ¼Ô",   0},
	{"±ê¤Î»ÙÇÛ¼Ô",  0}
  } },
  {                                     'G',0,  PM_GNOME, {
/*JP*/
#if 0
	{"Gnome",               0},
	{"Gnome",               0},
	{"Gnome Warrior",       0},
	{"Gnome Knight",        0},
	{"Gnome Lord","Gnome Lady"},
	{"Gnome Lord","Gnome Lady"},
	{"Gnome Noble",         0},
	{"Gnome Noble",           0},
	{"Gnome King","Gnome Queen"}
#endif
	{"¥Î¡¼¥à",               0},
	{"¥Î¡¼¥à",               0},
	{"¥Î¡¼¥à¤ÎÀï»Î",       0},
	{"¥Î¡¼¥à¤Îµ³»Î",        0},
	{"¥Î¡¼¥à¤Î¸ô¼ß","¥Î¡¼¥à¤Î½÷¸ô¼ß"},
	{"¥Î¡¼¥à¤Î¸ô¼ß","¥Î¡¼¥à¤Î½÷¸ô¼ß"},
	{"¥Î¡¼¥à¤Îµ®Â²",         0},
	{"¥Î¡¼¥à¤Îµ®Â²",           0},
	{"¥Î¡¼¥à¤Î²¦","¥Î¡¼¥à¤Î½÷²¦"}
  } },
  {					'H',0,	PM_HEALER, {
/*JP*/
#if 0
	{"Rhizotomist",  0},
	{"Empiric",	0},
	{"Embalmer",	0},
	{"Dresser",	0},
	{"Medici ossium",	0},
	{"Herbalist",	0},
	{"Magister",	0},
	{"Physician",	0},
	{"Chirurgeon",	0}
#endif
	{"¸«½¬¤¤",      0},
	{"°å»Õ¸«½¬¤¤",	0},
	{"´Ç¸î»Õ",	"´Ç¸îÉØ"},
	{"°å»Õ½õ¼ê",	0},
	{"ÌôÊª¼çÇ¤",	0},
	{"°å»Õ¼çÇ¤",	"´Ç¸î¼çÇ¤"},
	{"´ÁÊý°å",	0},
	{"Æâ²Ê°å",	0},
	{"³°²Ê°å",	0}
  } },
  {                                     'I',0,  PM_ICE_MAGE, {
/*JP*/
#if 0
	{"Chiller",       0},
	{"Chiller",       0},
	{"Chiller",       0},
	{"Froster",       0},
	{"Froster",       0},
	{"Froster",       0},
	{"Freezer",       0},
	{"Freezer",       0},
	{"Ice-Master",    0}
#endif
	{"ÎäµÑ¼Ô",       0},
	{"ÎäµÑ¼Ô",       0},
	{"ÎäµÑ¼Ô",       0},
	{"·ëÁú¼Ô",       0},
	{"·ëÁú¼Ô",       0},
	{"·ëÁú¼Ô",       0},
	{"Åà·ë¼Ô",       0},
	{"Åà·ë¼Ô",       0},
	{"É¹¤Î»ÙÇÛ¼Ô",    0}
  } },
#ifdef JFIGHTER
  {					'J',0,	PM_JFIGHTER, {
	{"¥Þ¡¼¥­¥å¥ê¡¼",	0},
	{"¥Ó¡¼¥Ê¥¹",	0},
	{"¥Þ¡¼¥º",	0},
	{"¥¸¥å¥Ô¥¿¡¼",	0},
	{"¥µ¥¿¡¼¥ó",	0},
	{"¥¦¥é¥Ì¥¹",	0},
	{"¥Í¥×¥Á¥å¡¼¥ó",	0},
	{"¥×¥ë¡¼¥È",	0},
	{"¥à¡¼¥ó",	0}
    } },
#endif
  {					'K',0,	PM_KNIGHT, {
#if 0
	{"Gallant",	0},
	{"Esquire",	0},
	{"Bachelor",	0},
	{"Sergeant",	0},
	{"Knight",	0},
	{"Banneret",	0},
	{"Chevalier",	0},
	{"Seignieur",	0},
	{"Paladin",	0}
#endif
	{"¸«½¬¤¤",	0},
	{"ÊâÊ¼",	0},
	{"Àï»Î",	"½÷Àï»Î"},
	{"µ³Ê¼",	0},
	{"½ÅÀï»Î",	0},
	{"µ³»Î",	0},
	{"½Åµ³»Î",	0},
	{"·®µ³»Î",	0},
	{"À»µ³»Î",	0}
  } },
  {                                     'L',0,  PM_HUMAN_WEREWOLF, {
/*JP*/
#if 0
	{"Infected",        0},
	{"Summoner",        0},
	{"Stalker",         0},
	{"Render",          0},
	{"Fiend",           0},
	{"Nightmare",       0},
	{"Devourer",        0},
	{"Greater Fiend",   0},
	{"Lycanthrope",     0}
#endif
	{"´¶À÷¼Ô",          0},
	{"¾¤´­¼Ô",          0},
	{"ÄÉÀ×¼Ô",          0},
	{"¶¯Ã¥¼Ô",          0},
	{"°­Îî",            0},
	{"Ì´Ëâ",            0},
	{"²îµ´",            0},
	{"Ëâ²¦",            0},
	{"Ëâ¿À",            0}
  } },
  {                                     'M',0,  PM_MONK, {
/*JP*/
#if 0
	{"Initiate",           0},
	{"Beginner",           0},
	{"Abbot",              0},
	{"Master of Flowers",  0},
	{"Master of Wind",     0},
	{"Master of Dragons",  0},
	{"Master of Elements", 0},
	{"Master of Spirits",  0},
	{"Grand Master",       0}
#endif
	{"¿·Æþ¼Ô",             0},
	{"½é¿´¼Ô",             0},
	{"Âç½¤Æ»±¡Ä¹",         0},
	{"²Ö¤Î¾Î¹æ¼Ô",         0},
	{"É÷¤Î¾Î¹æ¼Ô",         0},
	{"Îµ¤Î¾Î¹æ¼Ô",         0},
	{"ÀºÎî¤Î¾Î¹æ¼Ô",       0},
	{"À»Îî¤Î¾Î¹æ¼Ô",       0},
	{"ºÇ¹â°Ì¤Î¾Î¹æ¼Ô",   0}
  } },
  {                                     'N',0,  PM_NECROMANCER, {
/*JP*/
#if 0
	{"Gravedigger",      0},
	{"Embalmer",    0},
	{"Mortician", 0},
	{"Zombie Lord",    0},
	{"Ghoul Master","Ghoul Mistress"},
	{"Necromancer", 0},
	{"Necromancer", 0},
	{"Undead Master",      0},
	{"Lich Lord",        0}
#endif
	{"Êè·¡¤ê",      0},
	{"»à¿ÍÀ½ºî¼Ô",    0},
	{"Áòµ·²°", 0},
	{"¥¾¥ó¥Ó¥í¡¼¥É",    0},
	{"¥°¡¼¥ë¥Þ¥¹¥¿¡¼","¥°¡¼¥ë¥ß¥¹¥È¥ì¥¹"},
	{"»à¿Í»È¤¤", 0},
	{"»à¿Í»È¤¤", 0},
	{"¥¢¥ó¥Ç¥Ã¥È¥Þ¥¹¥¿¡¼",      0},
	{"¥ê¥Ã¥Á¥í¡¼¥É",        0}
  } },
  {					'P',0,	PM_PRIEST, {
/*JP*/
#if 0
	{"Aspirant",	0},
	{"Acolyte",	0},
	{"Adept",	0},
	{"Priest",	"Priestess"},
	{"Curate",	0},
	{"Canon",	"Canoness"},
	{"Lama",	0},
	{"Patriarch",	"Matriarch"},
	{"High Priest", "High Priestess"}
#endif
	{"½¤Æ»¼Ô",	"½¤Æ»½÷"},
	{"»ø¼Ô",	0},
	{"»øº×",	0},
	{"ÁÎÎ·",	"ÆôÁÎ"},
	{"½õÇ¤»Êº×",	0},
	{"À»¼Ô",	"À»½÷"},
	{"»Ê¶µ",	0},
	{"Âç»Ê¶µ",	0},
	{"ÂçÁÎ¾å",      0}
  } },
  {					'R',0,	PM_ROGUE, {
/*JP*/
#if 0
	{"Footpad",	0},
	{"Cutpurse",	0},
	{"Rogue",	0},
	{"Pilferer",	0},
	{"Robber",	0},
	{"Burglar",	0},
	{"Filcher",	0},
	{"Magsman",	"Magswoman"},
	{"Thief",	0}
#endif
	{"ÄÉ¤¤¤Ï¤®",	0},
	{"¤Ò¤Ã¤¿¤¯¤ê",	0},
	{"¥¹¥ê",	0},
	{"¤´¤í¤Ä¤­",	0},
	{"¤³¤½¤É¤í",	0},
	{"¶õÁã",	0},
	{"Å¥ËÀ",	"½÷Å¥ËÀ"},
	{"¶¯Åð",	0},
	{"ÂçÅ¥ËÀ",	0}
  } },
  {					'S',0,	PM_SAMURAI, {
/*JP*/
#if 0
	{"Hatamoto",	0},  /* Banner Knight */
	{"Ronin",	0},  /* no allegiance */
	{"Samurai",     0},  /* knight */
	{"Joshu",	0},  /* heads a castle */
	{"Ryoshu",	0},  /* has a territory */
	{"Kokushu",	0},  /* heads a province */
	{"Daimyo",	0},  /* a samurai lord */
	{"Kuge",	0},  /* Noble of the Court */
	{"Shogun",	0}   /* supreme commander, warlord */
#endif
	{"´úËÜ",	0},  /* Banner Knight */
	{"Ï²¿Í",	0},  /* no allegiance */
	{"Ç¦¼Ô",	"¤¯¥Î°ì"},  /* secret society */
	{"¾ë¼ç",	0},  /* heads a castle */
	{"ÎÎ¼ç",	0},  /* has a territory */
	{"¹ñ¼ç",	0},  /* heads a province */
	{"ÂçÌ¾",	"¹ø¸µ"},  /* a samurai lord */
	{"¸ø²È",	0},  /* Noble of the Court */
	{"¾­·³",	"Âç±ü"}   /* supreme commander, warlord */
  } },
#ifdef TOURIST
  {					'T',0,	PM_TOURIST, {
/*JP*/
#if 0
	{"Rambler",	0},
	{"Sightseer",	0},
	{"Excursionist",0},
	{"Peregrinator","Peregrinatrix"},
	{"Traveler",	0},
	{"Journeyer",	0},
	{"Voyager",	0},
	{"Explorer",	0},
	{"Adventurer",	0}
#endif
	{"¥×¡¼ÂÀÏº",	"¥×¡¼»Ò"},
	{"´Ñ¸÷µÒ",	0},
	{"¼þÍ·Î¹¹Ô¼Ô",  0},
	{"Ê×Îò¼Ô",      0},
	{"Î¹¹Ô¼Ô",	0},
	{"Î¹¿Í",	0},
	{"¹Ò³¤¼Ô",	0},
	{"Ãµ¸¡²È",	0},
	{"ËÁ¸±¼Ô",	0}
  } },
#endif
  {                                     'U',0,  PM_UNDEAD_SLAYER, {
/*JP*/
#if 0
	{"Assistant",   0},
	{"Eliminator",    0},
	{"Eliminator",    0},
	{"Exterminator",        0},
	{"Exterminator",        0},
	{"Destroyer",   0},
	{"Vindicator",  0},
	{"Vindicator",  0},
	{"Undead Slayer",       0}
#endif
	{"½õ¼ê",         0},
	{"½üµî¼Ô",       0},
	{"½üµî¼Ô",       0},
	{"ËÐÌÇ¼Ô",       0},
	{"ËÐÌÇ¼Ô",       0},
	{"ÇË²õ¼Ô",       0},
	{"¾ÚÌÀ¼Ô",       0},
	{"¾ÚÌÀ¼Ô",       0},
	{"¥¢¥ó¥Ç¥Ã¥È¥¹¥ì¥¤¥ä¡¼",       0}
  } },
  {					'V',0,	PM_VALKYRIE, {
#if 0
	{"Stripling",	0},
	{"Skirmisher",	0},
	{"Fighter",	0},
	{"Man-at-arms", "Woman-at-arms"},
	{"Warrior",	0},
	{"Swashbuckler",0},
	{"Hero",	"Heroine"},
	{"Champion",	0},
	{"Lord",	"Lady"}
#endif
	{"¸«½¬¤¤",	0},
	{"ÊâÊ¼",	0},
	{"Àï»Î",	"½÷Àï»Î"},
	{"µ³Ê¼",      "½÷½Åµ³Ê¼"},
	{"ÀïÆ®Ê¼",	0},
	{"¹¶·âÊ¼",      0},
	{"±ÑÍº",	0},
	{"Æ®»Î",	"½÷Æ®»Î"},
	{"Çì¼ß",	"½÷Çì¼ß"}
  } },
  {					'W',0,	PM_WIZARD, {
/*JP*/
#if 0
	{"Evoker",	0},
	{"Conjurer",	0},
	{"Thaumaturge", 0},
	{"Magician",	0},
	{"Warlock", "Witch"},
	{"Enchanter",	"Enchantress"},
	{"Sorcerer",	"Sorceress"},
	{"Wizard",	0},
	{"Mage",	0}
#endif
	{"¼êÉÊ»Õ",	0},
	{"´ñ½Ñ»Õ",	0},
	{"Àê¤¤»Õ",	0},
	{"Îî´¶»Õ",	0},
	{"¾¤´­»Õ",	0},
	{"ÍÅ½Ñ»Õ",      0},
        {"Ëâ½Ñ»Õ",      0},
	{"ËâË¡»È¤¤",	"Ëâ½÷"},
	{"ÂçËâË¡»È¤¤",	0}
  } },
};

STATIC_OVL const struct rank_title *
rank_array(pc)
char pc;
{
	register int i;

	for (i = 0; i < SIZE(all_classes); i++)
	    if (all_classes[i].plclass == pc) return all_classes[i].titles;
	return 0;
}

/* convert experience level (1..30) to rank index (0..8) */
int xlev_to_rank(xlev)
int xlev;
{
	return (xlev <= 2) ? 0 : (xlev <= 30) ? ((xlev + 2) / 4) : 8;
}

#if 0	/* not currently needed */
/* convert rank index (0..8) to experience level (1..30) */
int rank_to_xlev(rank)
int rank;
{
	return (rank <= 0) ? 1 : (rank <= 8) ? ((rank * 4) - 2) : 30;
}
#endif

const char *
rank_of(lev, pc, female)
int lev;
char pc;
boolean female;
{
	register int idx = xlev_to_rank((int)lev);
	const struct rank_title *ranks = rank_array(pc);

	if (ranks)
	    return( female && ranks[idx].f ? ranks[idx].f : ranks[idx].m );
	return(pl_character);
}

STATIC_OVL const char *
rank()
{
	return(rank_of(u.ulevel, u.role, flags.female));
}

int
title_to_mon(str, rank_indx, title_length)
const char *str;
int *rank_indx, *title_length;
{
	register int i, j;
	register const struct rank_title *ttl;

	for (i = 0; i < SIZE(all_classes); i++)
	    for (j = 0; j < 9; j++) {
		ttl = &all_classes[i].titles[j];
		if (!strncmpi(ttl->m, str, strlen(ttl->m))) {
		    if (rank_indx) *rank_indx = j;
		    if (title_length) *title_length = strlen(ttl->m);
		    return all_classes[i].mplayer_class;
		} else if (ttl->f && !strncmpi(ttl->f, str, strlen(ttl->f))) {
		    if (rank_indx) *rank_indx = j;
		    if (title_length) *title_length = strlen(ttl->f);
		    return all_classes[i].plclass == 'C' ? PM_CAVEWOMAN :
			   all_classes[i].plclass == 'P' ? PM_PRIESTESS :
			   all_classes[i].mplayer_class;
		}
	    }
	return NON_PM;
}

#endif /* OVL1 */
#ifdef OVLB

void
max_rank_sz()
{
	register int i, r, maxr = 0;
	const struct rank_title *ranks = rank_array(u.role);

	if (ranks) {
	    for (i = 0; i < 9; i++) {
		if ((r = strlen(ranks[i].m)) > maxr) maxr = r;
		if (ranks[i].f)
		    if ((r = strlen(ranks[i].f)) > maxr) maxr = r;
	    }
	    mrank_sz = maxr;
	} else
	    mrank_sz = strlen(pl_character);
}

#endif /* OVLB */
#ifdef OVL0

#ifdef SCORE_ON_BOTL
long
botl_score()
{
    int deepest = deepest_lev_reached(FALSE);
    long ugold = u.ugold + hidden_gold();

    if ((ugold -= u.ugold0) < 0L) ugold = 0L;
    return ugold + u.urexp + (long)(50 * (deepest - 1))
			  + (long)(deepest > 30 ? 10000 :
				   deepest > 20 ? 1000*(deepest - 20) : 0);
}
#endif

static void
bot1()
{
	char newbot1[MAXCO];
	register char *nb;
	register int i,j;

	Strcpy(newbot1, plname);
	if('a' <= newbot1[0] && newbot1[0] <= 'z') newbot1[0] += 'A'-'a';
	if( is_kanji1(newbot1, 9) )	/* JP */
		newbot1[9] = '_';
	newbot1[10] = 0;
/*JP	Sprintf(nb = eos(newbot1)," the ");*/
	Sprintf(nb = eos(newbot1)," ");

	if (Upolyd) {
		char mbot[BUFSZ];
		int k = 0;

/*JP		Strcpy(mbot, mons[u.umonnum].mname);*/
		Strcpy(mbot, jtrns_mon(mons[u.umonnum].mname, flags.female));
		while(mbot[k] != 0) {
		    if ((k == 0 || (k > 0 && mbot[k-1] == ' ')) &&
					'a' <= mbot[k] && mbot[k] <= 'z')
			mbot[k] += 'A' - 'a';
		    k++;
		}
		Sprintf(nb = eos(nb), mbot);
	} else
		Sprintf(nb = eos(nb), rank());

	Sprintf(nb = eos(nb),"  ");
	i = mrank_sz + 15;
	j = (nb + 2) - newbot1; /* aka strlen(newbot1) but less computation */
	if((i - j) > 0)
		Sprintf(nb = eos(nb),"%*s", i-j, " ");	/* pad with spaces */
	if (ACURR(A_STR) > 18) {
		if (ACURR(A_STR) > 118)
/*JP		    Sprintf(nb = eos(nb),"St:%2d ",ACURR(A_STR)-100);*/
		    Sprintf(nb = eos(nb),"¶¯:%2d ",ACURR(A_STR)-100);
		else if (ACURR(A_STR) < 118)
/*JP		    Sprintf(nb = eos(nb), "St:18/%02d ",ACURR(A_STR)-18);*/
		    Sprintf(nb = eos(nb), "¶¯:18/%02d ",ACURR(A_STR)-18);
		else
/*JP		    Sprintf(nb = eos(nb),"St:18/** ");*/
		    Sprintf(nb = eos(nb),"¶¯:18/** ");
	} else
/*JP		Sprintf(nb = eos(nb), "St:%-1d ",ACURR(A_STR));*/
		Sprintf(nb = eos(nb), "¶¯:%-1d ",ACURR(A_STR));
	Sprintf(nb = eos(nb),
/*JP		"Dx:%-1d Co:%-1d In:%-1d Wi:%-1d Ch:%-1d",*/
		"Áá:%-1d ÂÑ:%-1d ÃÎ:%-1d ¸­:%-1d Ì¥:%-1d",
		ACURR(A_DEX), ACURR(A_CON), ACURR(A_INT), ACURR(A_WIS), ACURR(A_CHA));
/*JP	Sprintf(nb = eos(nb), (u.ualign.type == A_CHAOTIC) ? "  Chaotic" :
			(u.ualign.type == A_NEUTRAL) ? "  Neutral" : "  Lawful");*/
	Sprintf(nb = eos(nb), (u.ualign.type == A_CHAOTIC) ? "  º®ÆÙ" :
			(u.ualign.type == A_NEUTRAL) ? "  ÃæÎ©" : "  Ãá½ø");
#ifdef SCORE_ON_BOTL
	if (flags.showscore)
/*JP	    Sprintf(nb = eos(nb), " S:%ld", botl_score());*/
	    Sprintf(nb = eos(nb), " ÅÀ:%ld", botl_score());
#endif
	curs(WIN_STATUS, 1, 0);
	putstr(WIN_STATUS, 0, newbot1);
}

static void
bot2()
{
	char  newbot2[MAXCO];
	register char *nb;
	register int con = ACURR(A_CON), wis = ACURR(A_WIS),
		     intl = ACURR(A_INT);
	int hp, hpmax, cbon, mbon, temp;

/* STEPHEN WHITE'S NEW CODE */

	     if (u.ulevel > 7) temp = 7 + ((u.ulevel - 7) / 2);
	else if (u.ulevel > 14) temp = 10 + ((u.ulevel - 14) / 3);
	else if (u.ulevel > 21) temp = 12 + ((u.ulevel - 21) / 4);
	else if (u.ulevel > 24) temp = 13;
	else temp = u.ulevel;

	if (con == 3) cbon = -3 * temp;
	else if (con == 4) cbon = -2 * temp;
	else if (con == 5) cbon = -2 * temp;
	else if (con == 6) cbon = -1 * temp;
	else if (con == 7) cbon = -1 * temp;
	else if (con < 15) cbon = 0 * temp;  
	else if (con == 15) cbon = 1 * temp;
	else if (con == 16) cbon = 2 * temp;
	else if (con == 17) cbon = 3 * temp;
	else if (con == 18) cbon = 4 * temp;
	else if (con == 19) cbon = 5 * temp;
	else if (con == 20) cbon = 5 * temp;
	else if (con == 21) cbon = 6 * temp;
	else if (con == 22) cbon = 6 * temp; 
	else if (con == 23) cbon = 7 * temp; 
	else if (con == 24) cbon = 7 * temp; 
	else cbon = 8 * temp;
	
	if (wis < 4) mbon = -3 * temp;
	else if (wis < 6) mbon = -2 * temp;
	else if (wis < 8) mbon = -1 * temp;
	else if (wis < 14) mbon = 0 * temp;
	else if (wis == 14) mbon = 1 * temp;
	else if (wis == 15) mbon = 2 * temp;
	else if (wis == 16) mbon = 2 * temp;
	else if (wis == 17) mbon = 3 * temp;
	else if (wis == 18) mbon = 3 * temp;
	else if (wis == 19) mbon = 4 * temp;
	else if (wis == 20) mbon = 4 * temp;
	else if (wis == 21) mbon = 5 * temp;
	else if (wis == 22) mbon = 5 * temp; 
	else if (wis == 23) mbon = 6 * temp; 
	else if (wis == 24) mbon = 6 * temp; 
	else mbon = 7 * temp;
	
	if (intl < 4) mbon += -2 * temp;
	else if (intl < 6) mbon += -1 * temp;
	else if (intl < 17) mbon += 0 * temp;
	else if (intl == 17) mbon += 1 * temp;
	else if (intl == 18) mbon += 1 * temp; 
	else if (intl == 19) mbon += 2 * temp;
	else if (intl == 20) mbon += 2 * temp;
	else if (intl == 21) mbon += 3 * temp;
	else if (intl == 22) mbon += 3 * temp;
	else if (intl == 23) mbon += 4 * temp; 
	else if (intl == 24) mbon += 4 * temp;
	else mbon += 5 * temp;
	
	u.uhpmax = u.uhpbase + cbon;
	if (u.uhpmax < 1) u.uhpmax = 1;
	if (u.uhp > u.uhpmax) u.uhp = u.uhpmax;
	
	u.uenmax = u.uenbase + mbon;
	if (u.uenmax < 0) u.uenmax = 0;
	if (u.uen > u.uenmax) u.uen = u.uenmax;


	hp = Upolyd ? u.mh : u.uhp;
	hpmax = Upolyd ? u.mhmax : u.uhpmax;

	if(hp < 0) hp = 0;
	  
/*JP	Sprintf(newbot2,"AC:%-2d ",u.uac);*/
	Sprintf(newbot2,"³»:%-2d ",u.uac);
  
	Sprintf(nb = eos(newbot2),
/*JP		"HP:%d(%d)  Pw:%d(%d) ",*/
		"ÂÎ:%d(%d)  Ëâ:%d(%d) ",
		hp, hpmax, u.uen, u.uenmax);

	if (Upolyd)
		Sprintf(nb = eos(nb), " HD:%d", mons[u.umonnum].mlevel);
/*JP	Sprintf(nb = eos(nb), " Level:%u  Exp:%-1ld(%-1ld)", u.ulevel,u.uexp,newuexp(u.ulevel));*/
	Sprintf(nb = eos(nb), " ¥ì¥Ù¥ë:%u  ·Ð¸³:%-1ld(%-1ld)", u.ulevel,u.uexp,newuexp(u.ulevel));
/*JP	Sprintf(nb = eos(nb), "  Wt:%ld/%ld",inv_weight()+weight_cap(),weight_cap());*/
	if(strcmp(hu_stat[u.uhs], "        ")) {
		Sprintf(nb = eos(nb), " ");
		Strcat(newbot2, hu_stat[u.uhs]);
	}
	curs(WIN_STATUS, 1, 1);
	putstr(WIN_STATUS, 0, newbot2);
  }
  
static void
bot3()
{
	char  newbot3[MAXCO];
	register char *nb;
	int cap = near_capacity();
  
/* TODO:        Add in dungeon name */
	if(Is_knox(&u.uz)) {
/*JP		Sprintf(newbot3, "%s ", dungeons[u.uz.dnum].dname);*/
		Sprintf(newbot3, "%s ", jtrns_obj('d', dungeons[u.uz.dnum].dname));
	} else if(In_quest(&u.uz)) {
/*JP		Sprintf(newbot3, "Home %d ", dunlev(&u.uz));*/
		Sprintf(newbot3, "¸Î¶¿ %d ", dunlev(&u.uz));
	} else if(In_endgame(&u.uz)) {
		Sprintf(newbot3,
			Is_astralevel(&u.uz) ? "ÀºÎî³¦ " : "ºÇ½ª»îÎý ");
/*	  if(strcmp(hu_stat[u.uhs], "        ")) {*/
/*JP	  if(In_endgame(&u.uz))                
		Sprintf(newbot3,
		Is_astralevel(&u.uz) ? "Astral Plane " : "End Game ");*/
	} else	{
/*JP		Sprintf(newbot3, "Dlvl:%-2d ", depth(&u.uz));*/
		Sprintf(newbot3, "ÃÏ²¼:%-2d ", depth(&u.uz));
	}
#ifdef SCORE_ON_BOTL
	Sprintf(nb = eos(newbot3),
/*JP		"%c:%-2ld  Score:%d", oc_syms[GOLD_CLASS],
		u.ugold, botl_score());*/
		"%c:%-2ld", oc_syms[GOLD_CLASS],
		u.ugold);
	Sprintf(nb = eos(nb), "  Áí½ÅÎÌ/Ç½ÎÏ:%ld/%ld",
		inv_weight()+weight_cap(),weight_cap());
/*JP	Sprintf(nb = eos(nb), "  Turn:%ld", moves);*/
	Sprintf(nb = eos(nb), "  ¥¿¡¼¥ó¿ô:%ld", moves);
#else
/*JP	Sprintf(nb = eos(newbot3), "  Turn:%ld", moves);*/
	Sprintf(nb = eos(newbot3), "  ¥¿¡¼¥ó¿ô:%ld", moves);
#endif
	
	Sprintf(nb = eos(nb), " ");
/*		Strcat(newbot3, hu_stat[u.uhs]);
	}*/
/*JP	if (Levitation)    Sprintf(nb = eos(nb), " Levitate");        
	if(Confusion)	   Sprintf(nb = eos(nb), " Conf");*/
	if (Levitation)    Sprintf(nb = eos(nb), " ÉâÍ·");        
	if(Confusion)	   Sprintf(nb = eos(nb), " º®Íð");
	if(Sick) {
		if (u.usick_type & SICK_VOMITABLE)
/*JP			   Sprintf(nb = eos(nb), " FoodPois");*/
			   Sprintf(nb = eos(nb), " ¿©ÆÇ");
		if (u.usick_type & SICK_NONVOMITABLE)
/*JP			   Sprintf(nb = eos(nb), " Ill");*/
			   Sprintf(nb = eos(nb), " ÉÂµ¤");
	}
/*JP	if(Blind)	   Sprintf(nb = eos(nb), " Blind");
	if(Stunned)	   Sprintf(nb = eos(nb), " Stun");
	if(Hallucination)  Sprintf(nb = eos(nb), " Hallu");*/
	if(Blind)	   Sprintf(nb = eos(nb), " ÌÕÌÜ");
	if(Stunned)	   Sprintf(nb = eos(nb), " âÁÚô");
	if(Hallucination)  Sprintf(nb = eos(nb), " ¸¸³Ð");
	if(cap > UNENCUMBERED)
		Sprintf(nb = eos(nb), " %s", enc_stat[cap]);
	curs(WIN_STATUS, 1, 2);
	putstr(WIN_STATUS, 0, newbot3);
}

void
bot()
{
	bot1();
	bot2();
	bot3();
	flags.botl = flags.botlx = 0;
}

#endif /* OVL0 */

/*botl.c*/
