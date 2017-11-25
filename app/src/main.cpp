//
//  main.cpp
//  Main
//
//  Created by Istvan Fehervari on 01/05/2017.
//  Copyright © 2017 com.ifehervari. All rights reserved.
//

#include "gui/mainwindow.h"
#include <QApplication>
#include "utils/proxystyle.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setStyle(new ProxyStyle);
    MainWindow w;
    w.show();

    return a.exec();
}
