all:
	xelatex -shell-escape main.tex
	xelatex -shell-escape main.tex
	rifle main.pdf
clean:
	rm main.log main.toc main.aux
