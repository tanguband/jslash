/*      SCCS Id: @(#)questpgr.c 3.2     95/08/19        */
/*	Copyright 1991, M. Stephenson		  */
/* NetHack may be freely redistributed.  See license for details. */

/*
**    Japanese version Copyright
**    (c) Issei Numata, Naoki Hamada, Shigehiro Miyashita, 1994-1996
**    changing point is marked `JP' (94/6/7)
**    JNetHack may be freely redistributed.  See license for details.
*/

#include "hack.h"
#include "dlb.h"

/*  quest-specific pager routines. */

#ifdef _MSC_VER
#include "../japanese/emalloc.h"
#define alloc(s) emalloc(s)
#endif

#include "qtext.h"

#define QTEXT_FILE	"quest.dat"

/* #define DEBUG */
/* uncomment for debugging */


static void FDECL(Fread, (genericptr_t,int,int,dlb *));
static struct qtmsg * FDECL(construct_qtlist, (long));
static unsigned NDECL(class_index);
static const char * NDECL(intermed);
static const char * NDECL(neminame);
static const char * NDECL(guardname);
static const char * NDECL(homebase);
static struct qtmsg * FDECL(msg_in, (struct qtmsg *,int));
static void FDECL(convert_arg, (CHAR_P));
static void NDECL(convert_line);
static void FDECL(deliver_by_pline, (struct qtmsg *));
static void FDECL(deliver_by_window, (struct qtmsg *,int));

static char	in_line[80], cvt_buf[64], out_line[128];
static struct	qtlists	qt_list;
static dlb	*msg_file;
/* used by ldrname() and neminame(), then copied into cvt_buf */
static char	nambuf[sizeof cvt_buf];

#ifdef DEBUG
static void NDECL(dump_qtlist);

static void
dump_qtlist()	/* dump the character msg list to check appearance */
{
	struct	qtmsg	*msg;
	long	size;

	for (msg = qt_list.chclass; msg->msgnum > 0; msg++) {
		pline("msgnum %d: delivery %c",
			msg->msgnum, msg->delivery);
		more();
		(void) dlb_fseek(msg_file, msg->offset, SEEK_SET);
		deliver_by_window(msg, NHW_TEXT);
	}
}
#endif /* DEBUG */

static void
Fread(ptr, size, nitems, stream)
genericptr_t	ptr;
int	size, nitems;
dlb	*stream;
{
	int cnt;

	if ((cnt = dlb_fread(ptr, size, nitems, stream)) != nitems) {

	    panic("PREMATURE EOF ON QUEST TEXT FILE!\nExpected %d bytes - got %d\n",
		    (size * nitems), (size * cnt));
	}
}

static struct qtmsg *
construct_qtlist(hdr_offset)
long	hdr_offset;
{
	struct qtmsg *msg_list;
	int	n_msgs;

	(void) dlb_fseek(msg_file, hdr_offset, SEEK_SET);
	Fread(&n_msgs, sizeof(int), 1, msg_file);
	msg_list = (struct qtmsg *)
		alloc((unsigned)(n_msgs+1)*sizeof(struct qtmsg));

	/*
	 * Load up the list.
	 */
	Fread((genericptr_t)msg_list, n_msgs*sizeof(struct qtmsg), 1, msg_file);

	msg_list[n_msgs].msgnum = -1;
	return(msg_list);
}

void
load_qtlist()
{

	int	n_classes, i;
	char	qt_classes[N_HDR];
	long	qt_offsets[N_HDR];

	msg_file = dlb_fopen(QTEXT_FILE, RDBMODE);
	if (!msg_file)
	    panic("\rCANNOT OPEN QUEST TEXT FILE %s.", QTEXT_FILE);

	/*
	 * Read in the number of classes, then the ID's & offsets for
	 * each header.
	 */

	Fread(&n_classes, sizeof(int), 1, msg_file);
	Fread(qt_classes, sizeof(char), n_classes, msg_file);
	Fread(qt_offsets, sizeof(long), n_classes, msg_file);

	/*
	 * Now construct the message lists for quick reference later
	 * on when we are actually paging the messages out.
	 */

	qt_list.common = qt_list.chclass = (struct qtmsg *)0;

	for (i = 0; i < n_classes; i++) {
	    char search_ch;
	    search_ch = pl_character[0];
	    if (pl_character[0] == 'E' && u.uelf_drow) search_ch = 'Q';
	    if (qt_classes[i] == COMMON_ID)
		qt_list.common = construct_qtlist(qt_offsets[i]);
	    else if (qt_classes[i] == search_ch)
		qt_list.chclass = construct_qtlist(qt_offsets[i]);
	}

	if (!qt_list.common || !qt_list.chclass)
	    impossible("load_qtlist: cannot load quest text.");
#ifdef DEBUG
	dump_qtlist();
#endif
	return;	/* no ***DON'T*** close the msg_file */
}

