#ifndef IMAGECOLLECTOR_H
#define IMAGECOLLECTOR_H

#include <QObject>
#include <QThread>
#include <QQuickItem>
#include <QSharedPointer>
#include <QQuickItemGrabResult>
#include <set>
#include <QDir>

#include <thread>
#include <map>



class WorkerThread : public QThread
{
    Q_OBJECT
public:
    void run() override {
        m_sampleParametrs.createFullPath();
        QString PATH = m_sampleParametrs.getFullPath();
        if(m_FrammeImage.save(PATH, "jpg"))
        {
            qDebug() << "success save framme " <<  m_sampleParametrs.m_FrammeId;
        }
        else
        {
            qDebug() << "error at saving ";
        }
    }
signals:
    void resultReady(const QString &s);
public:
    struct DestinationFolder
    {
        QString                                 m_gestureName;
        QString                                 m_AttemptId;
        QString                                 m_FrammeId;

        QString                                 m_fullPath;
        QString                                 m_prefixPath = "./Data";

        QString                                 getPath();
        QString                                 getFullPath();

        void                                    createFullPath();
    } m_sampleParametrs;
    QImage                                     m_FrammeImage;
};

class ImageCollector : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString gesstureName READ gesstureName WRITE setGesstureName NOTIFY gesstureNameChanged)
    Q_PROPERTY(QString attemptID READ attemptID WRITE setAttemptID NOTIFY attemptIDChanged)
    Q_PROPERTY(QString frammeID READ frammeID WRITE setFrammeID NOTIFY frammeIDChanged)
public:
    explicit ImageCollector(QObject *parent = nullptr);

    Q_INVOKABLE void getFramme();
    Q_INVOKABLE void setCamera( QQuickItem * ptr);

    QString gesstureName() const
    {
        return m_gesstureName;
    }

    QString attemptID() const
    {
        return m_attemptID;
    }

    QString frammeID() const
    {
        return m_frammeID;
    }

signals:
    void gesstureNameChanged(QString gesstureName);

    void attemptIDChanged(QString attemptID);

    void frammeIDChanged(QString frammeID);

public slots:
    Q_INVOKABLE void saveFramme();
    void setGesstureName(QString gesstureName)
    {
        if (m_gesstureName == gesstureName)
            return;

        m_gesstureName = gesstureName;
        emit gesstureNameChanged(m_gesstureName);
    }

    void setAttemptID(QString attemptID)
    {
        if (m_attemptID == attemptID)
            return;

        m_attemptID = attemptID;
        emit attemptIDChanged(m_attemptID);
    }

    void setFrammeID(QString frammeID)
    {
        if (m_frammeID == frammeID)
            return;

        m_frammeID = frammeID;
        emit frammeIDChanged(m_frammeID);
    }

private :

    QImage                                  m_FrammeImage;

    QSharedPointer<QQuickItem>              m_cameraPtr;
    QSharedPointer<QQuickItemGrabResult>    m_result;
    QVector<WorkerThread* >                 m_workersPool;
    QString m_gesstureName;
    QString m_attemptID;
    QString m_frammeID;
};

#endif // IMAGECOLLECTOR_H
