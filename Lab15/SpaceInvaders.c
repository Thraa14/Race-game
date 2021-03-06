#include "..//tm4c123gh6pm.h"
#include "Nokia5110.h"
#include "Random.h"
#include "TExaS.h"
#include "time.h"
#include "stdlib.h"

void PortF_Init(void);
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
void Delay100ms(unsigned long count); // time delay in 0.1 seconds
unsigned long TimerCount;
unsigned long Semaphore;
void MyCarMov(void);
void EnemyMov(void);

// Images

const unsigned char MyCar[] ={

 0x42, 0x4D, 0xC6, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x76, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x0A, 0x00, 0x00, 0x00, 0x01, 0x00, 0x04, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80,
 0x00, 0x00, 0x00, 0x80, 0x80, 0x00, 0x80, 0x00, 0x00, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x80, 0x00, 0x00, 0x80, 0x80, 0x80, 0x00, 0xC0, 0xC0, 0xC0, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF,
 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x0F, 0xF0, 0x00, 0xFF, 0xFF,
 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x0F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x0F, 0xFF, 0x00, 0x0F, 0xFF, 0x0F, 0xFF, 0xFF, 0x0F, 0xFF, 0x00, 0x0F, 0xF0, 0xF0, 0xFF, 0x00, 0x0F, 0xFF,
 0x00, 0x0F, 0xF0, 0xF0, 0xFF, 0x00, 0x0F, 0xFF, 0x00, 0x0F, 0xFF, 0x0F, 0xFF, 0xFF, 0x0F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFF,
 0x00, 0x00, 0x00, 0x0F, 0xF0, 0x00, 0xFF,
};




const unsigned char EnemyCar[] ={
 0x42, 0x4D, 0xC6, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x76, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x0A, 0x00, 0x00, 0x00, 0x01, 0x00, 0x04, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80,
 0x00, 0x00, 0x00, 0x80, 0x80, 0x00, 0x80, 0x00, 0x00, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x80, 0x00, 0x00, 0x80, 0x80, 0x80, 0x00, 0xC0, 0xC0, 0xC0, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF,
 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xF0,
 0x00, 0x00, 0x00, 0x00, 0x0F, 0xF0, 0x00, 0x0F, 0xF0, 0x0F, 0xF0, 0x0F, 0xF0, 0x00, 0x00, 0x00, 0xFF, 0xF0, 0x0F, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFF,
 0xF0, 0x0F, 0xF0, 0x0F, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x0F, 0xFF, 0xFF, 0xFF, 0xFF, 0xF0, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF
};





// image dimensions 

#define MyCarX        ((unsigned char)MyCar[18])
#define MyCarY        ((unsigned char)MyCar[22])
#define EnemyCarX        ((unsigned char)EnemyCar[18])
#define EnemyCarY        ((unsigned char)EnemyCar[22])


// Variables 
int carX = 0, carY;			  //player position(x, y)  
int EnemyCarY1 ,EnemyCarMove, EnemyCarMove1;		//enemies position and movement
int screen_width = 84, screenH = 47;						//screen diminssions
int tracks[3] = {28, 11, 45};										//tracks positions
int fSW1 = 0, fSW2 = 0;									//flags for 2 switches 
int score;

