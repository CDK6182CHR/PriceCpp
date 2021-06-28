#include "crnet.h"
#include <QFile>
#include <QDebug>
#include <QtCore>

using namespace std;

const QString CRNet::fileName(QString(":/rsc/adj.txt"));
CRNet*const CRNet::instance=new CRNet;

CRNet::CRNet():
    Graph<QString>(5000),currentSource(-1),currentTarget(-1),currentSsspResult(INF)
{
    read();
    qDebug()<<"CRNet vertex count"<<size<<endl;
}

CRNet *CRNet::getInstance()
{
    return instance;
}

int CRNet::mileBetween(const QString &from, const QString &to, QStringList &path)
{
    int v=getVertexPos(from),
        w=getVertexPos(to);
    if(v==-1||w==-1){
        qDebug()<<"whether source or target not existed in graph: "<<from<<" "<<to<<endl;
        return -1;
    }
    currentSource=v;
    currentTarget=w;
    currentPath=&path;
    sssp(from);
    return currentSsspResult;
}

int CRNet::read()
{
    QFile file(fileName);
    file.open(QFile::ReadOnly|QFile::Text);
    QTextStream in(&file);
    in.setCodec("UTF-8");
    if(!file.isOpen()){
        qDebug()<<"Fetal: Cannot open file "<<fileName<<endl;
        return 1;
    }
    while(!in.atEnd()){
        QString station1,station2,lineName;
        int mile;
        QString line=in.readLine();
        QStringList list=line.split(' ');
        if(list.length()!=4){
            qDebug()<<"Invalid mile file data: "<<line<<endl;
            continue;
        }
        lineName=list[0];
        station1=list[1];station2=list[2];
        mile=list[3].toInt();
        if(mile<=0){
            qDebug()<<"Invalid negative weight! "<<line<<endl;
            continue;
        }
        int v=findOrCreateVertex(station1),
            w=findOrCreateVertex(station2);
        insertEdge(v,w,mile,lineName);
        insertEdge(w,v,mile,lineName);
    }
    file.close();
    return 0;
}

void CRNet::processSsspResult(int s,int pi[], double mile[])
{
    qDebug()<<"prosssssr"<<endl;
    int w=currentTarget;
    currentSsspResult=mile[w];
    if(currentSsspResult==INF)
        return;
    stack<int> via;
    via.push(w);//终点先压栈
    int v = pi[w];
    while (v!=-1) {
//        if (vertexTable[v]->outDegree > 2)
        via.push(v);
        v = pi[v];
//        qDebug()<<v<<endl;
    }
    //先处理第一个区间
    via.pop();//起点先扔了
    int v1=via.top();
    via.pop();
    PEdge firstEdge=shortestEdge(s,v1);//为什么会null?? 202-204
    QString cLineName=firstEdge->data;    //当前所处线路
    QString cStart=getValue(s);           //当前所处线路的进入站名称
    double cMile=firstEdge->cost;         //当前所处线路的已经经过里程
    int lastIdx=v1;
    while (!via.empty()) {
//        currentPath->append(via.top());
        int v=via.top();
        PVertex pv=vertexTable[v];
        via.pop();
        PEdge pe=shortestEdge(lastIdx,v);
        const QString& intLineName=pe->data;
        if(intLineName!=cLineName){
            //线路变更
            QString s=cLineName+" "+cStart+"-"+
                    vertexTable[lastIdx]->data+" "+
                    QString::number(cMile)+" km";
            currentPath->append(s);
            cMile=pe->cost;
            cLineName=intLineName;
            cStart=vertexTable[lastIdx]->data;
        }
        else{
            cMile+=pe->cost;
        }
        lastIdx=v;
    }
    //最后一个区间
    PVertex pv=vertexTable[w];
    QString s0=cLineName+" "+cStart+"-"+pv->data+" "+
            QString::number(cMile)+" km";
    currentPath->append(s0);
    qDebug()<<"psssss ok"<<endl;
}
