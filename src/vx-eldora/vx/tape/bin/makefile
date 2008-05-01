#
#	Makefile for Directory ELDORA/vx/tape/bin
#	Author: Craig Walther
#	Date: 9/1/92
#
# Loads the houskeeping processor binaries together 
#
COMMON= /net/projects/common
#
OBJS= ads_length.o cal_sum.o chan_attn.o cip_cmds.o cip_init.o \
data_ray_length.o dlt_cmds.o drive_init.o drv_sel.o fill_sg.o flag_check.o\
initialize_tape.o mail_box.o md_sel_init.o md_sns_init.o nav_length.o\
parm_blk_init.o print_log_sns.o print_md_sns.o print_results.o\
print_rqst_sns.o print_stat.o read_tape.o reduce.o rqst_sns_init.o\
sg_init.o space_tape.o startRPC.o startup.o tape_cntrl_len.o\
tape_header.o tape_menu.o tape_remain.o tst_unt_rdy.o write_tape.o\
para_in.o para_init.o para_menu.o para_off.o para_on.o\
/ELDORA/vx/c++/bin/Task.o  /ELDORA/vx/tp41/tp41Lib.o\
/ELDORA/vx/c++/bin/Flags.o \
/ELDORA/rpc/lib/libVxLogger.a /ELDORA/rpc/lib/libVxHeader.a\
-L${COMMON}/lib/vxworks -lNet \
-L/local/lib/gcc-lib/68k/current -lstdc++ -lgcc

.KEEP_STATE:

combine:
	ld68k -X -Ur -o etape.o $(OBJS)
