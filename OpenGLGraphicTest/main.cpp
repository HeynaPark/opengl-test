#include <opencv2/opencv.hpp>
#include <GL/freeglut.h>

using namespace cv;

cv::Mat cvImage = cv::imread("frame.png");

int img_width = 1920;
int img_height = 1080;
float line_thick = 7.0f;
cv::Point startPoint(-800, -500);  // ½ÃÀÛÁ¡ ÁÂÇ¥
cv::Point endPoint(800, 500);      // ³¡Á¡ ÁÂÇ¥

void graphicOnFrame();

void saveImage(const char* filename, int width, int height) {
    int channels = 3; // RGB channels
    unsigned char* imageData = new unsigned char[4 * width * height];
    glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, imageData);

    Mat image(height, width, CV_8UC4, imageData);
    Mat rgbImage;
    cvtColor(image, rgbImage, COLOR_RGBA2BGR); // Convert RGBA to BGR

    flip(rgbImage, rgbImage, 0); // Flip the image vertically

    imwrite(filename, rgbImage);

    delete[] imageData;
}


void drawLine(Point2f& start, Point2f& end) {
    
    glLineWidth(line_thick);
    glEnable(GL_LINE_SMOOTH);
    glBegin(GL_LINES);

    glColor3f(0.0f, 0.0f, 1.0f); // (ÆÄ¶õ»ö)
    glVertex2f(start.x, start.y);

    glColor3f(1.0f, 0.0f, 0.0f); //  (»¡°£»ö)
    glVertex2f(end.x, end.y);

}


float opencvToOpenGLX(int x, int width) {
    return (2.0f * x / width) - 1.0f;
}

float opencvToOpenGLY(int y, int height) {
    return 1.0f - (2.0f * y / height);
}

void display() {
    Point2f start, end;

    glClear(GL_COLOR_BUFFER_BIT);

    start.x = opencvToOpenGLX(startPoint.x, img_width);
    end.x = opencvToOpenGLX(endPoint.x, img_width);
    
    start.y = opencvToOpenGLY(startPoint.y, img_height);
    end.y = opencvToOpenGLY(endPoint.y, img_height);

    std::cout << "point converse -> (start : " << start << " end : " << end << ")" << std::endl;

    drawLine(start, end);

    glEnd();
    glFlush(); 

    graphicOnFrame();

    //glutPostRedisplay();
    //saveImage("golf_line.png", 1920, 1080);
}

void onMouseClick(int event, int x, int y, int flags, void* userdata) {
    if (event == cv::EVENT_LBUTTONDOWN) {
        startPoint = cv::Point(x, y);
        std::cout << "Left button clicked at (" << x << ", " << y << ")" << std::endl;
    }
    else if (event == cv::EVENT_RBUTTONDOWN) {
        endPoint = cv::Point(x, y);
        std::cout << "Right button clicked at (" << x << ", " << y << ")\n" << std::endl;
        std::cout << " press > space bar <\n" << std::endl;
    }

}


void graphicOnFrame() {

    unsigned char* pixelData = new unsigned char[3 * img_width * 1080];
    glReadPixels(0, 0, img_width, img_height, GL_RGB, GL_UNSIGNED_BYTE, pixelData);

    // Create an OpenCV Mat from the OpenGL pixel data
    cv::Mat openGLImage(img_height, img_width, CV_8UC3);
    for (int y = 0; y < openGLImage.rows; ++y) {
        for (int x = 0; x < openGLImage.cols; ++x) {
            int flippedY = img_height - 1 - y;
            openGLImage.at<cv::Vec3b>(flippedY, x) = cv::Vec3b(pixelData[3 * (y * openGLImage.cols + x) + 2],
                pixelData[3 * (y * openGLImage.cols + x) + 1],
                pixelData[3 * (y * openGLImage.cols + x)]);
        }
    }

    delete[] pixelData;

    // Blend the OpenGL image and the OpenCV image
    cv::addWeighted(openGLImage, 1, cvImage, 1, 0.0, openGLImage);
    cv::imshow("frame", openGLImage);
    waitKey();

    //cv::imwrite("golf_line.png", openGLImage);
}


int main(int argc, char** argv) {

    std::cout << " >>>>>>  Press mouse buttons. \n ( left button : start, right button : end )\n\n" << std::endl;

    // get input points
    //Point2f prevStartPoint, prevEndPoint;
    //prevStartPoint = startPoint;
    //prevEndPoint = endPoint;
    cv::resize(cvImage, cvImage, cv::Size(img_width, img_height));
    namedWindow("frame");
    setMouseCallback("frame", onMouseClick);
    cv::imshow("frame", cvImage);
    waitKey(0);

    // draw opengl line
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(img_width, img_height);
    glutInitWindowPosition(img_width, 100);
    glutCreateWindow("Image with OpenGL and FreeGLUT");
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glutDisplayFunc(display);
    //glutMouseFunc(onMouseClick);
    
    glutMainLoop();

    return 0;
}
