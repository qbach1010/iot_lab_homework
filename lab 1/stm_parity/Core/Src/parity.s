.syntax unified
  .cpu cortex-m3
  .thumb
  .text

  .global parity
  .thumb_func


parity:
	//R0 chua 3 bit tu cong tac
	//R1 chua dia chi tra ket qua
	EOR R2, R0, R0 , LSR #1
	EOR R2, R2, R0 , LSR #2
	AND R2, R2, #1

	EOR R3, R2, #1

	STR R3, [R1] //tra ve odd parity, = 1 -> odd, =0 -> even

	BX LR
