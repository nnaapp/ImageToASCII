#include "image.h"
#include <cmath>
#include <fstream>
#define STB_IMAGE_IMPLEMENTATION // Image reading library (stbi_load(), stbi_image_free())
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION // Image writing library (stbi_write_png())
#include "stb_image_write.h"

// Loads an image from a file into an unsigned char *, converts it to a vector of unsigned chars if the load was successful, 
// and then returns true or false depending on the success of loading
bool loadImage(image &imageData, const string &fileName)
{
    unsigned char *data = stbi_load(fileName.c_str(), &imageData.width, &imageData.height, &imageData.elementspp, 0); // Loads the pixel data into the char *, stores the width/height/num elements per pixel; the 0 tells the function not to force a specific number of elements per pixel
    if (data != nullptr) // If data is not a null pointer, fill out the vector
    {
        imageData.pixels = vector<unsigned char>(data, data + imageData.width * imageData.height * 4);
    }
    stbi_image_free(data);
    return (data != nullptr); // Return true or false based on whether or not data is a null pointer
}

// Takes an image data vector as well as the width/height/elements per pixel of the data, and then converts it to grayscale
void grayscale(image &imageData)
{
    const int WIDTH = imageData.width;
    const int HEIGHT = imageData.height;
    const int ELEMENTSPP = imageData.elementspp;

    // Nested loops to run through the width AND height of the vector; it is one-dimensional, but it is being treated as a two-dimensional array mathematically
    for (int x = 0; x < WIDTH; x++)
    {
        for (int y = 0; y < HEIGHT; y++)
        {
            // Calculates the index to go to, since the vector is one-dimensional; y * w accounts for "height", adding x accounts for "width", 
            // and multiplying by elements accounts for multiple elements per pixel
            int index = ELEMENTSPP * (y * WIDTH + x);

            rgb curPixel = getPixel(imageData.pixels, index);
            
            // Calculates the weighted sum of the color values, which each color value must be set to in order to achieve grayscale
            // Loops through all 3 colors mathematically and applies the sum
            int weightedSum = static_cast<int>((curPixel.red * .299) + (curPixel.green * .587) + (curPixel.blue * .114));
            for (int i = 0; i < 3; i++)
            {
                imageData.pixels.at(index + i) = weightedSum;
            }
        }
    }

    return;
}

// Takes an image data vector as well as the width/height/elements per pixel of the data, and then increases the contrast based on user input
void contrast(image &imageData, double contrast)
{
    // Now having the contrast value, all possible outputs for values 0-255 can be calculated and casted to unsigned chars
    // Using a loop, a lookup table of contrast outputs is generated to avoid repeating any work over thousands of pixels
    unsigned char *contrast_table = new unsigned char [256];
    double newValue;
    for (int i = 0; i < 256; i++)
    {
        newValue = static_cast<double>(i);
        newValue = ((((newValue / 255) - .5) * contrast) + .5) * 255; // (color - 127.5) * c + 127.5

        if (newValue < 0)
            newValue = 0;
        else if (newValue > 255)
            newValue = 255;

        contrast_table[i] = static_cast<unsigned char>(newValue);
    }

    const int WIDTH = imageData.width;
    const int HEIGHT = imageData.height;
    const int ELEMENTSPP = imageData.elementspp;

    // Nested loops to run through the width AND height of the vector; it is one-dimensional, but it is being treated as a two-dimensional array mathematically
    for (int x = 0; x < WIDTH; x++)
    {
        for (int y = 0; y < HEIGHT; y++)
        {
            // Calculates the index to go to, since the vector is one-dimensional; y * w accounts for "height", adding x accounts for "width", 
            // and multiplying by elements accounts for multiple elements per pixel
            int index = ELEMENTSPP * (y * WIDTH + x);

            rgb curPixel = getPixel(imageData.pixels, index);

            // Looks up the contrast change for each color value in the current pixel, changing them accordingly 
            for (int i = 0; i < 3; i++)
            {
                imageData.pixels.at(index + i) = contrast_table[static_cast<int>(imageData.pixels.at(index + i))];
            }
        }
    }

    delete[] contrast_table;

    return;
}

