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
```
## GCC Compiler Working
![gcc](gcc_working.png)
## Assembly code conversion

Compile the c program using RISCV-V GNU Toolchain and dump the assembly code into water_level_assembly.txt using the below commands.

```
riscv32-unkown-elf-gcc -c -march=rv32i -mabi=ilp32 -ffreestanding  -nostdlib -o ./out san_disp.c
riscv32-unknown-elf-objdump -d  -r out > assembly.txt
```

## Assembly Code
```

out:     file format elf32-littleriscv


Disassembly of section .text:

00000000 <main>:
   0:	fe010113          	add	sp,sp,-32
   4:	00112e23          	sw	ra,28(sp)
   8:	00812c23          	sw	s0,24(sp)
   c:	02010413          	add	s0,sp,32
  10:	000007b7          	lui	a5,0x0
			10: R_RISCV_HI20	solenoid_valve_op
			10: R_RISCV_RELAX	*ABS*
  14:	0007a783          	lw	a5,0(a5) # 0 <main>
			14: R_RISCV_LO12_I	solenoid_valve_op
			14: R_RISCV_RELAX	*ABS*
  18:	00279793          	sll	a5,a5,0x2
  1c:	fef42623          	sw	a5,-20(s0)
  20:	000007b7          	lui	a5,0x0
			20: R_RISCV_HI20	led_op
			20: R_RISCV_RELAX	*ABS*
  24:	0007a783          	lw	a5,0(a5) # 0 <main>
			24: R_RISCV_LO12_I	led_op
			24: R_RISCV_RELAX	*ABS*
  28:	00379793          	sll	a5,a5,0x3
  2c:	fef42423          	sw	a5,-24(s0)
  30:	000007b7          	lui	a5,0x0
			30: R_RISCV_HI20	buzzer
			30: R_RISCV_RELAX	*ABS*
  34:	0007a783          	lw	a5,0(a5) # 0 <main>
			34: R_RISCV_LO12_I	buzzer
			34: R_RISCV_RELAX	*ABS*
  38:	00479793          	sll	a5,a5,0x4
  3c:	fef42223          	sw	a5,-28(s0)
  40:	fec42783          	lw	a5,-20(s0)
  44:	fe842703          	lw	a4,-24(s0)
  48:	fe442683          	lw	a3,-28(s0)
  4c:	00ff6f33          	or	t5,t5,a5
  50:	00ef6f33          	or	t5,t5,a4
  54:	00df6f33          	or	t5,t5,a3

