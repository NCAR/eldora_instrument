
static char FN[] = __FILE__;

#include <stdLoc.h>
#include <stdlib.h>
#include <tickLib.h>
#include <sysLib.h>
#include <vme.h>


#include "ELDRP7.h"
#include "ELDORA_RP7.hh"
#include "QuadDSP32C.hh"
#include "CollDSP32C.hh"

int bit_flag = 0;

extern "C" int rp7_menu();

int rp7_menu()
{
    int choice, choice_1, choice_2, status;
    
    ELDORA_RP7 *rp7_pntr = new ELDORA_RP7();

    while(1)
        {
            printf("\n");
            printf("\n");
            printf("ELDORA RP7 DIAGNOSTICS MENU \n");
            printf("\n");

            printf("1) Memory Tests \n");
            printf("2) Data Path Tests \n");
            printf("3) Perform Full Diagnostic \n");
            printf("4) Exit Diagnostics Menu \n");

            scanf(" %d",&choice);
            switch(choice)
                {
                  case 1:
                      do
                      {
                          printf("\n");
                          printf("\n");
                          printf("MEMORY TEST MENU \n");
                          printf("\n");
                          printf("1) Test Pulse Pair Processors \n");
                          printf("2) Test Doppler Processors \n");
                          printf("3) Test Collator \n");
                          printf("4) Perform Full Memory Test \n");
                          printf("5) Return to Main Menu \n");
                          scanf(" %d",&choice_1);
                          switch(choice_1)
                              {
                                case 1:
                                    do
                                    {
                                        printf("\n");
                                        printf("\n");
                                        printf("1) Test Freq 1 PPP \n");
                                        printf("2) Test Freq 2 PPP \n");
                                        printf("3) Test Freq 3 PPP \n");
                                        printf("4) Test Freq 4 PPP \n");
                                        printf("5) Test Freq 5 PPP \n");
                                        printf("6) Test all PPPs \n");
                                        printf("7) Return to Memory Test Menu \n");
                                        scanf(" %d",&choice_2);
                                        switch(choice_2)
                                            {
                                              case 1:
                                                  rp7_pntr->ppp_tst(1);
                                                  break;
                                              case 2:
                                                  rp7_pntr->ppp_tst(2);
                                                  break;
                                              case 3:
                                                  rp7_pntr->ppp_tst(3);
                                                  break;
                                              case 4:
                                                  rp7_pntr->ppp_tst(4);
                                                  break;
                                              case 5:
                                                  rp7_pntr->ppp_tst(5);
                                                  break;
                                              case 6:
                                                  rp7_pntr->ppp_tst(1);
                                                  rp7_pntr->ppp_tst(2);
                                                  rp7_pntr->ppp_tst(3);
                                                  rp7_pntr->ppp_tst(4);
                                                  rp7_pntr->ppp_tst(5);
                                                  break;
                                              case 7:
                                                  break;

                                            }
                                       
                                    }while(choice_2 != 7);
                                    break;
                                case 2:
                                    do
                                    {
                                        printf("\n");
                                        printf("\n");
                                        printf("1) Test DP #1 \n");
                                        printf("2) Test DP #2 \n");
                                        printf("3) Test Both DPs \n");
                                        printf("4) Return to Memory Test Menu \n");
                                        scanf(" %d",&choice_2);
                                        switch(choice_2)
                                            {
                                              case 1:
                                                  rp7_pntr->dp_tst(1);
                                                  break;
                                              case 2:
                                                  rp7_pntr->dp_tst(2);
                                                  break;
                                              case 3:
                                                  rp7_pntr->dp_tst(1);
                                                  rp7_pntr->dp_tst(2);
                                                  break;
                                              case 4:
                                                  break;

                                            }
                               
                                    }while(choice_2 != 4);
                                    break;
                                case 3:
                                    rp7_pntr->coll_tst();
                                    break;
                                case 4:
                                    rp7_pntr->ppp_tst(1);
                                    rp7_pntr->ppp_tst(2);
                                    rp7_pntr->ppp_tst(3);
                                    rp7_pntr->ppp_tst(4);
                                    rp7_pntr->ppp_tst(5);
                                    rp7_pntr->dp_tst(1);
                                    rp7_pntr->dp_tst(2);
                                    rp7_pntr->coll_tst();
                                    break;
                                case 5:
                                    break;
                              }
                      }while(choice_1 != 5);
                      break;
                  case 2:
                      do
                      {
                          printf("\n");
                          printf("\n");
                          printf("DATA PATH TEST MENU \n");
                          printf("\n");
                          printf("1) Test DIGIFs to PPPs \n");
                          printf("2) Test PPPs to DPs \n");
                          printf("3) Test DPs to Collator \n");
                          printf("4) Return to Main Menu \n");
                          scanf(" %d",&choice_1);
                          switch(choice_1)
                              {
                                case 1:
                                    do
                                        {
                                             printf("\n");
                                             printf("\n");
                                             printf("1) Test DIGIF to Freq 1 PPP I/F \n");
                                             printf("2) Test DIGIF to Freq 2 PPP I/F \n");
                                             printf("3) Test DIGIF to Freq 3 PPP I/F \n");
                                             printf("4) Test DIGIF to Freq 4 PPP I/F \n");
                                             printf("5) Test DIGIF to Freq 5 PPP I/F \n");
                                             printf("6) Test All DIGIF to PPP I/Fs \n");
                                             printf("7) Return to DATA PATH TEST MENU \n");
                                           scanf(" %d",&choice_2);
                                           switch(choice_2)
                                               {
                                                 case 1:
                                                     status=rp7_pntr->digif2ppp_tst(1);
                                                     if(!status)
                                                       printf("DIGIF to Freq 1 PPP I/F Test Passed \n");
                                                     break;
                                                 case 2:
                                                     status=rp7_pntr->digif2ppp_tst(2);
                                                     if(!status)
                                                       printf("DIGIF to Freq 2 PPP I/F Test Passed \n");
                                                     break;
                                                 case 3:
                                                     status=rp7_pntr->digif2ppp_tst(3);
                                                     if(!status)
                                                       printf("DIGIF to Freq 3 PPP I/F Test Passed \n");
                                                     break;
                                                 case 4:
                                                     status=rp7_pntr->digif2ppp_tst(4);
                                                     if(!status)
                                                       printf("DIGIF to Freq 4 PPP I/F Test Passed \n");
                                                     break;
                                                 case 5:
                                                     status=rp7_pntr->digif2ppp_tst(5);
                                                     if(!status)
                                                       printf("DIGIF to Freq 5 PPP I/F Test Passed \n");
                                                     break;
                                                 case 6:
                                                     status=rp7_pntr->digif2ppp_tst(1);
                                                     if(!status)
                                                       printf("DIGIF to Freq 1 PPP I/F Test Passed \n");
                                                     status=rp7_pntr->digif2ppp_tst(2);
                                                     if(!status)
                                                       printf("DIGIF to Freq 2 PPP I/F Test Passed \n");
                                                     status=rp7_pntr->digif2ppp_tst(3);
                                                     if(!status)
                                                       printf("DIGIF to Freq 3 PPP I/F Test Passed \n");
                                                     status=rp7_pntr->digif2ppp_tst(4);
                                                     if(!status)
                                                       printf("DIGIF to Freq 4 PPP I/F Test Passed \n");
                                                     status=rp7_pntr->digif2ppp_tst(5);
                                                     if(!status)
                                                       printf("DIGIF to Freq 5 PPP I/F Test Passed \n");
                                                     break;
                                                 case 7:
                                                     break;
                                               }
                                         }while(choice_2 != 7);   
                                         break;
                                     case 2:
                                         do
                                         {
                                             printf("\n");
                                             printf("\n");
                                             printf("1) Test Freq 1 PPP to DP I/F \n");
                                             printf("2) Test Freq 2 PPP to DP I/F \n");
                                             printf("3) Test Freq 3 PPP to DP I/F \n");
                                             printf("4) Test Freq 4 PPP to DP I/F \n");
                                             printf("5) Test Freq 5 PPP to DP I/F \n");
                                             printf("6) Test Full PPP to DP I/F \n");
                                             printf("7) Return to DATA PATH TEST MENU \n");
                                             scanf(" %d",&choice_2);
                                             switch(choice_2)
                                                 {
                                                   case 1:
                                                       status=rp7_pntr->ppp2dp_tst(1);
                                                       if(!status)
                                                         printf("Freq 1 PPP to DP I/F Test Passed \n");
                                                       break;
                                                   case 2:
                                                       status=rp7_pntr->ppp2dp_tst(2);
                                                       if(!status)
                                                         printf("Freq 2 PPP to DP I/F Test Passed \n");
                                                       break;
                                                   case 3:
                                                       status=rp7_pntr->ppp2dp_tst(3);
                                                       if(!status)
                                                         printf("Freq 3 PPP to DP I/F Test Passed \n");
                                                       break;
                                                   case 4:
                                                       status=rp7_pntr->ppp2dp_tst(4);
                                                       if(!status)
                                                         printf("Freq 4 PPP to DP I/F Test Passed \n");
                                                       break;
                                                   case 5:
                                                       status=rp7_pntr->ppp2dp_tst(5);
                                                       if(!status)
                                                         printf("Freq 5 PPP to DP I/F Test Passed \n");
                                                     break;
                                                   case 6:
                                                       status=rp7_pntr->ppp2dp_tst(1);
                                                       if(!status)
                                                         printf("Freq 1 PPP to DP I/F Test Passed \n");
                                                       status=rp7_pntr->ppp2dp_tst(2);
                                                       if(!status)
                                                         printf("Freq 2 PPP to DP I/F Test Passed \n");

                                                       status=rp7_pntr->ppp2dp_tst(3);
                                                       if(!status)
                                                         printf("Freq 3 PPP to DP I/F Test Passed \n");
                                                       status=rp7_pntr->ppp2dp_tst(4);
                                                       if(!status)
                                                         printf("Freq 4 PPP to DP I/F Test Passed \n");
                                                       status=rp7_pntr->ppp2dp_tst(5);
                                                       if(!status)
                                                         printf("Freq 5 PPP to DP I/F Test Passed \n");
                                                       break;
                                                   case 7:
                                                       break;
                                                 }
                                             
                                         }while(choice_2 != 7);
                                         break;
                                     case 3:
                                         do
                                         {
                                             printf("\n");
                                             printf("\n");
                                             printf("1) Test DP #1 to Collator I/F \n");
                                             printf("2) Test DP #2 to Collator I/F \n");
                                             printf("3) Test Full DP to Collator I/F \n");
                                             printf("4) Return to DATA PATH TEST MENU \n");
                                             scanf(" %d",&choice_2);
                                             switch(choice_2)
                                                 {
                                                   case 1:
                                                       status=rp7_pntr->dp2coll_tst(1);
                                                       if(!status)
                                                         printf("DP #1 to Collator I/F Test Passed \n");
                                                       break;
                                                   case 2:
                                                       status=rp7_pntr->dp2coll_tst(2);
                                                       if(!status)
                                                         printf("DP #2 to Collator I/F Test Passed \n");
                                                       break;
                                                   case 3:
                                                       status=rp7_pntr->dp2coll_tst(1);
                                                       if(!status)
                                                         printf("DP #1 to Collator I/F Test Passed \n");
                                                       status=rp7_pntr->dp2coll_tst(2);
                                                       if(!status)
                                                         printf("DP #2 to Collator I/F Test Passed \n");
                                                       break;
                                                   case 4:
                                                       break;
                                                 }
                                             
                                         }while(choice_2 != 4);
                                         break;
                                     case 4:
                                         break;

                                   }
                           }while(choice_1 != 4);
                      break;
                               
                  case 3:
                      // test Pulse Pair Processors
                      rp7_pntr->ppp_tst(1);
                      rp7_pntr->ppp_tst(2);
                      rp7_pntr->ppp_tst(3);
                      rp7_pntr->ppp_tst(4);
                      rp7_pntr->ppp_tst(5);
                      // test Doppler Processors
                      rp7_pntr->dp_tst(1);
                      rp7_pntr->dp_tst(2);
                      // test Collator
                      rp7_pntr->coll_tst();
                      // test DIGIF to PPP Interface
                      status=rp7_pntr->digif2ppp_tst(1);
                      if(!status)
                        printf("DIGIF to Freq 1 PPP I/F Test Passed \n");
                      status=rp7_pntr->digif2ppp_tst(2);
                      if(!status)
                        printf("DIGIF to Freq 2 PPP I/F Test Passed \n");
                      status=rp7_pntr->digif2ppp_tst(3);
                      if(!status)
                        printf("DIGIF to Freq 3 PPP I/F Test Passed \n");
                      status=rp7_pntr->digif2ppp_tst(4);
                      if(!status)
                        printf("DIGIF to Freq 4 PPP I/F Test Passed \n");
                      status=rp7_pntr->digif2ppp_tst(5);
                      if(!status)
                        printf("DIGIF to Freq 5 PPP I/F Test Passed \n");
                      // test PPP to DP Interface
                      status=rp7_pntr->ppp2dp_tst(1);
                      if(!status)
                        printf("Freq 1 PP to DP I/F Test Passed \n");
                      status=rp7_pntr->ppp2dp_tst(2);
                      if(!status)
                        printf("Freq 2 PP to DP I/F Test Passed \n");
                      status=rp7_pntr->ppp2dp_tst(3);
                      if(!status)
                        printf("Freq 3 PP to DP I/F Test Passed \n");
                      status=rp7_pntr->ppp2dp_tst(4);
                      if(!status)
                        printf("Freq 4 PP to DP I/F Test Passed \n");
                      status=rp7_pntr->ppp2dp_tst(5);
                      if(!status)
                        printf("Freq 5 PP to DP I/F Test Passed \n");
                      // test DP to Collator Interface
                      status=rp7_pntr->dp2coll_tst(1);
                      if(!status)
                        printf("DP #1 to Collator I/F Test Passed \n");
                      status=rp7_pntr->dp2coll_tst(2);
                      if(!status)
                        printf("DP #2 to Collator I/F Test Passed \n");
                      break;
                  case 4:
                      return(-1);
                      break;
                }
        }
}
