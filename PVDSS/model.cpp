#include "model.h"

model::model() : Bt(0)
{
}

QString model::getText() {
return QString::number(Bt);
}

void model::setValue(float value) {
Bt = value;
emit valueChanged(getText(), Bt);
}


float model::getValue()
{
return Bt;
}
void model::rasT(){
    --Bt;
    emit valueChanged(getText(), Bt);

}
