#include <QFile>
#include <QString>
#include <QTransform>
#include <QtGui>

#include "glwidget.h"

#include <iostream>

GLWidget::GLWidget(QWidget *parent) : QOpenGLWidget(parent) {
}

GLWidget::~GLWidget() {}

void GLWidget::loadFaces(const QString &path) {
  QFile json_file(path);
  if (!json_file.open(QIODevice::ReadOnly)) {
    qWarning("Failed to open file");
    exit(-1);
  }
  QByteArray json_data = json_file.readAll();
  QJsonDocument json_document(QJsonDocument::fromJson(json_data));
  face_collection.fromJson(json_document.array());

  // TODO: update the display inside the widget
}

void GLWidget::initializeGL() {
  glClearColor(0,0,0,0);                   // Set backGround color
  glShadeModel(GL_SMOOTH);
  glEnable(GL_DEPTH_TEST);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glDisable(GL_CULL_FACE);	//Make sure to get transparency of the back of the faces
}

void GLWidget::paintGL() {
  QSize viewport_size = size();
  glViewport(0, 0, viewport_size.width(), viewport_size.height());

  glMatrixMode(GL_PROJECTION);
  // TODO: define the point of view

  glMatrixMode(GL_MODELVIEW);
  // TODO: draw the model
}

void GLWidget::mouseMoveEvent(QMouseEvent *event) {
  // TODO:
  // - Capture mouse motions
  // - If left click is pressed: rotate
  // - If right click is pressed: translate
}

void GLWidget::wheelEvent(QWheelEvent *event) {
  // TODO: zoom
}
