/*
 *	$Id$
 *
 *	Module canned_xfer.c 		 
 *	Original Author: Craig Walther 
 *      Copywrited by the National Center for Atmospheric Research
 *	date  $Date$
 *
 * revision history
 * ----------------
 * $Log$
 *
 * description:  This program copies what the radar processor must do to
 *               the Multi Crate Pipeline (MCPL) and it sends known
 *               dummy data over the link.
 *
 */

#define OK_RPC
#define scope
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";
#include </vx/h/vxWorks.h>
#include </vx/h/stdioLib.h>
#include </vx/h/math.h>
#include </vx/h/taskLib.h>
#include </home/craig/mcpl/include/mcpl_def.h>
#include </ELDORA/rpc/include/Parameter.h>
#include </ELDORA/rpc/include/RadarDesc.h>
#include </ELDORA/rpc/include/Header.h>
#include </ELDORA/rpc/include/CellSpacing.h>
#include </ELDORA/rpc/include/Ray.h>
#include </ELDORA/rpc/include/Platform.h>
#include </ELDORA/rpc/include/FieldParam.h>

HeaderPtr header_pointer;

struct RAY
  {
  struct ray_i rayinfo;
  struct platform_i pltfrminfo;
  struct field_parameter_data fielddata;
  short  data [500][3];
  };

struct RAY ray;

/* Reserve one space for entire MAD structure to transfer */
struct MAD gmad;

