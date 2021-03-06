#include	"u.h"
#include	"lib.h"
#include	"mem.h"
#include	"dat.h"
#include	"fns.h"
#include	"io.h"

enum {
	Data=		0x60,	/* data port */

	Status=		0x64,	/* status port */
	 Inready=	0x01,	/*  input character ready */
	 Outbusy=	0x02,	/*  output busy */
	 Sysflag=	0x04,	/*  system flag */
	 Cmddata=	0x08,	/*  cmd==0, data==1 */
	 Inhibit=	0x10,	/*  keyboard/mouse inhibited */
	 Minready=	0x20,	/*  mouse character ready */
	 Rtimeout=	0x40,	/*  general timeout */
	 Parity=	0x80,

	Cmd=		0x64,	/* command port (write only) */
 
	Spec=	0x80,

	PF=	Spec|0x20,	/* num pad function key */
	View=	Spec|0x00,	/* view (shift window up) */
	KF=	Spec|0x40,	/* function key */
	Shift=	Spec|0x60,
	Break=	Spec|0x61,
	Ctrl=	Spec|0x62,
	Latin=	Spec|0x63,
	Caps=	Spec|0x64,
	Num=	Spec|0x65,
	No=	Spec|0x7F,	/* no mapping */

	Home=	KF|13,
	Up=	KF|14,
	Pgup=	KF|15,
	Print=	KF|16,
	Left=	View,
	Right=	View,
	End=	'\r',
	Down=	View,
	Pgdown=	View,
	Ins=	KF|20,
	Del=	0x7F,
};

uchar kbtab[] = 
{
[0x00]	No,	0x1b,	'1',	'2',	'3',	'4',	'5',	'6',
[0x08]	'7',	'8',	'9',	'0',	'-',	'=',	'\b',	'\t',
[0x10]	'q',	'w',	'e',	'r',	't',	'y',	'u',	'i',
[0x18]	'o',	'p',	'[',	']',	'\n',	Ctrl,	'a',	's',
[0x20]	'd',	'f',	'g',	'h',	'j',	'k',	'l',	';',
[0x28]	'\'',	'`',	Shift,	'\\',	'z',	'x',	'c',	'v',
[0x30]	'b',	'n',	'm',	',',	'.',	'/',	Shift,	No,
[0x38]	Latin,	' ',	Caps,	KF|1,	KF|2,	KF|3,	KF|4,	KF|5,
[0x40]	KF|6,	KF|7,	KF|8,	KF|9,	KF|10,	Num,	KF|12,	Home,
[0x48]	No,	No,	No,	No,	No,	No,	No,	No,
[0x50]	No,	No,	No,	No,	No,	No,	No,	KF|11,
[0x58]	KF|12,	No,	No,	No,	No,	No,	No,	No,
};

uchar kbtabshift[] =
{
[0x00]	No,	0x1b,	'!',	'@',	'#',	'$',	'%',	'^',
[0x08]	'&',	'*',	'(',	')',	'_',	'+',	'\b',	'\t',
[0x10]	'Q',	'W',	'E',	'R',	'T',	'Y',	'U',	'I',
[0x18]	'O',	'P',	'{',	'}',	'\n',	Ctrl,	'A',	'S',
[0x20]	'D',	'F',	'G',	'H',	'J',	'K',	'L',	':',
[0x28]	'"',	'~',	Shift,	'|',	'Z',	'X',	'C',	'V',
[0x30]	'B',	'N',	'M',	'<',	'>',	'?',	Shift,	No,
[0x38]	Latin,	' ',	Caps,	KF|1,	KF|2,	KF|3,	KF|4,	KF|5,
[0x40]	KF|6,	KF|7,	KF|8,	KF|9,	KF|10,	Num,	KF|12,	Home,
[0x48]	No,	No,	No,	No,	No,	No,	No,	No,
[0x50]	No,	No,	No,	No,	No,	No,	No,	KF|11,
[0x58]	KF|12,	No,	No,	No,	No,	No,	No,	No,
};

