#include "stdio.h"
#include "err.h"

#define MAX 32768

int main(int argc, char *argv[]) {
  
  if (argc < 2 || argc > 3) {
    errx(1, "Usage: %s [infile] <outfile>\n", argv[0]);
  }
  
  FILE *in, *out;
  
  char * outfile;
  
  if (argc == 3) {
    in = fopen(argv[1], "r");
    if (in == NULL) {
      err(2, "%s", argv[1]);
    }
    outfile = argv[2];
  } else {
    in = stdin;
    outfile = argv[1];
  }
  out = fopen(outfile, "w");
  
  if (out == NULL) {
    err(3, "%s", outfile);
  }
  
  char buffer[MAX];
  unsigned char seeked = 0;
  
  while (!feof(in)) {
    size_t read = fread(buffer, 1, MAX, in);
    if(ferror(in) != 0) {
      err(4, "%s", argv[1]);
    }
    unsigned short left = 0, right = 0;
    while (right < read) {
      if (buffer[right] == '\0') {
        // read a bunch of nulls
        while (buffer[right] == '\0' && right < read) {
          right ++;
        }
        // seek to the end of the nulls
        if (fseek(out, right-left, SEEK_CUR) != 0) {
          err(5, "%s", outfile);
        }
        seeked = 1;
      } else {
        // this appears to be data, copy as much as we can
        while (buffer[right] != '\0' && right < read) {
          right++;
        }
        fwrite(buffer+left, 1, right-left, out);
        if (ferror(out) != 0) {
          err(6, "%s", outfile);
        }
        seeked = 0;
      }
      left = right;
    }
  }
  if (seeked != 0) {
    // rewind by 1 character
    if(fseek(out, -1, SEEK_CUR) != 0) {
      err(7, "%s", outfile);
    }
    
    // write one null, to force the file to be the appropriate size
    if(fputc('\0', out) == EOF) {
      err(8, "%s", outfile);
    }
  }
  
  if (argc == 3 && fclose(in) != 0) {
    err(9, "%s", argv[1]);
  }
  
  if (fclose(out) != 0) {
    err(10, "%s", outfile);
  }
  return 0;
}