/* called at program exit */
void
unload_qtlist()
{
	if (msg_file)
	    (void) dlb_fclose(msg_file),  msg_file = 0;
	if (qt_list.common)
	    free((genericptr_t) qt_list.common),  qt_list.common = 0;
	if (qt_list.chclass)
	    free((genericptr_t) qt_list.chclass),  qt_list.chclass = 0;
	return;
}

static struct qt_matrix {

	const char *intermed;	/* intermediate goal text */
	const char *homebase;	/* leader's "location" */

	short	ldrnum,		/* mons[] indicies */
		neminum,
		guardnum;

	short	mtyp1, mtyp2;	/* monster types for enemies 0 == random */
	char	msym1, msym2;	/* monster classes for enemies */

	short	artinum;	/* index of quest artifact */

} qt_matrix[] = {
/*JP*/
#if 0
/* A */ { "the tomb of the Toltec Kings",
	  "the College of Archeology",
	  PM_LORD_CARNARVON, PM_MINION_OF_HUHETOTL, PM_STUDENT,
	  0, PM_HUMAN_MUMMY, S_SNAKE, S_MUMMY,
	  ART_ORB_OF_DETECTION },
#endif
/* A */ { "¥È¥ë¥Æ¥Ã¥¯²¦¤ÎÊè",
	  "¹Í¸Å³ØÂç³Ø",
	  PM_LORD_CARNARVON, PM_MINION_OF_HUHETOTL, PM_STUDENT,
	  0, PM_HUMAN_MUMMY, S_SNAKE, S_MUMMY,
	  ART_ORB_OF_DETECTION },

/*JP*/
#if 0
/* B */ { "the Duali Oasis",
	  "the Camp of the Duali Tribe",
	  PM_PELIAS, PM_THOTH_AMON, PM_CHIEFTAIN,
	  PM_OGRE, PM_TROLL, S_OGRE, S_TROLL,
	  ART_HEART_OF_AHRIMAN },
#endif
/* B */ { "¥Â¥å¥¢¥ê¤Î¥ª¥¢¥·¥¹",
	  "¥Â¥å¥¢¥êÂ²¤Î¥­¥ã¥ó¥×",
	  PM_PELIAS, PM_THOTH_AMON, PM_CHIEFTAIN,
	  PM_OGRE, PM_TROLL, S_OGRE, S_TROLL,
	  ART_HEART_OF_AHRIMAN },

/*JP*/
#if 0
/* C */ { "the Dragon's Lair",
	  "the Caves of the Ancestors",
	  PM_SHAMAN_KARNOV, PM_CHROMATIC_DRAGON, PM_NEANDERTHAL,
	  PM_BUGBEAR, PM_HILL_GIANT, S_HUMANOID, S_GIANT,
	  ART_SCEPTRE_OF_MIGHT },
#endif
/* C */ { "Îµ¤Î±£¤ì²È",
	  "ÂÀ¸Å¤ÎÆ¶·¢",
	  PM_SHAMAN_KARNOV, PM_CHROMATIC_DRAGON, PM_NEANDERTHAL,
	  PM_BUGBEAR, PM_HILL_GIANT, S_HUMANOID, S_GIANT,
	  ART_SCEPTRE_OF_MIGHT },

/*JP*/
#if 0
/* D */ { "the Transmuter's Cave",
	  "the Circle of Change",
	  PM_MASTER_SHIFTER, PM_TRANSMUTER, PM_SHIFTER,
	  PM_CHAMELEON, PM_GUARDIAN_NAGA, S_LIZARD, S_NAGA,
	  ART_MEDALLION_OF_SHIFTERS },
#endif
/* D */ { "ÀÅ¼ä¤ÎÆ¶·¢",
	  "ÊÑ²½¤Î²óÏ­",
	  PM_MASTER_SHIFTER, PM_TRANSMUTER, PM_SHIFTER,
	  PM_CHAMELEON, PM_GUARDIAN_NAGA, S_LIZARD, S_NAGA,
	  ART_MEDALLION_OF_SHIFTERS },

#if 0
/* E */ { "the Goblins' Cave",
	  "the great Circle of Earendil",
	  PM_EARENDIL, PM_GOBLIN_KING, PM_HIGH_ELF,
	  PM_URUK_HAI, PM_OGRE, S_ORC, S_OGRE,
	  ART_PALANTIR_OF_WESTERNESSE },
#endif
/* E */ { "¥´¥Ö¥ê¥ó¤ÎÆ¶·¢",
	  "¥¨¥¢¥ì¥ó¥Ç¥£¥ë¤Î°ÎÂç¤Ê¤ëÃÏ",
	  PM_EARENDIL, PM_GOBLIN_KING, PM_HIGH_ELF,
	  PM_URUK_HAI, PM_OGRE, S_ORC, S_OGRE,
	  ART_PALANTIR_OF_WESTERNESSE },

/*JP*/
#if 0
/* E */ { "the Goblins' Cave",
	  "the great Circle of Elwing",
	  PM_ELWING, PM_GOBLIN_KING, PM_HIGH_ELF,
	  PM_URUK_HAI, PM_OGRE, S_ORC, S_OGRE,
	  ART_PALANTIR_OF_WESTERNESSE },
#endif
/* E */ { "¥´¥Ö¥ê¥ó¤ÎÆ¶·¢",
	  "¥¨¥ë¥¦¥£¥ó¤Î°ÎÂç¤Ê¤ëÃÏ",
	  PM_ELWING, PM_GOBLIN_KING, PM_HIGH_ELF,
	  PM_URUK_HAI, PM_OGRE, S_ORC, S_OGRE,
	  ART_PALANTIR_OF_WESTERNESSE },

/*JP*/
#if 0
/* F */ { "the Water Mage's Cave",
	  "the great Circle of Flame",
	  PM_HIGH_FLAME_MAGE, PM_WATER_MAGE, PM_IGNITER,
	  PM_WATER_ELEMENTAL, PM_RUST_MONSTER, S_ELEMENTAL, S_RUSTMONST,
	  ART_CANDLE_OF_ETERNAL_FLAME },
#endif
/* F */ { "¿å¤ÎËâÆ»»Î¤ÎÆ¶·¢",
	  "±ê¤Î°ÎÂç¤Ê¤ëÃÏ",
	  PM_HIGH_FLAME_MAGE, PM_WATER_MAGE, PM_IGNITER,
	  PM_WATER_ELEMENTAL, PM_RUST_MONSTER, S_ELEMENTAL, S_RUSTMONST,
	  ART_CANDLE_OF_ETERNAL_FLAME },

#if 0
/* G */ { "the Undermines",
	  "the Gnomish Mines",
	  PM_RUGGO_THE_GNOME_KING, PM_LARETH, PM_GNOME_WARRIOR,
	  PM_DROW, PM_OGRE, S_ORC, S_OGRE,
	  ART_PICK_OF_FLANDAL_STEELSKIN },
#endif
/* G */ { "ÃÏ²¼¹Û»³",
	  "¥Î¡¼¥à¤Î¹Û»³",
	  PM_RUGGO_THE_GNOME_KING, PM_LARETH, PM_GNOME_WARRIOR,
	  PM_DROW, PM_OGRE, S_ORC, S_OGRE,
	  ART_PICK_OF_FLANDAL_STEELSKIN },

#if 0
/* H */ { "the Temple of Coeus",
	  "the Temple of Epidaurus",
	  PM_HIPPOCRATES, PM_CYCLOPS, PM_ATTENDANT,
	  PM_GIANT_RAT, PM_SNAKE, S_RODENT, S_YETI,
	  ART_STAFF_OF_AESCULAPIUS },
#endif
/* H */ { "¥³¥ª¥¹»û±¡",
	  "¥¨¥Ô¥À¥¦¥é¥¹»û±¡",
	  PM_HIPPOCRATES, PM_CYCLOPS, PM_ATTENDANT,
	  PM_GIANT_RAT, PM_SNAKE, S_RODENT, S_YETI,
	  ART_STAFF_OF_AESCULAPIUS },

/*JP*/
#if 0
/* I */ { "the Earth Mage's Cave",
	  "the great Ring of Ice",
	  PM_HIGH_ICE_MAGE, PM_EARTH_MAGE, PM_FROSTER,
	  PM_RUST_MONSTER, PM_XORN, S_RUSTMONST, S_XORN,
	  ART_STORM_WHISTLE },
#endif
/* I */ { "ÂçÃÏ¤ÎËâÆ»»Î¤ÎÆ¶·¢",
	  "É¹¤Î°ÎÂç¤Ê¤ëÃÏ",
	  PM_HIGH_ICE_MAGE, PM_EARTH_MAGE, PM_FROSTER,
	  PM_RUST_MONSTER, PM_XORN, S_RUSTMONST, S_XORN,
	  ART_STORM_WHISTLE },

#ifdef JFIGHTER
/* J */ { "ÃÏµå",
	  "·î¤ÎµÜÅÂ",
	  PM_PRINCESS_OF_MOON, PM_JEDEITE, PM_PLANETARY_FIGHTER,
	  S_SNAKE, S_ZOMBIE, S_SNAKE, S_ZOMBIE,
	  ART_SILVER_CRYSTAL },
#endif /* JFIGHTER */
#if 0
/* K */ { "the Isle of Glass",
	  "Camelot Castle",
	  PM_KING_ARTHUR, PM_IXOTH, PM_PAGE,
	  PM_QUASIT, PM_OCHRE_JELLY, S_IMP, S_JELLY,
	  ART_MAGIC_MIRROR_OF_MERLIN },
#endif
/* K */ { "¥¬¥é¥¹¤ÎÅç",
	  "¥­¥ã¥á¥í¥Ã¥È¾ë",
	  PM_KING_ARTHUR, PM_IXOTH, PM_PAGE,
	  PM_QUASIT, PM_OCHRE_JELLY, S_IMP, S_JELLY,
	  ART_MAGIC_MIRROR_OF_MERLIN },
/*JP*/
#if 0
/* L */ { "the Calerin Forest",
	  "the Nightmare Forest",
	  PM_HIGH_LYCANTHROPE, PM_SIR_LORIMAR, PM_FIEND,
	  PM_WOODLAND_ELF, PM_FOREST_CENTAUR, S_HUMAN, S_CENTAUR,
	  ART_STAFF_OF_WITHERING },
#endif
/* L */ { "Calerin¤Î¿¹",
	  "¶²ÉÝ¤Î¿¹",
	  PM_HIGH_LYCANTHROPE, PM_SIR_LORIMAR, PM_FIEND,
	  PM_WOODLAND_ELF, PM_FOREST_CENTAUR, S_HUMAN, S_CENTAUR,
	  ART_STAFF_OF_WITHERING },

#if 0
/* M */ { "the Monastery of the Earth-Lord",
	  "the Monastery of Chan-Sune",
	  PM_GRAND_MASTER, PM_MASTER_KAEN, PM_ABBOT,
	  PM_LIZARD, PM_XORN, S_LIZARD, S_XORN,
	  ART_MANTLE_OF_KNOWLEDGE },
#endif
/* M */ { "¥¢¡¼¥¹¥í¡¼¥É½¤Æ»±¡",
	  "¥Á¥ã¥ó¥·¥å¡¼¥ó½¤Æ»±¡",
	  PM_GRAND_MASTER, PM_MASTER_KAEN, PM_ABBOT,
	  PM_LIZARD, PM_XORN, S_LIZARD, S_XORN,
	  ART_MANTLE_OF_KNOWLEDGE },

#if 0
/* N */ { "the Lair of Maugneshaagar",
	  "the Tower of the Dark Lord",
	  PM_THE_DARK_LORD, PM_MAUGNESHAAGAR, PM_EMBALMER,
	  PM_NUPPERIBO, PM_MONGBAT, S_BAT, S_IMP,
	  ART_GREAT_DAGGER_OF_GLAURGNAA },
#endif
/* N */ { "¥Þ¥¦¥°¥Í¥Ã¥·¥ã¥¬¡¼¤Î±£¤ì²È",
	  "¥À¡¼¥¯¥í¡¼¥É¤ÎÅã",
	  PM_THE_DARK_LORD, PM_MAUGNESHAAGAR, PM_EMBALMER,
	  PM_NUPPERIBO, PM_MONGBAT, S_BAT, S_IMP,
	  ART_GREAT_DAGGER_OF_GLAURGNAA },

#if 0
/* P */ { "the Temple of Nalzok",
	  "the Great Temple",
	  PM_ARCH_PRIEST, PM_NALZOK, PM_ACOLYTE,
	  PM_HUMAN_ZOMBIE, PM_WRAITH, S_ZOMBIE, S_WRAITH,
	  ART_MITRE_OF_HOLINESS },
#endif
/* P */ { "¥Ê¥ë¥¾¥¯»û±¡",
	  "°ÎÂç¤Ê¤ë»û±¡",
	  PM_ARCH_PRIEST, PM_NALZOK, PM_ACOLYTE,
	  PM_HUMAN_ZOMBIE, PM_WRAITH, S_ZOMBIE, S_WRAITH,
	  ART_MITRE_OF_HOLINESS },
/*JP*/

#if 0
/* R */ { "the Assassins' Guild Hall",
	  "the Thieves' Guild Hall",
	  PM_MASTER_OF_THIEVES, PM_MASTER_ASSASSIN, PM_THUG,
	  PM_LEPRECHAUN, PM_GUARDIAN_NAGA, S_NYMPH, S_NAGA,
	  ART_MASTER_KEY_OF_THIEVERY },
#endif
/* R */ { "°Å»¦¼Ô¤Î¥®¥ë¥É",
	  "ÅðÂ±¤Î¥®¥ë¥É",
	  PM_MASTER_OF_THIEVES, PM_MASTER_ASSASSIN, PM_THUG,
	  PM_LEPRECHAUN, PM_GUARDIAN_NAGA, S_NYMPH, S_NAGA,
	  ART_MASTER_KEY_OF_THIEVERY },

/*JP*/
#if 0
/* S */ { "the Shogun's Castle",
	  "the castle of the Taro Clan",
	  PM_LORD_SATO, PM_ASHIKAGA_TAKAUJI, PM_ROSHI,
	  PM_WOLF, PM_STALKER, S_DOG, S_STALKER,
	  ART_TSURUGI_OF_MURAMASA },
#endif
/* S */ { "¾­·³¤Î¾ë",
	  "ÂÀÏº°ìÂ²¤Î¾ë",
	  PM_LORD_SATO, PM_ASHIKAGA_TAKAUJI, PM_NINJA,
	  PM_WOLF, PM_STALKER, S_DOG, S_STALKER,
	  ART_TSURUGI_OF_MURAMASA },

#ifdef TOURIST
/*JP*/
#if 0
/* T */ { "the Thieves' Guild Hall",
	  "Ankh-Morpork",
	  PM_TWOFLOWER, PM_MASTER_OF_THIEVES, PM_GUIDE,
	  PM_GIANT_SPIDER, PM_FOREST_CENTAUR, S_SPIDER, S_CENTAUR,
	  ART_YENDORIAN_EXPRESS_CARD },
#endif
/* T */ { "ÅðÂ±¤Î¥®¥ë¥É",
	  "Î¹¹Ô¥È¥é¥Ö¥ë¥»¥ó¥¿¡¼",
	  PM_TWOFLOWER, PM_MASTER_OF_THIEVES, PM_GUIDE,
	  PM_GIANT_SPIDER, PM_FOREST_CENTAUR, S_SPIDER, S_CENTAUR,
	  ART_YENDORIAN_EXPRESS_CARD },
#endif

/*JP*/
#if 0
/* U */ { "the Crypt of Vlad",
	  "the Temple of Light",
	  PM_VAN_HELSING, PM_VLAD_THE_IMPALER, PM_EXTERMINATOR,
	  PM_HUMAN_MUMMY, PM_VAMPIRE, S_MUMMY, S_VAMPIRE,
	  ART_STAKE_OF_VAN_HELSING },
#endif
/* U */ { "¥ô¥é¥É¤ÎÃÏ²¼À»Æ²",
	  "¥é¥¤¥È»û±¡",
	  PM_VAN_HELSING, PM_VLAD_THE_IMPALER, PM_EXTERMINATOR,
	  PM_HUMAN_MUMMY, PM_VAMPIRE, S_MUMMY, S_VAMPIRE,
	  ART_STAKE_OF_VAN_HELSING },

#if 0
/* V */ { "the cave of Surtur",
	  "the Shrine of Destiny",
	  PM_NORN, PM_LORD_SURTUR, PM_WARRIOR,
	  PM_FIRE_ANT, PM_FIRE_GIANT, S_ANT, S_GIANT,
	  ART_ORB_OF_FATE },
#endif
/* V */ { "¥µ¡¼¥¿¡¼¤ÎÆ¶·¢",
	  "±¿Ì¿¤ÎÀ»Æ²",
	  PM_NORN, PM_LORD_SURTUR, PM_WARRIOR,
	  PM_FIRE_ANT, PM_FIRE_GIANT, S_ANT, S_GIANT,
	  ART_ORB_OF_FATE },
/*JP*/
#if 0
/* W */ { "the Tower of Darkness",
	  "the Tower of the Balance",
	  PM_WIZARD_OF_BALANCE, PM_DARK_ONE, PM_APPRENTICE,
	  PM_VAMPIRE_BAT, PM_XORN, S_BAT, S_WRAITH,
	  ART_EYE_OF_THE_AETHIOPICA },
#endif
/* W */ { "°Å¹õ¤ÎÅã",
	  "Ä´ÏÂ¤ÎÅã",
	  PM_WIZARD_OF_BALANCE, PM_DARK_ONE, PM_APPRENTICE,
	  PM_VAMPIRE_BAT, PM_XORN, S_BAT, S_WRAITH,
	  ART_EYE_OF_THE_AETHIOPICA },

#if 0
/* E - drow */ { "the great Circle of Earendil",
	  "the Temple of Lolth",
	  PM_LOLTH, PM_EARENDIL, PM_DROW,
	  PM_HIGH_ELF, PM_GREY_ELF, S_DOG, S_HUMAN,
	  ART_TENTACLE_STAFF },
#endif
/* E - drow */ { "¥¨¥¢¥ì¥ó¥À¥ë¤Î°ÎÂç¤Ê¤ëÃÏ",
	  "¥í¥ë¥¹»û±¡",
	  PM_LOLTH, PM_EARENDIL, PM_DROW,
	  PM_HIGH_ELF, PM_GREY_ELF, S_DOG, S_HUMAN,
	  ART_TENTACLE_STAFF },

/* - */ { "", "", 0, 0, 0, 0, 0, 0, 0, 0 }
};

