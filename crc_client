#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

// XOR operation for division
void xor_operation(char* data, char* divisor, int pos, int divl) {
    for (int i = 0; i < divl; i++) {
        data[pos + i] = (data[pos + i] == divisor[i]) ? '0' : '1';
    }
}

int main() {
    int sockfd;
    struct sockaddr_in servaddr;
    char codeword[100], divisor[20], data1[100];

    // 1. Setup client socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(8080);
    servaddr.sin_addr.s_addr = INADDR_ANY;
    connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));

    // 2. Receive codeword and divisor
    recv(sockfd, codeword, sizeof(codeword), 0);
    recv(sockfd, divisor, sizeof(divisor), 0);

    printf("Received codeword: %s\n", codeword);
    printf("Received divisor : %s\n", divisor);

    int dl = strlen(codeword);
    int divl = strlen(divisor);

    // 3. Copy codeword to temp for division
    strcpy(data1, codeword);

    // 4. Perform modulo-2 division
    for (int i = 0; i <= dl - divl; i++) {
        if (data1[i] == '1') {
            xor_operation(data1, divisor, i, divl);
        }
    }

    // 5. Check if remainder is all zeros
    int error = 0;
    for (int i = dl - divl + 1; i < dl; i++) {
        if (data1[i] != '0') {
            error = 1;
            break;
        }
    }

    if (error == 0) {
        printf("No error detected. Data is valid.\n");
        printf("Extracted dataword: ");
        for (int i = 0; i < dl - divl + 1; i++) {
            printf("%c ", codeword[i]);
        }
        printf("\n");
    } else {
        printf("Error detected in received codeword!\n");
    }

    close(sockfd);
    return 0;
}
