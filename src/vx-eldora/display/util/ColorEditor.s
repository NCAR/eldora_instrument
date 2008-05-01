#NO_APP
gcc2_compiled.:
___gnu_compiled_cplusplus:
.data
_rcsid:
	.ascii "$Date$ $RCSfile$ $Revision$\0"
.lcomm LF0,4
.text
	.even
.globl ___3BoxR6WindowG11singlePointiiUc
___3BoxR6WindowG11singlePointiiUc:
	link a6,#0
	movel a2,sp@-
	movel a6@(8),a2
	movel a6@(20),d0
	moveb a6@(31),d1
	movel a6@(12),a2@
	lea a2@(4),a0
	movel a6@(16),a0@
	movel a6@(24),a2@(8)
	movel d0,a2@(12)
	clrl sp@-
	moveb d1,sp@(3)
	movel d0,sp@-
	movel a2@(8),sp@-
	movel a0@,sp@-
	movel a2@,sp@-
	jbsr _frect__6WindowG11singlePointiiUc
	movel a2,d0
	movel a6@(-4),a2
	unlk a6
	rts
	.even
.globl _contains__3BoxG11singlePoint
_contains__3BoxG11singlePoint:
	link a6,#0
	movel d3,sp@-
	movel d2,sp@-
	movel a6@(8),a0
	movel a6@(12),d0
	movel d0,d2
	clrw d2
	swap d2
	clrl d3
	movew d0,d3
	clrl d1
	movew a0@(4),d1
	clrl d0
	movew a0@(6),d0
	cmpl d2,d1
	jgt L1222
	cmpl d3,d0
	jle L1221
L1222:
	clrl d0
	jra L1220
L1221:
	addl a0@(8),d1
	addl a0@(12),d0
	cmpl d2,d1
	jgt L1224
	cmpl d3,d0
	jle L1220
L1224:
	clrl d0
L1220:
	movel a6@(-8),d2
	movel a6@(-4),d3
	unlk a6
	rts
	.even
.globl ___6ButtonR6WindowG11singlePointiiPcUc
___6ButtonR6WindowG11singlePointiiPcUc:
	link a6,#0
	movel d3,sp@-
	movel d2,sp@-
	movel a6@(8),d2
	movel a6@(28),d3
	clrl d0
	moveb a6@(35),d0
	movel d0,sp@-
	movel a6@(24),sp@-
	movel a6@(20),sp@-
	movel a6@(16),sp@-
	movel a6@(12),sp@-
	movel d2,sp@-
	jbsr ___3BoxR6WindowG11singlePointiiUc
	addw #20,sp
	movel d3,sp@
	movel d2,sp@-
	jbsr _label__6ButtonPc
	movel d2,d0
	movel a6@(-8),d2
	movel a6@(-4),d3
	unlk a6
	rts
	.even
.globl _label__6ButtonPc
_label__6ButtonPc:
	link a6,#-4
	movel a6@(8),a0
	movel a0@(4),a6@(-4)
	movew a0@(14),d0
	addw d0,a6@(-2)
	pea 127:w
	movel a6@(12),sp@-
	movel a6@(-4),sp@-
	movel a0@,sp@-
	jbsr _horText__6WindowG11singlePointPCcUc
	unlk a6
	rts
LC1:
	.ascii "Red <<\0"
LC2:
	.ascii "Red >>\0"
LC3:
	.ascii "Green <<\0"
LC4:
	.ascii "Green >>\0"
LC5:
	.ascii "Blue <<\0"
LC6:
	.ascii "Blue >>\0"
LC7:
	.ascii "Undo\0"
LC8:
	.ascii "Save\0"
LC9:
	.ascii "Load\0"
LC10:
	.ascii "Quit\0"
	.even
