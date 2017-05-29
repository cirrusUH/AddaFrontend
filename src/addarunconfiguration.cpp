
#include "addarunconfiguration.h"

#include <QDateTime>
#include <QFile>
#include <QTextStream>
#include <QDebug>
// #include <QFileDevice>

AddaRunConfiguration::AddaRunConfiguration(QObject *parent) :
    QObject(parent),
    material_source( MaterialSources::NOT_SET )
{

    addstuckjobmonitor=true;
    stuckjobExitAfterCompletedPercent=95;
    archerbudgetcode="";
    QueueName="standard";
    walltime="24:00:00";
    scriptfilename="";
    maxiter=-1;
    scatgridfilename="";
    dplsource=DplSource::NOT_SET;
    orientationsource=OrientationSources::NOT_SET;
    jobtype=JobType::ARRAY_JOB;
    iter="bcgs2";
}

AddaRunConfiguration::~AddaRunConfiguration()
{}

QString AddaRunConfiguration::Debug()
{
    QString tmp;
    /*
    if(geomfilelist.size() > 0 )
    {
        tmp += geomfilelist.at(0);
    }

    if( !geomfilelist.empty())
    {
    qDebug() << "geom files:\n " << geomfilelist.at(0);
    }

    if( !geometries.empty() )
    {
        const QString fname = geometries.first().getFileName();
        qDebug() << "Geometries:\n " << fname;
    }
*/
    qDebug() << "Materials re";
    if(!material_relist.empty())
        qDebug() << material_relist;

    qDebug() << "Materials im";
    if(!material_imlist.empty())
        qDebug() << material_imlist;

    tmp += "Outputdir=" + outputdirectory;

    qDebug() << tmp;

    emit logmsg(tmp);
    return tmp;


}

void AddaRunConfiguration::printWarren2008()
{
    Warren2008 w;

    QFile data("/tmp/output.dat");

    if (data.open(QFile::WriteOnly | QFile::Truncate)) {
        QTextStream out(&data);

        for(double l=1e7; l<2e9; l+=1e8)
        {
            std::complex<double> n = w.getRefractiveIndex(l,0);
            // qDebug() << l<<","<<n.real()<<","<<n.imag();
            // out  << l/1000<<","<<n.real()<<","<<n.imag() << "\n";
        }
    }
}

void AddaRunConfiguration::setOrientation(double alpha, double beta, double gamma)
{
    orientation_alphalist.clear();
    orientation_betalist.clear();
    orientation_gammalist.clear();

    orientation_alphalist.append(alpha);
    orientation_betalist.append(beta);
    orientation_gammalist.append(gamma);

    emit logmsg("Orientation set to: " + QString("(%1,%2,%3)").arg(alpha).arg(beta).arg(gamma) );
    orientationsource=OrientationSources::USER_SINGLE;
}

int AddaRunConfiguration::setOrientationSource(AddaRunConfiguration::OrientationSources src, int levelindex=0)
{
    orientationsource=src;

    if( src == OrientationSources::UH_SCHEME_I )
    {
        if( levelindex<0 || levelindex>2 )
        {
            emit errmsg("Wrong hierarchy level");
            return -1;
        }

        orientation_alphalist.clear();
        orientation_betalist.clear();
        orientation_gammalist.clear();

        if(levelindex==0)
        {

            orientation_alphalist = { 0 };
            orientation_betalist  = { 0, 27, 38, 48, 56, 63, 77, 83, 90 };
            orientation_gammalist = { 0 };


        }
        else if(levelindex==1)
        {
            orientation_alphalist = { 0 };
            orientation_betalist  = { 0, 27, 38, 48, 56, 63, 77, 83, 90 };
            orientation_gammalist = { 15, 30 };

        }
        else if(levelindex==2)
        {
            orientation_alphalist = { 0 };
            orientation_betalist  = { 0, 27, 38, 48, 56, 63, 77, 83, 90 };
            orientation_gammalist = { 0, 15, 30 };

        }

        else
        {
            emit errmsg("Wrong hierarchy level for UH Scheme I");
            orientation_alphalist.clear();
            orientation_betalist.clear();
            orientation_gammalist.clear();

            orientationsource = OrientationSources::NOT_SET;
            return -1;
        }


        int N = orientation_alphalist.size()*orientation_betalist.size()*orientation_gammalist.size();

        return N;
    }

    return -1;
}

void AddaRunConfiguration::setScatGridFilename(const QString &file)
{
    scatgridfilename=file;
}

void AddaRunConfiguration::setAddaExecutablePath(const QString&filename)
{
    addaexecutablefile=filename;
}

void AddaRunConfiguration::setLambda(double lambda)
{
    lambdalist.clear();
    lambdalist.append(lambda);

    QString msg2 = "lambda <qvector> set to: [ ";
    foreach( double l, lambdalist)
    {
        msg2 += QString::number(l) + ", ";
    }
    msg2+= " ]";

    emit logmsg( msg2 );
}

