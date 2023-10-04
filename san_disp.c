void read();
void operate(int);
void bottle_status();

int ir_sen_ip;
int solenoid_valve_op = 0;
int flow_sensor_ip;
int led_op = 0;
int buzzer = 0;
int counter_h = 0;
int counter_l = 0;
int reset_button = 0;
float water_used=0;

int main()
{ 
    int solenoid_reg, led_reg, buzzer_reg;
    solenoid_reg = solenoid_valve_op*4;
    led_reg = led_op*8;
    buzzer_reg = buzzer*16;
    asm(
	"or x30, x30, %0\n\t"
	"or x30, x30, %1\n\t"
	"or x30, x30, %2\n\t" 
	:"=r"(solenoid_reg),"=r"(led_reg),"=r"(buzzer_reg));
		
    while(1){
    //reset = digital_read(0);
    	asm(
	"andi %0, x30, 1\n\t"
	:"=r"(reset_button));
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
        asm(
	"or x30, x30,%0 \n\t"
	"or x30, x30,%1 \n\t"
	"or x30, x30,%2 \n\t" 
	:"=r"(solenoid_reg),"=r"(led_reg),"=r"(buzzer_reg));
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


void read()
{
   int ir_sen_ip_reg;
    //ir_sen_ip = digital_read(1);
    asm(
	"andi %0, x30, 2\n\t"
	:"=r"(ir_sen_ip_reg));
    ir_sen_ip = ir_sen_ip_reg/2;
    operate(ir_sen_ip);
}

void operate(int ir_value)
{
    int solenoid_reg;
    if(ir_value)
    {
    	
        solenoid_valve_op = 1;
        solenoid_reg = solenoid_valve_op*4;
        asm(
	"or x30, x30,%0 \n\t"
	:"=r"(solenoid_reg));
        //digital_write(4,solenoid_valve_op);
        counter_h++;
        bottle_status();
        

    }
    else {
        solenoid_valve_op = 0;
        //digital_write(4,solenoid_valve_op);
        solenoid_reg = solenoid_valve_op*4;
        asm(
	"or x30, x30,%0 \n\t"
	:"=r"(solenoid_reg));
        counter_l++;
    }

}

void bottle_status()
{
    int time = counter_h + counter_l;
    float freq = 1000000/time;
    float water = freq/7.5;
    float ls = water/60;
    int led_reg;
    int buzzer_reg;
    water_used = water_used+ls;
    if(ls==0.5)
    {
        led_op = 1;
        buzzer = 1;   
        //digital_write(5,led_op);
        //digital_write(6,buzzer);
        led_reg = led_op*8;
        buzzer_reg = buzzer*16;
        
        asm(
	"or x30, x30, %0\n\t"
	"or x30, x30, %1\n\t" 
	:"=r"(led_reg),	"=r"(buzzer_reg));
        
        
    }
}
