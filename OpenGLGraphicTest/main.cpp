#include <opencv2/opencv.hpp>
#include <GL/freeglut.h>
#include <random>
#include <ctime>


using namespace cv;
using namespace std;

cv::Mat cvImage = cv::imread("frame.png");

int img_width = 1920;
int img_height = 1080;
int alpha_blending = 80;
int line_thick = 6;
int randomNumber = 50;
vector<int> start_color = { 80, 20, 70 };

cv::Point startPoint(800, 561);     // Ω√¿€¡° ¡¬«•
cv::Point endPoint(1111, 800);      // ≥°¡° ¡¬«•

GLfloat startColor[3] = { 0.0f, 0.3f, 1.0f };
GLfloat endColor[3] = { 1.0f, 0.6f, 0.0f };

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
    
    std::cout << " line thickness : " << line_thick << std::endl;
    glEnable(GL_LINE_SMOOTH);

    glLineWidth(float(line_thick));
    glBegin(GL_LINES);

    glColor3f(start_color[0]/100.f, start_color[1] / 100.f, start_color[2] / 100.f); // (∆ƒ∂ıªˆ)
    glVertex2f(start.x, start.y);

    glColor3f(endColor[0], endColor[1], endColor[2]); //  (ª°∞£ªˆ)
    glVertex2f(end.x, end.y);

}


float opencvToOpenGLX(int x, int width) {
    return (2.0f * x / width) - 1.0f;
}

float opencvToOpenGLY(int y, int height) {
    return 1.0f - (2.0f * y / height);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT); 
    
    Point2f start, end;
    start.x = opencvToOpenGLX(startPoint.x, img_width);
    end.x = opencvToOpenGLX(endPoint.x, img_width);
    
    start.y = opencvToOpenGLY(startPoint.y, img_height);
    end.y = opencvToOpenGLY(endPoint.y, img_height);

    //std::cout << "point converse -> (start : " << start << " end : " << end << ")" << std::endl;

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
    glutPostRedisplay();
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
    std::cout << " alpha_blending : " << alpha_blending << std::endl;
    cv::addWeighted(openGLImage, alpha_blending /100.0, cvImage, 1, 0.0, openGLImage);
    cv::imshow("frame", openGLImage);
    waitKey(1);

    //cv::imwrite("golf_line.png", openGLImage);
}

void onAlphaBlendChange(int value, void* userdata) {
    alpha_blending = value;
    //graphicOnFrame();
    glutPostRedisplay();
}

void onLineThickChange(int value, void* userdata) {
    line_thick = value;
    glutPostRedisplay();
}

void onColorChange(int value, void* userdata) {
    int trackbar_id = *(static_cast<int*>(userdata));
    start_color[trackbar_id] = value;
    glutPostRedisplay();
}



int main(int argc, char** argv) {

    //init program
    cv::resize(cvImage, cvImage, cv::Size(img_width, img_height));
    namedWindow("frame");

    //interface
    std::cout << " >>>>>>  Press mouse buttons. \n ( left button : start, right button : end )\n\n" << std::endl;
    std::string text = "Click the mouse (Left button : start point, right button : end point) ";
    int fontFace = cv::FONT_HERSHEY_DUPLEX;
    double fontScale = 1.5;
    int thickness = 2;
    cv::putText(cvImage, text, cv::Point(100, 200), fontFace, fontScale, cv::Scalar(0, 255, 25), thickness);

    // draw opengl line
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(img_width, img_height);
    glutInitWindowPosition(img_width, 100);
    glutCreateWindow("Image with OpenGL and FreeGLUT");
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    setMouseCallback("frame", onMouseClick);
    cv::imshow("frame", cvImage);
    waitKey(1);

    //control panel
    cv::namedWindow("Control Panel");
    cv::moveWindow("Control Panel", 2000, 100);
    cv::resizeWindow("Control Panel", 800, 400);
    cv::createTrackbar("alpha blending", "Control Panel", &alpha_blending, 100, onAlphaBlendChange);
    cv::createTrackbar("line thickness", "Control Panel", &line_thick, 20, onLineThickChange);   
    int trackbar_r_id = 0;
    int trackbar_g_id = 1;
    int trackbar_b_id = 2;
    cv::createTrackbar("color r", "Control Panel", &start_color[0], 100, onColorChange, &trackbar_r_id);
    cv::createTrackbar("color g", "Control Panel", &start_color[1], 100, onColorChange, &trackbar_g_id);
    cv::createTrackbar("color b", "Control Panel", &start_color[2], 100, onColorChange, &trackbar_b_id);

    glutDisplayFunc(display);

    //glutMouseFunc(onMouseClick);
    
    glutMainLoop();

    return 0;
}
