// DAC.c
// DAC Driver
// Lab 5
// Samuel Zhang and Joel Wang
// 2/19/18

// SSI3Clk (SCLK, pin 2) connected to PD0
// SSI3Fss (!CS, pin 3) connected to PD1
// SSI3Tx (DIN, pin 1) connected to PD3
// see Figure 7.19 for complete schematic
#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"


//********DAC_Init*****************
// Initialize Max5353 12-bit DAC
// inputs:  initial voltage output (0 to 4095)
// outputs: none
// assumes: system clock rate less than 20 MHz
void DAC_Init(uint16_t data){
	volatile uint32_t delay;
  SYSCTL_RCGCSSI_R |= 0x08;       // activate SSI3
  SYSCTL_RCGCGPIO_R |= 0x08;      // activate port D
	delay = SYSCTL_RCGC2_R;               // allow time to finish activating
  GPIO_PORTD_AFSEL_R |= 0x0B;     // enable alt funct on PD0,1,3
  GPIO_PORTD_DEN_R |= 0x0B;       // configure PD0,1,3 as SSI
  GPIO_PORTD_PCTL_R = (GPIO_PORTD_PCTL_R&0xFFFF0F00)+0x00001011;
  GPIO_PORTD_AMSEL_R = 0;         // disable analog functionality on PA
  SSI3_CR1_R = 0x00000000;        // disable SSI, master mode
  SSI3_CPSR_R = 0x02;             // 8 MHz SSIClk 
  SSI3_CR0_R &= ~(0x0000FFF0);    // SCR = 0, SPH = 0, SPO = 0 Freescale
  SSI3_CR0_R |= 0x0F;             // DSS = 16-bit data
  SSI3_DR_R = data;               // load 'data' into transmit FIFO
  SSI3_CR1_R |= 0x00000002;       // enable SSI
	
}



//********DAC_Out*****************
// Send data to Max5353 12-bit DAC
// inputs:  voltage output (0 to 4095)
// outputs: none
void DAC_Out(uint16_t voltage){   
  while((SSI3_SR_R&0x00000002)==0){};// SSI Transmit FIFO Not Full
  SSI3_DR_R = voltage; }                // data out, no reply
