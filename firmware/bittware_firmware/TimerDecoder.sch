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
        SIGNAL Reset
        SIGNAL DataInOut(15:0)
        SIGNAL Cntrl(15:0)
        SIGNAL Delay(21:0)
        SIGNAL Width(21:0)
        SIGNAL Period(21:0)
        SIGNAL Mult(15:0)
        SIGNAL DataInOut(31:0)
        SIGNAL DataInOut(21:0)
        PORT Input Enable
        PORT Input AddrIn(2:0)
        PORT Input Clk
        PORT Input WR
        PORT Input Reset
        PORT Output Cntrl(15:0)
        PORT Output Delay(21:0)
        PORT Output Width(21:0)
        PORT Output Period(21:0)
        PORT Output Mult(15:0)
        PORT BiDirectional DataInOut(31:0)
        BEGIN BLOCKDEF register_16bit
            TIMESTAMP 2008 3 31 22 27 15
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
        BEGIN BLOCKDEF Register_19bit
            TIMESTAMP 2008 3 31 22 31 59
            RECTANGLE N 64 -256 320 0 
            LINE N 64 -224 0 -224 
            RECTANGLE N 320 -236 384 -212 
            LINE N 320 -224 384 -224 
            LINE N 64 -176 0 -176 
            RECTANGLE N 0 -44 64 -20 
            LINE N 0 -32 64 -32 
            LINE N 64 -80 0 -80 
            LINE N 64 -128 0 -128 
        END BLOCKDEF
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
            PIN Reset Reset
            PIN Enable En0
            PIN WR WR
            PIN DataIn(15:0) DataInOut(15:0)
            PIN DataOut(15:0) Cntrl(15:0)
        END BLOCK
        BEGIN BLOCK XLXI_19 register_16bit
            PIN Clk Clk
            PIN Reset Reset
            PIN Enable En4
            PIN WR WR
            PIN DataIn(15:0) DataInOut(15:0)
            PIN DataOut(15:0) Mult(15:0)
        END BLOCK
        BEGIN BLOCK XLXI_31 Register_19bit
            PIN Clk Clk
            PIN DataOut(21:0) Period(21:0)
            PIN Reset Reset
            PIN DataIn(21:0) DataInOut(21:0)
            PIN WR WR
            PIN Enable En3
        END BLOCK
        BEGIN BLOCK XLXI_32 Register_19bit
            PIN Clk Clk
            PIN DataOut(21:0) Width(21:0)
            PIN Reset Reset
            PIN DataIn(21:0) DataInOut(21:0)
            PIN WR WR
            PIN Enable En2
        END BLOCK
        BEGIN BLOCK XLXI_33 Register_19bit
            PIN Clk Clk
            PIN DataOut(21:0) Delay(21:0)
            PIN Reset Reset
            PIN DataIn(21:0) DataInOut(21:0)
            PIN WR WR
            PIN Enable En1
        END BLOCK
    END NETLIST
    BEGIN SHEET 1 3520 2720
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
        IOMARKER 256 992 Enable R180 28
        IOMARKER 448 1200 DataInOut(31:0) R180 28
        BEGIN BRANCH Reset
            WIRE 288 432 352 432
            BEGIN DISPLAY 352 432 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        IOMARKER 288 432 Reset R180 28
        BEGIN DISPLAY 1528 468 TEXT "Cntrl(1) = Invert Output (1) / Noninvert Output (0)"
            FONT 28 "Arial"
        END DISPLAY
        BEGIN DISPLAY 1528 516 TEXT "Cntrl(0) = Master On (1) / Off (0)"
            FONT 28 "Arial"
        END DISPLAY
        BEGIN DISPLAY 1528 372 TEXT "Cntrl(3) = Clk Divide LSB"
            FONT 28 "Arial"
        END DISPLAY
        BEGIN DISPLAY 1528 420 TEXT "Cntrl(2) = Global Clk (0) / Ext. Clk (1)"
            FONT 28 "Arial"
        END DISPLAY
        BEGIN DISPLAY 1528 324 TEXT "Cntrl(4) = Clk Divide MSB"
            FONT 28 "Arial"
        END DISPLAY
        BEGIN DISPLAY 1528 276 TEXT "Cntrl(15:5) = Not Used"
            FONT 28 "Arial"
        END DISPLAY
        BEGIN DISPLAY 1876 380 TEXT "11 = Divide By 8"
            FONT 22 "Arial"
        END DISPLAY
        BEGIN DISPLAY 1876 340 TEXT "01 = Divide By 2"
            FONT 22 "Arial"
        END DISPLAY
        BEGIN DISPLAY 1876 360 TEXT "10 = Divide By 4"
            FONT 22 "Arial"
        END DISPLAY
        BEGIN DISPLAY 1876 320 TEXT "00 = No Divide"
            FONT 22 "Arial"
        END DISPLAY
        BEGIN INSTANCE XLXI_1 1536 864 R0
        END INSTANCE
        BEGIN BRANCH Clk
            WIRE 1488 704 1536 704
            BEGIN DISPLAY 1488 704 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH Clk
            WIRE 1488 1056 1536 1056
            BEGIN DISPLAY 1488 1056 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH Clk
            WIRE 1488 1408 1536 1408
            BEGIN DISPLAY 1488 1408 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH Clk
            WIRE 1488 1760 1536 1760
            BEGIN DISPLAY 1488 1760 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH Cntrl(15:0)
            WIRE 1920 704 1952 704
        END BRANCH
        BEGIN BRANCH Delay(21:0)
            WIRE 1920 1056 1952 1056
        END BRANCH
        BEGIN BRANCH Width(21:0)
            WIRE 1920 1408 1952 1408
        END BRANCH
        BEGIN BRANCH Period(21:0)
            WIRE 1920 1760 1952 1760
        END BRANCH
        BEGIN BRANCH WR
            WIRE 1488 848 1536 848
            BEGIN DISPLAY 1488 848 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH En0
            WIRE 1488 800 1536 800
            BEGIN DISPLAY 1488 800 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH En1
            WIRE 1488 1152 1536 1152
            BEGIN DISPLAY 1488 1152 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH WR
            WIRE 1488 1200 1536 1200
            BEGIN DISPLAY 1488 1200 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH WR
            WIRE 1488 1552 1536 1552
            BEGIN DISPLAY 1488 1552 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH En2
            WIRE 1488 1504 1536 1504
            BEGIN DISPLAY 1488 1504 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH WR
            WIRE 1488 1904 1536 1904
            BEGIN DISPLAY 1488 1904 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH En3
            WIRE 1488 1856 1536 1856
            BEGIN DISPLAY 1488 1856 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH Reset
            WIRE 1488 752 1504 752
            WIRE 1504 752 1536 752
            BEGIN DISPLAY 1488 752 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH Reset
            WIRE 1488 1104 1536 1104
            BEGIN DISPLAY 1488 1104 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH Reset
            WIRE 1488 1456 1536 1456
            BEGIN DISPLAY 1488 1456 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH Reset
            WIRE 1488 1808 1536 1808
            BEGIN DISPLAY 1488 1808 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH DataInOut(15:0)
            WIRE 1488 896 1536 896
            BEGIN DISPLAY 1488 896 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH DataInOut(21:0)
            WIRE 1488 1248 1536 1248
            BEGIN DISPLAY 1488 1248 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH DataInOut(21:0)
            WIRE 1488 1600 1536 1600
            BEGIN DISPLAY 1488 1600 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH DataInOut(21:0)
            WIRE 1488 1952 1536 1952
            BEGIN DISPLAY 1488 1952 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        IOMARKER 1952 704 Cntrl(15:0) R0 28
        IOMARKER 1952 1056 Delay(21:0) R0 28
        IOMARKER 1952 1408 Width(21:0) R0 28
        IOMARKER 1952 1760 Period(21:0) R0 28
        BEGIN INSTANCE XLXI_19 1536 2272 R0
        END INSTANCE
        BEGIN BRANCH DataInOut(15:0)
            WIRE 1488 2304 1536 2304
            BEGIN DISPLAY 1488 2304 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH Reset
            WIRE 1488 2160 1536 2160
            BEGIN DISPLAY 1488 2160 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH En4
            WIRE 1488 2208 1536 2208
            BEGIN DISPLAY 1488 2208 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH WR
            WIRE 1488 2256 1536 2256
            BEGIN DISPLAY 1488 2256 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH Mult(15:0)
            WIRE 1920 2112 1952 2112
        END BRANCH
        BEGIN BRANCH Clk
            WIRE 1488 2112 1536 2112
            BEGIN DISPLAY 1488 2112 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        IOMARKER 1952 2112 Mult(15:0) R0 28
        BEGIN INSTANCE XLXI_31 1536 1984 R0
        END INSTANCE
        BEGIN INSTANCE XLXI_32 1536 1632 R0
        END INSTANCE
        BEGIN INSTANCE XLXI_33 1536 1280 R0
        END INSTANCE
        BEGIN BRANCH DataInOut(31:0)
            WIRE 448 1200 512 1200
            BEGIN DISPLAY 512 1200 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
    END SHEET
END SCHEMATIC
