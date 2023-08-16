#include <opencv2/opencv.hpp>
#include <GL/freeglut.h>

using namespace cv;


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


void drawLine(int x1, int y1, int x2, int y2) {
    glLineWidth(8.0f);

    glBegin(GL_LINES);

    glColor3f(1.0f, 0.0f, 0.0f); // 시작점 색상 (빨간색)
    glVertex2f(-0.5f, 0.8f);

    glColor3f(0.0f, 0.0f, 1.0f); // 끝점 색상 (파란색)
    glVertex2f(0.5f, -0.6f);


    glEnd();
}

void display() {

    glClear(GL_COLOR_BUFFER_BIT);

    drawLine(-0.8f, -0.5f, 0.8f, 0.5f);

    glFlush(); 
    saveImage("-.png", 1920, 1080);

}


int main(int argc, char** argv) {

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(1920, 1080);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Image with OpenGL and FreeGLUT");
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glutDisplayFunc(display);
    glutMainLoop();

    return 0;
}
