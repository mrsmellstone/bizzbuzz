#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

char mygetc(int fd){
    char buf = EOF;
    int er = 1;
    if((er = read(fd, &buf, 1)) < 0){
        printf("Error: cant read\n");
        return -1;
    }
    if(er == 0){
        return EOF;
    }
    return buf;
}

int main(int argc, char** argv)
{
    if(argc < 3){
        printf("Error: too few arguments\n");
        return 0;
    }
    int in = open(argv[1], O_RDONLY);
    if(in < 0){
        printf("Error: can't open in\n");
        return 0;
    }
    int out = open(argv[2], O_WRONLY);
    if(out < 0){
        printf("Error: can't open out\n");
        return 0;
    }
    char c = mygetc(in);
    while(c != EOF){
        int pos = 0;
        int valid = 1;
        int sum = 0;
        int last_char = -1;
        while(c == '\n' || c == ' '){
            if(write(out, &c, 1) < 0){
                printf("cant write1\n");
                return 0;
            }
            c = mygetc(in);
        }
        if(c == '+' || c == '-'){
            pos++;
            c = mygetc(in);
        }
        while(c != '\n' && c != ' ' && c != EOF){
            if(c < '0' || c > '9'){
                valid = 0;
            } else {
                sum += c - '0';
                last_char = c;
            }
            pos++;
            c = mygetc(in);
        }
        int flag = 1;
        if(valid){
            if(sum % 3 == 0){
                if(write(out, "bizz", 4) < 0){
                    printf("cant write1\n");
                    return 0;
                }
                flag = 0;
            }
            if(last_char == '0' || last_char == '5'){
                if(write(out, "buzz", 4) < 0){
                    printf("cant write2\n");
                    return 0;
                }
                flag = 0;
            }
            if(flag)
                valid = 0;
        }
        if(c != EOF)
            pos++;
        if(!valid){
            if(lseek(in, -pos, SEEK_CUR) < 0){
                printf("Error lseek\n");
                return 0;
            }
            c = mygetc(in);
            while(c != '\n' && c != ' ' && c != EOF){
                if(write(out, &c, 1) < 0){
                    printf("cant write4\n");
                    return 0;
                }
                c = mygetc(in);
            }
        }
    }
    close(out);
    close(in);
    return 0;
}