uchar kbtabesc1[] =
{
[0x00]	No,	No,	No,	No,	No,	No,	No,	No,
[0x08]	No,	No,	No,	No,	No,	No,	No,	No,
[0x10]	No,	No,	No,	No,	No,	No,	No,	No,
[0x18]	No,	No,	No,	No,	No,	Ctrl,	No,	No,
[0x20]	No,	No,	No,	No,	No,	No,	No,	No,
[0x28]	No,	No,	No,	No,	No,	No,	No,	No,
[0x30]	No,	No,	No,	No,	No,	No,	No,	Print,
[0x38]	Latin,	No,	No,	No,	No,	No,	No,	No,
[0x40]	No,	No,	No,	No,	No,	No,	Break,	Home,
[0x48]	Up,	Pgup,	No,	Down,	No,	Right,	No,	End,
[0x50]	Left,	Pgdown,	Ins,	Del,	No,	No,	No,	No,
[0x58]	No,	No,	No,	No,	No,	No,	No,	No,
};

struct latin
{
	uchar	l;
	char	c[2];
}latintab[] = {
	L'??',	"!!",	/* spanish initial ! */
	L'??',	"c|",	/* cent */
	L'??',	"c$",	/* cent */
	L'??',	"l$",	/* pound sterling */
	L'??',	"g$",	/* general currency */
	L'??',	"y$",	/* yen */
	L'??',	"j$",	/* yen */
	L'??',	"||",	/* broken vertical bar */
	L'??',	"SS",	/* section symbol */
	L'??',	"\"\"",	/* dieresis */
	L'??',	"cr",	/* copyright */
	L'??',	"cO",	/* copyright */
	L'??',	"sa",	/* super a, feminine ordinal */
	L'??',	"<<",	/* left angle quotation */
	L'??',	"no",	/* not sign, hooked overbar */
	L'??',	"--",	/* soft hyphen */
	L'??',	"rg",	/* registered trademark */
	L'??',	"__",	/* macron */
	L'??',	"s0",	/* degree (sup o) */
	L'??',	"+-",	/* plus-minus */
	L'??',	"s2",	/* sup 2 */
	L'??',	"s3",	/* sup 3 */
	L'??',	"''",	/* grave accent */
	L'??',	"mu",	/* mu */
	L'??',	"pg",	/* paragraph (pilcrow) */
	L'??',	"..",	/* centered . */
	L'??',	",,",	/* cedilla */
	L'??',	"s1",	/* sup 1 */
	L'??',	"so",	/* sup o */
	L'??',	">>",	/* right angle quotation */
	L'??',	"14",	/* 1/4 */
	L'??',	"12",	/* 1/2 */
	L'??',	"34",	/* 3/4 */
	L'??',	"??",	/* spanish initial ? */
	L'??',	"A`",	/* A grave */
	L'??',	"A'",	/* A acute */
	L'??',	"A^",	/* A circumflex */
	L'??',	"A~",	/* A tilde */
	L'??',	"A\"",	/* A dieresis */
	L'??',	"A:",	/* A dieresis */
	L'??',	"Ao",	/* A circle */
	L'??',	"AO",	/* A circle */
	L'??',	"Ae",	/* AE ligature */
	L'??',	"AE",	/* AE ligature */
	L'??',	"C,",	/* C cedilla */
	L'??',	"E`",	/* E grave */
	L'??',	"E'",	/* E acute */
	L'??',	"E^",	/* E circumflex */
	L'??',	"E\"",	/* E dieresis */
	L'??',	"E:",	/* E dieresis */
	L'??',	"I`",	/* I grave */
	L'??',	"I'",	/* I acute */
	L'??',	"I^",	/* I circumflex */
	L'??',	"I\"",	/* I dieresis */
	L'??',	"I:",	/* I dieresis */
	L'??',	"D-",	/* Eth */
	L'??',	"N~",	/* N tilde */
	L'??',	"O`",	/* O grave */
	L'??',	"O'",	/* O acute */
	L'??',	"O^",	/* O circumflex */
	L'??',	"O~",	/* O tilde */
	L'??',	"O\"",	/* O dieresis */
	L'??',	"O:",	/* O dieresis */
	L'??',	"OE",	/* O dieresis */
	L'??',	"Oe",	/* O dieresis */
	L'??',	"xx",	/* times sign */
	L'??',	"O/",	/* O slash */
	L'??',	"U`",	/* U grave */
	L'??',	"U'",	/* U acute */
	L'??',	"U^",	/* U circumflex */
	L'??',	"U\"",	/* U dieresis */
	L'??',	"U:",	/* U dieresis */
	L'??',	"UE",	/* U dieresis */
	L'??',	"Ue",	/* U dieresis */
	L'??',	"Y'",	/* Y acute */
	L'??',	"P|",	/* Thorn */
	L'??',	"Th",	/* Thorn */
	L'??',	"TH",	/* Thorn */
	L'??',	"ss",	/* sharp s */
	L'??',	"a`",	/* a grave */
	L'??',	"a'",	/* a acute */
	L'??',	"a^",	/* a circumflex */
	L'??',	"a~",	/* a tilde */
	L'??',	"a\"",	/* a dieresis */
	L'??',	"a:",	/* a dieresis */
	L'??',	"ao",	/* a circle */
	L'??',	"ae",	/* ae ligature */
	L'??',	"c,",	/* c cedilla */
	L'??',	"e`",	/* e grave */
	L'??',	"e'",	/* e acute */
	L'??',	"e^",	/* e circumflex */
	L'??',	"e\"",	/* e dieresis */
	L'??',	"e:",	/* e dieresis */
	L'??',	"i`",	/* i grave */
	L'??',	"i'",	/* i acute */
	L'??',	"i^",	/* i circumflex */
	L'??',	"i\"",	/* i dieresis */
	L'??',	"i:",	/* i dieresis */
	L'??',	"d-",	/* eth */
	L'??',	"n~",	/* n tilde */
	L'??',	"o`",	/* o grave */
	L'??',	"o'",	/* o acute */
	L'??',	"o^",	/* o circumflex */
	L'??',	"o~",	/* o tilde */
	L'??',	"o\"",	/* o dieresis */
	L'??',	"o:",	/* o dieresis */
	L'??',	"oe",	/* o dieresis */
	L'??',	"-:",	/* divide sign */
	L'??',	"o/",	/* o slash */
	L'??',	"u`",	/* u grave */
	L'??',	"u'",	/* u acute */
	L'??',	"u^",	/* u circumflex */
	L'??',	"u\"",	/* u dieresis */
	L'??',	"u:",	/* u dieresis */
	L'??',	"ue",	/* u dieresis */
	L'??',	"y'",	/* y acute */
	L'??',	"th",	/* thorn */
	L'??',	"p|",	/* thorn */
	L'??',	"y\"",	/* y dieresis */
	L'??',	"y:",	/* y dieresis */
	0,	0,
};

