#include "widget.h"
#include "ui_widget.h"
#include <math.h>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}

 static float Kus, CD, Smind, Smaxd, U0max,U0min, I0max,I0min;
 static float dac_u_max, dac_u_min, s_max , s_min,cur_max , cur_min, kd_min, kd_max;
 static int IndexDac;

float UrovnK( float X1, float X2, float Y1, float Y2) //Решение системы уровнений
{
    float K1 ;
    K1=(Y2-Y1)/(X2-X1);

return K1;
}
float UrovnB(float X1, float X2, float Y1, float Y2) //Решение системы уравнений
{
     float K1, B1 ;
    K1=(Y2-Y1)/(X2-X1);
    B1=Y1-K1*X1;
return B1;
}
double volt_s (double s)
{
double s1, d1, volt;
s1 = s - s_min;
d1 = (dac_u_max - dac_u_min) / (s_max - s_min);
volt = dac_u_min + d1*s1;
return (volt);
}

double current_s(double s)
{
  double s1, d1, current;
s1 = s - s_min;
d1 = (cur_max - cur_min) / (s_max - s_min);
current = cur_min + d1*s1;
return(current)  ;
}


float RasKd ( float s)
{
float kd;
    if (s <= 1.4)                                       kd = 0.463;
                else if ((s > 1.4)  && (s <= 4.0))  kd = 0.464;
                else if ((s > 4.0)  && (s <= 7.0))  kd = 0.465;
                else if ((s > 7.0)  && (s <= 13.0)) kd = 0.466;

              else if ((s > 13.0) && (s <= 18.0)) kd = 0.467;
              else if ((s > 18.0) && (s <= 30.0)) kd = 0.468;
              else if ((s > 30.0) && (s <= 38.0)) kd = 0.469;
             else if ((s > 38.0) && (s <= 48.0)) kd = 0.470;
            else if ((s > 48.0) && (s <= 55.0)) kd = 0.471;
              else if ((s > 55.0) && (s <= 65.0)) kd = 0.472;
                else if ((s > 65.0) && (s <= 75.0)) kd = 0.473;
                else if ((s > 75.0) && (s <= 85.0)) kd = 0.474;
                  else if ((s > 85.0) && (s <=100.0)) kd = 0.475;
                  else if ((s >100.0) && (s <=120.0)) kd = 0.476;
                  else if ((s >120.0) && (s <=140.0)) kd = 0.477;
                  else if ((s >140.0) && (s <=160.0)) kd = 0.478;
                  else if ((s >160.0) && (s <=180.0)) kd = 0.479;
                  else if ((s >180.0) && (s <=200.0)) kd = 0.480;
                  else if ((s >200.0) && (s <=230.0)) kd = 0.481;
                  else if ((s >230.0) && (s <=270.0)) kd = 0.482;
                  else if ((s >270.0) && (s <=300.0)) kd = 0.483;
                  else if ((s >300.0) && (s <=330.0)) kd = 0.484;
                  else if ((s >330.0) && (s <=360.0)) kd = 0.485;
                  else         kd = 0.486;
    return kd;
}

float Raskt ( float tmpr)
{
    float  kt;
  kt = tmpr * (0.0000424*tmpr + 0.01788) + 0.5365;
return kt;
}

float Rasx2_t ( float tmpr)
{
    float  x2_t;
  x2_t = 0.01112*(1+tmpr*(0.09965+tmpr*(0.0004777+tmpr*(0.00008223-0.0000002878*tmpr))));
return x2_t;
}

float XfS(float S,float tmpr) //Вычисление удельной проводимости из солесодержания
{
float Kt, Kd, x2_t,X;
Kt=Raskt(tmpr);
Kd=RasKd(S);
x2_t=Rasx2_t(tmpr);
X=(S*Kt)/Kd+x2_t;

    return X;
}

float SfX(float X,float tmpr)// вычисляем соль из удельной проводимости как в МК

