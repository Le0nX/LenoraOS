#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>

#define MYPYPE_NAME "/tmp/my_pype"
#define BUFFER 50

int main (int argc, char *argv[]){

	int fd, len;
	char buffer[BUFFER];

	if (mkfifo (MYPYPE_NAME, 0777)){  /* if succes fifo returnes 0*/
		perror ("mkfifo");
		return 1;
	}
	printf ("%s is created\n", MYPYPE_NAME);

	if ((fd = open (MYPYPE_NAME, O_RDONLY)) <= 0){
		perror ("open");
		return 1;
	}
	printf ("%s is opened\n", MYPYPE_NAME);

	do {
		memset (buffer, '\0', BUFFER);
		if ((len = read (fd, buffer, BUFFER-1)) <= 0){
		
			perror ("read");
			close (fd);
			remove (MYPYPE_NAME);
			return 0;
		}
	printf ("Incoming message (%d): %s\n", len, buffer);
	} while (1);
}
