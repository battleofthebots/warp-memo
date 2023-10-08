#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <resolv.h>
#include <arpa/inet.h>
#include <string.h>


const char *bye = "Thanks for playing!!!!!\n";
const char *invalid = "Invalid option\n";
int clientfd_clone;

struct timecard {
    void (*printhours)();
    char *name;
};

struct timecard *timecardlist[5];
int count = 0;

void print_timecard_name(struct timecard *t, int clientfd){
    puts(t->name);
    send(clientfd, t->name, strlen(t->name), 0);
}

void add_timecard(int clientfd){
    char buf[10];
    const char *mess1= "Note size: ";
    const char *mess2 = "Name: ";
    const char *mess3 = "Success !";
    const char *mess4 = "Full";
    int size;

    if(count > 5) {
        send(clientfd, mess4, strlen(mess4), 0);
        return;
    }

    for (int i=0; i < 5; i ++) {
        if(!timecardlist[i]){
            timecardlist[i] = (struct timecard*)malloc(sizeof(struct timecard));
            if (!timecardlist[i]) {
                puts("Alloc psssss");
                exit(-1);
            }

            timecardlist[i]->printhours = print_timecard_name;
            send(clientfd, mess1, strlen(mess1), 0);
            recv(clientfd, buf, sizeof(buf), 0);

            size = atoi(buf);

            timecardlist[i]->name = (char *)malloc(size);
            if (!timecardlist[i]->name) {
                puts("Allosc errpr");
                exit(-1);
            }

            send(clientfd, mess2, strlen(mess2), 0);
            recv(clientfd, timecardlist[i]->name, sizeof(size), 0);
            send(clientfd, mess3, strlen(mess3), 0);

            count++;
            break;
        }
    }
}

void del_timecard(int clientfd){
    char buf[4];
    int id;

    const char *mess = "Success!!";
    const char *mess1 = "Out of bound!";
    const char *mess2 = "Index :";

    send(clientfd, mess2, strlen(mess2), 0);
    recv(clientfd, buf, sizeof(buf), 0);

    id = atoi(buf);
    if (id < 0 || id >= count) {
        send(clientfd, mess1, strlen(mess1), 0);
        exit(0);
    }

    // THIS IS WHERE THE VULN IS
    if (timecardlist[id]) {
        free(timecardlist[id]->name);
        free(timecardlist[id]);
        send(clientfd, mess, strlen(mess), 0);
    }
}

void print_timecard(int clientfd){
    char buf[4];
    int id;
    const char *mess = "Index : ";
    const char *mess1 = "Out of bound!!";

    send(clientfd, mess, strlen(mess), 0);
    recv(clientfd, buf, sizeof(buf), 0);

    id = atoi(buf);
    if (id < 0 || id >= count) {
        send(clientfd, mess1, strlen(mess1), 0);
        exit(0);
    }

    if(timecardlist[id]) {
        timecardlist[id]->printhours(timecardlist[id], clientfd);
    }

}

void admin_debug(){
    const char *happynote = "YOU ARE THE TRUE HACKER!!!\n";
    send(clientfd_clone, happynote, strlen(happynote), 0);

    dup2(clientfd_clone, 0);
    dup2(clientfd_clone, 1);
    dup2(clientfd_clone, 2);

    execve("/bin/sh", 0, 0);
}

// UNCESSARY FUNCTION....WHY?? I DON"T KNOW. I WAS FEELING SPICY 
void menu(int clientfd){
  const char *options = "\n----------------------\n"
                        "       HackNote       \n"
                        "----------------------\n"
                        " 1. Add note          \n"
                        " 2. Delete note       \n"
                        " 3. Print note        \n"
                        " 4. Exit              \n"
                        "----------------------\n\0";
    send(clientfd, options, strlen(options), 0);
}

int timecard_main(int clientfd) {
    char buf[4];
    int breakloop = 0;

    while (!breakloop) {
        // Send menu
        menu(clientfd);

        if (recv(clientfd, buf, sizeof(buf), 0))
        {
            switch (atoi(buf)) {
            case 1:
                /* code */
                add_timecard(clientfd);
                break;
            case 2:
                del_timecard(clientfd);
                break;
            case 3:
                print_timecard(clientfd);
                break;
            case 4:
                breakloop = 1;
                send(clientfd, bye, strlen(bye), 0);
                exit(0);
                break;
            default:
                send(clientfd, invalid, strlen(invalid), 0);
                break;
            }
        }
    }
}

int main(){

    struct sockaddr_in server;
    struct sockaddr_in client;
    int addrlen = sizeof(client);
    int port = 1337;
    int clientfd;
    int sockfd;
    int option = 1;

    pid_t parent = getpid();
    pid_t child;

    printf("[+] Listening port: %d\n", port);

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        perror("Error with creating socket");
        exit(0);
    }

    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));

    bzero(&server, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = INADDR_ANY;


    if (bind(sockfd, (struct sockaddr*)&server, sizeof(server)) != 0) {
        perror("Socket bind error");
        exit(0);
    }

    if (listen(sockfd, 100) != 0){
        perror("Socket listen error");
        exit(0); 
    }

    while (1){
        clientfd = accept(sockfd, (struct sockaddr*)&client, &addrlen);
        clientfd_clone = clientfd;
        printf("%s:%d connected\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));
        child = fork();

        if (getpid() == parent) {
            close(clientfd);
        } else {
            timecard_main(clientfd);
        }
    }

    close(sockfd);
    return 0;
}
