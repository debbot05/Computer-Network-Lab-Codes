#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

int main() {
    int sockfd, connfd;
    struct sockaddr_in servaddr, cli;
    char dataword[100], codeword[100];
    char parity_mode;
    int count = 0;

    // Create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(8080);
    servaddr.sin_addr.s_addr = INADDR_ANY;
    bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
    listen(sockfd, 5);
    printf("Waiting for client...\n");
    socklen_t len = sizeof(cli);
    connfd = accept(sockfd, (struct sockaddr*)&cli, &len);
    printf("Client connected.\n");

    // Input
    printf("Enter the binary dataword: ");
    scanf("%s", dataword);
    printf("Enter parity mode (e for Even, o for Odd): ");
    scanf(" %c", &parity_mode);

    // Count 1s
    for (int i = 0; dataword[i] != '\0'; i++)
        if (dataword[i] == '1') count++;

    // Append parity bit
    strcpy(codeword, dataword);
    if (parity_mode == 'e') {
        strcat(codeword, (count % 2 == 0) ? "0" : "1");
    } else {
        strcat(codeword, (count % 2 == 0) ? "1" : "0");
    }

    printf("Codeword before error injection: %s\n", codeword);

    // Ask for error injection
    char choice;
    printf("Inject error? (y/n): ");
    scanf(" %c", &choice);
    if (choice == 'y' || choice == 'Y') {
        int pos;
        printf("Enter bit position to flip (1 to %lu): ", strlen(codeword));
        scanf("%d", &pos);
        if (pos >= 1 && pos <= strlen(codeword)) {
            codeword[pos - 1] = (codeword[pos - 1] == '1') ? '0' : '1';
            printf("Codeword after flipping bit %d: %s\n", pos, codeword);
        } else {
            printf("Invalid bit position. Sending original codeword.\n");
        }
    }

    // Send
    send(connfd, codeword, sizeof(codeword), 0);
    send(connfd, &parity_mode, sizeof(parity_mode), 0);

    printf("Codeword and mode sent to client.\n");
    close(sockfd);
    return 0;
}
