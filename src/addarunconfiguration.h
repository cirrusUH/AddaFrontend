#ifndef ADDARUNCONFIGURATION_H
#define ADDARUNCONFIGURATION_H

#include <QObject>
#include <QList>
#include <QVector>
#include <QStringList>

#include "addageometry.h"
#include "refractiveindices.h"

class Orientation
{
public:
    Orientation() :
        alpha_deg(0)
      , beta_deg(0)
      , gamma_deg(0)
    {}

    Orientation( double alpha_deg, double beta_deg, double gamma_deg)
        : alpha_deg(alpha_deg)
        , beta_deg(beta_deg)
        , gamma_deg(gamma_deg)
    {}

    double alpha_deg;
    double beta_deg;
    double gamma_deg;
};

class AddaRunConfiguration : public QObject
{
    Q_OBJECT

public:
    enum class ScriptType { CRAY_XC30, LINUX_BASH, PBS_GENERIC, WIN_POWERSHELL };

private:
    QString addaexecutablefile;

    ScriptType scripttype;

    //   QStringList geomfilelist;
    //   QList<bool> geomfilelisthasmaterial;

    QList<AddaGeometry> geometries;

    QString iter;
    QString additionalparameters;
    bool addstuckjobmonitor;
    int stuckjobExitAfterCompletedPercent;

    QString archerbudgetcode;
    QString QueueName;
    QString walltime;
    QString scriptfilename;


    int maxiter;
    QString scatgridfilename;

    int prepareVariables();
public:
    QVector<double> lambdalist;
    QVector<double> epslist;

    int numberOfTasks;

    enum class JobType {NOT_SET,ARRAY_JOB,FOR_LOOP};
    JobType jobtype;

    // Dipole per Lambda
    enum class DplSource {NOT_SET, COMPUTE_FROM_GRIDSIZE_AND_LAMBDA, USER_SET };
    DplSource dplsource;
    QVector<double> dpllist;
    int updateDpl();


    // materials
    enum class MaterialSources {NOT_SET, SINGLE_FROM_GEOM_FILE, LAMBDA_LINKED_SCHEME,USER_DEFINED_SINGLE,USER_DEFINED_LIST} ;
    MaterialSources material_source;
    QVector<double> material_relist;
    QVector<double> material_imlist;

    // Orientations
    enum class OrientationSources {NOT_SET, UH_SCHEME_I, USER_SINGLE};
    OrientationSources orientationsource;
    //QVector<Orientation> orientationlist;
    QVector<int> orientation_alphalist;
    QVector<int> orientation_betalist;
    QVector<int> orientation_gammalist;

    RefractiveIndices::Schemes material_scheme;

    // output
    QString outputdirectory;

public:
    explicit AddaRunConfiguration(QObject *parent = 0);
    ~AddaRunConfiguration();
    void setAddaExecutablePath(const QString &filename);
    void setLambda(double lambda);
    void setLambdas(QVector<double> lambdas);

    // Dipoles per Lambda
    int setDplComputeFromGeomFile();
    void setDplUser(double dpl);

    // Geometry File(s)
    void setGeometry(AddaGeometry &addageom);
    void addGeometry( const AddaGeometry addageom);
    void addGeometryFile(const QString&filename);
    void setGeometryFile(const QString&filename, bool hasmaterials);
    QString getFirstGeometryFileName();

    int getFirstGeometryNx();

    int getFirstGeometryNy();


    int getFirstGeometryNz();

    bool getFirstGeometryHasMaterials();
    bool getGeometriesEmpty();

    double getFirstGeometryGridSizeHint();


    unsigned int gcd(unsigned int u, unsigned int v);
    unsigned int getNumberOfOptimalCores();

    // Materials
    void useMaterialFromGeomFile();
    void setMaterialSource( MaterialSources m_src );
    void setMaterial(double m_re, double m_im );
    void updateMaterials();


    void setMaterialScheme( RefractiveIndices::Schemes scheme);
    void printWarren2008();

    // Orientations
    void setOrientation(double alpha , double beta, double gamma);
    //    void setOrientations( const QVector<Orientation> & orient );
    int setOrientationSource(OrientationSources src , int levelindex);

    // scattered Field
    void setScatGridFilename(const QString & file);

    // Script Type
    void writeScriptfile();
    void writeScriptfileArcher(const QString&filename);    
    void writeScriptfileLinuxBash(const QString&filename);
    void writeScriptfilePBSgeneric(const QString&filename);
   

    void setScriptType( ScriptType st);

    void setOutputDirectory(const QString&dir);

    // Solver
    void setIterativeSolver(const QString&iter);
    void setEpsList(const QVector<double> &epsInList);
    void setAdditionalParameters(const QString&params);
    void setMaxIter(int maxiter);

    void setScriptName(const QString&name);
    void egtScriptName(const QString&name);

    // PBS
    void setQueueName(const QString&s);
    void setJobtype(JobType type);
    void setJobtype(const QString &arg);

    // Archer
    void setStuckJobMonitorAdd(bool yesno);
    void setStuckJobMonitorPercent(int percentcompleted);
    void setArcherBudgetCode(const QString&bc);
    // Housekeeping

    QString printQList(const QVector<double> &qlist, QString name);

    QString Debug();

signals:
    void errmsg(const QString &msg);
    void logmsg(const QString &msg);
};

#endif // ADDARUNCONFIGURATION_H
