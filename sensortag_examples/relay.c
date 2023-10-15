/* XDCtools Header files */
#include <xdc/std.h>
#include <xdc/runtime/System.h>

/* BIOS Header files */
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/drivers/PIN.h>

/* Board Header files */
#include "Board.h"

#include <stdio.h>

#define TASKSTACKSIZE 512
Char taskStack[TASKSTACKSIZE];

/* Pin */
static PIN_Handle hRelay;
static PIN_State bRelay;
PIN_Config cRelay[] = {
    Board_RELAY | PIN_INPUT_EN | PIN_PULLDOWN,
    PIN_TERMINATE
};

Void relayFxn(UArg arg0, UArg arg1) {

	char str[32];

	while (1) {

		sprintf(str,"Relay: %d\n",PIN_getInputValue(Board_RELAY));
        System_printf(str);
        System_flush();

    	Task_sleep(1000000 / Clock_tickPeriod);
    }
}

int main(void) {

	Task_Params taskParams;
	Task_Handle taskHandle;

    Board_initGeneral();

	hRelay = PIN_open(&bRelay, cRelay);
	if(!hRelay) {
		System_abort("Error initializing relay pin\n");
	}

    Task_Params_init(&taskParams);
    taskParams.stackSize = TASKSTACKSIZE;
    taskParams.stack = &taskStack;
	taskParams.priority = 2;
    taskHandle = Task_create((Task_FuncPtr)relayFxn, &taskParams, NULL);
    if (taskHandle == NULL) {
    	System_abort("Task create failed!");
    }

    BIOS_start();

    return (0);
}
