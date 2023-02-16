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

		MOV AX, 1		 ;integer found
		PUSH AX
		POP AX
		MOV i, AX 
		PUSH i
		POP AX
		MOV i, AX 
		PUSH i
		MOV AX, i		; ax =  i
		CALL PRINT_NUMBER
		CALL NEWLINE

		MOV AX, 5		 ;integer found
		PUSH AX
		MOV AX, 8		 ;integer found
		PUSH AX
		POP AX		;8 popped
		POP CX		;5 popped
		ADD CX, AX
		PUSH CX
		POP AX
		MOV j, AX 
		PUSH j
		POP AX
		MOV j, AX 
		PUSH j
		MOV AX, j		; ax =  j
		CALL PRINT_NUMBER
		CALL NEWLINE

		MOV AX, i       ; i 
		MOV AX, 2		 ;integer found
		PUSH AX
		MOV AX, j       ; j 
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
		MOV [BP-2], AX		; move to k
		PUSH [BP-2]
		MOV AX, [BP -2]		; ax =  k 
		CALL PRINT_NUMBER
		CALL NEWLINE

		MOV AX, [BP-2]      ; k accessed 
		PUSH AX
		MOV AX, 9		 ;integer found
		PUSH AX
		POP CX
		POP AX
		CWD
		XOR DX, DX	;clearing DX
		DIV CX
		PUSH DX
		POP AX
		MOV [BP-6], AX		; move to m
		PUSH [BP-6]
		MOV AX, [BP -6]		; ax =  m 
		CALL PRINT_NUMBER
		CALL NEWLINE

		MOV AX, [BP-6]      ; m accessed 
		PUSH AX
		MOV AX, [BP-4]      ; ll accessed 
		PUSH AX
		POP BX		; ll popped
		POP AX		; m popped
		;CHECKING IF
		CMP AX, BX
		JLE label1
		PUSH 0
		JMP label2 
	label1:
		PUSH 1
	label2:
		POP AX
		MOV [BP-8], AX		; move to n
		PUSH [BP-8]
		MOV AX, [BP -8]		; ax =  n 
		CALL PRINT_NUMBER
		CALL NEWLINE

		MOV AX, i       ; i 
		MOV AX, j       ; j 
		POP BX		; j popped
		POP AX		; i popped
		;CHECKING IF
		CMP AX, BX
		JNE label3
		PUSH 0
		JMP label4 
	label3:
		PUSH 1
	label4:
		POP AX
		MOV [BP-10], AX		; move to o
		PUSH [BP-10]
		MOV AX, [BP -10]		; ax =  o 
		CALL PRINT_NUMBER
		CALL NEWLINE

		MOV AX, [BP-8]      ; n accessed 
		PUSH AX
		MOV AX, [BP-10]      ; o accessed 
		PUSH AX
		POP BX		; o popped
		POP AX		; n popped
		CMP AX, 0		; if ax = 1
		JNE label5 
		CMP BX, 0		; if ax = 1
		JNE label5 
		MOV AX, 0
		JMP label6 
	label5: 
		MOV AX, 1
	label6: 
		PUSH AX
		POP AX
		MOV [BP-12], AX		; move to p
		PUSH [BP-12]
		MOV AX, [BP -12]		; ax =  p 
		CALL PRINT_NUMBER
		CALL NEWLINE

		MOV AX, [BP-8]      ; n accessed 
		PUSH AX
		MOV AX, [BP-10]      ; o accessed 
		PUSH AX
		POP BX		; o popped
		POP AX		; n popped
		CMP AX, 0		; if ax = 1
		JE label7 
		CMP BX, 0		; if ax = 1
		JE label7 
		MOV AX, 1
		JMP label8 
	label7: 
		MOV AX, 0
	label8: 
		PUSH AX
		POP AX
		MOV [BP-12], AX		; move to p
		PUSH [BP-12]
		MOV AX, [BP -12]		; ax =  p 
		CALL PRINT_NUMBER
		CALL NEWLINE

		MOV AX, [BP - 12]		; ax = p
		INC AX		; p--
		MOV [BP - 12], AX
		PUSH AX
		MOV AX, [BP -12]		; ax =  p 
		CALL PRINT_NUMBER
		CALL NEWLINE

		MOV AX, [BP-12]      ; p accessed 
		PUSH AX
		POP AX			; p popped
		NEG AX
		PUSH AX
		POP AX
		MOV [BP-2], AX		; move to k
		PUSH [BP-2]
		MOV AX, [BP -2]		; ax =  k 
		CALL PRINT_NUMBER
		CALL NEWLINE

		MOV AX, 0		 ;integer found
		PUSH AX
		ADD SP ,12
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

