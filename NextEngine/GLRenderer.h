#ifndef GLRENDERER_H
#define GLRENDERER_H

#include <iostream>
#include <GL\glew.h>
#include "Shader.h"
#include <map>
#include <list>
#include <string>
#include <unordered_map>
#include "MeshVbo.h"
#include "glm.hpp"
#include "DrawableObject.h"
#include "Camera.h"

using namespace std;

class GLRenderer {
protected:
    int winWidth;
    int winHeight;
    glm::mat4 projectionMatrix;
    glm::mat4 modelMatrix;
    GLuint mMatrixId = -1;
    GLuint pMatrixId = -1;
    GLuint colorUniformId = -1;
    GLuint renderModeUniformId = -1;
    GLuint offsetXUniformId = -1;
    GLuint offsetYUniformId = -1;
    GLuint scaleXUniformId = -1;
    GLuint scaleYUniformId = -1;
    GLuint colorOverlayUniformId = -1;
    GLuint brightnessUniformId = -1;
    GLuint opacityUniformId = -1;

    GLuint gProgramId;
    int gPos2DLocation;
    int gTex2DLocation;
    void printProgramLog(GLuint program);
    bool initialize(string vertexShaderFile, string fragmentShaderFile);
    Shader* vertexShader;
    Shader* fragmentShader;
    map <string, MeshVbo*> shapes;
    void setMeshAttribId(MeshVbo* shape);
    Camera* camera;
    bool isViewportEnabled = true;
    float zoomRatio = 1.0f;

    std::unordered_map<std::string, unsigned int> textureHash;
    
public:

    GLRenderer(int w, int h);
    GLRenderer(int w, int h, Camera* cam);

    bool initGL(string vertexShaderFile, string fragmentShaderFile);

    void addMesh(string name, MeshVbo* shape);
    MeshVbo* getMesh(string name);
    void clearMesh();

    ~GLRenderer();
    void increaseZoomRatio(float newRatio);
    void decreaseZoomRatio(float newRatio);
    void setOrthoProjection(float left, float right, float bottom, float top);
    void setViewPort(int x, int y, int w, int h);
    void setClearColor(float r, float g, float b);

    GLuint getModelMatrixAttrId();
    GLuint getProjectionMatrixAttrId();
    GLuint getColorUniformId();
    GLuint getModeUniformId();
    GLuint getOffsetXUniformId();
    GLuint getOffsetYUniformId();
    GLuint getScaleXUniformId();
    GLuint getScaleYUniformId();
    GLuint getColorOverlayUniformId();
    GLuint getBrightnessUniformId();
    GLuint getOpacityUniformId();

    GLuint LoadTexture(string path);
    void setViewMatrix(const glm::mat4& viewMatrix);
    

    void toggleViewport();

    void setToggleViewport(bool value);

    void setGLViewport(int width, int height);

    void updateViewport(); 
    void updateCamera();
    void applyViewMatrix();
    void render(list<DrawableObject*>& objList, bool clear = true);
    glm::vec3 getCamPos();

    Camera* getCamera();

    void drawCameraOutline();
    
    bool getIsViewportEnabled() const;

	int getWindowWidth() const { return winWidth; }
	int getWindowHeight() const { return winHeight; }

};

#endif