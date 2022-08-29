.include "/home/surabhi_22/test/codes/m328Pdef.inc"
 
.def X = R26 
.def Y = R25 
.def Z = R18 
.def W = R19 
.def F = R20 
.def t = R21
.def mask = R23 
.def result = R24 
 
ldi R16, 0b00000000 
out DDRD, R16 
ldi R16, 0b00100000 
out DDRB, R16 
 
ldi mask, 0b00000001 
 
start: 
 
in R17, PIND 
lsr R17 
lsr R17 
mov X, R17 
and X, mask 
lsr R17 
mov Y, R17 
and Y, mask 
lsr R17 
mov Z, R17 
and Z, mask 
lsr R17 
mov W, R17 
and W, mask 

mov result, X
mov t, Y ; t = Y
eor t, mask ; t = Y’ 
and result, t ; result = XY’

mov t, X ; t = X
and t, W ; t = XW
or result, t ; result = XY’ + XW

mov t, X ; t= X
and t, Z ; t= XZ
or result, t ; result = XY’ + XW + XZ

mov t, Y ; t = Y
and t, Z ; t = YZ
or result, t ; t = XY’ + XW + XZ + YZ

mov t, W ; t = W
eor t, mask ; t = W’
and t, Z ; t = ZW’
or result, t ; t = XY’ + XW + XZ + YZ + ZW’

lsl result 
lsl result 
lsl result 
lsl result 
lsl result
out PORTB, result
