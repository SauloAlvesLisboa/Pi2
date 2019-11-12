#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <unistd.h>

int i2c_fd;
void ctrl_c(int sig)
{
        close(i2c_fd);
        exit(-1);
}

int main(void)
{
        char user_input=1, msp430_ret, slave_addr=0x0F;
        unsigned int c=0;
        signal(SIGINT, ctrl_c);
        i2c_fd = open("/dev/i2c-1", O_RDWR);
        ioctl(i2c_fd, I2C_SLAVE, slave_addr);

        //while(user_input!=0)
        for(int cont=1;cont<=3;cont++)
        {
                //puts("Digite um numero entre 1 e 5");
                //puts("para mandar o MSP430 piscar seus LEDs,");
                //puts("ou digite 0 para terminar o programa. ");
                //scanf("%d", &user_input);
                //c=1;
                //if(c==0){
                //      puts("enviando atravez do if c para a msp");
                  //      write(i2c_fd, &user_input, 1);
                    //    sleep(3);
                      //  read(i2c_fd, &msp430_ret,1);
                       // printf("retorna send = %d\n", msp430_ret);

                user_input = cont;
                printf("enviando para msp, condição %d\n",user_input);

                write(i2c_fd, &user_input, 1);
                sleep(0.5);
                read(i2c_fd, &msp430_ret,1);

                printf("msp retornou (send_data) = %d\n \n", msp430_ret);

                if (cont==1){
                        int temperatura=msp430_ret;
                        printf("Temperatura é: %d\n", msp430_ret);
                }
                //else if((user_input<0) || (user_input>5))
                //      puts("Valor invalido");
                //else if(user_input>0)
                //{
                //      write(i2c_fd, &user_input, 1);
                //      sleep(1+user_input/2);
                //      read(i2c_fd, &msp430_ret, 1);
                //      printf("MSP430_return = %d\n", msp430_ret);
                 //}
                puts("");
                //user_input++;
        }
        close(i2c_fd);
}

                
                
                