00000058 <.L6>:
  58:	001f7713          	and	a4,t5,1
  5c:	000007b7          	lui	a5,0x0
			5c: R_RISCV_HI20	reset_button
			5c: R_RISCV_RELAX	*ABS*
  60:	00e7a023          	sw	a4,0(a5) # 0 <main>
			60: R_RISCV_LO12_S	reset_button
			60: R_RISCV_RELAX	*ABS*
  64:	000007b7          	lui	a5,0x0
			64: R_RISCV_HI20	reset_button
			64: R_RISCV_RELAX	*ABS*
  68:	0007a783          	lw	a5,0(a5) # 0 <main>
			68: R_RISCV_LO12_I	reset_button
			68: R_RISCV_RELAX	*ABS*
  6c:	08078463          	beqz	a5,f4 <.L4>
			6c: R_RISCV_BRANCH	.L4
  70:	000007b7          	lui	a5,0x0
			70: R_RISCV_HI20	solenoid_valve_op
			70: R_RISCV_RELAX	*ABS*
  74:	0007a023          	sw	zero,0(a5) # 0 <main>
			74: R_RISCV_LO12_S	solenoid_valve_op
			74: R_RISCV_RELAX	*ABS*
  78:	000007b7          	lui	a5,0x0
			78: R_RISCV_HI20	led_op
			78: R_RISCV_RELAX	*ABS*
  7c:	0007a023          	sw	zero,0(a5) # 0 <main>
			7c: R_RISCV_LO12_S	led_op
			7c: R_RISCV_RELAX	*ABS*
  80:	000007b7          	lui	a5,0x0
			80: R_RISCV_HI20	buzzer
			80: R_RISCV_RELAX	*ABS*
  84:	0007a023          	sw	zero,0(a5) # 0 <main>
			84: R_RISCV_LO12_S	buzzer
			84: R_RISCV_RELAX	*ABS*
  88:	000007b7          	lui	a5,0x0
			88: R_RISCV_HI20	counter_h
			88: R_RISCV_RELAX	*ABS*
  8c:	0007a023          	sw	zero,0(a5) # 0 <main>
			8c: R_RISCV_LO12_S	counter_h
			8c: R_RISCV_RELAX	*ABS*
  90:	000007b7          	lui	a5,0x0
			90: R_RISCV_HI20	counter_l
			90: R_RISCV_RELAX	*ABS*
  94:	0007a023          	sw	zero,0(a5) # 0 <main>
			94: R_RISCV_LO12_S	counter_l
			94: R_RISCV_RELAX	*ABS*
  98:	000007b7          	lui	a5,0x0
			98: R_RISCV_HI20	water_used
			98: R_RISCV_RELAX	*ABS*
  9c:	0007a023          	sw	zero,0(a5) # 0 <main>
			9c: R_RISCV_LO12_S	water_used
			9c: R_RISCV_RELAX	*ABS*
  a0:	000007b7          	lui	a5,0x0
			a0: R_RISCV_HI20	solenoid_valve_op
			a0: R_RISCV_RELAX	*ABS*
  a4:	0007a783          	lw	a5,0(a5) # 0 <main>
			a4: R_RISCV_LO12_I	solenoid_valve_op
			a4: R_RISCV_RELAX	*ABS*
  a8:	00279793          	sll	a5,a5,0x2
  ac:	fef42623          	sw	a5,-20(s0)
  b0:	000007b7          	lui	a5,0x0
			b0: R_RISCV_HI20	led_op
			b0: R_RISCV_RELAX	*ABS*
  b4:	0007a783          	lw	a5,0(a5) # 0 <main>
			b4: R_RISCV_LO12_I	led_op
			b4: R_RISCV_RELAX	*ABS*
  b8:	00379793          	sll	a5,a5,0x3
  bc:	fef42423          	sw	a5,-24(s0)
  c0:	000007b7          	lui	a5,0x0
			c0: R_RISCV_HI20	buzzer
			c0: R_RISCV_RELAX	*ABS*
  c4:	0007a783          	lw	a5,0(a5) # 0 <main>
			c4: R_RISCV_LO12_I	buzzer
			c4: R_RISCV_RELAX	*ABS*
  c8:	00479793          	sll	a5,a5,0x4
  cc:	fef42223          	sw	a5,-28(s0)
  d0:	fec42783          	lw	a5,-20(s0)
  d4:	fe842703          	lw	a4,-24(s0)
  d8:	fe442683          	lw	a3,-28(s0)
  dc:	00ff6f33          	or	t5,t5,a5
  e0:	00ef6f33          	or	t5,t5,a4
  e4:	00df6f33          	or	t5,t5,a3
  e8:	f71ff06f          	j	58 <.L6>
			e8: R_RISCV_JAL	.L6

000000ec <.L5>:
  ec:	00000097          	auipc	ra,0x0
			ec: R_RISCV_CALL_PLT	read
			ec: R_RISCV_RELAX	*ABS*
  f0:	000080e7          	jalr	ra # ec <.L5>