{
    float Kt, Kd, x2_t,S, Xmin, Xmax;
    Kt=Raskt(tmpr);
    Xmin=XfS(Smind, tmpr);
    Xmax=XfS(Smaxd, tmpr);
    Kd =X*(kd_max- kd_min)/(Xmax-Xmin);
    x2_t=Rasx2_t(tmpr);

  S=(Kd*X-Kd*x2_t)/Kt;
    Kd=RasKd(S);


    S=(Kd*X-Kd*x2_t)/Kt;
        return S;
}

float SfXd26(double X,double tmpr)// вычисляем соль из удельной проводимости как в D26
{
    double S, Xtchv , Kt, Kd;
  Xtchv=0.01112*(1+((9.965/100*tmpr)+(4.777/10000*tmpr*tmpr))+(8.223*tmpr*tmpr*tmpr/100000)-(2.878*tmpr*tmpr*tmpr*tmpr/10000000)) ;
  Kt=1+(0.02*(tmpr-25))+(0.0000424*pow((tmpr-25),2)) ;
  S=(double)SfX((float) X,(float)tmpr);
  Kd=RasKd(S);
  S=(Kd*X-Kd*Xtchv)/Kt;
 return (float) S ;
}

void Widget::on_pushButton_clicked()
{
   // static double count;
    float K, B, Tobr25, Tobr70, Tsol25, Tsol70, Iobr25, Iobr70,Ktsol, Btsol, X1t, X2t, Y1t, Y2t, Kt, Bt, Ktmp, Btmp ;
     //  count=ui->Bt0->text().toDouble();
    //count++;
      //  ui->Bt->setText(QString::number(count));


        K= ui->Kt0->text().toFloat();
        B= ui->Bt0->text().toFloat();

        Tobr25=ui->T0low->text().toFloat();
        Tobr70=ui->T0top->text().toFloat();
        Tsol25=ui->Tslow->text().toFloat();
        Tsol70=ui->Tstop->text().toFloat();

        Iobr25=Tobr25+273 ;
        Iobr70=Tobr70+273 ;
        Ktsol=(Tsol70-Tsol25)/(Iobr70-Iobr25) ;
        Btsol=Tsol25-Ktsol*Iobr25 ;


        X1t=(Ktsol*Iobr25+Btsol-25)/10.012 ;
        X2t=(Ktsol*Iobr70+Btsol-25)/10.012 ;
        Y1t=0.09988*Tobr25-2.497 ;
        Y2t=0.09988*Tobr70-2.497 ;

        Kt=UrovnK(X1t,X2t,Y1t,Y2t );
        Bt=UrovnB(X1t,X2t,Y1t,Y2t );

        Ktmp=Kt*K;
        Btmp=Bt+B;
        Ktmp=round(Ktmp*1000)/1000;
        Btmp=round(Btmp*1000)/1000;
        ui->Bt->setText(QString::number(Btmp));
        ui->Kt->setText(QString::number(Ktmp));

}

