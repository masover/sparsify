#include "stdio.h"

#define MAX 32768

int main(int argc, char *argv[]) {
  FILE *in = fopen("a", "r");
  FILE *out = fopen("b", "w");
  
  char buffer[MAX];
  
  while (!feof(in)) {
    size_t read = fread(buffer, 1, MAX, in);
    unsigned short left = 0, right = 0;
    while (right < read) {
      if (buffer[right] == '\0') {
        while (buffer[right] == '\0' && right < read) {
          right ++;
        }
        fseek(out, right-left, SEEK_CUR);
      } else {
        while (buffer[right] != '\0' && right < read) {
          right++;
        }
        fwrite(buffer+left, 1, right-left, out);
      }
      left = right;
    }
  }
  return 0;
}