fastxfer()
{
    struct TPB  init;       /* Initializing Transfer Parameter Block (TPB) */
    struct TPB  tpb_to_proc; /* TPB to send TPB to any target processor */
    struct TPB  receive_gmad; /* TPB to receive a MAD structure */
    struct TPB  gmad_to_proc; /* TPB to send a MAD structure to any processor */
    struct TPB  receive_data; /* TPB to receive data on a remote node */
    struct TPB  bcast_tpb; /* TPB to broadcast data to all processors */
    
    /* Pointers to all defined TPB's */
    long *init_pntr, *bcast_tpb_pntr, *receive_data_pntr;
    long *tpb_to_proc_pntr,*gmad_to_proc_pntr,*receive_gmad_pntr;
    
    /* Pointers to other MCPL registers, storage for register values */
    short *status_reg, *control_reg, *short_fifo, *start_reg;
    long *long_fifo, *long_pntr;
    short status, control, start, mailbox;
    
    /* Arrays for address storage to mailboxes and data */
    
    int number_of_mads[3]; /* Number of mads in each processor 0=> no processor */
    int current_mad[3];    /* Current mad (Either 0 or 1) in each processor */
    short proc_nodes[3];   /* Array of node numbers for each processor */
#define DIS 0          /* All above arrays display processor is entry 0 */
#define REC 1          /* All above arrays recording processor is entry 1 */
#define GRID 2         /* All above arrays gridding processor is entry 2 */
    
    long madinfo[3][2][7];
    /* First dimension is processor number display=0, record=1, gridding=2
       Second dimension is for each mad in a processor: 2 maximum, 1 minimum
       Third dimension is for various defined parameters:
       [P] [M] [0] base address of MAD M for processor P
       [P] [M] [1] Number of data areas in MAD M for processor P
       [P] [M] [2] Size of data areas in MAD M for processor P (in Bytes)
       [P] [M] [3] Completion status word for each different processor
       [P] [M] [4] Number of current data area in MAD M for processor P
       [P] [M] [6] Maximum Size of memory available for MAD M in processor P */
    
#define MAD_BASE     0
#define NUM_AREAS    1
#define SIZE_AREAS   2
#define COMPLETE_S   3
#define NUM_C_AREA   4
#define MAX_MEM      6
    
    long *gmad_pntr;
    
    /* Define other needed tape structures */
    struct radar_d this_radar;
    struct cell_spacing_d cellspace;
    struct parameter_d param[8];
    
    /* General purpose variables */
    long i, j, timeout, sleep_time;
    int kill, proc, mad, logical_length;
    long current_ray_add, current_mailbox_add, seed;
    short *mailbox_test, number_procs, proc_count;
    float step;
    float ran2();
    
    /* Initialize general purpose variables */
    mailbox = 0;
    kill = 0;
    
    /* Initalize the arrays that index into the MAD descriptor arrays */
    current_mad[DIS] = 0;
    current_mad[REC] = 0;
    current_mad[GRID] = 0;
    
    number_of_mads[DIS] = NUM_STRUCT_DIS;
    number_of_mads[REC] = NUM_STRUCT_REC;
    number_of_mads[GRID] = NUM_STRUCT_GRID;
    
    proc_nodes[DIS] = DISPLAY_NODE;
    proc_nodes[REC] = RECORD_NODE;
    proc_nodes[GRID] = GRID_NODE;
    
    
    madinfo[DIS][0][MAD_BASE] = MADBASE_DIS_0;
    madinfo[DIS][1][MAD_BASE] = MADBASE_DIS_1;
    madinfo[REC][0][MAD_BASE] = MADBASE_REC_0;
    madinfo[REC][1][MAD_BASE] = MADBASE_REC_1;
    madinfo[GRID][0][MAD_BASE] = MADBASE_GRID_0;
    madinfo[GRID][1][MAD_BASE] = MADBASE_GRID_1;
    
    madinfo[DIS][0][COMPLETE_S] = C_STATUS_DIS;
    madinfo[DIS][1][COMPLETE_S] = C_STATUS_DIS;
    madinfo[REC][0][COMPLETE_S] = C_STATUS_REC;
    madinfo[REC][1][COMPLETE_S] = C_STATUS_REC;
    madinfo[GRID][0][COMPLETE_S] = C_STATUS_GRID;
    madinfo[GRID][1][COMPLETE_S] = C_STATUS_GRID;
    
    madinfo[DIS][0][MAX_MEM] = DIS_MEM_SIZE_0;
    madinfo[DIS][1][MAX_MEM] = DIS_MEM_SIZE_1;
    madinfo[REC][0][MAX_MEM] = REC_MEM_SIZE_0;
    madinfo[REC][1][MAX_MEM] = REC_MEM_SIZE_1;
    madinfo[GRID][0][MAX_MEM] = GRID_MEM_SIZE_0;
    madinfo[GRID][1][MAX_MEM] = GRID_MEM_SIZE_1;
    
    madinfo[DIS][0][NUM_C_AREA] = 0;
    madinfo[DIS][1][NUM_C_AREA] = 0;
    madinfo[REC][0][NUM_C_AREA] = 0;
    madinfo[REC][1][NUM_C_AREA] = 0;
    madinfo[GRID][0][NUM_C_AREA] = 0;
    madinfo[GRID][1][NUM_C_AREA] = 0;
    
    /* Set addresses for the MCPL card */
    
    status_reg =  (short *)(MCPL_BASE + STATUS_REG);
    control_reg = (short *)(MCPL_BASE + CONTROL_REG);
    short_fifo =  (short *)(MCPL_BASE + FIFO_REG);
    long_fifo =   (long *)(MCPL_BASE + FIFO_REG);
    start_reg =   (short *)(MCPL_BASE + START_REG);
    
    init_pntr =    (long *)&init.source_start_address;
    tpb_to_proc_pntr = (long *)&tpb_to_proc.source_start_address;
    receive_gmad_pntr = (long *)&receive_gmad.source_start_address;
    receive_data_pntr = (long *)&receive_data.source_start_address;
    gmad_to_proc_pntr = (long *)&gmad_to_proc.source_start_address;
    bcast_tpb_pntr =   (long *)&bcast_tpb.source_start_address;
    gmad_pntr =   (long *)&gmad.sync_word;
    
    /* Set up the initial TPB */
    
    init.source_start_address = 0;
    init.sink_start_address = 0;
    init.source_attribute = 0;
    init.sink_attribute = 0;
    init.transfer_length = 0;
    init.completion_status = 0x0002;  /* Disable interrupt Mailbox A24/D16OB */
    init.nack_count = 0;
    init.link_address = (long)tpb_to_proc_pntr;
    init.link_attribute = 0x3933;     /* Normal VMEBus Addressing, VME link after
					 trigger, address modifier $39, disable
					 BLT, 32 Xfers/burst, disable pipelineing,
					 link to VME */
    init.mcpl_control_word = 0;
    init.mailbox_address = (long)&mailbox;
    
    printf("mailbox address is: %lx\n",&mailbox);
    
    /* Check status register to see if this is a start from power-up */
    
    status = *status_reg;
    printf("Initial card status is: %x\n",status);
    
    /* If MCPL has not been initialized since power on status bit 0 will be set */
    
    if((status & 0x0001) != 0)
      {
	  printf("First MCPL access since power on\n");
	  *control_reg = HOLD;
	  *start_reg = 0x0001;
	  
	  timeout = 0;
	  do
	    {
		status = *status_reg;
		timeout++;
	    } while (((status & 0x0001) != 0) && (timeout < 300));
	  
	  if(timeout >= 300)
	    {
		printf("MCPL card still busy after issueing hold command, must exit\n");
		exit(0);
	    }
      }
    
    /* if this is not the first time the program has run, we need to send the hold
       command, but the card will respond by a "hold acknowledged" status in the
       mailbox */
    
    else
      {
	  printf("MCPL already running, must send hold command to stop\n");
	  
	  *control_reg = HOLD;
	  *start_reg = 0x0001;
	  
	  /* check for acknowledge of hold command */
	  
	  timeout = 0;
	  mailbox = 0;
	  do
	    {
		timeout++;
	    } while ((mailbox == 0) && (timeout < 3000));
	  
	  status = *status_reg;
	  if(mailbox != 0x2fff)
	    {
		printf("Hold command not acknowledged by MCPL, must exit\n");
		exit(1);
	    }
      }
    
    /* Send the init TPB to the MCPL */
    
    printf("Sending Initialization TPB \n");
    
    *control_reg = READ_INIT_TPB;
    long_pntr = (long *)&init.source_start_address;
    for(i=0; i<8; i++)
      *long_fifo = *(long_pntr + i);
    
    *start_reg = 0x0001;
    
    timeout = 0;
    mailbox = 0;
    do
      {
	  timeout++;
      } while ((mailbox == 0) && (timeout < 3000));
    
    status = *status_reg;
    if(mailbox == 0x1fff)
      printf("Init TPB transfer successful status is: %x\n",status);
    else
      {
	  printf("Init TPB transfer unsuccessful mailbox: %x status: %x\n",
		 mailbox,status);
	  exit(2);
      }
    
    /***************************************************************************/
    /* Setup Generic parts of TPB's to send a MAD structures to the processors */
    /***************************************************************************/
    
    /* The following lines are for tests only!! remove for real thing */
    cellspace.num_segments = 1;
    cellspace.num_cells[0] = 400;
    this_radar.num_parameter_des = 3;
    param[0].binary_format = 2;
    param[1].binary_format = 2;
    param[2].binary_format = 2;
    current_ray_add = (long)&ray.rayinfo.ray_info;
    current_mailbox_add = (long)&mailbox; 
    ray.rayinfo.true_scan_rate = 120;
    /* End of line that are for test only */
    
    /* The tpb_to_proc TPB sends the receive_gmad_tpb to a processor's dispatch
       table entry one.  All of the general purpose parmeters in the TPB will be
       set now and processor specific ones will be set later */
    
    tpb_to_proc.source_start_address = (long)receive_gmad_pntr;
    /* Data to be retrieved from
       receive_gmad structure */
    tpb_to_proc.sink_start_address = 0;           /* Going to an mcpl node so
						     no address is necessary */
    tpb_to_proc.source_attribute = 0x3963; /* Normal VMEBus Addressing, address
					      modifier $39, disable pipelining,
					      disable BLT, 256 transfers per burst,
					      32 bit transfers,VMEbus */
    tpb_to_proc.sink_attribute = 0x0001;     /* Going to a MCPL Node */
    tpb_to_proc.transfer_length = 7;         /* Eight Long words in a TPB */
    tpb_to_proc.completion_status = 0x0002;  /* Disable interrupt Mailbox A24/D16*/
    tpb_to_proc.nack_count = 0;              /* Do not retry */
    tpb_to_proc.link_address = (long)gmad_to_proc_pntr;
    /* link to gmad_to_proc TPB structure */
    tpb_to_proc.link_attribute = 0x3963; /* Normal VMEBus Addressing,VME link after
					    trigger, address modifier $39, disable
					    BLT, 256 Xfers/burst, disable
					    pipelining, link to VME */
    tpb_to_proc.mcpl_control_word = 0x0045;  /* Generic MCPL Node address,
						dispatch table entry #1, remain
						connected, dispatch table load,
						burst mode, compelled */
    tpb_to_proc.mailbox_address = (long)&mailbox;
    
    /* The gmad_to_proc TPB sends a MAD to an MCPL node as determined by 
       its dispatch table entry one.  All of the general purpose parmeters in this
       TPB will be set now and processor specific ones will be set later */
    
    gmad_to_proc.source_start_address = (long)gmad_pntr;
    /* Data to be retrieved from gmad structure */
    gmad_to_proc.sink_start_address = 0;      /* Going to an mcpl node so
						 no address is necessary */
    gmad_to_proc.source_attribute = 0x3963; /* Normal VMEBus Addressing, address
					       modifier $39, disable pipelining,
					       disable BLT,256 transfers per burst,
					       32 bit transfers,VMEbus */
    gmad_to_proc.sink_attribute = 0x0001;     /* Going to a MCPL Node */
    gmad_to_proc.transfer_length = (sizeof(struct MAD) + 4)/4;
    /* Size of MAD in Long words */
    gmad_to_proc.completion_status = 0x0002; /* Disable interrupt Mailbox A24/D16*/
    gmad_to_proc.nack_count = 0;             /* Do not retry */
    gmad_to_proc.link_address = (long)tpb_to_proc_pntr;
    /* link back to tpb_to_proc structure */ 
    gmad_to_proc.link_attribute = 0x3963; /* Normal VMEBus Addressing,VME link
					     after trigger, address modifier $39,
					     disable BLT, 256 Xfers/burst,
					     disable pipelining,link to VME */
    gmad_to_proc.mcpl_control_word = 0x004B; /* Generic MCPL Node address,
						dispatch table entry #1,DTX
						Dispatch Table execute,locked,
						noncompelled, remain connected */
    gmad_to_proc.mailbox_address = (long)&mailbox;
    
    /* The receive_gmad tpb loads data off of the MCPL and into VMEBus memory
       this TPB is loaded in dispatch table entry one, completion of transfer is
       not signaled */
    
    receive_gmad.source_start_address = 0; /* Data to be retrieved from mcpl */
    receive_gmad.sink_start_address = 0x0000000; /* Generic Start Address of the
						    MAD Memory */
    receive_gmad.source_attribute = 0x0001;     /* Comming From an MCPL Node */
    receive_gmad.sink_attribute = 0x0963;  /* Normal VMEBus Addressing, address
					      modifier $09,disable pipelining,
					      disable BLT, 256 transfers per burst,
					      32 bit transfers,VMEbus */
    receive_gmad.transfer_length = (sizeof(struct MAD) + 4)/4;
    /*  Long words in a MAD structure */
    receive_gmad.completion_status = 0;    /*  No completion Status */
    receive_gmad.nack_count = 0;           /* Do not retry */
    receive_gmad.link_address = 0;         /* Linking is not performed by target */
    receive_gmad.link_attribute = 0;       /* Linking is not performed by target */
    receive_gmad.mcpl_control_word = 0;    /* MCPL to VMEBus transfer only */
    receive_gmad.mailbox_address = 0;      /* No mail Box Being used */
    
    /***********************************************************************/
    /* Calculate the position and size of the data areas in all processors */
    /***********************************************************************/
    
    logical_length = sizeof(ray_i) + sizeof(platform_i) + 
      sizeof(field_parameter_data);
    
    for(i=0; i<this_radar.num_parameter_des; i++)
      {
	  for(j=0; j<cellspace.num_segments; j++)
	    {
		logical_length += param[i].binary_format * cellspace.num_cells[j];
	    }
      }
    number_procs = 0;
    for (proc=0; proc<3; proc++)
      {
	  if(number_of_mads[proc] != 0) number_procs++;
	  for (mad=0; mad<number_of_mads[proc]; mad++)
	    {
		i = floor((double)((logical_length + 0x400)/0x400));
		madinfo[proc][mad][SIZE_AREAS] = i * 0x400;
		if((madinfo[proc][mad][NUM_AREAS] = (madinfo[proc][mad][MAX_MEM]-0x1800)/
		    madinfo[proc][mad][SIZE_AREAS])>1000)
		  madinfo[proc][mad][NUM_AREAS] = 1000;
		
		printf("logical length = %d\n",logical_length);
		printf("area size = %ld\n",madinfo[proc][mad][SIZE_AREAS]);
		printf("max mem = %ld\n",madinfo[proc][mad][MAX_MEM]);
		printf("number of areas = %ld\n",madinfo[proc][mad][NUM_AREAS]);
		printf("MAD structure start address = %lx\n",gmad_pntr);
		
		/* fill the general MAD structure with the proper information */
		
		gmad.sync_word = 0x55555555;
		gmad.num_before_repeat = madinfo[proc][mad][NUM_AREAS];
		for(i=0; i<madinfo[proc][mad][NUM_AREAS]; i++)
		  {
		      gmad.mailboxes[i] = 0;
		      gmad.addresses[i] = madinfo[proc][mad][MAD_BASE] + 0x1800 +
			i * madinfo[proc][mad][SIZE_AREAS];
		  }
		
		/* Fill Generic holes in TPB's to send this MAD structure to the
		   proper processor */
		tpb_to_proc.mcpl_control_word = (tpb_to_proc.mcpl_control_word & 0x07FF)
		  | (proc_nodes[proc]<<11);
		gmad_to_proc.mcpl_control_word= (gmad_to_proc.mcpl_control_word & 0x07FF)
		  | (proc_nodes[proc]<<11);
		receive_gmad.sink_start_address = madinfo[proc][mad][MAD_BASE];
		
		/* Send the  receieve_gmad tpb to the proper processor */
		
		printf("triggering the mcpl node to transfer TPB to processor: %d\n"
		       ,proc);
		
		timeout = 0;
		mailbox = 0;
		*control_reg = TRIGGER;
		*start_reg = 0x0001;
		do
		  {
		      timeout++;
		  } while ((mailbox == 0) && (timeout < 3000));
		
		status = *status_reg;
		if(mailbox == 0x5FFF)
		  printf("TPB transfer to processor %d successful status is: %x\n",
			 proc,status);
		else
		  {
		      printf("TPB transfer unsuccessful proc: %d mailbox: %x status: %x\n",
			     proc,mailbox,status);
		      exit(2);
		  }
		
		/* Transfer MAD structure to remote node */
		
		printf("\nTransferring MAD: %d to processor: %d\n",mad,proc);
		
		timeout = 0;
		mailbox = 0;
		*control_reg = TRIGGER;
		*start_reg = 0x0001;
		do
		  {
		      timeout++;
		  } while ((mailbox == 0) && (timeout < 3000));
		
		status = *status_reg;
		if(timeout < 3000)
		  printf("Successful Xfer of MAD: %d to proc: %d status is: %x\n",
			 mad,proc,status);
		else
		  {
		      printf("Unsuccessful xfer MAD: %d, proc: %d mailbox: %x status: %x\n",
			     mad,proc,mailbox,status);
		      exit(5);
		  }
		
	    } /* For loop for each MAD */
      } /* For loop for each processor */
    
    /* Let the user select the various options */
    
    printf("\nEnter time to sleep between transfers of data in .01 seconds\n"); 
    scanf("%ld",&sleep_time);
    printf("Do you want sinusoidal(1), random(2) or ramp(3) data?\n");
    scanf("%ld",&i);

    switch(i)
      {
	case 1:   /* sinusoidal data */
	  
	  for(i=0; i<cellspace.num_cells[0]; i++)
	    {
		ray.data[i][0] = sin((double)i) * 10.;
		ray.data[i][1] = sin((double)i) * 40.;
		ray.data[i][2] = sin((double)i) * 100.;
	    }
	  break;
	  
	case 2:   /* random data */
	  seed = -3;
	  for(i=0; i<cellspace.num_cells[0]; i++)
	    {
		ray.data[i][0] = ran2(&seed) * 10.;
		ray.data[i][1] = ran2(&seed) * 40.;
		ray.data[i][2] = ran2(&seed) * 100.;
	    }
	  break;
	  
	case 3:   /* ramp data */
	  step =  1. / (float)cellspace.num_cells[0];
	  for(i=0; i<cellspace.num_cells[0]; i++)
	    {
		ray.data[i][0] = i * 10.;
		ray.data[i][1] = i * 40.;
		ray.data[i][2] = i * 100.;
	    }
	  break;
	  
	  default:
	  break;
      }

    printf("Forward (1) or aft (2) radar data?\n");
    scanf("%ld",&i);

    if (i == 1)
      ray.fielddata.radar_name[0] = 'F';
    else
      ray.fielddata.radar_name[0] = 'A';

    /*******************************************************************/
    /* Setup the generic TPBs that will do the actual transfer of data */
    /*******************************************************************/
    
    /* The tpb_to_proc TPB sends the receive_data_tpb to a processor's dispatch
       table entry zero.  All of the general purpose parmeters in the TPB will be
       set now and processor specific ones will be set later */
    
    tpb_to_proc.source_start_address = (long)receive_data_pntr;
    /* Data to be retrieved from
       receive_data structure */
    tpb_to_proc.sink_start_address = 0;           /* Going to an mcpl node so
						     no address is necessary */
    tpb_to_proc.source_attribute = 0x3963; /* Normal VMEBus Addressing, address
					      modifier $39, disable pipelining,
					      disable BLT, 256 transfers per burst,
					      32 bit transfers,VMEbus */
    tpb_to_proc.sink_attribute = 0x0001;     /* Going to a MCPL Node */
    tpb_to_proc.transfer_length = 7;         /* Eight Long words in a TPB */
    tpb_to_proc.completion_status = 0x0002;  /* Disable interrupt Mailbox A24/D16*/
    tpb_to_proc.nack_count = 0;              /* Do not retry */
    tpb_to_proc.link_address = 0;            /* Will be set later */
    tpb_to_proc.link_attribute = 0x3963; /* Normal VMEBus Addressing,VME link after
					    trigger, address modifier $39, disable
					    BLT, 256 Xfers/burst, disable
					    pipelining, link to VME */
    tpb_to_proc.mcpl_control_word = 0x0005;  /* Generic MCPL Node address,
						dispatch table entry #0, remain
						connected, dispatch table load,
						burst mode, compelled */
    tpb_to_proc.mailbox_address = (long)&mailbox;
    
    /* The bcast_tpb broadcasts the data to aall MCPL node as determined by 
       their dispatch table entries zero.  All of the general purpose parmeters
       in this TPB will be set now and processor specific ones will be set later */
    
    bcast_tpb.source_start_address = 0; /* Will be start of each data ray */
    bcast_tpb.sink_start_address = 0;      /* Going to an mcpl node so
					      no address is necessary */
    bcast_tpb.source_attribute = 0x3963; /* Normal VMEBus Addressing, address
					    modifier $39, disable pipelining,
					    disable BLT,256 transfers per burst,
					    32 bit transfers,VMEbus */
    bcast_tpb.sink_attribute = 0x0001;     /* Going to a MCPL Node */
    bcast_tpb.transfer_length = (logical_length + 4)/4;
    /* Size of a data ray in Long words */
    bcast_tpb.completion_status = 0x0002; /* Disable interrupt Mailbox A24/D16*/
    bcast_tpb.nack_count = 0;             /* Do not retry */
    bcast_tpb.link_address = (long)tpb_to_proc_pntr;
    /* link back to tpb_to_proc structure */ 
    bcast_tpb.link_attribute = 0x3963; /* Normal VMEBus Addressing,VME link
					  after trigger, address modifier $39,
					  disable BLT, 256 Xfers/burst,
					  disable pipelining,link to VME */
    bcast_tpb.mcpl_control_word = 0x003B; /* Broadcast MCPL Node address,
					     dispatch table entry #0,BCD
					     Broadcast data,locked,
					     noncompelled, remain connected */
    bcast_tpb.mailbox_address = 0;        /* will be current mailbox address */
    
    /* The receive_data tpb loads data off of the MCPL and into VMEBus memory
       this TPB is loaded in dispatch table entry one, completion of transfer is
       signaled in mailbox*/
    
    receive_data.source_start_address = 0; /* Data to be retrieved from mcpl */
    receive_data.sink_start_address = 0x0000000; /* Generic Start Address of the
						    data area in MAD Memory */
    receive_data.source_attribute = 0x0001;     /* Comming From an MCPL Node */
    receive_data.sink_attribute = 0x0963;  /* Normal VMEBus Addressing, address
					      modifier $09,disable pipelining,
					      disable BLT, 256 transfers per burst,
					      32 bit transfers,VMEbus */
    receive_data.transfer_length = (logical_length + 4)/4;
    /*  Long words in a data ray */
    receive_data.completion_status = 0x0006;
    /* A32/D16 mailbox completion Status */
    receive_data.nack_count = 0;           /* Do not retry */
    receive_data.link_address = 0;         /* Linking is not performed by target */
    receive_data.link_attribute = 0;       /* Linking is not performed by target */
    receive_data.mcpl_control_word = 0;    /* MCPL to VMEBus transfer only */
    receive_data.mailbox_address = 0;   /* Generic, mail Box address will change */
    
    /*************************************************************************/
    /***  All of above junk is part of initialization routine the following **/
    /***                   Should be placed in the real-time loop          ***/
    /*************************************************************************/
    
    do{
	proc_count = 0;
	for (proc=0; proc<3; proc++)
	  {
	      if(number_of_mads[proc] > 0)   /* Does the processor exist? */
		{
		    
		    
		    /* Fill Generic holes in TPB's to send a tpb structure to the
		       proper processor dispatch table entry zero */
		    
		    tpb_to_proc.mcpl_control_word = (tpb_to_proc.mcpl_control_word & 0x07FF)
		      | (proc_nodes[proc]<<11);
		    
		    /* Have to send all processors a tpb then only one broadcast of data */
		    tpb_to_proc.link_address = (long)tpb_to_proc_pntr;
		    if(++proc_count >= number_procs)
		      tpb_to_proc.link_address = (long)bcast_tpb_pntr;
		    
		    /* Make remote nodes stick data at proper place */
		    
		    receive_data.completion_status =
		      madinfo[proc][current_mad[proc]][COMPLETE_S];
		    
		    receive_data.sink_start_address = 
		      madinfo[proc][current_mad[proc]][MAD_BASE] + 0x1800 +
			madinfo[proc][current_mad[proc]][SIZE_AREAS] * 
			  madinfo[proc][current_mad[proc]][NUM_C_AREA];
		    receive_data.mailbox_address =
		      madinfo[proc][current_mad[proc]][MAD_BASE] + 8 +
			2 * madinfo[proc][current_mad[proc]][NUM_C_AREA];
		    
		    if(++madinfo[proc][current_mad[proc]][NUM_C_AREA] >=
		       madinfo[proc][current_mad[proc]][NUM_AREAS])
		      {
			  madinfo[proc][current_mad[proc]][NUM_C_AREA] = 0;
			  if(++current_mad[proc] >= number_of_mads[proc])
			    current_mad[proc] = 0;
		      }
		    
		    /* Send the receieve_data tpb to the proper processor */
		    
		    timeout = 0;
		    mailbox = 0;
		    *control_reg = TRIGGER;
		    *start_reg = 0x0001;
		    do
		      {
			  timeout++;
		      } while ((mailbox == 0) && (timeout < 3000));
		    
		    status = *status_reg;
		    if(mailbox != 0x5FFF)
		      {
			  printf("TPB transfer unsuccessful proc: %d mailbox: %x status: %x\n",
				 proc,mailbox,status);
			  exit(2);
		      }
		} /* Test to see if processor exists */
	  } /* for loop to get a receive_data tpb out to each processor */
	
	
	/* Transfer data ray structure to all remote nodes */
	
	bcast_tpb.source_start_address = current_ray_add;
	bcast_tpb.mailbox_address = current_mailbox_add;
	
	timeout = 0;
	mailbox_test = (short *)current_mailbox_add;
	*mailbox_test = 0;
	*control_reg = TRIGGER;
	*start_reg = 0x0001;
	do
	  {
	      timeout++;
	  } while ((*mailbox_test == 0) && (timeout < 3000));
	
	status = *status_reg;
	mailbox = *mailbox_test;
	if(timeout >= 3000)
	  {
	      printf("Unsuccessful data xfer mailbox: %x status: %x\n",
		     mailbox,status);
	      exit(5);
	  }
	
	++ray.fielddata.ray_count;
	if(++ray.pltfrminfo.rotation_angle > 360.)
	  ray.pltfrminfo.rotation_angle = 0;
	taskDelay(sleep_time);
	
    }while(kill == 0);
    
}
#define M 714025
#define IA 1366
#define IC 150889

float ran2(idum)
long *idum;
{
    static long iy,ir[98];
    static int iff=0;
    int j;
    
    if (*idum < 0 || iff == 0) {
	iff=1;
	if ((*idum=(IC-(*idum)) % M) < 0) *idum = -(*idum);
	for (j=1;j<=97;j++) {
	    *idum=(IA*(*idum)+IC) % M;
	    ir[j]=(*idum);
	}
	*idum=(IA*(*idum)+IC) % M;
	iy=(*idum);
    }
    j=1 + 97.0*iy/M;
    if (j > 97 || j < 1) printf("RAN2: This cannot happen.");
    iy=ir[j];
    *idum=(IA*(*idum)+IC) % M;
    ir[j]=(*idum);
    return (float) iy/M;
}
#undef M
#undef IA
#undef IC
