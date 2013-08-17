#define PART_TM4C1233H6PM 1

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <inc/hw_memmap.h>
#include <inc/hw_types.h>
#include <inc/hw_gpio.h>
#include <driverlib/debug.h>
#include <driverlib/gpio.h>
#include <driverlib/uart.h>
#include <driverlib/i2c.h>
#include <driverlib/rom.h>
#include <driverlib/timer.h>
#include <driverlib/interrupt.h>
#include <driverlib/pin_map.h>
#include <driverlib/sysctl.h>

#include <inc/tm4c1233h6pm.h>


#define toggleRed()  GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, ~GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_1))
#define toggleBlue()  GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, ~GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_2))

#define setMotor0(x) TimerMatchSet(WTIMER0_BASE, TIMER_A, SysCtlClockGet() / 10000 * x)
#define setMotor1(x) TimerMatchSet(WTIMER0_BASE, TIMER_B, SysCtlClockGet() / 10000 * x)
#define setMotor2(x) TimerMatchSet(WTIMER1_BASE, TIMER_A, SysCtlClockGet() / 10000 * x)
#define setMotor3(x) TimerMatchSet(WTIMER1_BASE, TIMER_B, SysCtlClockGet() / 10000 * x)

#define setServo(a,b) servos[a].duty = b

#define readEncoder0() enc

typedef struct {
    unsigned long port;
    unsigned long pin;
    float duty;
} tServo;

volatile tServo servos[8] = {
    {GPIO_PORTB_BASE, GPIO_PIN_6, 0.5},
    {GPIO_PORTB_BASE, GPIO_PIN_7, 0.5},
    {GPIO_PORTB_BASE, GPIO_PIN_4, 0.5},
    {GPIO_PORTB_BASE, GPIO_PIN_5, 0.5},
    {GPIO_PORTE_BASE, GPIO_PIN_4, 0.5},
    {GPIO_PORTE_BASE, GPIO_PIN_5, 0.5},
    {0, 0, 0.0},
    {0, 0, 0.0}
};

volatile unsigned char servoNdx = 0;

void WTimer5AIntHandler(void)
{
    // Move to next Servo Pin
    servoNdx = (servoNdx + 1) & 0x7;
   
    // Enable Servo Pin
    GPIOPinWrite(servos[servoNdx].port, servos[servoNdx].pin, 0xFF);

    // Set new servo value
    // TODO: Try to set servo w/o disabling timer
    TimerDisable(WTIMER5_BASE, TIMER_B);
    TimerLoadSet(WTIMER5_BASE, TIMER_B, SysCtlClockGet() / 50 / 8 * servos[servoNdx].duty);
    TimerEnable(WTIMER5_BASE, TIMER_B);


    // Ack Interrupt
    TimerIntClear(WTIMER5_BASE, TIMER_TIMA_TIMEOUT);        
}

void WTimer5BIntHandler(void)
{
    // Bring current Servo Pin low
    GPIOPinWrite(servos[servoNdx].port, servos[servoNdx].pin, 0);
    
    // Ack Interrupt
    TimerIntClear(WTIMER5_BASE, TIMER_TIMB_TIMEOUT);
}

typedef struct {
    int out[4];
} tFSMState;

static tFSMState FSM[4] = {
    {{ 0, 1,-1, 0}},
    {{-1, 0, 0, 1}},
    {{ 1, 0, 0,-1}},
    {{ 0,-1, 1, 0}}
};


typedef struct{
    long value;
    tFSMState next;
    tFSMState last;
    char lastval;
} QEIEncoder;

static QEIEncoder enc0;

static int freq[4] = {0,0,0,0};


void PortDIntHandler(void)
{
    unsigned char ndx;
    
    // QEI State Machine
    ndx = GPIOPinRead(GPIO_PORTD_BASE, GPIO_PIN_6 | GPIO_PIN_7) >> 6;
    enc0.value += enc0.next.out[ndx];
    enc0.lastval = GPIOPinRead(GPIO_PORTD_BASE, GPIO_PIN_6 | GPIO_PIN_7);
    enc0.last = enc0.next;
    enc0.next = FSM[ndx];

    freq[ndx]++;

    // Ack Interrupt
    GPIOIntClear(GPIO_PORTD_BASE, GPIO_PIN_6 | GPIO_PIN_7);
}


