    segment .data 
a:  DQ  2
b:  DQ  4
c:  DQ  6

    segment .text 
    global main 
;We're doing a + b * c
main: 
    mov rax, [b]    ; Load b 
    mov rbx, [c]    ; Load c
    mul rbx         ; Will multiply rax with rbx and store in rax. 
    xor rbx, rbx    
    mov rbx, [a]    ; Load a
    add rax, rbx    ; Add a to b * c
    xor rbx, rbx    ; Answer is stored in rax. Look at picture included in submission! 
    ret
