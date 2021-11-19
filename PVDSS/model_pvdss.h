#ifndef MODEL_PVDSS_H
#define MODEL_PVDSS_H


class model_pvdss : public QObject
{
    Q_OBJECT
public:
    explicit model_pvdss(QObject *parent = 0);

signals:



public slots:
    void RashT() ;
};

#endif // MODEL_PVDSS_H
