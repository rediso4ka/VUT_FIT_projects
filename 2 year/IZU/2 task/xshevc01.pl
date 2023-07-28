% Zadani c. 37:
% Napiste program resici ukol dany predikatem u37(LIN,VIN), kde LIN je vstupni 
% celociselny seznam a VIN je vstupni promenna obsahujici libovolne prirozene 
% cislo. Predikat je pravdivy (ma hodnotu true), pokud pocet lichych cisel 
% v seznamu LIN neni vetsi nez cislo v promenne VIN, jinak je predikat 
% nepravdivy (ma hodnotu false).

% Testovaci predikaty:                       			 
u37_1:- u37([4,-3,7,8,3,7,0,-20],2).			% false
u37_2:- u37([4,-3,7,8,3,7,0,-20],5).			% true
u37_3:- u37([],2).					% true
u37_r:- write('Zadej LIN: '),read(LIN),
	write('Zadej VIN: '),read(VIN),
	u37(LIN,VIN).

% Reseni:
u37([],N):- N >= 0.
u37([Head | Tail],N):- (0 is Head mod 2, u37(Tail,N)), ! ; (1 is Head mod 2, u37(Tail, N-1)).