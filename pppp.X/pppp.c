/* File:   main.c
 * Author: Santiago
 *
 */

// PIC16F887 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT
#pragma config WDTE = OFF
#pragma config PWRTE = OFF
#pragma config MCLRE = OFF
#pragma config CP = OFF
#pragma config CPD = OFF
#pragma config BOREN = OFF
#pragma config IESO = OFF
#pragma config FCMEN = OFF
#pragma config LVP = OFF

#pragma config BOR4V = BOR40V
#pragma config WRT = OFF
        
#include <xc.h>

#include <stdio.h>

#define _XTAL_FREQ 8000000

char resp;
uint8_t n;
uint8_t p3;
uint8_t p4;
uint8_t p1L;
uint8_t p1H;
uint8_t p2;
int ban = 0;
int fla = 0;
void setup(void);
void txt_UART(char txt[]); 
uint8_t read(uint8_t address);
void write(uint8_t address, uint8_t data);

    
void __interrupt() isr(void){
    
    if(PIR1bits.RCIF){

    }
    
    if (INTCONbits.RBIF){
        INTCONbits.RBIF = 0;
    }
    
    if (PIR1bits.ADIF){
        if (ADCON0bits.CHS == 0b0000){
            p1H = ADRESH;
            p1L = ADRESL;
            CCPR1L = (p1H>>1)+124;
            CCP1CONbits.DC1B1 = p1H & 0b01;
            CCP1CONbits.DC1B0 = p1L >> 7;
            
        }
        else if(ADCON0bits.CHS == 0b0001){
            p2 = ADRESH;
            CCPR2L = (p2>>1)+124;
            CCP2CONbits.DC2B1 = p2 & 0b01;
            CCP2CONbits.DC2B0 = ADRESL >> 7;
        }
        else if(ADCON0bits.CHS == 0b0011){
            p4 = ADRESH;
        PIR1bits.ADIF = 0;
        }
        else 
            p3 = ADRESH;
        PIR1bits.ADIF = 0;
    }
    
    
    if (INTCONbits.T0IF){
         OPTION_REGbits.PSA = 0;
         OPTION_REGbits.PS =  p3/20;
         __delay_us(500);
         

        if (fla == 1){ 
            n++;
            if (n < p4){
                PORTDbits.RD1 = 1;
            }
            else
                PORTDbits.RD1 = 0;
        } 

        if (PORTDbits.RD0 == 1){
            PORTDbits.RD0 = 0;
           __delay_us(500); //1000
        }
        else
             __delay_us(500);
            PORTDbits.RD0 = 1;
            
        TMR0 =255;
        __delay_us(100);
        INTCONbits.T0IF = 0;   
}    
}
void main(void) {
    setup();
    while(1){
        
        if(ADCON0bits.GO == 0){
            if(ADCON0bits.CHS == 0b0000)
                ADCON0bits.CHS = 0b0001;
            else if(ADCON0bits.CHS == 0b0001)
                 ADCON0bits.CHS = 0b0010;
            else if(ADCON0bits.CHS == 0b0010)
                 ADCON0bits.CHS = 0b0011;
            else        
                ADCON0bits.CHS = 0b0000;
            __delay_us(1000); //1000
            ADCON0bits.GO = 1;  
        }
        
        if (PORTBbits.RB0==0){
            if (ban == 0){
                ADCON0bits.ADON = 0;
                txt_UART("\r Que accion desea ejecutar? \r");
                txt_UART(" 1) Estirar \r");
                txt_UART(" 2) Flexionar \r");
                txt_UART(" 3) Guardar a EEPROM \r");
                txt_UART(" 4) Leer de EEPROM\r");
                txt_UART(" 5) I/O Sierra\r");
                while(PIR1bits.RCIF == 0);
                resp = RCREG;
                switch(resp){
                    
                case ('1'):
                    txt_UART("Estirado \r");
                    p1H = 0;
                    CCPR1L = (p1H>>1)+124;
                    CCP1CONbits.DC1B1 = p1H & 0b01;
                    p2 = 0;
                    CCPR2L = (p2>>1)+124;
                    CCP2CONbits.DC2B1 = p2 & 0b01;
                    p3 = 0;
                    break;
                    
                case ('2'):
                    txt_UART("Flexionado \r");
                    p1H = 255;
                    CCPR1L = (p1H>>1)+124;
                    CCP1CONbits.DC1B1 = p1H & 0b01;
                    p2 = 255;
                    CCPR2L = (p2>>1)+124;
                    CCP2CONbits.DC2B1 = p2 & 0b01;
                    p3 = 255;
                    break;
                    
                case ('3'):
                    txt_UART("Guardado \r");
                    write(0x00, p1H);
                    __delay_ms(10);
                    write(0x01, p2);
                    __delay_ms(10);
                    write(0x02, p3);
                    __delay_ms(10);
                    write(0x03, p4);
                    __delay_ms(1000);
                    ADCON0bits.ADON = 1;
                    break;   
                    
                case ('4'):
                    ADCON0bits.ADON = 0;
                    p1H = read(0x00);
                     __delay_ms(10);
                    CCPR1L = (p1H>>1)+124;
                    CCP1CONbits.DC1B1 = p1H & 0b01;
                    txt_UART("\rSe apago adon \r");
                    p2 = read(0x01);
                    __delay_ms(10);
                    CCPR2L = (p2>>1)+124;
                    CCP2CONbits.DC2B1 = p2 & 0b01;
                   p3 = read(0x02);
                    __delay_ms(10);
                    p4 = read(0x03);
                    __delay_ms(1000);
                    break;
                    
                case ('5'):
                    if(fla==1)
                        txt_UART("Sierra encendida \r");
                    else
                        txt_UART("Sierra apagada \r");
                    PORTDbits.RD3 = fla;
                    fla ^=1;
                    __delay_ms(1000);
                    break;
                    
                default:
                txt_UART("Se ha producido un error \r");
                }
                ban=1;
                txt_UART("Se cambio ban \r");
               __delay_ms(1000);
            }
            else
            {
                txt_UART("\rSe prendio adon \r");
                ADCON0bits.ADON = 1;
                ban = 0;
                __delay_ms(1000);
            }
        }
        if (PORTBbits.RB1 == 0){
            ADCON0bits.ADON = 1;
                write(0x00, p1H);
                __delay_ms(10);
                write(0x01, p2);
                __delay_ms(10);
                write(0x02, p3);
                __delay_ms(10);
                write(0x03, p4);
                txt_UART("\rSe prendio adon \r");
                __delay_ms(1000);
        }
        if (PORTBbits.RB2 == 0){
            ADCON0bits.ADON = 0;
                p1H = read(0x00);
                 __delay_ms(10);
                CCPR1L = (p1H>>1)+124;
                CCP1CONbits.DC1B1 = p1H & 0b01;
                txt_UART("\rSe apago adon \r");
                p2 = read(0x01);
                __delay_ms(10);
                CCPR2L = (p2>>1)+124;
                CCP2CONbits.DC2B1 = p2 & 0b01;
               p3 = read(0x02);
                __delay_ms(10);
                p4 = read(0x03);
                __delay_ms(1000);
        }
        if (PORTBbits.RB3 == 0){
            PORTDbits.RD3 = fla;
            fla ^=1;
            __delay_ms(1000);
        }
          __delay_ms(10); 
            
        }
    }
    


