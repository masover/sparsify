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
  
  while (!feof(in)) {
    size_t read = fread(buffer, 1, MAX, in);
    if(ferror(in) != 0) {
      err(4, "%s", argv[1]);
    }
    unsigned short left = 0, right = 0;
    while (right < read) {
      if (buffer[right] == '\0') {
        while (buffer[right] == '\0' && right < read) {
          right ++;
        }
        if (fseek(out, right-left, SEEK_CUR) != 0) {
          err(5, "%s", outfile);
        }
      } else {
        while (buffer[right] != '\0' && right < read) {
          right++;
        }
        fwrite(buffer+left, 1, right-left, out);
        if (ferror(out) != 0) {
          err(6, "%s", outfile);
        }
      }
      left = right;
    }
  }
  return 0;
}