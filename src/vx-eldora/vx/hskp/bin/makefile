#
#	Makefile for Directory ELDORA/vx/hskp/bin
#	Author: Craig Walther
#	Date: 9/1/92
#
# Loads the houskeeping processor binaries together 
#
#
COMM_DIR= /net/projects

OBJS= globals.o  command_gps.o vmevme_isr.o ReadStatusByte.o\
fill_nav_info.o get_time.o gps_isr.o gps_menu.o hskp.o\
init_clock.o init_gps.o init_vmevme.o read_data.o set_time.o\
start_clock.o start_vmevme.o startup.o stop_vmevme.o sync_irig.o\
tod_menu.o view_time.o fill_platform.o write_data.o go_motor.o goto_pos.o\
init_motor.o motor_menu.o get_pos.o read_reg.o read_rpm.o stop_motor.o\
set_vel.o write_reg.o init_iru.o /net/eldora/eldora/vx/tp41/tp41Lib.o\
Return_to_Local.o\
vmevme_menu.o convert_iru.o start_iru.o iru_menu.o ecbInts.o\
ecbGets.o ecbSets.o ecbShows.o ecbUtils.o mstrFuncs.o midbeam.o\
hskpIntInit.o just_set_vel.o hostStartup.o main_menu.o start_testpulse.o\
update_testpulse.o sum_testpulse.o fltpt.o init_ptrs.o lab_test.o\
send_cmnd_string.o xmit_err_isr.o init_dma.o listen.o start_ieee.o\
xmit_norm_isr.o init_gpib.o testp_rt.o testp_err_isr.o init_isr_vecs.o\
talk.o init_ieee.o stop_ieee.o xmit_rt.o test_pwr.o pgm_rt.o ReceiveData.o\
pwrmenu.o testp_norm_isr.o init_mini.o command_mini.o mini_isr.o\
get_year.o clock_menu.o send_packet.o mini_menu.o test_pos.o trap_pos.o\
pos_loop.o /net/eldora/eldora/rpc/lib/libVxHeader.a\
/net/eldora/eldora/vx/vmevme_int/sendInt.o -L${COMM_DIR}/common/lib/vxworks -lNet\
-L/usr/local/lib/gcc-lib/m68k-wrs-vxworks/current -lstdc++ -lgcc

.KEEP_STATE:

combine:
	ld68k -X -Ur -o housekeeper.o $(OBJS)
