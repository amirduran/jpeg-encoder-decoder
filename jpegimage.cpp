#include "jpegimage.h"
#include <QDebug>

JPEGImage::JPEGImage(): imageFilter("JPEG File (*.jpg)") {
    extensionList.push_back( "*.jpeg" );
    extensionList.push_back("*.jpg");
}

JPEGImage::~JPEGImage() {
    //delete jpeg
}

void JPEGImage::init(QWidget*) {
}

void JPEGImage::deinit() {
}

QString JPEGImage::filter() const {
    return imageFilter;
}

bool JPEGImage::exportImage(const ImageStatePtr imageState, const QString& filename) {
    if(imageState->format()!=QImage::Format_RGB888){
            cout<<"Invalid picture format. Picture format must be RGB 888";
            return false;
        }
    LayerPtr currentLayer=imageState->activeLayer();
    QImage currentPicture(currentLayer->getData(),imageState->width(),imageState->height(),QImage::Format_RGB888);
    JpegEncode j(currentPicture,filename);
    j.savePicture();
    return true;
}

QStringList JPEGImage::extensions() const {
    return extensionList;
}

ImageStatePtr JPEGImage::importImage(const QString& filename) {
    try {
        QFile inputFile(filename);
        JpegDecode picture(inputFile);
        //return ImageStatePtr(new ImageState(filename, QImage(picture.getImage())));
        return picture.imagePtr;
    } catch(const char err[]) {
        qDebug()<<"JPEG error: "<<err;
        return ImageStatePtr(0);
    }
}

Q_EXPORT_PLUGIN2(plugin_JPEGImage, JPEGImage)