000000f4 <.L4>:
  f4:	000007b7          	lui	a5,0x0
			f4: R_RISCV_HI20	led_op
			f4: R_RISCV_RELAX	*ABS*
  f8:	0007a783          	lw	a5,0(a5) # 0 <main>
			f8: R_RISCV_LO12_I	led_op
			f8: R_RISCV_RELAX	*ABS*
  fc:	f4079ee3          	bnez	a5,58 <.L6>
			fc: R_RISCV_BRANCH	.L6
 100:	000007b7          	lui	a5,0x0
			100: R_RISCV_HI20	buzzer
			100: R_RISCV_RELAX	*ABS*
 104:	0007a783          	lw	a5,0(a5) # 0 <main>
			104: R_RISCV_LO12_I	buzzer
			104: R_RISCV_RELAX	*ABS*
 108:	f40798e3          	bnez	a5,58 <.L6>
			108: R_RISCV_BRANCH	.L6
 10c:	000007b7          	lui	a5,0x0
			10c: R_RISCV_HI20	reset_button
			10c: R_RISCV_RELAX	*ABS*
 110:	0007a783          	lw	a5,0(a5) # 0 <main>
			110: R_RISCV_LO12_I	reset_button
			110: R_RISCV_RELAX	*ABS*
 114:	fc078ce3          	beqz	a5,ec <.L5>
			114: R_RISCV_BRANCH	.L5
 118:	f41ff06f          	j	58 <.L6>
			118: R_RISCV_JAL	.L6

0000011c <read>:
 11c:	fe010113          	add	sp,sp,-32
 120:	00112e23          	sw	ra,28(sp)
 124:	00812c23          	sw	s0,24(sp)
 128:	02010413          	add	s0,sp,32
 12c:	002f7793          	and	a5,t5,2
 130:	fef42623          	sw	a5,-20(s0)
 134:	fec42783          	lw	a5,-20(s0)
 138:	4017d713          	sra	a4,a5,0x1
 13c:	000007b7          	lui	a5,0x0
			13c: R_RISCV_HI20	ir_sen_ip
			13c: R_RISCV_RELAX	*ABS*
 140:	00e7a023          	sw	a4,0(a5) # 0 <main>
			140: R_RISCV_LO12_S	ir_sen_ip
			140: R_RISCV_RELAX	*ABS*
 144:	000007b7          	lui	a5,0x0
			144: R_RISCV_HI20	ir_sen_ip
			144: R_RISCV_RELAX	*ABS*
 148:	0007a783          	lw	a5,0(a5) # 0 <main>
			148: R_RISCV_LO12_I	ir_sen_ip
			148: R_RISCV_RELAX	*ABS*
 14c:	00078513          	mv	a0,a5
 150:	00000097          	auipc	ra,0x0
			150: R_RISCV_CALL_PLT	operate
			150: R_RISCV_RELAX	*ABS*
 154:	000080e7          	jalr	ra # 150 <read+0x34>
 158:	00000013          	nop
 15c:	01c12083          	lw	ra,28(sp)
 160:	01812403          	lw	s0,24(sp)
 164:	02010113          	add	sp,sp,32
 168:	00008067          	ret

