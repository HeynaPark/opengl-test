#include <opencv2/opencv.hpp>
#include <GL/freeglut.h>

using namespace cv;

GLuint textureID;
cv::Point startPoint(-800, -500);  // 시작점 좌표
cv::Point endPoint(800, 500);      // 끝점 좌표

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


void drawLine(Point& start, Point& end) {
    glLineWidth(4.0f);
    glBegin(GL_LINES);

    glColor3f(1.0f, 0.0f, 0.0f); // 시작점 색상 (빨간색)
    glVertex2f(start.x / 1920.0f, start.y / 1080.0f);

    glColor3f(0.0f, 0.0f, 1.0f); // 끝점 색상 (파란색)
    glVertex2f(end.x / 1920.0f, end.y / 1080.0f);



}

void loadTextureFromMat(const cv::Mat& image) {
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.cols, image.rows, 0, GL_RGB, GL_UNSIGNED_BYTE, image.data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void display() {

    glClear(GL_COLOR_BUFFER_BIT);



    drawLine(startPoint, endPoint);

    glEnd();


    //unsigned char* pixelData = new unsigned char[3 * 1920 * 1080];
    //glReadPixels(0, 0, 1920, 1080, GL_RGB, GL_UNSIGNED_BYTE, pixelData);

    //// Create an OpenCV Mat from the OpenGL pixel data
    //cv::Mat openGLImage(1080, 1920, CV_8UC3);
    //for (int y = 0; y < openGLImage.rows; ++y) {
    //    for (int x = 0; x < openGLImage.cols; ++x) {
    //        openGLImage.at<cv::Vec3b>(y, x) = cv::Vec3b(pixelData[3 * (y * openGLImage.cols + x) + 2],
    //            pixelData[3 * (y * openGLImage.cols + x) + 1],
    //            pixelData[3 * (y * openGLImage.cols + x)]);
    //    }
    //}

    //delete[] pixelData;

    //// Load an OpenCV image
    //cv::Mat cvImage = cv::imread("frame.png");  // Replace with your image path

    //// Resize the OpenCV image to match the OpenGL rendering resolution
    //cv::resize(cvImage, cvImage, cv::Size(1920, 1080));

    //// Blend the OpenGL image and the OpenCV image
    //cv::addWeighted(openGLImage, 1, cvImage, 1, 0.0, openGLImage);

    // Save the final image
 /*   cv::imwrite("final_image.png", openGLImage);
    cv::imshow("test", openGLImage);
    waitKey();*/


    glFlush(); 
    //saveImage("-.png", 1920, 1080);

}

void onMouseClick(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        // 마우스 좌표를 OpenGL 좌표로 변환
        float normalizedX = (2.0f * x) / 1920.0f - 1.0f;
        float normalizedY = 1.0f - (2.0f * y) / 1080.0f;

        // 첫 번째 점을 마우스 클릭한 위치로 업데이트
        startPoint = cv::Point(normalizedX * 1920.0f, normalizedY * 1080.0f);

        // 다시 그리기 요청
        glutPostRedisplay();
    }
    else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
        // 마우스 좌표를 OpenGL 좌표로 변환
        float normalizedX = (2.0f * x) / 1920.0f - 1.0f;
        float normalizedY = 1.0f - (2.0f * y) / 1080.0f;

        // 두 번째 점을 마우스 클릭한 위치로 업데이트
        endPoint = cv::Point(normalizedX * 1920.0f, normalizedY * 1080.0f);

        // 다시 그리기 요청
        glutPostRedisplay();
    }
}


int main(int argc, char** argv) {

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(1920, 1080);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Image with OpenGL and FreeGLUT");
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glutDisplayFunc(display);
    glutMouseFunc(onMouseClick);

    glutMainLoop();

    return 0;
}
