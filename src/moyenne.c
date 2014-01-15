/*********************************************************************
 * Outil minimal de statistiques
 * Calcule la moyenne et l'ecart-type de valeurs experiementales
 * pour differents parametres.
 * :copyright: Copyright 2013, see AUTHORS
 *             Copyright 2013, voir AUTHORS
 * :licence: LGPL, see LICENCE for details.
 *           LGPL, voir LICENCE pour plus de details.
 ********************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/types.h>
#include <malloc.h>
#include <errno.h>

#define LENGTH 50

#define TRUE  1
#define FALSE 0

#define MAXFILES 20

/* From http://mbays.freeshell.org/darcs-repos/hunt-ng/hunt-ng/lib/fgetln.c */

typedef struct {
    FILE *key;		/* file descriptor this bfr is associated with */
    char *bfr;		/* buffer for latest fgetln()ed line */
    int buflen;		/* length of the buffer */
} LINEREC;

static LINEREC tbl[MAXFILES] = { 0 };

char*
fgetln(FILE *fd, size_t *len)
{
    int idx;
    int c, lnsize;

    /* we want to do fgetln, eh?  Check to see if all the variables
     * are okay first (don't want to waste space if the file is bad
     * or the len argument is bogus (==zero)
     */
    if (fd == 0 || len == 0) {
	errno = EINVAL;
	return 0;
    }

    /* find an available slot in the table
     */
    for (idx = 0; idx < MAXFILES; idx++)
	if (tbl[idx].key == fd)
	    break;

    if (idx == MAXFILES) {
	for (idx=0; idx < MAXFILES; idx++)
	    if (tbl[idx].key == 0) {
		/* usable file slot */
		break;
	    }

	if (idx == MAXFILES) {
	    errno = ENFILE;
	    return 0;
	}
    }

    /* Initialize the buffer, if needed
     */
    if (tbl[idx].bfr == 0) {
	tbl[idx].bfr = malloc(200);
	if (tbl[idx].bfr == 0)
	    return 0;
	tbl[idx].buflen = 200;
    }
    tbl[idx].key = fd;

    /* we've got a buffer, so lets start writing a line into
     * it.
     */
    for (lnsize = 0; (c = getc(fd)) != EOF; ) {
	tbl[idx].bfr[lnsize++] = c;
	if (lnsize == tbl[idx].buflen) {
	    tbl[idx].bfr = realloc(tbl[idx].bfr, tbl[idx].buflen * 2);
	    if (tbl[idx].bfr == 0)
		return 0;
	    tbl[idx].buflen *= 2;
	}
	if (c == '\n')
	    break;
    }

    /* save out the size, then return to sender
     */
    *len = lnsize;
    return lnsize ? tbl[idx].bfr : 0;
} /* fgetln */

int main(int argc, char **argv){

  FILE *fd, *fd_out;
  char *line;
  int nb1, previous, nb;
  long double nb2;
  long double average, dev;
  size_t len;
  long double sum;

  if( 3 > argc ){
    printf("wrong nb of arguments\n");
    printf("usage: %s <data file> <output file>\n", argv[0]);
    return EXIT_FAILURE;
  }

  fd = fopen(argv[1], "r");
  if( NULL == fd){
    perror("open data file");
    return EXIT_FAILURE;
  }

  fd_out = fopen(argv[2], "a+");
  if( NULL == fd_out ){
    perror("open output file");
    return EXIT_FAILURE;
  }

  fprintf(fd_out, "# average and standard deviations from %s\n", argv[1]);

  previous = -1; average = 0; nb = 0; dev = 0; sum = 0;
  while( NULL != (line = fgetln(fd, &len)) ){
      if( ( '#' != line[0] ) && ( '\n' != line[0] ) && ( ' ' != line[0] ) ){ /* ignore comments and white lines */
	  sscanf(line, "%d %Lf", &nb1, &nb2);
	  //	  printf("read: %d\t%Lf\n", nb1, nb2);
	  if( ( previous != nb1 ) && ( previous != -1 ) ){
	      if( nb != 0 ){
              average /= nb;
              dev = sum/nb;
              dev -= (average * average);
              dev = sqrt(dev);
              fprintf(fd_out, "%d %Lf %Lf\n", previous, average, dev);
	      }
	      
	      average = nb2;
	      sum = nb2 * nb2;
	      //	      printf("nb2: %d, dev: %Lf, sum: %lfu\n", nb2, dev, sum);
	      nb = 1;
	      previous = nb1;
	  }
	  else {
	      previous = nb1;
	      average += nb2;
	      sum += (nb2 * nb2);
	      nb++;
	  }
      }
      
      //      printf( "%d    %d    %Lf    %Lf\n", nb, nb2, dev, sum );
  }  
  
  if( nb != 0 ){
    average /= nb;
    dev = sum/nb;
    dev -= (average * average);
    dev = sqrt(dev);
    fprintf(fd_out, "%d %Lf %Lf\n", previous, average, dev);
  }

  fclose(fd); fclose(fd_out);
  return EXIT_SUCCESS;

}
