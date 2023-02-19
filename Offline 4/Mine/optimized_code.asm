.MODEL SMALL
.STACK 500H
.DATA
	CR EQU 0DH
	NL EQU 0AH
	a DW ? 	;global variable a declared
	b DW ? 	;global variable b declared
	c DW ? 	;global variable c declared
.CODE
	func_a PROC
		PUSH BP		;save BP
		MOV BP, SP		;make BP = SP
		MOV AX, 7		 ;integer found
		MOV a, AX 
		MOV SP, BP
		POP BP
		RET 0
	func_a ENDP
	foo PROC
		PUSH BP		;save BP
		MOV BP, SP		;make BP = SP
		MOV AX, [BP+4]      ; a accessed 
		PUSH AX
		MOV AX, 3		 ;integer found
		POP CX		;a popped
		ADD AX, CX
		MOV [BP+4], AX		; move to a
		PUSH [BP+4]
		MOV AX, [BP+4]      ; a accessed 
		ADD SP, 2	;freeing the stack of the local variables
		MOV SP, BP
		POP BP
		RET 2
	foo ENDP
	bar PROC
		PUSH BP		;save BP
		MOV BP, SP		;make BP = SP
		MOV AX, 4		 ;integer found
		PUSH AX
		MOV AX, [BP+6]      ; a accessed 
		MOV CX, AX
		POP AX
		CWD
		MUL CX
		PUSH AX
		MOV AX, 2		 ;integer found
		PUSH AX
		MOV AX, [BP+4]      ; b accessed 
		MOV CX, AX
		POP AX
		CWD
		MUL CX
		POP CX		;4*a popped
		ADD AX, CX
		MOV c, AX 
		PUSH c
		MOV AX, c       ; c 
		POP AX
		ADD SP, 4	;freeing the stack of the local variables
		MOV SP, BP
		POP BP
		RET 4
	bar ENDP
	main PROC
		MOV AX, @DATA
		MOV DS, AX
		PUSH BP
		MOV BP, SP 
		SUB SP, 2  	;variable i declared
		SUB SP, 2  	;variable j declared
		SUB SP, 2  	;variable k declared
		SUB SP, 2  	;variable l declared
		MOV AX, 5		 ;integer found
		MOV [BP-2], AX		; move to i
		PUSH [BP-2]
		MOV AX, 6		 ;integer found
		MOV [BP-4], AX		; move to j
		PUSH [BP-4]
		CALL func_a
		PUSH AX
		MOV AX, a		; ax =  a
		CALL PRINT_NUMBER
		CALL NEWLINE
		MOV AX, [BP-2]		 ; access  i
		PUSH AX		; pushed i
		CALL foo
		MOV [BP-6], AX		; move to k
		PUSH [BP-6]
		MOV AX, [BP -6]		; ax =  k 
		CALL PRINT_NUMBER
		CALL NEWLINE
		MOV AX, [BP-2]		 ; access  i
		PUSH AX		; pushed i
		MOV AX, [BP-4]		 ; access  j
		PUSH AX		; pushed j
		CALL bar
		MOV [BP-8], AX		; move to l
		PUSH [BP-8]
		MOV AX, [BP -8]		; ax =  l 
		CALL PRINT_NUMBER
		CALL NEWLINE
		MOV AX, 6		 ;integer found
		PUSH AX
		MOV AX, [BP-2]		 ; access  i
		PUSH AX		; pushed i
		MOV AX, [BP-4]		 ; access  j
		PUSH AX		; pushed j
		CALL bar
		MOV CX, AX
		POP AX
		CWD
		MUL CX
		PUSH AX
		MOV AX, 2		 ;integer found
		POP CX		;6*bar(i,j) popped
		ADD CX, AX
		PUSH CX
		MOV AX, 3		 ;integer found
		PUSH AX
		MOV AX, [BP-2]		 ; access  i
		PUSH AX		; pushed i
		CALL foo
		MOV CX, AX
		POP AX
		CWD
		MUL CX
		POP CX		;6*bar(i,j)+2 popped
		SUB CX, AX
		MOV AX, CX
		MOV [BP-4], AX		; move to j
		PUSH [BP-4]
		MOV AX, [BP -4]		; ax =  j 
		CALL PRINT_NUMBER
		CALL NEWLINE
		MOV AX, 0		 ;integer found
		PUSH AX
		ADD SP ,8
		MOV SP, BP
		POP BP
		MOV AX, 4CH
		INT 21H
	main ENDP
	NEWLINE PROC
		PUSH AX
		PUSH DX
		MOV AH, 2
		MOV DL,CR
		INT 21H
		MOV DL,NL
		INT 21H
		POP DX
		POP AX
		RET
	NEWLINE ENDP
	PRINT_NUMBER PROC 
		PUSH BX
		PUSH CX
		PUSH DX
		PUSH AX
		CMP AX, 0
		JGE POSITIVE
		MOV CX, AX
		MOV AH, 2           
		MOV DL, '-'         ;Print a '-' sign
		INT 21H
		NEG CX              ;make AX positive
		MOV AX, CX
		POSITIVE:
			MOV CX, 0        ;Initialize character count
		PUSH_WHILE:
			XOR DX, DX  ;clear DX
			MOV BX, 10  ;BX has the divisor
			DIV BX
			PUSH DX     ;Division by 10 will have a remainder less than 8 bits
			INC CX       ;CX++
			CMP AX, 0
			JE END_PUSH_WHILE
			JMP PUSH_WHILE
		END_PUSH_WHILE:
			MOV AH, 2
		POP_WHILE:
			POP DX      ;Division by 10 will have a remaainder less than 8 bits
			ADD DL, '0'
			INT 21H     ;So DL will have the desired character
			DEC CX       ;CX--
			CMP CX, 0
			JLE END_POP_WHILE
			JMP POP_WHILE
			END_POP_WHILE:
			POP AX
			POP DX
			POP CX
			POP BX
			RET 
	PRINT_NUMBER ENDP
END MAIN
