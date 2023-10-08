void read();
void operate(int);
void bottle_status();
int __divsi3(int , int );
double __floatsidf(int );
double __divdf3(double , double );
int __fixdfsi(double );
double __muldf3(double , double);

int ir_sen_ip;
int solenoid_valve_op = 0;
int flow_sensor_ip;
int led_op = 0;
int buzzer = 0;
int counter_h = 0;
int counter_l = 0;
int reset_button = 0;
int water_used=0;


int main()
{ 
    int solenoid_reg, led_reg, buzzer_reg;
    solenoid_reg = solenoid_valve_op*4;
    led_reg = led_op*8;
    buzzer_reg = buzzer*16;
    asm volatile(
	"or x30, x30, %0\n\t"
	"or x30, x30, %1\n\t"
	"or x30, x30, %2\n\t" 
	:
	:"r"(solenoid_reg),"r"(led_reg),"r"(buzzer_reg)
	:"x30"
	);
		
    while(1){
    //reset = digital_read(0);
    	asm volatile(
	"andi %0, x30, 1\n\t"
	:"=r"(reset_button)
	:
	:
	);
    if(reset_button)
    {
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
	"or x30, x30,%0 \n\t"
	"or x30, x30,%1 \n\t"
	"or x30, x30,%2 \n\t" 
	:
	:"r"(solenoid_reg),"r"(led_reg),"r"(buzzer_reg)
	:"x30"
	);
    }
    else
    {
    while(!led_op && !buzzer && !reset_button){
        read();

    }
    }
    }
    return 0;
}

// Implementation for __muldf3
double __muldf3(double a, double b) {
    // Your implementation here
    return a * b;
}



void read()
{
   int ir_sen_ip_reg;
    //ir_sen_ip = digital_read(1);
    asm volatile(
	"andi %0, x30, 2\n\t"
	:"=r"(ir_sen_ip_reg)
	:
	:
	);
    ir_sen_ip = ir_sen_ip_reg>>1;
    operate(ir_sen_ip);
}

void operate(int ir_value)
{
    int solenoid_reg;
    if(ir_value)
    {
    	
        solenoid_valve_op = 1;
        solenoid_reg = solenoid_valve_op*4;
        asm volatile(
	"or x30, x30,%0 \n\t"
	:
	:"r"(solenoid_reg)
	:"x30"
	);
        //digital_write(4,solenoid_valve_op);
        counter_h++;
        bottle_status();
        

    }
    else {
        solenoid_valve_op = 0;
        //digital_write(4,solenoid_valve_op);
        solenoid_reg = solenoid_valve_op*4;
        asm volatile(
	"or x30, x30,%0 \n\t"
	:
	:"r"(solenoid_reg)
	:"x30"
	);
        counter_l++;
    }

}

// Implementations for missing functions
int __divsi3(int a, int b) {
    // Your implementation here
    return a / b;
}

double __floatsidf(int a) {
    // Your implementation here
    return (double)a;
}

double __divdf3(double a, double b) {
    // Your implementation here
    return a / b;
}

int __fixdfsi(double a) {
    // Your implementation here
    return (int)a;
}


void bottle_status()
{
    int time = counter_h + counter_l;
    int freq = 1000000>>time;
    int water = freq*0.13;
    int ls = water * 16.67;
    int led_reg;
    int buzzer_reg;
    water_used = water_used+ls;
    if(ls==500)
    {
        led_op = 1;
        buzzer = 1;   
        //digital_write(5,led_op);
        //digital_write(6,buzzer);
        led_reg = led_op*8;
        buzzer_reg = buzzer*16;
        
        asm volatile(
	"or x30, x30, %0\n\t"
	"or x30, x30, %1\n\t" 
	:
	:"r"(led_reg),	"r"(buzzer_reg)
	:"x30"
	);
        
        
    }
}