void Widget::on_Diapazon_activated(int index)
{

    switch(index)
    {
    case 0 :
            Kus=9.260;
            CD=1;
            Smind=0.1;
            Smaxd=1;
            dac_u_min = 0.0;
            dac_u_max = 10.0;
            cur_max=20;
            cur_min=4;
            s_min = 0.0;
            s_max = 0.9993;
            kd_min = 0.463;
            kd_max = 0.463;
        break;
            case 1 :
            Kus=7.378;
            CD=1;
            Smind=0.2;
            Smaxd=2;
            dac_u_min = 0.0;
            dac_u_max = 10.0;
            cur_max=20;
            cur_min=4;
            s_min = 0.0;
            s_max = 1.9993;
            kd_min = 0.463;
            kd_max = 0.464;

        break;
            case 2 :
            Kus=3.805;
            CD=1;
            Smind=0.4;
            Smaxd=4;
            dac_u_min = 0.0;
            dac_u_max = 10.0;
            cur_max=20;
            cur_min=4;
            s_min = 0.0;
            s_max = 3.9988;
            kd_min = 0.463;
            kd_max = 0.464;


        break;
            case 3 :
            Kus=9.260;
            CD=10;
            Smind=1;
            Smaxd=10;
            dac_u_min = 0.0;
            dac_u_max = 10.0;
            cur_max=20;
            cur_min=4;
            s_min = 0.0;
            s_max = 9.9932;
            kd_min = 0.463;//0.464;//13.08.2013
            kd_max = 0.466;


        break;
            case 4 :
            Kus=7.378;
            CD=10;
            Smind=2;
            Smaxd=20;
            dac_u_min = 0.0;
            dac_u_max = 10.0;
            cur_max=20;
            cur_min=4;
            s_min = 0.0;
            s_max = 19.9966;
            kd_min = 0.464;//0.465;//13.08.2013
            kd_max = 0.468;//0.467;//13.08.2013

        break;
        case 5 :
            Kus=3.805;
            CD=10;
            Smind=4;
            Smaxd=40;
            dac_u_min = 0.0;
            dac_u_max = 10.0;
            cur_max=20;
            cur_min=4;
            s_min = 0.0;
            s_max = 39.9751;
            kd_min = 0.464;//0.466;//13.08.2013
            kd_max = 0.470;//0.469;//13.08.2013


        break;
        case 6 :
            Kus=9.260;
            CD=100;
            Smind=10;
            Smaxd=100;
            dac_u_min = 0.0;
            dac_u_max = 10.0;
            cur_max=20;
            cur_min=4;
            s_min = 0.0;
            s_max = 99.9622;
            kd_min = 0.467;//0.468;//13.08.2013
            kd_max = 0.475;//0.474;//13.08.2013



        break;
        case 7 :
            Kus=3.805;
            CD=100;
            Smind=40;
            Smaxd=400;
            dac_u_min = 0.0;
            dac_u_max = 10.0;
            cur_max=20;
            cur_min=4;
            s_min = 0.0;
             s_max = 399.751;
             kd_min = 0.472;//0.474;//13.08.2013
             kd_max = 0.487;//0.485;//13.08.2013
            break;

    }
    ui->S0minSux->setText(QString::number(Smind));
    ui->S0maxSux->setText(QString::number(Smaxd));
}

void Widget::on_RunrasX_clicked()
{
    float Ks0, Bs0, Xe1, Xe2, Xs1, Xs2, Ks, Bs;

    Ks0= ui->Kx0->text().toFloat();
    Bs0= ui->Bx0->text().toFloat();
    Xe1=ui->X0min->text().toFloat();
    Xe2=ui->X0max->text().toFloat();
    Xs1=ui->Xsmin->text().toFloat();
    Xs2=ui->Xsmax->text().toFloat();

    Xe1=100/CD*Xe1;
    Xe2=100/CD*Xe2;
    Xs1=100/CD*Xs1;
    Xs2=100/CD*Xs2;

     Bs=(Xs1*Kus*Xe2-1000*Bs0*Xe2-Xs2*Kus*Xe1+1000*Bs0*Xe1)/(1000*(Xs1-Xs2)) ;
    Ks=(Ks0*(Kus*Xe1-1000*Bs))/(Kus*Xs1-1000*Bs0) ;
    Ks=round(Ks*1000)/1000;
    Bs=round(Bs*1000)/1000;
    ui->Bx->setText(QString::number(Bs));
    ui->Kx->setText(QString::number(Ks));
}

void Widget::on_RunrasS_clicked()
{
    float K0, B0, Smax, Smin, Ssmin, Ssmax, S0min, S0max,  Ks, Bs;

    K0= ui->Ks0->text().toFloat();
    B0= ui->Bs0->text().toFloat();
    S0min=ui->S0min->text().toFloat();
    S0max=ui->S0max->text().toFloat();
    Ssmin=ui->Ssmin->text().toFloat();
    Ssmax=ui->Ssmax->text().toFloat();

    Smax=-(B0-Ssmax)/K0;
    Smin=-(B0-Ssmin)/K0;

    Ks=(S0min-S0max)/(Smin-Smax);
    Bs=(S0max*Smin-S0min*Smax)/(Smin-Smax);
    Ks=round(Ks*1000)/1000;
    Bs=round(Bs*1000)/1000;
    ui->Bs->setText(QString::number(Bs));
    ui->Ks->setText(QString::number(Ks));
}

