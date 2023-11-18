#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

static struct termios init_setting, new_setting;
char seg_num[10] = {0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xd8, 0x80, 0x90};
char seg_dnum[10] = {0x40, 0x79, 0x24, 0x30, 0x19, 0x12, 0x02, 0x58, 0x00, 0x10};

#define D1 0x01
#define D2 0x02
#define D3 0x04
#define D4 0x08

void init_keyboard()
{
	tcgetattr(STDIN_FILENO, &init_setting);
	new_setting = init_setting;
	new_setting.c_lflag &= ~ICANON;
	new_setting.c_lflag &= ~ECHO;
	new_setting.c_cc[VMIN] = 0;
	new_setting.c_cc[VTIME] = 0;
	tcsetattr(0, TCSANOW, &new_setting);
}

void close_keyboard()
{
	tcsetattr(0, TCSANOW, &init_setting);
}

char get_key()
{
	char ch = -1;
	
	if(read(STDIN_FILENO, &ch, 1) != 1)
		ch = -1;
	return ch;
}

void print_menu()
{
	printf("\n---------menu---------\n");
	printf("[u] : count up\n");
	printf("[d] : count down\n");
	printf("[p] : count setting\n");
	printf("[q] : program exit\n");
	printf("-----------------------\n");
}


int main(int argc, char **argv) {
	unsigned short data[4];
	char key;
	int tmp_n;
	int delay_time;
	int digit4=0,digitmp=0;
	int d1=0,d2=0,d3=0,d4=0;
	int evencheck=0;
	int dev = open("/dev/my_segment", O_RDWR);
	
	char buff,tmp;
	char prev = 'r';
	int dev1 = open("/dev/my_up_button", O_RDWR);
	
	char buff1,tmp1;
	char prev1 = 'r';
	int dev2 = open("/dev/my_down_button",O_RDWR);
	if(dev == -1){
		printf("Opening was not possible!\n");
		return -1;
	}
	printf("device opening was successfull!\n");
	
	if(dev1 == -1){
		printf("Opening was not possible!\n");
		return -1;
	}
	printf("device1 opening was successfull!\n");
	
	if(dev2 == -1){
		printf("Opening was not possible!\n");
		return -1;
	}
	printf("device2 opening was successfull!\n");
	
	init_keyboard();
	print_menu();
	tmp_n=0;
	delay_time = 1000000;
	
	data[0] = (seg_num[0] << 4) | D1;
	data[1] = (seg_num[0] << 4) | D2;
	data[2] = (seg_num[0] << 4) | D3;
	data[3] = (seg_num[0] << 4) | D4;
	
	while(1){
		read(dev1, &buff, 1);
		prev = tmp;
		tmp = buff;
		if(prev != tmp){
			evencheck+=1;
			if((evencheck % 2) == 0){
				digit4 += 1;
				if(digit4==10000){
					digit4 = 0;
				}
				digitmp=digit4;
				d4 = digit4%10;
				digit4 /=10;
				d3 = digit4%10;
				digit4 /=10;
				d2 = digit4%10;
				digit4 /=10;
				d1 = digit4%10;
				data[0] = (seg_num[d1] << 4) | D1;
				data[1] = (seg_num[d2] << 4) | D2;
				data[2] = (seg_num[d3] << 4) | D3;
				data[3] = (seg_num[d4] << 4) | D4;	
				digit4 = digitmp;
				evencheck = 0;
			}	
		}
		read(dev2, &buff1, 1);
		prev1 = tmp1;
		tmp1 = buff1;
		if(prev1 != tmp1){
			evencheck+=1;
			if((evencheck % 2) == 0){
				digit4 -= 1;
				if(digit4==-1){
					digit4 = 9999;
				}
				digitmp=digit4;
				d4 = digit4%10;
				digit4 /=10;
				d3 = digit4%10;
				digit4 /=10;
				d2 = digit4%10;
				digit4 /=10;
				d1 = digit4%10;
				data[0] = (seg_num[d1] << 4) | D1;
				data[1] = (seg_num[d2] << 4) | D2;
				data[2] = (seg_num[d3] << 4) | D3;
				data[3] = (seg_num[d4] << 4) | D4;	
				digit4 = digitmp;
				evencheck = 0;
			}	
		}
		key = get_key();
		if(key == 'q'){
			printf("exit this program.\n");
			break;
		}
		else if(key == 'p'){
			delay_time = 1000000;
			tmp_n=0;
			data[0] = (seg_num[0] << 4) | D1;
			data[1] = (seg_num[0] << 4) | D2;
			data[2] = (seg_num[0] << 4) | D3;
			data[3] = (seg_num[0] << 4) | D4;	
		}
		else if(key == 'u'){
			digit4 += 1;
			if(digit4==10000){
				digit4 = 0;
			}
			digitmp=digit4;
			d4 = digit4%10;
			digit4 /=10;
			d3 = digit4%10;
			digit4 /=10;
			d2 = digit4%10;
			digit4 /=10;
			d1 = digit4%10;
			data[0] = (seg_num[d1] << 4) | D1;
			data[1] = (seg_num[d2] << 4) | D2;
			data[2] = (seg_num[d3] << 4) | D3;
			data[3] = (seg_num[d4] << 4) | D4;	
			digit4 = digitmp;		
		}
		else if(key == 'd'){
			digit4 -= 1;
			if(digit4==-1){
				digit4 = 9999;
			}
			digitmp=digit4;
			d4 = digit4%10;
			digit4 /=10;
			d3 = digit4%10;
			digit4 /=10;
			d2 = digit4%10;
			digit4 /=10;
			d1 = digit4%10;
			data[0] = (seg_num[d1] << 4) | D1;
			data[1] = (seg_num[d2] << 4) | D2;
			data[2] = (seg_num[d3] << 4) | D3;
			data[3] = (seg_num[d4] << 4) | D4;	
			digit4 = digitmp;	
		}
		write(dev, &data[tmp_n], 2);
		usleep(delay_time);
		
		tmp_n++;
		if(tmp_n>3){
			tmp_n = 0;
			if(delay_time > 5000){
				delay_time /= 2;
			}
		}
	}
	
	close_keyboard();
	write(dev, 0x0000, 2);
	close(dev);
	close(dev1);
	close(dev2);
	return 0;
}
