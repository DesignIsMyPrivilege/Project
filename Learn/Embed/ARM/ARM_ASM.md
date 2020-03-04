# ARM汇编

Operation{cond}{state} Rd,Rn,Operand2

<img src="C:\Users\Think\AppData\Roaming\Typora\typora-user-images\image-20200304202121454.png" style="zoom:60%">

<img src="C:\Users\Think\AppData\Roaming\Typora\typora-user-images\image-20200304202940032.png" alt="image-20200304202940032" style="zoom:50%;" />

向寄存器中一个值添加100

x86:`ADD eax,#100`

ARM:`ADD r0,r0,#100`

从一个寄存器指针加载到寄存器

x86:`MOV eax,DWORA PYR[ebx]`

ARM:`ldr r0,[r1]`

```
#   AREA Example,CODE,READONLY
#   ENTRY
#   CODE32
#START
#
#   END
   AREA testhello,CODE,READONLY
   ENTRY
   CODE32
START
   ADD R1,R2,#4
   END
```

寻址方式：

1. 立即数寻址

   `ADD R0,R0,#0x3F`

2. 寄存器寻址

   `ADD R0,R1,R2`  ##MOV和ADD只能针对寄存器进行操作，无法操作内存中的地址

3. 寄存器间接寻址

   `LDR R0,[R1]`  ##将R1中存储的数作为地址，将该地址中的数赋值给R0

   `STR R0,[R1]`  ##将R0中的数存储在将R1中存储的数作为地址的内存中

4. 寄存器移位寻址

   `ADD R3,R2,R1,LSL #2` ##将R1左移两位加上R2赋值给R3                                                                                                                                             

5. 基地址寻址

   `LDR R0,[R1,#4]` ##将R1中存储的数加4作为地址，将该地址中的数赋值给R0

   `LDR R0,[R1],#4` ##将将R1中存储的数作为地址，将该地址中的数取出加4赋值给R0

   `LDR R0,[R1,R2]`

6. 多寄存器寻址

   `LDMIA R0,{R1,R2,R3,R4}`

7. 相对寻址

   `BL NEXT` ##跳转到NEXT处

   `MOV PC,LR`