bits 16

org 0xFE000

start:

mov dx,0x3D8
mov al,0x29
out dx,al
mov dx,0x3D4
mov ax,0x1906
out dx,ax
mov ax,0x0709
out dx,ax
mov ax,0x8001
out dx,ax
mov ax,0xB800
mov ds,ax
xor bx,bx
mov byte [bx], 'A'
inc bx
mov byte [bx], 0x09

nop
nop
nop

jmp start

times 8176 - ($-$$) db 0x90

jmp start

times 8192 - ($-$$) db 0x90