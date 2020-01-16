#ifndef CaPTkTrainingModule_h
#define CaPTkTrainingModule_h

// The following header file is generated by CMake and thus it's located in
// the build directory. It provides an export macro for classes and functions
// that you want to be part of the public interface of your module.
#include <MitkCaPTkTrainingModuleExports.h>

#include <QObject>
#include <QFuture>
#include <QFutureWatcher>
#include <QProgressBar>

/** \class CaPTkTrainingModule
 *  \brief Training Module API
 */
class MITKCAPTKTRAININGMODULE_EXPORT CaPTkTrainingModule : public QObject
{
    Q_OBJECT

public:
    CaPTkTrainingModule(QObject *parent = 0);

    ~CaPTkTrainingModule() {}

    /** \brief Runs the algorithm
     * 
     * Execute the algorithm in a background thread. When the
     * algorithm finishes, OnAlgorithmFinished() is called.
     * 
     * @param QString featuresCsvPath path to the features csv file
     * @param QString responsesCsvPath path to the responses csv file
     * @param QString classificationKernelStr 
     * @param QString configurationStr ("Cross-validation", "Split Train/Test", "Split Train" or "Split Test")
     * @param QString folds number of folds (for configurationStr=="Cross-validation")
     * @param QString samples number of samples (for configurationStr=="Split Train/Test")
     * @param QString modelDirPath path to the model directory (for configurationStr=="Split Test") 
     * @param QString outputDirPath 
    */
    void Run(
        QString featuresCsvPath,
        QString responsesCsvPath,
        QString classificationKernelStr,
        QString configurationStr,
        QString folds,
        QString samples,
        QString modelDirPath,
        QString outputDirPath
    );

    void SetProgressBar(QProgressBar* progressBar);

    /** \struct Result
     *  \brief result of the execution of the algorithm
     * 
     * if ok == true, then everything went fine, 
     * else errorMessage is populated.
    */
    typedef struct Result 
    {
        bool ok = true;
        std::string errorMessage = "";
    } Result;

public slots:
    /** \brief This function runs in the main thread when 
     * the algorithm is finished
    */
    void OnAlgorithmFinished();

protected:

    /** \brief Runs the algorithm after the operations in Run
     * 
     * This can serve as a background thread. When the
     * algorithm finishes, OnAlgorithmFinished() is called.
     * The parameters are the same as Run()
     * 
     * @return the result struct (that contains the output or an errorMessage)
    */
    Result RunThread(
        QString& featuresCsvPath,
        QString& responsesCsvPath,
        QString& classificationKernelStr,
        QString& configurationStr,
        QString& folds,
        QString& samples,
        QString& modelDirPath,
        QString& outputDirPath
    );

    bool m_IsRunning = false;
    QFutureWatcher<Result> m_Watcher;
    QFuture<Result> m_FutureResult;

    QProgressBar* m_ProgressBar;
};

#endif // ! CaPTkTrainingModule_h
