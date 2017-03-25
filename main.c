/* ###################################################################
**     Filename    : main.c
**     Project     : Dlycore
**     Processor   : MK20DX256VLL7
**     Version     : Driver 01.01
**     Compiler    : GNU C Compiler
**     Date/Time   : 2017-03-23, 11:35, # CodeGen: 0
**     Abstract    :
**         Main module.
**         This module contains user's application code.
**     Settings    :
**     Contents    :
**         No public methods
**
** ###################################################################*/
/*!
** @file main.c
** @version 01.01
** @brief
**         Main module.
**         This module contains user's application code.
*/         
/*!
**  @addtogroup main_module main module documentation
**  @{
*/         
/* MODULE main */


/* Including needed modules to compile this module/procedure */
#include "Cpu.h"
#include "Events.h"
#include "WAIT1.h"
#include "MCUC1.h"
/* Including shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
/* User includes (#include below this line is not maintained by Processor Expert) */
//------LED1-------------------------------------
#define  LED01_LOC                     (1<<3)      //PTE3
#define  LED01_ON          GPIOE_PSOR = LED01_LOC;
#define  LED01_OFF         GPIOE_PCOR = LED01_LOC;
#define  LED01_NEG         GPIOE_PTOR = LED01_LOC;
#define  LED01_INIT        PORTE_PCR3   = PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK;  GPIOE_PDDR    |= LED01_LOC; // LED01

//------LED2-------------------------------------
#define  LED02_LOC                     (1<<24)      //PTE24
#define  LED02_ON          GPIOE_PSOR = LED02_LOC;
#define  LED02_OFF         GPIOE_PCOR = LED02_LOC;
#define  LED02_NEG         GPIOE_PTOR = LED02_LOC;
#define  LED02_INIT        PORTE_PCR24   = PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK;  GPIOE_PDDR    |= LED02_LOC;  // LED02

uint32 ret_lr,lr_a,lr_b,r7_a,r7_b,r7_r7; //Save LR + R7
uint32 dl_a,dl_b, taskb,dly; // Delay value
//--------------------------------------------
#define _ta (uint32*)(&_task_a)  // pointer to jumper init task
void DelayCore ( int ,int );     // core delay all Task
void _task_a (void);
void _task_b (void);
//----------------------------------------------
/*lint -save  -e970 Disable MISRA rule (6.3) checking. */
int main(void)
/*lint -restore Enable MISRA rule (6.3) checking. */
{
  /* Write your local variable definition here */
int i=0;
  /*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
  PE_low_level_init();
  /*** End of Processor Expert internal initialization.                    ***/
     SIM_SCGC5   |= ( SIM_SCGC5_PORTE_MASK); // init clock portE
     LED01_INIT;
     LED02_INIT;
       lr_a=0;  lr_b=0; dl_a=0; dl_b=0; taskb =0;
         DelayCore ( 1,200);
         taskb = 0x55;  // init task_A


     for(;;) {               //main task

    	 DelayCore ( 1,400);
    	 LED01_NEG;


  }

  /*** Don't write any code pass this line, or it will be deleted during code generation. ***/
  /*** RTOS startup code. Macro PEX_RTOS_START is defined by the RTOS component. DON'T MODIFY THIS CODE!!! ***/
  #ifdef PEX_RTOS_START
    PEX_RTOS_START();                  /* Startup of the selected RTOS. Macro is defined by the RTOS component. */
  #endif
  /*** End of RTOS startup code.  ***/
  /*** Processor Expert end of main routine. DON'T MODIFY THIS CODE!!! ***/
  for(;;){}
  /*** Processor Expert end of main routine. DON'T WRITE CODE BELOW!!! ***/
} /*** End of main routine. DO NOT MODIFY THIS TEXT!!! ***/

//------------------------
 void DelayCore ( int a,int b){
///	register int ret_lr __asm ("lr");


__asm ("mov %0, lr" : "=r" (ret_lr));   //Save LR
__asm ("mov %0, r7" : "=r" (r7_r7));    //Save R7

  if( a ==1 ){
                 lr_a = ret_lr;     // save lr
                 r7_a = r7_r7;
                 dl_a = b;           // delay

                if( taskb == 0x55){
                	taskb++;
                        __asm("bx %0"::"r" (_ta )); // Jumper task_a init
                }

    }else{
  	             lr_b = ret_lr; 	 // save lr
  	             r7_b = r7_r7;
  	             dl_b = b;           // delay
              }

  for( ;;){
                     WAIT1_Waitms(1);  // base 1ms

	                    if( dl_a ){

		               if( -- dl_a == 0){ // value delay Task_A



		            	   __asm("  adds	r7, r7, #12 \n\t"
		            	         "	mov	sp, r7 \n\t"
		            	  		 " ldr	r7, [sp], #4 \n\t" );       //" bx	lr  "
       	                    __asm("mov r7,%0":: "r" (r7_a )	);
		            	    __asm("bx %0":: "r" (lr_a ));          // return to Task_A

		  }

	  }//

	             if( dl_b ){

	  		  if( -- dl_b == 0){    // value delay Task_B

	  			  __asm( "  adds	r7, r7, #12 \n\t"
	                     "	mov	sp, r7 \n\t"
	  	            	 " ldr	r7, [sp], #4 \n\t" );           //" bx	lr  "

	  		     __asm("mov r7,%0":: "r" (r7_b )	);
	     	    __asm("bx %0"::  "r" (lr_b ));             // return to Task_B


	  		  }

	  	  }// others task...

  }

}
//--------------------------------------
 void _task_a (void){

	 for(;;){

	 DelayCore ( 2,50);
	 LED02_NEG;
	 DelayCore ( 2,50);
	 _task_a ();

	 }
 }
 //-------------------------------------
  void _task_b (void){

	  LED01_NEG;
	  LED02_NEG;
 		 DelayCore ( 2,10);
 	 LED01_NEG;
 	 LED02_NEG;
  }
  //-----------------------------------
