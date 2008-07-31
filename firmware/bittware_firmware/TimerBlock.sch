VERSION 6
BEGIN SCHEMATIC
    BEGIN ATTR DeviceFamilyName "virtex2"
        DELETE all:0
        EDITNAME all:0
        EDITTRAIT all:0
    END ATTR
    BEGIN NETLIST
        SIGNAL Cntrl(15:0)
        SIGNAL Delay(21:0)
        SIGNAL Width(21:0)
        SIGNAL Period(21:0)
        SIGNAL Mult(15:0)
        SIGNAL TimerClkOut
        SIGNAL TimerEnable
        SIGNAL Enable
        SIGNAL PulseOut
        SIGNAL Timer_Rst
        SIGNAL TimerTrigger
        SIGNAL Cntrl(4:3)
        SIGNAL CS
        SIGNAL AddrIn(2:0)
        SIGNAL DataInOut(31:0)
        SIGNAL BusClk
        SIGNAL WR
        SIGNAL IntTimerClk
        SIGNAL TimerClk
        SIGNAL Cntrl(2)
        SIGNAL ClkSelect
        SIGNAL XLXN_17
        SIGNAL OnePPS
        SIGNAL AddrTrigger
        SIGNAL XLXN_23
        PORT Input Enable
        PORT Output PulseOut
        PORT Input Timer_Rst
        PORT Input CS
        PORT Input AddrIn(2:0)
        PORT BiDirectional DataInOut(31:0)
        PORT Input BusClk
        PORT Input WR
        PORT Input IntTimerClk
        PORT Input TimerClk
        PORT Input OnePPS
        PORT Input AddrTrigger
        BEGIN BLOCKDEF TimerDecoder
            TIMESTAMP 2008 6 20 18 45 39
            LINE N 64 -544 0 -544 
            RECTANGLE N 0 -492 64 -468 
            LINE N 64 -480 0 -480 
            RECTANGLE N 0 -428 64 -404 
            LINE N 0 -416 64 -416 
            LINE N 64 -352 0 -352 
            RECTANGLE N 64 -576 416 -224 
            LINE N 64 -304 0 -304 
            LINE N 64 -256 0 -256 
            RECTANGLE N 416 -556 480 -532 
            LINE N 416 -544 480 -544 
            RECTANGLE N 416 -492 480 -468 
            LINE N 416 -480 480 -480 
            RECTANGLE N 416 -428 480 -404 
            LINE N 416 -416 480 -416 
            RECTANGLE N 416 -364 480 -340 
            LINE N 416 -352 480 -352 
            RECTANGLE N 416 -300 480 -276 
            LINE N 416 -288 480 -288 
        END BLOCKDEF
        BEGIN BLOCKDEF TimerModule
            TIMESTAMP 2008 6 20 18 45 39
            LINE N 64 -160 0 -160 
            LINE N 64 -672 0 -672 
            LINE N 64 -608 0 -608 
            LINE N 64 -544 0 -544 
            RECTANGLE N 0 -492 64 -468 
            LINE N 64 -480 0 -480 
            LINE N 320 -672 384 -672 
            RECTANGLE N 0 -428 64 -404 
            LINE N 64 -416 0 -416 
            RECTANGLE N 0 -364 64 -340 
            LINE N 64 -352 0 -352 
            RECTANGLE N 0 -300 64 -276 
            LINE N 64 -288 0 -288 
            RECTANGLE N 0 -236 64 -212 
            LINE N 64 -224 0 -224 
            RECTANGLE N 64 -704 320 -128 
        END BLOCKDEF
        BEGIN BLOCKDEF clockdivider
            TIMESTAMP 2008 6 20 18 45 39
            RECTANGLE N 64 -128 320 0 
            LINE N 64 -96 0 -96 
            RECTANGLE N 0 -44 64 -20 
            LINE N 64 -32 0 -32 
            LINE N 320 -96 384 -96 
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
        BEGIN BLOCKDEF clockselect
            TIMESTAMP 2008 6 20 18 45 39
            RECTANGLE N 64 -192 320 0 
            LINE N 64 -160 0 -160 
            LINE N 64 -96 0 -96 
            LINE N 64 -32 0 -32 
            LINE N 320 -160 384 -160 
        END BLOCKDEF
        BEGIN BLOCKDEF and2
            TIMESTAMP 2000 1 1 10 10 10
            LINE N 0 -64 64 -64 
            LINE N 0 -128 64 -128 
            LINE N 256 -96 192 -96 
            ARC N 96 -144 192 -48 144 -48 144 -144 
            LINE N 144 -48 64 -48 
            LINE N 64 -144 144 -144 
            LINE N 64 -48 64 -144 
        END BLOCKDEF
        BEGIN BLOCK XLXI_4 fd
            PIN C TimerClkOut
            PIN D Enable
            PIN Q TimerEnable
        END BLOCK
        BEGIN BLOCK XLXI_2 TimerModule
            PIN Clk TimerClkOut
            PIN Enable TimerEnable
            PIN Trigger TimerTrigger
            PIN Rst Timer_Rst
            PIN Cntrl(15:0) Cntrl(15:0)
            PIN Mult(15:0) Mult(15:0)
            PIN Delay(21:0) Delay(21:0)
            PIN Width(21:0) Width(21:0)
            PIN Period(21:0) Period(21:0)
            PIN PulseOut PulseOut
        END BLOCK
        BEGIN BLOCK XLXI_7 clockselect
            PIN ClkIn TimerClk
            PIN IntClkIn IntTimerClk
            PIN Sel Cntrl(2)
            PIN ClkOut ClkSelect
        END BLOCK
        BEGIN BLOCK XLXI_3 clockdivider
            PIN Clk ClkSelect
            PIN Sel(1:0) Cntrl(4:3)
            PIN ClkOut TimerClkOut
        END BLOCK
        BEGIN BLOCK XLXI_1 TimerDecoder
            PIN Enable CS
            PIN AddrIn(2:0) AddrIn(2:0)
            PIN Clk BusClk
            PIN WR WR
            PIN Reset Timer_Rst
            PIN DataInOut(31:0) DataInOut(31:0)
            PIN Cntrl(15:0) Cntrl(15:0)
            PIN Delay(21:0) Delay(21:0)
            PIN Width(21:0) Width(21:0)
            PIN Period(21:0) Period(21:0)
            PIN Mult(15:0) Mult(15:0)
        END BLOCK
        BEGIN BLOCK XLXI_5 fd
            PIN C TimerClkOut
            PIN D XLXN_17
            PIN Q TimerTrigger
        END BLOCK
        BEGIN BLOCK XLXI_8 and2
            PIN I0 XLXN_23
            PIN I1 TimerEnable
            PIN O XLXN_17
        END BLOCK
        BEGIN BLOCK XLXI_6 or2
            PIN I0 AddrTrigger
            PIN I1 OnePPS
            PIN O XLXN_23
        END BLOCK
    END NETLIST
    BEGIN SHEET 1 2720 1760
        INSTANCE XLXI_4 576 384 R0
        BEGIN BRANCH TimerClkOut
            WIRE 544 256 576 256
            BEGIN DISPLAY 544 256 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH Enable
            WIRE 544 128 576 128
        END BRANCH
        BEGIN BRANCH TimerEnable
            WIRE 960 128 1040 128
            WIRE 960 128 960 336
            WIRE 960 336 1008 336
            BEGIN DISPLAY 1040 128 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        IOMARKER 544 128 Enable R180 28
        BEGIN BRANCH TimerClkOut
            WIRE 1536 768 1616 768
            BEGIN DISPLAY 1536 768 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH TimerEnable
            WIRE 1536 832 1616 832
            BEGIN DISPLAY 1536 832 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH Cntrl(15:0)
            WIRE 1520 960 1616 960
            BEGIN DISPLAY 1520 960 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH Delay(21:0)
            WIRE 1520 1024 1616 1024
            BEGIN DISPLAY 1520 1024 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH Width(21:0)
            WIRE 1520 1088 1616 1088
            BEGIN DISPLAY 1520 1088 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH Period(21:0)
            WIRE 1520 1152 1616 1152
            BEGIN DISPLAY 1520 1152 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH Mult(15:0)
            WIRE 1520 1216 1616 1216
            BEGIN DISPLAY 1520 1216 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH PulseOut
            WIRE 2000 768 2064 768
        END BRANCH
        BEGIN INSTANCE XLXI_2 1616 1440 R0
        END INSTANCE
        IOMARKER 2064 768 PulseOut R0 28
        BEGIN BRANCH Timer_Rst
            WIRE 1520 1280 1616 1280
            BEGIN DISPLAY 1520 1280 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH TimerTrigger
            WIRE 1520 896 1616 896
            BEGIN DISPLAY 1520 896 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN INSTANCE XLXI_7 576 928 R0
        END INSTANCE
        BEGIN INSTANCE XLXI_3 576 1152 R0
        END INSTANCE
        BEGIN BRANCH TimerClkOut
            WIRE 960 1056 1008 1056
            BEGIN DISPLAY 1008 1056 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH Cntrl(4:3)
            WIRE 544 1120 576 1120
            BEGIN DISPLAY 544 1120 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN INSTANCE XLXI_1 576 1808 R0
        END INSTANCE
        BEGIN BRANCH CS
            WIRE 544 1264 576 1264
        END BRANCH
        BEGIN BRANCH AddrIn(2:0)
            WIRE 544 1328 576 1328
        END BRANCH
        BEGIN BRANCH DataInOut(31:0)
            WIRE 544 1392 576 1392
        END BRANCH
        BEGIN BRANCH BusClk
            WIRE 544 1456 576 1456
        END BRANCH
        BEGIN BRANCH WR
            WIRE 544 1504 576 1504
        END BRANCH
        BEGIN BRANCH Timer_Rst
            WIRE 544 1552 576 1552
        END BRANCH
        BEGIN BRANCH Cntrl(15:0)
            WIRE 1056 1264 1088 1264
            BEGIN DISPLAY 1088 1264 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH Delay(21:0)
            WIRE 1056 1328 1088 1328
            BEGIN DISPLAY 1088 1328 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH Width(21:0)
            WIRE 1056 1392 1088 1392
            BEGIN DISPLAY 1088 1392 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH Period(21:0)
            WIRE 1056 1456 1088 1456
            BEGIN DISPLAY 1088 1456 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH Mult(15:0)
            WIRE 1056 1520 1088 1520
            BEGIN DISPLAY 1088 1520 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        IOMARKER 544 1264 CS R180 28
        IOMARKER 544 1328 AddrIn(2:0) R180 28
        IOMARKER 544 1392 DataInOut(31:0) R180 28
        IOMARKER 544 1456 BusClk R180 28
        IOMARKER 544 1504 WR R180 28
        IOMARKER 544 1552 Timer_Rst R180 28
        BEGIN BRANCH IntTimerClk
            WIRE 480 832 560 832
            WIRE 560 832 576 832
        END BRANCH
        IOMARKER 480 832 IntTimerClk R180 28
        BEGIN BRANCH TimerClk
            WIRE 480 768 560 768
            WIRE 560 768 576 768
        END BRANCH
        IOMARKER 480 768 TimerClk R180 28
        BEGIN BRANCH Cntrl(2)
            WIRE 544 896 576 896
            BEGIN DISPLAY 544 896 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH ClkSelect
            WIRE 960 768 992 768
            BEGIN DISPLAY 992 768 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH ClkSelect
            WIRE 544 1056 576 1056
            BEGIN DISPLAY 544 1056 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH TimerClkOut
            WIRE 560 496 576 496
            WIRE 576 496 1296 496
            BEGIN DISPLAY 560 496 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH TimerTrigger
            WIRE 1680 368 1744 368
            WIRE 1744 368 1760 368
            BEGIN DISPLAY 1760 368 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH XLXN_17
            WIRE 1264 368 1280 368
            WIRE 1280 368 1296 368
        END BRANCH
        INSTANCE XLXI_6 592 496 R0
        BEGIN BRANCH OnePPS
            WIRE 560 368 592 368
        END BRANCH
        BEGIN BRANCH AddrTrigger
            WIRE 560 432 576 432
            WIRE 576 432 592 432
        END BRANCH
        IOMARKER 560 368 OnePPS R180 28
        IOMARKER 560 432 AddrTrigger R180 28
        BEGIN BRANCH XLXN_23
            WIRE 848 400 864 400
            WIRE 864 400 1008 400
        END BRANCH
        INSTANCE XLXI_8 1008 464 R0
        INSTANCE XLXI_5 1296 624 R0
    END SHEET
END SCHEMATIC