0000016c <operate>:
 16c:	fd010113          	add	sp,sp,-48
 170:	02112623          	sw	ra,44(sp)
 174:	02812423          	sw	s0,40(sp)
 178:	03010413          	add	s0,sp,48
 17c:	fca42e23          	sw	a0,-36(s0)
 180:	fdc42783          	lw	a5,-36(s0)
 184:	04078463          	beqz	a5,1cc <.L9>
			184: R_RISCV_BRANCH	.L9
 188:	000007b7          	lui	a5,0x0
			188: R_RISCV_HI20	solenoid_valve_op
			188: R_RISCV_RELAX	*ABS*
 18c:	00100713          	li	a4,1
 190:	00e7a023          	sw	a4,0(a5) # 0 <main>
			190: R_RISCV_LO12_S	solenoid_valve_op
			190: R_RISCV_RELAX	*ABS*
 194:	000007b7          	lui	a5,0x0
			194: R_RISCV_HI20	solenoid_valve_op
			194: R_RISCV_RELAX	*ABS*
 198:	0007a783          	lw	a5,0(a5) # 0 <main>
			198: R_RISCV_LO12_I	solenoid_valve_op
			198: R_RISCV_RELAX	*ABS*
 19c:	00279793          	sll	a5,a5,0x2
 1a0:	fef42623          	sw	a5,-20(s0)
 1a4:	fec42783          	lw	a5,-20(s0)
 1a8:	00ff6f33          	or	t5,t5,a5
 1ac:	000007b7          	lui	a5,0x0
			1ac: R_RISCV_HI20	counter_h
			1ac: R_RISCV_RELAX	*ABS*
 1b0:	0007a783          	lw	a5,0(a5) # 0 <main>
			1b0: R_RISCV_LO12_I	counter_h
			1b0: R_RISCV_RELAX	*ABS*
 1b4:	00178713          	add	a4,a5,1
 1b8:	000007b7          	lui	a5,0x0
			1b8: R_RISCV_HI20	counter_h
			1b8: R_RISCV_RELAX	*ABS*
 1bc:	00e7a023          	sw	a4,0(a5) # 0 <main>
			1bc: R_RISCV_LO12_S	counter_h
			1bc: R_RISCV_RELAX	*ABS*
 1c0:	00000097          	auipc	ra,0x0
			1c0: R_RISCV_CALL_PLT	bottle_status
			1c0: R_RISCV_RELAX	*ABS*
 1c4:	000080e7          	jalr	ra # 1c0 <operate+0x54>
 1c8:	0380006f          	j	200 <.L11>
			1c8: R_RISCV_JAL	.L11

000001cc <.L9>:
 1cc:	000007b7          	lui	a5,0x0
			1cc: R_RISCV_HI20	solenoid_valve_op
			1cc: R_RISCV_RELAX	*ABS*
 1d0:	0007a023          	sw	zero,0(a5) # 0 <main>
			1d0: R_RISCV_LO12_S	solenoid_valve_op
			1d0: R_RISCV_RELAX	*ABS*
 1d4:	000007b7          	lui	a5,0x0
			1d4: R_RISCV_HI20	solenoid_valve_op
			1d4: R_RISCV_RELAX	*ABS*
 1d8:	0007a783          	lw	a5,0(a5) # 0 <main>
			1d8: R_RISCV_LO12_I	solenoid_valve_op
			1d8: R_RISCV_RELAX	*ABS*
 1dc:	00279793          	sll	a5,a5,0x2
 1e0:	fef42623          	sw	a5,-20(s0)
 1e4:	fec42783          	lw	a5,-20(s0)
 1e8:	00ff6f33          	or	t5,t5,a5
 1ec:	000007b7          	lui	a5,0x0
			1ec: R_RISCV_HI20	counter_l
			1ec: R_RISCV_RELAX	*ABS*
 1f0:	0007a783          	lw	a5,0(a5) # 0 <main>
			1f0: R_RISCV_LO12_I	counter_l
			1f0: R_RISCV_RELAX	*ABS*
 1f4:	00178713          	add	a4,a5,1
 1f8:	000007b7          	lui	a5,0x0
			1f8: R_RISCV_HI20	counter_l
			1f8: R_RISCV_RELAX	*ABS*
 1fc:	00e7a023          	sw	a4,0(a5) # 0 <main>
			1fc: R_RISCV_LO12_S	counter_l
			1fc: R_RISCV_RELAX	*ABS*

00000200 <.L11>:
 200:	00000013          	nop
 204:	02c12083          	lw	ra,44(sp)
 208:	02812403          	lw	s0,40(sp)
 20c:	03010113          	add	sp,sp,48
 210:	00008067          	ret

