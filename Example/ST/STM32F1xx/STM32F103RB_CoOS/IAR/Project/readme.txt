Chip£º
    ST STM32103RBT6

Evaluation Board£º
    MCB STM32

Development Environment£º
    IAR 530

Complier£º
    ICCARM

Tasks:
    task_init       : initialize the target board's resources, and create the majority of tasks, and then delete itself and exit the scheduling
    lcd_display_adc : display the current value of ADC on LCD1602
    uart_print      : print the current AD value of ADC by serial port
    led_blink       : Control the flashing of LEDs
    time_display    : Display time, the format likes xx:xx:xx
    time_set        : set time 
    lcd_blink       : blink the hour, minute or second that is being set up and show it on LCD1602 when setting time 

Descriptions:
    1.This Demo simulates a real-time clock on LCD1602 which users can set time freely:
	 Press WKUP(S2) button to choose the hour, minute or second that you want to set, at the same time as set of exit(when press it the fourth time);
	 Press TAMP(S3) button to increase the hour,minute or second which was choosed to set by 1.
    
    2.System samples the value of AD at regular intervals and show it by histogram on LCD1602, at the same time the serial port prints the value of AD. You       can adjust the input voltage of AD by the regulation resistance on board, and you could see the change on LCD1602 or through the serial print.
    
    3.The 8 LEDs on board keep flickering by the style that set.

System Configuration:
    XTAL   freq      =  8.00 MHz
    SYSCLK freq      = 60.00 MHz
    system Tick freq = 100Hz (10ms)