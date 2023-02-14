.MODEL SMALL
.STACK 500H
.DATA
	CR EQU 0DH
	NL EQU 0AH
.CODE
	main PROC

		MOV AX, @DATA
		MOV DS, AX
		PUSH BP
		MOV BP, SP 


		SUB SP, 2  	;variable i declared 
		SUB SP, 2  	;variable j declared 
		SUB SP, 2  	;variable k declared 
		SUB SP, 2  	;variable ll declared 
		MOV CX, 0		 ;integer found
		PUSH CX

		POP AX
		MOV [BP-2], AX		; move to i
		PUSH [BP-2]

	label1:
		MOV CX, [BP-2]      ; i accessed 
		PUSH CX

		MOV CX, 6		 ;integer found
		PUSH CX

		POP BX		; 6 popped
		POP AX		; i popped
		;CHECKING IF
		CMP AX, BX
		JL label2
		PUSH 0
		JMP label3 
	label2:
		PUSH 1
	label3:
		POP AX
		CMP AX, 0
		JNE label5
		JMP label4
	label5:
		MOV AX, [BP -2]		; ax =  i 
		CALL PRINT_NUMBER
		CALL NEWLINE

		MOV AX, [BP - 2]		; ax = i
		INC AX		; i++
		MOV [BP - 2]AX
		PUSH AX

		JMP label1
	label4:
		MOV CX, 4		 ;integer found
		PUSH CX

		POP AX
		MOV [BP-6], AX		; move to k
		PUSH [BP-6]

		MOV CX, 6		 ;integer found
		PUSH CX

		POP AX
		MOV [BP-8], AX		; move to ll
		PUSH [BP-8]

	label6:
		MOV CX, [BP-6]      ; k accessed 
		PUSH CX

		MOV CX, 0		 ;integer found
		PUSH CX

		POP BX		; 0 popped
		POP AX		; k popped
		;CHECKING IF
		CMP AX, BX
		JG label7
		PUSH 0
		JMP label8 
	label7:
		PUSH 1
	label8:
		POP AX
		CMP AX, 0
		JNE label10
		JMP label9
	label10:
		MOV CX, [BP-8]      ; ll accessed 
		PUSH CX

		MOV CX, 3		 ;integer found
		PUSH CX

		POP AX		;3 popped
		POP CX		;ll popped
		ADD CX, AX
		PUSH CX

		POP AX
		MOV [BP-8], AX		; move to ll
		PUSH [BP-8]

		MOV AX, [BP - 6]		; ax = k
		DEC AX		; k--
		MOV [BP - 6]AX
		PUSH AX

		JMP label6
	label9:
		MOV AX, [BP -8]		; ax =  ll 
		CALL PRINT_NUMBER
		CALL NEWLINE

		MOV AX, [BP -6]		; ax =  k 
		CALL PRINT_NUMBER
		CALL NEWLINE

		MOV CX, 4		 ;integer found
		PUSH CX

		POP AX
		MOV [BP-6], AX		; move to k
		PUSH [BP-6]

		MOV CX, 6		 ;integer found
		PUSH CX

		POP AX
		MOV [BP-8], AX		; move to ll
		PUSH [BP-8]

	label11:
		MOV AX, [BP - 6]		; ax = k
		DEC AX		; k--
		MOV [BP - 6]AX
		PUSH AX

		POP AX
		INC AX
		CMP AX, 0
		JNE label13
		JMP label12
	label13:
		MOV CX, [BP-8]      ; ll accessed 
		PUSH CX

		MOV CX, 3		 ;integer found
		PUSH CX

		POP AX		;3 popped
		POP CX		;ll popped
		ADD CX, AX
		PUSH CX

		POP AX
		MOV [BP-8], AX		; move to ll
		PUSH [BP-8]

		JMP label11
	label12:
		MOV AX, [BP -8]		; ax =  ll 
		CALL PRINT_NUMBER
		CALL NEWLINE

		MOV AX, [BP -6]		; ax =  k 
		CALL PRINT_NUMBER
		CALL NEWLINE

		MOV CX, 0		 ;integer found
		PUSH CX

		ADD SP, 8	;freeing the stack of the local variables
		MOV SP, BP

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