static unsigned
class_index()
{
	switch (pl_character[0]) {

	    case 'A':	return(0);
	    case 'B':	return(1);
	    case 'C':	return(2);
	    case 'D':   return(3);
	    case 'E':   {
	       if (u.uelf_drow) return(22);
	       return((unsigned)(4+flags.female));
	    }
	    case 'F':   return(6);
	    case 'G':   return(7);
	    case 'H':   return(8);
	    case 'I':   return(9);
#ifdef JFIGHTER
	    case 'J':   return(10);
	    case 'K':   return(11);
	    case 'L':   return(12);
	    case 'M':   return(13);
	    case 'N':   return(14);
	    case 'P':   return(15);
	    case 'R':   return(16);
	    case 'S':   return(17);
#ifdef TOURIST
	    case 'T':   return(18);
	    case 'U':   return(19);
	    case 'V':   return(20);
	    case 'W':   return(21);
	    default:    return(23);
#else
	    case 'U':   return(18);
	    case 'V':   return(19);
	    case 'W':   return(20);
            default:    return(23);
#endif
#else  /* JFIGHTER */
	    case 'K':   return(10);
	    case 'L':   return(11);
	    case 'M':   return(12);
	    case 'N':   return(13);
	    case 'P':   return(14);
	    case 'R':   return(15);
	    case 'S':   return(16);
#ifdef TOURIST
	    case 'T':   return(17);
	    case 'U':   return(18);
	    case 'V':   return(19);
	    case 'W':   return(20);
	    default:    return(23);
#else
	    case 'U':   return(17);
	    case 'V':   return(18);
	    case 'W':   return(19);
            default:    return(23);
#endif
#endif /* JFIGHTER */
	}
}

