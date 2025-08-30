#include <math.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

float A, B, C;
float CubeWidth = 20;
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
float HorizontalOffset;

float calculateX(int i, int j, int k) {
    return j * sin(A) * sin(B) * cos(C) - k * cos(A) * sin(B) * cos(C) +
           j * cos(A) * sin(C) + k * sin(A) * sin(C) + i * cos(B) * cos(C); 
}

float calculateY(int i, int j, int k) {
    return j * cos(A) * cos(C) + k * sin(A) * cos(C) -
           j * sin(A) * sin(B) * sin(C) + k * cos(A) * sin(B) * sin(C) -
           i * cos(B) * sin(C); 
}

float calculateZ(int i, int j, int k) {
    return k * cos(A) * cos(B) - j * sin(A) * cos(B) + i * sin(B);
}

void calculateForSurface(float cubeX, float cubeY, float cubeZ, int ch) {
    x = calculateX(cubeX, cubeY, cubeZ);
    y = calculateY(cubeX, cubeY, cubeZ);
    z = calculateZ(cubeX, cubeY, cubeZ) + DistanceFromCam;

    ooz = 1 / z;

    xp = (int)(width / 2 + HorizontalOffset + K1 * ooz * x * 2);
    yp = (int)(height / 2 + K1 * ooz * y);

    idx = xp + yp * width;
    if (idx >= 0 && idx < width * height) {
        if (ooz > zBuffer[idx]) {
            zBuffer[idx] = ooz;
            buffer[idx] = ch;
        }
    }
}

int main() {
    printf("\x1b[2J");
    while (1) {
        memset(buffer, backgroundASCIICode, width * height);
        memset(zBuffer, 0, width * height * sizeof(float));
        CubeWidth = 10;
        HorizontalOffset = -2 * CubeWidth;
        
        // First cube
        for (float cubeX = -CubeWidth; cubeX < CubeWidth; cubeX += IncrementSpeed) {
            for (float cubeY = -CubeWidth; cubeY < CubeWidth; cubeY += IncrementSpeed) {
                calculateForSurface(cubeX, cubeY, -CubeWidth, '@');
                calculateForSurface(CubeWidth, cubeY, cubeX, '$');
                calculateForSurface(-CubeWidth, cubeY, -cubeX, '~');
                calculateForSurface(-cubeX, cubeY, CubeWidth, '#');
                calculateForSurface(cubeX, -CubeWidth, -cubeY, ';');
                calculateForSurface(cubeX, CubeWidth, cubeY, '+');
            }
        }

        CubeWidth = 10;
        HorizontalOffset = 1 * CubeWidth;
        
        // Second cube
        for (float cubeX = -CubeWidth; cubeX < CubeWidth; cubeX += IncrementSpeed) {
            for (float cubeY = -CubeWidth; cubeY < CubeWidth; cubeY += IncrementSpeed) {
                calculateForSurface(cubeX, cubeY, -CubeWidth, '@');
                calculateForSurface(CubeWidth, cubeY, cubeX, '$');
                calculateForSurface(-CubeWidth, cubeY, -cubeX, '~');
                calculateForSurface(-cubeX, cubeY, CubeWidth, '#');
                calculateForSurface(cubeX, -CubeWidth, -cubeY, ';');
                calculateForSurface(cubeX, CubeWidth, cubeY, '+');
            }
        }

        CubeWidth = 5;
        HorizontalOffset = 8 * CubeWidth;
        
        // Third cube
        for (float cubeX = -CubeWidth; cubeX < CubeWidth; cubeX += IncrementSpeed) {
            for (float cubeY = -CubeWidth; cubeY < CubeWidth; cubeY += IncrementSpeed) {
                calculateForSurface(cubeX, cubeY, -CubeWidth, '@');
                calculateForSurface(CubeWidth, cubeY, cubeX, '$');
                calculateForSurface(-CubeWidth, cubeY, -cubeX, '~');
                calculateForSurface(-cubeX, cubeY, CubeWidth, '#');
                calculateForSurface(cubeX, -CubeWidth, -cubeY, ';');
                calculateForSurface(cubeX, CubeWidth, cubeY, '+');
            }
        }

        printf("\x1b[H");
        for (int i = 0; i < width * height; i++) {
            putchar(i % width ? buffer[i] : '\n');
        }

        A += 0.05;
        B += 0.05;
        C += 0.01;
        usleep(80000); 
    }
    return 0;
}