int main(void){
	
	//////////////// initializations \\\\\\\\\\\\\\\\\\\

  TExaS_Init(SSI0_Real_Nokia5110_Scope);  // set system clock to 80 MHz
  Random_Init(1);
  Nokia5110_Init();
  Nokia5110_ClearBuffer();
	Nokia5110_DisplayBuffer();      // draw buffer
	
	PortF_Init();
	carY = 28;		//mid screean y = 28
	EnemyCarMove = 84-EnemyCarX;
	EnemyCarMove1 = 84-EnemyCarX;
	EnemyCarY1 = rand()%3;
	score = 0;
	
	//first screen
	
	Nokia5110_Clear();

  Nokia5110_SetCursor(2, 1);
  Nokia5110_OutString("Race Game");
  Nokia5110_SetCursor(2, 3);
  Nokia5110_OutString("To Start");
  Nokia5110_SetCursor(2, 5);
  Nokia5110_OutString("Press Sw1");
	
	// end  first sceen 
	
	// Game begin 
  
	while(1){
		if(((GPIO_PORTF_DATA_R&(1<<4)) == 0)) // start game only when switch 1 is on  
			break;
  }
	

	Delay100ms(50);  // Delay before start  

	MyCarMov();
	
	Delay100ms(50);	//Delay befor Game over screen

	
	// Game over screen 
	
  Nokia5110_Clear();
  Nokia5110_SetCursor(1, 0);
  Nokia5110_OutString("GAME OVER");
  Nokia5110_SetCursor(1, 2);
  Nokia5110_OutString("Don't try   again xD");
	Nokia5110_SetCursor(1, 5);
  Nokia5110_OutString("Score:");
	Nokia5110_SetCursor(6, 5);
  Nokia5110_OutUDec(score);
	
	while(1){
  }

}  //end of main

void PortF_Init(void){ volatile unsigned long delay;
  SYSCTL_RCGC2_R |= 0x00000020;     // 1) F clock
  delay = SYSCTL_RCGC2_R;           // delay   
  GPIO_PORTF_LOCK_R = 0x4C4F434B;   // 2) unlock PortF PF0  
  GPIO_PORTF_CR_R = 0x1F;           // allow changes to PF4-0       
  GPIO_PORTF_AMSEL_R = 0x00;        // 3) disable analog function
  GPIO_PORTF_PCTL_R = 0x00000000;   // 4) GPIO clear bit PCTL  
  GPIO_PORTF_DIR_R = 0x0E;          // 5) PF4,PF0 input, PF3,PfSW2,PfSW1 output   
  GPIO_PORTF_AFSEL_R = 0x00;        // 6) no alternate function
  GPIO_PORTF_PUR_R = 0x11;          // enable pullup resistors on PF4,PF0       
  GPIO_PORTF_DEN_R = 0x1F;          // 7) enable digital pins PF4-PF0        
}
void MyCarMov( void ){
while(1){
		Nokia5110_ClearBuffer();		
			
		if(!(((GPIO_PORTF_DATA_R&(1<<4)) == 0) && ((GPIO_PORTF_DATA_R&0x1) == 0))){
			if(((GPIO_PORTF_DATA_R&(1<<4)) == 0)){ // if switch 1 is pressed 
				if((carY > 11) && (fSW1 == 0)){
					carY -= 17;
					fSW1 = 1;
				}
			}
			else{
				fSW1 = 0;
			}

			if(((GPIO_PORTF_DATA_R&0x1) == 0)){ // if switch 2 is pressed 
				if((carY < 45) && (fSW2 == 0)){
					carY += 17;
					fSW2 = 1;
				}
			}
			else{
				fSW2 = 0;
			}
		}
		
		Nokia5110_PrintBMP(carX, carY, MyCar, 0);
		
		
		EnemyMov();
		
		if(EnemyCarMove1 <= carX+MyCarX && tracks[EnemyCarY1] == carY){
			break;
		}
			
}

}


void EnemyMov(void){
	
if(EnemyCarMove != 0){
			Nokia5110_PrintBMP(EnemyCarMove, tracks[EnemyCarY1], EnemyCar, 0);
			EnemyCarMove--;
		}
		
		if(EnemyCarMove == 0){
			
			if(EnemyCarMove1 == 0){
				score++;					//number of passed EnemyCars
				EnemyCarY1 = rand()%3;
				EnemyCarMove1 = 84-EnemyCarX;
			}
			Nokia5110_PrintBMP(EnemyCarMove1, tracks[EnemyCarY1], EnemyCar, 0);
		}
		
		
		EnemyCarMove1--; 

		Nokia5110_DisplayBuffer(); //Screen displaying 

		Delay100ms(8);
	} 

void Delay100ms(unsigned long count){unsigned long volatile time;
  while(count>0){
    time = 727240/80;  // 0.1sec at 80 MHz
    while(time){
	  	time--;
    }
    count--;
  }
}