short
quest_info(typ)
int typ;
{
	struct qt_matrix *qt = &qt_matrix[class_index()];

	switch (typ) {
	    case 0:		return qt->artinum;
	    case MS_LEADER:	return qt->ldrnum;
	    case MS_NEMESIS:	return qt->neminum;
	    case MS_GUARDIAN:	return qt->guardnum;
	    default:		impossible("quest_info(%d)", typ);
	}
	return 0;
}

const char *
ldrname()	/* return your class leader's name */
{
	int i = qt_matrix[class_index()].ldrnum;

/*JP	Sprintf(nambuf, "%s%s",
		type_is_pname(&mons[i]) ? "" : "the ",
		mons[i].mname);*/
	Sprintf(nambuf, "%s",jtrns_mon(mons[i].mname, -1));
	return nambuf;
}

static const char *
intermed()	/* return your intermediate target string */
{
	return(qt_matrix[class_index()].intermed);
}

boolean
is_quest_artifact(otmp)
struct obj *otmp;
{
	return((boolean)(otmp->oartifact == qt_matrix[class_index()].artinum));
}

static const char *
neminame()	/* return your class nemesis' name */
{
	int i = qt_matrix[class_index()].neminum;

/*JP	Sprintf(nambuf, "%s%s",
		type_is_pname(&mons[i]) ? "" : "the ",
		mons[i].mname);*/
	Sprintf(nambuf, "%s",jtrns_mon(mons[i].mname, -1));
	return nambuf;
}

