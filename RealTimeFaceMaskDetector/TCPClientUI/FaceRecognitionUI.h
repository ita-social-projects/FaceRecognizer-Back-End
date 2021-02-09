#pragma once

#include "ui_FaceRecognitionUI.h"
#include "FaceRecognizer.h"
#include "TCPClient.h"
#include "ImageData.h"

class FaceRecognitionUI : public QWidget
{
	Q_OBJECT

public:
	FaceRecognitionUI(QWidget *parent = Q_NULLPTR);
	~FaceRecognitionUI();

	bool m_exit_button_clicked = false;
	void UpdateWindow(TCPClient &client);
	QImage Mat2QImage(cv::Mat const& src);

	void Recognize(int camera_id=0);

public slots:
	void onExitButtonClicked();

public:
	Ui::FaceRecognitionUI ui;

private:
	ImageData img_data;
	bool run_analizer = true;
};

extern std::unique_ptr<FaceRecognizer> m_mask_recognizer;