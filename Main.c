#ifdef __linux__

  #include <stdio.h>
  #include <uuid/uuid.h>
  #include <unistd.h>
  #include <limits.h>

  void genFileName(char* fileName, size_t fileNameSize){
    if(fileNameSize<37){
      fprintf(stderr, "genFileName: buffer must be at least 37 bytes.\n");
      fileName[0] = '\0';
      return;
    }

    uuid_t uuid;
    uuid_generate_random(uuid);
    char uuidStr[37];
    uuid_unparse_lower(uuid, uuidStr);

    snprintf(fileName, sizeof(uuidStr), "%s", uuidStr);
  }

  void getSelfFileName(char* out){
    char path[PATH_MAX];
    ssize_t len;

    len = readlink("/proc/self/exe", path, sizeof(path)-1);
    if(len == -1){
      perror("Reading own file name failed.");
    }

    path[len] = '\0';

    
  }
  
  int main(void){
    const char* code = "#include <stdio.h>%c%cint main(void){%c const char* code = %c%s%c;%c  printf(code, 10, 10, 10, 34, code, 34, 10, 10, 10);%c  return 0;%c}";
    printf(code, 10, 10, 10, 34, code, 34, 10, 10, 10);
    return 0;
  }

#elif defined(_WIN32)
  
  #include <stdio.h>
  
  int main(void){
    fprintf(stderr, "Windows is not supported yet.\n");
    return -1;
  }

#elif defined(_WIN64)

  #include <stdio.h>
  
  int main(void){
    fprintf(stderr, "Windows is not supported yet.\n");
    return -1;
  }

#elif defined(__APPLE__)
  #include <stdio.h>
  
  int main(void){
    fprintf(stderr, "MacOS is not supported yet.\n");
    return -1;
  }

#else
  
  #include <stdio.h>
  
  int main(void){
    fprintf(stderr, "This platform is not supported.\n");
    return -1;
  }

#endif
