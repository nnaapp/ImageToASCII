#ifndef _IMAGE_H
#define _IMAGE_H

#include <iostream>
#include <vector>
#include <string>

using namespace std;

// rgb struct, to store color data of pixels neatly
struct rgb
{
    int red;
    int green;
    int blue;
};

// image struct, to store pixel and dimensional details of an iamge
struct image
{
    vector<unsigned char> pixels;
    int width;
    int height;
    int elementspp;
};

// Tables of characters, organized from most dense to most sparse, and the inverse of that
const string BRIGHTNESS_TABLE = "$@B%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/\\|()1{}[]?-_+~<>i!lI;:,\"^`\'.";
const string BRIGHTNESS_TABLE_INV = ".\'`^\",:;Il!i><~+_-?][}{1)(|\\/tfjrxnuvczXYUJCLQ0OZmwqpdbkhao*#MW&8%B@$";
// Constant, calculated based on the number of characters in the prior table, to divide a number 0-255 by and receive a number 0-BRIGHTNESS_TABLE.length()
const double BLOCK_TO_ASCII = 255.0 / static_cast<double>(BRIGHTNESS_TABLE.length() - 1);

bool loadImage(image &imageData, const string &fileName);

void grayscale(image &imageData);

void contrast(image &imageData, double contrast);

rgb getPixel(vector<unsigned char> &pixels, int index);

void writeImage(image &imageData, string fileName);

void writeASCII(image imageData, string fileName, int blockSize, string background);

#endif