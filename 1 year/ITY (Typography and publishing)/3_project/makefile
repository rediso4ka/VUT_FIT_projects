# Projekt: Typografie a publikovani - 3
# Autor:   Aleksandr Shevchenko
# Datum:   02.04.2022 

all:
	latex proj3.tex
	latex proj3.tex
	dvips -t a4 proj3.dvi -o proj3.ps
	ps2pdf -sPAPERSIZE=a4 proj3.ps
clean:
	rm -rf proj3.ps proj3.pdf proj3.out proj3.log proj3.dvi proj3.aux
