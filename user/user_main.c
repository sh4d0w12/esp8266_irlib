#include "ets_sys.h"
#include "osapi.h"
#include "gpio.h"
#include "os_type.h"
#include "user_config.h"
#include "stdout.h"

#define user_procTaskPrio        0
#define user_procTaskQueueLen    1
os_event_t    user_procTaskQueue[user_procTaskQueueLen];
static void user_procTask(os_event_t *events);

static volatile os_timer_t some_timer;


void some_timerfunc(void *arg)
{
	if (GPIO_INPUT_GET(12) == 0){
		os_printf("\nHowdie ho\n");
	}
	int config = GPIO_INPUT_GET(12);
	char buffer[30];
	os_sprintf(buffer,"\nconfig: %d\n",config);
	os_printf(buffer);
    //Do blinky stuff
    if (GPIO_REG_READ(GPIO_OUT_ADDRESS) & BIT2)
    {
        //Set GPIO2 to LOW
//        gpio_output_set(0, BIT2, BIT2, 0);
    	GPIO_OUTPUT_SET (2, 0);
        os_printf("\nLow\n");
    }
    else
    {
        //Set GPIO2 to HIGH
//        gpio_output_set(BIT2, 0, BIT2, 0);
        GPIO_OUTPUT_SET (2, 1);
        os_printf("\nHigh\n");
    }
}


////Do nothing function
//static void ICACHE_FLASH_ATTR
//user_procTask(os_event_t *events)
//{
//    os_delay_us(10);
////	read_ir();
//}


//Do nothing function
static void ICACHE_FLASH_ATTR
user_procTask(os_event_t *events)
{
	os_printf("\nReady for IR\n");
	int wait_count=200000; // 2 Seconds (200000 * 10 us)
//	GPIO_OUTPUT_SET (2, 1);
	while(GPIO_INPUT_GET(12) == 0){
		os_delay_us(10);
		if (wait_count<=0){
			break;
		}
		wait_count--;
	}

	if (wait_count>0){
		os_printf("\nIR reading\n");
//		GPIO_OUTPUT_SET (2, 0);
		wait_count=200000; // 2 Seconds
		int time_duration=0;
		int previous_state=1;
		int current_state;
		while(wait_count>0){
			current_state=GPIO_INPUT_GET(12);
			if (current_state!=previous_state){
				os_printf("duration: %d\n", 200000-wait_count);
				previous_state=current_state;
				wait_count=200000;
			}
			wait_count--;
		}
		os_printf("\nIR read\n");
	}
	system_os_post(user_procTaskPrio, 0, 0 );

}

//Init function 
void ICACHE_FLASH_ATTR
user_init()
{
	stdoutInit();
	os_printf("\nSetting up\n");
    // Initialize the GPIO subsystem.
    gpio_init();

    //Set GPIO2 to output mode
    PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO2_U, FUNC_GPIO2);
    //PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO12_U, FUNC_GPIO12);
//    PIN_PULLUP_EN(PERIPHS_IO_MUX_GPIO12_U);
    PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTDI_U,FUNC_GPIO12);
//    GPIO_OUTPUT_SET (12, 0);
    PIN_PULLDWN_DIS(PERIPHS_IO_MUX_MTDI_U);
    //Set GPIO2 low
//    gpio_output_set(0, BIT2, BIT2, 0);
    GPIO_OUTPUT_SET (2, 0);

    //Disarm timer
//    os_timer_disarm(&some_timer);

//    Setup timer
//    os_timer_setfn(&some_timer, (os_timer_func_t *)some_timerfunc, NULL);

    //Arm the timer
    //&some_timer is the pointer
    //1000 is the fire time in ms
    //0 for once and 1 for repeating
//    os_timer_arm(&some_timer, 100, 1);
    
    os_printf("\nReady GO GO\n");


    //Start os task
    system_os_task(user_procTask, user_procTaskPrio,user_procTaskQueue, user_procTaskQueueLen);
    system_os_post(user_procTaskPrio, 0, 0 );
}
