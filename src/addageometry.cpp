#include "addageometry.h"

AddaGeometry::AddaGeometry(std::string filename)
    : filename( filename )
    , geom_xDim(0)
    , geom_yDim(0)
    , geom_zDim(0)
    , occNdpls(0)
    , Nmat(0)
    , gridsize(0)
    , _hasmesh(false)
    , _hasgridsize(false)
    , _hasmaterial(false)

{
    const char*fname = filename.c_str();

    cout << "AddaGeometry::AddaGeometry(std::string filename="<< filename<<")";
    int ncid;

    int     status=nc_open(fname,NC_NOWRITE|NC_NETCDF4,&ncid);

    if (    status!=NC_NOERR) {
        // printf("status=%d\n", status); // NC_ENOTNC == -51
        if (status==NC_ENOTNC) {
            //LogWarning(EC_WARN,ERR_LOC_CALL,"'%s' is HDF5 but not a netCDF4 file.",fname);
            //return status;
        }
    }
    int geom_ndims, geom_dimids[NC_MAX_VAR_DIMS], geom_natts;
    char dimensionName[NC_MAX_NAME];
    nc_type geom_type;

    int geom_id;
    if (nc_inq_varid(ncid,"geom",&geom_id)!=NC_NOERR) {
        // LogError(ONE_POS,"No scatterer definition (variable: 'geom') in file %s found",fname);
    }

    if (nc_inq_var(ncid, geom_id,0,&geom_type,&geom_ndims,geom_dimids,&geom_natts)!=NC_NOERR) {
        //   LogError(ONE_POS,"Something is wrong with file %s ",fname);
    }

    if (geom_ndims!=3) {
        // LogError(ONE_POS,"Wrong dimensions of geom variable, expected 3 got %d file %s ",geom_ndims,fname);
    }

    // class members now
    //size_t geom_xDim, geom_yDim, geom_zDim;
    if (nc_inq_dim(ncid,geom_dimids[0],dimensionName,&geom_xDim)!=NC_NOERR) {
        //  LogError(ONE_POS,"Error reading grid box X size from file %s ",fname);

    }
    if (nc_inq_dim(ncid,geom_dimids[1],dimensionName,&geom_yDim)!=NC_NOERR) {
        //LogError(ONE_POS,"Error reading grid box Y size from file %s ",fname);
    }
    if (nc_inq_dim(ncid,geom_dimids[2],dimensionName,&geom_zDim)!=NC_NOERR) {
        // LogError(ONE_POS,"Error reading grid box Z size from file %s ",fname);
    }

    // printf("box size: %zu, %zu, %zu\n", geom_xDim,geom_yDim,geom_zDim);

    //int Nmat;
    if (nc_get_att_int(ncid, geom_id,"Nmat",&Nmat)!=NC_NOERR) {
        // LogError(ONE_POS,"Error reading Nmat from file %s", fname);
    }

    //unsigned long long occNdpls;
    if (nc_get_att_ulonglong(ncid,geom_id,"N_occupied_dipols",&occNdpls)!=NC_NOERR) {

    }

    // ncatted -a gridsize_nm,geom,m,d,"53.2" sphere.nc
    if (nc_get_att_double(ncid,geom_id,"gridsize_nm",&gridsize)!=NC_NOERR) {
        _hasgridsize=false;
    }
    else
    {
        _hasgridsize=true;
    }

    // check for materials - read them if available
    // ncatted -a material_re,geom,a,d,"1.1,2.2,3.3"  sphere.
    nc_type xtype;
    size_t len_re;
    if (nc_inq_att(ncid,geom_id,"material_re",&xtype, &len_re)!=NC_NOERR) {
        _hasmaterial=false;
        return;
    }

    size_t len_im;
    if ( nc_inq_attlen(ncid,geom_id, "material_im", &len_im)!=NC_NOERR) {
        _hasmaterial=false;
        return;
    }

    if( len_im != len_re )
    {
        printf("Error not all real and imaginary parts of the materials found\n");
        _hasmaterial=false;
        return;
    }

    if( int(len_re) < Nmat)
    {
        printf("Not enough materials given, for the domains specified in the scatterer geometry.\n");
        _hasmaterial=false;
        return;
    }


    materiallist_re.resize(len_re);

    if (nc_get_att (ncid, geom_id, "material_re", materiallist_re.data() ) !=NC_NOERR) {
        _hasmaterial=false;
        return;
    }

    materiallist_im.resize(len_im);

    if (nc_get_att (ncid, geom_id, "material_im", materiallist_im.data() ) !=NC_NOERR) {
        _hasmaterial=false;
        return;
    }

    _hasmaterial=true;


    nc_close(ncid);
}


int AddaGeometry::getXdim()
{
    return geom_xDim;
}

int AddaGeometry::getYdim()
{
    return geom_yDim;
}

int AddaGeometry::getZdim()
{
    return geom_zDim;
}

int AddaGeometry::getNmat()
{
    return Nmat;
}

unsigned long long AddaGeometry::getOccNdpls()
{
    return occNdpls;
}

double AddaGeometry::getGridSize()
{
    return gridsize;
}

const QString AddaGeometry::getFileName()
{
    return QString::fromStdString(filename);
}

bool AddaGeometry::hasmaterials()
{
    return _hasmaterial;
}

bool AddaGeometry::hasmesh()
{
    return _hasmesh;
}

bool AddaGeometry::hasgridsize()
{
    return _hasgridsize;
}

std::vector<double> AddaGeometry::getMaterialList_re()
{
    return materiallist_re;
}

std::vector<double> AddaGeometry::getMaterialList_im()
{
    return materiallist_im;
}

double AddaGeometry::getFirstMaterial_re()
{
    if( !materiallist_re.empty())
    {
        return materiallist_re.front();
    }

    return 0;
}

double AddaGeometry::getFirstMaterial_im()
{
    if( !materiallist_im.empty())
    {
        return materiallist_im.front();
    }

    return 0;
}

size_t AddaGeometry::computeMemoryPerProcessor()
{
    // see section 6.6
    // for n_p == n_z

    size_t nx=geom_xDim;
    size_t ny=geom_yDim;
    size_t nz=geom_zDim;

    // needs the max dipole count of every z-slice.
    // estimate as cube from occupied dipoles:
    double a = pow(double(occNdpls),(1.0/3.0));

    //  size_t nslice = ny*nx; // needs the max dipole count of every z-slice.
    size_t nslice = floor(a*a);

    size_t Mpp = (288*nx + 384*nz + 192*nx*nz)*ny + (271.0/463.0)*nslice; // bytes

    return Mpp;
}

size_t AddaGeometry::computeMemoryTotal()
{
    size_t nx=geom_xDim;
    size_t ny=geom_yDim;
    size_t nz=geom_zDim;
    size_t Nreal = occNdpls;
    size_t N = nx*ny*nz;

    // current scheme is np==nz



    size_t np = nz;


            /*
             12.1, table 3
             <type>    n_vec
             bcgs2     8
             bicg      4
             bicgstab  7
             cgnr      4
             csym      6
             qmr       7
             qmr2      6
          */

    double n_vec = 4;
    double CoeffSolver = 31 + 48*(n_vec +1);   // 271

    size_t Mtot = (288 + 384*(double(np)/double(nx)) + 192.0/double(np) )*N + (271.0/463.0)*Nreal ;// bytes

    return Mtot;
}


