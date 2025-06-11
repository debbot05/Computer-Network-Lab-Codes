#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

int main() {
    int sockfd;
    struct sockaddr_in servaddr;
    char codeword[100];
    char parity_mode;
    int count = 0;

    // Create socket and connect
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(8080);
    servaddr.sin_addr.s_addr = INADDR_ANY;

    connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));

    // Receive codeword and parity mode
    recv(sockfd, codeword, sizeof(codeword), 0);
    recv(sockfd, &parity_mode, sizeof(parity_mode), 0);

    printf("Received codeword: %s\n", codeword);
    printf("Parity mode: %s\n", (parity_mode == 'e') ? "Even" : "Odd");

    // Count number of 1s
    for (int i = 0; codeword[i] != '\0'; i++)
        if (codeword[i] == '1') count++;

    int valid = 0;
    if (parity_mode == 'e' && count % 2 == 0)
        valid = 1;
    if (parity_mode == 'o' && count % 2 == 1)
        valid = 1;

    if (valid) {
        printf("Parity Check Passed. No Error.\n");
        printf("Extracted dataword: ");
        for (int i = 0; i < strlen(codeword) - 1; i++)
            printf("%c", codeword[i]);
        printf("\n");
    } else {
        printf("Parity Check Failed. Error Detected in Codeword!\n");
    }

    close(sockfd);
    return 0;
}