00000214 <bottle_status>:
 214:	fd010113          	add	sp,sp,-48
 218:	02112623          	sw	ra,44(sp)
 21c:	02812423          	sw	s0,40(sp)
 220:	03010413          	add	s0,sp,48
 224:	000007b7          	lui	a5,0x0
			224: R_RISCV_HI20	counter_h
			224: R_RISCV_RELAX	*ABS*
 228:	0007a703          	lw	a4,0(a5) # 0 <main>
			228: R_RISCV_LO12_I	counter_h
			228: R_RISCV_RELAX	*ABS*
 22c:	000007b7          	lui	a5,0x0
			22c: R_RISCV_HI20	counter_l
			22c: R_RISCV_RELAX	*ABS*
 230:	0007a783          	lw	a5,0(a5) # 0 <main>
			230: R_RISCV_LO12_I	counter_l
			230: R_RISCV_RELAX	*ABS*
 234:	00f707b3          	add	a5,a4,a5
 238:	fef42623          	sw	a5,-20(s0)
 23c:	fec42783          	lw	a5,-20(s0)
 240:	000f4737          	lui	a4,0xf4
 244:	24070713          	add	a4,a4,576 # f4240 <.L14+0xf3ee4>
 248:	40f757b3          	sra	a5,a4,a5
 24c:	fef42423          	sw	a5,-24(s0)
 250:	fe842503          	lw	a0,-24(s0)
 254:	00000097          	auipc	ra,0x0
			254: R_RISCV_CALL_PLT	__floatsidf
			254: R_RISCV_RELAX	*ABS*
 258:	000080e7          	jalr	ra # 254 <bottle_status+0x40>
 25c:	00050713          	mv	a4,a0
 260:	00058793          	mv	a5,a1
 264:	000006b7          	lui	a3,0x0
			264: R_RISCV_HI20	.LC0
			264: R_RISCV_RELAX	*ABS*
 268:	0006a603          	lw	a2,0(a3) # 0 <main>
			268: R_RISCV_LO12_I	.LC0
			268: R_RISCV_RELAX	*ABS*
 26c:	0046a683          	lw	a3,4(a3)
			26c: R_RISCV_LO12_I	.LC0+0x4
			26c: R_RISCV_RELAX	*ABS*+0x4
 270:	00070513          	mv	a0,a4
 274:	00078593          	mv	a1,a5
 278:	00000097          	auipc	ra,0x0
			278: R_RISCV_CALL_PLT	__muldf3
			278: R_RISCV_RELAX	*ABS*
 27c:	000080e7          	jalr	ra # 278 <bottle_status+0x64>
 280:	00050713          	mv	a4,a0
 284:	00058793          	mv	a5,a1
 288:	00070513          	mv	a0,a4
 28c:	00078593          	mv	a1,a5
 290:	00000097          	auipc	ra,0x0
			290: R_RISCV_CALL_PLT	__fixdfsi
			290: R_RISCV_RELAX	*ABS*
 294:	000080e7          	jalr	ra # 290 <bottle_status+0x7c>
 298:	00050793          	mv	a5,a0
 29c:	fef42223          	sw	a5,-28(s0)
 2a0:	fe442503          	lw	a0,-28(s0)
 2a4:	00000097          	auipc	ra,0x0
			2a4: R_RISCV_CALL_PLT	__floatsidf
			2a4: R_RISCV_RELAX	*ABS*
 2a8:	000080e7          	jalr	ra # 2a4 <bottle_status+0x90>
 2ac:	00050713          	mv	a4,a0
 2b0:	00058793          	mv	a5,a1
 2b4:	000006b7          	lui	a3,0x0
			2b4: R_RISCV_HI20	.LC1
			2b4: R_RISCV_RELAX	*ABS*
 2b8:	0006a603          	lw	a2,0(a3) # 0 <main>
			2b8: R_RISCV_LO12_I	.LC1
			2b8: R_RISCV_RELAX	*ABS*
 2bc:	0046a683          	lw	a3,4(a3)
			2bc: R_RISCV_LO12_I	.LC1+0x4
			2bc: R_RISCV_RELAX	*ABS*+0x4
 2c0:	00070513          	mv	a0,a4
 2c4:	00078593          	mv	a1,a5
 2c8:	00000097          	auipc	ra,0x0
			2c8: R_RISCV_CALL_PLT	__muldf3
			2c8: R_RISCV_RELAX	*ABS*
 2cc:	000080e7          	jalr	ra # 2c8 <bottle_status+0xb4>
 2d0:	00050713          	mv	a4,a0
 2d4:	00058793          	mv	a5,a1
 2d8:	00070513          	mv	a0,a4
 2dc:	00078593          	mv	a1,a5
 2e0:	00000097          	auipc	ra,0x0
			2e0: R_RISCV_CALL_PLT	__fixdfsi
			2e0: R_RISCV_RELAX	*ABS*
 2e4:	000080e7          	jalr	ra # 2e0 <bottle_status+0xcc>
 2e8:	00050793          	mv	a5,a0
 2ec:	fef42023          	sw	a5,-32(s0)
 2f0:	000007b7          	lui	a5,0x0
			2f0: R_RISCV_HI20	water_used
			2f0: R_RISCV_RELAX	*ABS*
 2f4:	0007a703          	lw	a4,0(a5) # 0 <main>
			2f4: R_RISCV_LO12_I	water_used
			2f4: R_RISCV_RELAX	*ABS*
 2f8:	fe042783          	lw	a5,-32(s0)
 2fc:	00f70733          	add	a4,a4,a5
 300:	000007b7          	lui	a5,0x0
			300: R_RISCV_HI20	water_used
			300: R_RISCV_RELAX	*ABS*
 304:	00e7a023          	sw	a4,0(a5) # 0 <main>
			304: R_RISCV_LO12_S	water_used
			304: R_RISCV_RELAX	*ABS*
 308:	fe042703          	lw	a4,-32(s0)
 30c:	1f400793          	li	a5,500
 310:	04f71663          	bne	a4,a5,35c <.L14>
			310: R_RISCV_BRANCH	.L14
 314:	000007b7          	lui	a5,0x0
			314: R_RISCV_HI20	led_op
			314: R_RISCV_RELAX	*ABS*
 318:	00100713          	li	a4,1
 31c:	00e7a023          	sw	a4,0(a5) # 0 <main>
			31c: R_RISCV_LO12_S	led_op
			31c: R_RISCV_RELAX	*ABS*
 320:	000007b7          	lui	a5,0x0
			320: R_RISCV_HI20	buzzer
			320: R_RISCV_RELAX	*ABS*
 324:	00100713          	li	a4,1
 328:	00e7a023          	sw	a4,0(a5) # 0 <main>
			328: R_RISCV_LO12_S	buzzer
			328: R_RISCV_RELAX	*ABS*
 32c:	000007b7          	lui	a5,0x0
			32c: R_RISCV_HI20	led_op
			32c: R_RISCV_RELAX	*ABS*
 330:	0007a783          	lw	a5,0(a5) # 0 <main>
			330: R_RISCV_LO12_I	led_op
			330: R_RISCV_RELAX	*ABS*
 334:	00379793          	sll	a5,a5,0x3
 338:	fcf42e23          	sw	a5,-36(s0)
 33c:	000007b7          	lui	a5,0x0
			33c: R_RISCV_HI20	buzzer
			33c: R_RISCV_RELAX	*ABS*
 340:	0007a783          	lw	a5,0(a5) # 0 <main>
			340: R_RISCV_LO12_I	buzzer
			340: R_RISCV_RELAX	*ABS*
 344:	00479793          	sll	a5,a5,0x4
 348:	fcf42c23          	sw	a5,-40(s0)
 34c:	fdc42783          	lw	a5,-36(s0)
 350:	fd842703          	lw	a4,-40(s0)
 354:	00ff6f33          	or	t5,t5,a5
 358:	00ef6f33          	or	t5,t5,a4

0000035c <.L14>:
 35c:	00000013          	nop
 360:	02c12083          	lw	ra,44(sp)
 364:	02812403          	lw	s0,40(sp)
 368:	03010113          	add	sp,sp,48
 36c:	00008067          	ret
```

## Unique Instructions
```
Number of different instructions: 18
List of unique instructions:
lw
sll
j
jalr
beqz
and
bnez
sw
bne
sra
add
auipc
nop
or
lui
ret
li
mv
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
