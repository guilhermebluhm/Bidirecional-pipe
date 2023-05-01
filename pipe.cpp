#include <iostream>
#include <sys/wait.h>
#include <unistd.h>
#include <cstring>

int main(){

    const int BUFFER = 5;
    const int BUFFER_CHAR = 25;
    int fd1[2];
    int fd2[2];
    pid_t pid;

    if(pipe(fd1) == -1){
        std::cout << "error to create half-duplex pipe default POSIX\n";
        return -1;
    }

    if(pipe(fd2) == -1){
        std::cout << "error to create half-duplex pipe default POSIX\n";
        return -1;
    }

    pid = fork();

    if(pid < 0){
        std::cout << "error when create syscall fork\n";
        return -1;
    }
    else{
        if(pid > 0){
            std::cout<<"started parent process\n";
            int arrayRetrieve[BUFFER];
            char sendMessage[BUFFER_CHAR] = "hello pipe";
            
            close(fd2[0]);
            if(write(fd2[1], sendMessage, strlen(sendMessage)+1) == -1){
                std::cout << "error when writing in a new pipe\n";
                return -1;
            }
            close(fd2[1]);

            close(fd1[1]);
            if(read(fd1[0], arrayRetrieve, sizeof(arrayRetrieve)) == -1){
                std::cout << "error read in pipe\n";
                return -1;
            }
            close(fd1[0]);
            std::cout << "array retrieve from pipe-1 => ";
            for(size_t i{0} ; i < BUFFER ; i+=1){
                std::cout << arrayRetrieve[i] << " ";
            }
            std::cout << "\n";
            wait(NULL);
            std::cout<<"exited parent process\n";
        }
        else{
            std::cout << "started child process\n";
            int arrayToSend[BUFFER];
            char messageRetrieve[BUFFER_CHAR];
            close(fd2[1]);
            if(read(fd2[0], messageRetrieve, BUFFER_CHAR) == -1){
                std::cout << "erro\n";
                return -1;
            }
            close(fd2[0]);
            std::cout << "message from parent orocess in pipe-2: " << messageRetrieve << "\n";
            close(fd1[0]);
            for(size_t i{0} ; i < BUFFER ; i+=1){
                arrayToSend[i] = rand()%10+1;
            }
            //em caso de sucesso write/read retornam 0
            if(write(fd1[1], arrayToSend, sizeof(arrayToSend)) == -1){
                std::cout << "error write in pipe\n";
                return -1;
            }
            close(fd1[1]);
            std::cout<<"exited child process\n";
            exit(0);
        }
    }

}