#include "image.h"

using namespace std;

int main()
{
    // Prints a prompt presenting viable image types, and prompts the user for an input image name
    string fileName;
    cout << "\t~IMAGE TO ASCII CONVERTER~" << endl;
    cout << "Supported file types:" << endl;
    cout << "\tJPEG, non-12 bits per channel, and non-arithmetic" << endl;
    cout << "\tPNG 1/2/4/8/16 bits per channel" << endl;
    cout << "\tTGA" << endl;
    cout << "\tBMP, non-1 bit per channel, and non-RLE" << endl;
    cout << "\tPSD, composited, 8/16 bits per channel" << endl;
    cout << "\tGIF, non-animated" << endl;
    cout << "\tHDR, radiance RGBE format" << endl;
    cout << "\tPIC, softimage" << endl;
    cout << "\tPNM, PMM and PGM binaries" << endl;
    cout << "IF THE PROGRAM CRASHED WITHOUT AN ERROR MESSAGE, THE IMAGE YOU GAVE WAS OPENED BUT COULD NOT BE READ, TRY A DIFFERENT FORMAT OR TAKE A PNG SCREENSHOT OF IT (SHIFT + WINDOWS + S)" << endl;
    cout << "Image file directory: ";
    getline(cin, fileName);
    cout << endl;

    // Opens the image file from the user, returning an error if it doesn't open, and getting dimensional information by reference
    int width, height;
    int elements;
    const int variance = 25;
    image imageData;
    bool success = loadImage(imageData, fileName);
    if (!success)
    {
        cout << "Failed to load image. Exitting.";
        return 1;
    }

    // IMAGE PRE-PROCESSING, for a cleaner ASCII output
    grayscale(imageData);
    contrast(imageData);
    // Write out an image of the pre-processing
    writeImage(imageData);
    // Write out the ASCII results of the processed image
    writeASCII(imageData);
}