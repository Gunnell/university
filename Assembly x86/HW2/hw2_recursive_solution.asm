myss        SEGMENT PARA STACK 's'
            DW 100 DUP(?)
myss        ENDS

myds        SEGMENT PARA 'd'
sayi DW 10
myds        ENDS

mycs        SEGMENT PARA 'code'
            ASSUME CS:mycs, DS:myds, SS:myss

MAIN PROC FAR
   PUSH    DS
   XOR     AX, AX
   PUSH    AX
   MOV     AX, myds
   MOV     DS, AX

    PUSH    sayi
    CALL    FAR PTR CONWAY 
    ADD     SP, 2          ; Clean up the stack after CONWAY call
    CALL    FAR PTR PRINTINT        

    RETF
MAIN ENDP

CONWAY PROC FAR
PUSH BP
MOV BP, SP
PUSH AX
PUSH BX
PUSH CX
PUSH DX

MOV AX, [BP+6]  ; Get the input parameter from the stack

CMP AX, 0
JE  zero_case
CMP AX, 1
JE  one_case
CMP AX, 2
JE  two_case

DEC AX 
PUSH AX 
CALL  FAR PTR CONWAY 
ADD SP, 2  ; Clean up the stack after CONWAY call
MOV BX, AX 

DEC AX 
PUSH AX 
CALL  FAR PTR CONWAY  
ADD SP, 2  ; Clean up the stack after CONWAY call
MOV CX, AX 

SUB BX, CX 
PUSH BX 
CALL  FAR PTR CONWAY 
ADD SP, 2  ; Clean up the stack after CONWAY call

ADD AX, CX
JMP done

zero_case:
MOV AX, 0
JMP done

one_case:
MOV AX, 1
JMP done

two_case:
MOV AX, 1
JMP done

done:
POP DX
POP CX
POP BX
POP AX
POP BP
RETF                  
CONWAY ENDP

PRINTINT PROC FAR
    PUSH AX
    PUSH DX

    ; Convert AX to decimal and print
    MOV CX, 0
    MOV BX, 10

print_loop:
    XOR DX, DX
    DIV BX
    PUSH DX
    INC CX
    TEST AX, AX
    JNZ print_loop

print_digits:
    POP DX
    OR DL, '0'
    MOV AH, 02h
    INT 21h
    LOOP print_digits

    POP DX
    POP AX
    RETF                   
PRINTINT ENDP

mycs ENDS
END MAIN