void Widget::on_RunrasX_2_clicked()
{
      float Ks0, Bs0,  Smin, Smax, S0min, S0max,  Ks, Bs, Tmin, Tmax, Xe1, Xe2, Xs1, Xs2,Xe1f, Xe2f, Xs1f, Xs2f;
    // Расчет второго канала на сухую
    Ks0= ui->Ks0Sux->text().toFloat();
    Bs0= ui->Bs0Sux->text().toFloat();
   Smin= ui->SsminSux->text().toFloat();
    Smax=ui->SsmaxSux->text().toFloat();
    S0min= ui->S0minSux->text().toFloat();
     S0max=ui->S0maxSux->text().toFloat();

     Tmin=ui->TsminSux->text().toFloat();
       Tmax=ui->TsminSux->text().toFloat();
    Xs1f= XfS(Smin,Tmin);
    Xs2f= XfS(Smax,Tmax) ;
    Xe1f= XfS(S0min,Tmin);
    Xe2f= XfS(S0max,Tmax);

    Xe1=100/CD*Xe1f;
    Xe2=100/CD*Xe2f;
    Xs1=100/CD*Xs1f;
    Xs2=100/CD*Xs2f;

     Bs=(Xs1*Kus*Xe2-1000*Bs0*Xe2-Xs2*Kus*Xe1+1000*Bs0*Xe1)/(1000*(Xs1-Xs2)) ;
    Ks=(Ks0*(Kus*Xe1-1000*Bs))/(Kus*Xs1-1000*Bs0) ;
    Ks=round(Ks*1000)/1000;
    Bs=round(Bs*1000)/1000;
    ui->BsSux->setText(QString::number(Bs));
    ui->KsSux->setText(QString::number(Ks));

}

void Widget::on_RunTsux_clicked()
{
  float   K, B, Tobr25, Tobr70, Tsol25, Tsol70, Btsol, Ktsol, Kt, Bt, X1t, X2t, Y1t, Y2t, Ktmp, Btmp, Iobr25, Iobr70;
    K= ui->Kt0sux->text().toFloat();
    B= ui->Bt0sux->text().toFloat();

    Tobr25=ui->I0low->text().toFloat()-273;
    Tobr70=ui->I0top->text().toFloat()-273;
    Tsol25=ui->TslowSux->text().toFloat();
    Tsol70=ui->TstopSux->text().toFloat();
    //дальше как на мокрую
    Iobr25=Tobr25+273 ;
    Iobr70=Tobr70+273 ;
    Ktsol=(Tsol70-Tsol25)/(Iobr70-Iobr25) ;
    Btsol=Tsol25-Ktsol*Iobr25 ;


    X1t=(Ktsol*Iobr25+Btsol-25)/10.012 ;
    X2t=(Ktsol*Iobr70+Btsol-25)/10.012 ;
    Y1t=0.09988*Tobr25-2.497 ;
    Y2t=0.09988*Tobr70-2.497 ;

    Kt=UrovnK(X1t,X2t,Y1t,Y2t );
    Bt=UrovnB(X1t,X2t,Y1t,Y2t );

    Ktmp=Kt*K;
    Btmp=Bt+B;
    Ktmp=round(Ktmp*1000)/1000;
    Btmp=round(Btmp*1000)/1000;
    ui->Btsux->setText(QString::number(Btmp));
    ui->Ktsux->setText(QString::number(Ktmp));

}

void Widget::on_RejDAC_currentIndexChanged(int index)
{
    switch(index)
    {
    case 0 :

        break;
            case 1 :
    IndexDac=1;
         U0max=10 ;
          U0min=1;
          ui->UImin->setText(QString::number(U0min));
          ui->UImax->setText(QString::number(U0max));
          ui->remin->setText("U min,V");
           ui->remax->setText("U max,V");
        break;
            case 2 :
        IndexDac=2;
        I0max=20 ;
        I0min=5.6;
        ui->UImin->setText(QString::number(I0min));
        ui->UImax->setText(QString::number(I0max));
        ui->remin->setText("I min,mA");
         ui->remax->setText("I max,mA");
         //ui-> reVich->setTitle("Выходной ток,mA");
         break;
}
}

