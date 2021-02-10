#pragma once

#include "ui_FaceRecognitionUI.h"
#include "FaceRecognizer.h"
#include "TCPClient.h"
#include "ImageData.h"

class FaceRecognitionUI : public QWidget
{
	Q_OBJECT

public slots:
	void onExitButtonClicked();

public:
	FaceRecognitionUI(QWidget *parent = Q_NULLPTR);

	Ui::FaceRecognitionUI ui;

	QImage mat2QImage(cv::Mat const& src);

	void updateWindow(TCPClient& client);
  void recognize(int camera_id = 0);
	void sendImage(); // not finished

	~FaceRecognitionUI();

private:
	ImageData img_data;

	bool m_exit_button_clicked = false;
	bool run_analizer = true;
};