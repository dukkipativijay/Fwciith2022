#include "Fw_global_config.h"   // This defines application specific charactersitics

#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "timers.h"
#include "RtosTask.h"

/*    Include the generic headers required for QORC */
#include "eoss3_hal_gpio.h"
#include "eoss3_hal_rtc.h"
#include "eoss3_hal_timer.h"
#include "eoss3_hal_fpga_usbserial.h"
#include "ql_time.h"
#include "s3x_clock_hal.h"
#include "s3x_clock.h"
#include "s3x_pi.h"
#include "dbg_uart.h"
#include "cli.h"


extern const struct cli_cmd_entry my_main_menu[];


const char *SOFTWARE_VERSION_STR;

extern void qf_hardwareSetup();
static void nvic_init(void);
#define GPIO_OUTPUT_MODE (1)
#define GPIO_INPUT_MODE (0)
void PyHal_GPIO_SetDir(uint8_t gpionum,uint8_t iomode);
int PyHal_GPIO_GetDir(uint8_t gpionum);
int PyHal_GPIO_Set(uint8_t gpionum, uint8_t gpioval);
int PyHal_GPIO_Get(uint8_t gpionum);
#define	CLK_PIN	18
void disp(int num);

int main(void)
{
    uint32_t int Q1=0,Q2=0,Q3=0,Q4=0,Q44=1;
uint32_t int D1,D2,D3,D4;
     
    SOFTWARE_VERSION_STR = "qorc-onion-apps/qf_hello-fpga-gpio-ctlr";
    
    qf_hardwareSetup();
    nvic_init();

    dbg_str("\n\n");
    dbg_str( "##########################\n");
    dbg_str( "Quicklogic QuickFeather FPGA GPIO CONTROLLER EXAMPLE\n");
    dbg_str( "SW Version: ");
    dbg_str( SOFTWARE_VERSION_STR );
    dbg_str( "\n" );
    dbg_str( __DATE__ " " __TIME__ "\n" );
    dbg_str( "##########################\n\n");

    dbg_str( "\n\nHello GPIO!!\n\n");	// <<<<<<<<<<<<<<<<<<<<<  Change me!

    CLI_start_task( my_main_menu );
	HAL_Delay_Init();

	
/***************START FLASH IMPL***********************/
PyHal_GPIO_SetDir(2,0); //Input
PyHal_GPIO_SetDir(4,0); //Input
PyHal_GPIO_SetDir(6,0); //Input
PyHal_GPIO_SetDir(6,0); //Input
PyHal_GPIO_SetDir(22,1); //Output
PyHal_GPIO_SetDir(20,1); //Output
PyHal_GPIO_SetDir(18,1); //Output
PyHal_GPIO_SetDir(16,1); //Output
while(1) {

 PyHal_GPIO_Set(CLK_PIN, 0);
    HAL_DelayUSec(2000000);    
    PyHal_GPIO_Set(CLK_PIN, 1);
    
    sevenseg_setup();    //Sevenseg ready for display
    sevenseg(a,b,c,d,e,f,g); 
     HAL_DelayUSec(2000000);    
     
       Q1 = PyHal_GPIO_Get(2);
    Q2 = PyHal_GPIO_Get(4);
    Q3= PyHal_GPIO_Get(6)
    Q4= PyHal_GPIO_Get(8)
    PyHal_GPIO_Set(22,F);
    
    disp(Q4,Q3,Q2,Q1);
    //clk1();
    PyHal_GPIO_Set(CLK_PIN, 1);
     D1=Q44;
     Q1=D1;
    disp(Q4,Q3,Q2,Q1);
   // clk1();
   PyHal_GPIO_Set(CLK_PIN, 1);
   D2=Q1;
   Q2=D2;
    disp(Q4,Q3,Q2,Q1);
    //clk1();
    PyHal_GPIO_Set(CLK_PIN, 1);  
    D3=Q2;
    Q3=D3;
    disp(Q4,Q3,Q2,Q1);
   //clk1();
   PyHal_GPIO_Set(CLK_PIN, 1);
   D4=Q3;
   Q4=D4;
   Q44=!Q4;  

   disp(Q4,Q3,Q2,Q1);
      //clk1();
PyHal_GPIO_Set(CLK_PIN, 1);
}
/***********************END flash IMPL********************************/

    /* Start the tasks and timer running. */
    vTaskStartScheduler();
    dbg_str("\n");

while(1);
}