void AddaRunConfiguration::setLambdas(QVector<double> lambdas)
{
    lambdalist.clear();
    lambdalist=lambdas;

    QString msg2 = "lambda <qvector> set to: [ ";
    foreach( double l, lambdalist)
    {
        msg2 += QString::number(l) + ", ";
    }
    msg2+= " ]";

    emit logmsg( msg2 );
}

int AddaRunConfiguration::setDplComputeFromGeomFile()
{
    if(lambdalist.empty())
    {
        emit errmsg("No wavelenght(s) specified, unable to comput dipols per lambda values.");
        return -1;
    }

    if( geometries.empty())
    {
        emit errmsg("No geometry file selected.");
        return -1;
    }

    double gridsize = geometries.first().getGridSize();


    emit logmsg("Dipol per lambda values");
    emit logmsg("------------------------");
    emit logmsg("lambda(nm),dpl (#/lambda)");

    dpllist.clear();
    foreach( double l, lambdalist)
    {
        double dpl = l/gridsize;

        dpllist.append( dpl );

        QString msg = QString("%1, %2").arg(l).arg(dpl);
        if( dpl >= 10)
        {
            emit logmsg( msg);
        }
        else
        {
            emit errmsg( msg + " this is less than 10 per lambda!");
        }
    }

    dplsource=DplSource::COMPUTE_FROM_GRIDSIZE_AND_LAMBDA;
}

void AddaRunConfiguration::setDplUser(double dpl)
{
    dplsource = DplSource::USER_SET;
    dpllist.clear();
    dpllist.append(dpl);
}

void AddaRunConfiguration::setGeometry(AddaGeometry &addageom)
{
    geometries.clear();
    geometries.push_back( addageom );

    emit logmsg ("geometry file: " + addageom.getFileName() );
}

QString AddaRunConfiguration::getFirstGeometryFileName()
{
    if(!geometries.empty())
    {
        AddaGeometry geom = geometries.at(0);
        return  geom.getFileName();
    }

    return "";
}

/*
void AddaRunConfiguration::addGeometry(const AddaGeometry &addageom)
{
    geometries.push_back( addageom );
}


void AddaRunConfiguration::setGeometryFile(const QString&filename, bool hasmaterials=false)
{
    geomfilelist.push_back(filename);
    geomfilelisthasmaterial.push_back(hasmaterials);
}
*/

int AddaRunConfiguration::getFirstGeometryNx()
{
    if(!geometries.empty())
    {
        AddaGeometry geom = geometries.at(0);
        return geom.getXdim();
    }

    return -1;
}

int AddaRunConfiguration::getFirstGeometryNy()
{
    if(!geometries.empty())
    {
        AddaGeometry geom = geometries.at(0);
        return geom.getYdim();
    }

    return -1;
}

int AddaRunConfiguration::getFirstGeometryNz()
{
    if(!geometries.empty())
    {
        AddaGeometry geom = geometries.at(0);
        return geom.getZdim();
    }

    return -1;
}

void AddaRunConfiguration::setMaterialSource(AddaRunConfiguration::MaterialSources m_src)
{
    material_source = m_src;
}

void AddaRunConfiguration::setMaterial(double m_re, double m_im)
{
    material_relist.clear();
    material_imlist.clear();
    material_relist.append(m_re);
    material_imlist.append(m_im);

    emit logmsg( QString("Material set to: %1 +i %2").arg(m_re).arg(m_im)  );
}

void AddaRunConfiguration::updateMaterials()
{
    if( lambdalist.empty())
    {
        emit errmsg("No lambdas selected");
        return;
    }

    if( material_source == MaterialSources::LAMBDA_LINKED_SCHEME)
    {
        material_relist.clear();
        material_imlist.clear();

        if( material_scheme == RefractiveIndices::Schemes::WATER_ICE_WARREN_2008 )
        {
            Warren2008 warren2008;

            emit logmsg( QString("Material linked with wavelength using scheme:\n") );

            QString msg = "# Reference: Warren, S. G., and R. E. Brandt (2008), \n"
                    "# Optical constants of ice from the ultraviolet to the microwave: A revised compilation.\n"
                    "# J. Geophys. Res., 113, D14220, doi:10.1029/2007JD009744.\n"
                    "# available: http://www.atmos.washington.edu/ice_optical_constants/IOP_2008_ASCIItable.dat\n\n";

            emit logmsg(msg);


            emit logmsg("lambda(nm), n_re, n_im");
            emit logmsg("----------------------");

            foreach(double l, lambdalist)
            {
                std::complex<double> n = warren2008.getRefractiveIndex(l);
                material_relist.append( real(n) );
                material_imlist.append( imag(n) );

                emit logmsg(QString("%1,%2,%3").arg(l,6).arg(real(n),11).arg(imag(n),11) );
            }
        }
        else
        {
            emit errmsg("Unkown refindex(lambda) scheme.");
        }
    }
}

