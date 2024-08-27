#include "helpers.h"
#include <math.h>
#include <stdio.h>

void edgesDefineTempMatrix(int heightTempMatrix, int widthTempMatrix,
                           RGBTRIPLE tempMatrix[heightTempMatrix][widthTempMatrix], int height,
                           int width, RGBTRIPLE image[height][width]);
RGBTRIPLE setBlurPixel(int startI, int endI, int startJ, int endJ, float totalPixelSum, int height,
                       int width, RGBTRIPLE image[height][width]);
RGBTRIPLE setEdgePixel(int startHeight, int startWidth, int kernelX[3][3], int kernelY[3][3],
                       int heightTempMatrix, int widthTempMatrix,
                       RGBTRIPLE tempMatrix[heightTempMatrix][widthTempMatrix]);

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

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    int heightTempMatrix = height + 2;
    int widthTempMatrix = width + 2;
    RGBTRIPLE tempMatrix[heightTempMatrix][widthTempMatrix];

    edgesDefineTempMatrix(heightTempMatrix, widthTempMatrix, tempMatrix, height, width, image);

    int kernelX[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int kernelY[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] =
                setEdgePixel(i, j, kernelX, kernelY, heightTempMatrix, widthTempMatrix, tempMatrix);
        }
    }
}

void edgesDefineTempMatrix(int heightTempMatrix, int widthTempMatrix,
                           RGBTRIPLE tempMatrix[heightTempMatrix][widthTempMatrix], int height,
                           int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE blackPixel;
    blackPixel.rgbtRed = 0;
    blackPixel.rgbtGreen = 0;
    blackPixel.rgbtBlue = 0;

    for (int i = 0; i < heightTempMatrix; i++)
    {
        if (i == 0)
        {
            for (int j = 0; j < widthTempMatrix; j++)
            {
                tempMatrix[i][j] = blackPixel;
            }
        }
        else if (i == heightTempMatrix - 1)
        {
            for (int j = 0; j < widthTempMatrix; j++)
            {
                tempMatrix[i][j] = blackPixel;
            }
        }
        else
        {
            tempMatrix[i][0] = blackPixel;

            tempMatrix[i][widthTempMatrix - 1] = blackPixel;
        }
    }

    for (int i = 1; i < heightTempMatrix - 1; i++)
    {
        for (int j = 1; j < widthTempMatrix - 1; j++)
        {
            tempMatrix[i][j] = image[i - 1][j - 1];
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

RGBTRIPLE setEdgePixel(int startHeight, int startWidth, int kernelX[3][3], int kernelY[3][3],
                       int heightTempMatrix, int widthTempMatrix,
                       RGBTRIPLE tempMatrix[heightTempMatrix][widthTempMatrix])
{
    RGBTRIPLE tempPixel;
    int redGx = 0;
    int greenGx = 0;
    int blueGx = 0;
    int redGy = 0;
    int greenGy = 0;
    int blueGy = 0;

    for (int i = startHeight, ikernel = 0; i < startHeight + 3; i++, ikernel++)
    {
        for (int j = startWidth, jKernel = 0; j < startWidth + 3; j++, jKernel++)
        {
            redGx += tempMatrix[i][j].rgbtRed * kernelX[ikernel][jKernel];
            greenGx += tempMatrix[i][j].rgbtGreen * kernelX[ikernel][jKernel];
            blueGx += tempMatrix[i][j].rgbtBlue * kernelX[ikernel][jKernel];
            redGy += tempMatrix[i][j].rgbtRed * kernelY[ikernel][jKernel];
            greenGy += tempMatrix[i][j].rgbtGreen * kernelY[ikernel][jKernel];
            blueGy += tempMatrix[i][j].rgbtBlue * kernelY[ikernel][jKernel];
        }
    }

    int sumGxGyRed = round(sqrt((double) (redGx * redGx) + (redGy * redGy)));
    int sumGxGyGreen = round(sqrt((double) (greenGx * greenGx) + (greenGy * greenGy)));
    int sumGxGyBlue = round(sqrt((double) (blueGx * blueGx) + (blueGy * blueGy)));

    if (sumGxGyRed > 255)
    {
        tempPixel.rgbtRed = 255;
    }
    else
    {
        tempPixel.rgbtRed = sumGxGyRed;
    }
    if (sumGxGyGreen > 255)
    {
        tempPixel.rgbtGreen = 255;
    }
    else
    {
        tempPixel.rgbtGreen = sumGxGyGreen;
    }
    if (sumGxGyBlue > 255)
    {
        tempPixel.rgbtBlue = 255;
    }
    else
    {
        tempPixel.rgbtBlue = sumGxGyBlue;
    }

    return tempPixel;
}
