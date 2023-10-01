#include<stdio.h>

void read();
void operate(int);
void bottle_status();

int ir_sen_ip=1;
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
    while(!led_op && !buzzer && !reset_button){
        read();

    }
    if(reset_button)
    {
        solenoid_valve_op = 0;
        led_op = 0;
        buzzer = 0;
        counter_h = 0;
        counter_l = 0;
        water_used = 0;
    }
    return 0;
}


void read()
{
    operate(ir_sen_ip);
}

void operate(int ir_value)
{
    if(ir_value)
    {
        solenoid_valve_op = 1;
        counter_h++;
        printf("Solenoid Status ON \n");
        bottle_status();
        

    }
    else {
        solenoid_valve_op = 0;
        counter_l++;
    }

}

void bottle_status()
{
    int time = counter_h + counter_l;
    float freq = 1000000/time;
    float water = freq/7.5;
    float ls = water/60;
    water_used = water_used+ls;
    if(ls==0.5)
    {
        led_op = 1;
        buzzer = 1;
        printf("Bottle Empty\n");
    }
}
