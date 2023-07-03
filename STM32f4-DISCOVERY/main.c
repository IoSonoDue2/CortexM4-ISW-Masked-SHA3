#include<string.h>
#include "Encryption.h"

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>


#define MODE_1 "sha3_256"
#define MODE_2 "sha3_512"
#define MODE_3 "secret"
#define MODE_4 "shake_128"
#define MODE_5 "shake_256"

#define outlen_shake = 128



int mode = 0;

#define MAX_STR_LEN 100000

void clock_setup(void) {

//configura l'orologio del sistema per funzionare a 84MHz utilizzando un oscilatore ad alta velocità esterno (HSE) da 8MHz come fonte di clock e un PLL (Phase-Locked Loop) per moltiplicare la frequenza.
    rcc_clock_setup_pll(&rcc_hse_8mhz_3v3[RCC_CLOCK_3V3_84MHZ]);

//abilita i clock per GPIOA, USART2 e RNG
    rcc_periph_clock_enable(RCC_GPIOA);
    
    rcc_periph_clock_enable(RCC_USART2);

    rcc_periph_clock_enable(RCC_RNG);
}

void usart_setup(void) {
//imposta la velocità di trasmissione (baudrate) di USART2 a 115200 bps.
    usart_set_baudrate(USART2, 115200);
//imposta il numero di bit di dati per trasmissione o ricezione su USART2 a 8.
    usart_set_databits(USART2, 8);
//imposta il numero di bit di stop per trasmissione o ricezione su USART2 a 1.
    usart_set_stopbits(USART2, USART_STOPBITS_1);
//imposta la modalità di USART2 a TX e RX, il che significa che può trasmettere e ricevere dati.
    usart_set_mode(USART2, USART_MODE_TX_RX);
//imposta il parity di USART2 a nessuna, il che significa che non viene utilizzata il parity check per il controllo degli errori.
    usart_set_parity(USART2, USART_PARITY_NONE);
//imposta il controllo del flusso di USART2 a nessuno, il che significa che non viene utilizzato il controllo del flusso.
    usart_set_flow_control(USART2, USART_FLOWCONTROL_NONE);
//abilita il generatore di numeri casuali (RNG).
    rng_enable();
//infine abilita USART
    usart_enable(USART2);
}

void gpio_setup(void) {
  // Configura i pin GPIO2 e GPIO3 in modalità alternata per poter comunicare in seriale

    gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO2 | GPIO3);

    gpio_set_af(GPIOA, GPIO_AF7, GPIO2 | GPIO3);
}

void testing_gpio_setup(void) {
    // Configura GPIOB, GPIO6 per l'output e lo azzera, questo pin serve per testare un attacco side channel

    rcc_periph_clock_enable(RCC_GPIOB);

    gpio_mode_setup(GPIOB, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO6);
// Questa riga azzera (imposta a LOW) il pin GPIO6 su GPIOB.
    gpio_clear(GPIOB, GPIO6);
}

void read_string(char* buffer, int max_len) {
  // Legge una stringa da USART fino a quando non riceve un carattere di nuova riga, e poi reinvia il carattere per mostrare cosa si sta scrivendo
    int i;
    for (i = 0; i < max_len - 1; i++) {
        while (!usart_get_flag(USART2, USART_FLAG_RXNE));
        buffer[i] = usart_recv(USART2);
        if (buffer[i] == '\r' || buffer[i]=='\n' ) {
            buffer[i] = '\0';
            break;
        }
        //usart_send_blocking(USART2, buffer[i]); 
    }
    usart_send_blocking(USART2, '\n'); 
    usart_send_blocking(USART2, '\r');
}

void send_string(const char* buffer) {
  // Invia una stringa tramite USART, aggiungendo una nuova riga ogni 32 caratteri
    for (int i = 0; buffer[i] != '\0'; i++) {
        
        if(i%32==0){
            usart_send_blocking(USART2, '\n');
            usart_send_blocking(USART2, '\r');
            
        }
        usart_send_blocking(USART2, buffer[i]);

    }
    usart_send_blocking(USART2, '\n');
    usart_send_blocking(USART2, '\r');


}

void print_welcome_message(void) {
    char *welcome_message =
        "Welcome to:\n\n\r"
        ".d8888. db   db  .d8b.       d8888b.                  d88888b d8b   db  .o88b.  .d88b.  d8888b. d88888b d8888b. \n\r"
        "88'  YP 88   88 d8' `8b      VP  `8D                  88'     888o  88 d8P  Y8 .8P  Y8. 88  `8D 88'     88  `8D \n\r"
        "`8bo.   88ooo88 88ooo88        oooY'                  88ooooo 88V8o 88 8P      88    88 88   88 88ooooo 88oobY' \n\r"
        "  `Y8b. 88~~~88 88~~~88        ~~~b.      C8888D      88~~~~~ 88 V8o88 8b      88    88 88   88 88~~~~~ 88`8b   \n\r"
        "db   8D 88   88 88   88      db   8D                  88.     88  V888 Y8b  d8 `8b  d8' 88  .8D 88.     88 `88. \n\r"
        "`8888Y' YP   YP YP   YP      Y8888P'                  Y88888P VP   V8P  `Y88P'  `Y88P'  Y8888D' Y88888P 88   YD \n\n\r"
        "Usage:\n\r"
        "sha3_256 - Encode your messages with sha3-256!\n\r"
        "sha3_512 - Encode your messages with sha3-512!\n\r"
        "secret   - Print a sha3-256 encoded secret, and ask you to guess it :)\n\r";

    for (int i = 0; welcome_message[i] != '\0'; ++i) {
        usart_send_blocking(USART2, welcome_message[i]);
    }
}



