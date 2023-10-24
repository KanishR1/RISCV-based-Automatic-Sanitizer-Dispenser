/*
#include<stdio.h>
#include<stdlib.h>
*/
int main()
{ 
    //int printer,printer1;
    
    // Variable Declaration
    int ir_sen_ip_reg;
    int mask1 = 0xFFFFFFE3;
    int mask2 = 0xFFFFFFFB;
    int ir_sen_ip;//=1;//=1;
    int solenoid_valve_op=0; //= 1;
    int led_op = 0;
    int buzzer = 0;
    int counter_h = 0;
    int counter_l = 0;
    int reset_button = 0;
    int water_used=0;
    int solenoid_reg, led_reg, buzzer_reg;
    solenoid_reg = solenoid_valve_op*4;
    led_reg = led_op*8;
    buzzer_reg = buzzer*16;
    
    /*
    asm volatile(
    	"addi x30, x30, 1\n\t"
    	:
    	:
    	:"x30"
    	);
    */
    
    // Intialize the registers 
    asm volatile(
	"and x30, x30, %3\n\t"
	"or x30, x30, %0\n\t"
	"or x30, x30, %1\n\t"
	"or x30, x30, %2\n\t" 
	:
	:"r"(solenoid_reg),"r"(led_reg),"r"(buzzer_reg),"r"(mask1)
	:"x30"
	);

    // Actual Program execution brgins here
		
    while(1){
    //reset = digital_read(0);
    
     //Read Reset button input
    	asm volatile(
	"andi %0, x30, 1\n\t"
	:"=r"(reset_button)
	:
	:"x30"
	);
	
	/*
	asm volatile(
    	"addi %0, x30, 0\n\t"
    	:"=r"(printer)
    	:
    	:"x30"
    	);
    	printf("Printer = %d\n",printer);
	//printf("Inside while 1\n");
	//printf("Reset Button_val=%d\n",reset_button);
	*/
    if(reset_button)
    {
    	// printf("reset_cond\n");
        //break;
        solenoid_valve_op = 0;
        led_op = 0;
        buzzer = 0;
        counter_h = 0;
        counter_l = 0;
        water_used = 0;
        //digital_write(4,solenoid_valve_op);
        //digital_write(5,led_op);
        //digital_write(6,buzzer);
        solenoid_reg = solenoid_valve_op*4;
        led_reg = led_op*8;
        buzzer_reg = buzzer*16;
        asm volatile(
        "and x30, x30, %3\n\t"
	"or x30, x30,%0 \n\t"
	"or x30, x30,%1 \n\t"
	"or x30, x30,%2 \n\t" 
	:
	:"r"(solenoid_reg),"r"(led_reg),"r"(buzzer_reg),"r"(mask1)
	:"x30"
	);
	
	/*
	asm volatile(
    	"addi %0, x30, 0\n\t"
    	:"=r"(printer1)
    	:
    	:"x30"
    	);
    	printf("Printer1 = %d\n",printer1);
    	*/
    }
    else
    {
    /*
    asm volatile(
    	"addi %0, x30, 0\n\t"
    	:"=r"(printer1)
    	:
    	:"x30"
    	);
    	printf("Printer1 = %d\n",printer1);
    printf("Led_val = %d, Buzzer_val = %d, Reset_button = %d, IR_sensp = %d, solenoid_valve_op=%d\n",led_op,buzzer,reset_button,ir_sen_ip,solenoid_valve_op);
    //break;
    //if(!led_op)
    //	{printf("Hi"); break;}
    */
     	
    if(!led_op && !buzzer && !reset_button){
     //read();
      /*
       asm volatile(
    	"addi %0, x30, 0\n\t"
    	:"=r"(printer)
    	:
    	:"x30"
    	);
    	printf("Printer = %d\n",printer);
        // = 2;
    //ir_sen_ip = digital_read(1);
       */
        asm volatile(
	"andi %0, x30, 2\n\t"
	:"=r"(ir_sen_ip_reg)
	:
	:"x30"
	);
	
    
    ir_sen_ip = ir_sen_ip_reg>>1;
    // printf("Led_val = %d, Buzzer_val = %d, Reset_button = %d, IR_sensp = %d, solenoid_valve_op=%d\n",led_op,buzzer,reset_button,ir_sen_ip,solenoid_valve_op);
    // break;
    if(ir_sen_ip)
    {
    	// printf("got ir_sens\n");
    	
        solenoid_valve_op = 1;
        solenoid_reg = solenoid_valve_op*4;
        asm volatile(
	"and x30, x30, %1\n\t"
	"or x30, x30,%0 \n\t"
	:
	:"r"(solenoid_reg),"r"(mask2)
	:"x30"
	);
        //digital_write(4,solenoid_valve_op);
        counter_h++;
        //bottle_status();
        int time = counter_h + counter_l;
    	int freq = 0;
    	int dividend = 100, divisor = time;
    	while (dividend >= divisor) {
        dividend -= divisor;
        freq++;
    	}
       	int water = freq;
    	int ls = water * 17;
    	//int led_reg;
    	//int buzzer_reg;
    	water_used = water_used+ls;
    	if(ls>102)
   	{
   	//printf("Inside ls if\n");
        // break;
        led_op = 1;
        buzzer = 1;   
        solenoid_valve_op=0;
      //  break;
        //digital_write(5,led_op);
        //digital_write(6,buzzer);
        led_reg = led_op*8;
        buzzer_reg = buzzer*16;
        solenoid_reg = solenoid_valve_op*4;
        
        asm volatile(
        "and x30, x30, %3\n\t"
	"or x30, x30, %0\n\t"
	"or x30, x30, %1\n\t" 
	"or x30, x30, %2 \n\t"
	:
	:"r"(led_reg),	"r"(buzzer_reg), "r"(solenoid_reg),"r"(mask1) 
	:"x30"
	);
        
    }
        

    }
    else {
    	//printf("ir_sens not_recieved\n");
        solenoid_valve_op = 0;
        //digital_write(4,solenoid_valve_op);
        solenoid_reg = solenoid_valve_op*4;
        asm volatile(
        "and x30, x30, %1\n\t"
	"or x30, x30,%0 \n\t"
	:
	:"r"(solenoid_reg),"r"(mask2)
	:"x30"
	);
        counter_l++;
    }

}
}
}

return 0;
}

