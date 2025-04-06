# uberOS

An experimental simple OS for ubertooth_one. <br>

It use the ubertooth_one bootloader and dfu update mecanisme. <br>

The sources tree is : <br>
<ul>
  <li>core/ # contain all core code : interrupts, initialization, linker script, ..</li>
  <li>kernel/ # contain all os-related code except assembly exception handler</li>
  <li>app/*.[ch] # contain application code including main</li>
  <li>drivers/ # peripherals drivers code : UART, ...</li>
  <li>lib/ general purpose functions : list, ...</li>
</ul>

core/system_UBERTOOTH.c contains system initialization : corem3, clock, gpio <br>

A debug output is available on expansion IO. <br>
DWT->CYCCNT can be used to measure cycle. <br>
HardFault_Handler catch stacks and registers and print it to UART1 <br>
debug uart is implemented by drivers/lpc17_uart.c <br>
Clock is 100MhZ, systick configured to interrupt at 100Hz rate, <br>

The 3 task example light the leds at different rate, see kernel/simple_rtos.c <br>
A 4th usb task is used to handle firmware update request (switch to DFU mode). <br>
A very simple context switching is implemented to switch between tasks. <br>

Most of the code comes from uberooth_one project. <br>

![Screenshot from 2025-04-06 13-07-18](https://github.com/user-attachments/assets/6deb4635-869f-49a4-b609-772fc70767ad)
