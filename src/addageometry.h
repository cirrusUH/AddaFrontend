#ifndef ADDAGEOMETRY_H
#define ADDAGEOMETRY_H

#include <netcdf.h>

#include <string>
#include <iostream>
#include <ios>
#include <complex>
#include <vector>

#include <QString>

using std::cout;
using std::endl;

class AddaGeometry
{
    std::string filename;
    size_t geom_xDim, geom_yDim, geom_zDim;
    unsigned long long occNdpls;
    int Nmat;
    double gridsize;
    bool _hasmesh;
    bool _hasgridsize;
    bool _hasmaterial;

    std::vector<double> materiallist_re;
    std::vector<double> materiallist_im;

public:
    AddaGeometry(std::string filename);

    int getXdim();
    int getYdim();
    int getZdim();
    int getNmat();
    unsigned long long getOccNdpls();
    double getGridSize();
    const QString getFileName();

    bool hasmaterials();
    bool hasmesh();
    bool hasgridsize();

    std::vector<double> getMaterialList_re();
    std::vector<double> getMaterialList_im();

    double getFirstMaterial_re();
    double getFirstMaterial_im();

    size_t computeMemoryPerProcessor();
    size_t computeMemoryTotal();

};

#endif // ADDAGEOMETRY_H