.globl ___11ColorEditorR17GraphicControllerPc
___11ColorEditorR17GraphicControllerPc:
	link a6,#-104
	moveml #0x3c3c,sp@-
	movel a6@(8),a5
	movel a6@(12),d2
	movel a6@(16),d5
	movel #__vt$8Callback,a5@(4)
	movel #_junk,a5@
	movel #__vt$11ColorEditor,a5@(4)
	clrl sp@-
	clrl sp@-
	pea 1024:w
	pea 1280:w
	clrl sp@-
	clrl sp@-
	clrl sp@-
	movel a5@(432),sp@-
	movel #436,d4
	pea a5@(d4:l)
	jbsr ___6WindowP17GraphicControlleriUsUsUsUsUsUs
	addw #36,sp
	movel d2,a5@(432)
	tstl d5
	jeq L1236
	lea a6@(-32),a1
	movel a1,a6@(-40)
	lea a6@(-32),a1
	movel a1,a6@(-44)
	movel #__vt$3ios,a6@(-4)
	clrl sp@-
	clrl sp@-
	pea a6@(-32)
	jbsr _init__3iosP9streambufP7ostream
	addqw #8,sp
	movel d4,sp@
	pea 1:w
	movel d5,sp@-
	clrl sp@-
	pea a6@(-44)
	jbsr ___11fstreambaseiPCcii
	lea a6@(-40),a2
	addw #20,sp
	movel a2@,a0
	movel #__vt$7istream$3ios,a0@(28)
	clrl a6@(-36)
	movel a6@(-44),a0
	movel #__vt$8ifstream$3ios,a0@(28)
	movel a5,sp@-
	movel a2,sp@-
	jbsr ___rs__FR7istreamR11ColorEditor
	addqw #8,sp
	movel a6@(-44),a0
	movel #__vt$8ifstream$3ios,a0@(28)
	movel a2@,a0
	movel #__vt$7istream$3ios,a0@(28)
	movel a6@(-44),a0
	movel #__vt$11fstreambase$3ios,a0@(28)
	movel a6@(-40),a0
	movel #__vt$3ios,a0@(28)
	tstw a0@(14)
	jlt L1236
	movel a0@,a2
	tstl a2
	jeq L1236
	movel a2@(76),a0
	pea 3:w
	movew a0@(48),a1
	pea a1@(a2:l)
	movel a0@(52),a0
	jbsr a0@
	addqw #8,sp
L1236:
	moveq #30,d3
	movew #96,a2
	clrl d4
	clrl d5
L1295:
	cmpl d4,a2
	jle L1296
	pea 20:w
	jbsr ___builtin_new
	movel d0,d2
	movel #60,sp@
	pea a5@(436)
	pea a6@(-104)
	jbsr _memcpy
	addqw #8,sp
	addqw #4,sp
	moveb d4,d5
	movel d5,sp@-
	pea 20:w
	pea 20:w
	movel d3,sp@-
	pea a6@(-104)
	movel d2,sp@-
	jbsr ___3BoxR6WindowG11singlePointiiUc
	addw #24,sp
	movel d2,a5@(8,d4:l:4)
	moveq #31,d1
	cmpl d4,d1
	jeq L1304
	moveq #63,d1
	cmpl d4,d1
	jne L1303
L1304:
	movew d3,d0
	addw #50,d0
	clrl d3
	movew d0,d3
	jra L1297
