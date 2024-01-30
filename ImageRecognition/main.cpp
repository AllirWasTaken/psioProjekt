#include <iostream>
#include <stop_token>
#include <thread>

#include "Config.h"
#include "ObjectDetection.h"
#include "Image.h"
#include "StreamReciver.h"
#include "Labeler.h"
#include "ObjectClassifier.h"
#include "Predictor.h"


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
    Image colorCut(config.videoWorkX,config.videoWorkY);
    Predictor predictor;
    ObjectDetection detector;
    ObjectClassifier classifier(&config);
    Labeler labeler("../labels/");
    classifier.AnalyzeSavedSamples("../samples/");
    TaskStack helperStack(1000);
    Pixel background;
    background=255;

    if(!config.video){
        camera.Connect();
    }


    while(!stopToken.stop_requested()){
        //Recive frame from frontEnd

        config.MeasureTransfer();
        if(config.video){

        }
        else{
            camera.GetFrame(image);
        }
        //camera.GetEdgedImage(edgedImage,config.edgeDetectionThreshold1,config.edgeDetectionThreshold2);
        config.MeasureTransfer();
        config.MeasureFps();
        // Cut originalImage to smaller size to limit detection area
        image.CutImage(workImage);


        if(config.analyzeBackground){
            background=workImage.AnalzyzeBackground();
            config.analyzeBackground=0;
        }
        workImage.SetBackground(background);

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

            //OLD
            //workImage.BlobEdges(config.blobEdgesAmount);
            //END OLD

            workImage.BlobDetectionWithBackground(config.backgroundTolerance);
            workImage.FilterOutNoise(helperStack,config.filterNoiseThreshold);

            //Detecting objects in image
            if(config.detectObjects){

                detector.DetectObjects(workImage,config.ObjectNoiseThreshold);

            }

            if(config.debugMode&&config.detectObjects){

                detector.DrawObjects(workImage);

            }
            detector.CalculateObjectsVariables();
            image.CutImage(colorCut);
            for(int i=0;i<detector.Size();i++){
                classifier.AnalyzeAndCategorize(colorCut,detector[i]);
            }
        }

        if (config.debugMode || config.calibrationMode)
        {
            // Fit for debuging
            image.FitIntoImage(workImage);
        }
        else{
            detector.OffestObjects((config.videoX-config.videoWorkX)/2, (config.videoY-config.videoWorkY)/2);
            predictor.UpdatePredictor(detector.GetObjs());
            for(int i=0;i<detector.Size();i++){
                labeler.Label(image,detector[i]);
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

void FpsDisplayFunc(std::stop_token stopToken, Config& config)
{
    system("cls");
    COORD pos = {0, 0};
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    while (!stopToken.stop_requested())
    {
        SetConsoleCursorPosition(hConsole, pos);
        std::cout << "fps: " << config.fps << "   \n";
        std::cout << "processing time ms: " << config.time << "   \n";
        std::cout << "data transfer time ms: " << config.transferTime << "   \n";
        std::cout << "Press Enter to leave fps view\n";

        Sleep(50);
    }
}

void FpsFunc(Config& config)
{
    std::jthread fpsDisplayThread(FpsDisplayFunc, std::ref(config));
    while (getchar() != '\n');
}

void Help()
{
    std::cout << "You can use following commands:\n\t";
    std::cout << "help "
              << "fps "
              << "stats "
              << "settings "
              << "calibration "
              << "debug "
              << "clear "
              << "exit "
              << "analyze"
              << "\n";
}


void ObjectStatsDisplay(std::stop_token stopToken, Config& config)
{
    system("cls");
    COORD pos = {0, 0};
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    while (!stopToken.stop_requested())
    {
        SetConsoleCursorPosition(hConsole, pos);
        std::cout<<"Apples: "<<Predictor::Get(APPLE)<<std::endl;
        std::cout<<"Oranges: "<<Predictor::Get(ORANGE)<<std::endl;
        std::cout<<"Kiwis: "<<Predictor::Get(KIWI)<<std::endl;
        std::cout<<"Avocados: "<<Predictor::Get(AVOCADO)<<std::endl;
        std::cout<<"Lemons: "<<Predictor::Get(CITRUS)<<std::endl;

        Sleep(50);
    }
}


void ObjectStats(Config& config)
{
    std::jthread fpsDisplayThread(ObjectStatsDisplay, std::ref(config));
    while (getchar() != '\n');
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
    /*
    std::cout<<"video?"<<std::endl;

    int val=0;
    scanf("%d",&val);
    while(getchar()!='\n');

    if(val){
        config.video=true;
    }
    else{
        config.video=false;
    }
    */
    config.video=false;



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
        else if (command == "analyze")
        {
            config.analyzeBackground=1;
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
            ObjectStats(config);
        }
        else if (command == "exit")
        {
            serviceThread.request_stop();
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