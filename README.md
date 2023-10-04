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
```

## Assembly Code
```

san_disp.o:     file format elf32-littleriscv


Disassembly of section .text:

00000000 <main>:
   0:	fe010113          	add	sp,sp,-32
   4:	00112e23          	sw	ra,28(sp)
   8:	00812c23          	sw	s0,24(sp)
   c:	02010413          	add	s0,sp,32
  10:	000007b7          	lui	a5,0x0
  14:	0007a783          	lw	a5,0(a5) # 0 <main>
  18:	00279793          	sll	a5,a5,0x2
  1c:	fef42623          	sw	a5,-20(s0)
  20:	000007b7          	lui	a5,0x0
  24:	0007a783          	lw	a5,0(a5) # 0 <main>
  28:	00379793          	sll	a5,a5,0x3
  2c:	fef42423          	sw	a5,-24(s0)
  30:	000007b7          	lui	a5,0x0
  34:	0007a783          	lw	a5,0(a5) # 0 <main>
  38:	00479793          	sll	a5,a5,0x4
  3c:	fef42223          	sw	a5,-28(s0)
  40:	00df6f33          	or	t5,t5,a3
  44:	00ef6f33          	or	t5,t5,a4
  48:	00ff6f33          	or	t5,t5,a5
  4c:	fed42623          	sw	a3,-20(s0)
  50:	fee42423          	sw	a4,-24(s0)
  54:	fef42223          	sw	a5,-28(s0)

00000058 <.L6>:
  58:	001f7713          	and	a4,t5,1
  5c:	000007b7          	lui	a5,0x0
  60:	00e7a023          	sw	a4,0(a5) # 0 <main>
  64:	000007b7          	lui	a5,0x0
  68:	0007a783          	lw	a5,0(a5) # 0 <main>
  6c:	08078663          	beqz	a5,f8 <.L4>
  70:	000007b7          	lui	a5,0x0
  74:	0007a023          	sw	zero,0(a5) # 0 <main>
  78:	000007b7          	lui	a5,0x0
  7c:	0007a023          	sw	zero,0(a5) # 0 <main>
  80:	000007b7          	lui	a5,0x0
  84:	0007a023          	sw	zero,0(a5) # 0 <main>
  88:	000007b7          	lui	a5,0x0
  8c:	0007a023          	sw	zero,0(a5) # 0 <main>
  90:	000007b7          	lui	a5,0x0
  94:	0007a023          	sw	zero,0(a5) # 0 <main>
  98:	000007b7          	lui	a5,0x0
  9c:	00000713          	li	a4,0
  a0:	00e7a023          	sw	a4,0(a5) # 0 <main>
  a4:	000007b7          	lui	a5,0x0
  a8:	0007a783          	lw	a5,0(a5) # 0 <main>
  ac:	00279793          	sll	a5,a5,0x2
  b0:	fef42623          	sw	a5,-20(s0)
  b4:	000007b7          	lui	a5,0x0
  b8:	0007a783          	lw	a5,0(a5) # 0 <main>
  bc:	00379793          	sll	a5,a5,0x3
  c0:	fef42423          	sw	a5,-24(s0)
  c4:	000007b7          	lui	a5,0x0
  c8:	0007a783          	lw	a5,0(a5) # 0 <main>
  cc:	00479793          	sll	a5,a5,0x4
  d0:	fef42223          	sw	a5,-28(s0)
  d4:	00df6f33          	or	t5,t5,a3
  d8:	00ef6f33          	or	t5,t5,a4
  dc:	00ff6f33          	or	t5,t5,a5
  e0:	fed42623          	sw	a3,-20(s0)
  e4:	fee42423          	sw	a4,-24(s0)
  e8:	fef42223          	sw	a5,-28(s0)
  ec:	f6dff06f          	j	58 <.L6>

000000f0 <.L5>:
  f0:	00000097          	auipc	ra,0x0
  f4:	000080e7          	jalr	ra # f0 <.L5>

000000f8 <.L4>:
  f8:	000007b7          	lui	a5,0x0
  fc:	0007a783          	lw	a5,0(a5) # 0 <main>
 100:	f4079ce3          	bnez	a5,58 <.L6>
 104:	000007b7          	lui	a5,0x0
 108:	0007a783          	lw	a5,0(a5) # 0 <main>
 10c:	f40796e3          	bnez	a5,58 <.L6>
 110:	000007b7          	lui	a5,0x0
 114:	0007a783          	lw	a5,0(a5) # 0 <main>
 118:	fc078ce3          	beqz	a5,f0 <.L5>
 11c:	f3dff06f          	j	58 <.L6>

