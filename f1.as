MAIN:   mov S1.1 ,LENGTH 
        add r2 
LOOP:   jmp END,Or
        macro m1
          inc K
          mov S1.2 ,r3
        endmacro 
        prn #-5
        sub r1, r3
        m1
        bne LOOP
END:    hlt 
STR:    .string "ab"
LENGTH: .data 6,-9,15 
K:      .data 22,1,1
S1:     .struct 8, "ab"

