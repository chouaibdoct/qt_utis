#include <verdigris/wobjectdefs.h>
#include <verdigris/wobjectimpl.h>
#include <QApplication>
#include <iostream>
#include <memory>
#include <string>
#include <type_traits>

#include <utils/readonly_combobox.h>



int main(int argc, char *argv[]){
   QApplication a(argc, argv);
   
    ReadOnly<QComboBox> aa;
    aa.addItem("anaya");
    aa.addItem("howa");
    aa.setReadOnly(true);
    aa.show();
   
   return a.exec();
};
