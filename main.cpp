#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include<unistd.h>
#include <string>
#include <iostream>
#include <chrono>
#include <thread>
#include <time.h>


#define I2C_ADDRESS 0x68
#define DEV_REGISTERS_NUM 7

class clocks {
public:
    clocks() {};

    void setTime(int h, int m, int s) {
        if (conected) {
            char buf[10];
            buf[0] = 0x00;
            buf[1] = s;
            buf[2] = m;
            buf[3] = h;
            write(f, buf, 4);
        }
    }

    void setDate(int year, int month, int date, int day) {
        if (conected) {
            char buf[10];
            buf[0] = 0x04;
            buf[1] = day;
            buf[2] = date;
            buf[3] = month;
            buf[4] = year;
            write(f, buf, 4);
        }
    }

    void readTime() {
        if (conected) {
            char buf[10];
            buf[0] = 0x00;
            write(f, buf, 1);
            read(f,buf,DEV_REGISTERS_NUM);
            for(int i = 2; i >= 0; i--) {
                (i == 0) ? printf("%02X", buf[i]) : printf("%02X:", buf[i]);
            }
        }
    }

    void readDate() {
        if (conected) {
            char buf[10];
            buf[0] = 0x00;
            write(f, buf, 1);
            read(f,buf,DEV_REGISTERS_NUM);
            for(int i = 4; i< 7; i++) {
                (i ==6) ? printf("%02X", buf[i]) : printf("%02X.", buf[i]);
            }
        }
    }

    void end() {
        if (fileOpen)
            close(f);
        delete this;
    }

    void openAdapter(const char* location) {
        f = open(location, O_RDWR);
        /* Open the adapter and set the address of the I2C devuce */
        if ( f < 0 ) {
            perror("/dev/i2c-1:");
            return;
        }
        fileOpen = true;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }

    void getAcess(int ADDRESS) {
        if (fileOpen) {
            if (ioctl(f, I2C_SLAVE, ADDRESS) < 0) {
                perror("Failed to acquire bus access and/or talk to slave");
                return;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            conected = true;
            return;
        }
        perror("Adapter is not opened");
    }
    bool fileOpen = false;
    bool conected = false;
private:
    int f;
};

int main()
{
    clocks M41T00;
    M41T00.openAdapter("/dev/i2c-0");
    M41T00.getAcess(I2C_ADDRESS);
    M41T00.setTime(0,0,0);
    M41T00.setDate(0,0,0, 0);
    /*Loop below shows device time along with real time*/
    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        std::cout << "RTC Device time: ";
        M41T00.readTime();
        std::cout << " ";
        M41T00.readDate();
        time_t tm=time(NULL);
        std::cout << ", real time: " << ctime(&tm);
    }
}

