compile:
	gcc -Wall -Wextra -o exe.music_writer music_writer.c -lm
	./exe.music_writer
clean:
	rm exe.music_writer
