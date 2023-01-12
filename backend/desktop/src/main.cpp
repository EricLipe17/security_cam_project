#include <cstdio>
#include <cstdlib>
#include <string>
#include <iostream>
#include <sstream>

extern "C" {
#include "RtspStream.h"
}

#include <GL/glew.h>

#include "GLFW/glfw3.h"

static unsigned int nbFrames;
double lastTime = 0;

static void showFPS(GLFWwindow *pWindow)
{
    // Measure speed
    double currentTime = glfwGetTime();
    double delta = currentTime - lastTime;
    nbFrames++;
    if ( delta >= 1.0 ){
        double fps = nbFrames / delta;

        std::stringstream ss;
        ss << "IPCam" << " " << " [" << fps << " FPS]";

        glfwSetWindowTitle(pWindow, ss.str().c_str());

        nbFrames = 0;
        lastTime = currentTime;
    }
}


int main()
{
    // Setup screen to display video data
    GLFWwindow* pWindow;
    if (!glfwInit()) {
        printf("Couldn't init GLFW.\n");
        return 1;
    }

    // Initialize our stream
    const char* pUrl = "rtsp://192.168.1.20:554/video2";
    RtspStream* pStream = alloc_stream();
    init_stream(pStream, pUrl);
    init_buffers(pStream, 10);
    init_formatter(pStream, AV_PIX_FMT_RGB0, SWS_FAST_BILINEAR, nullptr, nullptr, nullptr);
    const int nFrameWidth = pStream->m_nWidth;
    const int nFrameHeight = pStream->m_nHeight;

    // Initialize window to view stream
    pWindow = glfwCreateWindow(nFrameWidth, nFrameHeight, "IPCam", nullptr, nullptr);
    if (!pWindow) {
        printf("Couldn't create window.\n");
        return 1;
    }
    glfwMakeContextCurrent(pWindow);

    // Must be initialized after a window, and it's context have been created.
    glewExperimental = true;
    GLenum err=glewInit();
    if(err!=GLEW_OK) {
        // Problem: glewInit failed, something is seriously wrong.
        std::cout << "glewInit failed: " << glewGetErrorString(err) << std::endl;
        exit(1);
    }

    // Set up texture
    GLuint tex_handle;
    glGenTextures(1, &tex_handle);
    glBindTexture(GL_TEXTURE_2D, tex_handle);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, nFrameWidth, nFrameHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, pStream->m_pFrameBuffer);
    glBindTexture(GL_TEXTURE_2D, 0);

    // Set up PBO to stream texture data
    unsigned int nPboId;
    glGenBuffers(1, &nPboId);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, nPboId);
    glBufferData(GL_PIXEL_UNPACK_BUFFER, pStream->m_nFrameBufferSize, pStream->m_pFrameBuffer, GL_STREAM_DRAW);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);

    // Setup orthographic projection
    int win_width, win_height;
    glfwGetFramebufferSize(pWindow, &win_width, &win_height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, win_width, win_height, 0, -1, 1);

    // Reset back to model view to render texture
    glMatrixMode(GL_MODELVIEW);

    while (!glfwWindowShouldClose(pWindow))
    {
        get_next_frame(pStream);
        format_frame(pStream);
        // RtspStream frame data to texture
        {
            // bind the texture and PBO
            glBindTexture(GL_TEXTURE_2D, tex_handle);
            glBindBuffer(GL_PIXEL_UNPACK_BUFFER, nPboId);

            // copy pixels from PBO to texture object
            // Use offset instead of pointer.
            glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, nFrameWidth, nFrameHeight, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
            // start to modify pixel values

            // bind PBO to update pixel values
            glBindBuffer(GL_PIXEL_UNPACK_BUFFER, nPboId);

            // map the buffer object into client's memory
            // Note that glMapBuffer() causes sync issue.
            // If GPU is working with this buffer, glMapBuffer() will wait(stall)
            // for GPU to finish its job. To avoid waiting (stall), you can call
            // first glBufferData() with NULL pointer before glMapBuffer().
            // If you do that, the previous data in PBO will be discarded and
            // glMapBuffer() returns a new allocated pointer immediately
            // even if GPU is still working with the previous data.
            glBufferData(GL_PIXEL_UNPACK_BUFFER, pStream->m_nFrameBufferSize, pStream->m_pFrameBuffer, GL_STREAM_DRAW);
            auto *pBuf = (GLubyte *) glMapBuffer(GL_PIXEL_UNPACK_BUFFER, GL_WRITE_ONLY);
            if (pBuf) {
                // update data directly on the mapped buffer
                for (int i = 0; i < pStream->m_nFrameBufferSize; ++i)
                {
                    pBuf[i] = pStream->m_pFrameBuffer[i];
                }
                glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);  // release pointer to mapping buffer
            }

            // it is good idea to release PBOs with ID 0 after use.
            // Once bound with 0, all pixel operations behave normal ways.
            glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // save the initial ModelView matrix before modifying ModelView matrix
        glPushMatrix();

        // Render texture
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, tex_handle);
        glBegin(GL_QUADS);
            glTexCoord2d(0,  0); glVertex2i(0, 0);

            glTexCoord2d(1,  0); glVertex2i( nFrameWidth, 0);

            glTexCoord2d(1,  1); glVertex2i( nFrameWidth,  nFrameHeight);

            glTexCoord2d(0,  1); glVertex2i(0,  nFrameHeight);

        glEnd();
        glDisable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);
        glPopMatrix();

        glfwSwapBuffers(pWindow);
        showFPS(pWindow);
        glfwPollEvents();
    }

    free_stream(pStream);

    return EXIT_SUCCESS;
}
