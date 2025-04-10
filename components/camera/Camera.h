#ifndef _CAMERA_H

class Camera {
    private:
        int y2, y3, y4, y5, y6, y7, y8, y9, xclk, pclk, vsync, href, siod, sioc, pwdn, reset;
    public:
        Camera(int y2, int y3, int y4, int y5, int y6, int y7, int y8, int y9, int xclk, int pclk, int vsync, int href, int siod, int sioc, int pwdn, int reset);
        void setUp();
        void startStreamServer();

};
#endif