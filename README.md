# RISCV_Automatic-Sanitizer-Dispenser
This github repository summarizes the progress made in the ASIC class for the riscv_project.

## Aim
The project's goal is to design an automatic sanitizer dispenser utilizing a specialized RISCV processor for bottle refill alerts and touch-free sanitizer distribution, with the objective of minimizing the machine's footprint, energy consumption, and overall expenses.

## Working
An infrared sensor is employed to detect the presence of a hand. Upon hand detection, the solenoid valve is activated. The sanitizer is directed through the flow sensor via the open solenoid valve, allowing the flow sensor to calculate the quantity of sanitizer dispensed. The system is configured to accommodate a sanitizer bottle with a capacity of half a liter. Upon detecting the dispensing of half a liter, the flow sensor triggers a buzzer, signaling the need for a sanitizer bottle refill.

## Circuit
This is not the depiction of the final circuit. <br>

![ckt](ckt_depict.png)
## Block Diagram
![blockd](bda.png)

## C Code
```
int main()
{ 
    int ir_sen_ip;
    int solenoid_valve_op = 1;
//int flow_sensor_ip;
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
        //read();
        int ir_sen_ip_reg;
    //ir_sen_ip = digital_read(1);
        asm volatile(
	"andi %0, x30, 2\n\t"
	:"=r"(ir_sen_ip_reg)
	:
	:
	);
    ir_sen_ip = ir_sen_ip_reg>>1;
    if(ir_sen_ip)
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
        //bottle_status();
        int time = counter_h + counter_l;
    	int freq = 0;
    	int dividend = 1000000, divisor = time;
    	while (dividend >= divisor) {
        dividend -= divisor;
        freq++;
    	}
       	int water = freq;
    	int ls = water * 17;
    	//int led_reg;
    	//int buzzer_reg;
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
}
}

return 0;
}
```

## GCC Compiler Working
![gcc](gcc_working.png)
## Assembly code conversion

Compile the c program using RISCV-V GNU Toolchain and dump the assembly code into water_level_assembly.txt using the below commands.

```
riscv32-unkown-elf-gcc -march=rv32i -mabi=ilp32 -ffreestanding  -nostdlib -o ./output.o san_disp.c
riscv32-unknown-elf-objdump -d  -r output.o > assembly.txt
```

