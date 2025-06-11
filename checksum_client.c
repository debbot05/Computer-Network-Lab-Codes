#include <stdio.h>
#include <string.h>
#include <math.h>
#include <arpa/inet.h>
#include <unistd.h>

int main() {
    int sockfd;
    struct sockaddr_in servaddr;
    char codeword[200];
    int sl, dl, i, j, k, c, t, sum[100];

    // Socket setup
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(8080);
    servaddr.sin_addr.s_addr = INADDR_ANY;
    connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));

    // Receive codeword and segment length
    recv(sockfd, codeword, sizeof(codeword), 0);
    recv(sockfd, &sl, sizeof(sl), 0);

    printf("📥 Received codeword: %s\n", codeword);
    printf("📥 Segment length: %d\n", sl);

    dl = strlen(codeword);
    for (i = 0; i < sl; i++) sum[i] = 0;

    for (i = dl; i > 0; i -= sl) {
        c = 0;
        k = sl - 1;
        for (j = i - 1; j >= i - sl; j--) {
            t = sum[k] + (codeword[j] - '0') + c;
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

    int error = 0;
    for (i = 0; i < sl; i++) {
        if (sum[i] != 1) {
            error = 1;
            break;
        }
    }

    if (error == 0) {
        printf("✅ Data received correctly.\n");
        printf("📦 Actual data: ");
        for (i = 0; i < dl - sl; i++)
            printf("%c ", codeword[i]);
        printf("\n");
    } else {
        printf("❌ Error detected in received data!\n");
    }

    close(sockfd);
    return 0;
}