00000120 <read>:
 120:	fe010113          	add	sp,sp,-32
 124:	00112e23          	sw	ra,28(sp)
 128:	00812c23          	sw	s0,24(sp)
 12c:	02010413          	add	s0,sp,32
 130:	002f7793          	and	a5,t5,2
 134:	fef42623          	sw	a5,-20(s0)
 138:	fec42783          	lw	a5,-20(s0)
 13c:	01f7d713          	srl	a4,a5,0x1f
 140:	00f707b3          	add	a5,a4,a5
 144:	4017d793          	sra	a5,a5,0x1
 148:	00078713          	mv	a4,a5
 14c:	000007b7          	lui	a5,0x0
 150:	00e7a023          	sw	a4,0(a5) # 0 <main>
 154:	000007b7          	lui	a5,0x0
 158:	0007a783          	lw	a5,0(a5) # 0 <main>
 15c:	00078513          	mv	a0,a5
 160:	00000097          	auipc	ra,0x0
 164:	000080e7          	jalr	ra # 160 <read+0x40>
 168:	00000013          	nop
 16c:	01c12083          	lw	ra,28(sp)
 170:	01812403          	lw	s0,24(sp)
 174:	02010113          	add	sp,sp,32
 178:	00008067          	ret

0000017c <operate>:
 17c:	fd010113          	add	sp,sp,-48
 180:	02112623          	sw	ra,44(sp)
 184:	02812423          	sw	s0,40(sp)
 188:	03010413          	add	s0,sp,48
 18c:	fca42e23          	sw	a0,-36(s0)
 190:	fdc42783          	lw	a5,-36(s0)
 194:	04078463          	beqz	a5,1dc <.L9>
 198:	000007b7          	lui	a5,0x0
 19c:	00100713          	li	a4,1
 1a0:	00e7a023          	sw	a4,0(a5) # 0 <main>
 1a4:	000007b7          	lui	a5,0x0
 1a8:	0007a783          	lw	a5,0(a5) # 0 <main>
 1ac:	00279793          	sll	a5,a5,0x2
 1b0:	fef42623          	sw	a5,-20(s0)
 1b4:	00ff6f33          	or	t5,t5,a5
 1b8:	fef42623          	sw	a5,-20(s0)
 1bc:	000007b7          	lui	a5,0x0
 1c0:	0007a783          	lw	a5,0(a5) # 0 <main>
 1c4:	00178713          	add	a4,a5,1
 1c8:	000007b7          	lui	a5,0x0
 1cc:	00e7a023          	sw	a4,0(a5) # 0 <main>
 1d0:	00000097          	auipc	ra,0x0
 1d4:	000080e7          	jalr	ra # 1d0 <operate+0x54>
 1d8:	0380006f          	j	210 <.L11>

000001dc <.L9>:
 1dc:	000007b7          	lui	a5,0x0
 1e0:	0007a023          	sw	zero,0(a5) # 0 <main>
 1e4:	000007b7          	lui	a5,0x0
 1e8:	0007a783          	lw	a5,0(a5) # 0 <main>
 1ec:	00279793          	sll	a5,a5,0x2
 1f0:	fef42623          	sw	a5,-20(s0)
 1f4:	00ff6f33          	or	t5,t5,a5
 1f8:	fef42623          	sw	a5,-20(s0)
 1fc:	000007b7          	lui	a5,0x0
 200:	0007a783          	lw	a5,0(a5) # 0 <main>
 204:	00178713          	add	a4,a5,1
 208:	000007b7          	lui	a5,0x0
 20c:	00e7a023          	sw	a4,0(a5) # 0 <main>

00000210 <.L11>:
 210:	00000013          	nop
 214:	02c12083          	lw	ra,44(sp)
 218:	02812403          	lw	s0,40(sp)
 21c:	03010113          	add	sp,sp,48
 220:	00008067          	ret

