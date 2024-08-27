#include "helpers.h"
#include <math.h>

RGBTRIPLE setBlurPixel(int startI, int endI, int startJ, int endJ, float totalPixelSum, int height,
                       int width, RGBTRIPLE image[height][width]);

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int colorRGBAverage =
                round((image[i][j].rgbtRed + image[i][j].rgbtGreen + image[i][j].rgbtBlue) / 3.0);

            image[i][j].rgbtRed = colorRGBAverage;
            image[i][j].rgbtGreen = colorRGBAverage;
            image[i][j].rgbtBlue = colorRGBAverage;
        }
    }
    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int sepiaRed = round((.393 * image[i][j].rgbtRed) + (.769 * image[i][j].rgbtGreen) +
                                 (.189 * image[i][j].rgbtBlue));
            int sepiaGreen = round((.349 * image[i][j].rgbtRed) + (.686 * image[i][j].rgbtGreen) +
                                   (.168 * image[i][j].rgbtBlue));
            int sepiaBlue = round((.272 * image[i][j].rgbtRed) + (.534 * image[i][j].rgbtGreen) +
                                  (.131 * image[i][j].rgbtBlue));

            if (sepiaRed > 255)
            {
                image[i][j].rgbtRed = 255;
            }
            else
            {
                image[i][j].rgbtRed = sepiaRed;
            }

            if (sepiaGreen > 255)
            {
                image[i][j].rgbtGreen = 255;
            }
            else
            {
                image[i][j].rgbtGreen = sepiaGreen;
            }

            if (sepiaBlue > 255)
            {
                image[i][j].rgbtBlue = 255;
            }
            else
            {
                image[i][j].rgbtBlue = sepiaBlue;
            }
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE tempMatrix[height][width];

    for (int i = 0; i < height; i++)
    {
        for (int j = 0, k = width - 1; j < width; j++, k--)
        {
            tempMatrix[i][k] = image[i][j];
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = tempMatrix[i][j];
        }
    }

    // Primeiro codigo que por algum motivo que eu nao sei nao funciona:
    // RGBTRIPLE tempPixel;
    // for (int i = 0; i < height; i++)
    // {
    //     for (int j = 0, k = width - 1; j < width; j++, k--)
    //     {
    //         tempPixel = image[i][k];
    //         image[i][k] = image[i][j];
    //         image[i][j] = tempPixel;
    //     }
    // }
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE tempMatrix[height][width];

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (i == 0)
            {
                if (j == 0)
                {
                    tempMatrix[i][j] = setBlurPixel(i, i+1, j, j+1, 4.0, height, width, image);
                }
                else if (j == width - 1)
                {
                    tempMatrix[i][j] = setBlurPixel(i, i+1, j-1, j, 4.0, height, width, image);
                }
                else
                {
                    tempMatrix[i][j] = setBlurPixel(i, i+1, j-1, j+1, 6.0, height, width, image);
                }
            }
            else if (i == height - 1)
            {
                if (j == 0)
                {
                    tempMatrix[i][j] = setBlurPixel(i-1, i, j, j+1, 4.0, height, width, image);
                }
                else if (j == width - 1)
                {
                    tempMatrix[i][j] = setBlurPixel(i-1, i, j-1, j, 4.0, height, width, image);
                }
                else
                {
                    tempMatrix[i][j] = setBlurPixel(i-1, i, j-1, j+1, 6.0, height, width, image);
                }
            }
            else
            {
                if (j == 0)
                {
                    tempMatrix[i][j] = setBlurPixel(i-1, i+1, j, j+1, 6.0, height, width, image);
                }
                else if (j == width - 1)
                {
                    tempMatrix[i][j] = setBlurPixel(i-1, i+1, j-1, j, 6.0, height, width, image);
                }
                else
                {
                    tempMatrix[i][j] = setBlurPixel(i-1, i+1, j-1, j+1, 9.0, height, width, image);
                }
            }
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = tempMatrix[i][j];
        }
    }
}

RGBTRIPLE setBlurPixel(int startI, int endI, int startJ, int endJ, float totalPixelSum, int height,
                       int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE tempPixel;
    int tempPixelRed = 0;
    int tempPixelGreen = 0;
    int tempPixelBlue = 0;

    while (startI <= endI)
    {
        int tempStartJ = startJ;
        while (tempStartJ <= endJ)
        {
            tempPixelRed += image[startI][tempStartJ].rgbtRed;
            tempPixelGreen += image[startI][tempStartJ].rgbtGreen;
            tempPixelBlue += image[startI][tempStartJ].rgbtBlue;

            tempStartJ++;
        }
        startI++;
    }

    tempPixel.rgbtRed = round(tempPixelRed / totalPixelSum);
    tempPixel.rgbtGreen = round(tempPixelGreen / totalPixelSum);
    tempPixel.rgbtBlue = round(tempPixelBlue / totalPixelSum);
    return tempPixel;
}