enum
{
	/* controller command byte */
	Cscs1=		(1<<6),		/* scan code set 1 */
	Cmousedis=	(1<<5),		/* mouse disable */
	Ckbddis=	(1<<4),		/* kbd disable */
	Csf=		(1<<2),		/* system flag */
	Cmouseint=	(1<<1),		/* mouse interrupt enable */
	Ckbdint=	(1<<0),		/* kbd interrupt enable */
};

static uchar ccc;
static int nokbd = 1;

int
latin1(int k1, int k2)
{
	struct latin *l;

	for(l=latintab; l->l; l++)
		if(k1==l->c[0] && k2==l->c[1])
			return l->l;
	return 0;
}

/*
 *  wait for output no longer busy
 */
static int
outready(void)
{
	int tries;

	for(tries = 0; (inb(Status) & Outbusy); tries++){
		if(tries > 500)
			return -1;
		delay(2);
	}
	return 0;
}

/*
 *  wait for input
 */
static int
inready(void)
{
	int tries;

	for(tries = 0; !(inb(Status) & Inready); tries++){
		if(tries > 500)
			return -1;
		delay(2);
	}
	return 0;
}

/*
 *  ask 8042 to enable the use of address bit 20
 */
void
i8042a20(void)
{
	outready();
	outb(Cmd, 0xD1);
	outready();
	outb(Data, 0xDF);
	outready();
}

/*
 *  ask 8042 to reset the machine
 */