void AddaRunConfiguration::setMaterialScheme(RefractiveIndices::Schemes scheme)
{
    material_scheme=scheme;
    setMaterialSource(AddaRunConfiguration::MaterialSources::LAMBDA_LINKED_SCHEME);

    updateMaterials();
}

void AddaRunConfiguration::useMaterialFromGeomFile()
{
    if( !geometries.empty())
    {

        if( geometries.first().hasmaterials())
        {
            setMaterial( geometries.first().getFirstMaterial_re(), geometries.first().getFirstMaterial_im() );
        }
    }
    setMaterialSource(AddaRunConfiguration::MaterialSources::SINGLE_FROM_GEOM_FILE);
    updateMaterials();
}

bool AddaRunConfiguration::getFirstGeometryHasMaterials()
{
    if( !geometries.empty() )
    {
        return geometries.first().hasmaterials();
    }

    return false;
}

bool AddaRunConfiguration::getGeometriesEmpty()
{
    return geometries.empty();
}

double AddaRunConfiguration::getFirstGeometryGridSizeHint()
{
    if(!geometries.empty() )
    {
        if( geometries.first().hasgridsize() )
        {
            return geometries.first().getGridSize();
        }
    }

    return -1;
}

unsigned int AddaRunConfiguration::gcd(unsigned int u, unsigned int v)
{
    // https://en.wikipedia.org/wiki/Binary_GCD_algorithm
    int shift;

    /* GCD(0,v) == v; GCD(u,0) == u, GCD(0,0) == 0 */
    if (u == 0) return v;
    if (v == 0) return u;

    /* Let shift := lg K, where K is the greatest power of 2
            dividing both u and v. */
    for (shift = 0; ((u | v) & 1) == 0; ++shift) {
        u >>= 1;
        v >>= 1;
    }

    while ((u & 1) == 0)
        u >>= 1;

    /* From here on, u is always odd. */
    do {
        /* remove all factors of 2 in v -- they are not common */
        /*   note: v is not zero, so while will terminate */
        while ((v & 1) == 0)  /* Loop X */
            v >>= 1;

        /* Now u and v are both odd. Swap if necessary so u <= v,
              then set v = v - u (which is even). For bignums, the
              swapping is just pointer movement, and the subtraction
              can be done in-place. */
        if (u > v) {
            unsigned int t = v; v = u; u = t;}  // Swap u and v.
        v = v - u;                       // Here v >= u.
    } while (v != 0);

    /* restore common factors of 2 */
    return u << shift;
}

unsigned int AddaRunConfiguration::getNumberOfOptimalCores()
{
    // this is described in section 6.6 and 12.5 of the adda 1.3b4 manual

    /* 6.6 The partition of the computational grid is optimal if both n_z and 2n_x divides
           the number of processors n_p (at least approximately, §12.2)

           12.2  FFTW3 most effective on grid: 2^p 3^q 5^r 7^s 11^u 13^v with (u + v ≤ 1)
           But in parallel mode these dimensions along the x- and
           z-axes must also divide n p and 2n p respectively. Therefore the increase of the
           dimensions (and hence simulation time) may be substantial, and not possible at all
           if the number of processors ... has more than one divisor of
           either 11 or 13 (or at least one larger than 13) for FFTW3 . Therefore it is strongly
           recommended not to use such “weird” number of processors.
         */


    // unsigned int Nx = getFirstGeometryNx();
    // unsigned int Ny = getFirstGeometryNy();
    unsigned int Nz = getFirstGeometryNz();

    // unsigned int GCD = gcd( Nz, 2*Nx );

    // this need clarification.
    return Nz;
}


QString AddaRunConfiguration::printQList(const QVector<double> &qlist, QString name)
{
    QString tmp;
    QString NAME = "declare -a " + name.toUpper();

    tmp = NAME + "=( " ;

    for( int i=0; i<qlist.size(); i++ )
    {
        tmp += QString::number( qlist.at(i) ) + " ";
    }

    tmp += ");\n";
    //tmp += "";

    qDebug() << tmp ;

    return tmp;
}

int AddaRunConfiguration::prepareVariables()
{

    if( lambdalist.empty() )
    {
        emit errmsg("No wavelength(s) set.");
        return -1;
    }

    updateDpl();

    if(material_source == MaterialSources::LAMBDA_LINKED_SCHEME)
    {
        updateMaterials();
    }

    if( epslist.empty() )
    {
        emit errmsg("No residual errors set (eps).");
        return -1;
    }

    if( material_relist.empty() )
    {
        emit errmsg("No material set (real).");
        return -1;
    }

    if( material_imlist.empty() )
    {
        emit errmsg("No material set (imag).");
        return -1;
    }

    QString geomfile = getFirstGeometryFileName();
    if( geomfile.length() == 0 )
    {
        emit errmsg("No scatterer geometry file was selected.");
        return -1;
    }



    return 0;
}

