VERSION 6
BEGIN SCHEMATIC
    BEGIN ATTR DeviceFamilyName "virtex2"
        DELETE all:0
        EDITNAME all:0
        EDITTRAIT all:0
    END ATTR
    BEGIN NETLIST
        SIGNAL Cntrl(15:0)
        SIGNAL Delay(15:0)
        SIGNAL Width(15:0)
        SIGNAL Period(15:0)
        SIGNAL Mult(15:0)
        SIGNAL TimerClkOut
        SIGNAL TimerEnable
        SIGNAL TimerClk
        SIGNAL Cntrl(4:3)
        SIGNAL Enable
        SIGNAL XLXN_1
        SIGNAL OnePPS
        SIGNAL PulseOut
        SIGNAL CS
        SIGNAL AddrIn(2:0)
        SIGNAL DataInOut(31:0)
        SIGNAL BusClk
        SIGNAL WR
        SIGNAL Timer_Rst
        SIGNAL AddrTrigger
        SIGNAL TimerTrigger
        PORT Input TimerClk
        PORT Input Enable
        PORT Input OnePPS
        PORT Output PulseOut
        PORT Input CS
        PORT Input AddrIn(2:0)
        PORT BiDirectional DataInOut(31:0)
        PORT Input BusClk
        PORT Input WR
        PORT Input Timer_Rst
        PORT Input AddrTrigger
        BEGIN BLOCKDEF TimerDecoder
            TIMESTAMP 2007 7 11 16 32 44
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
            TIMESTAMP 2007 7 11 16 55 17
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
            TIMESTAMP 2006 3 9 0 8 45
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
        BEGIN BLOCK XLXI_3 clockdivider
            PIN Clk TimerClk
            PIN Sel(1:0) Cntrl(4:3)
            PIN ClkOut TimerClkOut
        END BLOCK
        BEGIN BLOCK XLXI_5 fd
            PIN C TimerClkOut
            PIN D XLXN_1
            PIN Q TimerTrigger
        END BLOCK
        BEGIN BLOCK XLXI_4 fd
            PIN C TimerClkOut
            PIN D Enable
            PIN Q TimerEnable
        END BLOCK
        BEGIN BLOCK XLXI_6 or2
            PIN I0 AddrTrigger
            PIN I1 OnePPS
            PIN O XLXN_1
        END BLOCK
        BEGIN BLOCK XLXI_2 TimerModule
            PIN Clk TimerClkOut
            PIN Enable TimerEnable
            PIN Trigger TimerTrigger
            PIN Rst Timer_Rst
            PIN Cntrl(15:0) Cntrl(15:0)
            PIN Mult(15:0) Mult(15:0)
            PIN Delay(15:0) Delay(15:0)
            PIN Width(15:0) Width(15:0)
            PIN Period(15:0) Period(15:0)
            PIN PulseOut PulseOut
        END BLOCK
        BEGIN BLOCK XLXI_1 TimerDecoder
            PIN Enable CS
            PIN AddrIn(2:0) AddrIn(2:0)
            PIN Clk BusClk
            PIN DataInOut(31:0) DataInOut(31:0)
            PIN WR WR
            PIN Reset Timer_Rst
            PIN Cntrl(15:0) Cntrl(15:0)
            PIN Delay(15:0) Delay(15:0)
            PIN Width(15:0) Width(15:0)
            PIN Period(15:0) Period(15:0)
            PIN Mult(15:0) Mult(15:0)
        END BLOCK
    END NETLIST
    BEGIN SHEET 1 2720 1760
        BEGIN INSTANCE XLXI_3 576 864 R0
        END INSTANCE
        BEGIN BRANCH TimerClkOut
            WIRE 960 768 1008 768
            BEGIN DISPLAY 1008 768 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH TimerClk
            WIRE 544 768 576 768
        END BRANCH
        BEGIN BRANCH Cntrl(4:3)
            WIRE 544 832 576 832
            BEGIN DISPLAY 544 832 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        INSTANCE XLXI_5 576 704 R0
        INSTANCE XLXI_4 576 384 R0
        BEGIN BRANCH TimerClkOut
            WIRE 544 576 576 576
            BEGIN DISPLAY 544 576 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
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
            WIRE 960 128 992 128
            BEGIN DISPLAY 992 128 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        INSTANCE XLXI_6 288 544 R0
        BEGIN BRANCH XLXN_1
            WIRE 544 448 576 448
        END BRANCH
        BEGIN BRANCH OnePPS
            WIRE 256 416 288 416
        END BRANCH
        IOMARKER 544 768 TimerClk R180 28
        IOMARKER 544 128 Enable R180 28
        IOMARKER 256 416 OnePPS R180 28
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
        BEGIN BRANCH Delay(15:0)
            WIRE 1520 1024 1616 1024
            BEGIN DISPLAY 1520 1024 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH Width(15:0)
            WIRE 1520 1088 1616 1088
            BEGIN DISPLAY 1520 1088 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH Period(15:0)
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
        BEGIN INSTANCE XLXI_1 576 1520 R0
        END INSTANCE
        BEGIN BRANCH CS
            WIRE 544 976 576 976
        END BRANCH
        BEGIN BRANCH AddrIn(2:0)
            WIRE 544 1040 576 1040
        END BRANCH
        BEGIN BRANCH DataInOut(31:0)
            WIRE 544 1104 576 1104
        END BRANCH
        BEGIN BRANCH BusClk
            WIRE 544 1168 576 1168
        END BRANCH
        BEGIN BRANCH WR
            WIRE 544 1216 576 1216
        END BRANCH
        BEGIN BRANCH Timer_Rst
            WIRE 544 1264 576 1264
        END BRANCH
        BEGIN BRANCH Cntrl(15:0)
            WIRE 1056 976 1088 976
            BEGIN DISPLAY 1088 976 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH Delay(15:0)
            WIRE 1056 1040 1088 1040
            BEGIN DISPLAY 1088 1040 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH Width(15:0)
            WIRE 1056 1104 1088 1104
            BEGIN DISPLAY 1088 1104 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH Period(15:0)
            WIRE 1056 1168 1088 1168
            BEGIN DISPLAY 1088 1168 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH Mult(15:0)
            WIRE 1056 1232 1088 1232
            BEGIN DISPLAY 1088 1232 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        IOMARKER 544 976 CS R180 28
        IOMARKER 544 1040 AddrIn(2:0) R180 28
        IOMARKER 544 1104 DataInOut(31:0) R180 28
        IOMARKER 544 1168 BusClk R180 28
        IOMARKER 544 1216 WR R180 28
        IOMARKER 544 1264 Timer_Rst R180 28
        BEGIN BRANCH Timer_Rst
            WIRE 1520 1280 1616 1280
            BEGIN DISPLAY 1520 1280 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
        BEGIN BRANCH AddrTrigger
            WIRE 256 480 272 480
            WIRE 272 480 288 480
        END BRANCH
        BEGIN BRANCH TimerTrigger
            WIRE 960 448 992 448
            BEGIN DISPLAY 992 448 ATTR Name
                ALIGNMENT SOFT-LEFT
            END DISPLAY
        END BRANCH
        IOMARKER 256 480 AddrTrigger R180 28
        BEGIN BRANCH TimerTrigger
            WIRE 1520 896 1616 896
            BEGIN DISPLAY 1520 896 ATTR Name
                ALIGNMENT SOFT-RIGHT
            END DISPLAY
        END BRANCH
    END SHEET
END SCHEMATIC
