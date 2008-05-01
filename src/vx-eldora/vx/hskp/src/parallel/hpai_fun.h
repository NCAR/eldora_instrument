/*    Module:     hpai_fun.h
      Author:     Craig Walther
      Copyright:  National Center For Atmospheric Research
      Date:       October 9, 1991
      Purpose:    This include module declares all of the functions
                  that are used with the high power amplifier's
                  controller software
*/
scope void relay_cl();         /* Closes selected relays */
scope void relay_op();         /* Opens selected relays */
scope float opto_in();         /* Reads the selected optical input */
scope unsigned char para_in(); /* Reads the selected PIO port */
scope void para_off();         /*Sets selected output low on selected PIO port */
scope void para_on();          /*Sets selected output high on selected PIO port*/
scope void sample();           /* Samples all channels */
scope void calibrate();        /* Calibrates all channels */
scope void init_ports();       /* Initializes the two serial ports */
scope void test_errors();      /* Tests each input for errors */
scope short ad_in();           /* Sets gain reads A/D card channel specified */
scope short ad_init();         /* Initializes the A/D card */
scope void para_init();        /* Initializes the PIO card */
scope void gen_isr();          /* General interrupt service routine */
scope void timer_init();       /* Initialize the timer interrupt */
scope void prnt_scrn();        /* Print on the IEE Daystar Nova Module */
scope void asm_isr();          /* assembly code interrupt service routine */
scope void test_commands();    /* Test commands and take action */
scope void prnt_status();      /* Print status (on screen or remote port) */
scope void prnt_port();        /* Send information to the remote port */
scope short compare();         /*Compare commands, remote port to known inputs */
scope void dn_off();           /* Take HPA from standby or operate to off */
scope void dn_standby();       /* Take HPA from operate to standby */
scope void up_operate();       /* Take HPA from standby to operate */
scope void up_standby();       /* Take HPA from off to standby */
scope short index_increment(); /* Increments the fault index skips unused faults*/
scope void user_interact();    /* User interaction subroutime */
scope void pulse();            /* Pulses unused relay for timing analysis */
scope short cllbdyon();        /* Turns on collector and body supplies*/
scope short cllbdyoff();       /* Turns off collector and body supplies*/
scope void read_eeprom();      /* Reads parameters out of eeprom */
scope void write_eeprom();     /* writes parameters to eeprom */
scope void eetosram();         /* moves parameters out of EEPROM into SRAM */
scope void sramtoee();         /* moves parameters out of SRAM into EEPROM */
scope void setup();            /* user interface to setup EEPROM */
scope unsigned char in_from_b(); /* read integer input from port b */
scope float fin_from_b();      /* read floating point input from port b */
scope double cwatof();         /* Ascii to floating point converter */
scope void display_init();     /* Initialize standard display */
scope void display_update();   /* Update standard display */