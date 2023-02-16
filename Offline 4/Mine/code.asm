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

		MOV AX, 3		 ;integer found
		PUSH AX
		POP AX
		MOV [BP-2], AX		; move to i
		PUSH [BP-2]
		MOV AX, 8		 ;integer found
		PUSH AX
		POP AX
		MOV [BP-4], AX		; move to j
		PUSH [BP-4]
		MOV AX, 6		 ;integer found
		PUSH AX
		POP AX
		MOV [BP-6], AX		; move to k
		PUSH [BP-6]
		MOV AX, [BP-2]      ; i accessed 
		PUSH AX
		MOV AX, 3		 ;integer found
		PUSH AX
		POP BX		; 3 popped
		POP AX		; i popped
		;CHECKING IF
		CMP AX, BX
		JE label1
		PUSH 0
		JMP label2 
	label1:
		PUSH 1
	label2:
		POP AX
		CMP AX, 0
		JNE label3
		JMP label4
	label3:
		MOV AX, [BP -4]		; ax =  j 
		CALL PRINT_NUMBER
		CALL NEWLINE

	label4:
		MOV AX, [BP-4]      ; j accessed 
		PUSH AX
		MOV AX, 8		 ;integer found
		PUSH AX
		POP BX		; 8 popped
		POP AX		; j popped
		;CHECKING IF
		CMP AX, BX
		JL label5
		PUSH 0
		JMP label6 
	label5:
		PUSH 1
	label6:
		POP AX
		CMP AX, 0
		JNE label7
		JMP label8
	label7:
		MOV AX, [BP -2]		; ax =  i 
		CALL PRINT_NUMBER
		CALL NEWLINE

		JMP label9
	label8:
		MOV AX, [BP -6]		; ax =  k 
		CALL PRINT_NUMBER
		CALL NEWLINE

	label9:
		MOV AX, [BP-6]      ; k accessed 
		PUSH AX
		MOV AX, 6		 ;integer found
		PUSH AX
		POP BX		; 6 popped
		POP AX		; k popped
		;CHECKING IF
		CMP AX, BX
		JNE label10
		PUSH 0
		JMP label11 
	label10:
		PUSH 1
	label11:
		POP AX
		CMP AX, 0
		JNE label12
		JMP label13
	label12:
		MOV AX, [BP -6]		; ax =  k 
		CALL PRINT_NUMBER
		CALL NEWLINE

		JMP label14
	label13:
		MOV AX, [BP-4]      ; j accessed 
		PUSH AX
		MOV AX, 8		 ;integer found
		PUSH AX
		POP BX		; 8 popped
		POP AX		; j popped
		;CHECKING IF
		CMP AX, BX
		JG label15
		PUSH 0
		JMP label16 
	label15:
		PUSH 1
	label16:
		POP AX
		CMP AX, 0
		JNE label17
		JMP label18
	label17:
		MOV AX, [BP -4]		; ax =  j 
		CALL PRINT_NUMBER
		CALL NEWLINE

		JMP label19
	label18:
		MOV AX, [BP-2]      ; i accessed 
		PUSH AX
		MOV AX, 5		 ;integer found
		PUSH AX
		POP BX		; 5 popped
		POP AX		; i popped
		;CHECKING IF
		CMP AX, BX
		JL label20
		PUSH 0
		JMP label21 
	label20:
		PUSH 1
	label21:
		POP AX
		CMP AX, 0
		JNE label22
		JMP label23
	label22:
		MOV AX, [BP -2]		; ax =  i 
		CALL PRINT_NUMBER
		CALL NEWLINE

		JMP label24
	label23:
		MOV AX, 0		 ;integer found
		PUSH AX
		POP AX
		MOV [BP-6], AX		; move to k
		PUSH [BP-6]
		MOV AX, [BP -6]		; ax =  k 
		CALL PRINT_NUMBER
		CALL NEWLINE

	label24:
	label19:
	label14:
		MOV AX, 0		 ;integer found
		PUSH AX
		ADD SP ,6
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

