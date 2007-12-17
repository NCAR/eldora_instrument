VERSION 6
BEGIN SCHEMATIC
    BEGIN ATTR DeviceFamilyName "virtex2"
        DELETE all:0
        EDITNAME all:0
        EDITTRAIT all:0
    END ATTR
    BEGIN NETLIST
        SIGNAL HAClk
        SIGNAL AddrOut(31:0)
        SIGNAL AckIn
        SIGNAL MS0Out_l
        SIGNAL WrLOut_l
        SIGNAL WrHOut_l
        SIGNAL UserClk
        SIGNAL BusClk
        SIGNAL LinkClk
        SIGNAL HBG_l
        SIGNAL L2TxBP
        SIGNAL L2TxFIFODat(7:0)
        SIGNAL L2TxWrEn
        SIGNAL L2RxBP
        SIGNAL L2RxDataValid
        SIGNAL L2RxFIFODat(7:0)
        SIGNAL L3TxFIFODat(7:0)
        SIGNAL L3TxWrEn
        SIGNAL L3RxBP
        SIGNAL L3RxFIFODat(7:0)
        SIGNAL L3RxDataValid
        SIGNAL L3TxBP
        SIGNAL L3L2FIFODatIn(15:0)
        SIGNAL L2L3FIFODatIn(15:0)
        SIGNAL L2L3FIFODatOut(15:0)
        SIGNAL L3L2FIFODatOut(15:0)
        SIGNAL L3L2FIFOWr
        SIGNAL L3L2FIFORd
        SIGNAL L2L3FIFORd
        SIGNAL L2TxDat(7:0)
        SIGNAL L2RxDat(7:0)
        SIGNAL xv2_l2dat(7:0)
        SIGNAL L2TxEn
        SIGNAL L2ClkOut
        SIGNAL L2ClkIn
        SIGNAL L3TxDat(7:0)
        SIGNAL L3ClkOut
        SIGNAL L3ClkIn
        SIGNAL L3RxDat(7:0)
        SIGNAL L3TxEn
        SIGNAL xv2_l3dat(7:0)
        SIGNAL RdLIn_l
        SIGNAL RdHIn_l
        SIGNAL WrLIn_l
        SIGNAL WrHIn_l
        SIGNAL AddrIn(31:0)
        SIGNAL DataInReg(63:0)
        SIGNAL BML2SAddrWr
        SIGNAL BMS2LAddrWr
        SIGNAL BMS2LCntWr
        SIGNAL AckOE_l
        SIGNAL RdLOut_l
        SIGNAL L2L3FIFOWr
        SIGNAL L1TxBP
        SIGNAL L1TxFIFODat(7:0)
        SIGNAL L1TxWrEn
        SIGNAL L3L2FIFOCnt(7:0)
        SIGNAL L2L3FIFOCnt(7:0)
        SIGNAL L0RxDataValid
        SIGNAL L0RxFIFODat(7:0)
        SIGNAL L0RxBP
        SIGNAL xv2_l0dat(7:0)
        SIGNAL L0TxEn
        SIGNAL L0ClkOut
        SIGNAL L0ClkIn
        SIGNAL L0TxDat(7:0)
        SIGNAL L0RxDat(7:0)
        SIGNAL xv2_l1dat(7:0)
        SIGNAL L1TxEn
        SIGNAL L1ClkIn
        SIGNAL L1ClkOut
        SIGNAL L1TxDat(7:0)
        SIGNAL L1RxDat(7:0)
        SIGNAL SDL1FIFORdCnt(8:0)
        SIGNAL SDL1FIFODatOut(63:0)
        SIGNAL SDL1FIFOWr
        SIGNAL SDL1FIFORd
        SIGNAL SDL1FIFODatIn(63:0)
        SIGNAL L0SDFIFOWrCnt(8:0)
        SIGNAL L0SDFIFOWr
        SIGNAL L0SDFIFODatIn(63:0)
        SIGNAL L0SDFIFODatOut(63:0)
        SIGNAL L0SDFIFORd
        SIGNAL XLXN_3746
        SIGNAL XLXN_3748
        SIGNAL XLXN_3750
        SIGNAL XLXN_3751
        SIGNAL XLXN_3753
        SIGNAL XLXN_3757
        SIGNAL XLXN_3758
        SIGNAL XLXN_3762
        SIGNAL XLXN_3763
        SIGNAL XLXN_3764
        SIGNAL XLXN_3765
        SIGNAL XLXN_1071
        SIGNAL XLXN_1115
        SIGNAL XLXN_1429
        SIGNAL RdHOut_l
        SIGNAL BrstOut
        SIGNAL L0SDFIFORdCnt(8:0)
        SIGNAL SDL1FIFOWrCnt(8:0)
        SIGNAL L0SDBusInUse
        SIGNAL SlDataHOE_l
        SIGNAL SlDataLOE_l
        SIGNAL DataLOE_l
        SIGNAL BusCntlOE_l
        SIGNAL DataHOE_l
        SIGNAL SDL1BusGnt
        SIGNAL Hdr2IOEnRegWr
        SIGNAL Hdr2IORWRegWr
        SIGNAL Hdr1IOEnRegWr
        SIGNAL Hdr1IORWRegWr
        SIGNAL FPIOEnRegWrH
        SIGNAL FPIOEnRegWrL
        SIGNAL FPIORWRegWrH
        SIGNAL FPIORWRegWrL
        SIGNAL MiscRegWr
        SIGNAL Hdr1IOIn(16)
        SIGNAL Hdr2IOIn(16)
        SIGNAL xsw_usr(3:0)
        SIGNAL UserSw(3:0)
        SIGNAL xuser_led_1
        SIGNAL xuser_led_2
        SIGNAL UserLED(0)
        SIGNAL UserLED(1)
        SIGNAL xpa_scl
        SIGNAL XLXN_4369
        SIGNAL PASClIn
        SIGNAL PASDaOE_l
        SIGNAL XLXN_4372
        SIGNAL PASDaIn
        SIGNAL xpa_sda
        SIGNAL xflag1
        SIGNAL xflag0
        SIGNAL Hdr1IOOut(15:0)
        SIGNAL xhdr_d(15:00)
        SIGNAL Hdr1IOIn(15:0)
        SIGNAL Hdr1IOOE_l(15:0)
        SIGNAL Hdr1IOOE_l(17)
        SIGNAL Hdr1IOOut(17)
        SIGNAL Hdr1IOIn(17)
        SIGNAL xhdr_ack
        SIGNAL Hdr1IOOE_l(16)
        SIGNAL Hdr1IOOut(16)
        SIGNAL xhdr_clk
        SIGNAL Hdr1IOOE_l(18)
        SIGNAL Hdr1IOOut(18)
        SIGNAL Hdr1IOIn(18)
        SIGNAL Hdr2IOOut(15:0)
        SIGNAL Hdr2IOIn(15:0)
        SIGNAL Hdr2IOOE_l(15:0)
        SIGNAL Hdr2IOOE_l(17)
        SIGNAL Hdr2IOOut(17)
        SIGNAL Hdr2IOIn(17)
        SIGNAL Hdr2IOOE_l(16)
        SIGNAL Hdr2IOOut(16)
        SIGNAL Hdr2IOOE_l(18)
        SIGNAL Hdr2IOOut(18)
        SIGNAL Hdr2IOIn(18)
        SIGNAL xhdr2_d(15:00)
        SIGNAL xhdr2_ack
        SIGNAL xhdr2_dir
        SIGNAL xhdr2_clk
        SIGNAL xhdr_dir
        SIGNAL HBGEn_l
        SIGNAL PASClOE_l
        SIGNAL L0SDEF
        SIGNAL L0SDFF
        SIGNAL SDL1EF
        SIGNAL SDL1FF
        SIGNAL L2L3EF
        SIGNAL L2L3FF
        SIGNAL L3L2EF
        SIGNAL L3L2FF
        SIGNAL XLXN_5036
        SIGNAL FPIOOE_l(63:32)
        SIGNAL FPIOOut(63:32)
        SIGNAL FPIOIn(63:32)
        SIGNAL HBGSyncPre
        SIGNAL PreHBG_l
        SIGNAL xexep_n1
        SIGNAL FPIOCorner(2)
        SIGNAL xexep_n34
        SIGNAL FPIOCorner(3)
        SIGNAL FPIOOE_l(31:0)
        SIGNAL FPIOOut(31:0)
        SIGNAL FPIOIn(31:0)
        SIGNAL xexep_p1
        SIGNAL FPIOCorner(0)
        SIGNAL xexep_p34
        SIGNAL FPIOCorner(1)
        SIGNAL FPIOOut(63:0)
        SIGNAL FPIOOE_l(63:0)
        SIGNAL Hdr1IOOut(18:0)
        SIGNAL Hdr1IOOE_l(18:0)
        SIGNAL Hdr2IOOut(18:0)
        SIGNAL Hdr2IOOE_l(18:0)
        SIGNAL FPIOIn(63:0)
        SIGNAL Hdr2IOIn(18:0)
        SIGNAL Hdr1IOIn(18:0)
        SIGNAL MiscRegDatIn(31:0)
        SIGNAL BML2SAddrDatIn(31:0)
        SIGNAL BMS2LAddrDatIn(31:0)
        SIGNAL BMS2LCntDatIn(31:0)
        SIGNAL FPIORWDatInL(31:0)
        SIGNAL FPIORWDatInH(31:0)
        SIGNAL FPIOEnDatInL(31:0)
        SIGNAL FPIOEnDatInH(31:0)
        SIGNAL Hdr1IORWDatIn(31:0)
        SIGNAL Hdr1IOEnDatIn(31:0)
        SIGNAL Hdr2IORWDatIn(31:0)
        SIGNAL Hdr2IOEnDatIn(31:0)
        SIGNAL Hdr2IOEnDatOut(31:0)
        SIGNAL Hdr2IORWDatOut(31:0)
        SIGNAL Hdr1IOEnDatOut(31:0)
        SIGNAL Hdr1IORWDatOut(31:0)
        SIGNAL FPIOEnDatOutH(31:0)
        SIGNAL FPIOEnDatOutL(31:0)
        SIGNAL FPIORWDatOutH(31:0)
        SIGNAL BMS2LCntDatOut(31:0)
        SIGNAL BMS2LAddrDatOut(31:0)
        SIGNAL MiscRegDatOut(31:0)
        SIGNAL StatRegDatOut(31:0)
        SIGNAL BML2SAddrDatOut(31:0)
        SIGNAL RegDataOut(63:0)
        SIGNAL FPIORWDatOutL(31:0)
        SIGNAL xha_brst
        SIGNAL AckOut
        SIGNAL xha_ack
        SIGNAL DataIn(31:0)
        SIGNAL DataOut(31:0)
        SIGNAL xha_d(31:0)
        SIGNAL xha_a(31:0)
        SIGNAL DataIn(63:32)
        SIGNAL DataOut(63:32)
        SIGNAL xha_d(63:32)
        SIGNAL xha_d(63:0)
        SIGNAL xha_rdl_l
        SIGNAL xha_rdh_l
        SIGNAL xha_wrl_l
        SIGNAL xha_wrh_l
        SIGNAL XLXN_1211
        SIGNAL xha_ms1_l
        SIGNAL xha_ms2_l
        SIGNAL xha_ms3_l
        SIGNAL xha_ms0_l
        SIGNAL L2SDataOut(63:0)
        SIGNAL L2SAddrOut(31:0)
        SIGNAL S2LAddrOut(31:0)
        SIGNAL L2SMS0Out_l
        SIGNAL S2LMS0Out_l
        SIGNAL FPIOCorner(3:0)
        SIGNAL UserLED(1:0)
        SIGNAL L2SBrstOut
        SIGNAL S2LBrstOut
        SIGNAL L0SDBusGnt
        SIGNAL SDL1BusReq
        SIGNAL SDL1BusInUse
        SIGNAL L0SDBusEn(63:0)
        SIGNAL SDL1BusEnAr(31:0)
        SIGNAL XLXN_6091(7:0)
        SIGNAL L0SDBusReq
        SIGNAL DataIn(63:0)
        SIGNAL xv2_hbgen_l
        SIGNAL XLXN_6109
        SIGNAL XLXN_6104
        SIGNAL HBRReg_l
        SIGNAL xv2_hbg_l
        SIGNAL TmpEn(7:0)
        SIGNAL SDL1BusEn,SDL1BusEn,SDL1BusEn,SDL1BusEn,SDL1BusEn,SDL1BusEn,SDL1BusEn,SDL1BusEn
        SIGNAL TmpEn(7:0),TmpEn(7:0),TmpEn(7:0),TmpEn(7:0)
        SIGNAL xha_hbr_l
        SIGNAL HBR_l
        SIGNAL SDL1BusEn
        SIGNAL DataOut(63:0)
        SIGNAL XLXN_6122
        SIGNAL xv2_l2ack
        SIGNAL xv2_l2clk
        SIGNAL xv2_l3ack
        SIGNAL xv2_l3clk
        SIGNAL xv2_l0ack
        SIGNAL xv2_l0clk1
        SIGNAL xv2_l1ack
        SIGNAL xv2_l1clk1
        SIGNAL TimerAddrOut(31:0)
        SIGNAL TimerDataOut(31:0)
        SIGNAL TimerAddrWr
        SIGNAL TimerAddrIn(31:0)
        SIGNAL TimerDataWr
        SIGNAL TimerDataIn(31:0)
        SIGNAL WR
        SIGNAL AddrOutTimer(2:0)
        SIGNAL TimerClk
        SIGNAL GlobalEnable
        SIGNAL AddrTrigger
        SIGNAL OnePPS
        SIGNAL XLXN_6518(31:0)
        SIGNAL UserClkFB
        SIGNAL XLXN_1326
        SIGNAL XLXN_3759
        SIGNAL XLXN_3761
        SIGNAL XLXN_6556
        SIGNAL XLXN_6557
        SIGNAL XLXN_1428
        SIGNAL XLXN_6551
        SIGNAL xv2_reset_l
        SIGNAL XLXN_703
        SIGNAL xv2_haclk
        SIGNAL xuser_clk
        SIGNAL xexep_n6
        SIGNAL TimerClkIn
        SIGNAL Reset
        SIGNAL Timer_Rst
        SIGNAL TimerSelect(19:0)
        SIGNAL xexep_p7
        SIGNAL XLXN_6437
        SIGNAL xexep_n8
        SIGNAL XLXN_6497
        SIGNAL xexep_p6
        SIGNAL XLXN_6479
        SIGNAL xexep_p30
        SIGNAL XLXN_6480
        SIGNAL xexep_n29
        SIGNAL XLXN_6501
        SIGNAL xexep_p27
        SIGNAL XLXN_6655
        SIGNAL TimerSelect(1)
        SIGNAL xexep_p28
        SIGNAL XLXN_6662
        SIGNAL TimerSelect(6)
        SIGNAL xexep_n21
        SIGNAL XLXN_7137
        SIGNAL TimerSelect(11)
        SIGNAL xexep_n28
        SIGNAL XLXN_7437
        SIGNAL TimerSelect(16)
        SIGNAL xexep_p20
        SIGNAL XLXN_7473
        SIGNAL TimerSelect(2)
        SIGNAL xexep_n27
        SIGNAL XLXN_6837
        SIGNAL TimerSelect(7)
        SIGNAL xexep_p16
        SIGNAL XLXN_7149
        SIGNAL TimerSelect(12)
        SIGNAL xexep_p26
        SIGNAL XLXN_7449
        SIGNAL TimerSelect(17)
        SIGNAL XLXN_7485
        SIGNAL xexep_n16
        SIGNAL TimerSelect(3)
        SIGNAL xexep_p25
        SIGNAL XLXN_7161
        SIGNAL XLXN_7185
        SIGNAL xexep_n15
        SIGNAL TimerSelect(8)
        SIGNAL TimerSelect(13)
        SIGNAL xexep_n25
        SIGNAL XLXN_7497
        SIGNAL XLXN_7522
        SIGNAL xexep_p14
        SIGNAL TimerSelect(18)
        SIGNAL xexep_n22
        SIGNAL xexep_p29
        SIGNAL xexep_p22
        SIGNAL XLXN_7461
        SIGNAL TimerSelect(15)
        SIGNAL XLXN_7425
        SIGNAL TimerSelect(10)
        SIGNAL XLXN_7125
        SIGNAL TimerSelect(5)
        SIGNAL XLXN_6666
        SIGNAL xexep_n30
        SIGNAL TimerSelect(0)
        SIGNAL TimerSelect(4)
        SIGNAL xexep_n24
        SIGNAL XLXN_7173
        SIGNAL XLXN_7197
        SIGNAL xexep_p13
        SIGNAL TimerSelect(9)
        SIGNAL TimerSelect(14)
        SIGNAL xexep_p23
        SIGNAL XLXN_7509
        SIGNAL XLXN_7510
        SIGNAL xexep_n13
        SIGNAL TimerSelect(19)
        PORT BiDirectional xv2_l2dat(7:0)
        PORT BiDirectional xv2_l3dat(7:0)
        PORT BiDirectional xv2_l0dat(7:0)
        PORT BiDirectional xv2_l1dat(7:0)
        PORT Input xsw_usr(3:0)
        PORT Output xuser_led_1
        PORT Output xuser_led_2
        PORT BiDirectional xpa_scl
        PORT BiDirectional xpa_sda
        PORT Output xflag1
        PORT Output xflag0
        PORT BiDirectional xhdr_d(15:00)
        PORT BiDirectional xhdr_ack
        PORT BiDirectional xhdr_clk
        PORT BiDirectional xhdr2_d(15:00)
        PORT BiDirectional xhdr2_ack
        PORT BiDirectional xhdr2_dir
        PORT BiDirectional xhdr2_clk
        PORT BiDirectional xhdr_dir
        PORT Input xexep_n1
        PORT Input xexep_n34
        PORT Input xexep_p1
        PORT Input xexep_p34
        PORT BiDirectional xha_brst
        PORT BiDirectional xha_ack
        PORT BiDirectional xha_a(31:0)
        PORT BiDirectional xha_d(63:0)
        PORT BiDirectional xha_rdl_l
        PORT BiDirectional xha_rdh_l
        PORT BiDirectional xha_wrl_l
        PORT BiDirectional xha_wrh_l
        PORT BiDirectional xha_ms1_l
        PORT BiDirectional xha_ms2_l
        PORT BiDirectional xha_ms3_l
        PORT BiDirectional xha_ms0_l
        PORT Output xv2_hbgen_l
        PORT Output xv2_hbg_l
        PORT Input xha_hbr_l
        PORT Output xv2_l2ack
        PORT Input xv2_l2clk
        PORT Output xv2_l3ack
        PORT Input xv2_l3clk
        PORT Output xv2_l0ack
        PORT Input xv2_l0clk1
        PORT Output xv2_l1ack
        PORT Input xv2_l1clk1
        PORT Input xv2_reset_l
        PORT Input xv2_haclk
        PORT Input xuser_clk
        PORT Input xexep_n6
        PORT Input xexep_p7
        PORT Output xexep_n8
        PORT Output xexep_p6
        PORT Output xexep_p30
        PORT Output xexep_n29
        PORT Output xexep_p27
        PORT Output xexep_p28
        PORT Output xexep_n21
        PORT Output xexep_n28
        PORT Output xexep_p20
        PORT Output xexep_n27
        PORT Output xexep_p16
        PORT Output xexep_p26
        PORT Output xexep_n16
        PORT Output xexep_p25
        PORT Output xexep_n15
        PORT Output xexep_n25
        PORT Output xexep_p14
        PORT Output xexep_n22
        PORT Output xexep_p29
        PORT Output xexep_p22
        PORT Output xexep_n30
        PORT Output xexep_n24
        PORT Output xexep_p13
        PORT Output xexep_p23
        PORT Output xexep_n13
        BEGIN BLOCKDEF iobuf
            TIMESTAMP 2001 11 14 15 13 3
            LINE N 224 -128 128 -128 
            LINE N 160 -64 128 -64 
            LINE N 160 -128 160 -64 
            LINE N 0 -64 64 -64 
            LINE N 96 -140 96 -192 
            LINE N 0 -192 96 -192 
            LINE N 64 -96 64 -160 
            LINE N 128 -128 64 -96 
            LINE N 64 -160 128 -128 
            LINE N 128 -96 128 -32 
            LINE N 64 -64 128 -96 
            LINE N 128 -32 64 -64 
            LINE N 0 -128 64 -128 
        END BLOCKDEF
        BEGIN BLOCKDEF ibuf
            TIMESTAMP 2001 2 2 12 37 45
            LINE N 64 0 64 -64 
            LINE N 128 -32 64 0 
            LINE N 64 -64 128 -32 
            LINE N 224 -32 128 -32 
            LINE N 0 -32 64 -32 
        END BLOCKDEF
        BEGIN BLOCKDEF vcc
            TIMESTAMP 2001 2 2 12 37 29
            LINE N 64 -32 64 -64 
            LINE N 64 0 64 -32 
            LINE N 96 -64 32 -64 
        END BLOCKDEF
        BEGIN BLOCKDEF obuf
            TIMESTAMP 2001 2 2 12 37 45
            LINE N 64 0 64 -64 
            LINE N 128 -32 64 0 
            LINE N 64 -64 128 -32 
            LINE N 0 -32 64 -32 
            LINE N 224 -32 128 -32 
        END BLOCKDEF
        BEGIN BLOCKDEF gnd
            TIMESTAMP 2001 2 2 12 37 29
            LINE N 64 -64 64 -96 
            LINE N 76 -48 52 -48 
            LINE N 68 -32 60 -32 
            LINE N 88 -64 40 -64 
            LINE N 64 -64 64 -80 
            LINE N 64 -128 64 -96 
        END BLOCKDEF
        BEGIN BLOCKDEF ibufg
            TIMESTAMP 2001 2 2 12 37 45
            LINE N 64 0 64 -64 
            LINE N 128 -32 64 0 
            LINE N 64 -64 128 -32 
            LINE N 224 -32 128 -32 
            LINE N 0 -32 64 -32 
        END BLOCKDEF
        BEGIN BLOCKDEF bufg
            TIMESTAMP 2001 2 2 12 35 54
            LINE N 64 -64 64 0 
            LINE N 128 -32 64 -64 
            LINE N 64 0 128 -32 
            LINE N 224 -32 128 -32 
            LINE N 0 -32 64 -32 
        END BLOCKDEF
        BEGIN BLOCKDEF macro_virtex2_rev2
            TIMESTAMP 2002 10 8 22 18 10
            RECTANGLE N 64 -640 320 0 
            LINE N 64 -608 0 -608 
            LINE N 64 -544 0 -544 
            LINE N 64 -480 0 -480 
            LINE N 64 -288 0 -288 
            LINE N 64 -416 0 -416 
            LINE N 64 -352 0 -352 
            LINE N 64 -224 0 -224 
            LINE N 64 -160 0 -160 
            LINE N 64 -96 0 -96 
            RECTANGLE N 0 -108 64 -84 
            LINE N 64 -32 0 -32 
            RECTANGLE N 0 -44 64 -20 
            LINE N 320 -608 384 -608 
            LINE N 320 -544 384 -544 
            LINE N 320 -480 384 -480 
            LINE N 320 -416 384 -416 
            LINE N 320 -352 384 -352 
            LINE N 320 -288 384 -288 
            LINE N 320 -224 384 -224 
            LINE N 320 -160 384 -160 
            LINE N 320 -96 384 -96 
            RECTANGLE N 320 -108 384 -84 
            LINE N 320 -32 384 -32 
            RECTANGLE N 320 -44 384 -20 
        END BLOCKDEF
        BEGIN BLOCKDEF inv
            TIMESTAMP 2001 2 2 12 38 38
            LINE N 0 -32 64 -32 
            LINE N 224 -32 160 -32 
            LINE N 64 -64 128 -32 
            LINE N 128 -32 64 0 
            LINE N 64 0 64 -64 
            CIRCLE N 128 -48 160 -16 
        END BLOCKDEF
        BEGIN BLOCKDEF dcm
            TIMESTAMP 2001 5 18 11 1 17
            LINE N 64 -832 0 -832 
            RECTANGLE N 64 -1024 320 -64 
            LINE N 384 -960 320 -960 
            LINE N 384 -896 320 -896 
            LINE N 384 -832 320 -832 
            LINE N 384 -576 320 -576 
            LINE N 384 -768 320 -768 
            LINE N 384 -640 320 -640 
            LINE N 384 -704 320 -704 
            LINE N 384 -512 320 -512 
            LINE N 384 -448 320 -448 
            LINE N 384 -384 320 -384 
            LINE N 0 -960 64 -960 
            LINE N 0 -896 64 -896 
            LINE N 0 -384 64 -384 
            LINE N 0 -256 64 -256 
            LINE N 0 -192 64 -192 
            LINE N 0 -128 64 -128 
            LINE N 384 -128 320 -128 
            LINE N 384 -320 320 -320 
            LINE N 64 -112 80 -128 
            LINE N 80 -128 64 -144 
            RECTANGLE N 320 -336 384 -304 
            LINE N 64 -944 80 -960 
            LINE N 80 -960 64 -976 
            LINE N 320 -288 64 -288 
            LINE N 64 -880 80 -896 
            LINE N 80 -896 64 -912 
        END BLOCKDEF
        BEGIN BLOCKDEF startup_virtex2
            TIMESTAMP 2001 2 2 12 37 2
            LINE N 0 -320 64 -320 
            LINE N 0 -128 64 -128 
            LINE N 0 -256 64 -256 
            RECTANGLE N 64 -384 416 -64 
        END BLOCKDEF
        BEGIN BLOCKDEF fdp
            TIMESTAMP 2001 2 2 12 37 14
            LINE N 64 -112 80 -128 
            LINE N 80 -128 64 -144 
            RECTANGLE N 64 -320 320 -64 
            LINE N 384 -256 320 -256 
            LINE N 192 -320 192 -352 
            LINE N 192 -352 64 -352 
            LINE N 0 -256 64 -256 
            LINE N 0 -352 64 -352 
            LINE N 0 -128 64 -128 
        END BLOCKDEF
        BEGIN BLOCKDEF BiDir8
            TIMESTAMP 2002 12 27 23 36 55
            RECTANGLE N 64 -192 272 0 
            LINE N 64 -160 0 -160 
            LINE N 64 -96 0 -96 
            RECTANGLE N 0 -108 64 -84 
            LINE N 64 -32 0 -32 
            RECTANGLE N 0 -44 64 -20 
            LINE N 272 -64 336 -64 
            RECTANGLE N 272 -76 336 -52 
        END BLOCKDEF
        BEGIN BLOCKDEF BiDir32
            TIMESTAMP 2002 12 27 23 35 52
            RECTANGLE N 64 -192 272 0 
            LINE N 64 -160 0 -160 
            LINE N 64 -96 0 -96 
            RECTANGLE N 0 -108 64 -84 
            LINE N 64 -32 0 -32 
            RECTANGLE N 0 -44 64 -20 
            LINE N 272 -64 336 -64 
            RECTANGLE N 272 -76 336 -52 
        END BLOCKDEF
        BEGIN BLOCKDEF BWTitle
            TIMESTAMP 2003 2 10 19 1 56
            LINE N -176 -176 -1204 -176 
            BEGIN LINE W -1200 -212 -1200 -320 
            END LINE
            BEGIN LINE W -160 -220 -160 -320 
            END LINE
            BEGIN LINE W -1200 -320 -160 -320 
            END LINE
            LINE N -1200 -128 -160 -128 
            LINE N -1196 -220 -160 -220 
            BEGIN LINE W -416 -80 -160 -80 
            END LINE
            BEGIN LINE W -1200 -80 -416 -80 
            END LINE
            BEGIN LINE W -1200 -224 -1200 -80 
            END LINE
            BEGIN LINE W -208 -80 -216 -80 
            END LINE
            BEGIN LINE W -160 -220 -160 -80 
            END LINE
            LINE N -844 -128 -844 -80 
            LINE N -176 -176 -160 -176 
            LINE N -240 -128 -208 -128 
        END BLOCKDEF
        BEGIN BLOCKDEF sfifo1k16
            TIMESTAMP 2007 12 17 19 30 26
            RECTANGLE N 32 0 352 448 
            BEGIN LINE W 0 48 32 48 
            END LINE
            LINE N 0 112 32 112 
            LINE N 0 144 32 144 
            LINE N 0 336 32 336 
            LINE N 192 448 192 480 
            BEGIN LINE W 352 48 384 48 
            END LINE
            LINE N 352 112 384 112 
            LINE N 352 144 384 144 
            BEGIN LINE W 352 368 384 368 
            END LINE
        END BLOCKDEF
        BEGIN BLOCKDEF LtoF16
            TIMESTAMP 2003 2 11 21 4 46
            RECTANGLE N 64 -320 384 0 
            LINE N 64 -288 0 -288 
            LINE N 64 -224 0 -224 
            LINE N 64 -96 0 -96 
            RECTANGLE N 0 -108 64 -84 
            LINE N 64 -160 0 -160 
            LINE N 64 -32 0 -32 
            RECTANGLE N 0 -44 64 -20 
            LINE N 384 -288 448 -288 
            LINE N 384 -224 448 -224 
            LINE N 384 -160 448 -160 
            RECTANGLE N 384 -172 448 -148 
        END BLOCKDEF
        BEGIN BLOCKDEF F16toL
            TIMESTAMP 2003 2 11 21 5 6
            RECTANGLE N 64 -448 348 -128 
            LINE N 64 -416 0 -416 
            LINE N 64 -352 0 -352 
            LINE N 64 -288 0 -288 
            LINE N 64 -224 0 -224 
            RECTANGLE N 0 -236 64 -212 
            LINE N 64 -160 0 -160 
            RECTANGLE N 0 -172 64 -148 
            LINE N 352 -416 416 -416 
            LINE N 352 -352 416 -352 
            RECTANGLE N 352 -364 416 -340 
            LINE N 352 -288 416 -288 
        END BLOCKDEF
        BEGIN BLOCKDEF slavebusint
            TIMESTAMP 2003 2 20 20 9 58
            LINE N 64 368 0 368 
            RECTANGLE N 0 356 64 380 
            LINE N 64 448 0 448 
            RECTANGLE N 0 436 64 460 
            LINE N 64 528 0 528 
            RECTANGLE N 0 516 64 540 
            LINE N 64 608 0 608 
            RECTANGLE N 0 596 64 620 
            LINE N 64 688 0 688 
            RECTANGLE N 0 676 64 700 
            LINE N 64 768 0 768 
            RECTANGLE N 0 756 64 780 
            LINE N 64 848 0 848 
            RECTANGLE N 0 836 64 860 
            LINE N 64 928 0 928 
            RECTANGLE N 0 916 64 940 
            LINE N 64 1008 0 1008 
            RECTANGLE N 0 996 64 1020 
            LINE N 64 1088 0 1088 
            RECTANGLE N 0 1076 64 1100 
            LINE N 64 1168 0 1168 
            RECTANGLE N 0 1156 64 1180 
            LINE N 64 1248 0 1248 
            RECTANGLE N 0 1236 64 1260 
            LINE N 64 1328 0 1328 
            RECTANGLE N 0 1316 64 1340 
            LINE N 64 1408 0 1408 
            RECTANGLE N 0 1396 64 1420 
            LINE N 64 1488 0 1488 
            RECTANGLE N 0 1476 64 1500 
            LINE N 64 1568 0 1568 
            RECTANGLE N 0 1556 64 1580 
            LINE N 464 336 528 336 
            LINE N 464 416 528 416 
            LINE N 464 496 528 496 
            LINE N 464 576 528 576 
            LINE N 464 656 528 656 
            LINE N 464 736 528 736 
            LINE N 464 816 528 816 
            LINE N 464 896 528 896 
            LINE N 464 976 528 976 
            LINE N 464 1056 528 1056 
            LINE N 464 1136 528 1136 
            LINE N 464 1216 528 1216 
            LINE N 464 1296 528 1296 
            LINE N 464 1376 528 1376 
            LINE N 464 1456 528 1456 
            LINE N 464 1536 528 1536 
            LINE N 464 368 528 368 
            RECTANGLE N 464 356 528 380 
            LINE N 464 448 528 448 
            RECTANGLE N 464 436 528 460 
            LINE N 464 528 528 528 
            RECTANGLE N 464 516 528 540 
            LINE N 464 608 528 608 
            RECTANGLE N 464 596 528 620 
            LINE N 464 688 528 688 
            RECTANGLE N 464 676 528 700 
            LINE N 464 768 528 768 
            RECTANGLE N 464 756 528 780 
            LINE N 464 848 528 848 
            RECTANGLE N 464 836 528 860 
            LINE N 464 928 528 928 
            RECTANGLE N 464 916 528 940 
            LINE N 464 1008 528 1008 
            RECTANGLE N 464 996 528 1020 
            LINE N 464 1088 528 1088 
            RECTANGLE N 464 1076 528 1100 
            LINE N 464 1168 528 1168 
            RECTANGLE N 464 1156 528 1180 
            LINE N 464 1248 528 1248 
            RECTANGLE N 464 1236 528 1260 
            LINE N 464 1328 528 1328 
            RECTANGLE N 464 1316 528 1340 
            LINE N 464 1408 528 1408 
            RECTANGLE N 464 1396 528 1420 
            LINE N 464 1488 528 1488 
            RECTANGLE N 464 1476 528 1500 
            LINE N 464 1568 528 1568 
            RECTANGLE N 464 1556 528 1580 
            LINE N 464 224 528 224 
            RECTANGLE N 464 212 528 236 
            LINE N 64 32 0 32 
            RECTANGLE N 0 212 64 236 
            LINE N 448 640 448 640 
            LINE N 64 224 0 224 
            RECTANGLE N 464 260 528 284 
            LINE N 528 272 464 272 
            LINE N 64 272 0 272 
            RECTANGLE N 0 260 64 284 
            LINE N 64 0 0 0 
            LINE N 64 176 0 176 
            LINE N 64 144 0 144 
            LINE N 64 112 0 112 
            LINE N 64 80 0 80 
            LINE N 528 96 464 96 
            LINE N 528 144 464 144 
            LINE N 528 176 464 176 
            RECTANGLE N 64 -24 464 1584 
        END BLOCKDEF
        BEGIN BLOCKDEF afifo1k64
            TIMESTAMP 2007 12 17 19 32 17
            RECTANGLE N 32 0 352 448 
            LINE N 192 448 192 480 
            BEGIN LINE W 0 48 32 48 
            END LINE
            LINE N 0 144 32 144 
            LINE N 0 176 32 176 
            LINE N 0 368 32 368 
            LINE N 0 400 32 400 
            LINE N 352 48 384 48 
            BEGIN LINE W 352 176 384 176 
            END LINE
            BEGIN LINE W 352 240 384 240 
            END LINE
            LINE N 352 272 384 272 
            BEGIN LINE W 352 400 384 400 
            END LINE
        END BLOCKDEF
        BEGIN BLOCKDEF ltof64
            TIMESTAMP 2003 2 11 20 49 58
            RECTANGLE N 64 -320 384 0 
            LINE N 64 -288 0 -288 
            LINE N 64 -224 0 -224 
            LINE N 64 -96 0 -96 
            RECTANGLE N 0 -108 64 -84 
            LINE N 64 -160 0 -160 
            LINE N 64 -32 0 -32 
            RECTANGLE N 0 -44 64 -20 
            LINE N 384 -288 448 -288 
            LINE N 384 -224 448 -224 
            LINE N 384 -160 448 -160 
            RECTANGLE N 384 -172 448 -148 
        END BLOCKDEF
        BEGIN BLOCKDEF f64tol
            TIMESTAMP 2003 2 11 20 49 32
            RECTANGLE N 64 -448 348 -128 
            LINE N 64 -416 0 -416 
            LINE N 64 -352 0 -352 
            LINE N 64 -288 0 -288 
            LINE N 64 -224 0 -224 
            RECTANGLE N 0 -236 64 -212 
            LINE N 64 -160 0 -160 
            RECTANGLE N 0 -172 64 -148 
            LINE N 352 -416 416 -416 
            LINE N 352 -352 416 -352 
            RECTANGLE N 352 -364 416 -340 
            LINE N 352 -288 416 -288 
        END BLOCKDEF
        BEGIN BLOCKDEF bwInfo
            TIMESTAMP 2003 2 12 19 32 14
            LINE N -160 -48 -1200 -48 
            BEGIN LINE W -1200 -320 -160 -320 
            END LINE
            LINE N -1200 0 -160 0 
            LINE N -1196 -220 -160 -220 
            BEGIN LINE W -1200 48 -160 48 
            END LINE
            BEGIN LINE W -1200 -320 -1200 48 
            END LINE
            BEGIN LINE W -160 -320 -160 48 
            END LINE
            LINE N -844 0 -844 48 
            LINE N -240 0 -208 0 
            LINE N -360 0 -360 48 
            LINE N -448 -48 -448 0 
        END BLOCKDEF
        BEGIN BLOCKDEF SDtoF64
            TIMESTAMP 2003 2 21 1 23 42
            RECTANGLE N 64 -24 624 544 
            LINE N 64 80 0 80 
            LINE N 64 128 0 128 
            LINE N 64 0 0 0 
            LINE N 64 336 0 336 
            LINE N 64 160 0 160 
            LINE N 64 240 0 240 
            LINE N 64 32 0 32 
            LINE N 64 304 0 304 
            RECTANGLE N 0 500 64 524 
            LINE N 64 512 0 512 
            RECTANGLE N 0 292 64 316 
            LINE N 624 512 688 512 
            LINE N 624 112 688 112 
            LINE N 624 304 688 304 
            LINE N 624 368 688 368 
            LINE N 624 400 688 400 
            LINE N 624 336 688 336 
            LINE N 624 432 688 432 
            LINE N 624 80 688 80 
            LINE N 64 208 0 208 
            LINE N 624 480 688 480 
            RECTANGLE N 624 500 688 524 
            RECTANGLE N 624 292 688 316 
            RECTANGLE N 0 148 64 172 
            RECTANGLE N 0 228 64 252 
            LINE N 624 160 688 160 
            RECTANGLE N 624 148 688 172 
            LINE N 624 240 688 240 
            RECTANGLE N 624 228 688 252 
        END BLOCKDEF
        BEGIN BLOCKDEF F64toSD
            TIMESTAMP 2003 2 21 2 17 26
            RECTANGLE N 64 -32 608 496 
            LINE N 64 0 0 0 
            LINE N 64 176 0 176 
            LINE N 64 32 0 32 
            LINE N 64 208 0 208 
            LINE N 64 80 0 80 
            LINE N 64 400 0 400 
            RECTANGLE N 0 388 64 412 
            LINE N 608 464 672 464 
            LINE N 608 80 672 80 
            LINE N 608 256 672 256 
            RECTANGLE N 608 244 672 268 
            LINE N 608 320 672 320 
            LINE N 608 352 672 352 
            LINE N 608 288 672 288 
            LINE N 608 384 672 384 
            LINE N 608 416 672 416 
            RECTANGLE N 608 404 672 428 
            LINE N 608 112 672 112 
            LINE N 64 256 0 256 
            LINE N 0 464 64 464 
            RECTANGLE N 0 452 64 476 
            LINE N 64 432 0 432 
            LINE N 64 336 0 336 
            RECTANGLE N 0 196 64 220 
            RECTANGLE N 608 196 672 220 
            LINE N 608 208 672 208 
            RECTANGLE N 608 132 672 156 
            LINE N 608 144 672 144 
        END BLOCKDEF
        BEGIN BLOCKDEF and2b1
            TIMESTAMP 2001 2 2 12 38 38
            LINE N 64 -48 64 -144 
            LINE N 64 -144 144 -144 
            LINE N 144 -48 64 -48 
            ARC N 96 -144 192 -48 144 -48 144 -144 
            LINE N 256 -96 192 -96 
            LINE N 0 -128 64 -128 
            LINE N 0 -64 40 -64 
            CIRCLE N 40 -76 64 -52 
        END BLOCKDEF
        BEGIN BLOCKDEF bidir16en
            TIMESTAMP 2003 2 17 5 3 22
            RECTANGLE N 64 -192 272 0 
            LINE N 64 -160 0 -160 
            LINE N 64 -96 0 -96 
            RECTANGLE N 0 -108 64 -84 
            LINE N 64 -32 0 -32 
            RECTANGLE N 0 -44 64 -20 
            LINE N 272 -64 336 -64 
            RECTANGLE N 272 -76 336 -52 
            RECTANGLE N 0 -172 64 -148 
        END BLOCKDEF
        BEGIN BLOCKDEF bidir32en
            TIMESTAMP 2003 2 17 5 4 46
            RECTANGLE N 64 -192 272 0 
            LINE N 64 -160 0 -160 
            LINE N 64 -96 0 -96 
            RECTANGLE N 0 -108 64 -84 
            LINE N 64 -32 0 -32 
            RECTANGLE N 0 -44 64 -20 
            LINE N 272 -64 336 -64 
            RECTANGLE N 272 -76 336 -52 
            RECTANGLE N 0 -172 64 -148 
        END BLOCKDEF
        BEGIN BLOCKDEF ioregisters
            TIMESTAMP 2003 2 20 23 16 26
            LINE N 64 176 0 176 
            RECTANGLE N 0 164 64 188 
            LINE N 64 256 0 256 
            RECTANGLE N 0 244 64 268 
            LINE N 64 352 0 352 
            RECTANGLE N 0 340 64 364 
            LINE N 64 432 0 432 
            RECTANGLE N 0 420 64 444 
            LINE N 64 592 0 592 
            RECTANGLE N 0 580 64 604 
            LINE N 64 688 0 688 
            RECTANGLE N 0 676 64 700 
            LINE N 64 848 0 848 
            RECTANGLE N 0 836 64 860 
            LINE N 64 944 0 944 
            RECTANGLE N 0 932 64 956 
            LINE N 624 176 688 176 
            RECTANGLE N 624 164 688 188 
            LINE N 624 256 688 256 
            RECTANGLE N 624 244 688 268 
            LINE N 624 352 688 352 
            RECTANGLE N 624 340 688 364 
            LINE N 624 432 688 432 
            RECTANGLE N 624 420 688 444 
            LINE N 624 592 688 592 
            RECTANGLE N 624 580 688 604 
            LINE N 624 688 688 688 
            RECTANGLE N 624 676 688 700 
            LINE N 624 848 688 848 
            RECTANGLE N 624 836 688 860 
            LINE N 624 944 688 944 
            RECTANGLE N 624 932 688 956 
            LINE N 64 0 0 0 
            LINE N 64 32 0 32 
            LINE N 64 144 0 144 
            LINE N 64 224 0 224 
            LINE N 64 320 0 320 
            LINE N 64 400 0 400 
            LINE N 64 560 0 560 
            LINE N 64 768 0 768 
            LINE N 64 816 0 816 
            LINE N 64 912 0 912 
            LINE N 64 96 0 96 
            RECTANGLE N 0 84 64 108 
            LINE N 64 512 0 512 
            RECTANGLE N 0 500 64 524 
            LINE N 64 656 0 656 
            RECTANGLE N 0 756 64 780 
            LINE N 624 96 688 96 
            RECTANGLE N 624 84 688 108 
            LINE N 624 304 688 304 
            RECTANGLE N 624 292 688 316 
            LINE N 624 512 688 512 
            RECTANGLE N 624 500 688 524 
            LINE N 624 640 688 640 
            RECTANGLE N 624 628 688 652 
            LINE N 624 768 688 768 
            RECTANGLE N 624 756 688 780 
            LINE N 624 896 688 896 
            RECTANGLE N 624 884 688 908 
            RECTANGLE N 64 -36 624 976 
        END BLOCKDEF
        BEGIN BLOCKDEF miscstatregs
            TIMESTAMP 2003 2 21 2 12 44
            RECTANGLE N 64 -32 544 672 
            LINE N 64 0 0 0 
            LINE N 64 32 0 32 
            LINE N 64 80 0 80 
            LINE N 64 112 0 112 
            LINE N 64 192 0 192 
            LINE N 64 224 0 224 
            LINE N 64 272 0 272 
            LINE N 64 304 0 304 
            LINE N 64 352 0 352 
            LINE N 64 384 0 384 
            LINE N 64 432 0 432 
            LINE N 64 464 0 464 
            LINE N 64 512 0 512 
            LINE N 64 544 0 544 
            LINE N 64 592 0 592 
            RECTANGLE N 0 580 64 604 
            LINE N 64 640 0 640 
            RECTANGLE N 0 628 64 652 
            LINE N 544 512 608 512 
            LINE N 544 544 608 544 
            LINE N 544 0 608 0 
            LINE N 544 592 608 592 
            RECTANGLE N 544 580 608 604 
            LINE N 544 32 608 32 
            RECTANGLE N 0 100 64 124 
            RECTANGLE N 544 100 608 124 
            LINE N 544 112 608 112 
            RECTANGLE N 544 164 608 188 
            LINE N 544 176 608 176 
        END BLOCKDEF
        BEGIN BLOCKDEF m2_1b2
            TIMESTAMP 2001 2 2 12 39 29
            LINE N 320 -128 256 -128 
            LINE N 176 -32 96 -32 
            LINE N 176 -80 176 -32 
            LINE N 96 -64 96 -192 
            LINE N 256 -96 96 -64 
            LINE N 256 -160 256 -96 
            LINE N 96 -192 256 -160 
            CIRCLE N 56 -180 96 -140 
            LINE N 0 -160 56 -160 
            CIRCLE N 56 -116 96 -76 
            LINE N 0 -96 56 -96 
            LINE N 0 -32 96 -32 
        END BLOCKDEF
        BEGIN BLOCKDEF m2_1
            TIMESTAMP 2001 2 2 12 39 29
            LINE N 96 -64 96 -192 
            LINE N 256 -96 96 -64 
            LINE N 256 -160 256 -96 
            LINE N 96 -192 256 -160 
            LINE N 176 -32 96 -32 
            LINE N 176 -80 176 -32 
            LINE N 0 -32 96 -32 
            LINE N 320 -128 256 -128 
            LINE N 0 -96 96 -96 
            LINE N 0 -160 96 -160 
        END BLOCKDEF
        BEGIN BLOCKDEF buf
            TIMESTAMP 2001 2 2 12 35 54
            LINE N 0 -32 64 -32 
            LINE N 224 -32 128 -32 
            LINE N 64 0 128 -32 
            LINE N 128 -32 64 -64 
            LINE N 64 -64 64 0 
        END BLOCKDEF
        BEGIN BLOCKDEF and2
            TIMESTAMP 2001 2 2 12 38 38
            LINE N 0 -64 64 -64 
            LINE N 0 -128 64 -128 
            LINE N 256 -96 192 -96 
            ARC N 96 -144 192 -48 144 -48 144 -144 
            LINE N 144 -48 64 -48 
            LINE N 64 -144 144 -144 
            LINE N 64 -48 64 -144 
        END BLOCKDEF
        BEGIN BLOCKDEF ofd
            TIMESTAMP 2001 2 2 12 37 59
            RECTANGLE N 64 -320 320 -64 
            LINE N 84 -128 64 -140 
            LINE N 64 -116 84 -128 
            LINE N 384 -256 320 -256 
            LINE N 0 -256 64 -256 
            LINE N 0 -128 64 -128 
        END BLOCKDEF
        BEGIN BLOCKDEF or4
            TIMESTAMP 2001 4 25 16 50 36
            LINE N 0 -64 48 -64 
            LINE N 0 -128 64 -128 
            LINE N 0 -192 64 -192 
            LINE N 0 -256 48 -256 
            LINE N 256 -160 192 -160 
            ARC N 28 -208 204 -32 192 -160 112 -208 
            LINE N 112 -208 48 -208 
            LINE N 112 -112 48 -112 
            LINE N 48 -256 48 -208 
            LINE N 48 -64 48 -112 
            ARC N -40 -216 72 -104 48 -112 48 -208 
            ARC N 28 -288 204 -112 112 -112 192 -160 
        END BLOCKDEF
        BEGIN BLOCKDEF and4b1
            TIMESTAMP 2001 2 2 12 38 38
            LINE N 0 -64 40 -64 
            CIRCLE N 40 -76 64 -52 
            LINE N 0 -128 64 -128 
            LINE N 0 -192 64 -192 
            LINE N 0 -256 64 -256 
            LINE N 256 -160 192 -160 
            LINE N 64 -64 64 -256 
            LINE N 144 -112 64 -112 
            ARC N 96 -208 192 -112 144 -112 144 -208 
            LINE N 64 -208 144 -208 
        END BLOCKDEF
        BEGIN BLOCKDEF fd
            TIMESTAMP 2001 2 2 12 37 14
            RECTANGLE N 64 -320 320 -64 
            LINE N 0 -128 64 -128 
            LINE N 0 -256 64 -256 
            LINE N 384 -256 320 -256 
            LINE N 80 -128 64 -144 
            LINE N 64 -112 80 -128 
        END BLOCKDEF
        BEGIN BLOCKDEF and3b1
            TIMESTAMP 2001 2 2 12 38 38
            LINE N 0 -64 40 -64 
            CIRCLE N 40 -76 64 -52 
            LINE N 0 -128 64 -128 
            LINE N 0 -192 64 -192 
            LINE N 256 -128 192 -128 
            LINE N 64 -64 64 -192 
            ARC N 96 -176 192 -80 144 -80 144 -176 
            LINE N 144 -80 64 -80 
            LINE N 64 -176 144 -176 
        END BLOCKDEF
        BEGIN BLOCKDEF or2
            TIMESTAMP 2001 2 2 12 38 38
            LINE N 0 -64 64 -64 
            LINE N 0 -128 64 -128 
            LINE N 256 -96 192 -96 
            ARC N 28 -224 204 -48 112 -48 192 -96 
            ARC N -40 -152 72 -40 48 -48 48 -144 
            LINE N 112 -144 48 -144 
            ARC N 28 -144 204 32 192 -96 112 -144 
            LINE N 112 -48 48 -48 
        END BLOCKDEF
        BEGIN BLOCKDEF TimerBlock
            TIMESTAMP 2007 7 11 20 4 54
            LINE N 64 -416 0 -416 
            LINE N 336 -416 400 -416 
            LINE N 64 -320 0 -320 
            LINE N 64 -272 0 -272 
            LINE N 64 64 0 64 
            RECTANGLE N 0 -124 64 -100 
            LINE N 64 -112 0 -112 
            RECTANGLE N 0 -60 64 -36 
            LINE N 0 -48 64 -48 
            LINE N 64 -224 0 -224 
            LINE N 64 -176 0 -176 
            LINE N 64 16 0 16 
            RECTANGLE N 64 -448 336 96 
            LINE N 64 -368 0 -368 
        END BLOCKDEF
        BEGIN BLOCKDEF TimerInterface
            TIMESTAMP 2007 7 13 19 54 50
            LINE N 560 -32 624 -32 
            RECTANGLE N 560 -428 624 -404 
            LINE N 560 -416 624 -416 
            RECTANGLE N 560 -380 624 -356 
            LINE N 560 -368 624 -368 
            RECTANGLE N 560 -300 624 -276 
            LINE N 560 -288 624 -288 
            LINE N 560 -240 624 -240 
            RECTANGLE N 560 -140 624 -116 
            LINE N 560 -128 624 -128 
            LINE N 560 -192 624 -192 
            LINE N 224 -400 160 -400 
            LINE N 224 -368 160 -368 
            RECTANGLE N 160 -124 224 -100 
            LINE N 224 -112 160 -112 
            LINE N 224 -144 160 -144 
            RECTANGLE N 160 -252 224 -228 
            LINE N 224 -240 160 -240 
            LINE N 224 -272 160 -272 
            LINE N 560 -64 624 -64 
            RECTANGLE N 224 -464 560 16 
        END BLOCKDEF
        BEGIN BLOCK XLXI_735 BWTitle
            ATTR TitleFieldText "Remora IO Top"
            ATTR DateFieldText "February 10 2003"
            ATTR NameFieldText "Clocks & Reset"
        END BLOCK
        BEGIN BLOCK XLXI_736 BWTitle
            ATTR TitleFieldText "Remora IO Top"
            ATTR DateFieldText "February 10 2003"
            ATTR NameFieldText "Bus Arbitration"
        END BLOCK
        BEGIN BLOCK XLXI_737 BWTitle
            ATTR TitleFieldText "Remora IO Top"
            ATTR DateFieldText "February 10 2003"
            ATTR NameFieldText "Bus Control Signals"
        END BLOCK
        BEGIN BLOCK XLXI_738 BWTitle
            ATTR TitleFieldText "Remora IO Top"
            ATTR DateFieldText "February 10 2003"
            ATTR NameFieldText "Bus Slave Interface"
        END BLOCK
        BEGIN BLOCK XLXI_739 BWTitle
            ATTR TitleFieldText "Remora IO Top"
            ATTR DateFieldText "February 10 2003"
            ATTR NameFieldText "FIFO to/from Bus (SDRAM)"
        END BLOCK
        BEGIN BLOCK XLXI_742 BWTitle
            ATTR TitleFieldText "Remora IO Top"
            ATTR DateFieldText "February 10 2003"
            ATTR NameFieldText "Link 0 & 1"
        END BLOCK
        BEGIN BLOCK XLXI_744 BWTitle
            ATTR TitleFieldText "Remora IO Top"
            ATTR DateFieldText "February 10 2003"
            ATTR NameFieldText "Links 2 & 3 (Loopback)"
        END BLOCK
        BEGIN BLOCK XLXI_745 BWTitle
            ATTR TitleFieldText "Remora IO Top"
            ATTR DateFieldText "February 10 2003"
            ATTR NameFieldText "Link 2 & 3 Loopback FIFOs"
        END BLOCK
        BEGIN BLOCK XLXI_748 BWTitle
            ATTR TitleFieldText "Remora IO Top"
            ATTR DateFieldText "February 10 2003"
            ATTR NameFieldText "IO Registers & Front Panel IO"
        END BLOCK
        BEGIN BLOCK XLXI_749 BWTitle
            ATTR TitleFieldText "Remora IO Top"
            ATTR DateFieldText "February 10 2003"
            ATTR NameFieldText "Header Connector IO"
        END BLOCK
        BEGIN BLOCK XLXI_750 BWTitle
            ATTR TitleFieldText "Remora IO Top"
            ATTR DateFieldText "February 10 2003"
            ATTR NameFieldText "Misc & Stat Registers & Misc IO"
        END BLOCK
        BEGIN BLOCK L3L2FIFO sfifo1k16
            PIN DIN(15:0) L3L2FIFODatIn(15:0)
            PIN WR_EN L3L2FIFOWr
            PIN RD_EN L3L2FIFORd
            PIN CLK LinkClk
            PIN SINIT Reset
            PIN DOUT(15:0) L3L2FIFODatOut(15:0)
            PIN FULL L3L2FF
            PIN EMPTY L3L2EF
            PIN DATA_COUNT(7:0) L3L2FIFOCnt(7:0)
        END BLOCK
        BEGIN BLOCK L2L3FIFO sfifo1k16
            PIN DIN(15:0) L2L3FIFODatIn(15:0)
            PIN WR_EN L2L3FIFOWr
            PIN RD_EN L2L3FIFORd
            PIN CLK LinkClk
            PIN SINIT Reset
            PIN DOUT(15:0) L2L3FIFODatOut(15:0)
            PIN FULL L2L3FF
            PIN EMPTY L2L3EF
            PIN DATA_COUNT(7:0) L2L3FIFOCnt(7:0)
        END BLOCK
        BEGIN BLOCK L2L3FtoL F16toL
            PIN LinkClk LinkClk
            PIN Reset Reset
            PIN TxBP L3TxBP
            PIN FIFODat(15:0) L2L3FIFODatOut(15:0)
            PIN FIFORdCnt(7:0) L2L3FIFOCnt(7:0)
            PIN FIFORd L2L3FIFORd
            PIN LinkDat(7:0) L3TxFIFODat(7:0)
            PIN LinkWr L3TxWrEn
        END BLOCK
        BEGIN BLOCK L2L3LtoF LtoF16
            PIN LinkClk LinkClk
            PIN Reset Reset
            PIN LinkDat(7:0) L2RxFIFODat(7:0)
            PIN LinkDataValid L2RxDataValid
            PIN FIFOWrCnt(7:0) L2L3FIFOCnt(7:0)
            PIN RxBP L2RxBP
            PIN FIFOWr L2L3FIFOWr
            PIN FIFODat(15:0) L2L3FIFODatIn(15:0)
        END BLOCK
        BEGIN BLOCK L3L2FtoL F16toL
            PIN LinkClk LinkClk
            PIN Reset Reset
            PIN TxBP L2TxBP
            PIN FIFODat(15:0) L3L2FIFODatOut(15:0)
            PIN FIFORdCnt(7:0) L3L2FIFOCnt(7:0)
            PIN FIFORd L3L2FIFORd
            PIN LinkDat(7:0) L2TxFIFODat(7:0)
            PIN LinkWr L2TxWrEn
        END BLOCK
        BEGIN BLOCK L3L2LtoF LtoF16
            PIN LinkClk LinkClk
            PIN Reset Reset
            PIN LinkDat(7:0) L3RxFIFODat(7:0)
            PIN LinkDataValid L3RxDataValid
            PIN FIFOWrCnt(7:0) L3L2FIFOCnt(7:0)
            PIN RxBP L3RxBP
            PIN FIFOWr L3L2FIFOWr
            PIN FIFODat(15:0) L3L2FIFODatIn(15:0)
        END BLOCK
        BEGIN BLOCK XLXI_569 BiDir8
            PIN TriState XLXN_3763
            PIN DIN(7:0) L2TxDat(7:0)
            PIN DOUT(7:0) L2RxDat(7:0)
            PIN DIO(7:0) xv2_l2dat(7:0)
        END BLOCK
        BEGIN BLOCK XLXI_961 inv
            PIN I L2TxEn
            PIN O XLXN_3763
        END BLOCK
        BEGIN BLOCK XLXI_962 obuf
            PIN I L2ClkOut
            PIN O xv2_l2ack
        END BLOCK
        BEGIN BLOCK XLXI_469 ibuf
            PIN I xv2_l2clk
            PIN O L2ClkIn
        END BLOCK
        BEGIN BLOCK Link2 macro_virtex2_rev2
            PIN clk LinkClk
            PIN lxclkin L2ClkIn
            PIN rst Reset
            PIN verein XLXN_3746
            PIN psize XLXN_3751
            PIN txwrite L2TxWrEn
            PIN rxbp L2RxBP
            PIN rst_timeout XLXN_3746
            PIN lxdatain(7:0) L2RxDat(7:0)
            PIN txdata(7:0) L2TxFIFODat(7:0)
            PIN lxclkout L2ClkOut
            PIN lxdir L2TxEn
            PIN txbp L2TxBP
            PIN rx_vere_bad
            PIN rx_timeout
            PIN tx_ack_bad
            PIN tx_timeout
            PIN rxdatavalid L2RxDataValid
            PIN lxdataout(7:0) L2TxDat(7:0)
            PIN rxdata(7:0) L2RxFIFODat(7:0)
        END BLOCK
        BEGIN BLOCK XLXI_259 vcc
            PIN P XLXN_3751
        END BLOCK
        BEGIN BLOCK XLXI_258 gnd
            PIN G XLXN_3746
        END BLOCK
        BEGIN BLOCK Link3 macro_virtex2_rev2
            PIN clk LinkClk
            PIN lxclkin L3ClkIn
            PIN rst Reset
            PIN verein XLXN_3748
            PIN psize XLXN_3750
            PIN txwrite L3TxWrEn
            PIN rxbp L3RxBP
            PIN rst_timeout XLXN_3748
            PIN lxdatain(7:0) L3RxDat(7:0)
            PIN txdata(7:0) L3TxFIFODat(7:0)
            PIN lxclkout L3ClkOut
            PIN lxdir L3TxEn
            PIN txbp L3TxBP
            PIN rx_vere_bad
            PIN rx_timeout
            PIN tx_ack_bad
            PIN tx_timeout
            PIN rxdatavalid L3RxDataValid
            PIN lxdataout(7:0) L3TxDat(7:0)
            PIN rxdata(7:0) L3RxFIFODat(7:0)
        END BLOCK
        BEGIN BLOCK XLXI_262 gnd
            PIN G XLXN_3748
        END BLOCK
        BEGIN BLOCK XLXI_263 vcc
            PIN P XLXN_3750
        END BLOCK
        BEGIN BLOCK XLXI_585 BiDir8
            PIN TriState XLXN_3762
            PIN DIN(7:0) L3TxDat(7:0)
            PIN DOUT(7:0) L3RxDat(7:0)
            PIN DIO(7:0) xv2_l3dat(7:0)
        END BLOCK
        BEGIN BLOCK XLXI_471 ibuf
            PIN I xv2_l3clk
            PIN O L3ClkIn
        END BLOCK
        BEGIN BLOCK XLXI_995 obuf
            PIN I L3ClkOut
            PIN O xv2_l3ack
        END BLOCK
        BEGIN BLOCK XLXI_1011 inv
            PIN I L3TxEn
            PIN O XLXN_3762
        END BLOCK
        BEGIN BLOCK XLXI_565 BiDir8
            PIN TriState XLXN_3765
            PIN DIN(7:0) L0TxDat(7:0)
            PIN DOUT(7:0) L0RxDat(7:0)
            PIN DIO(7:0) xv2_l0dat(7:0)
        END BLOCK
        BEGIN BLOCK XLXI_1230 inv
            PIN I L0TxEn
            PIN O XLXN_3765
        END BLOCK
        BEGIN BLOCK XLXI_1224 obuf
            PIN I L0ClkOut
            PIN O xv2_l0ack
        END BLOCK
        BEGIN BLOCK XLXI_465 ibuf
            PIN I xv2_l0clk1
            PIN O L0ClkIn
        END BLOCK
        BEGIN BLOCK XLXI_588 BiDir8
            PIN TriState XLXN_3764
            PIN DIN(7:0) L1TxDat(7:0)
            PIN DOUT(7:0) L1RxDat(7:0)
            PIN DIO(7:0) xv2_l1dat(7:0)
        END BLOCK
        BEGIN BLOCK XLXI_1231 inv
            PIN I L1TxEn
            PIN O XLXN_3764
        END BLOCK
        BEGIN BLOCK XLXI_1225 obuf
            PIN I L1ClkOut
            PIN O xv2_l1ack
        END BLOCK
        BEGIN BLOCK XLXI_467 ibuf
            PIN I xv2_l1clk1
            PIN O L1ClkIn
        END BLOCK
        BEGIN BLOCK Link0 macro_virtex2_rev2
            PIN clk LinkClk
            PIN lxclkin L0ClkIn
            PIN rst Reset
            PIN verein XLXN_5036
            PIN psize XLXN_3753
            PIN txwrite XLXN_5036
            PIN rxbp L0RxBP
            PIN rst_timeout XLXN_5036
            PIN lxdatain(7:0) L0RxDat(7:0)
            PIN txdata(7:0) XLXN_6091(7:0)
            PIN lxclkout L0ClkOut
            PIN lxdir L0TxEn
            PIN txbp
            PIN rx_vere_bad
            PIN rx_timeout
            PIN tx_ack_bad
            PIN tx_timeout
            PIN rxdatavalid L0RxDataValid
            PIN lxdataout(7:0) L0TxDat(7:0)
            PIN rxdata(7:0) L0RxFIFODat(7:0)
        END BLOCK
        BEGIN BLOCK XLXI_1226 vcc
            PIN P XLXN_3753
        END BLOCK
        BEGIN BLOCK XLXI_1227 gnd
            PIN G XLXN_5036
        END BLOCK
        BEGIN BLOCK Link1 macro_virtex2_rev2
            PIN clk LinkClk
            PIN lxclkin L1ClkIn
            PIN rst Reset
            PIN verein XLXN_3757
            PIN psize XLXN_3758
            PIN txwrite L1TxWrEn
            PIN rxbp XLXN_3757
            PIN rst_timeout XLXN_3757
            PIN lxdatain(7:0) L1RxDat(7:0)
            PIN txdata(7:0) L1TxFIFODat(7:0)
            PIN lxclkout L1ClkOut
            PIN lxdir L1TxEn
            PIN txbp L1TxBP
            PIN rx_vere_bad
            PIN rx_timeout
            PIN tx_ack_bad
            PIN tx_timeout
            PIN rxdatavalid
            PIN lxdataout(7:0) L1TxDat(7:0)
            PIN rxdata(7:0)
        END BLOCK
        BEGIN BLOCK XLXI_1228 gnd
            PIN G XLXN_3757
        END BLOCK
        BEGIN BLOCK XLXI_1229 vcc
            PIN P XLXN_3758
        END BLOCK
        BEGIN BLOCK L0LtoF ltof64
            PIN LinkClk LinkClk
            PIN Reset Reset
            PIN LinkDat(7:0) L0RxFIFODat(7:0)
            PIN LinkDataValid L0RxDataValid
            PIN FIFOWrCnt(8:0) L0SDFIFOWrCnt(8:0)
            PIN RxBP L0RxBP
            PIN FIFOWr L0SDFIFOWr
            PIN FIFODat(63:0) L0SDFIFODatIn(63:0)
        END BLOCK
        BEGIN BLOCK L0FIFO afifo1k64
            PIN AINIT Reset
            PIN DIN(63:0) L0SDFIFODatIn(63:0)
            PIN WR_EN L0SDFIFOWr
            PIN WR_CLK LinkClk
            PIN RD_EN L0SDFIFORd
            PIN RD_CLK BusClk
            PIN FULL L0SDFF
            PIN WR_COUNT(8:0) L0SDFIFOWrCnt(8:0)
            PIN DOUT(63:0) L0SDFIFODatOut(63:0)
            PIN EMPTY L0SDEF
            PIN RD_COUNT(8:0) L0SDFIFORdCnt(8:0)
        END BLOCK
        BEGIN BLOCK L1FtoL f64tol
            PIN LinkClk LinkClk
            PIN Reset Reset
            PIN TxBP L1TxBP
            PIN FIFODat(63:0) SDL1FIFODatOut(63:0)
            PIN FIFORdCnt(8:0) SDL1FIFORdCnt(8:0)
            PIN FIFORd SDL1FIFORd
            PIN LinkDat(7:0) L1TxFIFODat(7:0)
            PIN LinkWr L1TxWrEn
        END BLOCK
        BEGIN BLOCK L1FIFO afifo1k64
            PIN AINIT Reset
            PIN DIN(63:0) SDL1FIFODatIn(63:0)
            PIN WR_EN SDL1FIFOWr
            PIN WR_CLK BusClk
            PIN RD_EN SDL1FIFORd
            PIN RD_CLK LinkClk
            PIN FULL SDL1FF
            PIN WR_COUNT(8:0) SDL1FIFOWrCnt(8:0)
            PIN DOUT(63:0) SDL1FIFODatOut(63:0)
            PIN EMPTY SDL1EF
            PIN RD_COUNT(8:0) SDL1FIFORdCnt(8:0)
        END BLOCK
        BEGIN BLOCK XLXI_1292 bwInfo
            BEGIN ATTR Revision 1
                VALUETYPE Float
            END ATTR
            ATTR NameFieldText "Remora IO Top Level Schematic"
            ATTR DateFieldText "February 10 2003"
            ATTR AuthorText "Kento"
        END BLOCK
        BEGIN BLOCK BusDCM dcm
            BEGIN ATTR CLKFX_DIVIDE 1
                VERILOG all:0 dp:1nosynth wsynop:1 wsynth:1
                VHDL all:0 gm:1nosynth wa:1 wd:1
                VALUETYPE Integer 1 32
            END ATTR
            BEGIN ATTR CLKFX_MULTIPLY 3
                VERILOG all:0 dp:1nosynth wsynop:1 wsynth:1
                VHDL all:0 gm:1nosynth wa:1 wd:1
                VALUETYPE Integer 1 32
            END ATTR
            BEGIN ATTR CLKDV_DIVIDE 1.5
                VERILOG all:0 dp:1nosynth wsynop:1 wsynth:1
                VHDL all:0 gm:1nosynth wa:1 wd:1
                VALUETYPE Float 1 16
            END ATTR
            BEGIN ATTR CLKIN_PERIOD "25 ns"
                VERILOG all:0 wsynop:1 wsynth:1
                VHDL all:0 wa:1 wd:1
            END ATTR
            PIN CLKFB BusClk
            PIN CLKIN HAClk
            PIN DSSEN XLXN_1429
            PIN PSCLK XLXN_1115
            PIN PSEN XLXN_1115
            PIN PSINCDEC XLXN_1115
            PIN RST Reset
            PIN CLK0 XLXN_1071
            PIN CLK180
            PIN CLK270
            PIN CLK2X
            PIN CLK2X180
            PIN CLK90
            PIN CLKDV
            PIN CLKFX
            PIN CLKFX180
            PIN LOCKED
            PIN PSDONE
            PIN STATUS(7:0)
        END BLOCK
        BEGIN BLOCK XLXI_854 bufg
            PIN I XLXN_1071
            PIN O BusClk
        END BLOCK
        BEGIN BLOCK XLXI_1288 gnd
            PIN G XLXN_1115
        END BLOCK
        BEGIN BLOCK XLXI_1289 gnd
            PIN G XLXN_1429
        END BLOCK
        BEGIN BLOCK XLXI_45(3:0) ibuf
            PIN I xsw_usr(3:0)
            PIN O UserSw(3:0)
        END BLOCK
        BEGIN BLOCK XLXI_47 obuf
            PIN I UserLED(1)
            PIN O xuser_led_2
        END BLOCK
        BEGIN BLOCK XLXI_48 obuf
            PIN I UserLED(0)
            PIN O xuser_led_1
        END BLOCK
        BEGIN BLOCK XLXI_1390 iobuf
            PIN I XLXN_4369
            PIN IO xpa_scl
            PIN O PASClIn
            PIN T PASClOE_l
        END BLOCK
        BEGIN BLOCK XLXI_1395 gnd
            PIN G XLXN_4369
        END BLOCK
        BEGIN BLOCK XLXI_1391 iobuf
            PIN I XLXN_4372
            PIN IO xpa_sda
            PIN O PASDaIn
            PIN T PASDaOE_l
        END BLOCK
        BEGIN BLOCK XLXI_1394 gnd
            PIN G XLXN_4372
        END BLOCK
        BEGIN BLOCK XLXI_143 obuf
            PIN I Hdr2IOIn(16)
            PIN O xflag1
        END BLOCK
        BEGIN BLOCK XLXI_142 obuf
            PIN I Hdr1IOIn(16)
            PIN O xflag0
        END BLOCK
        BEGIN BLOCK XLXI_1372 iobuf
            PIN I Hdr1IOOut(17)
            PIN IO xhdr_dir
            PIN O Hdr1IOIn(17)
            PIN T Hdr1IOOE_l(17)
        END BLOCK
        BEGIN BLOCK XLXI_43 iobuf
            PIN I Hdr1IOOut(16)
            PIN IO xhdr_ack
            PIN O Hdr1IOIn(16)
            PIN T Hdr1IOOE_l(16)
        END BLOCK
        BEGIN BLOCK XLXI_1373 iobuf
            PIN I Hdr1IOOut(18)
            PIN IO xhdr_clk
            PIN O Hdr1IOIn(18)
            PIN T Hdr1IOOE_l(18)
        END BLOCK
        BEGIN BLOCK XLXI_1417 iobuf
            PIN I Hdr2IOOut(17)
            PIN IO xhdr2_dir
            PIN O Hdr2IOIn(17)
            PIN T Hdr2IOOE_l(17)
        END BLOCK
        BEGIN BLOCK XLXI_1418 iobuf
            PIN I Hdr2IOOut(16)
            PIN IO xhdr2_ack
            PIN O Hdr2IOIn(16)
            PIN T Hdr2IOOE_l(16)
        END BLOCK
        BEGIN BLOCK XLXI_1419 iobuf
            PIN I Hdr2IOOut(18)
            PIN IO xhdr2_clk
            PIN O Hdr2IOIn(18)
            PIN T Hdr2IOOE_l(18)
        END BLOCK
        BEGIN BLOCK XLXI_1462 bidir16en
            PIN TriState(15:0) Hdr2IOOE_l(15:0)
            PIN DIN(15:0) Hdr2IOOut(15:0)
            PIN DOUT(15:0) Hdr2IOIn(15:0)
            PIN DIO(15:0) xhdr2_d(15:00)
        END BLOCK
        BEGIN BLOCK XLXI_1463 bidir16en
            PIN TriState(15:0) Hdr1IOOE_l(15:0)
            PIN DIN(15:0) Hdr1IOOut(15:0)
            PIN DOUT(15:0) Hdr1IOIn(15:0)
            PIN DIO(15:0) xhdr_d(15:00)
        END BLOCK
        BEGIN BLOCK XLXI_1601 BWTitle
            ATTR TitleFieldText "Remora IO Top"
            ATTR DateFieldText "February 10 2003"
            ATTR NameFieldText "Link 0 & 1 to/from FIFOs"
        END BLOCK
        BEGIN BLOCK XLXI_1465 bidir32en
            PIN TriState(31:0) FPIOOE_l(63:32)
            PIN DIN(31:0) FPIOOut(63:32)
            PIN DOUT(31:0) FPIOIn(63:32)
            PIN DIO(31:0)
        END BLOCK
        BEGIN BLOCK XLXI_1635 ibuf
            PIN I xexep_n1
            PIN O FPIOCorner(2)
        END BLOCK
        BEGIN BLOCK XLXI_1636 ibuf
            PIN I xexep_n34
            PIN O FPIOCorner(3)
        END BLOCK
        BEGIN BLOCK XLXI_1464 bidir32en
            PIN TriState(31:0) FPIOOE_l(31:0)
            PIN DIN(31:0) FPIOOut(31:0)
            PIN DOUT(31:0) FPIOIn(31:0)
            PIN DIO(31:0)
        END BLOCK
        BEGIN BLOCK XLXI_1634 ibuf
            PIN I xexep_p1
            PIN O FPIOCorner(0)
        END BLOCK
        BEGIN BLOCK XLXI_1633 ibuf
            PIN I xexep_p34
            PIN O FPIOCorner(1)
        END BLOCK
        BEGIN BLOCK IO_Registers ioregisters
            PIN Clk BusClk
            PIN Reset Reset
            PIN FPIORWRegWrL FPIORWRegWrL
            PIN FPIORWRegWrH FPIORWRegWrH
            PIN FPIOEnRegWrL FPIOEnRegWrL
            PIN FPIOEnRegWrH FPIOEnRegWrH
            PIN Hdr1IORWRegWr Hdr1IORWRegWr
            PIN Hdr1IOEnRegWr Hdr1IOEnRegWr
            PIN Hdr2IORWRegWr Hdr2IORWRegWr
            PIN Hdr2IOEnRegWr Hdr2IOEnRegWr
            PIN FPIOIn(63:0) FPIOIn(63:0)
            PIN Hdr1IOIn(18:0) Hdr1IOIn(18:0)
            PIN Hdr2IOIn(18:0) Hdr2IOIn(18:0)
            PIN FPIOOut(63:0) FPIOOut(63:0)
            PIN FPIOOE_l(63:0) FPIOOE_l(63:0)
            PIN Hdr1IOOut(18:0) Hdr1IOOut(18:0)
            PIN Hdr1IOOE_l(18:0) Hdr1IOOE_l(18:0)
            PIN Hdr2IOOut(18:0) Hdr2IOOut(18:0)
            PIN Hdr2IOOE_l(18:0) Hdr2IOOE_l(18:0)
            PIN FPIORWDatInL(31:0) FPIORWDatInL(31:0)
            PIN FPIORWDatInH(31:0) FPIORWDatInH(31:0)
            PIN FPIOEnDatInL(31:0) FPIOEnDatInL(31:0)
            PIN FPIOEnDatInH(31:0) FPIOEnDatInH(31:0)
            PIN Hdr1IORWDatIn(31:0) Hdr1IORWDatIn(31:0)
            PIN Hdr1IOEnDatIn(31:0) Hdr1IOEnDatIn(31:0)
            PIN Hdr2IORWDatIn(31:0) Hdr2IORWDatIn(31:0)
            PIN Hdr2IOEnDatIn(31:0) Hdr2IOEnDatIn(31:0)
            PIN FPIORWDatOutL(31:0) FPIORWDatOutL(31:0)
            PIN FPIORWDatOutH(31:0) FPIORWDatOutH(31:0)
            PIN FPIOEnDatOutL(31:0) FPIOEnDatOutL(31:0)
            PIN FPIOEnDatOutH(31:0) FPIOEnDatOutH(31:0)
            PIN Hdr1IORWDatOut(31:0) Hdr1IORWDatOut(31:0)
            PIN Hdr1IOEnDatOut(31:0) Hdr1IOEnDatOut(31:0)
            PIN Hdr2IORWDatOut(31:0) Hdr2IORWDatOut(31:0)
            PIN Hdr2IOEnDatOut(31:0) Hdr2IOEnDatOut(31:0)
        END BLOCK
        BEGIN BLOCK XLXI_119 iobuf
            PIN I BrstOut
            PIN IO xha_brst
            PIN O
            PIN T BusCntlOE_l
        END BLOCK
        BEGIN BLOCK XLXI_661 iobuf
            PIN I AckOut
            PIN IO xha_ack
            PIN O AckIn
            PIN T AckOE_l
        END BLOCK
        BEGIN BLOCK XLXI_1314 BiDir32
            PIN TriState DataLOE_l
            PIN DIN(31:0) DataOut(31:0)
            PIN DOUT(31:0) DataIn(31:0)
            PIN DIO(31:0) xha_d(31:0)
        END BLOCK
        BEGIN BLOCK XLXI_648 BiDir32
            PIN TriState BusCntlOE_l
            PIN DIN(31:0) AddrOut(31:0)
            PIN DOUT(31:0) AddrIn(31:0)
            PIN DIO(31:0) xha_a(31:0)
        END BLOCK
        BEGIN BLOCK XLXI_1313 BiDir32
            PIN TriState DataHOE_l
            PIN DIN(31:0) DataOut(63:32)
            PIN DOUT(31:0) DataIn(63:32)
            PIN DIO(31:0) xha_d(63:32)
        END BLOCK
        BEGIN BLOCK XLXI_1602 vcc
            PIN P AckOut
        END BLOCK
        BEGIN BLOCK XLXI_446 iobuf
            PIN I WrLOut_l
            PIN IO xha_wrl_l
            PIN O WrLIn_l
            PIN T BusCntlOE_l
        END BLOCK
        BEGIN BLOCK XLXI_447 iobuf
            PIN I WrHOut_l
            PIN IO xha_wrh_l
            PIN O WrHIn_l
            PIN T BusCntlOE_l
        END BLOCK
        BEGIN BLOCK XLXI_445 iobuf
            PIN I RdHOut_l
            PIN IO xha_rdh_l
            PIN O RdHIn_l
            PIN T BusCntlOE_l
        END BLOCK
        BEGIN BLOCK XLXI_444 iobuf
            PIN I RdLOut_l
            PIN IO xha_rdl_l
            PIN O RdLIn_l
            PIN T BusCntlOE_l
        END BLOCK
        BEGIN BLOCK XLXI_455 iobuf
            PIN I XLXN_1211
            PIN IO xha_ms1_l
            PIN O
            PIN T BusCntlOE_l
        END BLOCK
        BEGIN BLOCK XLXI_456 iobuf
            PIN I XLXN_1211
            PIN IO xha_ms2_l
            PIN O
            PIN T BusCntlOE_l
        END BLOCK
        BEGIN BLOCK XLXI_457 iobuf
            PIN I XLXN_1211
            PIN IO xha_ms3_l
            PIN O
            PIN T BusCntlOE_l
        END BLOCK
        BEGIN BLOCK XLXI_454 iobuf
            PIN I MS0Out_l
            PIN IO xha_ms0_l
            PIN O
            PIN T BusCntlOE_l
        END BLOCK
        BEGIN BLOCK XLXI_458 vcc
            PIN P XLXN_1211
        END BLOCK
        BEGIN BLOCK Misc_Stat_Regs miscstatregs
            PIN Clk BusClk
            PIN Reset Reset
            PIN MiscRegWr MiscRegWr
            PIN MiscRegDatIn(31:0) MiscRegDatIn(31:0)
            PIN L0SDEF L0SDEF
            PIN L0SDFF L0SDFF
            PIN SDL1EF SDL1EF
            PIN SDL1FF SDL1FF
            PIN L2L3EF L2L3EF
            PIN L2L3FF L2L3FF
            PIN L3L2EF L3L2EF
            PIN L3L2FF L3L2FF
            PIN PASClIn PASClIn
            PIN PASDaIn PASDaIn
            PIN UserSw(3:0) UserSw(3:0)
            PIN FPIOCorner(3:0) FPIOCorner(3:0)
            PIN PASClOE_l PASClOE_l
            PIN PASDaOE_l PASDaOE_l
            PIN HBGEn_l HBGEn_l
            PIN UserLED(1:0) UserLED(1:0)
            PIN HBGSyncPre HBGSyncPre
            PIN MiscRegDatOut(31:0) MiscRegDatOut(31:0)
            PIN StatRegDatOut(31:0) StatRegDatOut(31:0)
        END BLOCK
        BEGIN BLOCK SDtoFIFO SDtoF64
            PIN Clk BusClk
            PIN BMS2LAddrWr BMS2LAddrWr
            PIN Reset Reset
            PIN BMS2LCntWr BMS2LCntWr
            PIN BMS2LAddrDatIn(31:0) BMS2LAddrDatIn(31:0)
            PIN BMS2LCntDatIn(31:0) BMS2LCntDatIn(31:0)
            PIN BusGnt SDL1BusGnt
            PIN DataInReg(63:0) DataInReg(63:0)
            PIN FIFOWrCnt(8:0) SDL1FIFOWrCnt(8:0)
            PIN FIFOWr SDL1FIFOWr
            PIN BusReq SDL1BusReq
            PIN AddrOut(31:0) S2LAddrOut(31:0)
            PIN RdLOut_l RdLOut_l
            PIN RdHOut_l RdHOut_l
            PIN MS0Out_l S2LMS0Out_l
            PIN BrstOut S2LBrstOut
            PIN BusInUse SDL1BusInUse
            PIN Ack AckIn
            PIN FIFODat(63:0) SDL1FIFODatIn(63:0)
            PIN BMS2LAddrDatOut(31:0) BMS2LAddrDatOut(31:0)
            PIN BMS2LCntDatOut(31:0) BMS2LCntDatOut(31:0)
        END BLOCK
        BEGIN BLOCK FIFOtoSD F64toSD
            PIN Clk BusClk
            PIN BML2SAddrWr BML2SAddrWr
            PIN Reset Reset
            PIN BML2SAddrDatIn(31:0) BML2SAddrDatIn(31:0)
            PIN BusGnt L0SDBusGnt
            PIN FIFORdCnt(8:0) L0SDFIFORdCnt(8:0)
            PIN FIFORd L0SDFIFORd
            PIN BusReq L0SDBusReq
            PIN AddrOut(31:0) L2SAddrOut(31:0)
            PIN WrLOut_l WrLOut_l
            PIN WrHOut_l WrHOut_l
            PIN MS0Out_l L2SMS0Out_l
            PIN BrstOut L2SBrstOut
            PIN DataOut(63:0) L2SDataOut(63:0)
            PIN BusInUse L0SDBusInUse
            PIN Ack AckIn
            PIN FIFODat(63:0) L0SDFIFODatOut(63:0)
            PIN FIFOEmp L0SDEF
            PIN LinkDatVal L0RxDataValid
            PIN BML2SAddrDatOut(31:0) BML2SAddrDatOut(31:0)
            PIN BusDatEn(63:0) L0SDBusEn(63:0)
        END BLOCK
        BEGIN BLOCK XLXI_1711(7:0) gnd
            PIN G XLXN_6091(7:0)
        END BLOCK
        BEGIN BLOCK Slave_Bus_Int slavebusint
            PIN Clk BusClk
            PIN Reset Reset
            PIN RdLIn_l RdLIn_l
            PIN RdHIn_l RdHIn_l
            PIN WrLIn_l WrLIn_l
            PIN WrHIn_l WrHIn_l
            PIN AddrIn(31:0) AddrIn(31:0)
            PIN DataIn(63:0) DataIn(63:0)
            PIN AckOE_l AckOE_l
            PIN DataLOE_l SlDataLOE_l
            PIN DataHOE_l SlDataHOE_l
            PIN DataInReg(63:0) DataInReg(63:0)
            PIN Reg0DatOut(31:0) MiscRegDatOut(31:0)
            PIN Reg1DatOut(31:0) StatRegDatOut(31:0)
            PIN Reg2DatOut(31:0) TimerAddrOut(31:0)
            PIN Reg3DatOut(31:0) TimerDataOut(31:0)
            PIN Reg4DatOut(31:0) XLXN_6518(31:0)
            PIN Reg5DatOut(31:0) XLXN_6518(31:0)
            PIN Reg6DatOut(31:0) BMS2LAddrDatOut(31:0)
            PIN Reg7DatOut(31:0) BMS2LCntDatOut(31:0)
            PIN Reg8DatOut(31:0) FPIORWDatOutL(31:0)
            PIN Reg9DatOut(31:0) FPIORWDatOutH(31:0)
            PIN RegADatOut(31:0) FPIOEnDatOutL(31:0)
            PIN RegBDatOut(31:0) FPIOEnDatOutH(31:0)
            PIN RegCDatOut(31:0) Hdr1IORWDatOut(31:0)
            PIN RegDDatOut(31:0) Hdr1IOEnDatOut(31:0)
            PIN RegEDatOut(31:0) Hdr2IORWDatOut(31:0)
            PIN RegFDatOut(31:0) Hdr2IOEnDatOut(31:0)
            PIN Reg0Wr MiscRegWr
            PIN Reg1Wr
            PIN Reg2Wr TimerAddrWr
            PIN Reg3Wr TimerDataWr
            PIN Reg4Wr
            PIN Reg5Wr
            PIN Reg6Wr BMS2LAddrWr
            PIN Reg7Wr BMS2LCntWr
            PIN Reg8Wr FPIORWRegWrL
            PIN Reg9Wr FPIORWRegWrH
            PIN RegAWr FPIOEnRegWrL
            PIN RegBWr FPIOEnRegWrH
            PIN RegCWr Hdr1IORWRegWr
            PIN RegDWr Hdr1IOEnRegWr
            PIN RegEWr Hdr2IORWRegWr
            PIN RegFWr Hdr2IOEnRegWr
            PIN Reg0DatIn(31:0) MiscRegDatIn(31:0)
            PIN Reg1DatIn(31:0)
            PIN Reg2DatIn(31:0) TimerAddrIn(31:0)
            PIN Reg3DatIn(31:0) TimerDataIn(31:0)
            PIN Reg4DatIn(31:0)
            PIN Reg5DatIn(31:0)
            PIN Reg6DatIn(31:0) BMS2LAddrDatIn(31:0)
            PIN Reg7DatIn(31:0) BMS2LCntDatIn(31:0)
            PIN Reg8DatIn(31:0) FPIORWDatInL(31:0)
            PIN Reg9DatIn(31:0) FPIORWDatInH(31:0)
            PIN RegADatIn(31:0) FPIOEnDatInL(31:0)
            PIN RegBDatIn(31:0) FPIOEnDatInH(31:0)
            PIN RegCDatIn(31:0) Hdr1IORWDatIn(31:0)
            PIN RegDDatIn(31:0) Hdr1IOEnDatIn(31:0)
            PIN RegEDatIn(31:0) Hdr2IORWDatIn(31:0)
            PIN RegFDatIn(31:0) Hdr2IOEnDatIn(31:0)
            PIN RegDataOut(63:0) RegDataOut(63:0)
        END BLOCK
        BEGIN BLOCK XLXI_886 obuf
            PIN I HBGEn_l
            PIN O xv2_hbgen_l
        END BLOCK
        BEGIN BLOCK XLXI_1343 and2b1
            PIN I0 L0SDBusInUse
            PIN I1 SlDataLOE_l
            PIN O DataLOE_l
        END BLOCK
        BEGIN BLOCK XLXI_1345 and2b1
            PIN I0 L0SDBusInUse
            PIN I1 SlDataHOE_l
            PIN O DataHOE_l
        END BLOCK
        BEGIN BLOCK XLXI_1619 m2_1b2
            PIN D0 XLXN_6109
            PIN D1 HBR_l
            PIN S0 HBGEn_l
            PIN O BusCntlOE_l
        END BLOCK
        BEGIN BLOCK XLXI_1712 and2
            PIN I0 XLXN_6104
            PIN I1 HBG_l
            PIN O XLXN_6109
        END BLOCK
        BEGIN BLOCK HBGDelayDFF fdp
            PIN C BusClk
            PIN D HBG_l
            PIN PRE Reset
            PIN Q XLXN_6104
        END BLOCK
        BEGIN BLOCK XLXI_1728 fdp
            PIN C BusClk
            PIN D PreHBG_l
            PIN PRE Reset
            PIN Q HBG_l
        END BLOCK
        BEGIN BLOCK XLXI_1724 ofd
            PIN C BusClk
            PIN D PreHBG_l
            PIN Q xv2_hbg_l
        END BLOCK
        BEGIN BLOCK XLXI_1733 or4
            PIN I0 HBGSyncPre
            PIN I1 SDL1BusInUse
            PIN I2 L0SDBusInUse
            PIN I3 HBRReg_l
            PIN O PreHBG_l
        END BLOCK
        BEGIN BLOCK XLXI_1702(7:0) buf
            PIN I SDL1BusEn,SDL1BusEn,SDL1BusEn,SDL1BusEn,SDL1BusEn,SDL1BusEn,SDL1BusEn,SDL1BusEn
            PIN O TmpEn(7:0)
        END BLOCK
        BEGIN BLOCK XLXI_1701(31:0) buf
            PIN I TmpEn(7:0),TmpEn(7:0),TmpEn(7:0),TmpEn(7:0)
            PIN O SDL1BusEnAr(31:0)
        END BLOCK
        BEGIN BLOCK XLXI_1764 fd
            PIN C BusClk
            PIN D HBR_l
            PIN Q HBRReg_l
        END BLOCK
        BEGIN BLOCK XLXI_1765 ibuf
            PIN I xha_hbr_l
            PIN O HBR_l
        END BLOCK
        BEGIN BLOCK XLXI_1767 or2
            PIN I0 SDL1BusInUse
            PIN I1 SDL1BusReq
            PIN O XLXN_6122
        END BLOCK
        BEGIN BLOCK XLXI_1656(63:0) m2_1
            PIN D0 RegDataOut(63:0)
            PIN D1 L2SDataOut(63:0)
            PIN S0 L0SDBusEn(63:0)
            PIN O DataOut(63:0)
        END BLOCK
        BEGIN BLOCK XLXI_1676 m2_1
            PIN D0 L2SMS0Out_l
            PIN D1 S2LMS0Out_l
            PIN S0 SDL1BusEn
            PIN O MS0Out_l
        END BLOCK
        BEGIN BLOCK XLXI_1679 m2_1
            PIN D0 L2SBrstOut
            PIN D1 S2LBrstOut
            PIN S0 SDL1BusEn
            PIN O BrstOut
        END BLOCK
        BEGIN BLOCK XLXI_1672(31:0) m2_1
            PIN D0 L2SAddrOut(31:0)
            PIN D1 S2LAddrOut(31:0)
            PIN S0 SDL1BusEnAr(31:0)
            PIN O AddrOut(31:0)
        END BLOCK
        BEGIN BLOCK XLXI_1762 and4b1
            PIN I0 L0SDBusInUse
            PIN I1 HBG_l
            PIN I2 HBRReg_l
            PIN I3 SDL1BusReq
            PIN O SDL1BusGnt
        END BLOCK
        BEGIN BLOCK XLXI_1766 and3b1
            PIN I0 L0SDBusInUse
            PIN I1 HBG_l
            PIN I2 XLXN_6122
            PIN O SDL1BusEn
        END BLOCK
        BEGIN BLOCK XLXI_1763 and4b1
            PIN I0 XLXN_6122
            PIN I1 HBG_l
            PIN I2 HBRReg_l
            PIN I3 L0SDBusReq
            PIN O L0SDBusGnt
        END BLOCK
        BEGIN BLOCK XLXI_1873(31:0) gnd
            PIN G XLXN_6518(31:0)
        END BLOCK
        BEGIN BLOCK LinkDCM dcm
            BEGIN ATTR CLKFX_DIVIDE 1
                VERILOG all:0 dp:1nosynth wsynop:1 wsynth:1
                VHDL all:0 gm:1nosynth wa:1 wd:1
                VALUETYPE Integer 1 32
            END ATTR
            BEGIN ATTR CLKFX_MULTIPLY 3
                VERILOG all:0 dp:1nosynth wsynop:1 wsynth:1
                VHDL all:0 gm:1nosynth wa:1 wd:1
                VALUETYPE Integer 1 32
            END ATTR
            BEGIN ATTR CLKDV_DIVIDE 1.5
                VERILOG all:0 dp:1nosynth wsynop:1 wsynth:1
                VHDL all:0 gm:1nosynth wa:1 wd:1
                VALUETYPE Float 1 16
            END ATTR
            BEGIN ATTR CLKIN_PERIOD "22 ns"
                VERILOG all:0 wsynop:1 wsynth:1
                VHDL all:0 wa:1 wd:1
            END ATTR
            PIN CLKFB UserClkFB
            PIN CLKIN UserClk
            PIN DSSEN XLXN_3759
            PIN PSCLK XLXN_3761
            PIN PSEN XLXN_3761
            PIN PSINCDEC XLXN_3761
            PIN RST Reset
            PIN CLK0 UserClkFB
            PIN CLK180
            PIN CLK270
            PIN CLK2X
            PIN CLK2X180
            PIN CLK90
            PIN CLKDV
            PIN CLKFX XLXN_1326
            PIN CLKFX180
            PIN LOCKED
            PIN PSDONE
            PIN STATUS(7:0)
        END BLOCK
        BEGIN BLOCK XLXI_556 gnd
            PIN G XLXN_3759
        END BLOCK
        BEGIN BLOCK XLXI_490 bufg
            PIN I XLXN_1326
            PIN O LinkClk
        END BLOCK
        BEGIN BLOCK XLXI_424 gnd
            PIN G XLXN_3761
        END BLOCK
        BEGIN BLOCK TimerDCM dcm
            BEGIN ATTR CLKFX_DIVIDE 1
                VERILOG all:0 dp:1nosynth wsynop:1 wsynth:1
                VHDL all:0 gm:1nosynth wa:1 wd:1
                VALUETYPE Integer 1 32
            END ATTR
            BEGIN ATTR CLKFX_MULTIPLY 3
                VERILOG all:0 dp:1nosynth wsynop:1 wsynth:1
                VHDL all:0 gm:1nosynth wa:1 wd:1
                VALUETYPE Integer 1 32
            END ATTR
            BEGIN ATTR CLKDV_DIVIDE 1.5
                VERILOG all:0 dp:1nosynth wsynop:1 wsynth:1
                VHDL all:0 gm:1nosynth wa:1 wd:1
                VALUETYPE Float 1 16
            END ATTR
            BEGIN ATTR CLKIN_PERIOD "25 ns"
                VERILOG all:0 wsynop:1 wsynth:1
                VHDL all:0 wa:1 wd:1
            END ATTR
            PIN CLKFB TimerClk
            PIN CLKIN TimerClkIn
            PIN DSSEN XLXN_6557
            PIN PSCLK XLXN_6556
            PIN PSEN XLXN_6556
            PIN PSINCDEC XLXN_6556
            PIN RST Reset
            PIN CLK0 XLXN_6551
            PIN CLK180
            PIN CLK270
            PIN CLK2X
            PIN CLK2X180
            PIN CLK90
            PIN CLKDV
            PIN CLKFX
            PIN CLKFX180
            PIN LOCKED
            PIN PSDONE
            PIN STATUS(7:0)
        END BLOCK
        BEGIN BLOCK XLXI_1891 gnd
            PIN G XLXN_6556
        END BLOCK
        BEGIN BLOCK XLXI_1892 gnd
            PIN G XLXN_6557
        END BLOCK
        BEGIN BLOCK Startup startup_virtex2
            PIN CLK XLXN_1428
            PIN GSR Reset
            PIN GTS XLXN_1428
        END BLOCK
        BEGIN BLOCK XLXI_555 gnd
            PIN G XLXN_1428
        END BLOCK
        BEGIN BLOCK XLXI_1893 bufg
            PIN I XLXN_6551
            PIN O TimerClk
        END BLOCK
        BEGIN BLOCK XLXI_17 ibuf
            PIN I xv2_reset_l
            PIN O XLXN_703
        END BLOCK
        BEGIN BLOCK XLXI_307 inv
            PIN I XLXN_703
            PIN O Reset
        END BLOCK
        BEGIN BLOCK XLXI_123 ibufg
            PIN I xv2_haclk
            PIN O HAClk
        END BLOCK
        BEGIN BLOCK XLXI_124 ibufg
            PIN I xuser_clk
            PIN O UserClk
        END BLOCK
        BEGIN BLOCK XLXI_1781 ibufg
            PIN I xexep_n6
            PIN O TimerClkIn
        END BLOCK
        BEGIN BLOCK TimerIface TimerInterface
            PIN Clk BusClk
            PIN Reset Reset
            PIN AddrWr TimerAddrWr
            PIN DataWr TimerDataWr
            PIN AddrIn(31:0) TimerAddrIn(31:0)
            PIN DataIn(31:0) TimerDataIn(31:0)
            PIN DataInOut(31:0) TimerDataOut(31:0)
            PIN WR WR
            PIN GlobalEnable GlobalEnable
            PIN AddrTrigger AddrTrigger
            PIN Timer_Rst Timer_Rst
            PIN TimerAddrOut(2:0) AddrOutTimer(2:0)
            PIN AddrOut(31:0) TimerAddrOut(31:0)
            PIN TimerSelect(19:0) TimerSelect(19:0)
        END BLOCK
        BEGIN BLOCK XLXI_1837 ibufg
            PIN I xexep_p7
            PIN O XLXN_6437
        END BLOCK
        BEGIN BLOCK XLXI_1838 bufg
            PIN I XLXN_6437
            PIN O OnePPS
        END BLOCK
        BEGIN BLOCK XLXI_1858 obuf
            PIN I XLXN_6497
            PIN O xexep_n8
        END BLOCK
        BEGIN BLOCK XLXI_1855 gnd
            PIN G XLXN_6497
        END BLOCK
        BEGIN BLOCK XLXI_1848 obuf
            PIN I XLXN_6479
            PIN O xexep_p6
        END BLOCK
        BEGIN BLOCK XLXI_1846 gnd
            PIN G XLXN_6479
        END BLOCK
        BEGIN BLOCK XLXI_1849 obuf
            PIN I XLXN_6480
            PIN O xexep_p30
        END BLOCK
        BEGIN BLOCK XLXI_1847 gnd
            PIN G XLXN_6480
        END BLOCK
        BEGIN BLOCK XLXI_1860 obuf
            PIN I XLXN_6501
            PIN O xexep_n29
        END BLOCK
        BEGIN BLOCK XLXI_1859 gnd
            PIN G XLXN_6501
        END BLOCK
        BEGIN BLOCK XLXI_1924 obuf
            PIN I XLXN_6655
            PIN O xexep_p27
        END BLOCK
        BEGIN BLOCK XLXI_1925 gnd
            PIN G XLXN_6655
        END BLOCK
        BEGIN BLOCK Timer_2 TimerBlock
            PIN TimerClk TimerClk
            PIN Enable GlobalEnable
            PIN OnePPS OnePPS
            PIN CS TimerSelect(1)
            PIN AddrIn(2:0) AddrOutTimer(2:0)
            PIN BusClk BusClk
            PIN WR WR
            PIN Timer_Rst Timer_Rst
            PIN AddrTrigger AddrTrigger
            PIN DataInOut(31:0) TimerDataOut(31:0)
            PIN PulseOut XLXN_6662
        END BLOCK
        BEGIN BLOCK XLXI_1827 obuf
            PIN I XLXN_6662
            PIN O xexep_p28
        END BLOCK
        BEGIN BLOCK Timer7 TimerBlock
            PIN TimerClk TimerClk
            PIN Enable GlobalEnable
            PIN OnePPS OnePPS
            PIN CS TimerSelect(6)
            PIN AddrIn(2:0) AddrOutTimer(2:0)
            PIN BusClk BusClk
            PIN WR WR
            PIN Timer_Rst Timer_Rst
            PIN AddrTrigger AddrTrigger
            PIN DataInOut(31:0) TimerDataOut(31:0)
            PIN PulseOut XLXN_7137
        END BLOCK
        BEGIN BLOCK XLXI_2017 obuf
            PIN I XLXN_7137
            PIN O xexep_n21
        END BLOCK
        BEGIN BLOCK Timer12 TimerBlock
            PIN TimerClk TimerClk
            PIN Enable GlobalEnable
            PIN OnePPS OnePPS
            PIN CS TimerSelect(11)
            PIN AddrIn(2:0) AddrOutTimer(2:0)
            PIN BusClk BusClk
            PIN WR WR
            PIN Timer_Rst Timer_Rst
            PIN AddrTrigger AddrTrigger
            PIN DataInOut(31:0) TimerDataOut(31:0)
            PIN PulseOut XLXN_7437
        END BLOCK
        BEGIN BLOCK XLXI_2067 obuf
            PIN I XLXN_7437
            PIN O xexep_n28
        END BLOCK
        BEGIN BLOCK Timer17 TimerBlock
            PIN TimerClk TimerClk
            PIN Enable GlobalEnable
            PIN OnePPS OnePPS
            PIN CS TimerSelect(16)
            PIN AddrIn(2:0) AddrOutTimer(2:0)
            PIN BusClk BusClk
            PIN WR WR
            PIN Timer_Rst Timer_Rst
            PIN AddrTrigger AddrTrigger
            PIN DataInOut(31:0) TimerDataOut(31:0)
            PIN PulseOut XLXN_7473
        END BLOCK
        BEGIN BLOCK XLXI_2073 obuf
            PIN I XLXN_7473
            PIN O xexep_p20
        END BLOCK
        BEGIN BLOCK Timer3 TimerBlock
            PIN TimerClk TimerClk
            PIN Enable GlobalEnable
            PIN OnePPS OnePPS
            PIN CS TimerSelect(2)
            PIN AddrIn(2:0) AddrOutTimer(2:0)
            PIN BusClk BusClk
            PIN WR WR
            PIN Timer_Rst Timer_Rst
            PIN AddrTrigger AddrTrigger
            PIN DataInOut(31:0) TimerDataOut(31:0)
            PIN PulseOut XLXN_6837
        END BLOCK
        BEGIN BLOCK XLXI_1967 obuf
            PIN I XLXN_6837
            PIN O xexep_n27
        END BLOCK
        BEGIN BLOCK Timer8 TimerBlock
            PIN TimerClk TimerClk
            PIN Enable GlobalEnable
            PIN OnePPS OnePPS
            PIN CS TimerSelect(7)
            PIN AddrIn(2:0) AddrOutTimer(2:0)
            PIN BusClk BusClk
            PIN WR WR
            PIN Timer_Rst Timer_Rst
            PIN AddrTrigger AddrTrigger
            PIN DataInOut(31:0) TimerDataOut(31:0)
            PIN PulseOut XLXN_7149
        END BLOCK
        BEGIN BLOCK XLXI_2019 obuf
            PIN I XLXN_7149
            PIN O xexep_p16
        END BLOCK
        BEGIN BLOCK Timer13 TimerBlock
            PIN TimerClk TimerClk
            PIN Enable GlobalEnable
            PIN OnePPS OnePPS
            PIN CS TimerSelect(12)
            PIN AddrIn(2:0) AddrOutTimer(2:0)
            PIN BusClk BusClk
            PIN WR WR
            PIN Timer_Rst Timer_Rst
            PIN AddrTrigger AddrTrigger
            PIN DataInOut(31:0) TimerDataOut(31:0)
            PIN PulseOut XLXN_7449
        END BLOCK
        BEGIN BLOCK XLXI_2069 obuf
            PIN I XLXN_7449
            PIN O xexep_p26
        END BLOCK
        BEGIN BLOCK Timer18 TimerBlock
            PIN TimerClk TimerClk
            PIN Enable GlobalEnable
            PIN OnePPS OnePPS
            PIN CS TimerSelect(17)
            PIN AddrIn(2:0) AddrOutTimer(2:0)
            PIN BusClk BusClk
            PIN WR WR
            PIN Timer_Rst Timer_Rst
            PIN AddrTrigger AddrTrigger
            PIN DataInOut(31:0) TimerDataOut(31:0)
            PIN PulseOut XLXN_7485
        END BLOCK
        BEGIN BLOCK XLXI_2075 obuf
            PIN I XLXN_7485
            PIN O xexep_n16
        END BLOCK
        BEGIN BLOCK Timer4 TimerBlock
            PIN TimerClk TimerClk
            PIN Enable GlobalEnable
            PIN OnePPS OnePPS
            PIN CS TimerSelect(3)
            PIN AddrIn(2:0) AddrOutTimer(2:0)
            PIN BusClk BusClk
            PIN WR WR
            PIN Timer_Rst Timer_Rst
            PIN AddrTrigger AddrTrigger
            PIN DataInOut(31:0) TimerDataOut(31:0)
            PIN PulseOut XLXN_7161
        END BLOCK
        BEGIN BLOCK XLXI_2021 obuf
            PIN I XLXN_7161
            PIN O xexep_p25
        END BLOCK
        BEGIN BLOCK XLXI_2025 obuf
            PIN I XLXN_7185
            PIN O xexep_n15
        END BLOCK
        BEGIN BLOCK Timer9 TimerBlock
            PIN TimerClk TimerClk
            PIN Enable GlobalEnable
            PIN OnePPS OnePPS
            PIN CS TimerSelect(8)
            PIN AddrIn(2:0) AddrOutTimer(2:0)
            PIN BusClk BusClk
            PIN WR WR
            PIN Timer_Rst Timer_Rst
            PIN AddrTrigger AddrTrigger
            PIN DataInOut(31:0) TimerDataOut(31:0)
            PIN PulseOut XLXN_7185
        END BLOCK
        BEGIN BLOCK Timer14 TimerBlock
            PIN TimerClk TimerClk
            PIN Enable GlobalEnable
            PIN OnePPS OnePPS
            PIN CS TimerSelect(13)
            PIN AddrIn(2:0) AddrOutTimer(2:0)
            PIN BusClk BusClk
            PIN WR WR
            PIN Timer_Rst Timer_Rst
            PIN AddrTrigger AddrTrigger
            PIN DataInOut(31:0) TimerDataOut(31:0)
            PIN PulseOut XLXN_7497
        END BLOCK
        BEGIN BLOCK XLXI_2077 obuf
            PIN I XLXN_7497
            PIN O xexep_n25
        END BLOCK
        BEGIN BLOCK XLXI_2082 obuf
            PIN I XLXN_7522
            PIN O xexep_p14
        END BLOCK
        BEGIN BLOCK Timer19 TimerBlock
            PIN TimerClk TimerClk
            PIN Enable GlobalEnable
            PIN OnePPS OnePPS
            PIN CS TimerSelect(18)
            PIN AddrIn(2:0) AddrOutTimer(2:0)
            PIN BusClk BusClk
            PIN WR WR
            PIN Timer_Rst Timer_Rst
            PIN AddrTrigger AddrTrigger
            PIN DataInOut(31:0) TimerDataOut(31:0)
            PIN PulseOut XLXN_7522
        END BLOCK
        BEGIN BLOCK XLXI_2071 obuf
            PIN I XLXN_7461
            PIN O xexep_n22
        END BLOCK
        BEGIN BLOCK Timer16 TimerBlock
            PIN TimerClk TimerClk
            PIN Enable GlobalEnable
            PIN OnePPS OnePPS
            PIN CS TimerSelect(15)
            PIN AddrIn(2:0) AddrOutTimer(2:0)
            PIN BusClk BusClk
            PIN WR WR
            PIN Timer_Rst Timer_Rst
            PIN AddrTrigger AddrTrigger
            PIN DataInOut(31:0) TimerDataOut(31:0)
            PIN PulseOut XLXN_7461
        END BLOCK
        BEGIN BLOCK XLXI_2065 obuf
            PIN I XLXN_7425
            PIN O xexep_p29
        END BLOCK
        BEGIN BLOCK Timer11 TimerBlock
            PIN TimerClk TimerClk
            PIN Enable GlobalEnable
            PIN OnePPS OnePPS
            PIN CS TimerSelect(10)
            PIN AddrIn(2:0) AddrOutTimer(2:0)
            PIN BusClk BusClk
            PIN WR WR
            PIN Timer_Rst Timer_Rst
            PIN AddrTrigger AddrTrigger
            PIN DataInOut(31:0) TimerDataOut(31:0)
            PIN PulseOut XLXN_7425
        END BLOCK
        BEGIN BLOCK XLXI_2015 obuf
            PIN I XLXN_7125
            PIN O xexep_p22
        END BLOCK
        BEGIN BLOCK Timer6 TimerBlock
            PIN TimerClk TimerClk
            PIN Enable GlobalEnable
            PIN OnePPS OnePPS
            PIN CS TimerSelect(5)
            PIN AddrIn(2:0) AddrOutTimer(2:0)
            PIN BusClk BusClk
            PIN WR WR
            PIN Timer_Rst Timer_Rst
            PIN AddrTrigger AddrTrigger
            PIN DataInOut(31:0) TimerDataOut(31:0)
            PIN PulseOut XLXN_7125
        END BLOCK
        BEGIN BLOCK XLXI_1780 obuf
            PIN I XLXN_6666
            PIN O xexep_n30
        END BLOCK
        BEGIN BLOCK Timer_1 TimerBlock
            PIN TimerClk TimerClk
            PIN Enable GlobalEnable
            PIN OnePPS OnePPS
            PIN CS TimerSelect(0)
            PIN AddrIn(2:0) AddrOutTimer(2:0)
            PIN BusClk BusClk
            PIN WR WR
            PIN Timer_Rst Timer_Rst
            PIN AddrTrigger AddrTrigger
            PIN DataInOut(31:0) TimerDataOut(31:0)
            PIN PulseOut XLXN_6666
        END BLOCK
        BEGIN BLOCK Timer5 TimerBlock
            PIN TimerClk TimerClk
            PIN Enable GlobalEnable
            PIN OnePPS OnePPS
            PIN CS TimerSelect(4)
            PIN AddrIn(2:0) AddrOutTimer(2:0)
            PIN BusClk BusClk
            PIN WR WR
            PIN Timer_Rst Timer_Rst
            PIN AddrTrigger AddrTrigger
            PIN DataInOut(31:0) TimerDataOut(31:0)
            PIN PulseOut XLXN_7173
        END BLOCK
        BEGIN BLOCK XLXI_2023 obuf
            PIN I XLXN_7173
            PIN O xexep_n24
        END BLOCK
        BEGIN BLOCK XLXI_2027 obuf
            PIN I XLXN_7197
            PIN O xexep_p13
        END BLOCK
        BEGIN BLOCK Timer10 TimerBlock
            PIN TimerClk TimerClk
            PIN Enable GlobalEnable
            PIN OnePPS OnePPS
            PIN CS TimerSelect(9)
            PIN AddrIn(2:0) AddrOutTimer(2:0)
            PIN BusClk BusClk
            PIN WR WR
            PIN Timer_Rst Timer_Rst
            PIN AddrTrigger AddrTrigger
            PIN DataInOut(31:0) TimerDataOut(31:0)
            PIN PulseOut XLXN_7197
        END BLOCK
        BEGIN BLOCK Timer15 TimerBlock
            PIN TimerClk TimerClk
            PIN Enable GlobalEnable
            PIN OnePPS OnePPS
            PIN CS TimerSelect(14)
            PIN AddrIn(2:0) AddrOutTimer(2:0)
            PIN BusClk BusClk
            PIN WR WR
            PIN Timer_Rst Timer_Rst
            PIN AddrTrigger AddrTrigger
            PIN DataInOut(31:0) TimerDataOut(31:0)
            PIN PulseOut XLXN_7509
        END BLOCK
        BEGIN BLOCK XLXI_2079 obuf
            PIN I XLXN_7509
            PIN O xexep_p23
        END BLOCK
        BEGIN BLOCK XLXI_2080 obuf
            PIN I XLXN_7510
            PIN O xexep_n13
        END BLOCK
        BEGIN BLOCK Timer20 TimerBlock
            PIN TimerClk TimerClk
            PIN Enable GlobalEnable
            PIN OnePPS OnePPS
            PIN CS TimerSelect(19)
            PIN AddrIn(2:0) AddrOutTimer(2:0)
            PIN BusClk BusClk
            PIN WR WR
            PIN Timer_Rst Timer_Rst
            PIN AddrTrigger AddrTrigger
            PIN DataInOut(31:0) TimerDataOut(31:0)
            PIN PulseOut XLXN_7510
        END BLOCK
    END NETLIST
    BEGIN SHEET 1 2720 1760
        BEGIN INSTANCE XLXI_735 2880 1840 R0
        END INSTANCE
        BEGIN INSTANCE BusDCM 192 1616 R0
            BEGIN DISPLAY 72 -1112 ATTR CLKIN_PERIOD
                FONT 28 "Arial"
            END DISPLAY
        END INSTANCE
        BEGIN BRANCH XLXN_1071
            WIRE 576 656 656 656
            WIRE 656 656 656 688
        END BRANCH
        BEGIN BRANCH BusClk
            WIRE 656 912 656 992
            BEGIN DISPLAY 656 992 ATTR Name
                ALIGNMENT SOFT-VRIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH HAClk
            WIRE 144 656 192 656
            BEGIN DISPLAY 144 656 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH BusClk
            WIRE 144 720 192 720
            BEGIN DISPLAY 144 720 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH Reset
            WIRE 128 1232 192 1232
            BEGIN DISPLAY 128 1232 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH XLXN_1115
            WIRE 144 1360 192 1360
            WIRE 144 1360 144 1424
            WIRE 144 1424 192 1424
            WIRE 144 1424 144 1488
            WIRE 144 1488 192 1488
            WIRE 144 1488 144 1520
        END BRANCH
        INSTANCE XLXI_1288 80 1648 R0
        INSTANCE XLXI_1289 96 944 R0
        BEGIN BRANCH XLXN_1429
            WIRE 160 784 192 784
            WIRE 160 784 160 816
        END BRANCH
        BEGIN INSTANCE XLXI_1292 2880 1472 R0
        END INSTANCE
        INSTANCE XLXI_854 624 688 R90
        BEGIN INSTANCE LinkDCM 864 1600 R0
            BEGIN DISPLAY 72 -1160 ATTR CLKFX_MULTIPLY
                FONT 28 "Arial"
            END DISPLAY
            BEGIN DISPLAY 72 -1112 ATTR CLKIN_PERIOD
                FONT 28 "Arial"
            END DISPLAY
        END INSTANCE
        BEGIN BRANCH UserClk
            WIRE 832 640 864 640
            BEGIN DISPLAY 832 640 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH Reset
            WIRE 816 1216 864 1216
            BEGIN DISPLAY 816 1216 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH UserClkFB
            WIRE 1248 640 1296 640
            BEGIN DISPLAY 1296 640 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH UserClkFB
            WIRE 832 704 864 704
            BEGIN DISPLAY 832 704 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH XLXN_1326
            WIRE 1248 1088 1328 1088
            WIRE 1328 1088 1328 1120
        END BRANCH
        INSTANCE XLXI_556 768 928 R0
        BEGIN BRANCH LinkClk
            WIRE 1328 1344 1328 1424
            BEGIN DISPLAY 1328 1424 ATTR Name
                ALIGNMENT SOFT-VRIGHT
            END DISPLAY
        END BRANCH
        INSTANCE XLXI_424 752 1632 R0
        BEGIN BRANCH XLXN_3759
            WIRE 832 768 864 768
            WIRE 832 768 832 800
        END BRANCH
        BEGIN BRANCH XLXN_3761
            WIRE 816 1344 864 1344
            WIRE 816 1344 816 1408
            WIRE 816 1408 816 1472
            WIRE 816 1472 816 1504
            WIRE 816 1472 864 1472
            WIRE 816 1408 864 1408
        END BRANCH
        INSTANCE XLXI_490 1296 1120 R90
        BEGIN BRANCH TimerClkIn
            WIRE 1520 176 1568 176
            BEGIN DISPLAY 1520 176 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH TimerClk
            WIRE 1520 240 1568 240
            BEGIN DISPLAY 1520 240 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH Reset
            WIRE 1536 752 1568 752
            BEGIN DISPLAY 1536 752 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        INSTANCE Startup 2208 496 R0
        BEGIN BRANCH Reset
            WIRE 2160 176 2208 176
            BEGIN DISPLAY 2160 176 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        INSTANCE XLXI_555 2096 528 R0
        BEGIN BRANCH XLXN_1428
            WIRE 2160 240 2208 240
            WIRE 2160 240 2160 368
            WIRE 2160 368 2160 400
            WIRE 2160 368 2208 368
        END BRANCH
        BEGIN BRANCH XLXN_6551
            WIRE 1952 176 2016 176
            WIRE 2016 176 2016 208
        END BRANCH
        BEGIN BRANCH TimerClk
            WIRE 2016 432 2016 512
            BEGIN DISPLAY 2016 512 ATTR Name
                ALIGNMENT SOFT-VRIGHT
            END DISPLAY
        END BRANCH
        INSTANCE XLXI_1893 1984 208 R90
        BEGIN INSTANCE TimerDCM 1568 1136 R0
            BEGIN DISPLAY 72 -1112 ATTR CLKIN_PERIOD
                FONT 28 "Arial"
            END DISPLAY
        END INSTANCE
        BEGIN BRANCH XLXN_6556
            WIRE 1536 880 1568 880
            WIRE 1536 880 1536 944
            WIRE 1536 944 1568 944
            WIRE 1536 944 1536 1008
            WIRE 1536 1008 1568 1008
            WIRE 1536 1008 1536 1040
        END BRANCH
        INSTANCE XLXI_1891 1472 1168 R0
        BEGIN BRANCH xv2_reset_l
            WIRE 288 64 384 64
        END BRANCH
        BEGIN BRANCH XLXN_703
            WIRE 608 64 656 64
        END BRANCH
        BEGIN BRANCH Reset
            WIRE 880 64 912 64
            BEGIN DISPLAY 912 64 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        INSTANCE XLXI_17 384 96 R0
        INSTANCE XLXI_307 656 96 R0
        IOMARKER 288 64 xv2_reset_l R180 28
        BEGIN BRANCH xv2_haclk
            WIRE 288 176 384 176
        END BRANCH
        BEGIN BRANCH HAClk
            WIRE 608 176 672 176
            BEGIN DISPLAY 672 176 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        INSTANCE XLXI_123 384 208 R0
        BEGIN DISPLAY 672 144 TEXT "40 Mhz"
            FONT 32 "Arial"
        END DISPLAY
        IOMARKER 288 176 xv2_haclk R180 28
        BEGIN BRANCH xuser_clk
            WIRE 288 288 384 288
        END BRANCH
        BEGIN BRANCH UserClk
            WIRE 608 288 672 288
            BEGIN DISPLAY 672 288 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        INSTANCE XLXI_124 384 320 R0
        BEGIN DISPLAY 672 256 TEXT "41.667 Mhz"
            FONT 32 "Arial"
        END DISPLAY
        IOMARKER 288 288 xuser_clk R180 28
        BEGIN BRANCH xexep_n6
            WIRE 288 416 368 416
            WIRE 368 416 384 416
        END BRANCH
        BEGIN BRANCH TimerClkIn
            WIRE 608 416 624 416
            WIRE 624 416 672 416
            BEGIN DISPLAY 672 416 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        INSTANCE XLXI_1781 384 448 R0
        IOMARKER 288 416 xexep_n6 R180 28
        BEGIN DISPLAY 660 384 TEXT "10 Mhz"
            FONT 32 "Arial"
        END DISPLAY
        BEGIN BRANCH XLXN_6557
            WIRE 1504 304 1504 320
            WIRE 1504 304 1568 304
        END BRANCH
        INSTANCE XLXI_1892 1440 448 R0
    END SHEET
    BEGIN SHEET 2 2720 1760
        BEGIN INSTANCE XLXI_736 2880 1840 R0
        END INSTANCE
        BEGIN BRANCH HBGEn_l
            WIRE 1696 768 1792 768
            BEGIN DISPLAY 1696 768 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        INSTANCE XLXI_886 1792 800 R0
        BEGIN BRANCH xv2_hbgen_l
            WIRE 2016 768 2048 768
        END BRANCH
        BEGIN DISPLAY 1424 704 TEXT "This goes to a Mux that either loops HBR to HBG (Default HBGEn = 1) or uses the Virtex generated HBG"
            FONT 24 "Arial"
        END DISPLAY
        BEGIN BRANCH SlDataLOE_l
            WIRE 1696 1232 1776 1232
            BEGIN DISPLAY 1696 1232 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH DataLOE_l
            WIRE 2032 1264 2128 1264
            BEGIN DISPLAY 2128 1264 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH L0SDBusInUse
            WIRE 1712 1296 1776 1296
            BEGIN DISPLAY 1712 1296 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        INSTANCE XLXI_1343 1776 1360 R0
        BEGIN BRANCH SlDataHOE_l
            WIRE 1696 1392 1776 1392
            BEGIN DISPLAY 1696 1392 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH DataHOE_l
            WIRE 2032 1424 2128 1424
            BEGIN DISPLAY 2128 1424 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH L0SDBusInUse
            WIRE 1712 1456 1776 1456
            BEGIN DISPLAY 1712 1456 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        INSTANCE XLXI_1345 1776 1520 R0
        BEGIN BRANCH HBR_l
            WIRE 1584 1008 1680 1008
            BEGIN DISPLAY 1584 1008 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH HBGEn_l
            WIRE 1584 1072 1680 1072
            BEGIN DISPLAY 1584 1072 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH BusCntlOE_l
            WIRE 2000 976 2144 976
            BEGIN DISPLAY 2144 976 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        INSTANCE XLXI_1619 1680 1104 R0
        BEGIN BRANCH XLXN_6109
            WIRE 1552 944 1680 944
        END BRANCH
        INSTANCE XLXI_1712 1296 1040 R0
        INSTANCE HBGDelayDFF 864 1232 R0
        BEGIN BRANCH XLXN_6104
            WIRE 1248 976 1296 976
        END BRANCH
        BEGIN BRANCH BusClk
            WIRE 832 1104 864 1104
            BEGIN DISPLAY 832 1104 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH Reset
            WIRE 832 880 864 880
            BEGIN DISPLAY 832 880 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN DISPLAY 716 712 TEXT "This prevents drive contention with the FIN when the bus is released"
            FONT 24 "Arial"
        END DISPLAY
        INSTANCE XLXI_1728 272 1040 R0
        BEGIN BRANCH HBG_l
            WIRE 656 784 736 784
            WIRE 736 784 736 976
            WIRE 736 976 864 976
            WIRE 736 784 1264 784
            WIRE 1264 784 1264 912
            WIRE 1264 912 1296 912
            BEGIN DISPLAY 736 784 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BEGIN BRANCH Reset
            WIRE 224 688 272 688
            BEGIN DISPLAY 224 688 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH BusClk
            WIRE 224 912 272 912
            BEGIN DISPLAY 224 912 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH PreHBG_l
            WIRE 224 784 272 784
            BEGIN DISPLAY 224 784 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        IOMARKER 2048 768 xv2_hbgen_l R0 28
        BEGIN BRANCH HBRReg_l
            WIRE 960 272 1040 272
            WIRE 1040 272 1168 272
            BEGIN DISPLAY 1040 272 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BEGIN BRANCH PreHBG_l
            WIRE 1424 368 1488 368
            WIRE 1488 368 1568 368
            BEGIN DISPLAY 1488 368 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BEGIN BRANCH xv2_hbg_l
            WIRE 1952 368 1984 368
        END BRANCH
        INSTANCE XLXI_1724 1568 624 R0
        BEGIN BRANCH BusClk
            WIRE 1520 496 1568 496
            BEGIN DISPLAY 1520 496 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        INSTANCE XLXI_1733 1168 528 R0
        BEGIN BRANCH HBGSyncPre
            WIRE 1136 464 1168 464
            BEGIN DISPLAY 1136 464 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH L0SDBusInUse
            WIRE 1136 336 1168 336
            BEGIN DISPLAY 1136 336 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH SDL1BusInUse
            WIRE 1136 400 1168 400
            BEGIN DISPLAY 1136 400 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        IOMARKER 1984 368 xv2_hbg_l R0 28
        INSTANCE XLXI_1764 576 528 R0
        BEGIN BRANCH xha_hbr_l
            WIRE 240 272 304 272
        END BRANCH
        INSTANCE XLXI_1765 304 304 R0
        IOMARKER 240 272 xha_hbr_l R180 28
        BEGIN BRANCH HBR_l
            WIRE 528 272 544 272
            WIRE 544 272 576 272
            BEGIN DISPLAY 544 272 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BEGIN BRANCH BusClk
            WIRE 528 400 576 400
            BEGIN DISPLAY 528 400 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
    END SHEET
    BEGIN SHEET 3 2720 1760
        BEGIN INSTANCE XLXI_737 2880 1840 R0
        END INSTANCE
        BEGIN BRANCH xha_brst
            WIRE 2240 1312 2288 1312
        END BRANCH
        BEGIN BRANCH BusCntlOE_l
            WIRE 1984 1248 2016 1248
            BEGIN DISPLAY 1984 1248 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH BrstOut
            WIRE 1984 1312 2016 1312
            BEGIN DISPLAY 1984 1312 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        INSTANCE XLXI_119 2016 1440 R0
        BEGIN BRANCH AckOut
            WIRE 1808 1104 2016 1104
        END BRANCH
        INSTANCE XLXI_661 2016 1232 R0
        BEGIN BRANCH AckIn
            WIRE 1984 1168 2016 1168
            BEGIN DISPLAY 1984 1168 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH xha_ack
            WIRE 2240 1104 2304 1104
        END BRANCH
        BEGIN BRANCH AckOE_l
            WIRE 1984 1040 2016 1040
            BEGIN DISPLAY 1984 1040 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN INSTANCE XLXI_1314 1936 624 R0
        END INSTANCE
        BEGIN BRANCH DataLOE_l
            WIRE 1872 464 1936 464
            BEGIN DISPLAY 1872 464 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH DataIn(31:0)
            WIRE 1856 592 1936 592
            BEGIN DISPLAY 1856 592 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH DataOut(31:0)
            WIRE 1856 528 1936 528
            BEGIN DISPLAY 1856 528 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH xha_d(31:0)
            WIRE 2272 560 2384 560
            BEGIN DISPLAY 2384 560 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH xha_a(31:0)
            WIRE 2272 272 2320 272
        END BRANCH
        BEGIN INSTANCE XLXI_648 1936 336 R0
        END INSTANCE
        BEGIN BRANCH AddrIn(31:0)
            WIRE 1872 304 1936 304
            BEGIN DISPLAY 1872 304 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH BusCntlOE_l
            WIRE 1872 176 1936 176
            BEGIN DISPLAY 1872 176 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH AddrOut(31:0)
            WIRE 1872 240 1936 240
            BEGIN DISPLAY 1872 240 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN INSTANCE XLXI_1313 1936 880 R0
        END INSTANCE
        BEGIN BRANCH DataHOE_l
            WIRE 1872 720 1936 720
            BEGIN DISPLAY 1872 720 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH DataIn(63:32)
            WIRE 1856 848 1936 848
            BEGIN DISPLAY 1856 848 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH DataOut(63:32)
            WIRE 1856 784 1936 784
            BEGIN DISPLAY 1856 784 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH xha_d(63:32)
            WIRE 2272 816 2384 816
            BEGIN DISPLAY 2384 816 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH xha_d(63:0)
            WIRE 2448 688 2496 688
        END BRANCH
        INSTANCE XLXI_1602 1744 1104 R0
        IOMARKER 2288 1312 xha_brst R0 28
        IOMARKER 2304 1104 xha_ack R0 28
        IOMARKER 2320 272 xha_a(31:0) R0 28
        IOMARKER 2496 688 xha_d(63:0) R0 28
        INSTANCE XLXI_446 384 688 R0
        INSTANCE XLXI_447 400 896 R0
        BEGIN BRANCH xha_rdl_l
            WIRE 608 144 656 144
        END BRANCH
        BEGIN BRANCH xha_rdh_l
            WIRE 608 352 656 352
        END BRANCH
        BEGIN BRANCH xha_wrl_l
            WIRE 608 560 672 560
        END BRANCH
        BEGIN BRANCH xha_wrh_l
            WIRE 624 768 672 768
        END BRANCH
        BEGIN BRANCH WrHIn_l
            WIRE 320 832 400 832
            BEGIN DISPLAY 320 832 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH WrLIn_l
            WIRE 320 624 384 624
            BEGIN DISPLAY 320 624 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH RdHIn_l
            WIRE 320 416 384 416
            BEGIN DISPLAY 320 416 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH RdLIn_l
            WIRE 320 208 384 208
            BEGIN DISPLAY 320 208 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH WrLOut_l
            WIRE 320 560 384 560
            BEGIN DISPLAY 320 560 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH WrHOut_l
            WIRE 320 768 400 768
            BEGIN DISPLAY 320 768 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH BusCntlOE_l
            WIRE 320 80 352 80
            WIRE 352 80 384 80
            WIRE 352 80 352 288
            WIRE 352 288 384 288
            WIRE 352 288 352 496
            WIRE 352 496 384 496
            WIRE 352 496 352 704
            WIRE 352 704 400 704
            BEGIN DISPLAY 320 80 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH RdLOut_l
            WIRE 320 144 384 144
            BEGIN DISPLAY 320 144 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH RdHOut_l
            WIRE 320 352 384 352
            BEGIN DISPLAY 320 352 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        INSTANCE XLXI_445 384 480 R0
        INSTANCE XLXI_444 384 272 R0
        BEGIN BRANCH XLXN_1211
            WIRE 288 1216 400 1216
            WIRE 288 1216 288 1424
            WIRE 288 1424 400 1424
            WIRE 288 1424 288 1632
            WIRE 288 1632 400 1632
        END BRANCH
        BEGIN BRANCH xha_ms1_l
            WIRE 624 1216 688 1216
        END BRANCH
        INSTANCE XLXI_455 400 1344 R0
        BEGIN BRANCH BusCntlOE_l
            WIRE 336 944 368 944
            WIRE 368 944 400 944
            WIRE 368 944 368 1152
            WIRE 368 1152 400 1152
            WIRE 368 1152 368 1360
            WIRE 368 1360 400 1360
            WIRE 368 1360 368 1568
            WIRE 368 1568 400 1568
            BEGIN DISPLAY 336 944 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH xha_ms2_l
            WIRE 624 1424 704 1424
        END BRANCH
        INSTANCE XLXI_456 400 1552 R0
        BEGIN BRANCH xha_ms3_l
            WIRE 624 1632 688 1632
        END BRANCH
        INSTANCE XLXI_457 400 1760 R0
        INSTANCE XLXI_454 400 1136 R0
        BEGIN BRANCH xha_ms0_l
            WIRE 624 1008 688 1008
        END BRANCH
        BEGIN BRANCH MS0Out_l
            WIRE 336 1008 400 1008
            BEGIN DISPLAY 336 1008 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        INSTANCE XLXI_458 224 1216 R0
        IOMARKER 672 768 xha_wrh_l R0 28
        IOMARKER 672 560 xha_wrl_l R0 28
        IOMARKER 656 352 xha_rdh_l R0 28
        IOMARKER 656 144 xha_rdl_l R0 28
        IOMARKER 688 1216 xha_ms1_l R0 28
        IOMARKER 704 1424 xha_ms2_l R0 28
        IOMARKER 688 1632 xha_ms3_l R0 28
        IOMARKER 688 1008 xha_ms0_l R0 28
        INSTANCE XLXI_1656(63:0) 1232 784 R0
        BEGIN BRANCH DataOut(63:0)
            WIRE 1552 656 1584 656
            BEGIN DISPLAY 1584 656 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH RegDataOut(63:0)
            WIRE 1200 624 1232 624
            BEGIN DISPLAY 1200 624 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH L2SDataOut(63:0)
            WIRE 1200 688 1232 688
            BEGIN DISPLAY 1200 688 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH L0SDBusEn(63:0)
            WIRE 1200 752 1232 752
            BEGIN DISPLAY 1200 752 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        INSTANCE XLXI_1676 1232 1216 R0
        BEGIN BRANCH MS0Out_l
            WIRE 1552 1088 1584 1088
            BEGIN DISPLAY 1584 1088 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH L2SMS0Out_l
            WIRE 1200 1056 1232 1056
            BEGIN DISPLAY 1200 1056 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH S2LMS0Out_l
            WIRE 1200 1120 1232 1120
            BEGIN DISPLAY 1200 1120 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH SDL1BusEn
            WIRE 1200 1184 1232 1184
            BEGIN DISPLAY 1200 1184 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        INSTANCE XLXI_1679 1232 1536 R0
        BEGIN BRANCH BrstOut
            WIRE 1552 1408 1584 1408
            BEGIN DISPLAY 1584 1408 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH L2SBrstOut
            WIRE 1200 1376 1232 1376
            BEGIN DISPLAY 1200 1376 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH S2LBrstOut
            WIRE 1200 1440 1232 1440
            BEGIN DISPLAY 1200 1440 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH SDL1BusEn
            WIRE 1200 1504 1232 1504
            BEGIN DISPLAY 1200 1504 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        INSTANCE XLXI_1672(31:0) 1232 256 R0
        BEGIN BRANCH L2SAddrOut(31:0)
            WIRE 1200 96 1232 96
            BEGIN DISPLAY 1200 96 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH S2LAddrOut(31:0)
            WIRE 1200 160 1232 160
            BEGIN DISPLAY 1200 160 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH SDL1BusEnAr(31:0)
            WIRE 1168 224 1200 224
            WIRE 1200 224 1232 224
            BEGIN DISPLAY 1168 224 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH AddrOut(31:0)
            WIRE 1552 128 1584 128
            BEGIN DISPLAY 1584 128 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
    END SHEET
    BEGIN SHEET 4 2720 1760
        BEGIN INSTANCE XLXI_738 2880 1840 R0
        END INSTANCE
        BEGIN BRANCH BusClk
            WIRE 400 128 432 128
            BEGIN DISPLAY 400 128 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH Reset
            WIRE 400 160 432 160
            BEGIN DISPLAY 400 160 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH AddrIn(31:0)
            WIRE 400 352 432 352
            BEGIN DISPLAY 400 352 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH DataIn(63:0)
            WIRE 400 400 432 400
            BEGIN DISPLAY 400 400 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH DataInReg(63:0)
            WIRE 960 400 992 400
            BEGIN DISPLAY 992 400 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN INSTANCE Slave_Bus_Int 432 128 R0
        END INSTANCE
        BEGIN BRANCH SlDataHOE_l
            WIRE 960 304 992 304
            BEGIN DISPLAY 992 304 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH SlDataLOE_l
            WIRE 960 272 992 272
            BEGIN DISPLAY 992 272 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH AckOE_l
            WIRE 960 224 992 224
            BEGIN DISPLAY 992 224 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH WrLIn_l
            WIRE 400 208 432 208
            BEGIN DISPLAY 400 208 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH WrHIn_l
            WIRE 400 240 432 240
            BEGIN DISPLAY 400 240 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH RdLIn_l
            WIRE 400 272 432 272
            BEGIN DISPLAY 400 272 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH RdHIn_l
            WIRE 400 304 432 304
            BEGIN DISPLAY 400 304 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH MiscRegWr
            WIRE 960 464 992 464
            BEGIN DISPLAY 992 464 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH Hdr2IOEnRegWr
            WIRE 960 1664 992 1664
            BEGIN DISPLAY 992 1664 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH Hdr2IORWRegWr
            WIRE 960 1584 992 1584
            BEGIN DISPLAY 992 1584 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH Hdr1IOEnRegWr
            WIRE 960 1504 992 1504
            BEGIN DISPLAY 992 1504 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH Hdr1IORWRegWr
            WIRE 960 1424 992 1424
            BEGIN DISPLAY 992 1424 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH FPIOEnRegWrH
            WIRE 960 1344 992 1344
            BEGIN DISPLAY 992 1344 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH FPIOEnRegWrL
            WIRE 960 1264 992 1264
            BEGIN DISPLAY 992 1264 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH FPIORWRegWrH
            WIRE 960 1184 992 1184
            BEGIN DISPLAY 992 1184 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH FPIORWRegWrL
            WIRE 960 1104 992 1104
            BEGIN DISPLAY 992 1104 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH BMS2LCntWr
            WIRE 960 1024 992 1024
            BEGIN DISPLAY 992 1024 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH BMS2LAddrWr
            WIRE 960 944 992 944
            BEGIN DISPLAY 992 944 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH MiscRegDatIn(31:0)
            WIRE 960 496 992 496
            BEGIN DISPLAY 992 496 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH BMS2LAddrDatIn(31:0)
            WIRE 960 976 992 976
            BEGIN DISPLAY 992 976 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH BMS2LCntDatIn(31:0)
            WIRE 960 1056 992 1056
            BEGIN DISPLAY 992 1056 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH FPIORWDatInL(31:0)
            WIRE 960 1136 992 1136
            BEGIN DISPLAY 992 1136 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH FPIORWDatInH(31:0)
            WIRE 960 1216 992 1216
            BEGIN DISPLAY 992 1216 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH FPIOEnDatInL(31:0)
            WIRE 960 1296 992 1296
            BEGIN DISPLAY 992 1296 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH FPIOEnDatInH(31:0)
            WIRE 960 1376 992 1376
            BEGIN DISPLAY 992 1376 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH Hdr1IORWDatIn(31:0)
            WIRE 960 1456 992 1456
            BEGIN DISPLAY 992 1456 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH Hdr1IOEnDatIn(31:0)
            WIRE 960 1536 992 1536
            BEGIN DISPLAY 992 1536 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH Hdr2IORWDatIn(31:0)
            WIRE 960 1616 992 1616
            BEGIN DISPLAY 992 1616 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH Hdr2IOEnDatIn(31:0)
            WIRE 960 1696 992 1696
            BEGIN DISPLAY 992 1696 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH Hdr2IOEnDatOut(31:0)
            WIRE 400 1696 432 1696
            BEGIN DISPLAY 400 1696 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH Hdr2IORWDatOut(31:0)
            WIRE 400 1616 432 1616
            BEGIN DISPLAY 400 1616 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH Hdr1IOEnDatOut(31:0)
            WIRE 400 1536 432 1536
            BEGIN DISPLAY 400 1536 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH Hdr1IORWDatOut(31:0)
            WIRE 400 1456 432 1456
            BEGIN DISPLAY 400 1456 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH FPIOEnDatOutH(31:0)
            WIRE 400 1376 432 1376
            BEGIN DISPLAY 400 1376 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH FPIOEnDatOutL(31:0)
            WIRE 400 1296 432 1296
            BEGIN DISPLAY 400 1296 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH FPIORWDatOutH(31:0)
            WIRE 400 1216 432 1216
            BEGIN DISPLAY 400 1216 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH BMS2LCntDatOut(31:0)
            WIRE 368 1056 432 1056
            BEGIN DISPLAY 368 1056 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH BMS2LAddrDatOut(31:0)
            WIRE 368 976 432 976
            BEGIN DISPLAY 368 976 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH MiscRegDatOut(31:0)
            WIRE 400 496 432 496
            BEGIN DISPLAY 400 496 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH StatRegDatOut(31:0)
            WIRE 400 576 432 576
            BEGIN DISPLAY 400 576 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH RegDataOut(63:0)
            WIRE 960 352 992 352
            BEGIN DISPLAY 992 352 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH FPIORWDatOutL(31:0)
            WIRE 400 1136 432 1136
            BEGIN DISPLAY 400 1136 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN DISPLAY 1304 184 TEXT "0x0 32 bit miscelanous register"
            FONT 24 "Arial"
        END DISPLAY
        BEGIN DISPLAY 1304 208 TEXT "0x1 32 bit Status register (Read Only)"
            FONT 24 "Arial"
        END DISPLAY
        BEGIN DISPLAY 1304 228 TEXT "0x4 and 0x5 64 bit Link to SDRAM control Reg (0x4 is the current address and 0x5 is unused)"
            FONT 24 "Arial"
        END DISPLAY
        BEGIN DISPLAY 1304 248 TEXT "0x6 and 0x7 64 bit Link to SDRAM control Reg (0x4 is the current address and 0x5 is the number of 64 bit words to be transferred)"
            FONT 24 "Arial"
        END DISPLAY
        BEGIN DISPLAY 1304 268 TEXT "0x8 and 0x9 64 bit front panel connector Read/Write register"
            FONT 24 "Arial"
        END DISPLAY
        BEGIN DISPLAY 1304 292 TEXT "0xA and 0xB 64 bit front panel connector drive enable register"
            FONT 24 "Arial"
        END DISPLAY
        BEGIN DISPLAY 1304 316 TEXT "0xC 32 bit Header 1 connector Read/Write register"
            FONT 24 "Arial"
        END DISPLAY
        BEGIN DISPLAY 1304 340 TEXT "0xD 32 bit Header 1 connector drive enable register"
            FONT 24 "Arial"
        END DISPLAY
        BEGIN DISPLAY 1304 364 TEXT "0xE 32 bit Header 2 connector Read/Write register"
            FONT 24 "Arial"
        END DISPLAY
        BEGIN DISPLAY 1304 388 TEXT "0xF 32 bit Header 2 connector drive enable register"
            FONT 24 "Arial"
        END DISPLAY
        BEGIN DISPLAY 1280 116 TEXT "Starting at that address there are 14 32 bit registers"
            FONT 24 "Arial"
        END DISPLAY
        BEGIN DISPLAY 1280 92 TEXT "Chip base address is 0x00100000 on hammerhead bus (0x100000 (32 bit addr) offset from BAR2 from PCI Bus)"
            FONT 24 "Arial"
        END DISPLAY
        BEGIN DISPLAY 1280 148 TEXT "They are:"
            FONT 24 "Arial"
        END DISPLAY
        BEGIN DISPLAY 1264 68 TEXT "Address Map:"
            FONT 24 "Arial"
        END DISPLAY
        BEGIN BRANCH TimerDataWr
            WIRE 960 704 992 704
            BEGIN DISPLAY 992 704 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH TimerAddrWr
            WIRE 960 624 992 624
            BEGIN DISPLAY 992 624 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH TimerAddrIn(31:0)
            WIRE 960 656 992 656
            BEGIN DISPLAY 992 656 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH TimerDataIn(31:0)
            WIRE 960 736 992 736
            BEGIN DISPLAY 992 736 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH TimerAddrOut(31:0)
            WIRE 400 656 432 656
            BEGIN DISPLAY 400 656 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH TimerDataOut(31:0)
            WIRE 400 736 432 736
            BEGIN DISPLAY 400 736 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        INSTANCE XLXI_1873(31:0) 240 784 R90
        BEGIN BRANCH XLXN_6518(31:0)
            WIRE 368 848 400 848
            WIRE 400 848 400 896
            WIRE 400 896 432 896
            WIRE 400 816 400 848
            WIRE 400 816 432 816
        END BRANCH
    END SHEET
    BEGIN SHEET 5 2720 1760
        BEGIN INSTANCE XLXI_739 2880 1840 R0
        END INSTANCE
        BEGIN INSTANCE SDtoFIFO 1392 912 R0
        END INSTANCE
        BEGIN BRANCH SDL1BusReq
            WIRE 2080 992 2112 992
            BEGIN DISPLAY 2112 992 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH SDL1BusInUse
            WIRE 2080 1024 2112 1024
            BEGIN DISPLAY 2112 1024 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH SDL1FIFODatIn(63:0)
            WIRE 2080 1424 2112 1424
            BEGIN DISPLAY 2112 1424 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH SDL1FIFOWr
            WIRE 2080 1392 2112 1392
            BEGIN DISPLAY 2112 1392 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH S2LAddrOut(31:0)
            WIRE 2080 1216 2112 1216
            BEGIN DISPLAY 2112 1216 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH S2LBrstOut
            WIRE 2080 1344 2112 1344
            BEGIN DISPLAY 2112 1344 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH S2LMS0Out_l
            WIRE 2080 1248 2112 1248
            BEGIN DISPLAY 2112 1248 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH RdLOut_l
            WIRE 2080 1280 2112 1280
            BEGIN DISPLAY 2112 1280 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH RdHOut_l
            WIRE 2080 1312 2112 1312
            BEGIN DISPLAY 2112 1312 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH BMS2LCntDatOut(31:0)
            WIRE 2080 1152 2112 1152
            BEGIN DISPLAY 2112 1152 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH BMS2LAddrDatOut(31:0)
            WIRE 2080 1072 2112 1072
            BEGIN DISPLAY 2112 1072 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH BusClk
            WIRE 1360 912 1392 912
            BEGIN DISPLAY 1360 912 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH Reset
            WIRE 1360 944 1392 944
            BEGIN DISPLAY 1360 944 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH BMS2LCntDatIn(31:0)
            WIRE 1360 1152 1392 1152
            BEGIN DISPLAY 1360 1152 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH BMS2LAddrDatIn(31:0)
            WIRE 1360 1072 1392 1072
            BEGIN DISPLAY 1360 1072 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH SDL1BusGnt
            WIRE 1360 992 1392 992
            BEGIN DISPLAY 1360 992 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH BMS2LAddrWr
            WIRE 1360 1040 1392 1040
            BEGIN DISPLAY 1360 1040 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH BMS2LCntWr
            WIRE 1360 1120 1392 1120
            BEGIN DISPLAY 1360 1120 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH DataInReg(63:0)
            WIRE 1360 1216 1392 1216
            BEGIN DISPLAY 1360 1216 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH AckIn
            WIRE 1360 1248 1392 1248
            BEGIN DISPLAY 1360 1248 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH SDL1FIFOWrCnt(8:0)
            WIRE 1360 1424 1392 1424
            BEGIN DISPLAY 1360 1424 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH L0SDBusEn(63:0)
            WIRE 2080 464 2112 464
            BEGIN DISPLAY 2112 464 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH L0RxDataValid
            WIRE 1376 656 1408 656
            BEGIN DISPLAY 1376 656 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH L0SDFIFODatOut(63:0)
            WIRE 1376 784 1408 784
            BEGIN DISPLAY 1376 784 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH L0SDEF
            WIRE 1376 752 1408 752
            BEGIN DISPLAY 1376 752 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH L0SDFIFORdCnt(8:0)
            WIRE 1344 720 1408 720
            BEGIN DISPLAY 1344 720 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH AckIn
            WIRE 1376 576 1408 576
            BEGIN DISPLAY 1376 576 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH BML2SAddrDatIn(31:0)
            WIRE 1376 528 1408 528
            BEGIN DISPLAY 1376 528 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH BML2SAddrWr
            WIRE 1376 496 1408 496
            BEGIN DISPLAY 1376 496 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH L0SDBusGnt
            WIRE 1376 400 1408 400
            BEGIN DISPLAY 1376 400 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH Reset
            WIRE 1376 352 1408 352
            BEGIN DISPLAY 1376 352 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH BusClk
            WIRE 1376 320 1408 320
            BEGIN DISPLAY 1376 320 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH L0SDFIFORd
            WIRE 2080 784 2112 784
            BEGIN DISPLAY 2112 784 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH L2SDataOut(63:0)
            WIRE 2080 736 2112 736
            BEGIN DISPLAY 2112 736 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH L2SAddrOut(31:0)
            WIRE 2080 576 2112 576
            BEGIN DISPLAY 2112 576 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH L2SBrstOut
            WIRE 2080 704 2112 704
            BEGIN DISPLAY 2112 704 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH L2SMS0Out_l
            WIRE 2080 608 2112 608
            BEGIN DISPLAY 2112 608 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH WrLOut_l
            WIRE 2080 640 2112 640
            BEGIN DISPLAY 2112 640 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH WrHOut_l
            WIRE 2080 672 2112 672
            BEGIN DISPLAY 2112 672 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH L0SDBusInUse
            WIRE 2080 432 2112 432
            BEGIN DISPLAY 2112 432 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH BML2SAddrDatOut(31:0)
            WIRE 2080 528 2112 528
            BEGIN DISPLAY 2112 528 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN INSTANCE FIFOtoSD 1408 320 R0
        END INSTANCE
        BEGIN BRANCH L0SDBusReq
            WIRE 2080 400 2112 400
            BEGIN DISPLAY 2112 400 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH TmpEn(7:0)
            WIRE 2400 144 2432 144
            BEGIN DISPLAY 2432 144 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        INSTANCE XLXI_1702(7:0) 2176 176 R0
        BEGIN BRANCH SDL1BusEn,SDL1BusEn,SDL1BusEn,SDL1BusEn,SDL1BusEn,SDL1BusEn,SDL1BusEn,SDL1BusEn
            WIRE 2144 144 2176 144
            BEGIN DISPLAY 2144 144 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH TmpEn(7:0),TmpEn(7:0),TmpEn(7:0),TmpEn(7:0)
            WIRE 2128 64 2176 64
            BEGIN DISPLAY 2128 64 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH SDL1BusEnAr(31:0)
            WIRE 2400 64 2432 64
            BEGIN DISPLAY 2432 64 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        INSTANCE XLXI_1701(31:0) 2176 96 R0
        INSTANCE XLXI_1767 256 768 R0
        BEGIN BRANCH SDL1BusReq
            WIRE 224 640 256 640
            BEGIN DISPLAY 224 640 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH SDL1BusInUse
            WIRE 224 704 256 704
            BEGIN DISPLAY 224 704 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        INSTANCE XLXI_1762 592 1152 R0
        BEGIN BRANCH SDL1BusGnt
            WIRE 848 992 880 992
            BEGIN DISPLAY 880 992 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH HBRReg_l
            WIRE 544 960 592 960
            BEGIN DISPLAY 544 960 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH L0SDBusInUse
            WIRE 544 1088 592 1088
            BEGIN DISPLAY 544 1088 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH HBG_l
            WIRE 544 1024 592 1024
            BEGIN DISPLAY 544 1024 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH SDL1BusReq
            WIRE 544 896 592 896
            BEGIN DISPLAY 544 896 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        INSTANCE XLXI_1766 592 864 R0
        BEGIN BRANCH L0SDBusInUse
            WIRE 528 800 592 800
            BEGIN DISPLAY 528 800 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH HBG_l
            WIRE 528 736 592 736
            BEGIN DISPLAY 528 736 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH SDL1BusEn
            WIRE 848 736 880 736
            BEGIN DISPLAY 880 736 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        INSTANCE XLXI_1763 592 560 R0
        BEGIN BRANCH L0SDBusGnt
            WIRE 848 400 880 400
            BEGIN DISPLAY 880 400 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH HBRReg_l
            WIRE 544 368 592 368
            BEGIN DISPLAY 544 368 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH HBG_l
            WIRE 544 432 592 432
            BEGIN DISPLAY 544 432 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH L0SDBusReq
            WIRE 544 304 592 304
            BEGIN DISPLAY 544 304 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN DISPLAY 348 256 TEXT "This Gives SDRAM to Link 1 Traffic Priority"
            FONT 24 "Arial"
        END DISPLAY
        BEGIN BRANCH XLXN_6122
            WIRE 512 672 544 672
            WIRE 544 672 592 672
            WIRE 544 496 592 496
            WIRE 544 496 544 672
        END BRANCH
    END SHEET
    BEGIN SHEET 6 2720 1760
        BEGIN INSTANCE XLXI_742 2880 1840 R0
        END INSTANCE
        BEGIN BRANCH xv2_l0dat(7:0)
            WIRE 2272 496 2304 496
        END BRANCH
        BEGIN INSTANCE XLXI_565 1936 560 R0
        END INSTANCE
        INSTANCE XLXI_1230 1680 432 R0
        BEGIN BRANCH L0TxEn
            WIRE 1648 400 1680 400
            BEGIN DISPLAY 1648 400 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        INSTANCE XLXI_1224 2032 208 R0
        BEGIN BRANCH L0ClkOut
            WIRE 1968 176 1984 176
            WIRE 1984 176 2032 176
            BEGIN DISPLAY 1968 176 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH L0ClkIn
            WIRE 1968 288 2000 288
            BEGIN DISPLAY 1968 288 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        INSTANCE XLXI_465 2224 256 R180
        BEGIN BRANCH L0TxDat(7:0)
            WIRE 1888 464 1936 464
            BEGIN DISPLAY 1888 464 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH L0RxDat(7:0)
            WIRE 1888 528 1936 528
            BEGIN DISPLAY 1888 528 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN INSTANCE XLXI_588 1904 1392 R0
        END INSTANCE
        BEGIN BRANCH xv2_l1dat(7:0)
            WIRE 2240 1328 2304 1328
        END BRANCH
        INSTANCE XLXI_1231 1648 1264 R0
        BEGIN BRANCH L1TxEn
            WIRE 1616 1232 1648 1232
            BEGIN DISPLAY 1616 1232 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        INSTANCE XLXI_1225 2016 1056 R0
        BEGIN BRANCH L1ClkIn
            WIRE 1952 1136 1984 1136
            BEGIN DISPLAY 1952 1136 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH L1ClkOut
            WIRE 1952 1024 2016 1024
            BEGIN DISPLAY 1952 1024 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        INSTANCE XLXI_467 2208 1104 R180
        BEGIN BRANCH L1TxDat(7:0)
            WIRE 1856 1296 1904 1296
            BEGIN DISPLAY 1856 1296 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH L1RxDat(7:0)
            WIRE 1856 1360 1904 1360
            BEGIN DISPLAY 1856 1360 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN INSTANCE Link0 976 768 R0
        END INSTANCE
        BEGIN BRANCH LinkClk
            WIRE 944 160 976 160
            BEGIN DISPLAY 944 160 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH Reset
            WIRE 944 288 976 288
            BEGIN DISPLAY 944 288 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH L0ClkIn
            WIRE 944 224 976 224
            BEGIN DISPLAY 944 224 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        INSTANCE XLXI_1226 704 416 R0
        BEGIN BRANCH L0RxDat(7:0)
            WIRE 928 672 976 672
            BEGIN DISPLAY 928 672 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH L0RxBP
            WIRE 944 544 976 544
            BEGIN DISPLAY 944 544 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        INSTANCE XLXI_1227 704 608 R0
        BEGIN BRANCH L0TxDat(7:0)
            WIRE 1360 672 1392 672
            BEGIN DISPLAY 1392 672 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH L0RxDataValid
            WIRE 1360 608 1392 608
            BEGIN DISPLAY 1392 608 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH L0RxFIFODat(7:0)
            WIRE 1360 736 1392 736
            BEGIN DISPLAY 1392 736 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH L0ClkOut
            WIRE 1360 160 1392 160
            BEGIN DISPLAY 1392 160 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH L0TxEn
            WIRE 1360 224 1392 224
            BEGIN DISPLAY 1392 224 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN INSTANCE Link1 992 1600 R0
        END INSTANCE
        BEGIN BRANCH L1TxDat(7:0)
            WIRE 1376 1504 1408 1504
            BEGIN DISPLAY 1408 1504 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH L1ClkOut
            WIRE 1376 992 1408 992
            BEGIN DISPLAY 1408 992 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH L1TxBP
            WIRE 1376 1120 1408 1120
            BEGIN DISPLAY 1408 1120 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH L1TxEn
            WIRE 1376 1056 1408 1056
            BEGIN DISPLAY 1408 1056 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH L1TxFIFODat(7:0)
            WIRE 960 1568 992 1568
            BEGIN DISPLAY 960 1568 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH L1ClkIn
            WIRE 960 1056 992 1056
            BEGIN DISPLAY 960 1056 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH L1RxDat(7:0)
            WIRE 960 1504 992 1504
            BEGIN DISPLAY 960 1504 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH L1TxWrEn
            WIRE 960 1184 992 1184
            BEGIN DISPLAY 960 1184 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH LinkClk
            WIRE 960 992 992 992
            BEGIN DISPLAY 960 992 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH Reset
            WIRE 960 1120 992 1120
            BEGIN DISPLAY 960 1120 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        INSTANCE XLXI_1228 720 1440 R0
        INSTANCE XLXI_1229 720 1248 R0
        IOMARKER 2304 496 xv2_l0dat(7:0) R0 28
        IOMARKER 2304 1328 xv2_l1dat(7:0) R0 28
        BEGIN BRANCH XLXN_3753
            WIRE 768 416 976 416
        END BRANCH
        BEGIN BRANCH XLXN_3757
            WIRE 784 1312 944 1312
            WIRE 944 1312 992 1312
            WIRE 944 1312 944 1376
            WIRE 944 1376 944 1440
            WIRE 944 1440 992 1440
            WIRE 944 1376 992 1376
        END BRANCH
        BEGIN BRANCH XLXN_3758
            WIRE 784 1248 992 1248
        END BRANCH
        BEGIN BRANCH XLXN_3764
            WIRE 1872 1232 1904 1232
        END BRANCH
        BEGIN BRANCH XLXN_3765
            WIRE 1904 400 1936 400
        END BRANCH
        BEGIN BRANCH XLXN_5036
            WIRE 768 480 816 480
            WIRE 816 480 976 480
            WIRE 816 480 816 608
            WIRE 816 608 976 608
            WIRE 816 352 976 352
            WIRE 816 352 816 480
        END BRANCH
        INSTANCE XLXI_1711(7:0) 864 864 R0
        BEGIN BRANCH XLXN_6091(7:0)
            WIRE 928 736 976 736
        END BRANCH
        BEGIN BRANCH xv2_l0ack
            WIRE 2256 176 2336 176
        END BRANCH
        IOMARKER 2336 176 xv2_l0ack R0 28
        BEGIN BRANCH xv2_l0clk1
            WIRE 2224 288 2272 288
        END BRANCH
        IOMARKER 2272 288 xv2_l0clk1 R0 28
        BEGIN BRANCH xv2_l1ack
            WIRE 2240 1024 2320 1024
        END BRANCH
        IOMARKER 2320 1024 xv2_l1ack R0 28
        BEGIN BRANCH xv2_l1clk1
            WIRE 2208 1136 2256 1136
        END BRANCH
        IOMARKER 2256 1136 xv2_l1clk1 R0 28
    END SHEET
    BEGIN SHEET 7 2720 1760
        BEGIN INSTANCE L0LtoF 496 752 R0
        END INSTANCE
        INSTANCE L0FIFO 1856 384 R0
        BEGIN BRANCH Reset
            WIRE 2016 896 2048 896
            WIRE 2048 864 2048 896
            BEGIN DISPLAY 2016 896 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN INSTANCE L1FtoL 1888 1472 R0
        END INSTANCE
        INSTANCE L1FIFO 544 992 R0
        BEGIN BRANCH Reset
            WIRE 704 1504 736 1504
            WIRE 736 1472 736 1504
            BEGIN DISPLAY 704 1504 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH Reset
            WIRE 464 528 496 528
            BEGIN DISPLAY 464 528 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH LinkClk
            WIRE 464 464 496 464
            BEGIN DISPLAY 464 464 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH SDL1FIFORdCnt(8:0)
            WIRE 928 1392 960 1392
            BEGIN DISPLAY 960 1392 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH SDL1FIFODatOut(63:0)
            WIRE 928 1232 960 1232
            BEGIN DISPLAY 960 1232 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH SDL1FIFOWr
            WIRE 512 1136 544 1136
            BEGIN DISPLAY 512 1136 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH SDL1FIFORd
            WIRE 512 1360 544 1360
            BEGIN DISPLAY 512 1360 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH LinkClk
            WIRE 512 1392 544 1392
            BEGIN DISPLAY 512 1392 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH SDL1FIFODatIn(63:0)
            WIRE 512 1040 544 1040
            BEGIN DISPLAY 512 1040 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH BusClk
            WIRE 512 1168 544 1168
            BEGIN DISPLAY 512 1168 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH Reset
            WIRE 1856 1120 1888 1120
            BEGIN DISPLAY 1856 1120 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH LinkClk
            WIRE 1856 1056 1888 1056
            BEGIN DISPLAY 1856 1056 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH SDL1FIFODatOut(63:0)
            WIRE 1856 1248 1888 1248
            BEGIN DISPLAY 1856 1248 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH SDL1FIFORdCnt(8:0)
            WIRE 1856 1312 1888 1312
            BEGIN DISPLAY 1856 1312 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH L1TxBP
            WIRE 1856 1184 1888 1184
            BEGIN DISPLAY 1856 1184 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH SDL1FIFORd
            WIRE 2304 1056 2336 1056
            BEGIN DISPLAY 2336 1056 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH L1TxFIFODat(7:0)
            WIRE 2304 1120 2352 1120
            BEGIN DISPLAY 2352 1120 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH L1TxWrEn
            WIRE 2304 1184 2336 1184
            BEGIN DISPLAY 2336 1184 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH L0RxFIFODat(7:0)
            WIRE 464 656 496 656
            BEGIN DISPLAY 464 656 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH L0SDFIFOWrCnt(8:0)
            WIRE 464 720 496 720
            BEGIN DISPLAY 464 720 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH L0RxBP
            WIRE 944 464 976 464
            BEGIN DISPLAY 976 464 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH L0SDFIFOWr
            WIRE 944 528 976 528
            BEGIN DISPLAY 976 528 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH L0SDFIFODatIn(63:0)
            WIRE 944 592 976 592
            BEGIN DISPLAY 976 592 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH L0SDFIFODatOut(63:0)
            WIRE 2240 624 2272 624
            BEGIN DISPLAY 2272 624 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH LinkClk
            WIRE 1824 560 1856 560
            BEGIN DISPLAY 1824 560 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH L0SDFIFODatIn(63:0)
            WIRE 1824 432 1856 432
            BEGIN DISPLAY 1824 432 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH L0SDFIFOWr
            WIRE 1824 528 1856 528
            BEGIN DISPLAY 1824 528 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH L0SDFIFORd
            WIRE 1824 752 1856 752
            BEGIN DISPLAY 1824 752 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH BusClk
            WIRE 1824 784 1856 784
            BEGIN DISPLAY 1824 784 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH L0SDFIFOWrCnt(8:0)
            WIRE 2240 560 2272 560
            BEGIN DISPLAY 2272 560 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH L0SDFIFORdCnt(8:0)
            WIRE 2240 784 2272 784
            BEGIN DISPLAY 2272 784 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH SDL1FIFOWrCnt(8:0)
            WIRE 928 1168 960 1168
            BEGIN DISPLAY 960 1168 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH L0SDEF
            WIRE 2240 656 2272 656
            BEGIN DISPLAY 2272 656 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH L0RxDataValid
            WIRE 464 592 496 592
            BEGIN DISPLAY 464 592 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH SDL1FF
            WIRE 928 1040 960 1040
            BEGIN DISPLAY 960 1040 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH SDL1EF
            WIRE 928 1264 960 1264
            BEGIN DISPLAY 960 1264 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH L0SDFF
            WIRE 2240 432 2272 432
            BEGIN DISPLAY 2272 432 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN INSTANCE XLXI_1601 2880 1840 R0
        END INSTANCE
    END SHEET
    BEGIN SHEET 8 2720 1760
        BEGIN INSTANCE XLXI_744 2880 1840 R0
        END INSTANCE
        BEGIN BRANCH L2TxDat(7:0)
            WIRE 1632 496 1680 496
            BEGIN DISPLAY 1632 496 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH L2RxDat(7:0)
            WIRE 1632 560 1680 560
            BEGIN DISPLAY 1632 560 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH xv2_l2dat(7:0)
            WIRE 2016 528 2048 528
        END BRANCH
        BEGIN INSTANCE XLXI_569 1680 592 R0
        END INSTANCE
        INSTANCE XLXI_961 1424 464 R0
        BEGIN BRANCH L2TxEn
            WIRE 1392 432 1424 432
            BEGIN DISPLAY 1392 432 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH L2ClkOut
            WIRE 1712 208 1776 208
            BEGIN DISPLAY 1712 208 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        INSTANCE XLXI_962 1776 240 R0
        INSTANCE XLXI_469 1968 288 R180
        BEGIN BRANCH L2ClkIn
            WIRE 1712 320 1744 320
            BEGIN DISPLAY 1712 320 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN INSTANCE Link2 800 816 R0
        END INSTANCE
        BEGIN BRANCH L2ClkOut
            WIRE 1184 208 1216 208
            BEGIN DISPLAY 1216 208 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH L2TxFIFODat(7:0)
            WIRE 752 784 800 784
            BEGIN DISPLAY 752 784 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH L2TxDat(7:0)
            WIRE 1184 720 1216 720
            BEGIN DISPLAY 1216 720 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH L2ClkIn
            WIRE 768 272 800 272
            BEGIN DISPLAY 768 272 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH L2RxDat(7:0)
            WIRE 752 720 800 720
            BEGIN DISPLAY 752 720 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH L2TxWrEn
            WIRE 768 400 800 400
            BEGIN DISPLAY 768 400 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH L2TxBP
            WIRE 1184 336 1216 336
            BEGIN DISPLAY 1216 336 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH LinkClk
            WIRE 768 208 800 208
            BEGIN DISPLAY 768 208 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH Reset
            WIRE 768 336 800 336
            BEGIN DISPLAY 768 336 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH L2RxBP
            WIRE 768 592 800 592
            BEGIN DISPLAY 768 592 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH L2RxDataValid
            WIRE 1184 656 1216 656
            BEGIN DISPLAY 1216 656 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH L2TxEn
            WIRE 1184 272 1216 272
            BEGIN DISPLAY 1216 272 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH L2RxFIFODat(7:0)
            WIRE 1184 784 1216 784
            BEGIN DISPLAY 1216 784 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        INSTANCE XLXI_259 528 464 R0
        INSTANCE XLXI_258 528 656 R0
        BEGIN BRANCH L3TxFIFODat(7:0)
            WIRE 768 1552 784 1552
            WIRE 784 1552 800 1552
            BEGIN DISPLAY 768 1552 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH L3TxDat(7:0)
            WIRE 1184 1488 1200 1488
            WIRE 1200 1488 1216 1488
            BEGIN DISPLAY 1216 1488 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN INSTANCE Link3 800 1584 R0
        END INSTANCE
        BEGIN BRANCH L3ClkOut
            WIRE 1184 976 1200 976
            WIRE 1200 976 1216 976
            BEGIN DISPLAY 1216 976 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH L3ClkIn
            WIRE 768 1040 784 1040
            WIRE 784 1040 800 1040
            BEGIN DISPLAY 768 1040 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH L3RxDat(7:0)
            WIRE 768 1488 784 1488
            WIRE 784 1488 800 1488
            BEGIN DISPLAY 768 1488 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH L3TxWrEn
            WIRE 768 1168 784 1168
            WIRE 784 1168 800 1168
            BEGIN DISPLAY 768 1168 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH L3TxBP
            WIRE 1184 1104 1200 1104
            WIRE 1200 1104 1216 1104
            BEGIN DISPLAY 1216 1104 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH LinkClk
            WIRE 768 976 784 976
            WIRE 784 976 800 976
            BEGIN DISPLAY 768 976 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH Reset
            WIRE 768 1104 784 1104
            WIRE 784 1104 800 1104
            BEGIN DISPLAY 768 1104 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH L3TxEn
            WIRE 1184 1040 1200 1040
            WIRE 1200 1040 1216 1040
            BEGIN DISPLAY 1216 1040 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH L3RxBP
            WIRE 768 1360 784 1360
            WIRE 784 1360 800 1360
            BEGIN DISPLAY 768 1360 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        INSTANCE XLXI_263 528 1232 R0
        BEGIN BRANCH L3RxFIFODat(7:0)
            WIRE 1184 1552 1200 1552
            WIRE 1200 1552 1216 1552
            BEGIN DISPLAY 1216 1552 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH L3RxDataValid
            WIRE 1184 1424 1200 1424
            WIRE 1200 1424 1216 1424
            BEGIN DISPLAY 1216 1424 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN INSTANCE XLXI_585 1680 1344 R0
        END INSTANCE
        BEGIN BRANCH xv2_l3dat(7:0)
            WIRE 2016 1280 2048 1280
        END BRANCH
        BEGIN BRANCH L3TxDat(7:0)
            WIRE 1632 1248 1680 1248
            BEGIN DISPLAY 1632 1248 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH L3RxDat(7:0)
            WIRE 1632 1312 1680 1312
            BEGIN DISPLAY 1632 1312 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        INSTANCE XLXI_471 1968 1056 R180
        BEGIN BRANCH L3ClkIn
            WIRE 1712 1088 1744 1088
            BEGIN DISPLAY 1712 1088 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH L3ClkOut
            WIRE 1712 976 1776 976
            BEGIN DISPLAY 1712 976 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        INSTANCE XLXI_995 1776 1008 R0
        INSTANCE XLXI_1011 1424 1216 R0
        BEGIN BRANCH L3TxEn
            WIRE 1392 1184 1424 1184
            BEGIN DISPLAY 1392 1184 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        IOMARKER 2048 528 xv2_l2dat(7:0) R0 28
        IOMARKER 2048 1280 xv2_l3dat(7:0) R0 28
        BEGIN BRANCH XLXN_3746
            WIRE 592 528 640 528
            WIRE 640 528 800 528
            WIRE 640 528 640 656
            WIRE 640 656 800 656
        END BRANCH
        INSTANCE XLXI_262 528 1424 R0
        BEGIN BRANCH XLXN_3748
            WIRE 592 1296 640 1296
            WIRE 640 1296 800 1296
            WIRE 640 1296 640 1424
            WIRE 640 1424 800 1424
        END BRANCH
        BEGIN BRANCH XLXN_3750
            WIRE 592 1232 608 1232
            WIRE 608 1232 800 1232
        END BRANCH
        BEGIN BRANCH XLXN_3751
            WIRE 592 464 800 464
        END BRANCH
        BEGIN BRANCH XLXN_3762
            WIRE 1648 1184 1680 1184
        END BRANCH
        BEGIN BRANCH XLXN_3763
            WIRE 1648 432 1680 432
        END BRANCH
        BEGIN BRANCH xv2_l2ack
            WIRE 2000 208 2080 208
        END BRANCH
        IOMARKER 2080 208 xv2_l2ack R0 28
        BEGIN BRANCH xv2_l2clk
            WIRE 1968 320 2016 320
        END BRANCH
        IOMARKER 2016 320 xv2_l2clk R0 28
        BEGIN BRANCH xv2_l3ack
            WIRE 2000 976 2080 976
        END BRANCH
        IOMARKER 2080 976 xv2_l3ack R0 28
        BEGIN BRANCH xv2_l3clk
            WIRE 1968 1088 2016 1088
        END BRANCH
        IOMARKER 2016 1088 xv2_l3clk R0 28
    END SHEET
    BEGIN SHEET 9 2720 1760
        BEGIN INSTANCE XLXI_745 2880 1840 R0
        END INSTANCE
        INSTANCE L3L2FIFO 1136 800 R0
        BEGIN BRANCH Reset
            WIRE 1296 1312 1312 1312
            WIRE 1312 1312 1328 1312
            WIRE 1328 1280 1328 1312
            BEGIN DISPLAY 1296 1312 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH LinkClk
            WIRE 1104 1136 1120 1136
            WIRE 1120 1136 1136 1136
            BEGIN DISPLAY 1104 1136 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH L3L2FIFODatOut(15:0)
            WIRE 1520 848 1536 848
            WIRE 1536 848 1552 848
            BEGIN DISPLAY 1552 848 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH L3L2FIFOCnt(7:0)
            WIRE 1520 1168 1536 1168
            WIRE 1536 1168 1552 1168
            BEGIN DISPLAY 1552 1168 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH L3L2FIFODatIn(15:0)
            WIRE 1104 848 1120 848
            WIRE 1120 848 1136 848
            BEGIN DISPLAY 1104 848 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH L3L2FIFOWr
            WIRE 1104 912 1120 912
            WIRE 1120 912 1136 912
            BEGIN DISPLAY 1104 912 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH L3L2FIFORd
            WIRE 1104 944 1120 944
            WIRE 1120 944 1136 944
            BEGIN DISPLAY 1104 944 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        INSTANCE L2L3FIFO 1168 160 R0
        BEGIN BRANCH Reset
            WIRE 1328 672 1360 672
            WIRE 1360 640 1360 672
            BEGIN DISPLAY 1328 672 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH LinkClk
            WIRE 1136 496 1168 496
            BEGIN DISPLAY 1136 496 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH L2L3FIFODatIn(15:0)
            WIRE 1136 208 1168 208
            BEGIN DISPLAY 1136 208 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH L2L3FIFODatOut(15:0)
            WIRE 1552 208 1584 208
            BEGIN DISPLAY 1584 208 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH L2L3FIFOCnt(7:0)
            WIRE 1552 528 1584 528
            BEGIN DISPLAY 1584 528 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH L2L3FIFOWr
            WIRE 1136 272 1168 272
            BEGIN DISPLAY 1136 272 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH L2L3FIFORd
            WIRE 1136 304 1168 304
            BEGIN DISPLAY 1136 304 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH L2L3FIFORd
            WIRE 2448 240 2480 240
            BEGIN DISPLAY 2480 240 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH LinkClk
            WIRE 2000 240 2032 240
            BEGIN DISPLAY 2000 240 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH Reset
            WIRE 2000 304 2032 304
            BEGIN DISPLAY 2000 304 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN INSTANCE L2L3FtoL 2032 656 R0
        END INSTANCE
        BEGIN BRANCH L3TxWrEn
            WIRE 2448 368 2480 368
            BEGIN DISPLAY 2480 368 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH L2L3FIFODatOut(15:0)
            WIRE 1984 432 2000 432
            WIRE 2000 432 2032 432
            BEGIN DISPLAY 1984 432 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH L2L3FIFOCnt(7:0)
            WIRE 1984 496 2032 496
            BEGIN DISPLAY 1984 496 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH L3TxBP
            WIRE 2000 368 2032 368
            BEGIN DISPLAY 2000 368 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH L3TxFIFODat(7:0)
            WIRE 2448 304 2480 304
            BEGIN DISPLAY 2480 304 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN INSTANCE L2L3LtoF 288 528 R0
        END INSTANCE
        BEGIN BRANCH Reset
            WIRE 256 304 288 304
            BEGIN DISPLAY 256 304 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH LinkClk
            WIRE 256 240 288 240
            BEGIN DISPLAY 256 240 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH L2RxDataValid
            WIRE 256 368 288 368
            BEGIN DISPLAY 256 368 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH L2RxFIFODat(7:0)
            WIRE 256 432 288 432
            BEGIN DISPLAY 256 432 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH L2RxBP
            WIRE 736 240 768 240
            BEGIN DISPLAY 768 240 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH L2L3FIFOWr
            WIRE 736 304 768 304
            BEGIN DISPLAY 768 304 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH L2L3FIFODatIn(15:0)
            WIRE 736 368 768 368
            BEGIN DISPLAY 768 368 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH L2L3FIFOCnt(7:0)
            WIRE 256 496 288 496
            BEGIN DISPLAY 256 496 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN INSTANCE L3L2FtoL 2016 1296 R0
        END INSTANCE
        BEGIN BRANCH Reset
            WIRE 1984 944 2016 944
            BEGIN DISPLAY 1984 944 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH LinkClk
            WIRE 1984 880 2016 880
            BEGIN DISPLAY 1984 880 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH L3L2FIFORd
            WIRE 2432 880 2464 880
            BEGIN DISPLAY 2464 880 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH L3L2FIFODatOut(15:0)
            WIRE 1984 1072 2016 1072
            BEGIN DISPLAY 1984 1072 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH L3L2FIFOCnt(7:0)
            WIRE 1984 1136 2016 1136
            BEGIN DISPLAY 1984 1136 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH L2TxBP
            WIRE 1984 1008 2016 1008
            BEGIN DISPLAY 1984 1008 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH L2TxFIFODat(7:0)
            WIRE 2432 944 2480 944
            BEGIN DISPLAY 2480 944 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH L2TxWrEn
            WIRE 2432 1008 2464 1008
            BEGIN DISPLAY 2464 1008 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN INSTANCE L3L2LtoF 288 1184 R0
        END INSTANCE
        BEGIN BRANCH Reset
            WIRE 256 960 288 960
            BEGIN DISPLAY 256 960 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH LinkClk
            WIRE 256 896 288 896
            BEGIN DISPLAY 256 896 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH L3L2FIFOWr
            WIRE 736 960 768 960
            BEGIN DISPLAY 768 960 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH L3L2FIFODatIn(15:0)
            WIRE 736 1024 768 1024
            BEGIN DISPLAY 768 1024 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH L3L2FIFOCnt(7:0)
            WIRE 256 1152 288 1152
            BEGIN DISPLAY 256 1152 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH L3RxFIFODat(7:0)
            WIRE 256 1088 288 1088
            BEGIN DISPLAY 256 1088 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH L3RxDataValid
            WIRE 256 1024 288 1024
            BEGIN DISPLAY 256 1024 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH L3RxBP
            WIRE 736 896 768 896
            BEGIN DISPLAY 768 896 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH L3L2EF
            WIRE 1520 944 1552 944
            BEGIN DISPLAY 1552 944 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH L3L2FF
            WIRE 1520 912 1552 912
            BEGIN DISPLAY 1552 912 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH L2L3FF
            WIRE 1552 272 1584 272
            BEGIN DISPLAY 1584 272 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH L2L3EF
            WIRE 1552 304 1584 304
            BEGIN DISPLAY 1584 304 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
    END SHEET
    BEGIN SHEET 10 2720 1760
        BEGIN INSTANCE XLXI_748 2880 1840 R0
        END INSTANCE
        BEGIN BRANCH FPIOOE_l(63:32)
            WIRE 1904 832 1920 832
            WIRE 1920 832 1936 832
            BEGIN DISPLAY 1904 832 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH FPIOOut(63:32)
            WIRE 1904 896 1920 896
            WIRE 1920 896 1936 896
            BEGIN DISPLAY 1904 896 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH FPIOIn(63:32)
            WIRE 1904 960 1920 960
            WIRE 1920 960 1936 960
            BEGIN DISPLAY 1904 960 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN INSTANCE XLXI_1465 1936 992 R0
        END INSTANCE
        INSTANCE XLXI_1635 1984 1152 R0
        BEGIN BRANCH xexep_n1
            WIRE 1952 1120 1984 1120
        END BRANCH
        BEGIN BRANCH FPIOCorner(2)
            WIRE 2208 1120 2272 1120
            BEGIN DISPLAY 2272 1120 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        INSTANCE XLXI_1636 1984 1232 R0
        BEGIN BRANCH xexep_n34
            WIRE 1952 1200 1984 1200
        END BRANCH
        BEGIN BRANCH FPIOCorner(3)
            WIRE 2208 1200 2272 1200
            BEGIN DISPLAY 2272 1200 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN INSTANCE XLXI_1464 1936 336 R0
        END INSTANCE
        BEGIN BRANCH FPIOOE_l(31:0)
            WIRE 1904 176 1936 176
            BEGIN DISPLAY 1904 176 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH FPIOOut(31:0)
            WIRE 1904 240 1936 240
            BEGIN DISPLAY 1904 240 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH FPIOIn(31:0)
            WIRE 1904 304 1936 304
            BEGIN DISPLAY 1904 304 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        INSTANCE XLXI_1634 2016 496 R0
        BEGIN BRANCH xexep_p1
            WIRE 1984 464 2016 464
        END BRANCH
        BEGIN BRANCH FPIOCorner(0)
            WIRE 2240 464 2304 464
            BEGIN DISPLAY 2304 464 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        INSTANCE XLXI_1633 2016 576 R0
        BEGIN BRANCH xexep_p34
            WIRE 1984 544 2016 544
        END BRANCH
        BEGIN BRANCH FPIOCorner(1)
            WIRE 2240 544 2304 544
            BEGIN DISPLAY 2304 544 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        IOMARKER 1984 464 xexep_p1 R180 28
        IOMARKER 1984 544 xexep_p34 R180 28
        IOMARKER 1952 1120 xexep_n1 R180 28
        IOMARKER 1952 1200 xexep_n34 R180 28
        BEGIN INSTANCE IO_Registers 512 400 R0
        END INSTANCE
        BEGIN BRANCH FPIOOut(63:0)
            WIRE 1200 496 1232 496
            BEGIN DISPLAY 1232 496 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH FPIOOE_l(63:0)
            WIRE 1200 704 1232 704
            BEGIN DISPLAY 1232 704 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH Hdr1IOOut(18:0)
            WIRE 1200 912 1232 912
            BEGIN DISPLAY 1232 912 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH Hdr1IOOE_l(18:0)
            WIRE 1200 1040 1232 1040
            BEGIN DISPLAY 1232 1040 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH Hdr2IOOut(18:0)
            WIRE 1200 1168 1232 1168
            BEGIN DISPLAY 1232 1168 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH Hdr2IOOE_l(18:0)
            WIRE 1200 1296 1232 1296
            BEGIN DISPLAY 1232 1296 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH BusClk
            WIRE 480 400 512 400
            BEGIN DISPLAY 480 400 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH Reset
            WIRE 480 432 512 432
            BEGIN DISPLAY 480 432 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH FPIOIn(63:0)
            WIRE 480 496 512 496
            BEGIN DISPLAY 480 496 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH Hdr2IOEnRegWr
            WIRE 480 1312 512 1312
            BEGIN DISPLAY 480 1312 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH Hdr2IORWRegWr
            WIRE 480 1216 512 1216
            BEGIN DISPLAY 480 1216 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH Hdr2IOIn(18:0)
            WIRE 480 1168 496 1168
            WIRE 496 1168 512 1168
            BEGIN DISPLAY 480 1168 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH Hdr1IOEnRegWr
            WIRE 480 1056 512 1056
            BEGIN DISPLAY 480 1056 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH Hdr1IOIn(18:0)
            WIRE 480 912 496 912
            WIRE 496 912 512 912
            BEGIN DISPLAY 480 912 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH Hdr1IORWRegWr
            WIRE 480 960 512 960
            BEGIN DISPLAY 480 960 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH FPIOEnRegWrH
            WIRE 480 800 512 800
            BEGIN DISPLAY 480 800 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH FPIOEnRegWrL
            WIRE 480 720 512 720
            BEGIN DISPLAY 480 720 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH FPIORWRegWrH
            WIRE 480 624 512 624
            BEGIN DISPLAY 480 624 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH FPIORWRegWrL
            WIRE 480 544 512 544
            BEGIN DISPLAY 480 544 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH Hdr2IOEnDatOut(31:0)
            WIRE 1200 1344 1232 1344
            BEGIN DISPLAY 1232 1344 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH Hdr2IORWDatOut(31:0)
            WIRE 1200 1248 1232 1248
            BEGIN DISPLAY 1232 1248 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH Hdr1IOEnDatOut(31:0)
            WIRE 1200 1088 1232 1088
            BEGIN DISPLAY 1232 1088 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH Hdr1IORWDatOut(31:0)
            WIRE 1200 992 1232 992
            BEGIN DISPLAY 1232 992 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH FPIOEnDatOutH(31:0)
            WIRE 1200 832 1232 832
            BEGIN DISPLAY 1232 832 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH FPIOEnDatOutL(31:0)
            WIRE 1200 752 1232 752
            BEGIN DISPLAY 1232 752 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH FPIORWDatOutH(31:0)
            WIRE 1200 656 1232 656
            BEGIN DISPLAY 1232 656 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH FPIORWDatOutL(31:0)
            WIRE 1200 576 1232 576
            BEGIN DISPLAY 1232 576 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH Hdr2IOEnDatIn(31:0)
            WIRE 480 1344 512 1344
            BEGIN DISPLAY 480 1344 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH Hdr2IORWDatIn(31:0)
            WIRE 480 1248 512 1248
            BEGIN DISPLAY 480 1248 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH Hdr1IOEnDatIn(31:0)
            WIRE 480 1088 512 1088
            BEGIN DISPLAY 480 1088 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH Hdr1IORWDatIn(31:0)
            WIRE 480 992 512 992
            BEGIN DISPLAY 480 992 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH FPIOEnDatInH(31:0)
            WIRE 480 832 512 832
            BEGIN DISPLAY 480 832 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH FPIOEnDatInL(31:0)
            WIRE 480 752 512 752
            BEGIN DISPLAY 480 752 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH FPIORWDatInH(31:0)
            WIRE 480 656 512 656
            BEGIN DISPLAY 480 656 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH FPIORWDatInL(31:0)
            WIRE 480 576 512 576
            BEGIN DISPLAY 480 576 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
    END SHEET
    BEGIN SHEET 11 2720 1760
        BEGIN INSTANCE XLXI_749 2880 1840 R0
        END INSTANCE
        BEGIN BRANCH Hdr1IOOut(15:0)
            WIRE 1760 464 1840 464
            BEGIN DISPLAY 1760 464 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH xhdr_d(15:00)
            WIRE 2176 496 2224 496
        END BRANCH
        BEGIN BRANCH Hdr1IOIn(15:0)
            WIRE 1760 528 1840 528
            BEGIN DISPLAY 1760 528 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH Hdr1IOOE_l(15:0)
            WIRE 1760 400 1840 400
            BEGIN DISPLAY 1760 400 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        INSTANCE XLXI_1372 1936 1088 R0
        BEGIN BRANCH Hdr1IOOE_l(17)
            WIRE 1888 896 1936 896
            BEGIN DISPLAY 1888 896 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH Hdr1IOOut(17)
            WIRE 1888 960 1936 960
            BEGIN DISPLAY 1888 960 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH Hdr1IOIn(17)
            WIRE 1888 1024 1936 1024
            BEGIN DISPLAY 1888 1024 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH xhdr_ack
            WIRE 2160 752 2208 752
        END BRANCH
        INSTANCE XLXI_43 1936 880 R0
        BEGIN BRANCH Hdr1IOOE_l(16)
            WIRE 1904 688 1936 688
            BEGIN DISPLAY 1904 688 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH Hdr1IOOut(16)
            WIRE 1904 752 1936 752
            BEGIN DISPLAY 1904 752 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH Hdr1IOIn(16)
            WIRE 1904 816 1936 816
            BEGIN DISPLAY 1904 816 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        INSTANCE XLXI_1373 1936 1296 R0
        BEGIN BRANCH xhdr_clk
            WIRE 2160 1168 2208 1168
        END BRANCH
        BEGIN BRANCH Hdr1IOOE_l(18)
            WIRE 1888 1104 1936 1104
            BEGIN DISPLAY 1888 1104 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH Hdr1IOOut(18)
            WIRE 1888 1168 1936 1168
            BEGIN DISPLAY 1888 1168 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH Hdr1IOIn(18)
            WIRE 1888 1232 1936 1232
            BEGIN DISPLAY 1888 1232 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH Hdr2IOOut(15:0)
            WIRE 640 448 720 448
            BEGIN DISPLAY 640 448 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH Hdr2IOIn(15:0)
            WIRE 640 512 720 512
            BEGIN DISPLAY 640 512 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH Hdr2IOOE_l(15:0)
            WIRE 640 384 720 384
            BEGIN DISPLAY 640 384 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        INSTANCE XLXI_1417 816 1072 R0
        BEGIN BRANCH Hdr2IOOE_l(17)
            WIRE 768 880 816 880
            BEGIN DISPLAY 768 880 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH Hdr2IOOut(17)
            WIRE 768 944 816 944
            BEGIN DISPLAY 768 944 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH Hdr2IOIn(17)
            WIRE 768 1008 816 1008
            BEGIN DISPLAY 768 1008 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        INSTANCE XLXI_1418 816 864 R0
        BEGIN BRANCH Hdr2IOOE_l(16)
            WIRE 784 672 816 672
            BEGIN DISPLAY 784 672 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH Hdr2IOOut(16)
            WIRE 784 736 816 736
            BEGIN DISPLAY 784 736 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH Hdr2IOIn(16)
            WIRE 784 800 816 800
            BEGIN DISPLAY 784 800 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        INSTANCE XLXI_1419 816 1280 R0
        BEGIN BRANCH Hdr2IOOE_l(18)
            WIRE 768 1088 816 1088
            BEGIN DISPLAY 768 1088 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH Hdr2IOOut(18)
            WIRE 768 1152 816 1152
            BEGIN DISPLAY 768 1152 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH Hdr2IOIn(18)
            WIRE 768 1216 816 1216
            BEGIN DISPLAY 768 1216 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH xhdr2_d(15:00)
            WIRE 1056 480 1072 480
            WIRE 1072 480 1088 480
        END BRANCH
        BEGIN BRANCH xhdr2_ack
            WIRE 1040 736 1056 736
            WIRE 1056 736 1072 736
            WIRE 1072 736 1088 736
        END BRANCH
        BEGIN BRANCH xhdr2_dir
            WIRE 1040 944 1056 944
            WIRE 1056 944 1072 944
            WIRE 1072 944 1088 944
        END BRANCH
        BEGIN BRANCH xhdr2_clk
            WIRE 1040 1152 1056 1152
            WIRE 1056 1152 1072 1152
            WIRE 1072 1152 1088 1152
        END BRANCH
        BEGIN BRANCH xhdr_dir
            WIRE 2160 960 2208 960
        END BRANCH
        IOMARKER 2224 496 xhdr_d(15:00) R0 28
        IOMARKER 2208 752 xhdr_ack R0 28
        IOMARKER 2208 1168 xhdr_clk R0 28
        IOMARKER 2208 960 xhdr_dir R0 28
        IOMARKER 1088 480 xhdr2_d(15:00) R0 28
        IOMARKER 1088 736 xhdr2_ack R0 28
        IOMARKER 1088 944 xhdr2_dir R0 28
        IOMARKER 1088 1152 xhdr2_clk R0 28
        BEGIN INSTANCE XLXI_1462 720 544 R0
        END INSTANCE
        BEGIN INSTANCE XLXI_1463 1840 560 R0
        END INSTANCE
    END SHEET
    BEGIN SHEET 12 2720 1760
        BEGIN INSTANCE XLXI_750 2880 1840 R0
        END INSTANCE
        BEGIN BRANCH xsw_usr(3:0)
            WIRE 1728 160 1744 160
        END BRANCH
        INSTANCE XLXI_45(3:0) 1744 192 R0
        BEGIN BRANCH UserSw(3:0)
            WIRE 1968 160 2000 160
            BEGIN DISPLAY 2000 160 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH xuser_led_1
            WIRE 1968 272 2000 272
        END BRANCH
        BEGIN BRANCH xuser_led_2
            WIRE 1968 352 2000 352
        END BRANCH
        BEGIN BRANCH UserLED(0)
            WIRE 1712 272 1744 272
            BEGIN DISPLAY 1712 272 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH UserLED(1)
            WIRE 1712 352 1744 352
            BEGIN DISPLAY 1712 352 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        INSTANCE XLXI_47 1744 384 R0
        INSTANCE XLXI_48 1744 304 R0
        INSTANCE XLXI_1390 1824 736 R0
        BEGIN BRANCH xpa_scl
            WIRE 2048 608 2096 608
        END BRANCH
        BEGIN BRANCH PASClOE_l
            WIRE 1792 544 1824 544
            BEGIN DISPLAY 1792 544 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH XLXN_4369
            WIRE 1664 608 1664 640
            WIRE 1664 608 1824 608
        END BRANCH
        BEGIN BRANCH PASClIn
            WIRE 1792 672 1824 672
            BEGIN DISPLAY 1792 672 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        INSTANCE XLXI_1395 1600 768 R0
        INSTANCE XLXI_1391 1840 1008 R0
        BEGIN BRANCH PASDaOE_l
            WIRE 1808 816 1840 816
            BEGIN DISPLAY 1808 816 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH PASDaIn
            WIRE 1808 944 1840 944
            BEGIN DISPLAY 1808 944 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH xpa_sda
            WIRE 2064 880 2096 880
        END BRANCH
        BEGIN BRANCH xflag1
            WIRE 2000 1360 2048 1360
        END BRANCH
        INSTANCE XLXI_143 1776 1392 R0
        BEGIN BRANCH xflag0
            WIRE 2000 1216 2048 1216
        END BRANCH
        INSTANCE XLXI_142 1776 1248 R0
        BEGIN BRANCH Hdr1IOIn(16)
            WIRE 1728 1216 1776 1216
            BEGIN DISPLAY 1728 1216 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH Hdr2IOIn(16)
            WIRE 1728 1360 1776 1360
            BEGIN DISPLAY 1728 1360 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        IOMARKER 1728 160 xsw_usr(3:0) R180 28
        IOMARKER 2000 272 xuser_led_1 R0 28
        IOMARKER 2000 352 xuser_led_2 R0 28
        IOMARKER 2096 880 xpa_sda R0 28
        IOMARKER 2096 608 xpa_scl R0 28
        IOMARKER 2048 1216 xflag0 R0 28
        IOMARKER 2048 1360 xflag1 R0 28
        BEGIN BRANCH XLXN_4372
            WIRE 1664 880 1664 912
            WIRE 1664 880 1840 880
        END BRANCH
        INSTANCE XLXI_1394 1600 1040 R0
        BEGIN INSTANCE Misc_Stat_Regs 624 416 R0
        END INSTANCE
        BEGIN BRANCH MiscRegDatIn(31:0)
            WIRE 592 528 624 528
            BEGIN DISPLAY 592 528 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH UserSw(3:0)
            WIRE 592 1008 624 1008
            BEGIN DISPLAY 592 1008 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH FPIOCorner(3:0)
            WIRE 592 1056 624 1056
            BEGIN DISPLAY 592 1056 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH PASClIn
            WIRE 592 928 624 928
            BEGIN DISPLAY 592 928 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH PASDaIn
            WIRE 592 960 624 960
            BEGIN DISPLAY 592 960 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH L0SDEF
            WIRE 592 608 624 608
            BEGIN DISPLAY 592 608 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH L0SDFF
            WIRE 592 640 624 640
            BEGIN DISPLAY 592 640 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH SDL1EF
            WIRE 592 688 624 688
            BEGIN DISPLAY 592 688 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH SDL1FF
            WIRE 592 720 624 720
            BEGIN DISPLAY 592 720 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH L2L3EF
            WIRE 592 768 624 768
            BEGIN DISPLAY 592 768 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH L2L3FF
            WIRE 592 800 624 800
            BEGIN DISPLAY 592 800 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH L3L2EF
            WIRE 592 848 624 848
            BEGIN DISPLAY 592 848 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH L3L2FF
            WIRE 592 880 624 880
            BEGIN DISPLAY 592 880 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH BusClk
            WIRE 592 416 624 416
            BEGIN DISPLAY 592 416 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH Reset
            WIRE 592 448 624 448
            BEGIN DISPLAY 592 448 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH MiscRegWr
            WIRE 592 496 624 496
            BEGIN DISPLAY 592 496 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH UserLED(1:0)
            WIRE 1232 1008 1264 1008
            BEGIN DISPLAY 1264 1008 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH PASClOE_l
            WIRE 1232 928 1264 928
            BEGIN DISPLAY 1264 928 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH PASDaOE_l
            WIRE 1232 960 1264 960
            BEGIN DISPLAY 1264 960 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH HBGEn_l
            WIRE 1232 416 1264 416
            BEGIN DISPLAY 1264 416 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH HBGSyncPre
            WIRE 1232 448 1264 448
            BEGIN DISPLAY 1264 448 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH MiscRegDatOut(31:0)
            WIRE 1232 528 1264 528
            BEGIN DISPLAY 1264 528 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH StatRegDatOut(31:0)
            WIRE 1232 592 1264 592
            BEGIN DISPLAY 1264 592 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
    END SHEET
    BEGIN SHEET 13 5440 3520
        BEGIN BRANCH TimerDataOut(31:0)
            WIRE 768 192 784 192
            WIRE 784 192 800 192
            BEGIN DISPLAY 800 192 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH TimerAddrOut(31:0)
            WIRE 768 144 784 144
            WIRE 784 144 800 144
            BEGIN DISPLAY 800 144 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH AddrOutTimer(2:0)
            WIRE 768 272 784 272
            WIRE 784 272 800 272
            BEGIN DISPLAY 800 272 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH WR
            WIRE 768 320 784 320
            WIRE 784 320 800 320
            BEGIN DISPLAY 800 320 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH TimerSelect(19:0)
            WIRE 768 432 784 432
            WIRE 784 432 800 432
            BEGIN DISPLAY 800 432 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH GlobalEnable
            WIRE 768 368 784 368
            WIRE 784 368 800 368
            BEGIN DISPLAY 800 368 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH BusClk
            WIRE 256 160 272 160
            WIRE 272 160 304 160
            BEGIN DISPLAY 256 160 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH Reset
            WIRE 256 192 272 192
            WIRE 272 192 304 192
            BEGIN DISPLAY 256 192 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH TimerAddrWr
            WIRE 256 288 272 288
            WIRE 272 288 304 288
            BEGIN DISPLAY 256 288 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH TimerAddrIn(31:0)
            WIRE 256 320 272 320
            WIRE 272 320 304 320
            BEGIN DISPLAY 256 320 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH TimerDataWr
            WIRE 256 416 272 416
            WIRE 272 416 304 416
            BEGIN DISPLAY 256 416 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH TimerDataIn(31:0)
            WIRE 256 448 272 448
            WIRE 272 448 304 448
            BEGIN DISPLAY 256 448 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH AddrTrigger
            WIRE 768 496 784 496
            WIRE 784 496 800 496
            BEGIN DISPLAY 800 496 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN INSTANCE TimerIface 144 560 R0
        END INSTANCE
        BEGIN BRANCH Timer_Rst
            WIRE 768 528 800 528
            BEGIN DISPLAY 800 528 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH xexep_p7
            WIRE 272 640 304 640
        END BRANCH
        BEGIN BRANCH XLXN_6437
            WIRE 528 640 544 640
        END BRANCH
        INSTANCE XLXI_1837 304 672 R0
        BEGIN BRANCH OnePPS
            WIRE 768 640 784 640
            BEGIN DISPLAY 784 640 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        INSTANCE XLXI_1838 544 672 R0
        INSTANCE XLXI_1858 304 752 R0
        BEGIN BRANCH xexep_n8
            WIRE 528 720 544 720
            WIRE 544 720 752 720
        END BRANCH
        BEGIN BRANCH XLXN_6497
            WIRE 288 720 304 720
        END BRANCH
        INSTANCE XLXI_1855 160 656 R90
        INSTANCE XLXI_1848 304 832 R0
        BEGIN BRANCH xexep_p6
            WIRE 528 800 544 800
            WIRE 544 800 752 800
        END BRANCH
        BEGIN BRANCH XLXN_6479
            WIRE 288 800 304 800
        END BRANCH
        INSTANCE XLXI_1846 160 736 R90
        INSTANCE XLXI_1849 304 912 R0
        BEGIN BRANCH xexep_p30
            WIRE 528 880 544 880
            WIRE 544 880 752 880
        END BRANCH
        BEGIN BRANCH XLXN_6480
            WIRE 288 880 304 880
        END BRANCH
        INSTANCE XLXI_1847 160 816 R90
        INSTANCE XLXI_1860 304 992 R0
        BEGIN BRANCH xexep_n29
            WIRE 528 960 544 960
            WIRE 544 960 752 960
        END BRANCH
        BEGIN BRANCH XLXN_6501
            WIRE 288 960 304 960
        END BRANCH
        INSTANCE XLXI_1859 160 896 R90
        INSTANCE XLXI_1924 304 1072 R0
        BEGIN BRANCH xexep_p27
            WIRE 528 1040 544 1040
            WIRE 544 1040 752 1040
        END BRANCH
        BEGIN BRANCH XLXN_6655
            WIRE 288 1040 304 1040
        END BRANCH
        INSTANCE XLXI_1925 160 976 R90
        IOMARKER 272 640 xexep_p7 R180 28
        IOMARKER 752 720 xexep_n8 R0 28
        IOMARKER 752 800 xexep_p6 R0 28
        IOMARKER 752 880 xexep_p30 R0 28
        IOMARKER 752 960 xexep_n29 R0 28
        IOMARKER 752 1040 xexep_p27 R0 28
        BEGIN INSTANCE Timer_2 2176 560 R0
        END INSTANCE
        INSTANCE XLXI_1827 2576 176 R90
        BEGIN INSTANCE Timer7 2192 1200 R0
        END INSTANCE
        INSTANCE XLXI_2017 2592 816 R90
        BEGIN INSTANCE Timer12 2192 1856 R0
        END INSTANCE
        INSTANCE XLXI_2067 2592 1472 R90
        BEGIN INSTANCE Timer17 2192 2496 R0
        END INSTANCE
        INSTANCE XLXI_2073 2592 2112 R90
        BEGIN INSTANCE Timer3 2976 576 R0
        END INSTANCE
        INSTANCE XLXI_1967 3376 192 R90
        BEGIN INSTANCE Timer8 2976 1216 R0
        END INSTANCE
        INSTANCE XLXI_2019 3376 832 R90
        BEGIN INSTANCE Timer13 2976 1872 R0
        END INSTANCE
        INSTANCE XLXI_2069 3376 1488 R90
        BEGIN INSTANCE Timer18 2976 2512 R0
        END INSTANCE
        INSTANCE XLXI_2075 3376 2128 R90
        BEGIN INSTANCE Timer4 3776 592 R0
        END INSTANCE
        INSTANCE XLXI_2021 4176 208 R90
        INSTANCE XLXI_2025 4176 848 R90
        BEGIN INSTANCE Timer9 3776 1232 R0
        END INSTANCE
        BEGIN INSTANCE Timer14 3792 1888 R0
        END INSTANCE
        INSTANCE XLXI_2077 4192 1504 R90
        INSTANCE XLXI_2082 4192 2144 R90
        BEGIN INSTANCE Timer19 3792 2528 R0
        END INSTANCE
        BEGIN BRANCH TimerSelect(1)
            WIRE 2144 144 2176 144
            BEGIN DISPLAY 2144 144 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH BusClk
            WIRE 2144 240 2176 240
            BEGIN DISPLAY 2144 240 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH TimerClk
            WIRE 2144 288 2176 288
            BEGIN DISPLAY 2144 288 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH AddrTrigger
            WIRE 2144 624 2176 624
            BEGIN DISPLAY 2144 624 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH WR
            WIRE 2144 384 2176 384
            BEGIN DISPLAY 2144 384 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH GlobalEnable
            WIRE 2144 336 2176 336
            BEGIN DISPLAY 2144 336 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH TimerDataOut(31:0)
            WIRE 2144 512 2176 512
            BEGIN DISPLAY 2144 512 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH OnePPS
            WIRE 2144 576 2176 576
            BEGIN DISPLAY 2144 576 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH AddrOutTimer(2:0)
            WIRE 2144 448 2176 448
            BEGIN DISPLAY 2144 448 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH Timer_Rst
            WIRE 2144 192 2176 192
            BEGIN DISPLAY 2144 192 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH xexep_p28
            WIRE 2608 400 2608 416
            WIRE 2608 416 2608 480
        END BRANCH
        BEGIN BRANCH XLXN_6662
            WIRE 2576 144 2608 144
            WIRE 2608 144 2608 176
        END BRANCH
        BEGIN BRANCH TimerSelect(6)
            WIRE 2160 784 2192 784
            BEGIN DISPLAY 2160 784 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH BusClk
            WIRE 2160 880 2192 880
            BEGIN DISPLAY 2160 880 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH TimerClk
            WIRE 2160 928 2192 928
            BEGIN DISPLAY 2160 928 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH AddrTrigger
            WIRE 2160 1264 2192 1264
            BEGIN DISPLAY 2160 1264 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH WR
            WIRE 2160 1024 2192 1024
            BEGIN DISPLAY 2160 1024 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH GlobalEnable
            WIRE 2160 976 2192 976
            BEGIN DISPLAY 2160 976 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH TimerDataOut(31:0)
            WIRE 2160 1152 2192 1152
            BEGIN DISPLAY 2160 1152 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH OnePPS
            WIRE 2160 1216 2192 1216
            BEGIN DISPLAY 2160 1216 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH AddrOutTimer(2:0)
            WIRE 2160 1088 2192 1088
            BEGIN DISPLAY 2160 1088 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH Timer_Rst
            WIRE 2160 832 2192 832
            BEGIN DISPLAY 2160 832 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH xexep_n21
            WIRE 2624 1040 2624 1056
            WIRE 2624 1056 2624 1120
        END BRANCH
        BEGIN BRANCH XLXN_7137
            WIRE 2592 784 2624 784
            WIRE 2624 784 2624 816
        END BRANCH
        BEGIN BRANCH TimerSelect(11)
            WIRE 2160 1440 2192 1440
            BEGIN DISPLAY 2160 1440 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH BusClk
            WIRE 2160 1536 2192 1536
            BEGIN DISPLAY 2160 1536 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH TimerClk
            WIRE 2160 1584 2192 1584
            BEGIN DISPLAY 2160 1584 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH AddrTrigger
            WIRE 2160 1920 2192 1920
            BEGIN DISPLAY 2160 1920 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH WR
            WIRE 2160 1680 2192 1680
            BEGIN DISPLAY 2160 1680 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH GlobalEnable
            WIRE 2160 1632 2192 1632
            BEGIN DISPLAY 2160 1632 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH TimerDataOut(31:0)
            WIRE 2160 1808 2192 1808
            BEGIN DISPLAY 2160 1808 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH OnePPS
            WIRE 2160 1872 2192 1872
            BEGIN DISPLAY 2160 1872 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH AddrOutTimer(2:0)
            WIRE 2160 1744 2192 1744
            BEGIN DISPLAY 2160 1744 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH Timer_Rst
            WIRE 2160 1488 2192 1488
            BEGIN DISPLAY 2160 1488 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH xexep_n28
            WIRE 2624 1696 2624 1712
            WIRE 2624 1712 2624 1776
        END BRANCH
        BEGIN BRANCH XLXN_7437
            WIRE 2592 1440 2624 1440
            WIRE 2624 1440 2624 1472
        END BRANCH
        BEGIN BRANCH TimerSelect(16)
            WIRE 2160 2080 2192 2080
            BEGIN DISPLAY 2160 2080 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH BusClk
            WIRE 2160 2176 2192 2176
            BEGIN DISPLAY 2160 2176 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH TimerClk
            WIRE 2160 2224 2192 2224
            BEGIN DISPLAY 2160 2224 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH AddrTrigger
            WIRE 2160 2560 2192 2560
            BEGIN DISPLAY 2160 2560 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH WR
            WIRE 2160 2320 2192 2320
            BEGIN DISPLAY 2160 2320 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH GlobalEnable
            WIRE 2160 2272 2192 2272
            BEGIN DISPLAY 2160 2272 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH TimerDataOut(31:0)
            WIRE 2160 2448 2192 2448
            BEGIN DISPLAY 2160 2448 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH OnePPS
            WIRE 2160 2512 2192 2512
            BEGIN DISPLAY 2160 2512 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH AddrOutTimer(2:0)
            WIRE 2160 2384 2192 2384
            BEGIN DISPLAY 2160 2384 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH Timer_Rst
            WIRE 2160 2128 2192 2128
            BEGIN DISPLAY 2160 2128 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH xexep_p20
            WIRE 2624 2336 2624 2352
            WIRE 2624 2352 2624 2416
        END BRANCH
        BEGIN BRANCH XLXN_7473
            WIRE 2592 2080 2624 2080
            WIRE 2624 2080 2624 2112
        END BRANCH
        BEGIN BRANCH TimerSelect(2)
            WIRE 2944 160 2976 160
            BEGIN DISPLAY 2944 160 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH BusClk
            WIRE 2944 256 2976 256
            BEGIN DISPLAY 2944 256 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH TimerClk
            WIRE 2944 304 2976 304
            BEGIN DISPLAY 2944 304 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH AddrTrigger
            WIRE 2944 640 2976 640
            BEGIN DISPLAY 2944 640 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH WR
            WIRE 2944 400 2976 400
            BEGIN DISPLAY 2944 400 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH GlobalEnable
            WIRE 2944 352 2976 352
            BEGIN DISPLAY 2944 352 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH TimerDataOut(31:0)
            WIRE 2944 528 2976 528
            BEGIN DISPLAY 2944 528 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH OnePPS
            WIRE 2944 592 2976 592
            BEGIN DISPLAY 2944 592 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH AddrOutTimer(2:0)
            WIRE 2944 464 2976 464
            BEGIN DISPLAY 2944 464 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH Timer_Rst
            WIRE 2944 208 2976 208
            BEGIN DISPLAY 2944 208 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH xexep_n27
            WIRE 3408 416 3408 432
            WIRE 3408 432 3408 496
        END BRANCH
        BEGIN BRANCH XLXN_6837
            WIRE 3376 160 3408 160
            WIRE 3408 160 3408 192
        END BRANCH
        BEGIN BRANCH TimerSelect(7)
            WIRE 2944 800 2976 800
            BEGIN DISPLAY 2944 800 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH BusClk
            WIRE 2944 896 2976 896
            BEGIN DISPLAY 2944 896 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH TimerClk
            WIRE 2944 944 2976 944
            BEGIN DISPLAY 2944 944 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH AddrTrigger
            WIRE 2944 1280 2976 1280
            BEGIN DISPLAY 2944 1280 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH WR
            WIRE 2944 1040 2976 1040
            BEGIN DISPLAY 2944 1040 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH GlobalEnable
            WIRE 2944 992 2976 992
            BEGIN DISPLAY 2944 992 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH TimerDataOut(31:0)
            WIRE 2944 1168 2976 1168
            BEGIN DISPLAY 2944 1168 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH OnePPS
            WIRE 2944 1232 2976 1232
            BEGIN DISPLAY 2944 1232 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH AddrOutTimer(2:0)
            WIRE 2944 1104 2976 1104
            BEGIN DISPLAY 2944 1104 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH Timer_Rst
            WIRE 2944 848 2976 848
            BEGIN DISPLAY 2944 848 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH xexep_p16
            WIRE 3408 1056 3408 1072
            WIRE 3408 1072 3408 1136
        END BRANCH
        BEGIN BRANCH XLXN_7149
            WIRE 3376 800 3408 800
            WIRE 3408 800 3408 832
        END BRANCH
        BEGIN BRANCH TimerSelect(12)
            WIRE 2944 1456 2976 1456
            BEGIN DISPLAY 2944 1456 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH BusClk
            WIRE 2944 1552 2976 1552
            BEGIN DISPLAY 2944 1552 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH TimerClk
            WIRE 2944 1600 2976 1600
            BEGIN DISPLAY 2944 1600 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH AddrTrigger
            WIRE 2944 1936 2976 1936
            BEGIN DISPLAY 2944 1936 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH WR
            WIRE 2944 1696 2976 1696
            BEGIN DISPLAY 2944 1696 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH GlobalEnable
            WIRE 2944 1648 2976 1648
            BEGIN DISPLAY 2944 1648 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH TimerDataOut(31:0)
            WIRE 2944 1824 2976 1824
            BEGIN DISPLAY 2944 1824 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH OnePPS
            WIRE 2944 1888 2976 1888
            BEGIN DISPLAY 2944 1888 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH AddrOutTimer(2:0)
            WIRE 2944 1760 2976 1760
            BEGIN DISPLAY 2944 1760 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH Timer_Rst
            WIRE 2944 1504 2976 1504
            BEGIN DISPLAY 2944 1504 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH xexep_p26
            WIRE 3408 1712 3408 1728
            WIRE 3408 1728 3408 1792
        END BRANCH
        BEGIN BRANCH XLXN_7449
            WIRE 3376 1456 3408 1456
            WIRE 3408 1456 3408 1488
        END BRANCH
        BEGIN BRANCH TimerSelect(17)
            WIRE 2944 2096 2976 2096
            BEGIN DISPLAY 2944 2096 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH BusClk
            WIRE 2944 2192 2976 2192
            BEGIN DISPLAY 2944 2192 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH TimerClk
            WIRE 2944 2240 2976 2240
            BEGIN DISPLAY 2944 2240 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH AddrTrigger
            WIRE 2944 2576 2976 2576
            BEGIN DISPLAY 2944 2576 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH WR
            WIRE 2944 2336 2976 2336
            BEGIN DISPLAY 2944 2336 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH GlobalEnable
            WIRE 2944 2288 2976 2288
            BEGIN DISPLAY 2944 2288 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH TimerDataOut(31:0)
            WIRE 2944 2464 2976 2464
            BEGIN DISPLAY 2944 2464 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH OnePPS
            WIRE 2944 2528 2976 2528
            BEGIN DISPLAY 2944 2528 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH AddrOutTimer(2:0)
            WIRE 2944 2400 2976 2400
            BEGIN DISPLAY 2944 2400 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH Timer_Rst
            WIRE 2944 2144 2976 2144
            BEGIN DISPLAY 2944 2144 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH XLXN_7485
            WIRE 3376 2096 3408 2096
            WIRE 3408 2096 3408 2128
        END BRANCH
        BEGIN BRANCH xexep_n16
            WIRE 3408 2352 3408 2368
            WIRE 3408 2368 3408 2416
        END BRANCH
        BEGIN BRANCH TimerSelect(3)
            WIRE 3744 176 3776 176
            BEGIN DISPLAY 3744 176 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH BusClk
            WIRE 3744 272 3776 272
            BEGIN DISPLAY 3744 272 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH TimerClk
            WIRE 3744 320 3776 320
            BEGIN DISPLAY 3744 320 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH AddrTrigger
            WIRE 3744 656 3776 656
            BEGIN DISPLAY 3744 656 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH WR
            WIRE 3744 416 3776 416
            BEGIN DISPLAY 3744 416 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH GlobalEnable
            WIRE 3744 368 3776 368
            BEGIN DISPLAY 3744 368 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH TimerDataOut(31:0)
            WIRE 3744 544 3776 544
            BEGIN DISPLAY 3744 544 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH OnePPS
            WIRE 3744 608 3776 608
            BEGIN DISPLAY 3744 608 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH AddrOutTimer(2:0)
            WIRE 3744 480 3776 480
            BEGIN DISPLAY 3744 480 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH Timer_Rst
            WIRE 3744 224 3776 224
            BEGIN DISPLAY 3744 224 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH xexep_p25
            WIRE 4208 432 4208 448
            WIRE 4208 448 4208 512
        END BRANCH
        BEGIN BRANCH XLXN_7161
            WIRE 4176 176 4208 176
            WIRE 4208 176 4208 208
        END BRANCH
        BEGIN BRANCH XLXN_7185
            WIRE 4176 816 4208 816
            WIRE 4208 816 4208 848
        END BRANCH
        BEGIN BRANCH xexep_n15
            WIRE 4208 1072 4208 1088
            WIRE 4208 1088 4208 1152
        END BRANCH
        BEGIN BRANCH Timer_Rst
            WIRE 3744 864 3776 864
            BEGIN DISPLAY 3744 864 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH AddrOutTimer(2:0)
            WIRE 3744 1120 3776 1120
            BEGIN DISPLAY 3744 1120 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH OnePPS
            WIRE 3744 1248 3776 1248
            BEGIN DISPLAY 3744 1248 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH TimerDataOut(31:0)
            WIRE 3744 1184 3776 1184
            BEGIN DISPLAY 3744 1184 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH GlobalEnable
            WIRE 3744 1008 3776 1008
            BEGIN DISPLAY 3744 1008 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH WR
            WIRE 3744 1056 3776 1056
            BEGIN DISPLAY 3744 1056 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH AddrTrigger
            WIRE 3744 1296 3776 1296
            BEGIN DISPLAY 3744 1296 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH TimerClk
            WIRE 3744 960 3776 960
            BEGIN DISPLAY 3744 960 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH BusClk
            WIRE 3744 912 3776 912
            BEGIN DISPLAY 3744 912 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH TimerSelect(8)
            WIRE 3744 816 3776 816
            BEGIN DISPLAY 3744 816 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH TimerSelect(13)
            WIRE 3760 1472 3792 1472
            BEGIN DISPLAY 3760 1472 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH BusClk
            WIRE 3760 1568 3792 1568
            BEGIN DISPLAY 3760 1568 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH TimerClk
            WIRE 3760 1616 3792 1616
            BEGIN DISPLAY 3760 1616 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH AddrTrigger
            WIRE 3760 1952 3792 1952
            BEGIN DISPLAY 3760 1952 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH WR
            WIRE 3760 1712 3792 1712
            BEGIN DISPLAY 3760 1712 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH GlobalEnable
            WIRE 3760 1664 3792 1664
            BEGIN DISPLAY 3760 1664 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH TimerDataOut(31:0)
            WIRE 3760 1840 3792 1840
            BEGIN DISPLAY 3760 1840 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH OnePPS
            WIRE 3760 1904 3792 1904
            BEGIN DISPLAY 3760 1904 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH AddrOutTimer(2:0)
            WIRE 3760 1776 3792 1776
            BEGIN DISPLAY 3760 1776 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH Timer_Rst
            WIRE 3760 1520 3792 1520
            BEGIN DISPLAY 3760 1520 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH xexep_n25
            WIRE 4224 1728 4224 1744
            WIRE 4224 1744 4224 1808
        END BRANCH
        BEGIN BRANCH XLXN_7497
            WIRE 4192 1472 4224 1472
            WIRE 4224 1472 4224 1504
        END BRANCH
        BEGIN BRANCH XLXN_7522
            WIRE 4192 2112 4224 2112
            WIRE 4224 2112 4224 2144
        END BRANCH
        BEGIN BRANCH xexep_p14
            WIRE 4224 2368 4224 2384
            WIRE 4224 2384 4224 2448
        END BRANCH
        BEGIN BRANCH Timer_Rst
            WIRE 3760 2160 3792 2160
            BEGIN DISPLAY 3760 2160 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH AddrOutTimer(2:0)
            WIRE 3760 2416 3792 2416
            BEGIN DISPLAY 3760 2416 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH OnePPS
            WIRE 3760 2544 3792 2544
            BEGIN DISPLAY 3760 2544 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH TimerDataOut(31:0)
            WIRE 3760 2480 3792 2480
            BEGIN DISPLAY 3760 2480 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH GlobalEnable
            WIRE 3760 2304 3792 2304
            BEGIN DISPLAY 3760 2304 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH WR
            WIRE 3760 2352 3792 2352
            BEGIN DISPLAY 3760 2352 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH AddrTrigger
            WIRE 3760 2592 3792 2592
            BEGIN DISPLAY 3760 2592 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH TimerClk
            WIRE 3760 2256 3792 2256
            BEGIN DISPLAY 3760 2256 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH BusClk
            WIRE 3760 2208 3792 2208
            BEGIN DISPLAY 3760 2208 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH TimerSelect(18)
            WIRE 3760 2112 3792 2112
            BEGIN DISPLAY 3760 2112 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        IOMARKER 2608 480 xexep_p28 R90 28
        IOMARKER 2624 1120 xexep_n21 R90 28
        IOMARKER 2624 1776 xexep_n28 R90 28
        IOMARKER 2624 2416 xexep_p20 R90 28
        IOMARKER 3408 496 xexep_n27 R90 28
        IOMARKER 3408 1136 xexep_p16 R90 28
        IOMARKER 3408 1792 xexep_p26 R90 28
        IOMARKER 3408 2416 xexep_n16 R90 28
        IOMARKER 4208 512 xexep_p25 R90 28
        IOMARKER 4224 1808 xexep_n25 R90 28
        IOMARKER 4224 2448 xexep_p14 R90 28
        INSTANCE XLXI_2071 1792 2112 R90
        BEGIN INSTANCE Timer16 1392 2496 R0
        END INSTANCE
        INSTANCE XLXI_2065 1792 1472 R90
        BEGIN INSTANCE Timer11 1392 1856 R0
        END INSTANCE
        INSTANCE XLXI_2015 1792 816 R90
        BEGIN INSTANCE Timer6 1392 1200 R0
        END INSTANCE
        INSTANCE XLXI_1780 1776 176 R90
        BEGIN INSTANCE Timer_1 1376 560 R0
        END INSTANCE
        BEGIN BRANCH xexep_n22
            WIRE 1824 2336 1824 2416
        END BRANCH
        BEGIN BRANCH xexep_p29
            WIRE 1824 1696 1824 1776
        END BRANCH
        BEGIN BRANCH xexep_p22
            WIRE 1824 1040 1824 1120
        END BRANCH
        BEGIN BRANCH XLXN_7461
            WIRE 1792 2080 1824 2080
            WIRE 1824 2080 1824 2112
        END BRANCH
        BEGIN BRANCH Timer_Rst
            WIRE 1360 2128 1392 2128
            BEGIN DISPLAY 1360 2128 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH GlobalEnable
            WIRE 1360 2272 1392 2272
            BEGIN DISPLAY 1360 2272 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH AddrOutTimer(2:0)
            WIRE 1360 2384 1392 2384
            BEGIN DISPLAY 1360 2384 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH TimerDataOut(31:0)
            WIRE 1360 2448 1392 2448
            BEGIN DISPLAY 1360 2448 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH OnePPS
            WIRE 1360 2512 1392 2512
            BEGIN DISPLAY 1360 2512 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH WR
            WIRE 1360 2320 1392 2320
            BEGIN DISPLAY 1360 2320 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH AddrTrigger
            WIRE 1360 2560 1392 2560
            BEGIN DISPLAY 1360 2560 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH TimerClk
            WIRE 1360 2224 1392 2224
            BEGIN DISPLAY 1360 2224 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH BusClk
            WIRE 1360 2176 1392 2176
            BEGIN DISPLAY 1360 2176 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH TimerSelect(15)
            WIRE 1360 2080 1392 2080
            BEGIN DISPLAY 1360 2080 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH XLXN_7425
            WIRE 1792 1440 1824 1440
            WIRE 1824 1440 1824 1472
        END BRANCH
        BEGIN BRANCH Timer_Rst
            WIRE 1360 1488 1392 1488
            BEGIN DISPLAY 1360 1488 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH GlobalEnable
            WIRE 1360 1632 1392 1632
            BEGIN DISPLAY 1360 1632 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH AddrOutTimer(2:0)
            WIRE 1360 1744 1392 1744
            BEGIN DISPLAY 1360 1744 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH TimerDataOut(31:0)
            WIRE 1360 1808 1392 1808
            BEGIN DISPLAY 1360 1808 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH OnePPS
            WIRE 1360 1872 1392 1872
            BEGIN DISPLAY 1360 1872 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH WR
            WIRE 1360 1680 1392 1680
            BEGIN DISPLAY 1360 1680 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH AddrTrigger
            WIRE 1360 1920 1392 1920
            BEGIN DISPLAY 1360 1920 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH TimerClk
            WIRE 1360 1584 1392 1584
            BEGIN DISPLAY 1360 1584 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH BusClk
            WIRE 1360 1536 1392 1536
            BEGIN DISPLAY 1360 1536 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH TimerSelect(10)
            WIRE 1360 1440 1392 1440
            BEGIN DISPLAY 1360 1440 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH XLXN_7125
            WIRE 1792 784 1824 784
            WIRE 1824 784 1824 816
        END BRANCH
        BEGIN BRANCH Timer_Rst
            WIRE 1360 832 1392 832
            BEGIN DISPLAY 1360 832 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH GlobalEnable
            WIRE 1360 976 1392 976
            BEGIN DISPLAY 1360 976 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH AddrOutTimer(2:0)
            WIRE 1360 1088 1392 1088
            BEGIN DISPLAY 1360 1088 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH TimerDataOut(31:0)
            WIRE 1360 1152 1392 1152
            BEGIN DISPLAY 1360 1152 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH OnePPS
            WIRE 1360 1216 1392 1216
            BEGIN DISPLAY 1360 1216 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH WR
            WIRE 1360 1024 1392 1024
            BEGIN DISPLAY 1360 1024 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH AddrTrigger
            WIRE 1360 1264 1392 1264
            BEGIN DISPLAY 1360 1264 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH TimerClk
            WIRE 1360 928 1392 928
            BEGIN DISPLAY 1360 928 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH BusClk
            WIRE 1360 880 1392 880
            BEGIN DISPLAY 1360 880 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH TimerSelect(5)
            WIRE 1360 784 1392 784
            BEGIN DISPLAY 1360 784 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH XLXN_6666
            WIRE 1776 144 1808 144
            WIRE 1808 144 1808 176
        END BRANCH
        BEGIN BRANCH xexep_n30
            WIRE 1808 400 1808 480
        END BRANCH
        BEGIN BRANCH Timer_Rst
            WIRE 1344 192 1376 192
            BEGIN DISPLAY 1344 192 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH GlobalEnable
            WIRE 1344 336 1376 336
            BEGIN DISPLAY 1344 336 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH AddrOutTimer(2:0)
            WIRE 1344 448 1376 448
            BEGIN DISPLAY 1344 448 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH TimerDataOut(31:0)
            WIRE 1344 512 1376 512
            BEGIN DISPLAY 1344 512 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH OnePPS
            WIRE 1344 576 1376 576
            BEGIN DISPLAY 1344 576 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH WR
            WIRE 1344 384 1376 384
            BEGIN DISPLAY 1344 384 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH AddrTrigger
            WIRE 1344 624 1376 624
            BEGIN DISPLAY 1344 624 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH TimerClk
            WIRE 1344 288 1376 288
            BEGIN DISPLAY 1344 288 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH BusClk
            WIRE 1344 240 1376 240
            BEGIN DISPLAY 1344 240 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH TimerSelect(0)
            WIRE 1344 144 1376 144
            BEGIN DISPLAY 1344 144 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        IOMARKER 1824 1120 xexep_p22 R90 28
        IOMARKER 1824 1776 xexep_p29 R90 28
        IOMARKER 1824 2416 xexep_n22 R90 28
        IOMARKER 1808 480 xexep_n30 R90 28
        BEGIN INSTANCE Timer5 4592 592 R0
        END INSTANCE
        INSTANCE XLXI_2023 4992 208 R90
        INSTANCE XLXI_2027 4992 848 R90
        BEGIN INSTANCE Timer10 4592 1232 R0
        END INSTANCE
        BEGIN INSTANCE Timer15 4608 1888 R0
        END INSTANCE
        INSTANCE XLXI_2079 5008 1504 R90
        INSTANCE XLXI_2080 5008 2144 R90
        BEGIN INSTANCE Timer20 4608 2528 R0
        END INSTANCE
        BEGIN BRANCH TimerSelect(4)
            WIRE 4560 176 4592 176
            BEGIN DISPLAY 4560 176 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH BusClk
            WIRE 4560 272 4592 272
            BEGIN DISPLAY 4560 272 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH TimerClk
            WIRE 4560 320 4592 320
            BEGIN DISPLAY 4560 320 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH AddrTrigger
            WIRE 4560 656 4592 656
            BEGIN DISPLAY 4560 656 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH WR
            WIRE 4560 416 4592 416
            BEGIN DISPLAY 4560 416 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH GlobalEnable
            WIRE 4560 368 4592 368
            BEGIN DISPLAY 4560 368 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH TimerDataOut(31:0)
            WIRE 4560 544 4592 544
            BEGIN DISPLAY 4560 544 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH OnePPS
            WIRE 4560 608 4592 608
            BEGIN DISPLAY 4560 608 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH AddrOutTimer(2:0)
            WIRE 4560 480 4592 480
            BEGIN DISPLAY 4560 480 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH Timer_Rst
            WIRE 4560 224 4592 224
            BEGIN DISPLAY 4560 224 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH xexep_n24
            WIRE 5024 432 5024 448
            WIRE 5024 448 5024 512
        END BRANCH
        BEGIN BRANCH XLXN_7173
            WIRE 4992 176 5024 176
            WIRE 5024 176 5024 208
        END BRANCH
        BEGIN BRANCH XLXN_7197
            WIRE 4992 816 5024 816
            WIRE 5024 816 5024 848
        END BRANCH
        BEGIN BRANCH xexep_p13
            WIRE 5024 1072 5024 1088
            WIRE 5024 1088 5024 1152
        END BRANCH
        BEGIN BRANCH Timer_Rst
            WIRE 4560 864 4592 864
            BEGIN DISPLAY 4560 864 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH AddrOutTimer(2:0)
            WIRE 4560 1120 4592 1120
            BEGIN DISPLAY 4560 1120 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH OnePPS
            WIRE 4560 1248 4592 1248
            BEGIN DISPLAY 4560 1248 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH TimerDataOut(31:0)
            WIRE 4560 1184 4592 1184
            BEGIN DISPLAY 4560 1184 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH GlobalEnable
            WIRE 4560 1008 4592 1008
            BEGIN DISPLAY 4560 1008 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH WR
            WIRE 4560 1056 4592 1056
            BEGIN DISPLAY 4560 1056 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH AddrTrigger
            WIRE 4560 1296 4592 1296
            BEGIN DISPLAY 4560 1296 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH TimerClk
            WIRE 4560 960 4592 960
            BEGIN DISPLAY 4560 960 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH BusClk
            WIRE 4560 912 4592 912
            BEGIN DISPLAY 4560 912 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH TimerSelect(9)
            WIRE 4560 816 4592 816
            BEGIN DISPLAY 4560 816 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH TimerSelect(14)
            WIRE 4576 1472 4608 1472
            BEGIN DISPLAY 4576 1472 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH BusClk
            WIRE 4576 1568 4608 1568
            BEGIN DISPLAY 4576 1568 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH TimerClk
            WIRE 4576 1616 4608 1616
            BEGIN DISPLAY 4576 1616 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH AddrTrigger
            WIRE 4576 1952 4608 1952
            BEGIN DISPLAY 4576 1952 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH WR
            WIRE 4576 1712 4608 1712
            BEGIN DISPLAY 4576 1712 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH GlobalEnable
            WIRE 4576 1664 4608 1664
            BEGIN DISPLAY 4576 1664 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH TimerDataOut(31:0)
            WIRE 4576 1840 4608 1840
            BEGIN DISPLAY 4576 1840 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH OnePPS
            WIRE 4576 1904 4608 1904
            BEGIN DISPLAY 4576 1904 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH AddrOutTimer(2:0)
            WIRE 4576 1776 4608 1776
            BEGIN DISPLAY 4576 1776 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH Timer_Rst
            WIRE 4576 1520 4608 1520
            BEGIN DISPLAY 4576 1520 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH xexep_p23
            WIRE 5040 1728 5040 1744
            WIRE 5040 1744 5040 1808
        END BRANCH
        BEGIN BRANCH XLXN_7509
            WIRE 5008 1472 5040 1472
            WIRE 5040 1472 5040 1504
        END BRANCH
        BEGIN BRANCH XLXN_7510
            WIRE 5008 2112 5040 2112
            WIRE 5040 2112 5040 2144
        END BRANCH
        BEGIN BRANCH xexep_n13
            WIRE 5040 2368 5040 2384
            WIRE 5040 2384 5040 2448
        END BRANCH
        BEGIN BRANCH Timer_Rst
            WIRE 4576 2160 4608 2160
            BEGIN DISPLAY 4576 2160 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH AddrOutTimer(2:0)
            WIRE 4576 2416 4608 2416
            BEGIN DISPLAY 4576 2416 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH OnePPS
            WIRE 4576 2544 4608 2544
            BEGIN DISPLAY 4576 2544 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH TimerDataOut(31:0)
            WIRE 4576 2480 4608 2480
            BEGIN DISPLAY 4576 2480 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH GlobalEnable
            WIRE 4576 2304 4608 2304
            BEGIN DISPLAY 4576 2304 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH WR
            WIRE 4576 2352 4608 2352
            BEGIN DISPLAY 4576 2352 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH AddrTrigger
            WIRE 4576 2592 4608 2592
            BEGIN DISPLAY 4576 2592 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH TimerClk
            WIRE 4576 2256 4608 2256
            BEGIN DISPLAY 4576 2256 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH BusClk
            WIRE 4576 2208 4608 2208
            BEGIN DISPLAY 4576 2208 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH TimerSelect(19)
            WIRE 4576 2112 4608 2112
            BEGIN DISPLAY 4576 2112 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        IOMARKER 5024 512 xexep_n24 R90 28
        IOMARKER 5024 1152 xexep_p13 R90 28
        IOMARKER 5040 1808 xexep_p23 R90 28
        IOMARKER 5040 2448 xexep_n13 R90 28
        IOMARKER 4208 1152 xexep_n15 R90 28
    END SHEET
END SCHEMATIC
