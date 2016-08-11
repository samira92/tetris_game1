#include <avr/io.h>
#include <util/delay.h> 
#include <stdlib.h>

#define plusA PORTC
#define plusB PORTA
#define ground PORTB
#define disp_delay 1
#define btn_left (1<<PD1)
#define btn_right (1<<PD2)
#define btn_rot (1<<PD3)
#define btn_down (1<<PD0)
unsigned char blocks[48]={	0b11000000,
							0b11000000,
							0b00000000,
							0b11000000,
							0b11000000,
							0b00000000,
							0b11000000,
							0b11000000,
							0b00000000,
							0b11000000,
							0b11000000,
							0b00000000,
							0b11100000,
							0b00000000,
							0b00000000,
							0b10000000,
							0b10000000,
							0b10000000,
							0b11100000,
							0b00000000,
							0b00000000,
							0b10000000,
							0b10000000,
							0b10000000,
							0b11100000,
							0b01000000,
							0b00000000,
							0b10000000,
							0b11000000,
							0b10000000,
							0b01000000,
							0b11100000,
							0b00000000,
							0b00100000,
							0b01100000,
							0b00100000,
							0b01000000,
							0b01000000,
							0b01100000,
							0b01110000,
							0b01000000,
							0b00000000,
							0b01100000,
							0b01000000,
							0b01000000,
							0b00010000,
							0b01110000,
							0b00000000};
							
unsigned char area[17]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff};
unsigned int speed = 30;
unsigned int flag_shape=0;
unsigned int flag_rot=1;
unsigned int flag_push=2;
unsigned int flag_btn=0;
void display (unsigned int h);
void button (void);
void area_check(void);
void clear_line(void);
void change_block(void);
unsigned int stop_block(unsigned int h);
void game_over(void);
int main (void){
	DDRD&=~(btn_left|btn_right|btn_down|btn_rot);
	DDRA=0xff;
	DDRB=0xff;
	DDRC=0xff;
	while(1){
		next:
		game_over();
		change_block();
		clear_line();
		speed=30;
		for(unsigned int height=0;height!=16;height++){
			for(unsigned int ref=0;ref!=speed;ref++){
				display(height);
			}
			if(stop_block(height)){
				goto next;
			}
		}
	}
}

void display (unsigned int h){
for(unsigned int i=0;i!=16;i++){
		for(unsigned int r=0;r!=3;r++){
			if(i==h+r){
				ground =~(blocks[12*(flag_shape)+3*(flag_rot)+r]>>flag_push);
			}
		}
		ground &=~(area[i]);
		if(i/8==0){
			plusB=0x00;
			plusA=(1<<i);
		}
		else{
			plusA=0x00;
			plusB=(1<<i%8);
		}
		button();
		_delay_ms(disp_delay);
		ground=0xff;
	}
	//ground=0xff;
}
void button (void){
	if((PIND&btn_left)){
	if(flag_btn==0){
		if(flag_push!=5)
		flag_push=flag_push+1;
		flag_btn=1;
		}
	}
	else if((PIND&btn_right)){
	if(flag_btn==0){
		if(flag_push!=0)
		flag_push=flag_push-1;
		flag_btn=1;
		}
	}
	else if((PIND&btn_down)){
	if(flag_btn==0){
		speed=10;
		flag_btn=1;
		}
	}
	else if((PIND&btn_rot)){
	if(flag_btn==0){
		flag_rot=(flag_rot+1)%4;
		flag_btn=1;
		}
	}
	else{
		flag_btn=0;
	}
}
void clear_line(void){
	for(unsigned int l=0;l!=16;l++){
		if(area[l]==0xff){
			while(l){
				area[l]=area[l-1];
				l--;
			}
		}
	}
}

void change_block(void){
	int r = rand() % 4;
	flag_shape=r;
	flag_rot=(flag_rot+13)%4;
}

unsigned int stop_block(unsigned int h){
	unsigned int skip=0;
	for(unsigned int k=3;k!=0;k--){
		if(area[h+1]&(blocks[12*(flag_shape)+3*(flag_rot)+k-1]>>flag_push)){
			for(unsigned int a=0;a!=3;a++){
				area[h-a]|=(blocks[12*(flag_shape)+3*(flag_rot)+a]>>flag_push);
			}
			skip=1;
		}
	}
	return skip;
}
void game_over(void){
	if(area[0]!=0x00){
		for(unsigned int j=0;j!=16;j++){
			area[j]=0x00;
		}
	_delay_ms(5000);
	}
}