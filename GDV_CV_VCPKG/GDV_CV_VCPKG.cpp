// GDV_CV_VCPKG.cpp : Diese Datei enthält die Funktion "main". Hier beginnt und endet die Ausführung des Programms.
//

#include <iostream>
#include <opencv2/opencv.hpp>


static const std::string PATH_YOSHI = "C:\\Users\\lars\\Documents\\Uni\\SS21\\gdv\\Praktikum3\\yoshi.png";
static const std::string PATH_MASK = "C:\\Users\\lars\\Documents\\Uni\\SS21\\gdv\\Praktikum3\\mask.png";
static const std::string PATH_INPUT = "C:\\Users\\lars\\Documents\\Uni\\SS21\\gdv\\Praktikum3\\FigSource.png";
static const std::string PATH_TARGET = "C:\\Users\\lars\\Documents\\Uni\\SS21\\gdv\\Praktikum3\\FigTarget.png";
static const std::string SAVE_TO_PATH = "C:\\Users\\lars\\Documents\\Uni\\SS21\\gdv\\Praktikum3\\yoshi_save.png";

void aufgabe1() {
    // read yoshi 
    cv::Mat yoshi = cv::imread(PATH_YOSHI);

    // print width, height, and number of channels to the console
    std::cout << "Breite: " << yoshi.cols << " Hoehe: " << yoshi.rows << " Farbkanaele: " << yoshi.channels() << '\n';
    
    // convert to floating point
    yoshi.convertTo(yoshi, CV_32FC3, 1 / 255.0);

    cv::namedWindow("Aufgabe1", cv::WINDOW_AUTOSIZE); // Create a window
    cv::imshow("Aufgabe1", yoshi); // Show our image inside it.
    cv::waitKey(0);

    // specify the rectangle and draw it to the yoshi image
    cv::Rect r(yoshi.cols / 2, yoshi.rows / 2, 10, 10);
    cv::rectangle(yoshi, r, cv::Scalar(0, 0, 255), -1); // -1 for filling the rectangle

    // set each 5. row to 0 
    for (int i = 0; i < yoshi.rows; i++) {
        if (!(i % 5))
            yoshi.row(i) *= 0; // easy solution 

            // more explicit solution
            //yoshi.row(i) = cv::Mat::zeros(cv::Size(yoshi.cols, 1), CV_32FC3);
    }
    cv::namedWindow("Aufgabe1", cv::WINDOW_AUTOSIZE); // Create a window
    cv::imshow("Aufgabe1", yoshi); // Show our image inside it.
    cv::waitKey(0);

    cv::imwrite(SAVE_TO_PATH, yoshi);
    
}

void aufgabe2() {
    // read yoshi and convert to hsv
    cv::Mat yoshi = cv::imread(PATH_YOSHI);
    cv::cvtColor(yoshi, yoshi, cv::COLOR_BGR2HSV);

    // read mask and convert to grayscale
    cv::Mat mask = cv::imread(PATH_MASK);
    cv::cvtColor(mask, mask, cv::COLOR_BGR2GRAY);

    cv::Mat yoshiEdit; // placeholder for the resulting image
    cv::Mat maskCopy = mask.clone(); // clone to add hue over the mask area

    // split yoshi into channels 
    std::vector<cv::Mat> channels;
    split(yoshi, channels);
    cv::Mat hChannel = channels[0];

    // bitwise not the mask to invert it 
    cv::bitwise_not(mask, mask);
    // and the inverted mask and the h-channel -> all masked values are 0
    cv::bitwise_and(hChannel, mask, hChannel);
    // scale the original mask to the desired hue value and add it to the h-channel
    maskCopy = ((maskCopy / 255) * 125);
    hChannel += maskCopy;

    // merge the channels back together
    channels[0] = hChannel;
    merge(channels, yoshiEdit);
    cv::cvtColor(yoshiEdit, yoshiEdit, cv::COLOR_HSV2BGR);

    cv::namedWindow("Window", cv::WINDOW_AUTOSIZE); // Create a window
    cv::imshow("Window", yoshiEdit); // Show our image inside it.
    cv::waitKey(0);
}


enum ColorSpace {
    RGB, 
    HSV, 
    LAB
};

void aufgabe3(ColorSpace cs = ColorSpace::LAB, std::string window_name= "") {
    // read input and target 
    cv::Mat input = cv::imread(PATH_INPUT);
    cv::Mat target = cv::imread(PATH_TARGET);

    cv::namedWindow("Original"+window_name, cv::WINDOW_AUTOSIZE); // Create a window
    cv::imshow("Original" + window_name, input); // Show our image inside it.

    // convert input and target to floating point in range (0,1)
    input.convertTo(input, CV_32FC3, 1 / 255.0);
    target.convertTo(target, CV_32FC3, 1 / 255.0);

    switch (cs)
    {
    case ColorSpace::LAB:
        // convert input and target to lab colorspace
        cv::cvtColor(input, input, cv::ColorConversionCodes::COLOR_BGR2Lab);
        cv::cvtColor(target, target, cv::ColorConversionCodes::COLOR_BGR2Lab);
        break;
    case ColorSpace::HSV:
        // convert input and target to hsv colorspace
        cv::cvtColor(input, input, cv::ColorConversionCodes::COLOR_BGR2HSV);
        cv::cvtColor(target, target, cv::ColorConversionCodes::COLOR_BGR2HSV);
        break;
    case ColorSpace::RGB:
        // convert input and target to hsv colorspace
        cv::cvtColor(input, input, cv::ColorConversionCodes::COLOR_BGR2RGB);
        cv::cvtColor(target, target, cv::ColorConversionCodes::COLOR_BGR2RGB);
        break;
    }
    

    // split input and taget into channels 
    std::vector<cv::Mat> inputChannels;
    split(input, inputChannels);
    std::vector<cv::Mat> targetChannels; 
    split(target, targetChannels);
    
    cv::Scalar inputMeans; 
    cv::Scalar inputStds;
    cv::meanStdDev(input, inputMeans, inputStds);

    cv::Scalar targetMeans;
    cv::Scalar targetStds;
    cv::meanStdDev(target, targetMeans, targetStds);
    
    // calculations for each channel 
    for (int i = 0; i < 3; i++) {
        inputChannels[i] = (((inputChannels[i] - inputMeans[i]) / inputStds[i]) * targetStds[i]) + targetMeans[i];
    }
    merge(inputChannels, input);

    // convert input back to bgr
    switch (cs)
    {
    case ColorSpace::LAB:
        
        cv::cvtColor(input, input, cv::ColorConversionCodes::COLOR_Lab2BGR);
        break;
    case ColorSpace::HSV:
        cv::cvtColor(input, input, cv::ColorConversionCodes::COLOR_HSV2BGR);
        break;
    case ColorSpace::RGB:
        cv::cvtColor(input, input, cv::ColorConversionCodes::COLOR_RGB2BGR);
        break;
    }
    
    cv::namedWindow("Result" + window_name, cv::WINDOW_AUTOSIZE); // Create a window
    cv::imshow("Result" + window_name, input); // Show our image inside it.
    cv::waitKey(0);
}

int main()
{
    aufgabe1();
    aufgabe2();
    aufgabe3(ColorSpace::RGB, " RGB");
    aufgabe3(ColorSpace::HSV, " HSV");
    aufgabe3(ColorSpace::LAB, " LAB");
    return 0;
}
