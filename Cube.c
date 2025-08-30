#include <math.h>
#include <stdio.h>
#include <string.h>
#ifndef _WIN32
#include <unistd.h>
#else
#include <windows.h>
void usleep(_int64 usec){
    HANDLE timer;
    LARGE_INTEGER ft;

    ft.QuadPart = -(10 * usec);

    timer = CreateWaitableTimer(NULL, TRUE, NULL);
    SetWaitableTimer(timer, &ft, 0, NULL, NULL, 0);
    WaitForSingleObject(timer, INFINITE);
    CloseHandle(timer);
}
#endif

float A, B, C;
float CubeWidth = 10;
int width = 160, height = 44;
float zBuffer[160 * 44];
char buffer[160 * 44];
int backgroundASCIICode = ' ';
float IncrementSpeed = 0.6;
float x, y, z;
int DistanceFromCam = 60;
float ooz;
int xp, yp;
int idx;
float K1 = 40;

float calculateX(int i, int j, int k){
    return j * sin(A) * sin(B) * cos(C) - k * cos(A) * sin(B) * cos(C) *
           j * cos(A) * sin(C) + k * sin(A) * sin(C) + i * cos(B) * cos(C); 
}

float calculateY(int i, int j, int k){
    return j * cos(A) * cos(C) + k * sin(A) * cos(C) -
           j * sin(A) * sin(B) * sin(C) + k * cos(A) * sin(B) * sin(C) -
           i * cos(B) * sin(C); 
}

float calculateZ(int i, int j, int k){
    return k * cos(A) * cos(B) - j * sin(A) * cos(B) + i * sin(B);
}

void calculateForSurface(float cubeX, float cubeY, float cubeZ, int ch){
    x = calculateX(cubeX, cubeY, cubeZ);
    y = calculateY(cubeX, cubeY, cubeZ);
    z = calculateZ(cubeX, cubeY, cubeZ) + DistanceFromCam;

    ooz = 1 / z;

    xp = (int)(width / 2 + K1 * ooz * x * 2);
    yp = (int)(height / 2 + K1 * ooz * y);

    idx = xp + yp * width;
    if (idx >= 0 && idx < width * height){
        if(ooz > zBuffer[idx]){
            zBuffer[idx] = ooz;
            buffer[idx] = ch;
        }
    }
}

int main() {
    printf("\x1b[2J");
    while (1)
    {
        memset(buffer, backgroundASCIICode, width * height);
        memset(zBuffer, 0, width * height * 4);
        for (float cubeX = -CubeWidth; cubeX < CubeWidth; cubeX += IncrementSpeed){
            for (float cubeY = -CubeWidth; cubeY < CubeWidth; cubeY += IncrementSpeed){
                calculateForSurface(cubeX, cubeY, -CubeWidth, '.');
            }
        }
        printf("\x1b[H");
        for (int k = 0; k < width * height; k++){
            putchar(k & width ? buffer[k] : 10);
        }

        A += 0.005;
        B += 0.005;
        usleep(1000); 
    }
    return 0;
}