#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>

int is_power_of_two(int n) {
    return (n && !(n & (n - 1)));
}

int main() {
    int sockfd, connfd;
    struct sockaddr_in servaddr, cli;
    char data[200], padded[200];
    int sl, dl, i, j, k, c, t, sum[100];

    // Socket setup
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(8080);
    servaddr.sin_addr.s_addr = INADDR_ANY;
    bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
    listen(sockfd, 5);
    printf("🚀 Waiting for client...\n");
    socklen_t len = sizeof(cli);
    connfd = accept(sockfd, (struct sockaddr*)&cli, &len);
    printf("✅ Client connected.\n");

    // Input
    printf("Enter dataword (binary): ");
    scanf("%s", data);
    dl = strlen(data);

    while (1) {
        printf("Enter segment length (power of 2): ");
        scanf("%d", &sl);
        if (is_power_of_two(sl)) break;
        printf("❌ Segment length must be a power of 2!\n");
    }

    // Padding
    int pad = (dl % sl == 0) ? 0 : sl - (dl % sl);
    memset(padded, '0', pad);
    strcpy(padded + pad, data);
    dl = strlen(padded);
    padded[dl] = '\0';

    // Calculate checksum
    for (i = 0; i < sl; i++) sum[i] = 0;

    for (i = dl; i > 0; i -= sl) {
        c = 0;
        k = sl - 1;
        for (j = i - 1; j >= i - sl; j--) {
            t = sum[k] + (padded[j] - '0') + c;
            sum[k] = t % 2;
            c = t / 2;
            k--;
        }
        if (c == 1) {
            for (j = sl - 1; j >= 0; j--) {
                t = sum[j] + c;
                sum[j] = t % 2;
                c = t / 2;
            }
        }
    }

    char checksum[sl + 1];
    for (i = 0; i < sl; i++)
        checksum[i] = (sum[i] == 0) ? '1' : '0';
    checksum[sl] = '\0';

    // Form codeword = data + checksum
    strcat(padded, checksum);
    printf("✅ Codeword before error injection: %s\n", padded);

    // Error injection
    char choice;
    printf("Inject error? (y/n): ");
    scanf(" %c", &choice);
    if (choice == 'y' || choice == 'Y') {
        int pos;
        printf("Enter bit position to flip (1 to %lu): ", strlen(padded));
        scanf("%d", &pos);
        if (pos >= 1 && pos <= strlen(padded)) {
            padded[pos - 1] = (padded[pos - 1] == '0') ? '1' : '0';
            printf("💥 Codeword after error at position %d: %s\n", pos, padded);
        } else {
            printf("⚠️ Invalid bit position.\n");
        }
    }

    // Send codeword and segment length
    send(connfd, padded, sizeof(padded), 0);
    send(connfd, &sl, sizeof(sl), 0);
    printf("📤 Sent codeword and segment length to client.\n");

    close(sockfd);
    return 0;
}
