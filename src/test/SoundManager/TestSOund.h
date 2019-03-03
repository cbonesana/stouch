#ifndef TESTSOUND_H
#define TESTSOUND_H

#include <QObject>

class TestSOund : public QObject
{
    Q_OBJECT
public:
    explicit TestSOund(QObject *parent = 0);
    
    void play(QString s);

signals:

    void sound(QString sound, int r);
    
public slots:
    
};

#endif // TESTSOUND_H
