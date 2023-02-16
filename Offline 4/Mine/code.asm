.MODEL SMALL
.STACK 500H
.DATA
	CR EQU 0DH
	NL EQU 0AH
.CODE
	f PROC
		;starting procedure f
		PUSH BP		;save BP
		MOV BP, SP		;make BP = SP

		SUB SP, 2  	;variable k declared

		MOV AX, 5		 ;integer found
		PUSH AX
		POP AX
		MOV [BP-2], AX		; move to k
		PUSH [BP-2]
	label1:
		MOV AX, [BP-2]      ; k accessed 
		PUSH AX
		MOV AX, 0		 ;integer found
		PUSH AX
		POP BX		; 0 popped
		POP AX		; k popped
		;CHECKING IF
		CMP AX, BX
		JG label2
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
		MOV AX, [BP + 4]		; ax = a
		INC AX		; a--
		MOV [BP + 4], AX
		PUSH AX
		MOV AX, [BP - 2]		; ax = k
		DEC AX		; k--
		MOV [BP + 2], AX
		PUSH AX
		JMP label1
	label4:
		MOV AX, 3		 ;integer found
		PUSH AX
		MOV AX, [BP+4]      ; a accessed 
		PUSH AX
		POP CX
		POP AX
		CWD
		MUL CX
		PUSH AX
		MOV AX, 7		 ;integer found
		PUSH AX
		POP AX		;7 popped
		POP CX		;3*a popped
		SUB CX, AX
		PUSH CX
		MOV AX, 9		 ;integer found
		PUSH AX
		POP AX
		MOV [BP+4], AX		; move to a
		PUSH [BP+4]
		POP AX
		ADD SP, 4	;freeing the stack of the local variables
		MOV SP, BP
		POP BP
		RET
	f ENDP

	g PROC
		;starting procedure g
		PUSH BP		;save BP
		MOV BP, SP		;make BP = SP

		SUB SP, 2  	;variable x declared
		SUB SP, 2  	;variable i declared


		MOV AX, [BP+6]		; a accessed 
		PUSH AX		; pushed a
		CALL f

		MOV AX, [BP+6]      ; a accessed 
		PUSH AX
		POP AX		;a popped
		POP CX		;f(a) popped
		ADD CX, AX
		PUSH CX
		MOV AX, [BP+4]      ; b accessed 
		PUSH AX
		POP AX		;b popped
		POP CX		;f(a)+a popped
		ADD CX, AX
		PUSH CX
		MOV [BP-2], AX		; move to x
		PUSH [BP-2]
		MOV AX, 0		 ;integer found
		PUSH AX
		POP AX
		MOV [BP-4], AX		; move to i
		PUSH [BP-4]
	label6:
		MOV AX, [BP-4]      ; i accessed 
		PUSH AX
		MOV AX, 7		 ;integer found
		PUSH AX
		POP BX		; 7 popped
		POP AX		; i popped
		;CHECKING IF
		CMP AX, BX
		JL label7
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
		MOV AX, [BP - 4]		; ax = i
		INC AX		; i--
		MOV [BP + 4], AX
		PUSH AX
		JMP label6
	label9:
		MOV AX, [BP-4]      ; i accessed 
		PUSH AX
		MOV AX, 3		 ;integer found
		PUSH AX
		POP CX
		POP AX
		CWD
		XOR DX, DX	;clearing DX
		DIV CX
		PUSH DX
		MOV AX, 0		 ;integer found
		PUSH AX
		POP BX		; 0 popped
		POP AX		; i%3 popped
		;CHECKING IF
		CMP AX, BX
		JE label11
		PUSH 0
		JMP label12 
	label11:
		PUSH 1
	label12:
		POP AX
		CMP AX, 0
		JNE label13
		JMP label14
	label13:
		MOV AX, [BP-2]      ; x accessed 
		PUSH AX
		MOV AX, 5		 ;integer found
		PUSH AX
		POP AX		;5 popped
		POP CX		;x popped
		ADD CX, AX
		PUSH CX
		POP AX
		MOV [BP-2], AX		; move to x
		PUSH [BP-2]
		JMP label15
	label14:
		MOV AX, [BP-2]      ; x accessed 
		PUSH AX
		MOV AX, 1		 ;integer found
		PUSH AX
		POP AX		;1 popped
		POP CX		;x popped
		SUB CX, AX
		PUSH CX
		POP AX
		MOV [BP-2], AX		; move to x
		PUSH [BP-2]
	label15:
		MOV AX, [BP-2]      ; x accessed 
		PUSH AX
		POP AX
		ADD SP, 8	;freeing the stack of the local variables
		MOV SP, BP
		POP BP
		RET
	g ENDP

	main PROC
		MOV AX, @DATA
		MOV DS, AX
		PUSH BP
		MOV BP, SP 


		SUB SP, 2  	;variable a declared
		SUB SP, 2  	;variable b declared
		SUB SP, 2  	;variable i declared

		MOV AX, 1		 ;integer found
		PUSH AX
		POP AX
		MOV [BP-2], AX		; move to a
		PUSH [BP-2]
		MOV AX, 2		 ;integer found
		PUSH AX
		POP AX
		MOV [BP-4], AX		; move to b
		PUSH [BP-4]

		MOV AX, [BP-2]		 ; access  a
		PUSH AX		; pushed a
		MOV AX, [BP-4]		 ; access  b
		PUSH AX		; pushed b
		CALL g

		MOV [BP-2], AX		; move to a
		PUSH [BP-2]
		MOV AX, [BP -2]		; ax =  a 
		CALL PRINT_NUMBER
		CALL NEWLINE

		MOV AX, 0		 ;integer found
		PUSH AX
		POP AX
		MOV [BP-6], AX		; move to i
		PUSH [BP-6]
	label16:
		MOV AX, [BP-6]      ; i accessed 
		PUSH AX
		MOV AX, 4		 ;integer found
		PUSH AX
		POP BX		; 4 popped
		POP AX		; i popped
		;CHECKING IF
		CMP AX, BX
		JL label17
		PUSH 0
		JMP label18 
	label17:
		PUSH 1
	label18:
		POP AX
		CMP AX, 0
		JNE label20
		JMP label19
	label20:
		MOV AX, [BP - 6]		; ax = i
		INC AX		; i--
		MOV [BP + 6], AX
		PUSH AX
		JMP label16
	label19:
		MOV AX, 3		 ;integer found
		PUSH AX
		POP AX
		MOV [BP-2], AX		; move to a
		PUSH [BP-2]
	label21:
		MOV AX, [BP-2]      ; a accessed 
		PUSH AX
		MOV AX, 0		 ;integer found
		PUSH AX
		POP BX		; 0 popped
		POP AX		; a popped
		;CHECKING IF
		CMP AX, BX
		JG label22
		PUSH 0
		JMP label23 
	label22:
		PUSH 1
	label23:
		POP AX
		CMP AX, 0
		JNE label25
		JMP label24
	label25:
		MOV AX, [BP - 4]		; ax = b
		INC AX		; b--
		MOV [BP + 4], AX
		PUSH AX
		MOV AX, [BP - 2]		; ax = a
		DEC AX		; a--
		MOV [BP + 2], AX
		PUSH AX
		JMP label21
	label24:
		MOV AX, [BP -2]		; ax =  a 
		CALL PRINT_NUMBER
		CALL NEWLINE

		MOV AX, [BP -4]		; ax =  b 
		CALL PRINT_NUMBER
		CALL NEWLINE

		MOV AX, [BP -6]		; ax =  i 
		CALL PRINT_NUMBER
		CALL NEWLINE

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

