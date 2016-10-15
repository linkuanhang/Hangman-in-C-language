out_file = hangman.out
gtk_file = hangman_gtk.c
gtk_cflags = `pkg-config --cflags gtk+-3.0`
gtk_libs = `pkg-config --libs gtk+-3.0`

main:
	gcc -o ${out_file} ${gtk_file} ${gtk_cflags} ${gtk_libs}
	
