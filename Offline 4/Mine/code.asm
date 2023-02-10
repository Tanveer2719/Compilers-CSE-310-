.MODEL SMALL
.STACK 500H
.DATA
	CR EQU 0DH
	NL EQU 0AH
	i DW ? 	;global variable i declared
	j DW ? 	;global variable j declared


.CODE


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
		PUSH CX
		PUSH BX
		PUSH DX
		PUSH AX
		MOV CX, 0
		MOV DX,0

		EXTRACT: 
			CMP AX, 0
			JE SHOW
			MOV BX, 10
			DIV BX
			PUSH DX
			XOR DX,DX
			INC CX
		JMP EXTRACT

		SHOW: 
			CMP CX, 0
			JE E
			POP DX
			ADD DX,48
			MOV AH,2
			INT 21H
			DEC CX
		JMP SHOW

		E: 
			POP AX
			POP DX
			POP BX
			POP CX
		RET
	PRINT_NUMBER ENDP

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
		XOR DX	;clearing DX
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

END MAIN

