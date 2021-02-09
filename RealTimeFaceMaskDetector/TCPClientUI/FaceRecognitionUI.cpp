#include "TCPClient.h"

FaceRecognitionUI::FaceRecognitionUI(QWidget* parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    connect(ui.exit_button, &QPushButton::clicked, this, &FaceRecognitionUI::onExitButtonClicked);
}

void FaceRecognitionUI::onExitButtonClicked()
{
    m_exit_button_clicked = true;
    close();


    qDebug() << "QQQQ!!!";
};

void FaceRecognitionUI::Recognize(TCPClient &client)
{
    
    while (!m_exit_button_clicked)
    {
        m_mask_recognizer->StartRecognition();
        cv::Mat image = m_mask_recognizer->GetImage();
        QImage frame = Mat2QImage(image);
        QPixmap map = QPixmap::fromImage(frame.scaled(640, 480, Qt::KeepAspectRatio, Qt::FastTransformation));
        ui.frame->setPixmap(map);
        ui.frame->show();
        cv::waitKey(30);
        std::vector<char> buffer;
        cv::imwrite("image_face.png", m_mask_recognizer->GetImage());

        std::ifstream image_face_send;
        client.ConvertImageToBinary(image_face_send, buffer);
        client.SendBinaryMessage(buffer);

        qDebug() << "QQQQ\n   ";
    }
}

QImage FaceRecognitionUI::Mat2QImage(cv::Mat const& src)
{
    cv::Mat temp; // make the same cv::Mat
    cvtColor(src, temp, cv::COLOR_BGR2RGB); // cvtColor Makes a copt, that what i need
    QImage dest((const uchar*)temp.data, temp.cols, temp.rows, temp.step, QImage::Format_RGB888);
    dest.bits(); // enforce deep copy, see documentation 
    // of QImage::QImage ( const uchar * data, int width, int height, Format format )
    return dest;
}
std::unique_ptr<MaskRecognizer> m_mask_recognizer = std::make_unique<MaskRecognizer>();

std::vector<char> GetImageBytesVector()
{
    cv::Mat image = m_mask_recognizer->GetImage();
    std::vector<char> bytes_vector;
    if (image.isContinuous()) {
        // array.assign(mat.datastart, mat.dataend); // <- has problems for sub-matrix like mat = big_mat.row(i)
        bytes_vector.assign(image.data, image.data + image.total() * image.channels());
    }
    else {
        for (int i = 0; i < image.rows; ++i) {
            bytes_vector.insert(bytes_vector.end(), image.ptr<char>(i), image.ptr<char>(i) + image.cols * image.channels());
        }
    }
    return bytes_vector;
}


FaceRecognitionUI::~FaceRecognitionUI()
{
}