int AddaRunConfiguration::updateDpl()
{
    if(dplsource==DplSource::COMPUTE_FROM_GRIDSIZE_AND_LAMBDA)
    {
        setDplComputeFromGeomFile();
    }
    else if( dplsource==DplSource::NOT_SET)
    {
        emit errmsg("Dpl not set");
        return -1;
    }
    else if( dplsource==DplSource::USER_SET)
    {

    }
    else
    {
        qDebug() << "Wrong dpl source, should never happen";
    }

    return 0;
}

void AddaRunConfiguration::writeScriptfile()
{

    if( scripttype == ScriptType::CRAY_XC30)
    {
        writeScriptfileArcher(outputdirectory+"/"+scriptfilename );
    }
    else if( scripttype == ScriptType::LINUX_BASH)
    {
        writeScriptfileLinuxBash(outputdirectory+"/"+scriptfilename );
    }
    else if( scripttype == ScriptType::PBS_GENERIC)
    {
        writeScriptfilePBSgeneric(outputdirectory+"/"+scriptfilename );
    }
    else
    {
        emit errmsg("Unkown script type");
    }
}

void AddaRunConfiguration::writeScriptfileLinuxBash(const QString&filename)
{
    QFile file(filename);
    if( !file.open(QIODevice::WriteOnly | QIODevice::Truncate) )
    {
        emit errmsg( filename + " " +file.errorString() );
        return;
    }

    // QFileDevice::Permissions  p = file.permissions();
    file.setPermissions(QFile::ReadGroup |QFile::WriteOwner | QFile::ReadOwner | QFile::ReadOther   | QFile::ExeOwner);

    QTextStream ts(&file);
    ts << "#!/bin/bash --login\n"
       << "# This is an machine generated script, generated ";

    QTime nowTime;
    QDate nowDate;
    ts << nowDate.currentDate().toString("yyyy-MM-dd_")
       << nowTime.currentTime().toString("hh") +"h"
       << nowTime.currentTime().toString("mm");
    ts << "\n";
    ts << printQList( lambdalist, "LAMBDA");
    ts << "# script ends\n";

    ts << "ADDABINARY=" << addaexecutablefile << endl;

    ts << "cd " << outputdirectory << endl;


    ts << "exec $ADDABINARY  -lambda  532 -iter " << iter << " -shape read "<< getFirstGeometryFileName()<< " > out-$$ 2> err-$$" << endl;

    
    emit logmsg("Written: " + filename );
    emit logmsg( "ScriptType::LINUX_BASH");
}

