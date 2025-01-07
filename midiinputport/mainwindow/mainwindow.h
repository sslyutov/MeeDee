/*!\brief mainwindow declaration of mainwindow/creation class providing the 
* \file mainwindow.h
* \date January 6 
* \author Sergey Slyutov */

#ifndef _MAINWINDOW_H

#define _MAINWINDOW_H

#include <QMainWindow>

class CMainWindow: public QMainWindow
{

Q_OBJECT

public: 

    CMainWindow();
    
    virtual ~CMainWindow(){};

};

#endif // _MAINWINDOW_H
