.MODEL SMALL
.STACK 500H
.DATA
	CR EQU 0DH
	NL EQU 0AH
	i DW ? 	;global variable i declared
	j DW ? 	;global variable j declared


.CODE
	main PROC

		MOV AX, @DATA
		MOV DS, AX
		PUSH BP
		MOV BP, SP 
		SUB SP, 2  	;variable k declared 
		SUB SP, 2  	;variable ll declared 
		SUB SP, 2  	;variable m declared 
		SUB SP, 2  	;variable n declared 
		SUB SP, 2  	;variable o declared 
		SUB SP, 2  	;variable p declared 
		MOV CX, 1
		PUSH CX

		POP AX
		MOV i, AX 
		PUSH i

		POP AX
		CALL PRINT_NUMBER
		CALL NEWLINE

		MOV CX, 5
		PUSH CX

		MOV CX, 8
		PUSH CX

		POP AX		;8 popped
		POP CX		;5 popped
		ADD CX, AX
		PUSH CX

		POP AX
		MOV j, AX 
		PUSH j

		POP AX
		CALL PRINT_NUMBER
		CALL NEWLINE

		MOV CX, i
		PUSH CX

		MOV CX, 2
		PUSH CX

		MOV CX, j
		PUSH CX

		POP CX
		POP AX
		CWD
		MUL CX
		PUSH AX

		POP AX		;2*j popped
		POP CX		;i popped
		ADD CX, AX
		PUSH CX

		POP AX
		MOV [BX], AX
		PUSH [BX]

		POP AX
		CALL PRINT_NUMBER
		CALL NEWLINE

		MOV CX, [BX]
		PUSH CX

		MOV CX, 9
		PUSH CX

		POP CX
		POP AX
		CWD
		XOR DX, DX	;clearing DX
		DIV CX
		PUSH DX

		POP AX
		MOV [BX-8], AX
		PUSH [BX-8]

		POP AX
		CALL PRINT_NUMBER
		CALL NEWLINE

		ADD SP, 12	;freeing the stack of the local variables
		POP BP	; restoring BP


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
		MOV AH, 2           
		MOV DL, '-'         ;Print a '-' sign
		INT 21H
		NEG AX              ;make AX positive
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
			;Print newline
			MOV DL, 0DH
			INT 21H
			MOV DL, 0AH
			INT 21H
			POP AX
			POP DX
			POP CX
			POP BX
			RET 
		PRINT_INTEGER ENDP
END MAIN