void AddaRunConfiguration::writeScriptfilePBSgeneric(const QString &filename)
{
    int ret = prepareVariables();
    if( ret <0 )
    {
        return;
    }

    if(filename.length() == 0)
    {
        return;
    }

    QFile file(filename);
    if( !file.open(QIODevice::WriteOnly | QIODevice::Truncate) )
    {
        emit errmsg( filename + " " +file.errorString() );
        return;
    }

    // takes care of unset values and the n_re(lmbd), n_im(lmbd) schemes.


    QTextStream ts(&file);
    ts << "#!/bin/bash --login\n"
       << "# This is a machine generated script,  ";

    QTime nowTime;
    QDate nowDate;
    ts << nowDate.currentDate().toString("yyyy-MM-dd_")
       << nowTime.currentTime().toString("hh") +"h"
       << nowTime.currentTime().toString("mm");
    ts << "\n";

    int Nalpha = orientation_alphalist.size();
    int Nbeta  = orientation_betalist.size();
    int Ngamma = orientation_gammalist.size();
    int Neps = epslist.size();
    int Nlambda = lambdalist.size();

    int Njobs = Neps * Nlambda * Nalpha * Nbeta * Ngamma;

    // flatten arrays
    QVector<double> epsvar;
    QVector<double> lambdavar;
    QVector<double> dplvar;
    QVector<double> orientationvar_alpha;
    QVector<double> orientationvar_beta;
    QVector<double> orientationvar_gamma;

    // currently only if it is a function of lambda, otherwise only one material
    QVector<double> materialvar_re;
    QVector<double> materialvar_im;

    // add a dummy value at index 0, this is needed for
    // the array job invocation, it is never used.
    epsvar.append(-1);
    lambdavar.append(-1);
    dplvar.append(-1);

    if( material_source==MaterialSources::LAMBDA_LINKED_SCHEME)
    {
        materialvar_re.append(-1);
        materialvar_im.append(-1);
    }

    orientationvar_alpha.append(-1);
    orientationvar_beta.append(-1);
    orientationvar_gamma.append(-1);

    for(int ei=0; ei<Neps; ei++) {
        for(int li=0; li<Nlambda; li++) {
            // dpl are not a dimensions, but need to be supplied for every lambda
            for(int ai=0; ai<Nalpha; ai++) {
                for(int bi=0; bi<Nbeta; bi++) {
                    for(int gi=0; gi<Ngamma; gi++) {
                        {
                            epsvar.append( epslist.at(ei) );

                            lambdavar.append( lambdalist.at(li) );

                            if( dplsource == DplSource::COMPUTE_FROM_GRIDSIZE_AND_LAMBDA)
                            {
                                dplvar.append( dpllist.at(li) );
                            }

                            if( material_source == MaterialSources::LAMBDA_LINKED_SCHEME)
                            {
                                materialvar_re.append( material_relist.at(li) );
                                materialvar_im.append( material_imlist.at(li) );
                            }
                            orientationvar_alpha.append( orientation_alphalist.at(ai) );
                            orientationvar_beta.append( orientation_betalist.at(bi) );
                            orientationvar_gamma.append( orientation_gammalist.at(gi) );
                        }
                    }}}}}


    // setup the requested cores
    unsigned int totalcores = Njobs * getNumberOfOptimalCores();

    // PE contains 24 cores
    unsigned int select_PE = floor( 0.5 + double(totalcores)/24.0 ) ;

    // config

    ts << "#PBS -q " << QueueName << endl;

    ts << "#PBS -l walltime=" << walltime <<endl;
    ts << "#PBS -r y "<<endl;
    ts << endl;

    ts << "export PBS_O_WORKDIR=$(readlink -f $PBS_O_WORKDIR)"<<endl;
    ts << endl;

    ts << "cd $PBS_O_WORKDIR"<<endl;
    ts << endl;

    ts << "export OMP_NUM_THREADS=1"<<endl;
    ts << "ADDABINARY=" << addaexecutablefile << endl;
    ts << "# "<< getNumberOfOptimalCores() <<" is the number of cores (which is the select var * 24 cores per node)"<<endl;
    ts << "ADDAEXEC=\"mpirun -n " << getNumberOfOptimalCores() << endl;
    ts << endl;

    ts << "# Variable definitions " << endl;
    ts << "# first entry is a dummy to shifts the array to index 1" << endl;

    ts << printQList( epsvar, "EEPS");
    ts << printQList( lambdavar, "LLAMBDA");
    ts << printQList( dplvar, "DDPL");

    ts << printQList( orientationvar_alpha, "AALPHA");
    ts << printQList( orientationvar_beta, "BBETA");
    ts << printQList( orientationvar_gamma, "GGAMMA");

    if( material_source == MaterialSources::LAMBDA_LINKED_SCHEME)
    {
        ts << printQList( materialvar_re, "MMREAL");
        ts << printQList( materialvar_im, "MMIMAG");
    }

    ts << endl;

    ts << "DIR=" << outputdirectory << endl;
    ts << "cd $DIR"<<endl;


    ts << endl;
    ts << "NJOBS=" << Njobs << endl;

    ts << "" << endl;
    ts << "for i in $(seq 1 $NJOBS); " << endl;
    ts << "do"<<endl;

    if(jobtype==JobType::ARRAY_JOB)
    {
        ts << "i=$PBS_ARRAY_INDEX" << endl;
        ts << "#for i in $(seq 1 $NJOBS); " << endl;
        ts << "#do"<<endl;
    }
    else if(jobtype == JobType::FOR_LOOP)
    {
        ts << "#i=$PBS_ARRAY_INDEX" << endl;
        ts << "for i in $(seq 1 $NJOBS); "<< endl;
        ts << "do"<<endl;
    }
    else
    {
        emit errmsg("Unkwon jobtype");
        return;
    }

    ts << "  ALPHA=\"${AALPHA[$i]}\"" << endl;
    ts << "   BETA=\"${BBETA[$i]}\""  << endl;
    ts << "  GAMMA=\"${GGAMMA[$i]}\"" << endl;
    ts << "    EPS=\"${EEPS[$i]}\""   << endl;
    ts << " LAMBDA=\"${LLAMBDA[$i]}\""<< endl;
    ts << "    DPL=\"${DDPL[$i]}\""   << endl;

    if( material_source == MaterialSources::LAMBDA_LINKED_SCHEME)
    {
        ts << "    MREAL=\"$MMREALVAR[$i]}\""<<endl;
        ts << "    MIMAG=\"$MMIMAGVAR[$i]}\""<<endl;
    }

    ts << endl;

    ts << "$ADDAEXEC $ADDABINARY ";

    if(dplsource==DplSource::USER_SET)
    {
        ts << " -dpl " << dpllist.at(0);
    }
    else if( dplsource==DplSource::COMPUTE_FROM_GRIDSIZE_AND_LAMBDA)
    {
        ts << " -dpl $DPL  ";
    }

    ts << " -lambda $LAMBDA ";  // -prop 0 0 1

    if( material_relist.size()>0 || material_imlist.size()>0 )
    {
        if( material_source == MaterialSources::LAMBDA_LINKED_SCHEME)
        {
            ts << "-m $MREAL $MIMAG ";
        }
        else // only one material for now
        {
            ts << "-m " << material_relist.at(0) << " " << material_imlist.at(0) ;
        }
    }

    QString geomfile = getFirstGeometryFileName();
    if( geomfile.length() == 0 )
    {
        emit errmsg("No scatterer geometry file was selected.");
        return;
    }

    ts  << " -shape read " << geomfile;

    if( material_source == MaterialSources::SINGLE_FROM_GEOM_FILE )
    {
        ts << " -m " << material_relist.at(0) << " " << material_imlist.at(0);
    }

    ts  << " -orient $ALPHA $BETA $GAMMA "
        << " -scat_grid_inp " << scatgridfilename << " -store_scat_grid -scat_matr both"
        << " -iter " << iter
        << " -eps $EPS"
        << " -maxiter " << maxiter
        << " > $DIR/out-$PBS_JOBID-$i &"<<endl;


    // --------------------------------------- /\ ---- for loop ----------

    if(jobtype==JobType::ARRAY_JOB)
    {
    }
    else if(jobtype == JobType::FOR_LOOP)
    {
         ts << "done"<<endl;
    }
    else
    {
        emit errmsg("Unkwon jobtype");
        return;
    }

    // wait for all bg jobs to complete
    ts << "wait" << endl;

    emit logmsg("Written: " + filename );
    emit logmsg("cd " + outputdirectory);
    emit logmsg("# qsub " + filename );
}


