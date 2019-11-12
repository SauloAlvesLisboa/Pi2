#include <msp430g2553.h>
#include <legacymsp430.h> //biblioteca para facilitar a chamada das interrupções

#define ADC_CHANNELS 6 //Definindo a quantida de canais AD a serem lidos
#define LED 0x3F;
#define SCL BIT6
#define SDA BIT7
#define DLY 0x6000

unsigned int samples[ADC_CHANNELS]; //Vetor para guardar o valor do registrador

int led_buff=0;

void Init_P2(void);
void Init_AD(void);

int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;  //Parar o cachorrão
  BCSCTL1 = CALBC1_1MHZ;    //setando o clock da cpu em 1MHZ
  DCOCTL = CALDCO_1MHZ;
  TA0CCR0 = 10000-1;   //Timer A
  TA0CTL = TASSEL_2 + ID_3 + MC_1 + TAIE;  //Timer A


  Init_P2();  //Inicializando a Porta P1
  Init_AD();  //Inicializando as Portas Analógicas
  init_I2C(0xF);  //Inicia comunicação I2C
  _BIS_SR(LPM0_bits+GIE); //Modo de baixo consumo com interrupções habilitadas
  return 0;

}

//Rotina de Tratamento da Interrupção do Timer A
interrupt(TIMER0_A1_VECTOR) TA0_ISR(void)
{

  TA0CTL &= ~TAIFG; //Reseta a Flag da interrupção
  // Manda comecar uma conversao
  ADC10CTL0 &= ~ENC;                    // Abilitando o adc
  while (ADC10CTL1 & BUSY);


  ADC10SA = (unsigned int)samples;      //salvando as leituras no vetor
  ADC10CTL0 |= ENC + ADC10SC;             // Sampling and conversion start
  //__bis_SR_register(CPUOFF + GIE);        // LPM0, ADC10_ISR will force exit
 //ju
  if (samples[0]>=500){
      P2OUT |= BIT3;
    }
  if (samples[0]<500){
      P2OUT &= ~BIT3;
    }
  if (samples[1]>=500){
      P2OUT |= BIT5;
    }
  if (samples[1]<500){
      P2OUT &= ~BIT5;
    }
  if (samples[2]>=500){
      P2OUT |= BIT4;
    }
  if (samples[2]<500){
      P2OUT &= ~BIT4;
    }
  if (samples[3]>=500){
      P2OUT |= BIT2;
    }
  if (samples[3]<500){
      P2OUT &= ~BIT2;
    }
  if (samples[4]>=500){
      P2OUT |= BIT1;
    }
  if (samples[4]<500){
      P2OUT &= ~BIT1;
    }
  if (samples[5]>=500){
      P2OUT |= BIT0;
    }
  if (samples[5]<500){
      P2OUT &= ~BIT0;
    }

}

void Init_P2(void)
{
  P2DIR |= LED;
}

void Init_AD(void)
{
  //ADC10CTL0 = Control Parameters; ADC10SHT_2 = Sample and Hold time clock cycles(2=16 clock cycles); MSC = Multiple Samples; ADC10ON = Ativa o ADC10
  ADC10CTL0 = ADC10SHT_2 + SREF_0 + MSC + ADC10ON;
  //INCH_5 = Porta A5 para inicio das conversões; CONSEQ_1 = Sequence of channels converted once; ADC10DIV+0 = ADC10 clock divider; ADC10SSEL_3 = ADC10 clock source; SHS_0 = Sample and Hold source Select
  ADC10CTL1 = INCH_5 + CONSEQ_1 + ADC10DIV_0 + ADC10SSEL_3 + SHS_0;
  //ADC10 data transfer channels; Tem que ser o mesmo número de entradas analógicas
  ADC10DTC1 = ADC_CHANNELS;
  //Máscara para os pinos analógicos.
  ADC10AE0 |= BIT5+BIT4+BIT3+BIT2+BIT1+BIT0;

}


//I2C
void Atraso(volatile unsigned int x)
{
  while(x--);
}

void Send_Data(volatile unsigned char c)
{
  while((IFG2&UCA0TXIFG)==0);
  UCA0TXBUF = c;
}

void Pisca(unsigned char pino, char piscadas, unsigned int tempo)
{
  piscadas <<= 1;
  while(piscadas--)
  {
    P1OUT ^= pino;
    Atraso(tempo);
  }
}

void init_I2C(volatile unsigned int slave_address)
{
  UCB0CTL1 |= UCSWRST;                      // Enable SW reset
  UCB0CTL1 &= ~UCTR;
  UCB0CTL0 = UCMODE_3 + UCSYNC;             // I2C Master, synchronous mode
  UCB0I2COA = slave_address; //|UCGCEN;
  P1SEL |= SCL | SDA;                       // Assign I2C pins to USCI_B0
  P1SEL2|= SCL | SDA;                       // Assign I2C pins to USCI_B0
  UCB0STAT &= ~(UCSTPIFG | UCSTTIFG);
  UCB0CTL1 &= ~UCSWRST;                     // Clear SW reset, resume operation
  IE2 |= UCB0RXIE;// + UCB0TXIE;               //Enable RX and TX interrupt
  //UCB0I2CIE |= UCSTPIE | UCSTTIE;
}

//void Transmit(char data[], int len)
void Transmit(unsigned int data)
{
    UCB0TXBUF = data;
  //volatile unsigned int i;
  //for(i=0; i<len; i++)
  //{
    //while((IFG2 & UCB0TXIFG)==0);
    //UCB0TXBUF = data[i];
  //}
}

interrupt(USCIAB0TX_VECTOR) USCIAB0TX_ISR(void)
{
  int d, retorna;
  //char send_data[2];

  if(IFG2 & UCB0RXIFG)
  {
    d = UCB0RXBUF;
    //if(d<6)
      //Pisca(LED, d, DLY);
    if(d==1){
      //retorna = 100;
      retorna = samples[0];
    }
    else if(d==2){
      //retorna = 50;
      retorna = samples[1];
    }
    else if(d==3){
      retorna = samples[2];
    }
    while( (UCB0STAT & UCSTTIFG)==0);
    //send_data[0] = retorna;
    //send_data[1] = 0;
    //Transmit(send_data,1);
    Transmit(retorna);
    UCB0STAT &= ~(UCSTPIFG | UCSTTIFG);
  }
  IFG2 &= ~(UCB0TXIFG|UCB0RXIFG);
}
