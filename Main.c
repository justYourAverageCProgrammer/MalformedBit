#ifdef __linux__
  
  #include <linux/limits.h>
  #include <stdio.h>
  #include <uuid/uuid.h>
  #include <unistd.h>
  #include <string.h>
  #include <stdlib.h>
  #include <fcntl.h>
  #include <errno.h>

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

  void getSelfFilePath(char* out, size_t outSize){
    if(outSize == 0){
      fprintf(stderr, "outSize must be greater than 0!\n");
      return;
    }
    ssize_t len;

    len = readlink("/proc/self/exe", out, outSize-1);
    if(len == -1){
      perror("Reading own file name failed.");
      out[0] = '\0';
      return;
    }

    out[len] = '\0';
  }

  void genFile(unsigned char* content, size_t contentLength, char* fileName, int* fileUni){
    int fd = open(fileName, O_WRONLY | O_CREAT | O_EXCL, 0777);
    if(fd==-1){
      if(errno == EEXIST){
        perror("File being created already exists.");
        *fileUni = 0;
        return;
      } else{
        perror("Error creating file descriptor.");
        return;
      }
    }
    *fileUni = 1;

    FILE* file = fdopen(fd, "wb"); 

    if(file == NULL){
      char errMessage[strlen(fileName)+50];
      snprintf(errMessage, sizeof(errMessage), "Creating file %s failed.", fileName);
      perror(errMessage);
      return;
    }

    if(fwrite(content, sizeof(unsigned char), contentLength, file) != contentLength){
      perror("Error writing to file");
      return;
    }

    fclose(file);
  }

  void readFileContent(unsigned char** fileContent, const char* path, size_t* fileLength){
    FILE* binary = fopen(path, "rb");

    if(binary == NULL){
      char errMessage[strlen(path)+50];
      snprintf(errMessage, sizeof(errMessage), "Reading file %s failed.", path);
      *fileContent = NULL;
      perror(errMessage);
      return;
    }
    
    fseek(binary, 0, SEEK_END);
    long tmp = ftell(binary);
    if(tmp<0){
      perror("Error finding file length.");
      *fileContent = NULL;
      return;
    }
    *fileLength = tmp;
    fseek(binary, 0, SEEK_SET);

    unsigned char* binaryContent = (unsigned char*)malloc(*fileLength*sizeof(unsigned char));
    fread(binaryContent, sizeof(unsigned char), *fileLength, binary);
    fclose(binary);

    *fileContent = binaryContent;
  }

  int main(void){
    //get executable path
    char selfPath[PATH_MAX];
    getSelfFilePath(selfPath, sizeof(selfPath));
    if(selfPath[0]=='\0'){
      return -1;
    }

    //now read self
    unsigned char* fileContent;
    size_t fileLength;
    readFileContent(&fileContent, selfPath, &fileLength);
    if(fileContent==NULL){
      return -1;
    }

    //file creation loop where it trys creating a file with a unique name
    int trying = 1;
    int fileUni = 1;
    while(trying){
      char fileName[37];
      genFileName(fileName, sizeof(fileName));
      if(fileName[0]=='\0'){
        return -1;
      }
      
      genFile(fileContent, fileLength, fileName, &fileUni);
      if(fileUni){
        trying=0;
      }
    }
    //clean up
    free(fileContent);
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