00000224 <bottle_status>:
 224:	fd010113          	add	sp,sp,-48
 228:	02112623          	sw	ra,44(sp)
 22c:	02812423          	sw	s0,40(sp)
 230:	03010413          	add	s0,sp,48
 234:	000007b7          	lui	a5,0x0
 238:	0007a703          	lw	a4,0(a5) # 0 <main>
 23c:	000007b7          	lui	a5,0x0
 240:	0007a783          	lw	a5,0(a5) # 0 <main>
 244:	00f707b3          	add	a5,a4,a5
 248:	fef42623          	sw	a5,-20(s0)
 24c:	000f47b7          	lui	a5,0xf4
 250:	24078713          	add	a4,a5,576 # f4240 <.L16+0xf3f08>
 254:	fec42783          	lw	a5,-20(s0)
 258:	02f747b3          	div	a5,a4,a5
 25c:	00078513          	mv	a0,a5
 260:	00000097          	auipc	ra,0x0
 264:	000080e7          	jalr	ra # 260 <bottle_status+0x3c>
 268:	00050793          	mv	a5,a0
 26c:	fef42423          	sw	a5,-24(s0)
 270:	000007b7          	lui	a5,0x0
 274:	0007a583          	lw	a1,0(a5) # 0 <main>
 278:	fe842503          	lw	a0,-24(s0)
 27c:	00000097          	auipc	ra,0x0
 280:	000080e7          	jalr	ra # 27c <bottle_status+0x58>
 284:	00050793          	mv	a5,a0
 288:	fef42223          	sw	a5,-28(s0)
 28c:	000007b7          	lui	a5,0x0
 290:	0007a583          	lw	a1,0(a5) # 0 <main>
 294:	fe442503          	lw	a0,-28(s0)
 298:	00000097          	auipc	ra,0x0
 29c:	000080e7          	jalr	ra # 298 <bottle_status+0x74>
 2a0:	00050793          	mv	a5,a0
 2a4:	fef42023          	sw	a5,-32(s0)
 2a8:	000007b7          	lui	a5,0x0
 2ac:	0007a783          	lw	a5,0(a5) # 0 <main>
 2b0:	fe042583          	lw	a1,-32(s0)
 2b4:	00078513          	mv	a0,a5
 2b8:	00000097          	auipc	ra,0x0
 2bc:	000080e7          	jalr	ra # 2b8 <bottle_status+0x94>
 2c0:	00050793          	mv	a5,a0
 2c4:	00078713          	mv	a4,a5
 2c8:	000007b7          	lui	a5,0x0
 2cc:	00e7a023          	sw	a4,0(a5) # 0 <main>
 2d0:	000007b7          	lui	a5,0x0
 2d4:	0007a583          	lw	a1,0(a5) # 0 <main>
 2d8:	fe042503          	lw	a0,-32(s0)
 2dc:	00000097          	auipc	ra,0x0
 2e0:	000080e7          	jalr	ra # 2dc <bottle_status+0xb8>
 2e4:	00050793          	mv	a5,a0
 2e8:	00078463          	beqz	a5,2f0 <.L15>
 2ec:	04c0006f          	j	338 <.L16>

000002f0 <.L15>:
 2f0:	000007b7          	lui	a5,0x0
 2f4:	00100713          	li	a4,1
 2f8:	00e7a023          	sw	a4,0(a5) # 0 <main>
 2fc:	000007b7          	lui	a5,0x0
 300:	00100713          	li	a4,1
 304:	00e7a023          	sw	a4,0(a5) # 0 <main>
 308:	000007b7          	lui	a5,0x0
 30c:	0007a783          	lw	a5,0(a5) # 0 <main>
 310:	00379793          	sll	a5,a5,0x3
 314:	fcf42e23          	sw	a5,-36(s0)
 318:	000007b7          	lui	a5,0x0
 31c:	0007a783          	lw	a5,0(a5) # 0 <main>
 320:	00479793          	sll	a5,a5,0x4
 324:	fcf42c23          	sw	a5,-40(s0)
 328:	00ef6f33          	or	t5,t5,a4
 32c:	00ff6f33          	or	t5,t5,a5
 330:	fce42e23          	sw	a4,-36(s0)
 334:	fcf42c23          	sw	a5,-40(s0)

00000338 <.L16>:
 338:	00000013          	nop
 33c:	02c12083          	lw	ra,44(sp)
 340:	02812403          	lw	s0,40(sp)
 344:	03010113          	add	sp,sp,48
 348:	00008067          	ret
```

## Unique Instructions
```
Number of different instructions: 19
List of unique instructions:
add
sll
lw
bnez
and
lui
li
ret
j
sw
sra
or
div
nop
jalr
srl
auipc
mv
beqz
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