// Takes an index and image vector, and returns the color values in an rgb struct for that pixel
rgb getPixel(vector<unsigned char> &pixels, int index)
{
    rgb pixel;
    pixel.red = static_cast<int>(pixels.at(index + 0)); 
    pixel.green = static_cast<int>(pixels.at(index + 1)); 
    pixel.blue = static_cast<int>(pixels.at(index + 2));

    return pixel;
}

// Writes the modified image out to a PNG file of the user's choice, taking the image and its width/height/elements per pixel as arguments
// Outputs a PNG even if the input was anything else
void writeImage(image &imageData, string fileName)
{
    // Converts the vector BACK to a pointer, which was its original format when it was read from an image
    int numPixels = imageData.pixels.size();
    unsigned char *imageptr;
    imageptr = new unsigned char [numPixels];
    for (int i = 0; i < numPixels; i++)
    {
        imageptr[i] = imageData.pixels.at(i);
    }

    const int WIDTH = imageData.width;
    const int HEIGHT = imageData.height;
    const int ELEMENTSPP = imageData.elementspp;

    // Writes the pointer out to a PNG file in the directory of the executable
    stbi_write_png(fileName.c_str(), WIDTH, HEIGHT, ELEMENTSPP, imageptr, WIDTH * ELEMENTSPP);
    delete[] imageptr;
    cout << "Image output complete." << endl << endl;
    return;
}

// Writes the given image out as ASCII text by averaging the brightnesses of "blocks" of pixels 
// (assuming grayscale) and running that through a global table that corresponds to ASCII characters
void writeASCII(image imageData, string fileName, int blockSize, string background)
{
    ofstream outFile(fileName);

    // Calculate the block width from the block height (aka blockSize)
    const double BLOCK_W_H_RATIO = 0.375;
    int blockH = blockSize;
    int blockW = round(static_cast<double>(blockH) * (BLOCK_W_H_RATIO)); // Casts the height to a double, then multiples it by the % of the height the width should be

    // Set the inversion bool to true or false based on the background brightness
    bool inverse;
    if (background.compare("dark") == 0)
    {
        inverse = true;
    }
    else if (background.compare("light") == 0)
    {
        inverse = false;
    }
    else
    {
        cout << "Invalid background brightness argument (not \"dark\" or \"light\"), defaulting to \"dark\"." << endl;
        inverse = true;
    }

    const int WIDTH = imageData.width;
    const int HEIGHT = imageData.height;
    const int ELEMENTSPP = imageData.elementspp;

    // Nested loops to run through the width AND height of the vector; it is one-dimensional, but it is being treated as a two-dimensional array mathematically
    // Instead of incrementing by one, this increments by blockH/blockW, so as to avoid re-reading pixels that are read in the SECOND nested for loops
    for (int y = 0; y < HEIGHT - blockH; y += blockH)
    {
        for (int x = 0; x < WIDTH - blockW; x += blockW)
        {
            // Calculates the index to go to, since the vector is one-dimensional; y * w accounts for "height", adding x accounts for "width", 
            // and multiplying by elements accounts for multiple elements per pixel
            int index = ELEMENTSPP * (y * WIDTH + x);

            int blockSum = 0;
            // Reads blockW pixels "right" and blockH pixels "down", which the first loops will skip, and sums all of their brightness values
            // Reads only the red of the rgb, as they are all the same (again, assuming grayscale)
            for (int bh = 0; bh < blockH; bh++)
            {
                for (int bw = 0; bw < blockW; bw++)
                {
                    int subIndex = index + (ELEMENTSPP * (bh * WIDTH + bw));
                    blockSum += static_cast<int>(imageData.pixels.at(subIndex));
                }
            }
            // Averages the sum of brightness values for this block
            double blockAvg = blockSum / (blockW * blockH);
            // Divides the average by the header's BLOCK_TO_ASCII constant to get an in-bounds number, rounding to an int
            int tableIndex = round(blockAvg / BLOCK_TO_ASCII);
            
            // Switching inversion based on the prior calculated value, put the tableIndex integer through the table of ASCII characters, and print the result to the output text file
            if (!inverse)
                outFile << BRIGHTNESS_TABLE.at(tableIndex);
            else if (inverse)
                outFile << BRIGHTNESS_TABLE_INV.at(tableIndex);
        }
        outFile << endl;
    }

    outFile.close();

    cout << "Text output complete.";

    return;
}