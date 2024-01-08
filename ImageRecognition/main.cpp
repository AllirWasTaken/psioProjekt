#include <iostream>
#include <stop_token>
#include <thread>

#include "Config.h"
#include "ObjectDetection.h"
#include "Image.h"
#include "StreamReciver.h"


// Has to be after winsock2.h
#include <windows.h>


void ImageProcessingService(std::stop_token stopToken, Config& config)
try
{
    CameraStream camera;
    config.fps = 0;
    std::vector<uint8_t> buffer;
    Image image(config.videoX,config.videoY);
    Image edgedImage(config.videoX,config.videoY);
    Image workImage(config.videoWorkX,config.videoWorkY);
    ObjectDetection detector;


    while(config.work){
        //Recive frame from frontEnd

        config.MeasureTransfer();
        camera.GetFrame(image);
        camera.GetEdgedImage(edgedImage,config.edgeDetectionThreshold1,config.edgeDetectionThreshold2);
        config.MeasureTransfer();
        config.MeasureFps();
        // Cut originalImage to smaller size to limit detection area
        edgedImage.CutImage(workImage);

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
            workImage.BlobEdges(config.blobEdgesAmount);
            workImage.FilterOutNoise(config.filterNoiseThreshold);

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
                Object & tempObj=detector.GetObjects()[i];
                image.DrawSquare(tempObj.pos.x,tempObj.pos.y,tempObj.width,tempObj.height);
            }
        }
        config.MeasureFps();
        // Send frame to frontEnd
        camera.DisplayFrame(image);
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
    std::cout<<"edge1|edgeDetectionThreshold1|"<<config.edgeDetectionThreshold1<<"\n";
    std::cout<<"edge2|edgeDetectionThreshold2|"<<config.edgeDetectionThreshold2<<"\n";
    std::cout<<"blob|blobEdgesAmount|"<<config.blobEdgesAmount<<"\n";
    std::cout<<"filter|filterNoiseThreshold|"<<config.filterNoiseThreshold<<"\n";
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
    if (command == "edge1")
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
    else if(command=="edge2"){
        commandSelected=4;
    }
    else
    {
        return;
    }

    std::cout << "Give new value for setting:\n>>";
    int newValue;
    int check = scanf("%d", &newValue);
    while (getchar() != '\n');
    if (!check)
        return;

    if (commandSelected == 1)
    {
        config.edgeDetectionThreshold1 = newValue;
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
        config.edgeDetectionThreshold2 = newValue;
    }
    else if(commandSelected==5){
        config.ObjectNoiseThreshold=newValue;
    }
    else if(commandSelected==6){
        config.detectObjects=newValue;
    }

}
struct ThreadPack{
    int work;
    Config& config;
};
void FpsDisplayFunc(ThreadPack* pack)
{


    system("cls");
    COORD pos = {0, 0};
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    while (pack->work)
    {
        SetConsoleCursorPosition(hConsole, pos);
        std::cout<<"fps: "<<pack->config.fps<<"   \n";
        std::cout<<"processing time ms: "<<pack->config.time<<"   \n";
        std::cout<<"data transfer time ms: "<<pack->config.transferTime<<"   \n";
        std::cout<<"Press Enter to leave fps view\n";

        Sleep(50);
    }
}
void FpsFunc(Config& config)
{
    ThreadPack pack{
        .work=1,
        .config=config
    };
    std::thread fpsDisplayThread(FpsDisplayFunc, &pack);
    while (getchar() != '\n');
    pack.work=0;
    fpsDisplayThread.join();
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