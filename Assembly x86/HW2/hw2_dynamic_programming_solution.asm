myss SEGMENT PARA STACK 's'
    DW 100 DUP(?)            
myss ENDS

myds SEGMENT PARA 'd'
    sayi DW 10                
    results DW 100 DUP(?)    
myds ENDS

mycs SEGMENT PARA 'code'
    ASSUME CS:mycs, DS:myds, SS:myss

MAIN PROC FAR   
    PUSH    DS               
    XOR    AX, AX
    PUSH   AX                 
    MOV    AX, myds
    MOV    DS, AX             

    MOV     AX, 0
    MOV     DI, OFFSET results
    MOV     CX, 100
INIT_RESULTS:
    MOV     [DI], AX         
    ADD     DI, 2
    LOOP    INIT_RESULTS

    PUSH    sayi              
    CALL    FAR PTR CONWAY   
    POP     AX               
    CALL    FAR PTR PRINTINT 

    RETF                      
MAIN ENDP

CONWAY PROC FAR
    PUSH BP                  
    PUSH AX                   
    PUSH DX
    PUSH BX
    PUSH SI
    PUSH DI

    MOV BP, SP                
    MOV AX, [BP+12]           

    MOV SI, AX
    SHL SI, 1                
    MOV DI, OFFSET results
    ADD DI, SI               

    CMP AX, 0
    JNE l1
    MOV AX, 0                
    JMP son

l1: 
    CMP AX, 1
    JNE l2
l3: MOV AX, 1                 
    JMP son

l2: 
    CMP AX, 2
    JE l3                    

    MOV BX, [DI]              
    CMP BX, 0
    JNE cached

    DEC AX 
    PUSH AX 
    CALL  FAR PTR CONWAY      
    POP BX 
    PUSH BX 
    CALL  FAR PTR CONWAY      
    POP DX 
    INC AX
    SUB AX, BX 
    PUSH AX
    CALL  FAR PTR CONWAY     
    POP AX 
    ADD AX, DX              

    MOV [DI], AX             
    JMP son

cached:
    MOV AX, BX               

son: 
    MOV [BP+12],AX            
    POP DI                  
    POP SI
    POP BX
    POP DX
    POP AX
    POP BP

    RETF                    
CONWAY ENDP

PRINTINT PROC FAR
    PUSH AX
    PUSH BX
    PUSH CX
    PUSH DX

    MOV BX, 10              

print_loop:
    XOR DX, DX
    DIV BX                   
    PUSH DX                  
    OR  AX, AX
    JNZ print_loop

print_digits:
    POP DX                   
    ADD DL, '0'              
    MOV AH, 02h
    INT 21h                  
    OR  SP, SP
    JNZ print_digits         

    POP DX                  
    POP CX
    POP BX
    POP AX

    MOV AH, 4Ch
    INT 21h                              
    RETF                                     
PRINTINT ENDP

mycs ENDS
     END MAIN
