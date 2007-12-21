VERSION 6
BEGIN SCHEMATIC
    BEGIN ATTR DeviceFamilyName "virtex2"
        DELETE all:0
        EDITNAME all:0
        EDITTRAIT all:0
    END ATTR
    BEGIN NETLIST
        SIGNAL CLK
        SIGNAL DATA(15:0)
        SIGNAL WR
        SIGNAL ENABLE
        SIGNAL ADDR(2:0)
        SIGNAL ADDR(0)
        SIGNAL ADDR(1)
        SIGNAL ADDR(2)
        SIGNAL REG_1
        SIGNAL REG_2
        SIGNAL REG_3
        SIGNAL REG_4
        SIGNAL REG_5
        SIGNAL REG_6
        SIGNAL REG_7
        SIGNAL REG_8
        SIGNAL CNTL(15:0)
        SIGNAL DELAY(15:0)
        SIGNAL WIDTH(15:0)
        SIGNAL PERIOD(15:0)
        SIGNAL OFF_PER(15:0)
        SIGNAL OFF_WID(15:0)
        SIGNAL OFF_DEL(15:0)
        SIGNAL MULT(15:0)
        PORT Input CLK
        PORT BiDirectional DATA(15:0)
        PORT Input WR
        PORT Input ENABLE
        PORT Input ADDR(2:0)
        PORT Output CNTL(15:0)
        PORT Output DELAY(15:0)
        PORT Output WIDTH(15:0)
        PORT Output PERIOD(15:0)
        PORT Output OFF_PER(15:0)
        PORT Output OFF_WID(15:0)
        PORT Output OFF_DEL(15:0)
        PORT Output MULT(15:0)
        BEGIN BLOCKDEF register_16bit
            TIMESTAMP 2005 11 29 22 10 54
            LINE N 64 32 0 32 
            LINE N 64 -160 0 -160 
            LINE N 64 -96 0 -96 
            RECTANGLE N 0 -44 64 -20 
            LINE N 64 -32 0 -32 
            RECTANGLE N 320 -172 384 -148 
            LINE N 320 -160 384 -160 
            RECTANGLE N 64 -192 320 64 
        END BLOCKDEF
        BEGIN BLOCKDEF d3_8e
            TIMESTAMP 2001 2 2 12 36 51
            LINE N 0 -576 64 -576 
            LINE N 0 -512 64 -512 
            LINE N 0 -448 64 -448 
            LINE N 384 -576 320 -576 
            LINE N 384 -512 320 -512 
            LINE N 384 -448 320 -448 
            LINE N 384 -384 320 -384 
            LINE N 384 -320 320 -320 
            LINE N 384 -256 320 -256 
            LINE N 384 -192 320 -192 
            LINE N 384 -128 320 -128 
            RECTANGLE N 64 -640 320 -64 
            LINE N 0 -128 64 -128 
        END BLOCKDEF
        BEGIN BLOCK XLXI_9 d3_8e
            PIN A0 ADDR(0)
            PIN A1 ADDR(1)
            PIN A2 ADDR(2)
            PIN E ENABLE
            PIN D0 REG_1
            PIN D1 REG_2
            PIN D2 REG_3
            PIN D3 REG_4
            PIN D4 REG_5
            PIN D5 REG_6
            PIN D6 REG_7
            PIN D7 REG_8
        END BLOCK
        BEGIN BLOCK XLXI_1 register_16bit
            PIN CLK CLK
            PIN ENABLE REG_2
            PIN DATA(15:0) DATA(15:0)
            PIN DATA_OUT(15:0) DELAY(15:0)
            PIN WR WR
        END BLOCK
        BEGIN BLOCK XLXI_2 register_16bit
            PIN CLK CLK
            PIN ENABLE REG_3
            PIN DATA(15:0) DATA(15:0)
            PIN DATA_OUT(15:0) WIDTH(15:0)
            PIN WR WR
        END BLOCK
        BEGIN BLOCK XLXI_3 register_16bit
            PIN CLK CLK
            PIN ENABLE REG_4
            PIN DATA(15:0) DATA(15:0)
            PIN DATA_OUT(15:0) PERIOD(15:0)
            PIN WR WR
        END BLOCK
        BEGIN BLOCK XLXI_36 register_16bit
            PIN CLK CLK
            PIN ENABLE REG_1
            PIN DATA(15:0) DATA(15:0)
            PIN DATA_OUT(15:0) CNTL(15:0)
            PIN WR WR
        END BLOCK
        BEGIN BLOCK XLXI_43 register_16bit
            PIN CLK CLK
            PIN ENABLE REG_7
            PIN DATA(15:0) DATA(15:0)
            PIN DATA_OUT(15:0) OFF_PER(15:0)
            PIN WR WR
        END BLOCK
        BEGIN BLOCK XLXI_42 register_16bit
            PIN CLK CLK
            PIN ENABLE REG_6
            PIN DATA(15:0) DATA(15:0)
            PIN DATA_OUT(15:0) OFF_WID(15:0)
            PIN WR WR
        END BLOCK
        BEGIN BLOCK XLXI_41 register_16bit
            PIN CLK CLK
            PIN ENABLE REG_5
            PIN DATA(15:0) DATA(15:0)
            PIN DATA_OUT(15:0) OFF_DEL(15:0)
            PIN WR WR
        END BLOCK
        BEGIN BLOCK XLXI_44 register_16bit
            PIN CLK CLK
            PIN ENABLE REG_8
            PIN DATA(15:0) DATA(15:0)
            PIN DATA_OUT(15:0) MULT(15:0)
            PIN WR WR
        END BLOCK
    END NETLIST
    BEGIN SHEET 1 3520 2720
        IOMARKER 464 160 CLK R180 28
        BEGIN BRANCH CLK
            WIRE 464 160 592 160
            BEGIN DISPLAY 592 160 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH DATA(15:0)
            WIRE 464 240 592 240
            BEGIN DISPLAY 592 240 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        IOMARKER 464 240 DATA(15:0) R180 28
        IOMARKER 464 320 WR R180 28
        BEGIN BRANCH WR
            WIRE 464 320 592 320
            BEGIN DISPLAY 592 320 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        INSTANCE XLXI_9 736 1008 R0
        BEGIN BRANCH ENABLE
            WIRE 464 880 736 880
        END BRANCH
        BEGIN BRANCH ADDR(2:0)
            WIRE 480 400 528 400
            WIRE 528 400 528 432
            WIRE 528 432 528 496
            WIRE 528 496 528 560
            WIRE 528 560 528 576
        END BRANCH
        BUSTAP 528 432 624 432
        BUSTAP 528 496 624 496
        BUSTAP 528 560 624 560
        BEGIN BRANCH ADDR(0)
            WIRE 624 432 672 432
            WIRE 672 432 736 432
            BEGIN DISPLAY 672 432 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BEGIN BRANCH ADDR(1)
            WIRE 624 496 672 496
            WIRE 672 496 736 496
            BEGIN DISPLAY 672 496 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BEGIN BRANCH ADDR(2)
            WIRE 624 560 672 560
            WIRE 672 560 736 560
            BEGIN DISPLAY 672 560 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BEGIN BRANCH REG_1
            WIRE 1120 432 1200 432
            BEGIN DISPLAY 1200 432 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH REG_2
            WIRE 1120 496 1200 496
            BEGIN DISPLAY 1200 496 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH REG_3
            WIRE 1120 560 1200 560
            BEGIN DISPLAY 1200 560 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH REG_4
            WIRE 1120 624 1200 624
            BEGIN DISPLAY 1200 624 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH REG_5
            WIRE 1120 688 1200 688
            BEGIN DISPLAY 1200 688 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH REG_6
            WIRE 1120 752 1200 752
            BEGIN DISPLAY 1200 752 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH REG_7
            WIRE 1120 816 1200 816
            BEGIN DISPLAY 1200 816 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH REG_8
            WIRE 1120 880 1200 880
            BEGIN DISPLAY 1200 880 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        IOMARKER 480 400 ADDR(2:0) R180 28
        IOMARKER 464 880 ENABLE R180 28
        BEGIN BRANCH DELAY(15:0)
            WIRE 2048 1520 2064 1520
            WIRE 2064 1520 2128 1520
        END BRANCH
        BEGIN INSTANCE XLXI_1 1664 1680 R0
        END INSTANCE
        BEGIN BRANCH WIDTH(15:0)
            WIRE 2048 1872 2064 1872
            WIRE 2064 1872 2128 1872
        END BRANCH
        BEGIN INSTANCE XLXI_2 1664 2032 R0
        END INSTANCE
        BEGIN BRANCH PERIOD(15:0)
            WIRE 2048 2224 2064 2224
            WIRE 2064 2224 2128 2224
        END BRANCH
        BEGIN INSTANCE XLXI_3 1664 2384 R0
        END INSTANCE
        BEGIN INSTANCE XLXI_36 1664 1344 R0
        END INSTANCE
        BEGIN BRANCH CNTL(15:0)
            WIRE 2048 1184 2128 1184
        END BRANCH
        BEGIN INSTANCE XLXI_43 2512 2384 R0
        END INSTANCE
        BEGIN BRANCH OFF_PER(15:0)
            WIRE 2896 2224 3056 2224
        END BRANCH
        BEGIN INSTANCE XLXI_42 2512 2032 R0
        END INSTANCE
        BEGIN BRANCH OFF_WID(15:0)
            WIRE 2896 1872 3056 1872
        END BRANCH
        BEGIN INSTANCE XLXI_41 2512 1680 R0
        END INSTANCE
        BEGIN BRANCH OFF_DEL(15:0)
            WIRE 2896 1520 3056 1520
        END BRANCH
        BEGIN INSTANCE XLXI_44 2512 1344 R0
        END INSTANCE
        BEGIN BRANCH MULT(15:0)
            WIRE 2896 1184 2928 1184
            WIRE 2928 1184 3056 1184
        END BRANCH
        BEGIN BRANCH DATA(15:0)
            WIRE 1568 1312 1584 1312
            WIRE 1584 1312 1664 1312
            BEGIN DISPLAY 1568 1312 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH DATA(15:0)
            WIRE 1568 1648 1584 1648
            WIRE 1584 1648 1664 1648
            BEGIN DISPLAY 1568 1648 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH CLK
            WIRE 1568 1520 1664 1520
            BEGIN DISPLAY 1568 1520 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH CLK
            WIRE 1568 1184 1664 1184
            BEGIN DISPLAY 1568 1184 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH REG_1
            WIRE 1568 1248 1584 1248
            WIRE 1584 1248 1664 1248
            BEGIN DISPLAY 1568 1248 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH WR
            WIRE 1568 1376 1664 1376
            BEGIN DISPLAY 1568 1376 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH REG_2
            WIRE 1568 1584 1584 1584
            WIRE 1584 1584 1664 1584
            BEGIN DISPLAY 1568 1584 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH WR
            WIRE 1568 1712 1664 1712
            BEGIN DISPLAY 1568 1712 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH CLK
            WIRE 1568 1872 1664 1872
            BEGIN DISPLAY 1568 1872 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH REG_3
            WIRE 1568 1936 1584 1936
            WIRE 1584 1936 1664 1936
            BEGIN DISPLAY 1568 1936 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH DATA(15:0)
            WIRE 1568 2000 1584 2000
            WIRE 1584 2000 1664 2000
            BEGIN DISPLAY 1568 2000 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH WR
            WIRE 1568 2064 1664 2064
            BEGIN DISPLAY 1568 2064 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH CLK
            WIRE 1568 2224 1664 2224
            BEGIN DISPLAY 1568 2224 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH REG_4
            WIRE 1568 2288 1584 2288
            WIRE 1584 2288 1664 2288
            BEGIN DISPLAY 1568 2288 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH DATA(15:0)
            WIRE 1568 2352 1584 2352
            WIRE 1584 2352 1664 2352
            BEGIN DISPLAY 1568 2352 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH WR
            WIRE 1568 2416 1664 2416
            BEGIN DISPLAY 1568 2416 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH CLK
            WIRE 2432 2224 2512 2224
            BEGIN DISPLAY 2432 2224 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH REG_7
            WIRE 2432 2288 2512 2288
            BEGIN DISPLAY 2432 2288 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH DATA(15:0)
            WIRE 2432 2352 2512 2352
            BEGIN DISPLAY 2432 2352 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH WR
            WIRE 2432 2416 2512 2416
            BEGIN DISPLAY 2432 2416 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH CLK
            WIRE 2432 1872 2512 1872
            BEGIN DISPLAY 2432 1872 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH REG_6
            WIRE 2432 1936 2512 1936
            BEGIN DISPLAY 2432 1936 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH DATA(15:0)
            WIRE 2432 2000 2512 2000
            BEGIN DISPLAY 2432 2000 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH WR
            WIRE 2432 2064 2512 2064
            BEGIN DISPLAY 2432 2064 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH WR
            WIRE 2432 1712 2512 1712
            BEGIN DISPLAY 2432 1712 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH DATA(15:0)
            WIRE 2432 1648 2512 1648
            BEGIN DISPLAY 2432 1648 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH REG_5
            WIRE 2432 1584 2512 1584
            BEGIN DISPLAY 2432 1584 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH CLK
            WIRE 2432 1520 2512 1520
            BEGIN DISPLAY 2432 1520 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH WR
            WIRE 2432 1376 2512 1376
            BEGIN DISPLAY 2432 1376 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH DATA(15:0)
            WIRE 2432 1312 2512 1312
            BEGIN DISPLAY 2432 1312 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH REG_8
            WIRE 2432 1248 2512 1248
            BEGIN DISPLAY 2432 1248 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH CLK
            WIRE 2432 1184 2512 1184
            BEGIN DISPLAY 2432 1184 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        IOMARKER 2128 1520 DELAY(15:0) R0 28
        IOMARKER 2128 1872 WIDTH(15:0) R0 28
        IOMARKER 2128 2224 PERIOD(15:0) R0 28
        IOMARKER 3056 2224 OFF_PER(15:0) R0 28
        IOMARKER 3056 1872 OFF_WID(15:0) R0 28
        IOMARKER 3056 1520 OFF_DEL(15:0) R0 28
        IOMARKER 3056 1184 MULT(15:0) R0 28
        IOMARKER 2128 1184 CNTL(15:0) R0 28
        BEGIN DISPLAY 1720 768 TEXT "CNTL(4) = CLK Divide MSB"
            FONT 30 "Arial"
        END DISPLAY
        BEGIN DISPLAY 1720 832 TEXT "CNTL(3) = CLK Divide LSB"
            FONT 30 "Arial"
        END DISPLAY
        BEGIN DISPLAY 1724 896 TEXT "CNTL(2) = Global CLK (0) / Ext. CLK (1)"
            FONT 30 "Arial"
        END DISPLAY
        BEGIN DISPLAY 1724 960 TEXT "CNTL(1) = Invert Output (1) / Noninvert Output (0)"
            FONT 30 "Arial"
        END DISPLAY
        BEGIN DISPLAY 1724 1024 TEXT "CNTL(0) = Master On (1) / Off (0)"
            FONT 30 "Arial"
        END DISPLAY
        BEGIN DISPLAY 2068 812 TEXT "10 = Divide By 4"
            FONT 25 "Arial"
        END DISPLAY
        BEGIN DISPLAY 2068 836 TEXT "11 = Divide By 8"
            FONT 25 "Arial"
        END DISPLAY
        BEGIN DISPLAY 2068 768 TEXT "00 = No Divide"
            FONT 25 "Arial"
        END DISPLAY
        BEGIN DISPLAY 2068 788 TEXT "01 = Divide By 2"
            FONT 25 "Arial"
        END DISPLAY
        BEGIN DISPLAY 1720 712 TEXT "CNTL(15:5) = Not Used"
            FONT 30 "Arial"
        END DISPLAY
    END SHEET
END SCHEMATIC