void AddaRunConfiguration::writeScriptfileArcher(const QString &filename)
{
    int ret = prepareVariables();
    if( ret <0 )
    {
        return;
    }

    if(filename.length() == 0)
    {
        return;
    }

    QFile file(filename);
    if( !file.open(QIODevice::WriteOnly | QIODevice::Truncate) )
    {
        emit errmsg( filename + " " +file.errorString() );
        return;
    }

    // takes care of unset values and the n_re(lmbd), n_im(lmbd) schemes.


    QTextStream ts(&file);
    ts << "#!/bin/bash --login\n"
       << "# This is a machine generated script,  ";

    QTime nowTime;
    QDate nowDate;
    ts << nowDate.currentDate().toString("yyyy-MM-dd_")
       << nowTime.currentTime().toString("hh") +"h"
       << nowTime.currentTime().toString("mm");
    ts << "\n";

    int Nalpha = orientation_alphalist.size();
    int Nbeta  = orientation_betalist.size();
    int Ngamma = orientation_gammalist.size();
    int Neps = epslist.size();
    int Nlambda = lambdalist.size();

    int Njobs = Neps * Nlambda * Nalpha * Nbeta * Ngamma;

    // flatten arrays
    QVector<double> epsvar;
    QVector<double> lambdavar;
    QVector<double> dplvar;
    QVector<double> orientationvar_alpha;
    QVector<double> orientationvar_beta;
    QVector<double> orientationvar_gamma;

    // currently only if it is a function of lambda, otherwise only one material
    QVector<double> materialvar_re;
    QVector<double> materialvar_im;

    // add a dummy value at index 0, this is needed for
    // the array job invocation, it is never used.
    epsvar.append(-1);
    lambdavar.append(-1);
    dplvar.append(-1);

    if( material_source==MaterialSources::LAMBDA_LINKED_SCHEME)
    {
        materialvar_re.append(-1);
        materialvar_im.append(-1);
    }

    orientationvar_alpha.append(-1);
    orientationvar_beta.append(-1);
    orientationvar_gamma.append(-1);

    for(int ei=0; ei<Neps; ei++) {
        for(int li=0; li<Nlambda; li++) {
            // dpl are not a dimensions, but need to be supplied for every lambda
            for(int ai=0; ai<Nalpha; ai++) {
                for(int bi=0; bi<Nbeta; bi++) {
                    for(int gi=0; gi<Ngamma; gi++) {
                        {
                            epsvar.append( epslist.at(ei) );

                            lambdavar.append( lambdalist.at(li) );

                            if( dplsource == DplSource::COMPUTE_FROM_GRIDSIZE_AND_LAMBDA)
                            {
                                dplvar.append( dpllist.at(li) );
                            }

                            if( material_source == MaterialSources::LAMBDA_LINKED_SCHEME)
                            {
                                materialvar_re.append( material_relist.at(li) );
                                materialvar_im.append( material_imlist.at(li) );
                            }
                            orientationvar_alpha.append( orientation_alphalist.at(ai) );
                            orientationvar_beta.append( orientation_betalist.at(bi) );
                            orientationvar_gamma.append( orientation_gammalist.at(gi) );
                        }
                    }}}}}


    // setup the requested cores
    unsigned int totalcores = Njobs * getNumberOfOptimalCores();

    // PE contains 24 cores
    unsigned int select_PE = floor( 0.5 + double(totalcores)/24.0 ) ;

    // config
    ts << "# Archer queues: long <48h, regular('standard') < 24h"<<endl;
    if( QueueName.compare("standard"))
    {
        ts << "#PBS -q " << QueueName << endl;
    }
    ts << "#PBS -l walltime=" << walltime <<endl;
    ts << "#PBS -A " << archerbudgetcode << endl;

    ts << "#PBS -l select="<< select_PE << endl;
    ts << "#PBS -r y "<<endl;

    ts << "# Archer standard queue"<<endl;
    ts << "# -J 1-10 for arrays, this has to be in the script"<<endl;
    ts << endl;
    ts << "export PBS_O_WORKDIR=$(readlink -f $PBS_O_WORKDIR)"<<endl;
    ts << "module load fftw"<<endl;
    ts << endl;

    ts << "cd $PBS_O_WORKDIR"<<endl;
    ts << endl;

    ts << "export OMP_NUM_THREADS=1"<<endl;
    ts << "ADDABINARY=" << addaexecutablefile << endl;
    ts << "# "<< getNumberOfOptimalCores() <<" is the number of cores (which is the select var * 24 cores per node)"<<endl;
    ts << "ADDAEXEC=\"aprun -n "<< getNumberOfOptimalCores() <<"\"  # select "<< select_PE <<endl;
    ts << endl;
    ts << endl;  

    ts << "# Variable definitions " << endl;
    ts << "# first entry is a dummy, to shift the array to index 1, never used." << endl;

    ts << printQList( epsvar, "EEPS");
    ts << printQList( lambdavar, "LLAMBDA");
    ts << printQList( dplvar, "DDPL");

    ts << printQList( orientationvar_alpha, "AALPHA");
    ts << printQList( orientationvar_beta, "BBETA");
    ts << printQList( orientationvar_gamma, "GGAMMA");

    if( material_source == MaterialSources::LAMBDA_LINKED_SCHEME)
    {
        ts << printQList( materialvar_re, "MMREAL");
        ts << printQList( materialvar_im, "MMIMAG");
    }

    ts << endl;

    ts << "DIR=" << outputdirectory << endl;
    ts << "cd $DIR"<<endl;


    ts << endl;
    ts << "NJOBS=" << Njobs << endl;

    ts << "" << endl;
    ts << "for i in $(seq 1 $NJOBS); " << endl;
    ts << "do"<<endl;

    if(jobtype==JobType::ARRAY_JOB)
    {
        ts << "i=$PBS_ARRAY_INDEX" << endl;
        ts << "#for i in $(seq 1 $NJOBS); " << endl;
        ts << "#do"<<endl;
    }
    else if(jobtype == JobType::FOR_LOOP)
    {
        ts << "#i=$PBS_ARRAY_INDEX" << endl;
        ts << "for i in $(seq 1 $NJOBS); "<< endl;
        ts << "do"<<endl;
    }
    else
    {
        emit errmsg("Unkwon jobtype");
        return;
    }

    ts << "  ALPHA=\"${AALPHA[$i]}\"" << endl;
    ts << "   BETA=\"${BBETA[$i]}\""  << endl;
    ts << "  GAMMA=\"${GGAMMA[$i]}\"" << endl;
    ts << "    EPS=\"${EEPS[$i]}\""   << endl;
    ts << " LAMBDA=\"${LLAMBDA[$i]}\""<< endl;
    ts << "    DPL=\"${DDPL[$i]}\""   << endl;

    if( material_source == MaterialSources::LAMBDA_LINKED_SCHEME)
    {
        ts << "    MREAL=\"$MMREALVAR[$i]}\""<<endl;
        ts << "    MIMAG=\"$MMIMAGVAR[$i]}\""<<endl;
    }

    ts << endl;

    ts << "$ADDAEXEC $ADDABINARY ";

    if(dplsource==DplSource::USER_SET)
    {
        ts << " -dpl " << dpllist.at(0);
    }
    else if( dplsource==DplSource::COMPUTE_FROM_GRIDSIZE_AND_LAMBDA)
    {
        ts << " -dpl $DPL  ";
    }

    ts << " -lambda $LAMBDA ";  // -prop 0 0 1

    if( material_relist.size()>0 || material_imlist.size()>0 )
    {
        if( material_source == MaterialSources::LAMBDA_LINKED_SCHEME)
        {
            ts << "-m $MREAL $MIMAG ";
        }
        else // only one material for now
        {
            ts << "-m " << material_relist.at(0) << " " << material_imlist.at(0) ;
        }
    }

    QString geomfile = getFirstGeometryFileName();
    if( geomfile.length() == 0 )
    {
        emit errmsg("No scatterer geometry file was selected.");
        return;
    }

    ts  << " -shape read " << geomfile;

    if( material_source == MaterialSources::SINGLE_FROM_GEOM_FILE )
    {
        ts << " -m " << material_relist.at(0) << " " << material_imlist.at(0);
    }

    ts  << " -orient $ALPHA $BETA $GAMMA "
        << " -scat_grid_inp " << scatgridfilename << " -store_scat_grid -scat_matr both"
        << " -iter " << iter
        << " -eps $EPS"
        << " -maxiter " << maxiter
        << " > $DIR/out-$PBS_JOBID-$i &"<<endl;


    // --------------------------------------- /\ ---- for loop ----------

    if(jobtype==JobType::ARRAY_JOB)
    {
    }
    else if(jobtype == JobType::FOR_LOOP)
    {
         ts << "done"<<endl;
    }
    else
    {
        emit errmsg("Unkwon jobtype");
        return;
    }

    // include monitor for stuck jobs
    if(addstuckjobmonitor)
    {
        ts << endl;
        ts << "# Safeguard against jobs that get stuck." << endl;
        ts << "# complete this percentage of jobs for sure"<<endl;
        ts << "COMPLETEJOBSPERCENT=" << stuckjobExitAfterCompletedPercent << endl;

        ts << "PARTJOBS=$(echo \"$NJOBS-$NJOBS*$COMPLETEJOBSPERCENT/100\" | bc)"<<endl;
        ts << "RUNTIMESECS=0"<<endl;
        ts << "echo \"Waiting until we are left with $PARTJOBS jobs running, which is $COMPLETEJOBSPERCENT % of $NJOBS jobs\" >> waitingout.txt"<<endl;
        ts << "while [ $(jobs | wc -l) -ge $PARTJOBS ] ; "<<endl;
        ts << "do"<<endl;
        ts << "   # echo $(jobs -p | wc -l)"<<endl;
        ts << "   sleep 1;"<<endl;
        ts << "   let RUNTIMESECS++"<<endl;
        ts << "done"<<endl;

        ts << "#echo \"Reached this after $RUNTIMESECS\" s"<<endl;
        ts << "EXTRAWAITPERCENT=10"<<endl;
        ts << "let EXTRATIME=$(echo \"$RUNTIMESECS*$EXTRAWAITPERCENT/100\" | bc)"<<endl;
        ts << "echo \"Wait an additional\" $EXTRAWAITPERCENT\"%, which is\" $EXTRATIME \"sec\" >> waitingout.txt"<<endl;
        ts << "# one job is the sleep command, this needs accounting for so \"-ge 1\" instead of 0"<<endl;
        ts << "while [ $EXTRATIME -ge 0 ] && [ $(jobs | wc -l) -ge 1 ]; "<<endl;
        ts << "do"<<endl;
        ts << "  #echo jobs still running $(jobs | wc -l)"<<endl;
        ts << "  #echo $EXTRATIME"<<endl;
        ts << "  sleep 1;"<<endl;
        ts << "  let EXTRATIME--"<<endl;
        ts << "done"<<endl;
        ts << "echo \"Terminated \"  $(jobs | wc -l) \" jobs\" >> waitingout.txt"<<endl;
    }
    else
    {
        // wait for all bg jobs to complete
        ts << "wait" << endl;
    }

    emit logmsg("Written: " + filename );
    emit logmsg("cd " + outputdirectory);
    emit logmsg("checkScript " + filename );
    emit logmsg("# qsub " + filename );
}