void txt_UART(char txt[]){
    int i = 0;
    while(txt[i] != '\0'){
        while(!TXSTAbits.TRMT);
        TXREG = txt[i]; // Enviamos el caracter actual
        i++;
    }
}

uint8_t read(uint8_t address){
    EEADR = address;
    EECON1bits.EEPGD = 0;
    EECON1bits.RD = 1;
    return EEDAT;
}

void write(uint8_t address, uint8_t data){
    EEADR = address;
    EEDAT = data;
    EECON1bits.EEPGD = 0;
    EECON1bits.WREN = 1;
    INTCONbits.GIE = 0;
    INTCONbits.PEIE = 0;
    EECON2 = 0x55;
    EECON2 = 0xAA;
    EECON1bits.WR = 1;
    EECON1bits.WREN = 0;
    INTCONbits.RBIF = 0;
    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;
}

void setup(void){
    ANSEL = 0b00001111;
    ANSELH = 0;
    
    TRISA = 0b00001111;
    PORTA = 0;
    TRISD = 0;
    PORTD = 0;
    TRISB = 0b00001111;
    PORTB = 0;
    OSCCONbits.IRCF = 0b111;
    OPTION_REGbits.nRBPU = 0;
    INTCONbits.RBIE = 1;
    OSCCONbits.SCS = 1;
    ADCON1bits.ADFM = 0;
    ADCON1bits.VCFG0 = 0;
    ADCON1bits.VCFG1 = 0;
    ADCON0bits.ADCS = 0b10;
    ADCON0bits.ADON = 1;
    __delay_us(50);
    TRISCbits.TRISC2 = 1;
    TRISCbits.TRISC1 = 1;
    CCP1CONbits.P1M = 0;
    CCP1CONbits.CCP1M = 0b1100;
    CCP2CONbits.CCP2M = 0b1100;
    CCPR1L = 0x0F;
    CCP1CONbits.DC1B = 0;
    CCPR2L = 0x0F;
    CCP2CONbits.DC2B1 = 0;
    CCP2CONbits.DC2B0 = 0;
    OPTION_REGbits.T0CS = 0;
    PR2 = 255;
    T2CONbits.T2CKPS = 0b11;
    T2CONbits.TMR2ON = 1;
    PIR1bits.TMR2IF = 0;
    while (PIR1bits.TMR2IF == 0);
    PIR1bits.TMR2IF = 0;
    TRISCbits.TRISC2 = 0;
    TRISCbits.TRISC1 = 0;
    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;
    PIE1bits.ADIE = 1;
    INTCONbits.TMR0IE = 1;
    PIR1bits.ADIF = 0;
    INTCONbits.T0IF = 0;
    INTCONbits.RBIF = 0;
    PIE1bits.TMR1IE = 1;   
    TXSTAbits.SYNC = 0;
    TXSTAbits.BRGH = 1; 
    BAUDCTLbits.BRG16 = 1; 
    SPBRG = 207; 
    SPBRGH = 0;
    RCSTAbits.SPEN = 1;
    RCSTAbits.RX9 = 0; 
    RCSTAbits.CREN = 1; 
    TXSTAbits.TXEN = 1;  
}