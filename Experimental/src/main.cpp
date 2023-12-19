#include <iostream>
#include "Sockets.h"
#include "windows.h"
#include "Image.h"


#define VIDEO_X 720
#define VIDEO_Y 720
#define VIDEO_WORK_X 720
#define VIDEO_WORK_Y 560
#define CALIBTATION_FRAMES 25

int main(void){

    SocketServer server(12345);
    server.WaitForClientToConnect();
    std::vector<uint8_t> buffer;
    Image image(VIDEO_X,VIDEO_Y);
    Image workImage(VIDEO_WORK_X,VIDEO_WORK_Y);
    int calibration=CALIBTATION_FRAMES;


    while(true){
        //Recive frame from frontEnd
        if(server.ReciveData(buffer))break;
        image.ConvertStreamToImage(buffer);
        //Cut originalImage to smaller size to limit detection area
        image.CutImage(workImage);

        if(calibration){
            //Calibration loop
            for(int y=0;y<VIDEO_WORK_Y;y++){
                for(int x=0;x<VIDEO_WORK_X;x++){
                    workImage[y][x].b=255;
                }
            }
            calibration--;
            
        }
        else{
            //Main loop for program
            workImage.EdgeDetection(13);
            workImage.BlobEdges(4);
            workImage.FillObjectsWithBackgroundFilling();
        }
        

        
        
        //Fit for debuging
        image.FitIntoImage(workImage);

        //Send frame to frontEnd
        image.ConvertImageToStream(buffer);
        server.SendData(buffer);
    }

    Sleep(10);


    return 0;
}