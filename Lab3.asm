	
	THUMB

	PUBLIC EightBitHistogram
	SECTION .text: CODE
 
 
 ;largura R0, Comprimento R1, Bit0_IM R2, Bit0_Hist R3
 
 Start	
		MUL		R0, R0, R1 ;Tamanho da imagem
		CMP 	R0, #65536
		ITGT	
		BXGT	Return_Zero
		
		MOV		R1,#0
		PUSH 	{R4}
		MOV		R4,#0
Loop_Zero
		CMP		R1,#256
		ITTTLT	
		STRB	R4,[R3],#1
		ADDLT	R1,#1
		BLT		Loop_Zero
		
		SUB		R3,#255 		
		MOV		R1,#0
		
Histograma
		LDRB	R4,[R2],#1
		LDRB	R5,[R3,(R4<<4)]
		ADD		R5,#1
		STRB	R5,[R3,(R4<<4)]
		CMP		R1,R0
		ITTLT
		ADDLT	R1,#1
		BLT		Histograma
		B		Final

Return_Zero	;Maior que 64K
		MOV		R0,#0
		BX		LR
		
Final
		POP 	{R4}
		BX		LR