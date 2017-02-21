.386
.MODEL flat, STDCALL

GetStdHandle PROTO :DWORD
WriteConsoleA PROTO :DWORD,:DWORD,:DWORD,:DWORD,:DWORD
ReadConsoleA  PROTO :DWORD,:DWORD,:DWORD,:DWORD,:DWORD
ExitProcess PROTO :DWORD

.data
	STD_OUTPUT_HANDLE                    equ -11
	STD_INPUT_HANDLE                    equ -10

	prompt DB "Wprowadz liczbe: ", 0
	promptL	DD	$ - prompt 


	text1 DD 0

	source BYTE 10 dup ( 0 )
	destination  BYTE 20 dup ( 0 )

	tab1 DW 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1
	tab2 DW 1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1

	varA			DD  0;

	rout			DD	0 ; faktyczna liczba wyprowadzonych znak�w
	hout 			DD 	0 ; uchwyt wejscia
	hin				DD  0 ; uchwyt wyjscia
	nOfCharsRead	DD  0 ; uchwyt wyjscia

.code


main proc

	push	STD_OUTPUT_HANDLE
	call	GetStdHandle	; wywo�anie funkcji GetStdHandle
	mov		hout, EAX	; deskryptor wyj�ciowego bufora konsoli
	
	push	STD_INPUT_HANDLE
	call	GetStdHandle	; wywo�anie funkcji GetStdHandle
	mov		hin, EAX	; deskryptor wyj�ciowego bufora konsoli
	
	push	0				; rezerwa, musi by� zero
	push	OFFSET rout 	; wska�nik na faktyczn� liczba wyprowadzonych znak�w 
	push	promptL			; liczba znak�w
	push	OFFSET prompt 	; wska�nik na tekst
 	push	hout			; deskryptor buforu konsoli
	call	WriteConsoleA	; wywo�anie funkcji WriteConsoleA   

	push	0						; rezerwa, musi by� zero
	push	OFFSET  nOfCharsRead	; adres do miejsca pami�ci do kt�rego zapisana zostanie liczba zczytanych znak�w
	push	10						; liczba znak�w kt�ra ma zosta� odczytana z konsoli, 
	push	OFFSET  source			; adress tablicy do kt�rej zapisane zostan� odczytane znaki
	push	hin						; uchwyt wej�ciowy do konsoli, pobierany za pomoc� procedury GetStdHandle(4.1), 
	call	ReadConsoleA 
	mov		EBX, OFFSET inputBuffer 
	add		EBX, nOfCharsRead
	mov		[EBX-2], BYTE PTR 0 
	MOV		EAX, offset source 
	push	offset source 
	call	atoi 
	mov		varA , EAX



	
main endp


atoi proc uses esi edx inputBuffAddr:DWORD
	mov esi, inputBuffAddr
	xor edx, edx
	xor EAX, EAX
	mov AL, BYTE PTR [esi]
	cmp eax, 2dh
	je parseNegative

	.Repeat
		lodsb
		.Break .if !eax
		imul edx, edx, 10
		sub eax, "0"
		add edx, eax
	.Until 0
	mov EAX, EDX
	jmp endatoi

	parseNegative:
	inc esi
	.Repeat
		lodsb
		.Break .if !eax
		imul edx, edx, 10
		sub eax, "0"
		add edx, eax
	.Until 0

	xor EAX,EAX
	sub EAX, EDX
	jmp endatoi

	endatoi:
	ret
atoi endp

END


