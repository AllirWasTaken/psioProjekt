#include <iostream>
#include <stop_token>
#include <thread>

#include "Config.h"
#include "ObjectDetection.h"
#include "Image.h"


// Has to be after winsock2.h
#include <windows.h>


void ImageProcessingService(std::stop_token stopToken, Config& config)
try
{
    config.fps = 0;
    std::vector<uint8_t> buffer;
    Image image(config.videoX,config.videoY);
    Image workImage(config.videoWorkX,config.videoWorkY);
    ObjectDetection detector;


    while(config.work){
        //Recive frame from frontEnd
        config.MeasureFps();

        image.ConvertStreamToImage(buffer);
        // Cut originalImage to smaller size to limit detection area
        image.CutImage(workImage);

        if (config.calibrationMode)
        {
            // Calibration loop
            for (int y = 0; y < config.videoWorkY; y++)
            {
                for (int x = 0; x < config.videoWorkX; x++)
                {
                    workImage[y][x].b = 255;
                }
            }
        }
        else{
            //Main loop for program
            //Image processing for detection
            workImage.EdgeDetection(config.edgeDetectionThreshold,config.edgeRange);
            workImage.BlobEdges(config.blobEdgesAmount);
            workImage.FilterOutNoise(config.filterNoiseThreshold);
            workImage.Antialiasing(config.antialiasingIterations);

            //Detecting objects in image
            if(config.detectObjects){
                detector.DetectObjects(workImage,config.ObjectNoiseThreshold);
            }

            if(config.debugMode&&config.detectObjects){
                detector.DrawObjects(workImage);
            }
            detector.CalculateObjectsVariables();

            // Detecting objects in image
        }

        if (config.debugMode || config.calibrationMode)
        {
            // Fit for debuging
            image.FitIntoImage(workImage);
        }
        else{
            detector.OffestObjects((config.videoX-config.videoWorkX)/2, (config.videoY-config.videoWorkY)/2);
            for(int i=0;i<detector.GetObjects().size();i++){
                image.DrawSquare(detector.GetObjects()[i].pos.x,detector.GetObjects()[i].pos.y,10);
            }
        }

        // Send frame to frontEnd
        image.ConvertImageToStream(buffer);
        config.MeasureTime();
    }
    exit(0);
}
catch (const std::exception &e)
{
    std::cout << "Error in service thread:" << e.what() << std::endl;
    exit(-1);
}
catch (...)
{
    std::cout << "Unknown error in service thread" << std::endl;
    exit(-1);
}

//Menu functions
void Settings(Config &config)
{
    system("cls");
    std::cout<<"Select setting to change or click enter to return\n\n";
    std::cout<<"command|variable name|current value\n\n";
    std::cout<<"edge|edgeDetectionThreshold|"<<config.edgeDetectionThreshold<<"\n";
    std::cout<<"blob|blobEdgesAmount|"<<config.blobEdgesAmount<<"\n";
    std::cout<<"filter|filterNoiseThreshold|"<<config.filterNoiseThreshold<<"\n";
    std::cout<<"anti|antialiasingIterations|"<<config.antialiasingIterations<<"\n";
    std::cout<<"objfilter|ObjectNoiseThreshold|"<<config.ObjectNoiseThreshold<<"\n";
    std::cout<<"detect|detectObjects|"<<config.detectObjects<<"\n";
    std::cout<<">>";


    char commandbuffer[101];
    std::string command;
    int commandSelected;
    scanf("%100[^'\n']", commandbuffer);
    while (getchar() != '\n')
        ;
    command = commandbuffer;
    if (command == "edge")
    {
        commandSelected = 1;
    }
    else if (command == "blob")
    {
        commandSelected = 2;
    }
    else if (command == "filter")
    {
        commandSelected = 3;
    }
    else if(command=="objfilter"){
        commandSelected=5;
    }
    else if(command=="detect"){
        commandSelected=6;
    }
    else if(command=="anti"){
        commandSelected=4;
    }
    else
    {
        return;
    }

    std::cout << "Give new value for setting:\n>>";
    int newValue;
    int check = scanf("%d", &newValue);
    while (getchar() != '\n')
        ;
    if (!check)
        return;

    if (commandSelected == 1)
    {
        config.edgeDetectionThreshold = newValue;
    }
    else if (commandSelected == 2)
    {
        config.blobEdgesAmount = newValue;
    }
    else if (commandSelected == 3)
    {
        config.filterNoiseThreshold = newValue;
    }
    else if (commandSelected == 4)
    {
        config.antialiasingIterations = newValue;
    }
    else if(commandSelected==5){
        config.ObjectNoiseThreshold=newValue;
    }
    else if(commandSelected==6){
        config.detectObjects=newValue;
    }

}

void FpsDisplayFunc(std::stop_token stopToken, const Config& config)
{
    system("cls");
    COORD pos = {0, 0};
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    while (!stopToken.stop_requested())
    {
        SetConsoleCursorPosition(hConsole, pos);
        float delivery=1000/config.fps;
        delivery=delivery-config.time;
        std::cout<<"fps: "<<config.fps<<"   \n";
        std::cout<<"processing time ms: "<<config.time<<"   \n";
        std::cout<<"data delivery time ms:"<<delivery<<"    \n";
        std::cout<<"Press Enter to leave fps view\n";

        Sleep(50);
    }
}
void FpsFunc(const Config& config)
{
    std::jthread fpsDisplayThread(FpsDisplayFunc, config);
    while (getchar() != '\n');
}
void Help()
{
    std::cout << "You can use following commands:\n\t";
    std::cout << "help "
              << "fps "
              << "objects "
              << "stats "
              << "settings "
              << "calibration "
              << "debug "
              << "sample "
              << "clear "
              << "exit "
              << "\n";
}

void Objects()
{
}

void ObjectStats()
{
}

void Sample()
{
}



int main(void)
try
{
    // Setting up console for nicer display
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = false;
    SetConsoleCursorInfo(hConsole, &cursorInfo);

    // Initalization of config and command handling

    Config config;
    config.SetDefault();
    char commandbuffer[101];
    std::string command;

    // Service start up
    std::jthread serviceThread(ImageProcessingService, std::ref(config));

    // Menu for controling the processing settings
    while (true)
    {
        // Main menu display, fps refresed every 50 milisec
        std::cout << "Fruit detecion project"
                  << "\n";
        std::cout << ">>";

        scanf("%100[^'\n']", commandbuffer);
        while (getchar() != '\n')
            ;
        command = commandbuffer;
        if (command == "help")
        {
            Help();
        }
        else if (command == "fps")
        {
            FpsFunc(config);
            system("cls");
        }
        else if (command == "objects")
        {
            Objects();
        }
        else if (command == "settings")
        {
            Settings(config);
            system("cls");
        }
        else if (command == "calibration")
        {
            config.calibrationMode = !config.calibrationMode;
        }
        else if (command == "debug")
        {
            config.debugMode = !config.debugMode;
        }
        else if (command == "clear")
        {
            system("cls");
        }
        else if (command == "stats")
        {
            ObjectStats();
        }
        else if (command == "sample")
        {
            Sample();
        }
        else if (command == "exit")
        {
            config.work = false;
            break;
        }
        else
        {
            std::cout << "Command has not been found, type \"help\" to see command list\n";
        }
    }
    return 0;
}
catch(const std::exception& e)
{
    std::cout << "Fatal error: " << e.what() << std::endl;
}
catch(...)
{
    std::cout << "Unknown fatal error" << std::endl;
}