#ifdef __linux__

  #include <stdio.h>
  #include <uuid/uuid.h>
  #include <unistd.h>
  #include <limits.h>
  #include <string.h>

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

  void getSelfFileName(char** out){
    char path[PATH_MAX];
    ssize_t len;

    len = readlink("/proc/self/exe", path, sizeof(path)-1);
    if(len == -1){
      perror("Reading own file name failed.");
    }

    path[len] = '\0';

    char* token;
    char* prev = strtok(path, "/");
    while((token=strtok(NULL, "/")) != NULL) prev = token;
    *out = prev;
  }
  
  int main(void){
    char* fileName;
    getSelfFileName(&fileName);
    printf("%s\n", fileName);
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