static const char *
guardname()	/* return your class leader's guard monster name */
{
	int i = qt_matrix[class_index()].guardnum;

/*JP	return(mons[i].mname);*/
	return(jtrns_mon(mons[i].mname, -1));
}

static const char *
homebase()	/* return your class leader's location */
{
	return(qt_matrix[class_index()].homebase);
}

boolean
leaderless()    /* return true if leader is dead */
{
	int i = qt_matrix[class_index()].ldrnum;
	/* BUG: This doesn't take the possibility of resurrection
		via wand or spell of undead turning into account. */
	return (boolean)(mvitals[i].died > 0);
}

static struct qtmsg *
msg_in(qtm_list, msgnum)
struct qtmsg *qtm_list;
int	msgnum;
{
	struct qtmsg *qt_msg;

	for (qt_msg = qtm_list; qt_msg->msgnum > 0; qt_msg++)
	    if (qt_msg->msgnum == msgnum) return(qt_msg);

	return((struct qtmsg *)0);
}

static void
convert_arg(c)
char c;
{
	register const char *str;

	switch (c) {

	    case 'p':	str = plname;
			break;
	    case 'c':	str = jtrns_mon(pl_character, flags.female);
			break;
	    case 'r':	str = rank_of(u.ulevel, pl_character[0], flags.female);
			break;
	    case 'R':	str = rank_of(MIN_QUEST_LEVEL, pl_character[0],
							  flags.female);
			break;
/*JP	    case 's':	str = (flags.female) ? "sister" : "brother";*/
	    case 's':	str = (flags.female) ? "Ëå" : "Äï";
			break;
/*JP	    case 'S':	str = (flags.female) ? "daughter" : "son";*/
	    case 'S':	str = (flags.female) ? "Ì¼" : "Â©»Ò";
			break;
	    case 'l':	str = ldrname();
			break;
	    case 'i':	str = intermed();
			break;
/*JP
	    case 'o':	str = the(artiname(qt_matrix[class_index()].artinum));
*/
	    case 'o':	str = jtrns_obj('A',(artiname(qt_matrix[class_index()].artinum)));
			break;
	    case 'n':	str = neminame();
			break;
	    case 'g':	str = guardname();
			break;
	    case 'H':	str = homebase();
			break;
	    case 'a':	str = align_str(u.ualignbase[1]);
			break;
	    case 'A':	str = align_str(u.ualign.type);
			break;
	    case 'd':	str = align_gname(u.ualignbase[1]);
			break;
	    case 'D':	str = align_gname(A_LAWFUL);
			break;
/*JP	    case 'C':	str = "chaotic";*/
	    case 'C':	str = "º®ÆÙ";	        
			break;
/*JP	    case 'N':	str = "neutral";*/
	    case 'N':	str = "ÃæÎ©";	        
			break;
/*JP	    case 'L':	str = "lawful";*/
	    case 'L':	str = "Ãá½ø";	        
			break;
/*JP	    case 'x':	str = Blind ? "sense" : "see";*/
	    case 'x':	str = Blind ? "´¶¤¸" : "¸«";
			break;
	    case '%':	str = "%";
			break;
	     default:	str = "";
			break;
	}
	Strcpy(cvt_buf, str);
}

