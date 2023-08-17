#include <opencv2/opencv.hpp>
#include <GL/freeglut.h>

using namespace cv;

cv::Mat cvImage = cv::imread("frame.png");

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
    return (2.0f * y / height)- 1.0f;
}

void display() {
    Point2f start, end;

    glClear(GL_COLOR_BUFFER_BIT);

    start.x = opencvToOpenGLX(startPoint.x, 1920);
    end.x = opencvToOpenGLX(endPoint.x, 1920);
    
    start.y = opencvToOpenGLY(startPoint.y, 1080);
    end.y = opencvToOpenGLY(endPoint.y, 1080);

    std::cout << "point converse -> (start : " << start << " end : " << end << ")" << std::endl;

    drawLine(start, end);

    glEnd();

    graphicOnFrame();

    glFlush(); 
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
        std::cout << "Right button clicked at (" << x << ", " << y << ")" << std::endl;
    }
}


void graphicOnFrame() {

    unsigned char* pixelData = new unsigned char[3 * 1920 * 1080];
    glReadPixels(0, 0, 1920, 1080, GL_RGB, GL_UNSIGNED_BYTE, pixelData);

    // Create an OpenCV Mat from the OpenGL pixel data
    cv::Mat openGLImage(1080, 1920, CV_8UC3);
    for (int y = 0; y < openGLImage.rows; ++y) {
        for (int x = 0; x < openGLImage.cols; ++x) {
            openGLImage.at<cv::Vec3b>(y, x) = cv::Vec3b(pixelData[3 * (y * openGLImage.cols + x) + 2],
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

    // get input points
    cv::resize(cvImage, cvImage, cv::Size(1920, 1080));
    namedWindow("frame");
    setMouseCallback("frame", onMouseClick);
    cv::imshow("frame", cvImage);
    waitKey(0);

    // draw opengl line
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(1920, 1080);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Image with OpenGL and FreeGLUT");
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glutDisplayFunc(display);
    //glutMouseFunc(onMouseClick);
    
    glutMainLoop();

    return 0;
}
