#include <stdlib.h>
#include <stdio.h>
#include "Util.h"
#include <string.h>

#define RGB_BYTE_FORMAT '6'

void write_header(FILE* file, int raw_out, int cols, int rows, int maxval) {
	if(raw_out)
      fprintf(file, "P5\n");
    else
      fprintf(file, "P2\n");

    fprintf(file, "%d %d \n", cols, rows);
    fprintf(file, "%d\n", maxval);
}

int main(int argc, char* argv[])
{
    FILE *ifp;
    gray* graymap;
    int ich1, ich2, rows, cols, maxval=255, pgmraw_in, pgmraw_out;
    int i, j;

    /* Arguments */
    if ( argc != 3 ){
      printf("\nUsage: %s file_in file_out \n\n", argv[0]);
      exit(0);
    }

    /* Opening input file */
    ifp = fopen(argv[1],"r");
    if (ifp == NULL) {
      printf("error in opening file %s\n", argv[1]);
      exit(1);
    }

    /*  Magic number reading */
    ich1 = getc( ifp );
    if ( ich1 == EOF ) {
        pm_erreur( "EOF / read error / magic number" );
    }
    ich2 = getc( ifp );
    if ( ich2 == EOF ) {
        pm_erreur( "EOF /read error / magic number" );
    }
  
    if(ich2 != RGB_BYTE_FORMAT) {
        pm_erreur(" wrong file type ");
    }
    pgmraw_in = 1;

    /* Reading image dimensions */
    cols = pm_getint( ifp );
    rows = pm_getint( ifp );
    maxval = pm_getint( ifp );

    /* Memory allocation  */
    graymap = (gray *) malloc(cols * rows * sizeof(gray));

    /* Reading */
    for(i = 0; i < rows; i++)
      for(j = 0; j < cols ; j++){
        if(pgmraw_in) {
          graymap[i * cols + j] = pm_getrawbyte(ifp) ;
        }
        else {
          graymap[i * cols + j] = pm_getint(ifp);
        }
      }

    
    /* Closing input file */
    fclose(ifp);
    
    /* Opening output file */
	printf("size of argv[2] is  %d\n", strlen(argv[2]));

    char* outname = (char*)malloc(strlen(argv[2]) + 2 + 4 + 1);
    memcpy(outname + 2, argv[2], strlen(argv[2]));
    outname[1] = '_';
    outname[strlen(argv[2]) + 2] = '.';
    outname[strlen(argv[2]) + 2 + 1] = 'p';
    outname[strlen(argv[2]) + 2 + 2] = 'g';
    outname[strlen(argv[2]) + 2 + 3] = 'm';
    outname[strlen(argv[2]) + 2 + 4] = '\0';

	printf("size of outname is  %d\n", strlen(outname));

	printf("1\n");

    outname[0] = 'r';
	printf("argv[2] is  %s\n", argv[2]);
	printf("filename is %s\n", outname);
    FILE* ofp_r = fopen(outname, "w");

    outname[0] = 'g';
    FILE* ofp_g = fopen(outname, "w");

    outname[0] = 'b';
    FILE* ofp_b = fopen(outname, "w");

	
    
    free(outname);
	printf("2\n");

    if (ofp_r == NULL || ofp_g == NULL || ofp_b == NULL) {
      printf("error in opening file %s\n", argv[2]);
      exit(1);
    }

	write_header(ofp_r, pgmraw_out, cols, rows, maxval);
	write_header(ofp_g, pgmraw_out, cols, rows, maxval);
	write_header(ofp_b, pgmraw_out, cols, rows, maxval);

    /* Writing */
    // pgmraw_in is the same as pgmraw_out
    pgmraw_out = pgmraw_in;

    for (i = 0; i < rows; i++)
      for (j = 0; j < cols; j++) {
        if (pgmraw_out) {
          fprintf(ofp_r, "%c", graymap[i * cols + j]);
          fprintf(ofp_g, "%c", graymap[i * cols + j + 1]);
          fprintf(ofp_b, "%c", graymap[i * cols + j + 2]);
        }
        else {
          fprintf(ofp_r, "%d", graymap[i * cols + j]);
          fprintf(ofp_g, "%d", graymap[i * cols + j + 1]);
          fprintf(ofp_b, "%d", graymap[i * cols + j + 2]);
        }
        
      }
    

	
    /* Closing output files */
    fclose(ofp_r);
    fclose(ofp_g);
    fclose(ofp_b);
    
    return 0;
}