## Assembly Code
```

kanish:     file format elf32-littleriscv


Disassembly of section .text:

00010054 <main>:
   10054:	fa010113          	addi	sp,sp,-96
   10058:	04812e23          	sw	s0,92(sp)
   1005c:	06010413          	addi	s0,sp,96
   10060:	00100793          	li	a5,1
   10064:	fcf42823          	sw	a5,-48(s0)
   10068:	fe042623          	sw	zero,-20(s0)
   1006c:	fe042423          	sw	zero,-24(s0)
   10070:	fe042223          	sw	zero,-28(s0)
   10074:	fe042023          	sw	zero,-32(s0)
   10078:	fc042623          	sw	zero,-52(s0)
   1007c:	fc042e23          	sw	zero,-36(s0)
   10080:	fd042783          	lw	a5,-48(s0)
   10084:	00279793          	slli	a5,a5,0x2
   10088:	fcf42423          	sw	a5,-56(s0)
   1008c:	fec42783          	lw	a5,-20(s0)
   10090:	00379793          	slli	a5,a5,0x3
   10094:	fcf42223          	sw	a5,-60(s0)
   10098:	fe842783          	lw	a5,-24(s0)
   1009c:	00479793          	slli	a5,a5,0x4
   100a0:	fcf42023          	sw	a5,-64(s0)
   100a4:	fc842783          	lw	a5,-56(s0)
   100a8:	fc442703          	lw	a4,-60(s0)
   100ac:	fc042683          	lw	a3,-64(s0)
   100b0:	00ff6f33          	or	t5,t5,a5
   100b4:	00ef6f33          	or	t5,t5,a4
   100b8:	00df6f33          	or	t5,t5,a3
   100bc:	001f7793          	andi	a5,t5,1
   100c0:	fcf42623          	sw	a5,-52(s0)
   100c4:	fcc42783          	lw	a5,-52(s0)
   100c8:	18078663          	beqz	a5,10254 <main+0x200>
   100cc:	fc042823          	sw	zero,-48(s0)
   100d0:	fe042623          	sw	zero,-20(s0)
   100d4:	fe042423          	sw	zero,-24(s0)
   100d8:	fe042223          	sw	zero,-28(s0)
   100dc:	fe042023          	sw	zero,-32(s0)
   100e0:	fc042e23          	sw	zero,-36(s0)
   100e4:	fd042783          	lw	a5,-48(s0)
   100e8:	00279793          	slli	a5,a5,0x2
   100ec:	fcf42423          	sw	a5,-56(s0)
   100f0:	fec42783          	lw	a5,-20(s0)
   100f4:	00379793          	slli	a5,a5,0x3
   100f8:	fcf42223          	sw	a5,-60(s0)
   100fc:	fe842783          	lw	a5,-24(s0)
   10100:	00479793          	slli	a5,a5,0x4
   10104:	fcf42023          	sw	a5,-64(s0)
   10108:	fc842783          	lw	a5,-56(s0)
   1010c:	fc442703          	lw	a4,-60(s0)
   10110:	fc042683          	lw	a3,-64(s0)
   10114:	00ff6f33          	or	t5,t5,a5
   10118:	00ef6f33          	or	t5,t5,a4
   1011c:	00df6f33          	or	t5,t5,a3
   10120:	f9dff06f          	j	100bc <main+0x68>
   10124:	002f7793          	andi	a5,t5,2
   10128:	faf42e23          	sw	a5,-68(s0)
   1012c:	fbc42783          	lw	a5,-68(s0)
   10130:	4017d793          	srai	a5,a5,0x1
   10134:	faf42c23          	sw	a5,-72(s0)
   10138:	fb842783          	lw	a5,-72(s0)
   1013c:	0e078a63          	beqz	a5,10230 <main+0x1dc>
   10140:	00100793          	li	a5,1
   10144:	fcf42823          	sw	a5,-48(s0)
   10148:	fd042783          	lw	a5,-48(s0)
   1014c:	00279793          	slli	a5,a5,0x2
   10150:	fcf42423          	sw	a5,-56(s0)
   10154:	fc842783          	lw	a5,-56(s0)
   10158:	00ff6f33          	or	t5,t5,a5
   1015c:	fe442783          	lw	a5,-28(s0)
   10160:	00178793          	addi	a5,a5,1
   10164:	fef42223          	sw	a5,-28(s0)
   10168:	fe442703          	lw	a4,-28(s0)
   1016c:	fe042783          	lw	a5,-32(s0)
   10170:	00f707b3          	add	a5,a4,a5
   10174:	faf42a23          	sw	a5,-76(s0)
   10178:	fc042c23          	sw	zero,-40(s0)
   1017c:	000f47b7          	lui	a5,0xf4
   10180:	24078793          	addi	a5,a5,576 # f4240 <__global_pointer$+0xe27d0>
   10184:	fcf42a23          	sw	a5,-44(s0)
   10188:	fb442783          	lw	a5,-76(s0)
   1018c:	faf42823          	sw	a5,-80(s0)
   10190:	0200006f          	j	101b0 <main+0x15c>
   10194:	fd442703          	lw	a4,-44(s0)
   10198:	fb042783          	lw	a5,-80(s0)
   1019c:	40f707b3          	sub	a5,a4,a5
   101a0:	fcf42a23          	sw	a5,-44(s0)
   101a4:	fd842783          	lw	a5,-40(s0)
   101a8:	00178793          	addi	a5,a5,1
   101ac:	fcf42c23          	sw	a5,-40(s0)
   101b0:	fd442703          	lw	a4,-44(s0)
   101b4:	fb042783          	lw	a5,-80(s0)
   101b8:	fcf75ee3          	bge	a4,a5,10194 <main+0x140>
   101bc:	fd842783          	lw	a5,-40(s0)
   101c0:	faf42623          	sw	a5,-84(s0)
   101c4:	fac42703          	lw	a4,-84(s0)
   101c8:	00070793          	mv	a5,a4
   101cc:	00479793          	slli	a5,a5,0x4
   101d0:	00e787b3          	add	a5,a5,a4
   101d4:	faf42423          	sw	a5,-88(s0)
   101d8:	fdc42703          	lw	a4,-36(s0)
   101dc:	fa842783          	lw	a5,-88(s0)
   101e0:	00f707b3          	add	a5,a4,a5
   101e4:	fcf42e23          	sw	a5,-36(s0)
   101e8:	fa842703          	lw	a4,-88(s0)
   101ec:	1f400793          	li	a5,500
   101f0:	06f71263          	bne	a4,a5,10254 <main+0x200>
   101f4:	00100793          	li	a5,1
   101f8:	fef42623          	sw	a5,-20(s0)
   101fc:	00100793          	li	a5,1
   10200:	fef42423          	sw	a5,-24(s0)
   10204:	fec42783          	lw	a5,-20(s0)
   10208:	00379793          	slli	a5,a5,0x3
   1020c:	fcf42223          	sw	a5,-60(s0)
   10210:	fe842783          	lw	a5,-24(s0)
   10214:	00479793          	slli	a5,a5,0x4
   10218:	fcf42023          	sw	a5,-64(s0)
   1021c:	fc442783          	lw	a5,-60(s0)
   10220:	fc042703          	lw	a4,-64(s0)
   10224:	00ff6f33          	or	t5,t5,a5
   10228:	00ef6f33          	or	t5,t5,a4
   1022c:	0280006f          	j	10254 <main+0x200>
   10230:	fc042823          	sw	zero,-48(s0)
   10234:	fd042783          	lw	a5,-48(s0)
   10238:	00279793          	slli	a5,a5,0x2
   1023c:	fcf42423          	sw	a5,-56(s0)
   10240:	fc842783          	lw	a5,-56(s0)
   10244:	00ff6f33          	or	t5,t5,a5
   10248:	fe042783          	lw	a5,-32(s0)
   1024c:	00178793          	addi	a5,a5,1
   10250:	fef42023          	sw	a5,-32(s0)
   10254:	fec42783          	lw	a5,-20(s0)
   10258:	e60792e3          	bnez	a5,100bc <main+0x68>
   1025c:	fe842783          	lw	a5,-24(s0)
   10260:	e4079ee3          	bnez	a5,100bc <main+0x68>
   10264:	fcc42783          	lw	a5,-52(s0)
   10268:	ea078ee3          	beqz	a5,10124 <main+0xd0>
   1026c:	e51ff06f          	j	100bc <main+0x68>
```

## Unique Instructions
```
Number of different instructions: 17
List of unique instructions:
bnez
or
srai
add
beqz
andi
sw
bge
bne
slli
sub
mv
j
lui
addi
li
lw
```
## Word of thanks
I sciencerly thank Mr. Kunal Gosh(Founder/VSD) for helping me out to complete this flow smoothly.

## Acknowledgement
1. Kunal Ghosh, VSD Corp. Pvt. Ltd.
2. Skywater Foundry
3. Alwin Shaju,Colleague,IIIT B
4. Mayank Kabra

## Reference
1. https://github.com/SakethGajawada/RISCV-GNU
2. https://how2electronics.com/arduino-water-flow-sensor-measure-flow-rate-volume/
