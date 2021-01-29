#pragma once
//#include <vector>

//#include <QWidget>
//#include <QImage>
//#include <QDebug>
//
//#include "ui_FaceRecognitionUI.h"
//#include "MaskRecognizer.h"
//#include "TCPClient.h"
//
//class FaceRecognitionUI : public QWidget
//{
//	Q_OBJECT
//
//public:
//	FaceRecognitionUI(QWidget *parent = Q_NULLPTR);
//	~FaceRecognitionUI();
//
//	bool m_exit_button_clicked = false;
//	void Recognize(TCPClient &client);
//	QImage Mat2QImage(cv::Mat const& src);
//	friend std::vector<char> GetImageBytesVector();
//
//public slots:
//	void onExitButtonClicked();
//
//public:
//	Ui::FaceRecognitionUI ui;
//};
//
//extern std::unique_ptr<MaskRecognizer> m_mask_recognizer;