#include "image.h"

using namespace std;

string getInImage();
string getOutImage();
string getOutTxt();
double getContrast();
int getBlockSize();
string getBackground();

int main(int argc, char *argv[])
{
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
    cout << "IF THE PROGRAM CRASHED WITHOUT AN ERROR MESSAGE, THE IMAGE YOU GAVE WAS OPENED BUT COULD NOT BE READ, TRY A DIFFERENT FORMAT OR TAKE A PNG SCREENSHOT OF IT" << endl << endl;

    const int EXPECTED_ARGUMENTS = 7; // Constant for how many arguments were expected from the user
    int argumentsCount = argc; // Get number of arguments given

    if (argumentsCount > 7) // Return 1 if too many arguments were given, just to avoid possible errors and correct the user
    {
        cout << "Too many arguments, syntax: *.exe inputImage outputImage outputTextFile contrastValue(0.0-4.0) blockSize background(dark/light)" << endl;
        return 1;
    }

    string inImageName, outImageName, outTxtName, background;
    int blockSize;
    double contrastVal;

    // Loops as argumentsCount(argc) times, to put each argv into its associated (easier to read) variable
    // This probably isn't the best solution, but loops and switches seemed like the way to go
    for (int i = 1; i < argumentsCount; i++)
    {
        switch (i)
        {
            case 1:
                inImageName = argv[1];
                break;
            case 2:
                outImageName = argv[2];
                break;
            case 3:
                outTxtName = argv[3];
                break;
            case 4:
                contrastVal = stof(argv[4]);
                break;
            case 5:
                blockSize = stoi(argv[5]);
                break;
            case 6:
                background = argv[6];
                break;
            default:
                cout << "No command line arguments given, syntax: *.exe inputImage outputImage outputTextFile contrastValue(0.0-4.0) blockSize background(dark/light)" << endl << endl;
                break;
        }
    }
    // The same as above, but it runs until argumentsCount is equal to the expected number of arguments
    // Instead of reading from argv, it fills in the blank spots with user input, to get the complete set of variables needed
    while (argumentsCount < EXPECTED_ARGUMENTS)
    {
        switch (argumentsCount)
        {
            case 1:
                cout << "Input image name not given as argument." << endl;
                inImageName = getInImage();
                break;
            case 2:
                cout << "Outout image name not given as argument." << endl;
                outImageName = getOutImage();
                break;
            case 3:
                cout << "Output text file name not given as argument." << endl;
                outTxtName = getOutTxt();
                break;
            case 4:
                cout << "Contrast value not given as argument." << endl;
                contrastVal = getContrast();
                break;
            case 5:
                cout << "Block size not given as argument." << endl;
                blockSize = getBlockSize();
                break;
            case 6:
                background = "dark";
                break;
        }
        argumentsCount++;
    }

    // If the contrast value given as an argument is invalid, get a new one from the user
    if (contrastVal < 0.0 || contrastVal > 4.0)
    {
        cout << "Contrast value argument was invalid." << endl;
        contrastVal = getContrast();
    }

    // Opens the image file from the user, returning an error if it doesn't open, and getting dimensional information by reference
    int width, height;
    int elements;
    const int variance = 25;
    image imageData;
    bool success = loadImage(imageData, inImageName);
    if (!success)
    {
        cout << "Failed to load image. Exitting.";
        return 1;
    }

    // IMAGE PRE-PROCESSING, for a cleaner ASCII output
    grayscale(imageData);
    contrast(imageData, contrastVal);
    // Write out an image of the pre-processing
    writeImage(imageData, outImageName);
    // Write out the ASCII results of the processed image
    writeASCII(imageData, outTxtName, blockSize, background);
}

// Gets the name/file path of the input image
string getInImage()
{
    string name;
    cout << "Input image (file extension included!): ";
    getline(cin, name);
    cout << endl;

    return name;
}

// Gets the name/file path of the output image
string getOutImage()
{
    string name;
    cout << "Output image (.png included!): ";
    getline(cin, name);
    cout << endl;

    return name;
}

// Gets the name/file path of the output text file
string getOutTxt()
{
    string name;
    cout << "Output text file name (File extension included!): ";
    getline(cin, name);
    cout << endl;

    return name;
}

// Gets contrast value
double getContrast()
{
    double contrastVal;
    cout << "Contrast value (0.0 - 4.0): ";
    cin >> contrastVal;
    while (contrastVal < 0.0 || contrastVal > 4.0)
    {
        cout << "Invalid contrast value, pick one between 0.0 and 4.0: ";
        cin >> contrastVal;
    }
    cin.ignore();
    cout << endl;

    return contrastVal;
}

// Gets the block size (height, width is calculated from this)
int getBlockSize()
{
    int blockSize;
    cout << "Block size: ";
    cin >> blockSize;
    while (blockSize < 1)
    {
        cout << "Invalid, please enter an integer greater than 0: ";
        cin >> blockSize;
    }
    cin.ignore();
    cout << endl;

    return blockSize;
}