void AddaRunConfiguration::setScriptType(AddaRunConfiguration::ScriptType st)
{
    scripttype = st;
}

void AddaRunConfiguration::setOutputDirectory(const QString &dir)
{
    outputdirectory = dir;
}

void AddaRunConfiguration::setIterativeSolver(const QString &iter)
{
    this->iter=iter;
}

void AddaRunConfiguration::setEpsList(const QVector<double> &epsInList)
{
    QString msg="eps set to: [ ";

    epslist.clear();
    foreach(double e, epsInList)
    {
        epslist.append(e);
        msg += QString::number(e) +" ";
    }

    msg += " ]";

    emit logmsg(msg);
}


void AddaRunConfiguration::setAdditionalParameters(const QString &params)
{
    additionalparameters = params;
}

void AddaRunConfiguration::setMaxIter(int maxiter)
{
    this->maxiter=maxiter;
    emit logmsg (QString("maxiter set to: %1").arg(maxiter) );
}

void AddaRunConfiguration::setScriptName(const QString &name)
{
    scriptfilename = name;
}

void AddaRunConfiguration::setQueueName(const QString &s)
{
    QueueName=s;
}

void AddaRunConfiguration::setJobtype(JobType type)
{
    jobtype=type;
}


void AddaRunConfiguration::setJobtype(const QString &arg)
{
    if(arg=="JobType::ARRAY_JOB")
    {
        jobtype=JobType::ARRAY_JOB;
    }
    else if( arg=="JobType::FOR_LOOP")
    {
        jobtype=JobType::FOR_LOOP;
    }
    else
    {
        emit errmsg("Unkown jobtyoe "+ arg);
        jobtype=JobType::NOT_SET;

    }
}


void AddaRunConfiguration::setStuckJobMonitorAdd(bool yesno)
{
    addstuckjobmonitor = yesno;
}

void AddaRunConfiguration::setStuckJobMonitorPercent(int percentcompleted)
{
    stuckjobExitAfterCompletedPercent = percentcompleted;
}

void AddaRunConfiguration::setArcherBudgetCode(const QString &bc)
{
    archerbudgetcode=bc;
}