static void
convert_line()
{
	char *c, *cc;
	char xbuf[BUFSZ];

	cc = out_line;
	for (c = xcrypt(in_line, xbuf); *c; c++) {

	    *cc = 0;
	    switch(*c) {

		case '\r':
		case '\n':
			*(++cc) = 0;
			return;

		case '%':
			if (*(c+1)) {
			    convert_arg(*(++c));
			    switch (*(++c)) {

					/* insert "a"/"an" prefix */
				case 'A': Strcat(cc, An(cvt_buf));
				    cc += strlen(cc);
				    continue; /* for */
				case 'a': Strcat(cc, an(cvt_buf));
				    cc += strlen(cc);
				    continue; /* for */

					/* capitalize */
				case 'C': cvt_buf[0] = highc(cvt_buf[0]);
				    break;

					/* pluralize */
				case 'P': cvt_buf[0] = highc(cvt_buf[0]);
				case 'p': Strcpy(cvt_buf, makeplural(cvt_buf));
				    break;

					/* append possessive suffix */
				case 'S': cvt_buf[0] = highc(cvt_buf[0]);
				case 's': Strcpy(cvt_buf, s_suffix(cvt_buf));
				    break;

					/* strip any "the" prefix */
				case 't': if (!strncmpi(cvt_buf, "the ", 4)) {
					Strcat(cc, &cvt_buf[4]);
					cc += strlen(cc);
					continue; /* for */
				    }
				    break;

				default: --c;	/* undo switch increment */
				    break;
			    }
			    Strcat(cc, cvt_buf);
			    cc += strlen(cvt_buf);
			    break;
			}	/* else fall through */

		default:
			*cc++ = *c;
			break;
	    }
	}
	if (cc >= out_line + sizeof out_line)
	    panic("convert_line: overflow");
	*cc = 0;
	return;
}