L1303:
	bfextu d3{#0:#16},d0
	addw #30,d0
	bfins d0,d3{#0:#16}
L1297:
	addql #1,d4
	jra L1295
L1296:
	moveq #55,d3
	notb d3
	pea 126:w
	pea LC1
	pea 100:w
	pea 50:w
	movel d3,sp@-
	movel #436,d2
	lea a5@(0,d2:l),a4
	movel a4,sp@-
	pea 22:w
	lea ___builtin_new,a3
	jbsr a3@
	movel d0,sp@
	lea ___6ButtonR6WindowG11singlePointiiPcUc,a2
	jbsr a2@
	movel d0,a5@(396)
	bfextu d3{#0:#16},d0
	addw #110,d0
	bfins d0,d3{#0:#16}
	pea 126:w
	pea LC2
	pea 100:w
	pea 50:w
	movel d3,sp@-
	movel a4,sp@-
	pea 22:w
	jbsr a3@
	movel d0,sp@
	jbsr a2@
	movel d0,a5@(392)
	movew d3,d0
	addw #70,d0
	clrl d3
	movew d0,d3
	addw #52,sp
	movel #126,sp@
	pea LC3
	pea 100:w
	pea 50:w
	movel d3,sp@-
	movel a4,sp@-
	pea 22:w
	jbsr a3@
	movel d0,sp@
	jbsr a2@
	movel d0,a5@(404)
	bfextu d3{#0:#16},d0
	addw #110,d0
	bfins d0,d3{#0:#16}
	pea 126:w
	pea LC4
	pea 100:w
	pea 50:w
	movel d3,sp@-
	movel a4,sp@-
	pea 22:w
	jbsr a3@
	movel d0,sp@
	jbsr a2@
	movel d0,a5@(400)
	movew d3,d0
	addw #70,d0
	clrl d3
	movew d0,d3
	addw #52,sp
	movel #126,sp@
	pea LC5
	pea 100:w
	pea 50:w
	movel d3,sp@-
	movel a4,sp@-
	pea 22:w
	jbsr a3@
	movel d0,sp@
	jbsr a2@
	movel d0,a5@(412)
	bfextu d3{#0:#16},d0
	addw #110,d0
	bfins d0,d3{#0:#16}
	pea 126:w
	pea LC6
	pea 100:w
	pea 50:w
	movel d3,sp@-
	movel a4,sp@-
	pea 22:w
	jbsr a3@
	movel d0,sp@
	jbsr a2@
	movel d0,a5@(408)
	movel a5@(396),a0
	addw #56,sp
	movel a5,a0@(18)
	movel a5@(404),a0
	movel a5,a0@(18)
	movel a5@(412),a0
	movel a5,a0@(18)
	movel a5@(392),a0
	movel a5,a0@(18)
	movel a5@(400),a0
	movel a5,a0@(18)
	movel a5@(408),a0
	movel a5,a0@(18)
	movew d3,d0
	addw #70,d0
	clrl d3
	movew d0,d3
	pea 126:w
	pea LC7
	pea 100:w
	pea 50:w
	movel d3,sp@-
	addl a5,d2
	movel d2,sp@-
	pea 22:w
	jbsr a3@
	movel d0,sp@
	jbsr a2@
	movel d0,a5@(428)
	bfextu d3{#0:#16},d0
	addw #110,d0
	bfins d0,d3{#0:#16}
	pea 126:w
	pea LC8
	pea 100:w
	pea 50:w
	movel d3,sp@-
	movel d2,sp@-
	pea 22:w
	jbsr a3@
	movel d0,sp@
	jbsr a2@
	movel d0,a5@(420)
	bfextu d3{#0:#16},d0
	addw #110,d0
	bfins d0,d3{#0:#16}
	addw #52,sp
	movel #126,sp@
	pea LC9
	pea 100:w
	pea 50:w
	movel d3,sp@-
	movel d2,sp@-
	pea 22:w
	jbsr a3@
	movel d0,sp@
	jbsr a2@
	movel d0,a5@(416)
	bfextu d3{#0:#16},d0
	addw #110,d0
	bfins d0,d3{#0:#16}
	pea 126:w
	pea LC10
	pea 100:w
	pea 50:w
	movel d3,sp@-
	movel d2,sp@-
	pea 22:w
	jbsr a3@
	movel d0,sp@
	jbsr a2@
	movel d0,a5@(424)
	movel a5@(428),a0
	addw #56,sp
	movel a5,a0@(18)
	movel a5@(416),a0
	movel a5,a0@(18)
	movel a5@(420),a0
	movel a5,a0@(18)
	movel a5@(424),a0
	movel a5,a0@(18)
	clrl a5@(2548)
	pea 127:w
	movel a5@(8),sp@-
	movel a5,sp@-
	jbsr _lines__11ColorEditorR4CBoxUc
	clrl a5@(2552)
	movel a5,d0
	moveml a6@(-136),#0x3c3c
	unlk a6
	rts
	.even
.globl _run__11ColorEditor
_run__11ColorEditor:
	link a6,#0
	moveml #0x3838,sp@-
	movel a6@(8),a3
	lea _contains__3BoxG11singlePoint,a4
L1339:
	tstl a3@(2552)
	jne L1340
	movel a3@(496),sp@-
	jbsr _waitOnInterrupt__5Mouse
	movel a3@(496),sp@-
	jbsr _getXY__5Mouse
	movel d0,a0
	movel a0@,d3
	movel d3,sp@-
	movel a3@(428),sp@-
	jbsr a4@
	addqw #8,sp
	addqw #8,sp
	tstl d0
	jeq L1342
	movel a3@(428),a0
	jra L1390
L1342:
	movel d3,sp@-
	movel a3@(420),sp@-
	jbsr a4@
	addqw #8,sp
	tstl d0
	jeq L1346
	movel a3@(420),a0
	jra L1390
L1346:
	movel d3,sp@-
	movel a3@(416),sp@-
	jbsr a4@
	addqw #8,sp
	tstl d0
	jeq L1350
	movel a3@(416),a0
	jra L1390
L1350:
	movel d3,sp@-
	movel a3@(424),sp@-
	jbsr a4@
	addqw #8,sp
	tstl d0
	jeq L1354
	movel a3@(424),a0
	jra L1390
L1354:
	movel d3,sp@-
	movel a3@(392),sp@-
	jbsr a4@
	addqw #8,sp
	tstl d0
	jeq L1358
	movel a3@(392),a0
	jra L1390
L1358:
	movel d3,sp@-
	movel a3@(396),sp@-
	jbsr a4@
	addqw #8,sp
	tstl d0
	jeq L1362
	movel a3@(396),a0
	jra L1390
L1362:
	movel d3,sp@-
	movel a3@(400),sp@-
	jbsr a4@
	addqw #8,sp
	tstl d0
	jeq L1366
	movel a3@(400),a0
	jra L1390
L1366:
	movel d3,sp@-
	movel a3@(404),sp@-
	jbsr a4@
	addqw #8,sp
	tstl d0
	jeq L1370
	movel a3@(404),a0
	jra L1390
L1370:
	movel d3,sp@-
	movel a3@(408),sp@-
	jbsr a4@
	addqw #8,sp
	tstl d0
	jeq L1374
	movel a3@(408),a0
	jra L1390
L1374:
	movel d3,sp@-
	movel a3@(412),sp@-
	jbsr a4@
	addqw #8,sp
	tstl d0
	jeq L1378
	movel a3@(412),a0
L1390:
	movel a0@(18),a1
	movel a1@(4),a2
	movel a1@,sp@-
	movew a2@(8),a0
	pea a0@(a1:l)
	movel a2@(12),a0
	jbsr a0@
	addqw #8,sp
	jra L1339
L1378:
	movel a3@(2548),d0
	pea 126:w
	movel a3@(8,d0:l:4),sp@-
	movel a3,sp@-
	jbsr _lines__11ColorEditorR4CBoxUc
	addqw #8,sp
	addqw #4,sp
	movel a3@(8),a2
	moveq #96,d4
	clrl d2
L1383:
	cmpl d2,d4
	jle L1339
	movel d3,sp@-
	movel a2,sp@-
	jbsr _contains__3BoxG11singlePoint
	addqw #8,sp
	tstl d0
	jeq L1387
	movel a2@(16),a3@(2548)
L1387:
	pea 127:w
	movel a2,sp@-
	movel a3,sp@-
	jbsr _lines__11ColorEditorR4CBoxUc
	addqw #8,sp
	addqw #4,sp
	addql #1,d2
	addw #20,a2
	jra L1383
L1340:
	moveml a6@(-24),#0x1c1c
	unlk a6
	rts
	.even
.globl _doCallback__11ColorEditorR5Actor
_doCallback__11ColorEditorR5Actor:
	link a6,#0
	movel a6@(8),a0
	moveq #-16,d0
	addl a6@(12),d0
	movel d0,d1
	cmpl a0@(416),d0
	jne L1392
	movel d0,sp@-
	movel a0,sp@-
	jbsr _load__11ColorEditorR6Button
	jra L1393
L1392:
	cmpl a0@(420),d0
	jne L1394
	movel d0,sp@-
	movel a0,sp@-
	jbsr _save__11ColorEditorR6Button
	jra L1393
L1394:
	cmpl a0@(428),d0
	jne L1396
	movel d0,sp@-
	movel a0,sp@-
	jbsr _undo__11ColorEditorR6Button
	jra L1393
L1396:
	cmpl a0@(424),d0
	jne L1398
	movel d0,sp@-
	movel a0,sp@-
	jbsr _quit__11ColorEditorR6Button
	jra L1393
L1398:
	movel d1,sp@-
	movel a0,sp@-
	jbsr _adjustColors__11ColorEditorR6Button
L1393:
	unlk a6
	rts
LC11:
	.ascii "Enter name of file to load: \0"
	.even
.globl _load__11ColorEditorR6Button
_load__11ColorEditorR6Button:
	link a6,#-300
	moveml #0x2030,sp@-
	movel a6@(8),d2
	pea LC11
	pea _cout
	jbsr ___ls__7ostreamPCc
	pea a6@(-256)
	pea _cin
	jbsr ___rs__7istreamPc
	addqw #8,sp
	addqw #8,sp
	lea a6@(-288),a3
	movel a3,a6@(-296)
	lea a6@(-288),a3
	movel a3,a6@(-300)
	movel #__vt$3ios,a6@(-260)
	clrl sp@-
	clrl sp@-
	pea a6@(-288)
	jbsr _init__3iosP9streambufP7ostream
	addqw #8,sp
	movel #436,sp@
	pea 1:w
	pea a6@(-256)
	clrl sp@-
	pea a6@(-300)
	jbsr ___11fstreambaseiPCcii
	lea a6@(-296),a2
	addw #20,sp
	movel a2@,a0
	movel #__vt$7istream$3ios,a0@(28)
	clrl a6@(-292)
	movel a6@(-300),a0
	movel #__vt$8ifstream$3ios,a0@(28)
	movel d2,sp@-
	pea a6@(-296)
	jbsr ___rs__FR7istreamR11ColorEditor
	addqw #8,sp
	movel a6@(-300),a0
	movel #__vt$8ifstream$3ios,a0@(28)
	movel a2@,a0
	movel #__vt$7istream$3ios,a0@(28)
	movel a6@(-300),a0
	movel #__vt$11fstreambase$3ios,a0@(28)
	movel a6@(-296),a0
	movel #__vt$3ios,a0@(28)
	tstw a0@(14)
	jlt L1411
	movel a0@,a2
	tstl a2
	jeq L1411
	movel a2@(76),a0
	pea 3:w
	movew a0@(48),a1
	pea a1@(a2:l)
	movel a0@(52),a0
	jbsr a0@
L1411:
	moveml a6@(-312),#0xc04
	unlk a6
	rts
LC12:
	.ascii "Enter name of file to save: \0"
	.even
.globl _save__11ColorEditorR6Button
_save__11ColorEditorR6Button:
	link a6,#-296
	moveml #0x2030,sp@-
	movel a6@(8),d2
	pea LC12
	pea _cout
	jbsr ___ls__7ostreamPCc
	pea a6@(-256)
	pea _cin
	jbsr ___rs__7istreamPc
	addqw #8,sp
	addqw #8,sp
	lea a6@(-288),a3
	movel a3,a6@(-292)
	lea a6@(-288),a3
	movel a3,a6@(-296)
	movel #__vt$3ios,a6@(-260)
	clrl sp@-
	clrl sp@-
	pea a6@(-288)
	jbsr _init__3iosP9streambufP7ostream
	addqw #8,sp
	movel #436,sp@
	pea 2:w
	pea a6@(-256)
	clrl sp@-
	pea a6@(-296)
	jbsr ___11fstreambaseiPCcii
	lea a6@(-292),a2
	addw #20,sp
	movel a2@,a0
	movel #__vt$7ostream$3ios,a0@(28)
	movel a6@(-296),a0
	movel #__vt$8ofstream$3ios,a0@(28)
	movel d2,sp@-
	pea a6@(-292)
	jbsr ___ls__FR7ostreamR11ColorEditor
	addqw #8,sp
	movel a6@(-296),a0
	movel #__vt$8ofstream$3ios,a0@(28)
	movel a2@,a0
	movel #__vt$7ostream$3ios,a0@(28)
	movel a6@(-296),a0
	movel #__vt$11fstreambase$3ios,a0@(28)
	movel a6@(-292),a0
	movel #__vt$3ios,a0@(28)
	tstw a0@(14)
	jlt L1470
	movel a0@,a2
	tstl a2
	jeq L1470
	movel a2@(76),a0
	pea 3:w
	movew a0@(48),a1
	pea a1@(a2:l)
	movel a0@(52),a0
	jbsr a0@
L1470:
	moveml a6@(-308),#0xc04
	unlk a6
	rts
	.even
.globl _quit__11ColorEditorR6Button
_quit__11ColorEditorR6Button:
	link a6,#0
	movel a6@(8),a0
	moveq #1,d0
	movel d0,a0@(2552)
	unlk a6
	rts
	.even
.globl _undo__11ColorEditorR6Button
_undo__11ColorEditorR6Button:
	link a6,#0
	movel a6@(8),a1
	movel a1@(2548),a0
	lea a1@(a0:l:4),a0
	movel a0@(500),a0@(1524)
	pea 256:w
	pea a1@(1524)
	movel a1@(432),sp@-
	jbsr _setColorMap__17GraphicControllerPli
	unlk a6
	rts
	.even
.globl _adjustColors__11ColorEditorR6Button
_adjustColors__11ColorEditorR6Button:
	link a6,#0
	movel a3,sp@-
	movel a2,sp@-
	movel a6@(8),a2
	movel a6@(12),d0
	movel d0,d1
	cmpl a2@(392),d0
	jne L1522
	movel a2@(2548),d0
	lea a2@(d0:l:4),a3
	movel a3,d0
	movel d0,a0
	addw #1524,a0
	cmpb #255,a0@(3)
	jeq L1525
	addql #1,a0@
	jra L1525
L1522:
	cmpl a2@(400),d0
	jne L1526
	movel a2@(2548),d0
	lea a2@(d0:l:4),a3
	movel a3,d0
	movel d0,a0
	addw #1524,a0
	movel a0@,d0
	andl #65280,d0
	cmpl #65280,d0
	jeq L1525
	addl #256,a0@
	jra L1525
L1526:
	cmpl a2@(408),d0
	jne L1530
	movel a2@(2548),d0
	lea a2@(d0:l:4),a3
	movel a3,d0
	movel d0,a0
	addw #1524,a0
	movel a0@,d0
	andl #16711680,d0
	cmpl #16711680,d0
	jeq L1525
	addl #65536,a0@
	jra L1525
L1530:
	cmpl a2@(396),d0
	jne L1534
	movel a2@(2548),a0
	lea a2@(a0:l:4),a0
	lea a0@(1524),a1
	tstb a0@(1527)
	jeq L1525
	subql #1,a1@
	jra L1525
L1534:
	cmpl a2@(404),d0
	jne L1538
	movel a2@(2548),a0
	lea a2@(a0:l:4),a0
	lea a0@(1524),a1
	movew a0@(1526),d0
	andw #-256,d0
	jeq L1525
	addl #-256,a1@
	jra L1525
L1538:
	cmpl a2@(412),d1
	jne L1525
	movel a2@(2548),d0
	lea a2@(d0:l:4),a3
	movel a3,d0
	movel d0,a0
	addw #1524,a0
	movel a0@,d0
	andl #16711680,d0
	jeq L1525
	addl #-65536,a0@
L1525:
	movel a2@(2548),a0
	lea a2@(a0:l:4),a0
	movel a0@(1524),a0@(500)
	pea 256:w
	pea a2@(1524)
	movel a2@(432),sp@-
	jbsr _setColorMap__17GraphicControllerPli
	movel a6@(-8),a2
	movel a6@(-4),a3
	unlk a6
	rts
	.even
.globl _lines__11ColorEditorR4CBoxUc
_lines__11ColorEditorR4CBoxUc:
	link a6,#0
	moveml #0x3f20,sp@-
	movel a6@(8),d5
	movel a6@(12),a2
	moveb a6@(19),d4
	movel a2,sp@-
	jbsr _upperLeft__3Box
	movel d0,a0
	addqw #4,sp
	movel a0@,d2
	movew #65535,d1
	movew a0@,d0
	addw d1,d0
	bfins d0,d2{#0:#16}
	addw d1,d2
	movel a2@(12),d3
	addql #1,d3
	movel a2@(8),d0
	addql #1,d0
	bfextu d2{#0:#16},d1
	addw d0,d1
	bfins d1,d7{#0:#16}
	andl #0xFF,d4
	movel d4,sp@-
	movel d7,sp@-
	movel d2,sp@-
	addl #436,d5
	movel d5,sp@-
	lea _line__6WindowG11singlePointT1Uc,a2
	jbsr a2@
	clrl d6
	movew d2,d6
	addw d3,d2
	addw d3,d7
	movel d4,sp@-
	movel d7,sp@-
	movel d2,sp@-
	movel d5,sp@-
	jbsr a2@
	movew d6,d2
	movel d7,d6
	clrw d6
	swap d6
	bfextu d2{#0:#16},d0
	bfins d0,d7{#0:#16}
	addw #28,sp
	movel d4,sp@
	movel d7,sp@-
	movel d2,sp@-
	movel d5,sp@-
	jbsr a2@
	bfins d6,d7{#0:#16}
	bfextu d7{#0:#16},d0
	bfins d0,d2{#0:#16}
	movel d4,sp@-
	movel d7,sp@-
	movel d2,sp@-
	movel d5,sp@-
	jbsr a2@
	moveml a6@(-28),#0x4fc
	unlk a6
	rts
	.even
.globl __$_11ColorEditor
__$_11ColorEditor:
	link a6,#0
	moveml #0x2030,sp@-
	movel a6@(8),a3
	movel #__vt$11ColorEditor,a3@(4)
	movel a3@(392),sp@-
	lea ___builtin_delete,a2
	jbsr a2@
	movel a3@(396),sp@-
	jbsr a2@
	movel a3@(400),sp@-
	jbsr a2@
	movel a3@(404),sp@-
	jbsr a2@
	movel a3@(408),sp@-
	jbsr a2@
	movel a3@(412),sp@-
	jbsr a2@
	movel a3@(416),sp@-
	jbsr a2@
	movel a3@(420),sp@-
	jbsr a2@
	addw #28,sp
	movel a3@(428),sp@
	jbsr a2@
	movel a3@(424),sp@-
	jbsr a2@
	moveq #96,d2
	subl a2,a2
	addqw #8,sp
L1553:
	movel a3@(8,a2:l:4),sp@-
	jbsr ___builtin_delete
	addqw #4,sp
	addqw #1,a2
	cmpl a2,d2
	jgt L1553
	pea a3@(436)
	jbsr _clear__6Window
	addqw #4,sp
	btst #0,a6@(15)
	jeq L1557
	movel a3,sp@-
	jbsr ___builtin_delete
L1557:
	moveml a6@(-12),#0xc04
	unlk a6
	rts
	.even
.globl ___ls__FR7ostreamR11ColorEditor
___ls__FR7ostreamR11ColorEditor:
	link a6,#0
	moveml #0x3820,sp@-
	movel a6@(8),d4
	movel a6@(12),a2
	addw #1524,a2
	movel #256,d3
	clrl d2
L1562:
	movel a2@+,sp@-
	movel d4,sp@-
	jbsr ___ls__7ostreaml
	addqw #8,sp
	addql #1,d2
	cmpl d2,d3
	jgt L1562
	moveml a6@(-16),#0x41c
	unlk a6
	rts
	.even
.globl ___rs__FR7istreamR11ColorEditor
___rs__FR7istreamR11ColorEditor:
	link a6,#0
	moveml #0x3830,sp@-
	movel a6@(8),d4
	movel a6@(12),a3
	lea a3@(1524),a2
	movel #256,d3
	clrl d2
L1568:
	movel a2,sp@-
	movel d4,sp@-
	jbsr ___rs__7istreamRl
	addqw #8,sp
	addql #1,d2
	addqw #4,a2
	cmpl d2,d3
	jgt L1568
	pea 1024:w
	pea a3@(1524)
	pea a3@(500)
	jbsr _memcpy
	moveml a6@(-20),#0xc1c
	unlk a6
	rts
	.even
.globl _colors
_colors:
	link a6,#-2700
	movel a2,sp@-
	movel d2,sp@-
	movel a6@(8),d2
	pea 250:w
	pea 2048:w
	pea 1024:w
	pea 1280:w
	movel #-2147483648,sp@-
	lea a6@(-144),a2
	movel a2,sp@-
	jbsr ___17GraphicControllerPvUsUsUsUc
	movel d2,sp@-
	movel a2,sp@-
	lea a6@(-2700),a2
	movel a2,sp@-
	jbsr ___11ColorEditorR17GraphicControllerPc
	addw #32,sp
	movel a2,sp@
	jbsr _run__11ColorEditor
	pea 2:w
	movel a2,sp@-
	jbsr __$_11ColorEditor
	movel a6@(-2708),d2
	movel a6@(-2704),a2
	unlk a6
	rts
	.even
.globl __GLOBAL_$I$__3BoxR6WindowG11singlePointiiUc
__GLOBAL_$I$__3BoxR6WindowG11singlePointiiUc:
	link a6,#0
	unlk a6
	rts
	.even
__vt$11ColorEditor:
	.word 0
	.word 0
	.long 1
	.word 0
	.word 0
	.long _doCallback__11ColorEditorR5Actor
	.even
__vt$8Callback:
	.word 0
	.word 0
	.long 1
	.word 0
	.word 0
	.long _doCallback__8CallbackR5Actor
	.even
_upperLeft__3Box:
	link a6,#0
	movel a6@(8),a0
	movel a0@(4),LF0
	movel #LF0,d0
	unlk a6
	rts
	.even
_doCallback__8CallbackR5Actor:
	link a6,#0
	unlk a6
	rts
.lcomm _junk,2
