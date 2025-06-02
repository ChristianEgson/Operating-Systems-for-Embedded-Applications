#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kern_levels.h>
#include <linux/uaccess.h>
#include <linux/fs.h>
#include <linux/ioctl.h>
#include <linux/random.h>

MODULE_LICENSE("GPL");

#define CODECRACKER_IOCTL_BASE 0xF0
#define CODECRACKER_IOCTL_RESET _IO(CODECRACKER_IOCTL_BASE, 0)
#define CODECRACKER_IOCTL_GUESS _IOWR(CODECRACKER_IOCTL_BASE, 1, struct codecracker_guess)
#define CODECRACKER_BUF_SIZE 256

static char codecracker_buffer[CODECRACKER_BUF_SIZE];
static long codecracker_ioctl(struct file *file, unsigned int cmd, unsigned long arg);
int isReg;
int major;
int codecracker_offset_r;
int codecracker_offset_w;
int codecracker_size;
static char secret_code[5];
static int attempts;

int codecracker_init(void);
void codecracker_end(void);
static ssize_t codecracker_write(struct file*, const char*, size_t, loff_t*);
static ssize_t codecracker_read(struct file*, char*, size_t, loff_t*);

static void generate_code(void) 
{
    int i;
    for (i = 0; i < 4; i++)
    {
        secret_code[i] = (get_random_long() % 10) + '0';
    }
    secret_code[4] = '\0';
    attempts = 0;
    printk(KERN_INFO "codecracker4310: Secret code generated.\n");
}
static struct file_operations codecracker_fops =
{
    .read = codecracker_read,
    .write = codecracker_write,
    .unlocked_ioctl = codecracker_ioctl,
};

struct codecracker_guess {
    char user_guess[5];
    int correct_position;
    int correct_digit_wrong_position;
};

int codecracker_init(void)
{
    major = register_chrdev(0, "codecracker", &codecracker_fops);
    codecracker_offset_w=0;
    codecracker_offset_r=0;
    codecracker_size=0;

    if(major <0) {
        isReg=0;
        printk(KERN_INFO " codecracker4310: Start FAIL \n");
    } else {
        isReg=1;
        printk(KERN_INFO " codecracker4310: Start here \n");
    }

    return 0;
}

void codecracker_end(void)
{
    if(isReg){
        unregister_chrdev(major, "codecracker");
    }
    printk(KERN_INFO " codecracker4310: End here \n");

}

static ssize_t codecracker_write(struct file *fp, const char *buf, size_t count, loff_t *op)
{
    int ret =0;
    if(codecracker_offset_w +count >= CODECRACKER_BUF_SIZE)
    {
        printk(KERN_INFO " codecracker4310: Write overflow. Abort. \n");
        return -1;
    }

    printk(KERN_INFO " codecracker4310: Copy from user. \n");
    ret=copy_from_user(&codecracker_buffer[codecracker_offset_w],buf, count);
    if(ret != 0)
    {
        printk(KERN_INFO "codecracker4310: ERR copy from user. \n");
        return -1;
    }
    codecracker_offset_w= codecracker_offset_w + count;

    return count;
}

static ssize_t codecracker_read(struct file *fp, char *buf, size_t count, loff_t* offset)
{
    int ret;
    if(codecracker_offset_r + count >= CODECRACKER_BUF_SIZE)
    {
        count = CODECRACKER_BUF_SIZE - codecracker_offset_r;

    }
    printk(KERN_INFO "codecracker4310: Copy to user. \n");
    ret=copy_to_user(buf, &codecracker_buffer[codecracker_offset_r], count);
    if(ret != 0)
    {
        printk(KERN_INFO " codecracker 4310: ERR copy to user. \n");
        return -1;
    }

    codecracker_offset_r = codecracker_offset_r + count;

    return count;
}

static long codecracker_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    struct codecracker_guess guess;
    int i, j;
    bool matched[4] = {false};
    bool used[4] = {false};
    int correct_position = 0;
    int correct_digit = 0;

    switch (cmd) {
        case CODECRACKER_IOCTL_RESET:
            generate_code();
            return 0;

        case CODECRACKER_IOCTL_GUESS:
            if(copy_from_user(&guess, (void __user *)arg, sizeof(guess)))
                return -EFAULT;

            attempts = attempts + 1;

            //Step 1: Count digits in correct position
            for (i = 0; i < 4; i++)
            {
                if (guess.user_guess[i] == secret_code[i])
                {
                    correct_position++;
                    matched[i] = true;
                    used[i] = true;
                }
            }

            //Count digits that are correct, but in wrong position
            for (i = 0; i < 4; i++)
            {
                if (matched[i])
                    continue;
                for (j = 0; j < 4; j++)
                {
                    if (!used[j] && guess.user_guess[i] == secret_code[j])
                    {
                        correct_digit++;
                        used[j] = true;
                        break;
                    }
                }
            }

            guess.correct_position = correct_position;
            guess.correct_digit_wrong_position = correct_digit;

            if (copy_to_user((void __user *)arg, &guess, sizeof(guess)))
                return -EFAULT;

            memset(codecracker_buffer, 0, CODECRACKER_BUF_SIZE);
            snprintf(codecracker_buffer, CODECRACKER_BUF_SIZE,
                    "Guess %d: %s | Correct Position: %d, Wrong Position: %d\n",
                    attempts, guess.user_guess, correct_position, correct_digit);
                codecracker_offset_r = 0;

                return 0;

            default:
                return -EINVAL;

    }
}

module_init(codecracker_init);
module_exit(codecracker_end);
