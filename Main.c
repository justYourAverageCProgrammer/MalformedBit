#include <stdio.h>

int main(void){
  const char* code = "#include <stdio.h>%c%cint main(void){%c const char* code = %c%s%c;%c  printf(code, 10, 10, 10, 34, code, 34, 10, 10, 10);%c  return 0;%c}";
  printf(code, 10, 10, 10, 34, code, 34, 10, 10, 10);
  return 0;
}
