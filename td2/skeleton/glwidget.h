#pragma once

#include <QMatrix4x4>
#include <QOpenGLWidget>
#include <QString>

#include "face.h"

class GLWidget : public QOpenGLWidget {
public:
  Q_OBJECT
public:
  GLWidget(QWidget *parent = 0);
  ~GLWidget();
  QSize sizeHint() const { return QSize(400, 400); }

  void loadFaces(const QString &path);

protected:
  void initializeGL() override;
  void paintGL() override;

  void wheelEvent(QWheelEvent *event) override;
  void mouseMoveEvent(QMouseEvent *event) override;

  FaceCollection face_collection;
};
