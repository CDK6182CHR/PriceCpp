#ifndef CRNET_H
#define CRNET_H
#include <QString>
#include <graph.h>

class CRNet : public Graph<QString>
{
    static const QString fileName;
    int currentSource,currentTarget;
    QStringList* currentPath;
    int currentSsspResult;
    CRNet();//private constructor
    static std::unique_ptr<CRNet> instance;
public:
    static CRNet* getInstance();
    int mileBetween(const QString& from,const QString& to,QStringList& path);
    int read();
    virtual void processSsspResult(int s,int pi[],double mile[])override;
};

#endif // CRNET_H
