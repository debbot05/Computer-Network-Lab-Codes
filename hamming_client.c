#include <stdio.h>
#include <math.h>
#include <string.h>
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
    int sockfd;
    struct sockaddr_in servaddr;
    char data[100];
    int data1[100], data2[100];
    int dl, r, i = 0, j, k, z, c, l;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(8080);
    servaddr.sin_addr.s_addr = INADDR_ANY;
    connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));

    recv(sockfd, data, sizeof(data), 0);
    reverse(data); // match server format
    printf("📥 Received codeword: %s\n", data);

    dl = strlen(data);
    while (pow(2, i) < dl + 1)
        i++;
    r = i;

    j = dl - 1;
    for (i = 1; i <= dl; i++) {
        data1[i] = data[j--] - '0';
    }

    l = 1;
    int count = 0;
    for (i = 0; i < r; i++) {
        z = pow(2, i);
        c = 0;
        for (j = z; j <= dl; j = z + k) {
            for (k = j; k < z + j && k <= dl; k++) {
                c += data1[k];
            }
        }
        data2[l] = c % 2;
        count += data2[l];
        l++;
    }

    if (count == 0) {
        printf("Actual data received.\n");
    } else {
        printf("Wrong data received.\n");
        j = 0;
        for (i = r; i >= 1; i--) {
            if (data2[i] == 1)
                j += pow(2, i - 1);
        }
        printf("Error at position %d\n", j);
        data1[j] = !data1[j];
        printf("Corrected codeword is: ");
        for (i = dl; i >= 1; i--)
            printf("%d", data1[i]);
        printf("\n");
    }

    printf("Extracted dataword is: ");
    for (i = dl, j = 0; i >= 1; i--) {
        int is_parity = 0;
        for (k = 0; k < r; k++) {
            if (i == pow(2, k)) {
                is_parity = 1;
                break;
            }
        }
        if (!is_parity)
            printf("%d", data1[i]);
    }
    printf("\n");

    close(sockfd);
    return 0;
}
