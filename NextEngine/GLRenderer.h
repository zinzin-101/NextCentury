#ifndef GLRENDERER_H
#define GLRENDERER_H

#include <iostream>
#include <GL\glew.h>
#include "Shader.h"
#include <map>
#include <list>
#include <string>
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

    GLuint gProgramId;
    int gPos2DLocation;
    int gTex2DLocation;
    void printProgramLog(GLuint program);
    bool initialize(string vertexShaderFile, string fragmentShaderFile);
    Shader* vertexShader;
    Shader* fragmentShader;
    map <string, MeshVbo*> shapes;
    void setMeshAttribId(MeshVbo* shape);
    Camera camera;
    bool isViewportEnabled;
    float zoomRatio = 540;
public:

    GLRenderer(int w, int h);
    GLRenderer(int w, int h, const Camera& cam);

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

    GLuint LoadTexture(string path);
    void setViewMatrix(const glm::mat4& viewMatrix);
    

    void toggleViewport(); 
    void updateViewport(); 
    void updateCamera(const glm::vec3& playerPosition);
    void applyViewMatrix();
    void render(list<DrawableObject*>& objList);
    glm::vec3 camPos();

    void drawCameraOutline();
    
};

#endif