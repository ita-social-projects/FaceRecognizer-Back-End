#pragma once
#include <chrono>
#include <QtCore/QDebug>
#include <QtGui/QPainter>
#include <thread>
#include <utility>
#include <future>

#include "ui_FaceRecognitionUI.h"
#include "FaceRecognizer.h"
#include "TCPClient.h"
#include "TimeCounting.h"

#define IDCAM 0
#define TIME_PERIOD 5


class FaceRecognitionUI : public QWidget
{
	Q_OBJECT

public slots:
	void onExitButtonClicked();

public:
	FaceRecognitionUI(QWidget *parent = Q_NULLPTR);

	Ui::FaceRecognitionUI ui;

	QImage mat2QImage();
	void displayFrame();
	void updateWindow(TCPClient& client);
	void sendImage(TCPClient& client, cv::UMat face_img);
	bool is_ready(std::future<faceInfo> const& f);
	void SetPanelText();
	~FaceRecognitionUI();

private:
	cv::UMat m_image;
	bool m_async_is_permitted = true;
	bool m_is_all_in_mask = true;
	bool m_exit_button_clicked = false;
};