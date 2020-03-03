; Mini process stack
; rem (ebp - 4)
; ebp 
; return address 
; 13 (ebp + 8)


  mov eax, [ebp+8]  ; Move n = 13 into eax 
  mov [ebp-4], eax    ; Move 13 into 'rem' which is ebp - 4 
L1: 
  mov eax, [ebp-4]    ; Rem variable 
  mov ebx,0           ; 0 to keep track of loop 
  cmp eax, ebx 
  jle L2              ; If rem is <= 0, go to L2  
  sub eax, 2          ; eax = eax - 2 
  mov [ebp-4], eax    ; Move back into rem 
  jmp L1              ; Jump back to L1 to compare again
L2:
  xor eax, eax 
  xor ebx, ebx        ; Clear registers 
  mov eax, [ebp-4]    ; Load up rem into register
  cmp eax, 0          ; 
  je  EVEN            ; rem can only be -1 or 0. If 0, it is even 
  xor eax, eax        ; Clear registers 
  xor ebx, ebx 
  ret 1               ; 

EVEN: 
  xor eax, eax      
  xor ebx, ebx 
  ret 0 