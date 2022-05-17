GLOBAL cpuVendor
GLOBAL getKey
GLOBAL hasKey
GLOBAL getRTCInfo

section .text
	
cpuVendor:
	push rbp
	mov rbp, rsp

	push rbx

	mov rax, 0
	cpuid


	mov [rdi], ebx
	mov [rdi + 4], edx
	mov [rdi + 8], ecx

	mov byte [rdi+13], 0

	mov rax, rdi

	pop rbx

	mov rsp, rbp
	pop rbp
	ret

; devuelve 1 si hay una tecla y cero sino
hasKey:
    push rbp
    mov rbp, rsp

    mov rax,0
    in al, 0x64
    and al, 0x01
    
    mov rsp, rbp
    pop rbp
    ret

getKey:
    push rbp
    mov rbp, rsp
    mov rax,0 ;limpio el registro

    in al,0x60 ;agarro la tecla presionada

    mov rsp, rbp
    pop rbp
    ret


getRTCInfo:
    push rbp
    mov rbp,rsp

    mov rax, rdi
    out 70h, al
    mov rax,0
    in al, 71h

    leave
    ret