int main(void)
{ 
    // Clock (80MHz)
    SysCtlClockSet(SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);
    
    
    // GPIO
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2);
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0);

    
    // UART
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 115200, (UART_CONFIG_PAR_NONE | UART_CONFIG_STOP_ONE | UART_CONFIG_WLEN_8));
    
    UARTEnable(UART0_BASE);
    

    // GPIO (Motors)
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_6 | GPIO_PIN_7);
    GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    
    // PWM (Motors)
    SysCtlPeripheralEnable(SYSCTL_PERIPH_WTIMER0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_WTIMER1);
    GPIOPinConfigure(GPIO_PC4_WT0CCP0);
    GPIOPinConfigure(GPIO_PC5_WT0CCP1);
    GPIOPinConfigure(GPIO_PC6_WT1CCP0);
    GPIOPinConfigure(GPIO_PC7_WT1CCP1);
    GPIOPinTypeTimer(GPIO_PORTC_BASE, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7);

    TimerConfigure(WTIMER0_BASE, TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_PWM | TIMER_CFG_B_PWM);
    TimerLoadSet(WTIMER0_BASE, TIMER_BOTH, SysCtlClockGet() / 10000); //10KHz
    TimerMatchSet(WTIMER0_BASE, TIMER_A, 1);
    TimerMatchSet(WTIMER0_BASE, TIMER_B, 1);

    TimerConfigure(WTIMER1_BASE, TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_PWM | TIMER_CFG_B_PWM);
    TimerLoadSet(WTIMER1_BASE, TIMER_BOTH, SysCtlClockGet() / 10000); //10KHz
    TimerMatchSet(WTIMER1_BASE, TIMER_A, 1);
    TimerMatchSet(WTIMER1_BASE, TIMER_B, 1);
    
    TimerEnable(WTIMER0_BASE, TIMER_A);
    TimerEnable(WTIMER0_BASE, TIMER_B);
    TimerEnable(WTIMER1_BASE, TIMER_A);
    TimerEnable(WTIMER1_BASE, TIMER_B);


    // Motors Fwd
    GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_6 | GPIO_PIN_7, GPIO_PIN_6 | GPIO_PIN_7);
    GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_0 | GPIO_PIN_1, GPIO_PIN_0 | GPIO_PIN_1);

    setMotor0(0.95);
    setMotor1(0.65);
    setMotor2(0.35);
    setMotor3(0.05);


    // Soft/Hard Servo PWM Generator
    //  Enable Servo Pins
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7);
    GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_4 | GPIO_PIN_5);
    GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7, 0);
    GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4 | GPIO_PIN_5, 0);

    //  Register up to 8 servos to single timer
    SysCtlPeripheralEnable(SYSCTL_PERIPH_WTIMER5);
    TimerConfigure(WTIMER5_BASE, TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_PERIODIC | TIMER_CFG_B_PERIODIC);
    TimerLoadSet(WTIMER5_BASE, TIMER_A, SysCtlClockGet() / 50 / 8); // Mux 8 Servos
    TimerLoadSet(WTIMER5_BASE, TIMER_B, SysCtlClockGet() / 50 / 16);
   
    IntEnable(INT_WTIMER5A);
    IntEnable(INT_WTIMER5B);
    TimerIntEnable(WTIMER5_BASE, TIMER_TIMA_TIMEOUT | TIMER_TIMB_TIMEOUT);
    TimerEnable(WTIMER5_BASE, TIMER_BOTH);
    
    
    // I2C
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C0);
    GPIOPinConfigure(GPIO_PB2_I2C0SCL);
    GPIOPinConfigure(GPIO_PB3_I2C0SDA);
    GPIOPinTypeI2C(GPIO_PORTB_BASE, GPIO_PIN_3);
    GPIOPinTypeI2CSCL(GPIO_PORTB_BASE, GPIO_PIN_2);
    I2CMasterInitExpClk(I2C0_BASE, SysCtlClockGet(), true);
    
    I2CMasterEnable(I2C0_BASE);


    //  Setup PCA9557
    I2CMasterSlaveAddrSet(I2C0_BASE, 0x18, false);  // PCA9775 Addr
    I2CMasterDataPut(I2C0_BASE, 0x03);              // Set Direction
    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_START);
    while(I2CMasterBusy(I2C0_BASE));
    I2CMasterDataPut(I2C0_BASE, 0x0F);              // GPIO: Inputs; Ctrl Sigs & FETs: Ouputs
    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_FINISH);
    while(I2CMasterBusy(I2C0_BASE));

    I2CMasterSlaveAddrSet(I2C0_BASE, 0x18, false);  // Set Polarity
    I2CMasterDataPut(I2C0_BASE, 0x02);             
    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_START);
    while(I2CMasterBusy(I2C0_BASE));
    I2CMasterDataPut(I2C0_BASE, 0x00);            
    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_FINISH);
    while(I2CMasterBusy(I2C0_BASE));

    I2CMasterSlaveAddrSet(I2C0_BASE, 0x18, false);  // Set Outputs Directions
    I2CMasterDataPut(I2C0_BASE, 0x01);             
    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_START);
    while(I2CMasterBusy(I2C0_BASE));
    I2CMasterDataPut(I2C0_BASE, 0x80 | 0x10);            
    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_FINISH);
    while(I2CMasterBusy(I2C0_BASE));


    /*for(i=0; i<4; i++){
        I2CMasterSlaveAddrSet(I2C0_BASE, 0x18, false);
        I2CMasterDataPut(I2C0_BASE, 0x01);
        I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_SINGLE_SEND);
        while(I2CMasterBusy(I2C0_BASE));

        I2CMasterSlaveAddrSet(I2C0_BASE, 0x18, true);
        I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_SINGLE_RECEIVE);
        while(I2CMasterBusy(I2C0_BASE));

        printf("x%0X\r\n", I2CMasterDataGet(I2C0_BASE));
    }*/



    // SPI
    /*SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI0);
    GPIOPinConfigure(GPIO_PA2_SSI0CLK);
    GPIOPinConfigure(GPIO_PA4_SSI0RX);
    GPIOPinConfigure(GPIO_PA5_SSI0TX);
    GPIOPinTypeSSI(GPIO_PORTA_BASE, GPIO_PIN_2 | GPIO_PIN_4 | GPIO_PIN_5);
    SSIConfigSetExpClk(SSI0_BASE, SysCtlClockGet(), SSI_FRF_MOTO_MODE_1, SSI_MODE_MASTER, 1000000, 16);
    
    SSIEnable(SSI0_BASE);
*/
    // Unlock PD7
    HWREG(GPIO_PORTD_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
    HWREG(GPIO_PORTD_BASE + GPIO_O_CR) = GPIO_PIN_7;
    HWREG(GPIO_PORTD_BASE + GPIO_O_LOCK) = 0;


    //  Encoders (Port D)
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    GPIOPinTypeGPIOInput(GPIO_PORTD_BASE, GPIO_PIN_6 | GPIO_PIN_7);
    GPIOIntTypeSet(GPIO_PORTD_BASE, GPIO_PIN_6 | GPIO_PIN_7, GPIO_BOTH_EDGES);
    GPIOIntEnable(GPIO_PORTD_BASE, GPIO_PIN_6 | GPIO_PIN_7);
    
    IntEnable(INT_GPIOD);

    // Enable Interrupts
    IntMasterEnable();


    printf("\n\n");

    while(1){
        // LED On
        //toggleRed();

        /*switch(UARTCharGet(UART0_BASE))
        {
            case 'w':
                percentage += 0.1;
                break;
            case 's':
                percentage -= 0.1;
                break;
            case 'q':
                percentage += 0.01;
                break;
            case 'a':
                percentage -= 0.01;
                break;
        }*/
        

        // Toggle Motors & Solenoids @ .5 Hz
        /*toggleRed();
        SysCtlDelay(SysCtlClockGet() / 3);

        I2CMasterSlaveAddrSet(I2C0_BASE, 0x18, false);  // Set Outputs Directions
        I2CMasterDataPut(I2C0_BASE, 0x01);             
        I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_START);
        while(I2CMasterBusy(I2C0_BASE));
        I2CMasterDataPut(I2C0_BASE, 0x00);            
        I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_FINISH);
        while(I2CMasterBusy(I2C0_BASE));


        toggleRed();
        SysCtlDelay(SysCtlClockGet() / 3);

        I2CMasterSlaveAddrSet(I2C0_BASE, 0x18, false);  // Set Outputs Directions
        I2CMasterDataPut(I2C0_BASE, 0x01);             
        I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_START);
        while(I2CMasterBusy(I2C0_BASE));
        I2CMasterDataPut(I2C0_BASE, 0xF0);            
        I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_FINISH);
        while(I2CMasterBusy(I2C0_BASE));
        */

      /*
        setServo(0, 0.8);
        setServo(1, 0.8);
        //printf("\r%%: %d\n", (unsigned int) (servos[0].duty * 100));
        SysCtlDelay(SysCtlClockGet() / 3);

        setServo(0, 0.2);
        setServo(1, 0.2);
        //printf("\r%%: %d\n", (unsigned int) (servos[0].duty * 100));
        SysCtlDelay(SysCtlClockGet() / 3);
*/
        //readEncoder0();
        //readEncoder1();


        SysCtlDelay(SysCtlClockGet() / 3 / 10);

        printf("Enc0: %ld\r\n", enc0.value);

        toggleRed();

/*
        I2CMasterSlaveAddrSet(I2C0_BASE, 0x18, false);  // Set Outputs Directions
        I2CMasterDataPut(I2C0_BASE, 0x01);             
        I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_START);
        while(I2CMasterBusy(I2C0_BASE));
        if (GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4) == 0)
            I2CMasterDataPut(I2C0_BASE, 0x80);            
        else
            I2CMasterDataPut(I2C0_BASE, 0x70);            
        I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_FINISH);
        while(I2CMasterBusy(I2C0_BASE));*/


        /*I2CMasterSlaveAddrSet(I2C0_BASE, 0x18, false);
        I2CMasterDataPut(I2C0_BASE, 0x00);
        I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_SINGLE_SEND);
        while(I2CMasterBusy(I2C0_BASE));

        I2CMasterSlaveAddrSet(I2C0_BASE, 0x18, true);
        I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_SINGLE_RECEIVE);
        while(I2CMasterBusy(I2C0_BASE));

        if( (I2CMasterDataGet(I2C0_BASE) & 0x01) != 0)
            toggleBlue();*/
    }
}
