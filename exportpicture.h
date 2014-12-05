#ifndef EXPORTPICTURE_H
#define EXPORTPICTURE_H

using namespace std;
#include "vector"
#include <QImage>
#include <iostream>

class ExportPicture
{
public:
    vector<vector<int> >luminance;
    vector<vector<int> >chrominanceCb;
    vector<vector<int> >chrominanceCr;
    int height;
    int width;
    const QString &fileName;
    ExportPicture();
    ExportPicture(QImage &image, const QString &fileName);
    void fillVectorsWithData(QImage &image);
    void deleteDataFromMatrix();

};

#endif // EXPORTPICTURE_H
