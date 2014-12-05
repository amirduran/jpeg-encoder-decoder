#include "exportpicture.h"
#include <QColor>


ExportPicture::ExportPicture():luminance(0,vector<int>(0,0)),chrominanceCb(0,vector<int>(0,0)),chrominanceCr(0,vector<int>(0,0)),height(0),width(0),fileName(0){



}
ExportPicture::ExportPicture(QImage &image, const QString &fileName):luminance(0,vector<int>(0,0)),chrominanceCb(0,vector<int>(0,0)),chrominanceCr(0,vector<int>(0,0)),height(image.width()),width(image.height()),fileName(fileName){

    //Picture dimensions must be devidible by 8. If they are not, then we have to set them upp
    if(height%8 == 0 && width%8 == 0){
        luminance.resize(height);
        chrominanceCb.resize(height);
        chrominanceCr.resize(height);

        for(int i=0;i<height;i++){
            luminance[i].resize(width);
            chrominanceCb[i].resize(width);
            chrominanceCr[i].resize(width);
        }
    }
    else if(height%8==0 && width%8!=0){
        luminance.resize(height);
        chrominanceCb.resize(height);
        chrominanceCr.resize(height);

        for(unsigned int i=0;i<luminance.size();i++){
            luminance[i].resize(width+8-(width%8));
            chrominanceCb[i].resize(width+8-(width%8));
            chrominanceCr[i].resize(width+8-(width%8));
        }
    }
    else if(height%8!=0 && width%8==0){
        luminance.resize(height+8-(height%8));
        chrominanceCb.resize(height+8-(height%8));
        chrominanceCr.resize(height+8-(height%8));

        for(unsigned int i=0;i<luminance.size();i++){
            luminance[i].resize(width);
            chrominanceCb[i].resize(width);
            chrominanceCr[i].resize(width);

        }
    }
    else if(height%8!=0 && width%8!=0){
        luminance.resize(height+8-(height%8));
        chrominanceCb.resize(height+8-(height%8));
        chrominanceCr.resize(height+8-(height%8));

        for(unsigned int i=0;i<luminance.size();i++){
            luminance[i].resize(width+8-(width%8));
            chrominanceCb[i].resize(width+8-(width%8));
            chrominanceCr[i].resize(width+8-(width%8));
        }
    }

    fillVectorsWithData(image);
}

void ExportPicture::fillVectorsWithData(QImage &image){
    QRgb rgb;

    for(int i=0;i<height;i++){
        for(int j=0;j<width;j++){
            rgb=image.pixel(j,i);
            QColor c(rgb);
            luminance[i][j]=c.red();
            chrominanceCb[i][j]=c.green();
            chrominanceCr[i][j]=c.blue();
        }
    }
}

void ExportPicture::deleteDataFromMatrix(){
    luminance.clear();
    chrominanceCb.clear();
    chrominanceCr.clear();
}
