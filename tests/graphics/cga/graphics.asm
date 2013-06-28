bits 16

start:

mov ax,0xB800
mov es,ax
xor di,di
mov ax,0xF000
mov ds,ax
mov si,0xE000

mov dx,0x3D8
mov al,0x2B
out dx,al
mov dx,0x3D4
mov ax,0x1906
out dx,ax
mov ax,0x0709
out dx,ax
mov ax,0x8001
out dx,ax

main:

movsb

cmp si,0xFFF0
jge fix

cont:

cmp di,0x2000
jge fix2

jmp near main

fix:

mov si,0xE000
jmp cont

fix2:

xor di,di
jmp near main

times 0x1FF0 - ($-$$) db 0x56

jmp 0xF000:0xE000

times 0x2000 - ($-$$) db 0x90