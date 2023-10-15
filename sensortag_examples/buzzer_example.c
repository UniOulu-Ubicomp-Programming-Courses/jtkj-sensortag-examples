/* XDCtools files */
#include <xdc/std.h>
#include <xdc/runtime/System.h>

/* BIOS Header files */
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/drivers/PIN.h>
#include <ti/drivers/pin/PINCC26XX.h>

/* Board Header files */
#include "Board.h"
#include "buzzer.h" // remember to add buzzer.h and buzzer.c into project directory!

/* Task */
#define STACKSIZE 2048
Char taskStack[STACKSIZE];

static PIN_Handle hBuzzer;
static PIN_State sBuzzer;
PIN_Config cBuzzer[] = {
  Board_BUZZER | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MAX,
  PIN_TERMINATE
};

Void taskFxn(UArg arg0, UArg arg1) {

  while (1) {
    buzzerOpen(hBuzzer);
    buzzerSetFrequency(2000);
    Task_sleep(50000 / Clock_tickPeriod);
    buzzerClose();

    Task_sleep(950000 / Clock_tickPeriod);
  }

}

Int main(void) {

  Task_Handle task;
  Task_Params taskParams;

  // Initialize board
  Board_initGeneral();

  // Buzzer
  hBuzzer = PIN_open(&sBuzzer, cBuzzer);
  if (hBuzzer == NULL) {
    System_abort("Pin open failed!");
  }

  Task_Params_init(&taskParams);
  taskParams.stackSize = STACKSIZE;
  taskParams.stack = &taskStack;
  task = Task_create((Task_FuncPtr)taskFxn, &taskParams, NULL);
  if (task == NULL) {
    System_abort("Task create failed!");
  }

  /* Sanity check */
  System_printf("Beeps!\n");
  System_flush();

  /* Start BIOS */
  BIOS_start();

  return (0);
}
