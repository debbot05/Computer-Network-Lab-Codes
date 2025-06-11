#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>

void reverse(char* str) {
    int n = strlen(str);
    for (int i = 0; i < n / 2; i++) {
        char temp = str[i];
        str[i] = str[n - i - 1];
        str[n - i - 1] = temp;
    }
}

int main() {
    int sockfd, connfd;
    struct sockaddr_in servaddr, cli;
    char data[100];
    int data1[100], data2[200];
    int dl, r, i = 0, j = 0, k, z, c;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(8080);
    bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
    listen(sockfd, 5);
    printf("Waiting for connection...\n");
    socklen_t len = sizeof(cli);
    connfd = accept(sockfd, (struct sockaddr*)&cli, &len);
    printf("Connected to client.\n");

    printf("Enter the dataword (binary): ");
    scanf("%s", data);
    dl = strlen(data);

    while (pow(2, i) < dl + i + 1) i++;
    r = i;
    printf("No of redundant bits: %d\n", r);

    for (i = 0; i < dl; i++)
        data1[i] = data[i] - '0';

    for (i = 0; i < 200; i++)
        data2[i] = -1;

    for (i = 0; i < r; i++) {
        z = pow(2, i);
        data2[z] = 999;
    }

    for (i = dl + r, j = 0; i >= 1; i--) {
        if (data2[i] != 999)
            data2[i] = data1[j++];
    }

    for (i = 0; i < r; i++) {
        z = pow(2, i);
        c = 0;
        for (j = z; j <= dl + r; j = z + k) {
            for (k = j; k < z + j && k <= dl + r; k++) {
                if (data2[k] != 999)
                    c += data2[k];
            }
        }
        data2[z] = c % 2;
    }

    char codeword[200] = "";
    for (i = dl + r; i >= 1; i--) {
        char bit[2];
        sprintf(bit, "%d", data2[i]);
        strcat(codeword, bit);
    }

    reverse(codeword);  // make bit 1 the leftmost
    printf("Generated codeword: %s\n", codeword);

    char choice;
    printf("Do you want to inject an error? (y/n): ");
    scanf(" %c", &choice);

    if (choice == 'y' || choice == 'Y') {
        int pos;
        int len_code = strlen(codeword);
        printf("Enter bit position to flip (1 to %d): ", len_code);
        scanf("%d", &pos);
        if (pos >= 1 && pos <= len_code) {
            pos--; // 0-indexed
            codeword[pos] = (codeword[pos] == '0') ? '1' : '0';
            printf("Codeword with error at position %d: %s\n", pos + 1, codeword);
        } else {
            printf("Invalid position. Sending original codeword.\n");
        }
    } else {
        printf("Codeword sent without error: %s\n", codeword);
    }

    send(connfd, codeword, sizeof(codeword), 0);
    printf("Codeword sent to client.\n");

    close(sockfd);
    return 0;
}
