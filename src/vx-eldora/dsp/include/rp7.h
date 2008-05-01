/*
		Module:	RP7.H
		Author:	Grant Gray
		Date:	6/8/89

This is an "include" file which defines all the special I/O locationsin the
memory spaces of the Quad DSP and Collator boards.

*/
/* Quad DSP and Collator */
 
#define	A_PORT	0xFFDF04
#define	B_PORT	0xFFDF08
#define	C_PORT	0xFFDF10
#define	A_DSY	0xFFDF20
#define	B_DSY	0xFFDF40
#define	C_DSY	0xFFDF80
#define	FIFOCLR	0xFFDF00
#define A_SH1	0xFFDF24
#define B_SH1	0xFFDF48
#define C_SH1	0xFFDF90

/* Quad DSP only */

#define	DRDFLG	0xFFDF1C
#define CLRINT1 0xFFDF0C
#define CLRINT2 0xFFDF14
#define INTOUT2 0xFFDF18

/* Collator only */


#define	CRDLTCH	0xFFDE00
#define	CRDRFLG	0xFFDE10
#define	CDAC	0xFFDE30
#define	CLEDON	0xFFDE60
#define	CLEDOFF	0xFFDE70
#define	FSENSE	0xFFDF80
#define	A_SENSE	0
#define	B_SENSE	1
#define	C_SENSE	2
#define	ALLSENSE	3