static void
deliver_by_pline(qt_msg)
struct qtmsg *qt_msg;
{
	long	size;

	for (size = 0; size < qt_msg->size; size += (long)strlen(in_line)) {
	    (void) dlb_fgets(in_line, 80, msg_file);
	    convert_line();
	    pline(out_line);
	}

}

static void
deliver_by_window(qt_msg, how)
struct qtmsg *qt_msg;
int how;
{
	long	size;
	winid datawin = create_nhwindow(how);

	for (size = 0; size < qt_msg->size; size += (long)strlen(in_line)) {
	    (void) dlb_fgets(in_line, 80, msg_file);
	    convert_line();
	    putstr(datawin, 0, out_line);
	}
	display_nhwindow(datawin, TRUE);
	destroy_nhwindow(datawin);
}

void
com_pager(msgnum)
int	msgnum;
{
	struct qtmsg *qt_msg;

	if (!(qt_msg = msg_in(qt_list.common, msgnum))) {
		impossible("com_pager: message %d not found.", msgnum);
		return;
	}

	(void) dlb_fseek(msg_file, qt_msg->offset, SEEK_SET);
	if (qt_msg->delivery == 'p') deliver_by_pline(qt_msg);
	else if (msgnum == 1) deliver_by_window(qt_msg, NHW_MENU);
	else		     deliver_by_window(qt_msg, NHW_TEXT);
	return;
}

void
qt_pager(msgnum)
int	msgnum;
{
	struct qtmsg *qt_msg;

	if (!(qt_msg = msg_in(qt_list.chclass, msgnum))) {
		impossible("qt_pager: message %d not found.", msgnum);
		return;
	}

	(void) dlb_fseek(msg_file, qt_msg->offset, SEEK_SET);
	if (qt_msg->delivery == 'p' && strcmp(windowprocs.name, "X11"))
		deliver_by_pline(qt_msg);
	else	deliver_by_window(qt_msg, NHW_TEXT);
	return;
}

struct permonst *
qt_montype()
{
	int qclass = class_index();
	int qpm;

	if (rn2(5)) {
		qpm = qt_matrix[qclass].mtyp1;
		if (qpm && rn2(5) && !(mvitals[qpm].mvflags & G_GENOD))
			return(&mons[qpm]);
		return(mkclass(qt_matrix[qclass].msym1,0));
	}
	qpm = qt_matrix[qclass].mtyp2;
	if (qpm && rn2(5) && !(mvitals[qpm].mvflags & G_GENOD))
		return(&mons[qpm]);
	return(mkclass(qt_matrix[qclass].msym2,0));
}

/*questpgr.c*/