void
i8042reset(void)
{
	int i, x;

	if(nokbd)
		return;

	*((ushort*)KADDR(0x472)) = 0x1234;	/* BIOS warm-boot flag */

	/*
	 *  newer reset the machine command
	 */
	outready();
	outb(Cmd, 0xFE);
	outready();

	/*
	 *  Pulse it by hand (old somewhat reliable)
	 */
	x = 0xDF;
	for(i = 0; i < 5; i++){
		x ^= 1;
		outready();
		outb(Cmd, 0xD1);
		outready();
		outb(Data, x);	/* toggle reset */
		delay(100);
	}
}

/*
 *  keyboard interrupt
 */
static void
i8042intr(Ureg*, void*)
{
	int s, c;
	static int esc1, esc2;
	static int alt, caps, ctl, num, shift;
	static int lstate, k1, k2;
	int keyup;

	/*
	 *  get status
	 */
	s = inb(Status);
	if(!(s&Inready))
		return;

	/*
	 *  get the character
	 */
	c = inb(Data);

	/*
	 *  if it's the aux port...
	 */
	if(s & Minready)
		return;

	/*
	 *  e0's is the first of a 2 character sequence
	 */
	if(c == 0xe0){
		esc1 = 1;
		return;
	} else if(c == 0xe1){
		esc2 = 2;
		return;
	}

	keyup = c&0x80;
	c &= 0x7f;
	if(c > sizeof kbtab){
		c |= keyup;
		if(c != 0xFF)	/* these come fairly often: CAPSLOCK U Y */
			print("unknown key %ux\n", c);
		return;
	}

	if(esc1){
		c = kbtabesc1[c];
		esc1 = 0;
	} else if(esc2){
		esc2--;
		return;
	} else if(shift)
		c = kbtabshift[c];
	else
		c = kbtab[c];

	if(caps && c<='z' && c>='a')
		c += 'A' - 'a';

	/*
	 *  keyup only important for shifts
	 */
	if(keyup){
		switch(c){
		case Latin:
			alt = 0;
			break;
		case Shift:
			shift = 0;
			break;
		case Ctrl:
			ctl = 0;
			break;
		}
		return;
	}

	/*
 	 *  normal character
	 */
	if(!(c & Spec)){
		if(ctl){
			if(alt && c == Del)
				warp86("\nCtrl-Alt-Del\n", 0);
			c &= 0x1f;
		}
		switch(lstate){
		case 1:
			k1 = c;
			lstate = 2;
			return;
		case 2:
			k2 = c;
			lstate = 0;
			c = latin1(k1, k2);
			if(c == 0){
				kbdchar(k1);
				c = k2;
			}
			/* fall through */
		default:
			break;
		}
	} else {
		switch(c){
		case Caps:
			caps ^= 1;
			return;
		case Num:
			num ^= 1;
			return;
		case Shift:
			shift = 1;
			return;
		case Latin:
			alt = 1;
			lstate = 1;
			return;
		case Ctrl:
			ctl = 1;
			return;
		}
	}
	kbdchar(c);
}

static char *initfailed = "i8042: kbdinit failed\n";

static int
outbyte(int port, int c)
{
	outb(port, c);
	if(outready() < 0) {
		vga = 0;
		print(initfailed);
		return -1;
	}
	return 0;
}

void
i8042init(void)
{
	int c, try;

	/* wait for a quiescent controller */
	try = 1000;
	while(try-- > 0 && (c = inb(Status)) & (Outbusy | Inready)) {
		if(c & Inready)
			inb(Data);
		delay(1);
	}
	if (try <= 0) {
		vga = 0;
		print(initfailed);
		return;
	}

	/* get current controller command byte */
	outb(Cmd, 0x20);
	if(inready() < 0){
		print("i8042: kbdinit can't read ccc\n");
		ccc = 0;
	} else
		ccc = inb(Data);

	/* enable kbd xfers and interrupts */
	ccc &= ~Ckbddis;
	ccc |= Csf | Ckbdint | Cscs1;
	if(outready() < 0) {
		vga = 0;
		print(initfailed);
		return;
	}

	nokbd = 0;

	/* disable mouse */
	if (outbyte(Cmd, 0x60) < 0 || outbyte(Data, ccc) < 0)
		print("i8042: kbdinit mouse disable failed\n");

	setvec(VectorKBD, i8042intr, 0);
}