int read_mode_from_usart(void) {
    char buffer[10];
    int i = 0;
    char c;

    read_string(buffer,10);


    if (strcmp(buffer, MODE_1) == 0) {
        mode = 1;
        return 1;
    } else if (strcmp(buffer, MODE_2) == 0) {
        mode = 2;
        return 1;
    } else if (strcmp(buffer, MODE_3) == 0) {
        mode = 3;
        return 1;
    } else if (strcmp(buffer, MODE_4) == 0) {
        mode = 3;
        return 1;
    } else if (strcmp(buffer, MODE_5) == 0) {
        mode = 3;
        return 1;
    }
    return 0;
}


int main(void) {
  // Configura l'hardware, poi entra in un loop che stampa il messaggio di benvenuto, legge la modalità e gestisce la logica di crittografia in base alla modalità selezionata

    char input_buffer[MAX_STR_LEN];

    unsigned char output_256[32];
    unsigned char output_512[64];
    unsigned char output_shake[outlen_shake];


    char output_buffer_256[256+1];
    char output_buffer_512[512+1];
    char output_buffer_shake[8*outlen_shake+1];

    char* secret= "C4n_Y0u_PwN_ISW_ScH3m3_?\0";
    char secret_256 [32]; 
    char secret_buffer_256 [256+1];
    char* win= "No way, tell me the truth, did you reverse the executable?\0";
    char* ops= "ahahah noob!\0";

    clock_setup();
    gpio_setup();
    usart_setup();
    testing_gpio_setup();
    int flag=0;

    do{
        print_welcome_message();
        flag=read_mode_from_usart();
        if(flag && mode ==3){
            gpio_set(GPIOB, GPIO6);
            sha3_256(secret_256,secret,strlen(secret));
            gpio_clear(GPIOB, GPIO6);
            for (int i = 0, k=0; i < 32; i++) {
                for (int j = 7; j >= 0; j--) {
                  if (secret_256[i] & (1 << j)) {
                    secret_buffer_256[k]='1';
                  } else {
                    secret_buffer_256[k]='0';
                  }
                  k++;
                }
            }
            secret_buffer_256[256]='\0';
            send_string(secret_buffer_256);

            read_string(input_buffer, MAX_STR_LEN);


            sha3_256(output_256,input_buffer,strlen(input_buffer));
            for (int i = 0,k=0; i < 32; i++) {
                for (int j = 7; j >= 0; j--) {
                  if (output_256[i] & (1 << j)) {
                    output_buffer_256[k]='1';
                  } else {
                    output_buffer_256[k]='0';
                  }
                  k++;
                }
            }
           
          output_buffer_256[256]='\0';
          if(strcmp(output_buffer_256,secret_buffer_256)==0){
            send_string(win); 
            return 0;
          }else{
            send_string(ops); 
            flag=0;
          }
        }
    }while(!flag);

    

    while (1) {
        read_string(input_buffer, MAX_STR_LEN);
        


    

        if (mode == 1){
          gpio_set(GPIOB, GPIO6);
          sha3_256(output_256,input_buffer,strlen(input_buffer));
          //send_string(input_buffer);
          gpio_clear(GPIOB, GPIO6);
          for (int i = 0,k=0; i < 32; i++) {

            for (int j = 7; j >= 0; j--) {
              if (output_256[i] & (1 << j)) {
                output_buffer_256[k]='1';
              } else {
                output_buffer_256[k]='0';
              }
              k++;
            }
          }
          output_buffer_256[256]='\0';

          send_string(output_buffer_256); 
        }
        else if(mode == 2){
          gpio_set(GPIOB, GPIO6);
          sha3_512(output_512,input_buffer,strlen(input_buffer));
          gpio_clear(GPIOB, GPIO6);
          for (int i = 0,k=0; i < 64; i++) {
            for (int j = 7; j >= 0; j--) {
              if (output_512[i] & (1 << j)) {
                output_buffer_512[k]='1';
              } else {
                output_buffer_512[k]='0';
              }
              k++;
            }
          }
          output_buffer_512[512]='\0';
          send_string(output_buffer_512);
        }
        else if(mode == 4){
          gpio_set(GPIOB, GPIO6);
          shake128(output_shake,outlen_shake,input_buffer,strlen(input_buffer));
          gpio_clear(GPIOB, GPIO6);
          for (int i = 0,k=0; i < outlen_shake; i++) {
            for (int j = 7; j >= 0; j--) {
              if (output_shake[i] & (1 << j)) {
                output_buffer_shake[k]='1';
              } else {
                output_buffer_shake[k]='0';
              }
              k++;
            }
          }
          output_buffer_shake[outlen_shake*8]='\0';
          send_string(output_buffer_shake);
        }
        else if(mode == 5){
          gpio_set(GPIOB, GPIO6);
          shake256(output_shake,outlen_shake,input_buffer,strlen(input_buffer));
          gpio_clear(GPIOB, GPIO6);
          for (int i = 0,k=0; i < outlen_shake; i++) {
            for (int j = 7; j >= 0; j--) {
              if (output_shake[i] & (1 << j)) {
                output_buffer_shake[k]='1';
              } else {
                output_buffer_shake[k]='0';
              }
              k++;
            }
          }
          output_buffer_shake[outlen_shake*8]='\0';
          send_string(output_buffer_shake);
        }

        
        
    }

    return 0;
}