static void nvic_init(void)
 {
    // To initialize system, this interrupt should be triggered at main.
    // So, we will set its priority just before calling vTaskStartScheduler(), not the time of enabling each irq.
    NVIC_SetPriority(Ffe0_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
    NVIC_SetPriority(SpiMs_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
    NVIC_SetPriority(CfgDma_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
    NVIC_SetPriority(Uart_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
    NVIC_SetPriority(FbMsg_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
 }    


//needed for startup_EOSS3b.s asm file
void SystemInit(void)
{

}

//gpionum --> 0 --> 31 corresponding to the IO PADs
//gpioval --> 0 or 1
#define FGPIO_DIRECTION_REG (0x40024008)
#define FGPIO_OUTPUT_REG (0x40024004)
#define FGPIO_INPUT_REG (0x40024000)
//Set GPIO(=gpionum) Mode: Input(iomode = 0) or Output(iomode = 1)
//Before Set/et GPIO value, the direction must be correctly set
void PyHal_GPIO_SetDir(uint8_t gpionum,uint8_t iomode)
{
    uint32_t tempscratch32;

    if (gpionum > 31)
        return;

    tempscratch32 = *(uint32_t*)(FGPIO_DIRECTION_REG);
    if (iomode)
        *(uint32_t*)(FGPIO_DIRECTION_REG) = tempscratch32 | (0x1 << gpionum);
    else
        *(uint32_t*)(FGPIO_DIRECTION_REG) = tempscratch32 & (~(0x1 << gpionum));

}


//Get current GPIO(=gpionum) Mode: Input(iomode = 0) or Output(iomode = 1)
int PyHal_GPIO_GetDir(uint8_t gpionum)
{
    uint32_t tempscratch32;
    int result = 0;

    if (gpionum > 31)
        return -1;

    tempscratch32 = *(uint32_t*)(FGPIO_DIRECTION_REG);

    result = ((tempscratch32 & (0x1 << gpionum)) ? GPIO_OUTPUT_MODE : GPIO_INPUT_MODE);

    return result;
}

//Set GPIO(=gpionum) to 0 or 1 (= gpioval)
//The direction must be set as Output for this GPIO already
//Return value = 0, success OR -1 if error.
int PyHal_GPIO_Set(uint8_t gpionum, uint8_t gpioval)
{
    uint32_t tempscratch32;

    if (gpionum > 31)
        return -1;

    tempscratch32 = *(uint32_t*)(FGPIO_DIRECTION_REG);

    //Setting Direction moved out as separate API, we will only check
    //*(uint32_t*)(FGPIO_DIRECTION_REG) = tempscratch32 | (0x1 << gpionum);
    if (!(tempscratch32 & (0x1 << gpionum)))
    {
        //Direction not Set to Output
        return -1;
    }
    
    tempscratch32 = *(uint32_t*)(FGPIO_OUTPUT_REG);

    if(gpioval > 0)
    {
        *(uint32_t*)(FGPIO_OUTPUT_REG) = tempscratch32 | (0x1 << gpionum);
    }
    else
    {
        *(uint32_t*)(FGPIO_OUTPUT_REG) = tempscratch32 & ~(0x1 << gpionum);
    }    

    return 0;
}
//Get GPIO(=gpionum): 0 or 1 returned (or in erros -1)
//The direction must be set as Input for this GPIO already
int PyHal_GPIO_Get(uint8_t gpionum)
{
    uint32_t tempscratch32;
    uint32_t gpioval_input;

    if (gpionum > 31)
        return -1;

    tempscratch32 = *(uint32_t*)(FGPIO_INPUT_REG);
    gpioval_input = (tempscratch32 >> gpionum) & 0x1;

    return ((int)gpioval_input);
}