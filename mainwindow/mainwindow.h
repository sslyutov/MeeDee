/*!\brief mainwindow declaration of mainwindow/creation class providing the 
* \file mainwindow.h
* \date January 6 
* \author Sergey Slyutov */

#ifndef _MAINWINDOW_H

#define _MAINWINDOW_H

#include <QMainWindow>

#include <QPainter>

class CMainWindow: public QMainWindow
{

Q_OBJECT

public: 

    CMainWindow();
    
    virtual ~CMainWindow(){};
    
protected:
    void resizeEvent(QResizeEvent *event) override {
            QMainWindow::resizeEvent(event);

            // Scale the image to fit the new window size
            QImage scaledImage = m_bgimage.scaled(size(), Qt::KeepAspectRatioByExpanding);
            
            // Set the scaled image as the background
            QPalette palette;
            palette.setBrush(QPalette::Window, QBrush(scaledImage));
            setPalette(palette);
        }

private:
    QImage m_bgimage;
    
};

#endif // _MAINWINDOW_H
