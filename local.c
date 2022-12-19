#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#define ROOTPORT 7865
#define TLDPORT 
#define AUTHPORT 
#define LOCALPORT 7866
int main() {
        int socketfd = 0, localfd = 0;
        int rootfd = 0, tldfd = 0, authfd = 0;
        socklen_t length = sizeof(struct sockaddr_in);
        struct sockaddr_in host_addr, root_addr, tld_addr, auth_addr, client_addr;
        char buffer[512], root[20], tld[30], auth[100];
        char rootip[30], tldip[30], authip[30];
        int recvbytes, sentbytes;
        socketfd = socket(AF_INET, SOCK_DGRAM, 0);
        if(socketfd < 0) {
                fprintf(stderr, "Error in socket creation.\n");
                return -1;
        }
        host_addr.sin_family = AF_INET;
        host_addr.sin_port = htons(LOCALPORT);
        inet_pton(AF_INET, "127.0.0.1", &host_addr.sin_addr);
        if(bind(socketfd, (struct sockaddr*)&host_addr, sizeof(host_addr)) < 0) {
                fprintf(stderr, "Error in binding port to socket.\n");
                return -1;
        }
  while(1) {
                recvbytes = recvfrom(socketfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&client_addr, &length);
                if(strncmp(buffer, "exit", sizeof("exit")) == 0){
                        strcpy(root,buffer);
                        strcpy(auth,buffer);
                        strcpy(tld,buffer);
                        printf("exiting...\n");
                        break;
                }
                else{
                        fprintf(stdout, "Request from client : %s\n", buffer);
                        strcpy(auth, buffer);
                        int i = 0, j = 0, k = 0;
                        while(buffer[i++] != '.');
                        while(buffer[i] != '.') {
                                tld[j++] = buffer[i++];
                        }
                tld[j++] = buffer[i++];
                while(buffer[i] != ' ' && buffer[i] != '\0') {
                        tld[j++] = buffer[i];
                        root[k++] = buffer[i];
                        i++;
                }
                tld[j] = '\0';
                root[k] = '\0';
                }
        rootfd = socket(AF_INET, SOCK_DGRAM, 0);
        if(rootfd < 0){
                fprintf(stderr, "Error in socket creation.\n");
                return -1;
        }
 	root_addr.sin_family = AF_INET;
        root_addr.sin_port = htons(ROOTPORT);
        inet_pton(AF_INET, "127.0.0.1", &root_addr.sin_addr);
        sentbytes = sendto(rootfd, root, strlen(root) + 1, 0, (struct sockaddr*)&root_addr, length);
       recvbytes = recvfrom(rootfd, rootip, sizeof(rootip), 0, NULL, NULL);
       fprintf(stdout, "TLD server IP for %s: %s.\n", root, rootip);
       close(rootfd);
        }
close(socketfd);
        return 0;
}