void Widget::on_RasDAC_clicked()
{
    double  Bdac, Kdac, Usmin, Usmax,Ismin, Ismax, B0, K0, volt_S0max, volt_S0min, volt_Ssmax, volt_Ssmin,Ssmin, Ssmax;
 double current_S0max, current_S0min, current_Ssmax,  current_Ssmin;
    K0= ui->Kt0Dac->text().toDouble();
    B0= ui->Bt0Dac->text().toDouble();

    Ssmin= ui->SminDac->text().toDouble();
    Ssmax= ui->SmaxDac->text().toDouble();
    switch (IndexDac) {
    case 1:
        // U

        Usmin= ui->UImin->text().toDouble();
        Usmax= ui->UImax->text().toDouble();
        U0max=10;
        U0min=1;

        volt_S0max= volt_s (double (Smaxd));
        volt_S0min= volt_s (double (Smind));
        volt_Ssmax= volt_s (Ssmax);
        volt_Ssmin=volt_s (Ssmin);

     Bdac=-(2*U0min*Usmax*B0*volt_S0max-2*U0max*Usmin*B0*volt_S0min-4*U0max*Usmin*volt_S0min*volt_Ssmax+4*U0min*Usmax*volt_S0max*volt_Ssmin)/(2*U0min*Usmax*volt_Ssmin-2*U0max*Usmin*volt_Ssmax+(U0min*Usmax-U0max*Usmin)*B0) ;
     Kdac= ((U0min*Usmax-U0max*Usmin)*B0+(2097152*U0min*Usmax-2097152*U0max*Usmin)*B0*K0+2*Usmax*Usmin*volt_S0max-2*Usmax*Usmin*volt_S0min+(-4194304*U0max*Usmin*K0-2*U0max*Usmin)*volt_Ssmax+(4194304*U0min*Usmax*K0+2*U0min*Usmax)*volt_Ssmin)/(4194304*Usmax*Usmin*volt_S0min-4194304*Usmax*Usmin*volt_S0max);

        break;
    case 2:
        I0max=20;
        I0min=5.6;\
        Ismin= ui->UImin->text().toDouble();//Разобраться
        Ismax= ui->UImax->text().toDouble();//Разобраться

        current_S0max=current_s (double (Smaxd));
        current_S0min=current_s (double (Smind));
        current_Ssmax= current_s (Ssmax);
        current_Ssmin= current_s (Ssmin);

       // Kdac=(-50331648*I0min*Ismax+50331648*I0max*Ismin+(I0min*Ismax-I0max*Ismin)*B0+Ismax*Ismin*current_S0max-Ismax*Ismin*current_S0min-I0max*Ismin*current_Ssmax+I0min*Ismax*current_Ssmin+(2097152*I0min*Ismax*current_Ssmin-2097152*I0max*Ismin*current_Ssmax+(2097152*I0min*Ismax-2097152*I0max*Ismin)*B0)*K0)/(2097152*Ismax*Ismin*current_S0min-2097152*Ismax*Ismin*current_S0max);
        Kdac=((I0min*Ismax-I0max*Ismin)*B0+Ismax*Ismin*current_S0max-Ismax*Ismin*current_S0min-I0max*Ismin*current_Ssmax+I0min*Ismax*current_Ssmin+(2097152*I0min*Ismax*current_Ssmin-2097152*I0max*Ismin*current_Ssmax+(2097152*I0min*Ismax-2097152*I0max*Ismin)*B0)*K0)/(2097152*Ismax*Ismin*current_S0min-2097152*Ismax*Ismin*current_S0max);
        Bdac=-(I0min*Ismax*B0*current_S0max-I0max*Ismin*B0*current_S0min-I0max*Ismin*current_S0min*current_Ssmax+I0min*Ismax*current_S0max*current_Ssmin)/(I0min*Ismax*current_Ssmin-I0max*Ismin*current_Ssmax+(I0min*Ismax-I0max*Ismin)*B0);
//# Bdac=-((I0min*Ismax*B0+50331648*Ismax*Ismin-50331648*I0min*Ismax)*current_S0max+((50331648*I0max-50331648*Ismax)*Ismin-I0max*Ismin*B0)*current_S0min-I0max*Ismin*current_S0min*current_Ssmax+I0min*Ismax*current_S0max*current_Ssmin+(2097152*I0min*Ismax*current_S0max*current_Ssmin-2097152*I0max*Ismin*current_S0min*current_Ssmax-2097152*I0max*Ismin*B0*current_S0min+2097152*I0min*Ismax*B0*current_S0max)*K0)/(((2097152*I0min*Ismax-2097152*I0max*Ismin)*B0-2097152*I0max*Ismin*current_Ssmax+2097152*I0min*Ismax*current_Ssmin)*K0+I0min*Ismax*current_Ssmin-I0max*Ismin*current_Ssmax+(I0min*Ismax-I0max*Ismin)*B0+50331648*I0max*Ismin-50331648*I0min*Ismax);
        break;
    default:

        break;

    }
    Kdac=round(Kdac*1000)/1000;
    Bdac=round(Bdac*1000)/1000;
    ui->BtDac->setText(QString::number(Bdac));
    ui->KtDac->setText(QString::number(Kdac));
}

