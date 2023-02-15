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
		;starting procedure func_a
		PUSH BP		;save BP
		MOV BP, SP		;make BP = SP

		MOV CX, 7		 ;integer found
		PUSH CX
		POP AX
		MOV a, AX 
		PUSH a
		MOV CX, a       ; a accessed
		PUSH CX
		POP AX
		MOV SP, BP
		POP BP
		RET
	func_a ENDP

	main PROC
		MOV AX, @DATA
		MOV DS, AX
		PUSH BP
		MOV BP, SP 


		CALL func_a
		POP AX
		MOV a, AX 
		PUSH a
		MOV AX, a		; ax =  a
		CALL PRINT_NUMBER
		CALL NEWLINE
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
		;if(AX < -1) then the number is positive
		CMP AX, 0
		JGE POSITIVE
		;else, the number is negative
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
			;quotient is in AX and remainder is in DX
			PUSH DX     ;Division by 10 will have a remainder less than 8 bits
			INC CX       ;CX++
			;if(AX == 0) then break the loop
			CMP AX, 0
			JE END_PUSH_WHILE
		;else continue
			JMP PUSH_WHILE
		END_PUSH_WHILE:
			MOV AH, 2
		POP_WHILE:
			POP DX      ;Division by 10 will have a remaainder less than 8 bits
			ADD DL, '0'
			INT 21H     ;So DL will have the desired character
			DEC CX       ;CX--
			;if(CX <= 0) then end loop
			CMP CX, 0
			JLE END_POP_WHILE
			;else continue
			JMP POP_WHILE
			END_POP_WHILE:
			POP AX
			POP DX
			POP CX
			POP BX
			RET 
	PRINT_NUMBER ENDP
END MAIN

