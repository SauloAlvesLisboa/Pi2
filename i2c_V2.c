#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <unistd.h>
#include <stddef.h>
#include <string.h>

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
        int temp_quente,temp_fria,rot,vbat,vgerador,ibat;

        signal(SIGINT, ctrl_c);
        i2c_fd = open("/dev/i2c-1", O_RDWR);
        ioctl(i2c_fd, I2C_SLAVE, slave_addr);

        int i=0;
        //while(i==0){
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
                printf("Pedindo valor do sensor %d\n",user_input);
                
                write(i2c_fd, &user_input, 1);
                sleep(0.5);
                read(i2c_fd, &msp430_ret,1);
                sleep(0.5);
                printf("msp retornou = %d\n \n", msp430_ret);

                if (cont==1){
                        temp_quente=msp430_ret;
                        printf("Valor do sensor em A5 é: %d\n", msp430_ret);
                }
                else if (cont==2){
                        temp_fria=msp430_ret;
                        printf("Valor do sensor em A4 é: %d\n", msp430_ret);
                }
                else if (cont==3){
                        rot=msp430_ret;
                        printf("Valor do sensor em A3: %d\n", msp430_ret);
                }
                else if (cont==4){
                        vbat=msp430_ret;
                        printf("Valopr do sensor 4 é %d\n", msp430_ret);
                }
                else if (cont==5){
                        vgerador=msp430_ret;
                        printf("Valopr do sensor 4 é %d\n", msp430_ret);
                }
                else if (cont==6){
                        ibat=msp430_ret;
                        printf("Valopr do sensor 4 é %d\n", msp430_ret);
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
        //}
        close(i2c_fd);

        FILE * fp;
        //int x[5] = {0,1,2,5,5};
        fp = fopen ("file.txt", "w+");
        //char c[] = "oi";
        //fputc(x,fp);
        fprintf(fp, "%d,%d,%d,",temp_quente,temp_fria,rot);
        fclose(fp);

        char command[50];

        strcpy(command, "python3 read_gps.py" );
        system(command);
        return (0);
}

