.global led2_on
led2_on:
   ldr r11,=0xE0200204
   ldr r12,[r11]
   bic r12,r12,#(1<<1)
   str r12,[r11]
   mov pc,lr