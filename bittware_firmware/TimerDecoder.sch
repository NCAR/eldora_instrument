VERSION 6
BEGIN SCHEMATIC
    BEGIN ATTR DeviceFamilyName "virtex2"
        DELETE all:0
        EDITNAME all:0
        EDITTRAIT all:0
    END ATTR
    BEGIN NETLIST
        SIGNAL En0
        SIGNAL En1
        SIGNAL En2
        SIGNAL En3
        SIGNAL En4
        SIGNAL En5
        SIGNAL En6
        SIGNAL En7
        SIGNAL AddrIn(0)
        SIGNAL AddrIn(1)
        SIGNAL AddrIn(2)
        SIGNAL Enable
        SIGNAL AddrIn(2:0)
        SIGNAL Clk
        SIGNAL WR
        SIGNAL Cntrl(15:0)
        SIGNAL Delay(15:0)
        SIGNAL Width(15:0)
        SIGNAL Period(15:0)
        SIGNAL Mult(15:0)
        SIGNAL DataInOut(31:0)
        SIGNAL DataInOut(15)
        SIGNAL DataInOut(14)
        SIGNAL DataInOut(13)
        SIGNAL DataInOut(12)
        SIGNAL DataInOut(11)
        SIGNAL DataInOut(10)
        SIGNAL DataInOut(9)
        SIGNAL DataInOut(8)
        SIGNAL DataInOut(7)
        SIGNAL DataInOut(6)
        SIGNAL DataInOut(5)
        SIGNAL DataInOut(4)
        SIGNAL DataInOut(3)
        SIGNAL DataInOut(2)
        SIGNAL DataInOut(1)
        SIGNAL DataInOut(0)
        SIGNAL Reset
        SIGNAL DataInOut(15:0)
        PORT Input Enable
        PORT Input AddrIn(2:0)
        PORT Input Clk
        PORT Input WR
        PORT Output Cntrl(15:0)
        PORT Output Delay(15:0)
        PORT Output Width(15:0)
        PORT Output Period(15:0)
        PORT Output Mult(15:0)
        PORT BiDirectional DataInOut(31:0)
        PORT Input Reset
        BEGIN BLOCKDEF register_16bit
            TIMESTAMP 2006 3 9 0 12 48
            LINE N 64 -160 0 -160 
            RECTANGLE N 320 -172 384 -148 
            LINE N 320 -160 384 -160 
            RECTANGLE N 0 20 64 44 
            LINE N 0 32 64 32 
            RECTANGLE N 64 -192 320 64 
            LINE N 64 -112 0 -112 
            LINE N 64 -64 0 -64 
            LINE N 64 -16 0 -16 
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
        BEGIN BLOCK XLXI_19 register_16bit
            PIN Clk Clk
            PIN DataIn(15:0) DataInOut(15:0)
            PIN DataOut(15:0) Mult(15:0)
            PIN Reset Reset
            PIN Enable En4
            PIN WR WR
        END BLOCK
        BEGIN BLOCK XLXI_20 register_16bit
            PIN Clk Clk
            PIN DataIn(15:0) DataInOut(15:0)
            PIN DataOut(15:0)
            PIN Reset Reset
            PIN Enable En5
            PIN WR WR
        END BLOCK
        BEGIN BLOCK XLXI_21 register_16bit
            PIN Clk Clk
            PIN DataIn(15:0) DataInOut(15:0)
            PIN DataOut(15:0)
            PIN Reset Reset
            PIN Enable En6
            PIN WR WR
        END BLOCK
        BEGIN BLOCK XLXI_22 register_16bit
            PIN Clk Clk
            PIN DataIn(15:0) DataInOut(15:0)
            PIN DataOut(15:0)
            PIN Reset Reset
            PIN Enable En7
            PIN WR WR
        END BLOCK
        BEGIN BLOCK XLXI_23 d3_8e
            PIN A0 AddrIn(0)
            PIN A1 AddrIn(1)
            PIN A2 AddrIn(2)
            PIN E Enable
            PIN D0 En0
            PIN D1 En1
            PIN D2 En2
            PIN D3 En3
            PIN D4 En4
            PIN D5 En5
            PIN D6 En6
            PIN D7 En7
        END BLOCK
        BEGIN BLOCK XLXI_1 register_16bit
            PIN Clk Clk
            PIN DataIn(15:0) DataInOut(15:0)
            PIN DataOut(15:0) Cntrl(15:0)
            PIN Reset Reset
            PIN Enable En0
            PIN WR WR
        END BLOCK
        BEGIN BLOCK XLXI_2 register_16bit
            PIN Clk Clk
            PIN DataIn(15:0) DataInOut(15:0)
            PIN DataOut(15:0) Delay(15:0)
            PIN Reset Reset
            PIN Enable En1
            PIN WR WR
        END BLOCK
        BEGIN BLOCK XLXI_9 register_16bit
            PIN Clk Clk
            PIN DataIn(15:0) DataInOut(15:0)
            PIN DataOut(15:0) Width(15:0)
            PIN Reset Reset
            PIN Enable En2
            PIN WR WR
        END BLOCK
        BEGIN BLOCK XLXI_10 register_16bit
            PIN Clk Clk
            PIN DataIn(15:0) DataInOut(15:0)
            PIN DataOut(15:0) Period(15:0)
            PIN Reset Reset
            PIN Enable En3
            PIN WR WR
        END BLOCK
    END NETLIST
    BEGIN SHEET 1 3520 2720
        BEGIN INSTANCE XLXI_19 2336 1152 R0
        END INSTANCE
        BEGIN INSTANCE XLXI_20 2336 1504 R0
        END INSTANCE
        BEGIN INSTANCE XLXI_21 2336 1856 R0
        END INSTANCE
        BEGIN INSTANCE XLXI_22 2336 2208 R0
        END INSTANCE
        INSTANCE XLXI_23 576 1120 R0
        BEGIN BRANCH En0
            WIRE 960 544 992 544
            BEGIN DISPLAY 992 544 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH En1
            WIRE 960 608 992 608
            BEGIN DISPLAY 992 608 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH En2
            WIRE 960 672 992 672
            BEGIN DISPLAY 992 672 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH En3
            WIRE 960 736 992 736
            BEGIN DISPLAY 992 736 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH En4
            WIRE 960 800 992 800
            BEGIN DISPLAY 992 800 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH En5
            WIRE 960 864 992 864
            BEGIN DISPLAY 992 864 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH En6
            WIRE 960 928 992 928
            BEGIN DISPLAY 992 928 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH En7
            WIRE 960 992 992 992
            BEGIN DISPLAY 992 992 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH AddrIn(0)
            WIRE 432 544 480 544
            WIRE 480 544 576 544
            BEGIN DISPLAY 480 544 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BEGIN BRANCH AddrIn(1)
            WIRE 432 608 480 608
            WIRE 480 608 576 608
            BEGIN DISPLAY 480 608 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BEGIN BRANCH AddrIn(2)
            WIRE 432 672 480 672
            WIRE 480 672 576 672
            BEGIN DISPLAY 480 672 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BEGIN BRANCH Enable
            WIRE 256 992 576 992
        END BRANCH
        BEGIN BRANCH AddrIn(2:0)
            WIRE 288 528 336 528
            WIRE 336 528 336 544
            WIRE 336 544 336 608
            WIRE 336 608 336 672
            WIRE 336 672 336 688
        END BRANCH
        IOMARKER 288 528 AddrIn(2:0) R180 28
        BUSTAP 336 544 432 544
        BUSTAP 336 608 432 608
        BUSTAP 336 672 432 672
        BEGIN BRANCH Clk
            WIRE 288 304 352 304
            BEGIN DISPLAY 352 304 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH WR
            WIRE 288 368 352 368
            BEGIN DISPLAY 352 368 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        IOMARKER 288 304 Clk R180 28
        IOMARKER 288 368 WR R180 28
        BEGIN BRANCH Clk
            WIRE 2288 992 2336 992
            BEGIN DISPLAY 2288 992 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH Clk
            WIRE 2288 1344 2336 1344
            BEGIN DISPLAY 2288 1344 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH Clk
            WIRE 2288 1696 2336 1696
            BEGIN DISPLAY 2288 1696 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH Clk
            WIRE 2288 2048 2336 2048
            BEGIN DISPLAY 2288 2048 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN INSTANCE XLXI_1 1536 1152 R0
        END INSTANCE
        BEGIN INSTANCE XLXI_2 1536 1504 R0
        END INSTANCE
        BEGIN INSTANCE XLXI_9 1536 1856 R0
        END INSTANCE
        BEGIN INSTANCE XLXI_10 1536 2208 R0
        END INSTANCE
        BEGIN BRANCH Clk
            WIRE 1488 992 1536 992
            BEGIN DISPLAY 1488 992 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH Clk
            WIRE 1488 1344 1536 1344
            BEGIN DISPLAY 1488 1344 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH Clk
            WIRE 1488 1696 1536 1696
            BEGIN DISPLAY 1488 1696 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH Clk
            WIRE 1488 2048 1536 2048
            BEGIN DISPLAY 1488 2048 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        IOMARKER 256 992 Enable R180 28
        BEGIN BRANCH Cntrl(15:0)
            WIRE 1920 992 1952 992
        END BRANCH
        BEGIN BRANCH Delay(15:0)
            WIRE 1920 1344 1952 1344
        END BRANCH
        BEGIN BRANCH Width(15:0)
            WIRE 1920 1696 1952 1696
        END BRANCH
        BEGIN BRANCH Period(15:0)
            WIRE 1920 2048 1952 2048
        END BRANCH
        BEGIN BRANCH Mult(15:0)
            WIRE 2720 992 2752 992
        END BRANCH
        IOMARKER 1952 992 Cntrl(15:0) R0 28
        IOMARKER 1952 1344 Delay(15:0) R0 28
        IOMARKER 1952 1696 Width(15:0) R0 28
        IOMARKER 1952 2048 Period(15:0) R0 28
        IOMARKER 2752 992 Mult(15:0) R0 28
        BEGIN DISPLAY 1600 800 TEXT "Cntrl(1) = Invert Output (1) / Noninvert Output (0)"
            FONT 28 "Arial"
        END DISPLAY
        BEGIN DISPLAY 1600 848 TEXT "Cntrl(0) = Master On (1) / Off (0)"
            FONT 28 "Arial"
        END DISPLAY
        BEGIN DISPLAY 1600 704 TEXT "Cntrl(3) = Clk Divide LSB"
            FONT 28 "Arial"
        END DISPLAY
        BEGIN DISPLAY 1600 752 TEXT "Cntrl(2) = Global Clk (0) / Ext. Clk (1)"
            FONT 28 "Arial"
        END DISPLAY
        BEGIN DISPLAY 1600 656 TEXT "Cntrl(4) = Clk Divide MSB"
            FONT 28 "Arial"
        END DISPLAY
        BEGIN DISPLAY 1600 608 TEXT "Cntrl(15:5) = Not Used"
            FONT 28 "Arial"
        END DISPLAY
        BEGIN BRANCH DataInOut(31:0)
            WIRE 448 1200 512 1200
            WIRE 512 1200 512 1216
            WIRE 512 1216 512 1248
            WIRE 512 1248 512 1280
            WIRE 512 1280 512 1312
            WIRE 512 1312 512 1344
            WIRE 512 1344 512 1376
            WIRE 512 1376 512 1408
            WIRE 512 1408 512 1440
            WIRE 512 1440 512 1472
            WIRE 512 1472 512 1504
            WIRE 512 1504 512 1536
            WIRE 512 1536 512 1568
            WIRE 512 1568 512 1600
            WIRE 512 1600 512 1632
            WIRE 512 1632 512 1664
            WIRE 512 1664 512 1696
            WIRE 512 1696 512 1712
        END BRANCH
        BUSTAP 512 1216 608 1216
        BUSTAP 512 1248 608 1248
        BUSTAP 512 1280 608 1280
        BUSTAP 512 1312 608 1312
        BUSTAP 512 1344 608 1344
        BUSTAP 512 1376 608 1376
        BUSTAP 512 1408 608 1408
        BUSTAP 512 1440 608 1440
        BUSTAP 512 1472 608 1472
        BUSTAP 512 1504 608 1504
        BUSTAP 512 1536 608 1536
        BUSTAP 512 1568 608 1568
        BUSTAP 512 1600 608 1600
        BUSTAP 512 1632 608 1632
        BUSTAP 512 1664 608 1664
        BUSTAP 512 1696 608 1696
        IOMARKER 448 1200 DataInOut(31:0) R180 28
        BEGIN BRANCH DataInOut(15)
            WIRE 608 1216 640 1216
            WIRE 640 1216 704 1216
            BEGIN DISPLAY 640 1216 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BEGIN BRANCH DataInOut(14)
            WIRE 608 1248 640 1248
            WIRE 640 1248 704 1248
            BEGIN DISPLAY 640 1248 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BEGIN BRANCH DataInOut(13)
            WIRE 608 1280 640 1280
            WIRE 640 1280 704 1280
            BEGIN DISPLAY 640 1280 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BEGIN BRANCH DataInOut(12)
            WIRE 608 1312 640 1312
            WIRE 640 1312 704 1312
            BEGIN DISPLAY 640 1312 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BEGIN BRANCH DataInOut(11)
            WIRE 608 1344 640 1344
            WIRE 640 1344 704 1344
            BEGIN DISPLAY 640 1344 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BEGIN BRANCH DataInOut(10)
            WIRE 608 1376 640 1376
            WIRE 640 1376 704 1376
            BEGIN DISPLAY 640 1376 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BEGIN BRANCH DataInOut(9)
            WIRE 608 1408 640 1408
            WIRE 640 1408 704 1408
            BEGIN DISPLAY 640 1408 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BEGIN BRANCH DataInOut(8)
            WIRE 608 1440 640 1440
            WIRE 640 1440 704 1440
            BEGIN DISPLAY 640 1440 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BEGIN BRANCH DataInOut(7)
            WIRE 608 1472 640 1472
            WIRE 640 1472 704 1472
            BEGIN DISPLAY 640 1472 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BEGIN BRANCH DataInOut(6)
            WIRE 608 1504 640 1504
            WIRE 640 1504 704 1504
            BEGIN DISPLAY 640 1504 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BEGIN BRANCH DataInOut(5)
            WIRE 608 1536 640 1536
            WIRE 640 1536 704 1536
            BEGIN DISPLAY 640 1536 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BEGIN BRANCH DataInOut(4)
            WIRE 608 1568 640 1568
            WIRE 640 1568 704 1568
            BEGIN DISPLAY 640 1568 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BEGIN BRANCH DataInOut(3)
            WIRE 608 1600 640 1600
            WIRE 640 1600 704 1600
            BEGIN DISPLAY 640 1600 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BEGIN BRANCH DataInOut(2)
            WIRE 608 1632 640 1632
            WIRE 640 1632 704 1632
            BEGIN DISPLAY 640 1632 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BEGIN BRANCH DataInOut(1)
            WIRE 608 1664 640 1664
            WIRE 640 1664 704 1664
            BEGIN DISPLAY 640 1664 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BEGIN BRANCH DataInOut(0)
            WIRE 608 1696 640 1696
            WIRE 640 1696 704 1696
            BEGIN DISPLAY 640 1696 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BEGIN BRANCH WR
            WIRE 1488 1136 1536 1136
            BEGIN DISPLAY 1488 1136 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH En0
            WIRE 1488 1088 1536 1088
            BEGIN DISPLAY 1488 1088 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH En1
            WIRE 1488 1440 1536 1440
            BEGIN DISPLAY 1488 1440 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH WR
            WIRE 1488 1488 1536 1488
            BEGIN DISPLAY 1488 1488 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH WR
            WIRE 1488 1840 1536 1840
            BEGIN DISPLAY 1488 1840 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH En2
            WIRE 1488 1792 1536 1792
            BEGIN DISPLAY 1488 1792 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH WR
            WIRE 1488 2192 1536 2192
            BEGIN DISPLAY 1488 2192 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH En3
            WIRE 1488 2144 1536 2144
            BEGIN DISPLAY 1488 2144 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH WR
            WIRE 2288 2192 2336 2192
            BEGIN DISPLAY 2288 2192 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH En7
            WIRE 2288 2144 2336 2144
            BEGIN DISPLAY 2288 2144 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH WR
            WIRE 2288 1840 2336 1840
            BEGIN DISPLAY 2288 1840 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH En6
            WIRE 2288 1792 2336 1792
            BEGIN DISPLAY 2288 1792 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH WR
            WIRE 2288 1488 2336 1488
            BEGIN DISPLAY 2288 1488 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH En5
            WIRE 2288 1440 2336 1440
            BEGIN DISPLAY 2288 1440 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH WR
            WIRE 2288 1136 2336 1136
            BEGIN DISPLAY 2288 1136 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH En4
            WIRE 2288 1088 2336 1088
            BEGIN DISPLAY 2288 1088 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH Reset
            WIRE 1488 1040 1504 1040
            WIRE 1504 1040 1536 1040
            BEGIN DISPLAY 1488 1040 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH Reset
            WIRE 1488 1392 1536 1392
            BEGIN DISPLAY 1488 1392 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH Reset
            WIRE 1488 1744 1536 1744
            BEGIN DISPLAY 1488 1744 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH Reset
            WIRE 1488 2096 1536 2096
            BEGIN DISPLAY 1488 2096 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH Reset
            WIRE 2288 2096 2336 2096
            BEGIN DISPLAY 2288 2096 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH Reset
            WIRE 2272 1744 2336 1744
            BEGIN DISPLAY 2272 1744 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH Reset
            WIRE 2288 1392 2336 1392
            BEGIN DISPLAY 2288 1392 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH Reset
            WIRE 2288 1040 2336 1040
            BEGIN DISPLAY 2288 1040 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH DataInOut(15:0)
            WIRE 800 1152 800 1216
            WIRE 800 1216 800 1248
            WIRE 800 1248 800 1280
            WIRE 800 1280 800 1312
            WIRE 800 1312 800 1344
            WIRE 800 1344 800 1376
            WIRE 800 1376 800 1408
            WIRE 800 1408 800 1440
            WIRE 800 1440 800 1472
            WIRE 800 1472 800 1504
            WIRE 800 1504 800 1536
            WIRE 800 1536 800 1568
            WIRE 800 1568 800 1600
            WIRE 800 1600 800 1632
            WIRE 800 1632 800 1664
            WIRE 800 1664 800 1696
            WIRE 800 1696 800 1712
            WIRE 800 1152 848 1152
            BEGIN DISPLAY 848 1152 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BUSTAP 800 1216 704 1216
        BUSTAP 800 1248 704 1248
        BUSTAP 800 1280 704 1280
        BUSTAP 800 1312 704 1312
        BUSTAP 800 1344 704 1344
        BUSTAP 800 1376 704 1376
        BUSTAP 800 1408 704 1408
        BUSTAP 800 1440 704 1440
        BUSTAP 800 1472 704 1472
        BUSTAP 800 1504 704 1504
        BUSTAP 800 1536 704 1536
        BUSTAP 800 1568 704 1568
        BUSTAP 800 1600 704 1600
        BUSTAP 800 1632 704 1632
        BUSTAP 800 1664 704 1664
        BUSTAP 800 1696 704 1696
        BEGIN BRANCH DataInOut(15:0)
            WIRE 1488 1184 1536 1184
            BEGIN DISPLAY 1488 1184 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH DataInOut(15:0)
            WIRE 1488 1536 1536 1536
            BEGIN DISPLAY 1488 1536 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH DataInOut(15:0)
            WIRE 1488 1888 1536 1888
            BEGIN DISPLAY 1488 1888 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH DataInOut(15:0)
            WIRE 1488 2240 1536 2240
            BEGIN DISPLAY 1488 2240 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH DataInOut(15:0)
            WIRE 2288 2240 2336 2240
            BEGIN DISPLAY 2288 2240 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH DataInOut(15:0)
            WIRE 2288 1888 2336 1888
            BEGIN DISPLAY 2288 1888 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH DataInOut(15:0)
            WIRE 2288 1536 2336 1536
            BEGIN DISPLAY 2288 1536 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH DataInOut(15:0)
            WIRE 2288 1184 2336 1184
            BEGIN DISPLAY 2288 1184 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH Reset
            WIRE 288 432 352 432
            BEGIN DISPLAY 352 432 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        IOMARKER 288 432 Reset R180 28
        BEGIN DISPLAY 1948 712 TEXT "11 = Divide By 8"
            FONT 22 "Arial"
        END DISPLAY
        BEGIN DISPLAY 1948 672 TEXT "01 = Divide By 2"
            FONT 22 "Arial"
        END DISPLAY
        BEGIN DISPLAY 1948 692 TEXT "10 = Divide By 4"
            FONT 22 "Arial"
        END DISPLAY
        BEGIN DISPLAY 1948 652 TEXT "00 = No Divide"
            FONT 22 "Arial"
        END DISPLAY
    END SHEET
END SCHEMATIC