void Widget::on_RusSuperSpeed_clicked()
{
    double I1, I2, U1, U2, Z1, Z2, Bch1, Bch2, Kch1, Kch2, Cs, X1, X2, X1r, X2r, Ks0, Bs0, Xe1, Xe2, Xs1, Xs2;
    float  T, Smk1,Smk2, S1d26, S2d26, B0, K0;
    I1= ui->I1->text().toDouble();
    I2= ui->I2->text().toDouble();

    U1= ui->U1->text().toDouble();
    U2= ui->U2->text().toDouble();

    Cs= ui->Cs->text().toDouble();

    X1= ui->X1->text().toDouble();
    X2= ui->X2->text().toDouble();
    X1= X1/1000;
    X2= X2/1000;
    Ks0=1;
    Bs0=0;
//Расчет 1 канал ,как расчет по Х
    Z1=1000*U1/I1;
    Z2=1000*U2/I2;
    X1r=Cs/Z1;
    X2r=Cs/Z2;

    Z1=round(Z1);
    Z2=round(Z2);
    ui->Z1->setText(QString::number(Z1));
    ui->Z2->setText(QString::number(Z2));

    Xe1=100/CD*X1r;
    Xe2=100/CD*X2r;
    Xs1=100/CD*X1;
    Xs2=100/CD*X2;

     Bch1=(Xs1*Kus*Xe2-1000*Bs0*Xe2-Xs2*Kus*Xe1+1000*Bs0*Xe1)/(1000*(Xs1-Xs2)) ;
    Kch1=(Ks0*(Kus*Xe1-1000*Bch1))/(Kus*Xs1-1000*Bs0) ;
    Kch1=round(Kch1*1000)/1000;
    Bch1=round(Bch1*1000)/1000;
    ui->Bch1->setText(QString::number(Bch1));
    ui->Kch1->setText(QString::number(Kch1));

    // расчет 2 канала для компенсации разницы ПО микроконтролера и формул в методике
    //Считаем соль как МК
    T=45;
Smk1=SfX((float) X1r,T);
Smk2=SfX((float) X2r, T);
// Считаем соль как в Д26
S1d26=SfXd26 ((double)X1r,(double)T);
S2d26=SfXd26 ((double)X2r,(double)T);
B0=0;
K0=1;
float Smax, Smin;
Smax=-(B0-Smk2)/K0;
Smin=-(B0-Smk1)/K0;

Kch2=(S1d26-S2d26)/(Smin-Smax);
Bch2=(S2d26*Smin-S1d26*Smax)/(Smin-Smax);

Kch2=round(Kch2*1000)/1000;
Bch2=round(Bch2*1000)/1000;
ui->Bch2->setText(QString::number(Bch2));
ui->Kch2->setText(QString::number(Kch2));
}
