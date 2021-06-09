// GDV_CV_VCPKG.cpp : Diese Datei enthält die Funktion "main". Hier beginnt und endet die Ausführung des Programms.
//

#include <iostream>
#include <opencv2/opencv.hpp>


void aufgabe2() {
    // read yoshi and convert to hsv
    cv::Mat yoshi = cv::imread("C:\\Users\\lars\\Documents\\Uni\\SS21\\gdv\\Praktikum3\\yoshi.png");
    cv::cvtColor(yoshi, yoshi, cv::COLOR_BGR2HSV);

    // read mask and convert to grayscale
    cv::Mat mask = cv::imread("C:\\Users\\lars\\Documents\\Uni\\SS21\\gdv\\Praktikum3\\mask.png");
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

void aufgabe2_alt() {

}

enum ColorSpace {
    RGB, 
    HSV, 
    LAB
};

void aufgabe3(ColorSpace cs = ColorSpace::LAB, std::string window_name= "") {
    // read input and target 
    cv::Mat input = cv::imread("C:\\Users\\lars\\Documents\\Uni\\SS21\\gdv\\Praktikum3\\FigSource.png");
    cv::Mat target = cv::imread("C:\\Users\\lars\\Documents\\Uni\\SS21\\gdv\\Praktikum3\\FigTarget.png");

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

    aufgabe3(ColorSpace::RGB, " RGB");
    aufgabe3(ColorSpace::HSV, " HSV");
    aufgabe3(ColorSpace::LAB, " LAB");
    return 0;
    aufgabe2();

    

    return 0;

    /*
    double minVal, maxVal;
    cv::Point minLoc, maxLoc;

    cv::minMaxLoc(maskCopy, &minVal, &maxVal, &minLoc, &maxLoc);
    std::cout << "Min Val: " << minVal << " Max Val: " << maxVal << '\n';
    */
    
    


    return 0;
    cv::Mat frame = cv::imread("C:\\Users\\lars\\Documents\\Uni\\SS21\\gdv\\Praktikum3\\yoshi.png"); // Read the file
    if (frame.empty()) // Check for invalid input
    {
        std::cout << "Could not open or find the frame" << std::endl;
        return -1;
    }
    cv::namedWindow("Window", cv::WINDOW_AUTOSIZE); // Create a window
    cv::imshow("Window", frame); // Show our image inside it.
    cv::waitKey(0);

    std::cout << "Anzahl Farbkanaele: " << frame.channels() << '\n';
    std::cout << "Beite: " << frame.cols << " Hoehe: " << frame.rows << '\n';
    
    cv::Mat frameFloat;
    frame.convertTo(frameFloat, CV_32FC3, 1/255.0);
    std::cout << (frameFloat.type() == CV_32FC3) << '\n';


    //cv::Rect r = cv::Rect(frameFloat.cols / 2, frameFloat.rows / 2, 10, 10);
    //cv::rectangle(frameFloat, r, cv::Scalar(0,0,255), 2, 8, 0);

    for (int i = 0; i < frameFloat.cols; i++) {
        
        if (i % 5) {
            cv::Rect r = cv::Rect(0, i, frameFloat.rows, 1);
            cv::rectangle(frameFloat, r, cv::Scalar(0, 0, 0), 0, 8, 0);
        }
    }

    cv::namedWindow("Window", cv::WINDOW_AUTOSIZE);
    cv::imshow("Window", frameFloat);
    cv::waitKey(0); // Wait for a keystroke in the window

    cv::imwrite("C:\\Users\\lars\\Documents\\Uni\\SS21\\gdv\\Praktikum3\\yoshi_write.png",frame);

    return 0;
}

// Programm ausführen: STRG+F5 oder Menüeintrag "Debuggen" > "Starten ohne Debuggen starten"
// Programm debuggen: F5 oder "Debuggen" > Menü "Debuggen starten"

// Tipps für den Einstieg: 
//   1. Verwenden Sie das Projektmappen-Explorer-Fenster zum Hinzufügen/Verwalten von Dateien.
//   2. Verwenden Sie das Team Explorer-Fenster zum Herstellen einer Verbindung mit der Quellcodeverwaltung.
//   3. Verwenden Sie das Ausgabefenster, um die Buildausgabe und andere Nachrichten anzuzeigen.
//   4. Verwenden Sie das Fenster "Fehlerliste", um Fehler anzuzeigen.
//   5. Wechseln Sie zu "Projekt" > "Neues Element hinzufügen", um neue Codedateien zu erstellen, bzw. zu "Projekt" > "Vorhandenes Element hinzufügen", um dem Projekt vorhandene Codedateien hinzuzufügen.
//   6. Um dieses Projekt später erneut zu öffnen, wechseln Sie zu "Datei" > "Öffnen" > "Projekt", und wählen Sie die SLN-Datei aus.
