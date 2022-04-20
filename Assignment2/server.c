// Server side C/C++ program to demonstrate Socket programming
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <errno.h>
#include <pwd.h>


#define PORT 8080

int main(int argc, char const *argv[])
{
    int server_fd, new_socket, val_read;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    char *hello = "Hello from server";

    if (argv[0][2] == 'Y')
    {
        int fd = argv[0][1];
        val_read = read(fd, buffer, 1024);
        printf("%s\n", buffer);
        send(fd, hello, strlen(hello), 0);
        printf("Hello message sent\n");
        return 0;
    }

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR,
	&opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );

    if (bind(server_fd, (struct sockaddr *)&address,
	sizeof(address)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                       (socklen_t*)&addrlen))<0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    pid_t current_Pid = fork();

    if (current_Pid == 0)
    {
        printf("Child process is  Running\n");
        struct passwd *pwd;
        char users[]="nobody";
        if ((pwd = getpwnam(users)) == NULL){
            perror("getpwnam() error");
            exit(EXIT_FAILURE);
        }
        printf("Display of  userid of nobody : %d\n", (int) pwd->pw_uid);
        int user_Id = setuid(pwd->pw_uid);
        if (user_Id == -1)
        {
            printf("setuid command failed and  returned -1\n");
            return 0;
        }

        char fileDist_args[3];
        fileDist_args[0] = new_socket;
        fileDist_args[1] = new_socket;
        fileDist_args[2] = 'Y';
        execl(argv[0], fileDist_args, NULL);
    }

    else if (current_Pid > 0)
    {
        wait(NULL);
        printf("Parent process is running\n");
    }
    else
    {
        perror("Unable to fork");
        _exit(2);
    }

    return 0;
}
