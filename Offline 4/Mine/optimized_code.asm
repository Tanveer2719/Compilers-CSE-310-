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
		MOV AX, 0		 ;integer found
		MOV [BP-2], AX		; move to i
		PUSH [BP-2]
	label1:				; label for checking boolean expression
		MOV AX, [BP-2]      ; i accessed 
		PUSH AX
		MOV AX, 6		 ;integer found
		MOV BX, AX
		POP AX		; i popped
		CMP AX, BX
		JL label2
		PUSH 0
		JMP label3 
	label2:
		PUSH 1
	label3:
		POP AX
		CMP AX, 0
		JNE label5		; move to b_true
		JMP label4		; move to b_false
		label6:				; label for incrementing or decrementing
		MOV AX, [BP - 2]		; ax = i
		INC AX		; i--
		MOV [BP - 2], AX
		PUSH AX
		JMP label1		; move to the condition check again
	label5:				 label for b_true
		MOV AX, [BP -2]		; ax =  i 
		CALL PRINT_NUMBER
		CALL NEWLINE
		JMP label6				;go to incrementing or decrementing
	label4:				 label for statements.next
		MOV AX, 4		 ;integer found
		MOV [BP-6], AX		; move to k
		PUSH [BP-6]
		MOV AX, 6		 ;integer found
		MOV [BP-8], AX		; move to ll
		PUSH [BP-8]
	label7:
		MOV AX, [BP-6]      ; k accessed 
		PUSH AX
		MOV AX, 0		 ;integer found
		MOV BX, AX
		POP AX		; k popped
		CMP AX, BX
		JG label8
		PUSH 0
		JMP label9 
	label8:
		PUSH 1
	label9:
		POP AX
		CMP AX, 0
		JNE label11
		JMP label10
	label11:
		MOV AX, [BP-8]      ; ll accessed 
		PUSH AX
		MOV AX, 3		 ;integer found
		POP CX		;ll popped
		ADD AX, CX
		MOV [BP-8], AX		; move to ll
		PUSH [BP-8]
		MOV AX, [BP - 6]		; ax = k
		DEC AX		; k--
		MOV [BP - 6], AX
		PUSH AX
		JMP label7
	label10:
		MOV AX, [BP -8]		; ax =  ll 
		CALL PRINT_NUMBER
		CALL NEWLINE
		MOV AX, [BP -6]		; ax =  k 
		CALL PRINT_NUMBER
		CALL NEWLINE
		MOV AX, 4		 ;integer found
		MOV [BP-6], AX		; move to k
		PUSH [BP-6]
		MOV AX, 6		 ;integer found
		MOV [BP-8], AX		; move to ll
		PUSH [BP-8]
	label12:
		MOV AX, [BP - 6]		; ax = k
		DEC AX		; k--
		MOV [BP - 6], AX
		INC AX
		CMP AX, 0
		JNE label14
		JMP label13
	label14:
		MOV AX, [BP-8]      ; ll accessed 
		PUSH AX
		MOV AX, 3		 ;integer found
		POP CX		;ll popped
		ADD AX, CX
		MOV [BP-8], AX		; move to ll
		PUSH [BP-8]
		JMP label12
	label13:
		MOV AX, [BP -8]		; ax =  ll 
		CALL PRINT_NUMBER
		CALL NEWLINE
		MOV AX, [BP -6]		; ax =  k 
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
