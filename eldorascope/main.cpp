#include "SdrScope.h"

#include <qapplication.h>
#include <qstring.h>
#include <iostream>
#include "Reader.h"

int
main(int argc, char** argv)
{

    // create the readers
    TestReader<PulseReader> pulseReader(subscriber, pulseTopic);
    TestReader<TSReader> tsReader(subscriber, tsTopic);

   QApplication app(argc, argv);
   QDialog* dialog = new QDialog;
   
   // create our test dialog. It will contain an SdrScope
   SdrScope s(dialog);

  // if we don't show() the dialog, nothing appears!
   dialog->show();

  // run the whole thing
   app.exec();

   return 0;
}
