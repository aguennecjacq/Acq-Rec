#include "viewer_widget.h"
#include <iostream>
#include <QFileDialog>

//Constructeur de la class ViewerWidget
ViewerWidget::ViewerWidget() {
  layout = new QGridLayout(this);
  //bouton loadfile
  load_file_button = new QPushButton("Load file");
  layout->addWidget(load_file_button, 0, 0);
  connect(load_file_button, SIGNAL(released()), this, SLOT(loadFile()));
  gl_widget = new GLWidget();
  layout->addWidget(gl_widget, 1, 0);
}

// methode loadFile de la class ViewerWidget
void ViewerWidget::loadFile() {
  QString file_name = QFileDialog::getOpenFileName(this,"Select Json File");
  //std::cout << "file name = " << file_name.toStdString() << std::endl;	
  gl_widget->loadFaces(file_name);
}
