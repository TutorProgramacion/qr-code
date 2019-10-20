#include <opencv2/objdetect.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

void drawQRCodeContour(Mat& color_image, vector<Point> pts, string url)
{
	if (!pts.empty())
	{
		for (int i = 0; i < 4; i++) {
			line(color_image, pts.at(i), pts.at((i + 1) % 4), Scalar(0, 255, 0), 1, LINE_AA);
		}

		RNG rng(1000);
		for (size_t i = 0; i < 4; i++)
		{
			Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
			circle(color_image, pts[i], 6.5, color, FILLED, LINE_AA);
		}

		putText(color_image, url, Point(10, 30), FONT_HERSHEY_SIMPLEX, 1.0, Scalar(0, 0, 255), 1, LINE_AA);
	}
}

int main(int argc, char* argv[])
{
	// Init QR Detector
	QRCodeDetector qr = QRCodeDetector::QRCodeDetector();

	VideoCapture capture;
	if (capture.open(0)) {

		while (true) {

			Mat frame;
			capture >> frame;

			if (frame.empty()) continue;

			vector<Point> pts;
			if (qr.detect(frame, pts)) { 

				string decoded = qr.decode(frame, pts);
				if(!decoded.empty()) drawQRCodeContour(frame, pts, decoded);
			}

			imshow("QR", frame);

			if(waitKey(30) == 27) break;
		}

		capture.release();
	}
}