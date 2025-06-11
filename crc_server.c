#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>

void xor_operation(char* dividend, char* divisor, int pos, int divl) {
    for (int i = 0; i < divl; i++) {
        dividend[pos + i] = (dividend[pos + i] == divisor[i]) ? '0' : '1';
    }
}

int main() {
    int sockfd, connfd;
    struct sockaddr_in servaddr, cli;
    char data[100], divisor[20], temp[100], codeword[100];

    // Socket setup
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(8080);
    bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
    listen(sockfd, 5);
    printf("Server ready. Waiting for client...\n");
    socklen_t len = sizeof(cli);
    connfd = accept(sockfd, (struct sockaddr*)&cli, &len);
    printf("Client connected.\n");

    // Inputs
    printf("Enter the dataword: ");
    scanf("%s", data);
    printf("Enter the generator polynomial (divisor): ");
    scanf("%s", divisor);

    int dl = strlen(data);
    int divl = strlen(divisor);

    // Append zeros to data
    strcpy(temp, data);
    for (int i = 0; i < divl - 1; i++)
        temp[dl + i] = '0';
    temp[dl + divl - 1] = '\0';
    printf("Updated dividend: %s\n", temp);

    // Perform division
    char data1[100];
    strcpy(data1, temp);
    for (int i = 0; i <= dl - 1; i++) {
        if (data1[i] == '1') {
            xor_operation(data1, divisor, i, divl);
        }
    }

    // Create final codeword = data + remainder
    strcpy(codeword, data);
    strncat(codeword, data1 + dl, divl - 1);
    printf("Final codeword: %s\n", codeword);

    // Ask to inject error
    char choice;
    printf("Do you want to inject an error into the codeword? (y/n): ");
    scanf(" %c", &choice);
    if (choice == 'y' || choice == 'Y') {
        int pos;
        int len = strlen(codeword);
        printf("Enter the bit position to flip (1 to %d): ", len);
        scanf("%d", &pos);
        if (pos >= 1 && pos <= len) {
            pos--; // 0-indexed
            codeword[pos] = (codeword[pos] == '0') ? '1' : '0';
            printf("Codeword after flipping bit %d: %s\n", pos + 1, codeword);
        } else {
            printf("Invalid bit position. Sending original codeword.\n");
        }
    } else {
        printf("Codeword sent without error: %s\n", codeword);
    }

    // Send to client
    send(connfd, codeword, sizeof(codeword), 0);
    send(connfd, divisor, sizeof(divisor), 0);

    printf("Codeword and divisor sent to client.\n");

    close(sockfd);
    return 0;
}
