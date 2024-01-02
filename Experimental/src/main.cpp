#include <iostream>
#include <thread>
#include "Sockets.h"
#include <windows.h>
#include "Image.h"
#include "Config.h"
#include "ObjectDetection.h"

Config config;

void *ImageProcessingService(void* arg){
    Config *configPtr=(Config*)arg;
    configPtr->fps=0;
    SocketServer server(12345);
    server.WaitForClientToConnect();
    std::vector<uint8_t> buffer;
    Image image(configPtr->videoX,configPtr->videoY);
    Image workImage(configPtr->videoWorkX,configPtr->videoWorkY);
    ObjectDetection detector;


    while(configPtr->work){
        //Recive frame from frontEnd
        if(server.ReciveData(buffer))break;
        configPtr->MeasureFps();
        image.ConvertStreamToImage(buffer);
        //Cut originalImage to smaller size to limit detection area
        image.CutImage(workImage);

        if(configPtr->calibrationMode){
            //Calibration loop
            for(int y=0;y<configPtr->videoWorkY;y++){
                for(int x=0;x<configPtr->videoWorkX;x++){
                    workImage[y][x].b=255;
                }
            }
        }
        else{
            //Main loop for program
            //Image processing for detection
            workImage.EdgeDetection(configPtr->edgeDetectionThreshold);
            workImage.BlobEdges(configPtr->blobEdgesAmount);
            workImage.FilterOutNoise(configPtr->filterNoiseThreshold);
            workImage.Antialiasing(configPtr->antialiasingIterations);

            //Detecting objects in image
            if(configPtr->detectObjects){
                detector.DetectObjects(workImage,configPtr->ObjectNoiseThreshold);
            }

            if(configPtr->debugMode&&configPtr->detectObjects){
                detector.DrawObjects(workImage);
            }
            detector.CalculateObjectsVariables();

        }
        

        
        if(configPtr->debugMode||configPtr->calibrationMode){
            //Fit for debuging
            image.FitIntoImage(workImage);
        }
        else{
            detector.OffestObjects((configPtr->videoX-configPtr->videoWorkX)/2, (configPtr->videoY-configPtr->videoWorkY)/2);
            for(int i=0;i<detector.GetObjects().size();i++){
                image.DrawSquare(detector.GetObjects()[i].pos.x,detector.GetObjects()[i].pos.y,10);
            }
        }

        //Send frame to frontEnd
        image.ConvertImageToStream(buffer);
        configPtr->MeasureTime();
        server.SendData(buffer);
    }

    exit(0);
    return nullptr;
}

//Menu functions
void Settings(){
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
    scanf("%100[^'\n']",commandbuffer);
    while(getchar()!='\n');
    command=commandbuffer;
    if(command=="edge"){
        commandSelected=1;
    }
    else if(command=="blob"){
        commandSelected=2;
    }
    else if(command=="filter"){
        commandSelected=3;
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
    else {
        return;
    }

    std::cout<<"Give new value for setting:\n>>";
    int newValue;
    int check=scanf("%d",&newValue);
    while(getchar()!='\n');
    if(!check)return;


    if(commandSelected==1){
        config.edgeDetectionThreshold=newValue;
    }
    else if(commandSelected==2){
        config.blobEdgesAmount=newValue;
    }
    else if(commandSelected==3){
        config.filterNoiseThreshold=newValue;
    }
    else if(commandSelected==4){
        config.antialiasingIterations=newValue;
    }
    else if(commandSelected==5){
        config.ObjectNoiseThreshold=newValue;
    }
    else if(commandSelected==6){
        config.detectObjects=newValue;
    }



}
void* FpsDisplayFunc(void* args){
    system("cls");
    int *work=(int*)args;
    COORD pos = {0, 0};
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    

    while(*work){
        SetConsoleCursorPosition(hConsole, pos);
        float delivery=1000/config.fps;
        delivery=delivery-config.time;
        std::cout<<"fps: "<<config.fps<<"   \n";
        std::cout<<"processing time ms: "<<config.time<<"   \n";
        std::cout<<"data delivery time ms:"<<delivery<<"    \n";
        std::cout<<"Press Enter to leave fps view\n";
        Sleep(50);
    }
    

    return nullptr;
}
void FpsFunc(){
    int work=1;
    std::thread fpsDisplayThread(FpsDisplayFunc,(void*)&work);
    while(getchar()!='\n');
    work=0;
    fpsDisplayThread.join();
}
void Help(){
    std::cout<<"You can use following commands:\n\t";
    std::cout<<"help "
    <<"fps "
    <<"objects "
    <<"stats "
    <<"settings "
    <<"calibration "
    <<"debug "
    <<"sample "
    <<"clear "
    <<"exit "
    <<"\n";
}

void Objects(){

}

void ObjectStats(){

}

void Sample(){

}




int main(void){
    //Setting up console for nicer display
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO     cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible=false;
    SetConsoleCursorInfo(hConsole, &cursorInfo);

    //Initalization of config and command handling

    config.SetDefault();
    char commandbuffer[101];
    std::string command;


    //Service start up
    std::thread serviceThread(ImageProcessingService,(void*)&config);



    //Menu for controling the processing settings
    while(true){
        //Main menu display, fps refresed every 50 milisec
        std::cout<<"Fruit detecion project"<<"\n";
        std::cout<<">>";

        scanf("%100[^'\n']",commandbuffer);
        while(getchar()!='\n');
        command=commandbuffer;
        if(command=="help"){
            Help();
        }
        else if(command=="fps"){
            FpsFunc();
            system("cls");
        }
        else if(command=="objects"){
            Objects();
        }
        else if(command=="settings"){
            Settings();
            system("cls");
        }
        else if(command=="calibration"){
            config.calibrationMode=!config.calibrationMode;
        }
        else if(command=="debug"){
            config.debugMode=!config.debugMode;
        }
        else if(command=="clear"){
            system("cls");
        }
        else if(command=="stats"){
            ObjectStats();
        }
        else if(command=="sample"){
            Sample();
        }
        else if(command=="exit"){
            config.work=false;
            break;
        }
        else{
            std::cout<<"Command has not been found, type \"help\" to see command list\n";
        }
    }



    serviceThread.join();

    return 0;
}