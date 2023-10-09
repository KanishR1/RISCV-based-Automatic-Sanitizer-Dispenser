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
void read();
void operate(int);
void bottle_status();
int division(int,int);

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


int division(int dividend, int divisor) {
    int quotient = 0;
    
   
    while (dividend >= divisor) {
        dividend -= divisor;
        quotient++;
    }

    return quotient;
}


void bottle_status()
{
    int time = counter_h + counter_l;
    int freq = division(1000000,time);
    int water = freq;
    int ls = water * 17;
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

output.o:     file format elf32-littleriscv


Disassembly of section .text:

00010094 <main>:
   10094:	fe010113          	add	sp,sp,-32
   10098:	00112e23          	sw	ra,28(sp)
   1009c:	00812c23          	sw	s0,24(sp)
   100a0:	02010413          	add	s0,sp,32
   100a4:	000117b7          	lui	a5,0x11
   100a8:	3687a783          	lw	a5,872(a5) # 11368 <solenoid_valve_op>
   100ac:	00279793          	sll	a5,a5,0x2
   100b0:	fef42623          	sw	a5,-20(s0)
   100b4:	80c1a783          	lw	a5,-2036(gp) # 11370 <led_op>
   100b8:	00379793          	sll	a5,a5,0x3
   100bc:	fef42423          	sw	a5,-24(s0)
   100c0:	8101a783          	lw	a5,-2032(gp) # 11374 <buzzer>
   100c4:	00479793          	sll	a5,a5,0x4
   100c8:	fef42223          	sw	a5,-28(s0)
   100cc:	fec42783          	lw	a5,-20(s0)
   100d0:	fe842703          	lw	a4,-24(s0)
   100d4:	fe442683          	lw	a3,-28(s0)
   100d8:	00ff6f33          	or	t5,t5,a5
   100dc:	00ef6f33          	or	t5,t5,a4
   100e0:	00df6f33          	or	t5,t5,a3
   100e4:	001f7713          	and	a4,t5,1
   100e8:	80e1ae23          	sw	a4,-2020(gp) # 11380 <reset_button>
   100ec:	81c1a783          	lw	a5,-2020(gp) # 11380 <reset_button>
   100f0:	06078463          	beqz	a5,10158 <main+0xc4>
   100f4:	000117b7          	lui	a5,0x11
   100f8:	3607a423          	sw	zero,872(a5) # 11368 <solenoid_valve_op>
   100fc:	8001a623          	sw	zero,-2036(gp) # 11370 <led_op>
   10100:	8001a823          	sw	zero,-2032(gp) # 11374 <buzzer>
   10104:	8001aa23          	sw	zero,-2028(gp) # 11378 <counter_h>
   10108:	8001ac23          	sw	zero,-2024(gp) # 1137c <counter_l>
   1010c:	8201a023          	sw	zero,-2016(gp) # 11384 <water_used>
   10110:	000117b7          	lui	a5,0x11
   10114:	3687a783          	lw	a5,872(a5) # 11368 <solenoid_valve_op>
   10118:	00279793          	sll	a5,a5,0x2
   1011c:	fef42623          	sw	a5,-20(s0)
   10120:	80c1a783          	lw	a5,-2036(gp) # 11370 <led_op>
   10124:	00379793          	sll	a5,a5,0x3
   10128:	fef42423          	sw	a5,-24(s0)
   1012c:	8101a783          	lw	a5,-2032(gp) # 11374 <buzzer>
   10130:	00479793          	sll	a5,a5,0x4
   10134:	fef42223          	sw	a5,-28(s0)
   10138:	fec42783          	lw	a5,-20(s0)
   1013c:	fe842703          	lw	a4,-24(s0)
   10140:	fe442683          	lw	a3,-28(s0)
   10144:	00ff6f33          	or	t5,t5,a5
   10148:	00ef6f33          	or	t5,t5,a4
   1014c:	00df6f33          	or	t5,t5,a3
   10150:	f95ff06f          	j	100e4 <main+0x50>
   10154:	020000ef          	jal	10174 <read>
   10158:	80c1a783          	lw	a5,-2036(gp) # 11370 <led_op>
   1015c:	f80794e3          	bnez	a5,100e4 <main+0x50>
   10160:	8101a783          	lw	a5,-2032(gp) # 11374 <buzzer>
   10164:	f80790e3          	bnez	a5,100e4 <main+0x50>
   10168:	81c1a783          	lw	a5,-2020(gp) # 11380 <reset_button>
   1016c:	fe0784e3          	beqz	a5,10154 <main+0xc0>
   10170:	f75ff06f          	j	100e4 <main+0x50>

00010174 <read>:
   10174:	fe010113          	add	sp,sp,-32
   10178:	00112e23          	sw	ra,28(sp)
   1017c:	00812c23          	sw	s0,24(sp)
   10180:	02010413          	add	s0,sp,32
   10184:	002f7793          	and	a5,t5,2
   10188:	fef42623          	sw	a5,-20(s0)
   1018c:	fec42783          	lw	a5,-20(s0)
   10190:	4017d713          	sra	a4,a5,0x1
   10194:	000117b7          	lui	a5,0x11
   10198:	36e7a223          	sw	a4,868(a5) # 11364 <__DATA_BEGIN__>
   1019c:	000117b7          	lui	a5,0x11
   101a0:	3647a783          	lw	a5,868(a5) # 11364 <__DATA_BEGIN__>
   101a4:	00078513          	mv	a0,a5
   101a8:	018000ef          	jal	101c0 <operate>
   101ac:	00000013          	nop
   101b0:	01c12083          	lw	ra,28(sp)
   101b4:	01812403          	lw	s0,24(sp)
   101b8:	02010113          	add	sp,sp,32
   101bc:	00008067          	ret

000101c0 <operate>:
   101c0:	fd010113          	add	sp,sp,-48
   101c4:	02112623          	sw	ra,44(sp)
   101c8:	02812423          	sw	s0,40(sp)
   101cc:	03010413          	add	s0,sp,48
   101d0:	fca42e23          	sw	a0,-36(s0)
   101d4:	fdc42783          	lw	a5,-36(s0)
   101d8:	02078e63          	beqz	a5,10214 <operate+0x54>
   101dc:	000117b7          	lui	a5,0x11
   101e0:	00100713          	li	a4,1
   101e4:	36e7a423          	sw	a4,872(a5) # 11368 <solenoid_valve_op>
   101e8:	000117b7          	lui	a5,0x11
   101ec:	3687a783          	lw	a5,872(a5) # 11368 <solenoid_valve_op>
   101f0:	00279793          	sll	a5,a5,0x2
   101f4:	fef42623          	sw	a5,-20(s0)
   101f8:	fec42783          	lw	a5,-20(s0)
   101fc:	00ff6f33          	or	t5,t5,a5
   10200:	8141a783          	lw	a5,-2028(gp) # 11378 <counter_h>
   10204:	00178713          	add	a4,a5,1
   10208:	80e1aa23          	sw	a4,-2028(gp) # 11378 <counter_h>
   1020c:	0a0000ef          	jal	102ac <bottle_status>
   10210:	0300006f          	j	10240 <operate+0x80>
   10214:	000117b7          	lui	a5,0x11
   10218:	3607a423          	sw	zero,872(a5) # 11368 <solenoid_valve_op>
   1021c:	000117b7          	lui	a5,0x11
   10220:	3687a783          	lw	a5,872(a5) # 11368 <solenoid_valve_op>
   10224:	00279793          	sll	a5,a5,0x2
   10228:	fef42623          	sw	a5,-20(s0)
   1022c:	fec42783          	lw	a5,-20(s0)
   10230:	00ff6f33          	or	t5,t5,a5
   10234:	8181a783          	lw	a5,-2024(gp) # 1137c <counter_l>
   10238:	00178713          	add	a4,a5,1
   1023c:	80e1ac23          	sw	a4,-2024(gp) # 1137c <counter_l>
   10240:	00000013          	nop
   10244:	02c12083          	lw	ra,44(sp)
   10248:	02812403          	lw	s0,40(sp)
   1024c:	03010113          	add	sp,sp,48
   10250:	00008067          	ret

00010254 <division>:
   10254:	fd010113          	add	sp,sp,-48
   10258:	02812623          	sw	s0,44(sp)
   1025c:	03010413          	add	s0,sp,48
   10260:	fca42e23          	sw	a0,-36(s0)
   10264:	fcb42c23          	sw	a1,-40(s0)
   10268:	fe042623          	sw	zero,-20(s0)
   1026c:	0200006f          	j	1028c <division+0x38>
   10270:	fdc42703          	lw	a4,-36(s0)
   10274:	fd842783          	lw	a5,-40(s0)
   10278:	40f707b3          	sub	a5,a4,a5
   1027c:	fcf42e23          	sw	a5,-36(s0)
   10280:	fec42783          	lw	a5,-20(s0)
   10284:	00178793          	add	a5,a5,1
   10288:	fef42623          	sw	a5,-20(s0)
   1028c:	fdc42703          	lw	a4,-36(s0)
   10290:	fd842783          	lw	a5,-40(s0)
   10294:	fcf75ee3          	bge	a4,a5,10270 <division+0x1c>
   10298:	fec42783          	lw	a5,-20(s0)
   1029c:	00078513          	mv	a0,a5
   102a0:	02c12403          	lw	s0,44(sp)
   102a4:	03010113          	add	sp,sp,48
   102a8:	00008067          	ret

000102ac <bottle_status>:
   102ac:	fd010113          	add	sp,sp,-48
   102b0:	02112623          	sw	ra,44(sp)
   102b4:	02812423          	sw	s0,40(sp)
   102b8:	03010413          	add	s0,sp,48
   102bc:	8141a703          	lw	a4,-2028(gp) # 11378 <counter_h>
   102c0:	8181a783          	lw	a5,-2024(gp) # 1137c <counter_l>
   102c4:	00f707b3          	add	a5,a4,a5
   102c8:	fef42623          	sw	a5,-20(s0)
   102cc:	fec42583          	lw	a1,-20(s0)
   102d0:	000f47b7          	lui	a5,0xf4
   102d4:	24078513          	add	a0,a5,576 # f4240 <__global_pointer$+0xe26dc>
   102d8:	f7dff0ef          	jal	10254 <division>
   102dc:	fea42423          	sw	a0,-24(s0)
   102e0:	fe842783          	lw	a5,-24(s0)
   102e4:	fef42223          	sw	a5,-28(s0)
   102e8:	fe442703          	lw	a4,-28(s0)
   102ec:	00070793          	mv	a5,a4
   102f0:	00479793          	sll	a5,a5,0x4
   102f4:	00e787b3          	add	a5,a5,a4
   102f8:	fef42023          	sw	a5,-32(s0)
   102fc:	8201a703          	lw	a4,-2016(gp) # 11384 <water_used>
   10300:	fe042783          	lw	a5,-32(s0)
   10304:	00f70733          	add	a4,a4,a5
   10308:	82e1a023          	sw	a4,-2016(gp) # 11384 <water_used>
   1030c:	fe042703          	lw	a4,-32(s0)
   10310:	1f400793          	li	a5,500
   10314:	02f71e63          	bne	a4,a5,10350 <bottle_status+0xa4>
   10318:	00100713          	li	a4,1
   1031c:	80e1a623          	sw	a4,-2036(gp) # 11370 <led_op>
   10320:	00100713          	li	a4,1
   10324:	80e1a823          	sw	a4,-2032(gp) # 11374 <buzzer>
   10328:	80c1a783          	lw	a5,-2036(gp) # 11370 <led_op>
   1032c:	00379793          	sll	a5,a5,0x3
   10330:	fcf42e23          	sw	a5,-36(s0)
   10334:	8101a783          	lw	a5,-2032(gp) # 11374 <buzzer>
   10338:	00479793          	sll	a5,a5,0x4
   1033c:	fcf42c23          	sw	a5,-40(s0)
   10340:	fdc42783          	lw	a5,-36(s0)
   10344:	fd842703          	lw	a4,-40(s0)
   10348:	00ff6f33          	or	t5,t5,a5
   1034c:	00ef6f33          	or	t5,t5,a4
   10350:	00000013          	nop
   10354:	02c12083          	lw	ra,44(sp)
   10358:	02812403          	lw	s0,40(sp)
   1035c:	03010113          	add	sp,sp,48
   10360:	00008067          	ret

```

## Unique Instructions
```
Number of different instructions: 19
List of unique instructions:
lui
jal
j
li
bne
bge
lw
sra
sub
ret
beqz
mv
bnez
nop
add
sw
sll
or
and

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
