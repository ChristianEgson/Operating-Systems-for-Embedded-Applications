#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

#define SIZE 100

#define CODECRACKER_IOCTL_BASE 0xF0
#define CODECRACKER_IOCTL_RESET _IO(CODECRACKER_IOCTL_BASE, 0)
#define CODECRACKER_IOCTL_GUESS _IOWR(CODECRACKER_IOCTL_BASE, 1, struct codecracker_guess)

struct codecracker_guess
{
    char user_guess[5];
    int correct_position;
    int correct_digit_wrong_position;
};

int main()
{
    char output[SIZE];
    struct codecracker_guess guess;
    int fd = open("/dev/codecracker", O_RDWR);

    if (fd == -1)
    {
        printf("ERROR\n");
        return -1;
    }

    printf("Game Resetting...\n");
    if (ioctl(fd, CODECRACKER_IOCTL_RESET) < 0)
    {
        printf("ERROR: ioctl RESET failed\n");
        close(fd);
        return -1;
    }
    
    while (1)
    {
        memset(&guess, 0, sizeof(guess));
        printf("\nEnter a 4-digit guess: ");
        scanf("%4s", guess.user_guess);

        if (ioctl(fd, CODECRACKER_IOCTL_GUESS, &guess) < 0)
        {
            printf("ERROR: ioctl GUESS failed\n");
            break;
        }

        printf("-> Correct digits in correct position: %d\n", guess.correct_position);
        printf("-> Correct digits but in wrong position: %d\n", guess.correct_digit_wrong_position);

        memset(output, 0, SIZE);
        read(fd, output, SIZE);
        printf("-> %s\n", output);

        if (guess.correct_position == 4)
        {
            printf("You have cracked the code!\n");
            break;
        }
    }

    close(fd);
    return 0;
}

//gcc codecracker_test.c
// ./a.out
