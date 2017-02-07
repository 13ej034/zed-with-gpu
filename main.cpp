#include <iostream>
#include <chrono>
#include <zed/Camera.hpp>
#include <opencv2/core/core.hpp>;
#include <opencv2/highgui/highgui.hpp>;
#include <opencv2/imgproc/imgproc.hpp>;

// Input from keyboard
char keyboard = ' ';

int main(int argc, char** argv){
	// Initialize ZED color stream in VGA and depth in Performance mode
	sl::zed::Camera *zed = new sl::zed::Camera(sl::zed::VGA, 100.0);
	sl::zed::InitParams initParams = sl::zed::InitParams(sl::zed::MODE::PERFORMANCE);
	zed->init(initParams);

	// Loop until 'q' is pressed
	while (keyboard != 'q') 
	{
		auto s = std::chrono::system_clock::now();
		if (!zed->grab(sl::zed::SENSING_MODE::FILL))
		{
			// Retrieve left color image
			sl::zed::Mat left = zed->retrieveImage(sl::zed::SIDE::LEFT);
			cv::Mat l = sl::zed::slMat2cvMat(left);
			cv::imshow("left", l);

			// Retrieve right color image
			sl::zed::Mat right = zed->retrieveImage(sl::zed::SIDE::RIGHT);
			cv::Mat r = sl::zed::slMat2cvMat(right);
			cv::imshow("right", r);

			// Retrieve depth map
			sl::zed::Mat depthmap = zed->retrieveMeasure(sl::zed::MEASURE::DEPTH);
			cv::Mat d = sl::zed::slMat2cvMat(depthmap);
			cv::Mat d_64;
			d.convertTo(d_64, CV_64F);

			// Normalize depth map
			sl::zed::Mat depth_nn = zed->normalizeMeasure(sl::zed::MEASURE::DEPTH);
			cv::Mat dn = sl::zed::slMat2cvMat(depth_nn);
			cv::imshow("normalize", dn);
		}
		keyboard = cv::waitKey(15);
		auto e = std::chrono::system_clock::now();
		double f = std::chrono::duration_cast<std::chrono::milliseconds>(e - s).count();
		std::cout << "t = " << f << "[ms]" << std::endl;
	}

	delete zed;
	return 0;
}