 #
 # df_cosa.s
 #
 
	
	global	splimit%,spgrow%
splimit%:
	long	0
spgrow%:
	rts
	
	global	rtc_tick
rtc_tick:
	rts
	
	global	initrtc_%
initrtc_%:
	movm.l	&0xc0c0,-(%a7)
	mov.w	%d0,-(%a7)
	clr.w	-(%a7)
	mov.l	&rtc_0%,(0x60+5)*4
	jsr	rtc_init
	add.w	&4,%a7
	mov.l	%d0,%d7
	movm.l	(%a7)+,&0x0303
	rts
rtc_0%:
	movm.l	&0xc0c0,-(%a7)
	mov.l	&0,%d0
	mov.l	%d0,-(%a7)
	jsr	rtc_intr
	add.w	&4,%a7
	movm.l	(%a7)+,&0x0303
	trap	&13
BC0%:
	movm.l	&0xc0c0,-(%a7)
	mov.l	&0,%d0
	mov.l	%d0,-(%a7)
	jsr	BCintr
	add.w	&4,%a7
	movm.l	(%a7)+,&0x0303
	trap	&13
abo_0%:
	movm.l	&0xc0c0,-(%a7)
	mov.l	&0,%d0
	mov.l	%d0,-(%a7)
	jsr	abo_intr
	add.w	&4,%a7
	movm.l	(%a7)+,&0x0303
	trap	&13
t1_0%:
	movm.l	&0xc0c0,-(%a7)
	mov.l	&0,%d0
	mov.l	%d0,-(%a7)
	jsr	t1_intr
	add.w	&4,%a7
	movm.l	(%a7)+,&0x0303
	trap	&13
	
	global	load_vec
load_vec:
	jsr	initdebugio
	mov.l	&nullispex,(13+32)*4
	mov.l	&BC0%,408
	mov.l	&abo_0%,412
	mov.l	&t1_0%,468
	mov.l	%d7,-(%a7)
	mov.l	&(0*256),%d0
	jsr	initrtc_%
	mov.l	(%a7)+,%d7
	mov	%a7,%usp
	mov.l	&0x2000,%d0
	mov	%d0,%sr
	rts
	
nullispex:
	add.w	&8,%a7
	rte
