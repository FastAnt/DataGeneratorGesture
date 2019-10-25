#include "imagecollector.h"

#include <mutex>
#include <chrono>
#include <set>

ImageCollector::ImageCollector(QObject *parent) : QObject(parent)
{
    int workerCount = 5;
    for(int i = 0 ; i < workerCount ; i++)
    {
        m_workersPool.push_back(new WorkerThread());
        //m_workersPool[i]->moveToThread();
    }
}

void ImageCollector::getFramme()
{
    if(m_cameraPtr.data())
    {
        m_result = m_cameraPtr->grabToImage();
        connect(m_result.data(), &QQuickItemGrabResult::ready,
                this, &ImageCollector::saveFramme);
    }

}
void ImageCollector::saveFramme()
{
    for(WorkerThread * worker : m_workersPool)
    {
        if(!worker->isRunning())
        {
            worker->m_sampleParametrs.m_gestureName = m_gesstureName;
            worker->m_sampleParametrs.m_AttemptId = m_attemptID;
            worker->m_sampleParametrs.m_FrammeId = m_frammeID;
            worker->m_FrammeImage  = m_result->image().convertToFormat(QImage::Format_ARGB32);
            worker->run();
        }
    }
}

void ImageCollector::setCamera(QQuickItem *ptr)
{
    m_cameraPtr.reset(ptr);
}

QString WorkerThread::DestinationFolder::getPath()
{
    return QString(  QString("Data") + "/" + m_gestureName+ "/" + m_AttemptId  );
}

QString WorkerThread::DestinationFolder::getFullPath()
{
    return QString( m_prefixPath + "/" + m_gestureName+ "/" + m_AttemptId + "/" + m_FrammeId );
}

void WorkerThread::DestinationFolder::createFullPath()
{

    if(!QDir(m_prefixPath).exists())
    {
        if(!QDir().mkdir(m_prefixPath))
        {
            qDebug() << "error on creation " << m_prefixPath;
            return;
        }
    }
    if(!QDir(m_prefixPath + QString("/")+ m_gestureName).exists())
    {
        if(QDir().mkdir((m_prefixPath + QString("/")+ m_gestureName)))
        {
            qDebug() << "error on creation " <<(m_prefixPath + QString("/")+ m_gestureName);
            return;
        }
    }
    if(!QDir(m_prefixPath + QString("/")+ m_gestureName+ QString("/")
             + m_AttemptId ).exists())
    {
        if(QDir().mkdir((m_prefixPath + QString("/")+ m_gestureName+ QString("/")
                         + m_AttemptId )))
        {
            qDebug() << "error on creation " <<(m_prefixPath + QString("/")+ m_gestureName+ QString("/")
                                                + m_AttemptId );
            return;
        }
    }
}
