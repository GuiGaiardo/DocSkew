#include "opencv2/opencv.hpp"
#include <iostream>
#include <algorithm>

float initial_estimate(cv::Mat &image){
    cv::Mat processed, c_hough;
    std::vector<cv::Vec2f> lines;

    bool try_again = true;
    int k_size = 30;
    while (try_again){
        cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(k_size, k_size));

        cv::erode(image.clone(), processed, element, cv::Point(-1, -1), 1);
        cv::Canny(processed, c_hough, 240.0, 250.0);

        cv::HoughLines(c_hough, lines, 1, CV_PI/3600, 200, 0, 0);

        if (lines.size() > 10){
            try_again = false;
        } else if (k_size > 0) {
            k_size -= 5;
        } else {
            return 30;
        }
    }

    std::vector<float> deviations;
    for (size_t i = 0; i < lines.size() && deviations.size() < 17; i++){
        float rho = lines[i][0], theta = lines[i][1] * 180 / CV_PI;

        float dev = 0;
        bool plot = true;

        if (theta < 16){
            dev = theta;
        } else if (theta > 74 && theta < 106){
            dev = theta - 90.0f;
        } else if (theta > 154){
            dev = theta - 180.0f;
        } else {
            plot = false;
        }

        if (plot)
            deviations.push_back(dev);
    }

    std::sort(deviations.begin(), deviations.end());
    float median = deviations[deviations.size()/2];

    return median;
}

float estimate_visualize(cv::Mat &image){
    cv::Mat processed, c_hough, to_show;
    to_show = image.clone();
    std::vector<cv::Vec2f> lines;

    bool try_again = true;
    int k_size = 30;
    while (try_again){
        cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(k_size, k_size));

        cv::erode(image.clone(), processed, element, cv::Point(-1, -1), 1);
        cv::Canny(processed, c_hough, 240.0, 250.0);

        cv::HoughLines(c_hough, lines, 1, CV_PI/3600, 200, 0, 0);

        if (lines.size() > 10){
            try_again = false;
        } else if (k_size > 0) {
            k_size -= 5;
        } else {
            return 30;
        }
    }
    cv::cvtColor(to_show, to_show, cv::COLOR_GRAY2BGR);

    std::vector<float> deviations;
    for (size_t i = 0; i < lines.size() && deviations.size() < 17; i++){
        float rho = lines[i][0], theta = lines[i][1] * 180 / CV_PI;

        float dev = 0;
        bool plot = true;

        if (theta < 16){
            dev = theta;
        } else if (theta > 74 && theta < 106){
            dev = theta - 90.0f;
        } else if (theta > 154){
            dev = theta - 180.0f;
        } else {
            plot = false;
        }

        if (plot){
            deviations.push_back(dev);
            cv::Point pt1, pt2;
            double a = cos(lines[i][1]), b = sin(lines[i][1]);
            double x0 = a*rho, y0 = b*rho;
            pt1.x = cvRound(x0 + 5000*(-b));
            pt1.y = cvRound(y0 + 5000*(a));
            pt2.x = cvRound(x0 - 5000*(-b));
            pt2.y = cvRound(y0 - 5000*(a));
            line(to_show, pt1, pt2, cv::Scalar(0,0,255), 1, CV_AA);
        }
    }
    std::cout << "Found " << deviations.size() << " lines." << std::endl;
    std::sort(deviations.begin(), deviations.end());
    float median = deviations[deviations.size()/2];

    std::cout << "Theta found: " << median << std::endl;
    cv::imwrite("processed.png", c_hough);
    cv::imwrite("initial_estimate.png", to_show);
    std::cout << "Input anything to continue." << std::endl;
    std::cin.get();

    return median;
}

std::string get_image_name(std::string file_name){
    int idx = file_name.find("IMG");
    return file_name.substr(idx);
}


int main(int argc, char** argv){
    int visualize = 0;

	if (argc < 2){
        std::cout << "Usage" << std::endl << argv[0] << " <images_dir> <visualization_on>" << std::endl;
        std::cout << "Example:" << std::endl << argv[0] << " icdar2013_benchmarking_dataset/" << std::endl;
		return 0;
    } else if (argc == 3){
        visualize = 1;
    }

    std::string images_directory(argv[1]);
    images_directory += "/*.tif";

    std::vector<cv::String> filenames;
    cv::glob(images_directory, filenames);
    if (visualize){
        for (int f=0; f<filenames.size(); f++){
            std::cout << get_image_name(filenames[f]) << std::endl;
            cv::Mat image = cv::imread(filenames[f], 0);
            float estimate = estimate_visualize(image);
        }
    } else {
        #pragma omp parallel for schedule(dynamic)
        for (int f=0; f<filenames.size(); f++){
            cv::Mat image = cv::imread(filenames[f], 0);
            float estimate = initial_estimate(image);
            std::cout << get_image_name(filenames[f]) << ", " << estimate << std::endl;
        }
    }

}
