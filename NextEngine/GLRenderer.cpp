#include "GLRenderer.h"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include "SDL_surface.h"
#include "SDL_image.h"

using namespace std;
GLRenderer::GLRenderer(int w, int h) : winWidth(w), winHeight(h), camera(new Camera()) {}

GLRenderer::GLRenderer(int w, int h, Camera* cam) : winWidth(w), winHeight(h), camera(cam) {
    // Initialize other members if needed
    projectionMatrix = glm::ortho(-1.f, 1.f, -1.f, 1.f);
    glViewport(0, 0, winWidth, winHeight);
}
bool GLRenderer::initGL(string vertexShaderFile, string fragmentShaderFile) {
    // Initialize glew
    GLenum glewError = glewInit();
    if (glewError != GLEW_OK) {
        cout << "Error initializing GLEW! " << glewGetErrorString(glewError) << endl;
        return false;
    }


    //Initialize OpenGL
    if (!initialize(vertexShaderFile, fragmentShaderFile)) {
        cout << "Unable to initialize OpenGL! " << endl;
        return false;
    }
    return true;
}

bool GLRenderer::initialize(string vertexShaderFile, string fragmentShaderFile) {
    //Success flag
    bool success = true;

    //Generate program
    gProgramId = glCreateProgram();
    vertexShader = new Shader(vertexShaderFile, GL_VERTEX_SHADER);
    if (!vertexShader->loadSource()) {
        return false;
    }
    fragmentShader = new Shader(fragmentShaderFile, GL_FRAGMENT_SHADER);
    if (!fragmentShader->loadSource()) {
        return false;
    }
    glAttachShader(gProgramId, vertexShader->getShaderId());
    glAttachShader(gProgramId, fragmentShader->getShaderId());


    //Link program
    glLinkProgram(gProgramId);

    //Check for errors
    GLint programSuccess = GL_TRUE;
    glGetProgramiv(gProgramId, GL_LINK_STATUS, &programSuccess);
    if (programSuccess != GL_TRUE) {
        cout << "Error linking program " << gProgramId << endl;
        printProgramLog(gProgramId);
        return false;
    }

    //Get vertex attribute location
    gPos2DLocation = glGetAttribLocation(gProgramId, "pos2D");
    if (gPos2DLocation == -1) {
        cout << "pos2D is not a valid glsl program variable" << endl;
        return false;
    }
    gTex2DLocation = glGetAttribLocation(gProgramId, "inTexCoord");
    if (gTex2DLocation == -1) {
        cout << "inTexCoord is not a valid glsl program variable" << endl;
        return false;
    }

    //Setting color uniform id
    colorUniformId = glGetUniformLocation(gProgramId, "color");
    if (colorUniformId == -1) {
        cout << "color is not a valid glsl uniform variable" << endl;
        return false;
    }

    //Set up uniform id attribute
    pMatrixId = glGetUniformLocation(gProgramId, "pMatrix");
    if (pMatrixId == -1) {
        cout << "pMatrix is not a valid glsl uniform variable" << endl;
        return false;
    }
    mMatrixId = glGetUniformLocation(gProgramId, "mMatrix");
    if (mMatrixId == -1) {
        cout << "mMatrix is not a valid glsl uniform variable" << endl;
        return false;
    }
    renderModeUniformId = glGetUniformLocation(gProgramId, "renderMode");
    if (renderModeUniformId == -1) {
        cout << "renderMode is not a valid glsl uniform variable" << endl;
        return false;
    }
    offsetXUniformId = glGetUniformLocation(gProgramId, "offsetX");
    if (offsetXUniformId == -1) {
        cout << "offsetX is not a valid glsl uniform variable" << endl;
        return false;
    }
    offsetYUniformId = glGetUniformLocation(gProgramId, "offsetY");
    if (offsetYUniformId == -1) {
        cout << "offsetY is not a valid glsl uniform variable" << endl;
        return false;
    }
    scaleXUniformId = glGetUniformLocation(gProgramId, "scaleX");
    if (scaleXUniformId == -1) {
        cout << "scaleXUniformId is not a valid glsl uniform variable" << endl;
        return false;
    }
    scaleYUniformId = glGetUniformLocation(gProgramId, "scaleY");
    if (scaleYUniformId == -1) {
        cout << "scaleYUniformId is not a valid glsl uniform variable" << endl;
        return false;
    }

    colorOverlayUniformId = glGetUniformLocation(gProgramId, "colorOverlay");
    if (colorOverlayUniformId == -1) {
        cout << "colorOverlay is not a valid glsl uniform variable" << endl;
        return false;
    }

    brightnessUniformId = glGetUniformLocation(gProgramId, "brightness");
    if (brightnessUniformId == -1) {
        cout << "brightness is not a valid glsl uniform variable" << endl;
        return false;
    }

    glEnableVertexAttribArray(gPos2DLocation);
    glEnableVertexAttribArray(gTex2DLocation);

    //Initialize clear color
    glClearColor(1.0f, 1.0f, 1.0f, 1.f);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    return true;

}

void GLRenderer::render(list<DrawableObject*>& objList, bool clear) {
    // Clear color buffer
    if (clear) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    // Update window with OpenGL rendering
    glUseProgram(gProgramId);

    // Set projection matrix
    glUniformMatrix4fv(pMatrixId, 1, GL_FALSE, glm::value_ptr(this->projectionMatrix));

    // Set view matrix
    glm::mat4 view = camera->getViewMatrix();
    setViewMatrix(view);

    // Calculate the model matrix if necessary, or use the default
    glm::mat4 cam = glm::mat4(1.0f); 

    applyViewMatrix();

    // Loop through objects and call render with the model transform
    for (DrawableObject* obj : objList) {
        if (!obj->getIsActive()) {
            continue;
        }

        obj->render(cam); 
        obj->drawCollider();
    }

    // Draw the camera outline
    //drawCameraOutline();

    // Unbind program
    glUseProgram(0);
}

void GLRenderer::setMeshAttribId(MeshVbo* shape) {
    shape->setAttribId(gPos2DLocation, gTex2DLocation);
}

void GLRenderer::addMesh(string name, MeshVbo* shape) {
    setMeshAttribId(shape);
    shapes[name] = shape;
}

MeshVbo* GLRenderer::getMesh(string name) {
    if (shapes.find(name) == shapes.end()) {
        return nullptr;
    }
    else {
        return shapes[name];
    }
}

void GLRenderer::clearMesh() {
    for (map<string, MeshVbo*>::iterator it = shapes.begin(); it != shapes.end(); it++) {
        delete it->second;
    }

    shapes.clear();
}

void GLRenderer::printProgramLog(GLuint program) {
    //Make sure name is shader
    if (glIsProgram(program)) {
        //Program log length
        int infoLogLength = 0;
        int maxLength = infoLogLength;

        //Get info string length
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

        //Allocate string
        char* infoLog = new char[maxLength];

        //Get info log
        glGetProgramInfoLog(program, maxLength, &infoLogLength, infoLog);
        if (infoLogLength > 0)
        {
            //Print Log
            cout << infoLog << endl;

        }

        //Deallocate string
        delete[] infoLog;
    }
    else {
        printf("Name %d is not a program\n", program);
    }
}

GLRenderer::~GLRenderer() {
    if (gPos2DLocation != -1) {
        glDisableVertexAttribArray(gPos2DLocation);
    }
    //if (camera != nullptr) {
    //    delete camera;
    //}
}

void GLRenderer::setOrthoProjection(float left, float right, float bottom, float top) {
    projectionMatrix = glm::ortho(left, right, bottom, top);
}

void GLRenderer::setViewPort(int x, int y, int w, int h) {
    glViewport(x, y, w, h);
}

void GLRenderer::setClearColor(float r, float g, float b) {
    glClearColor(r, g, b, 1.0);
}

GLuint GLRenderer::getModelMatrixAttrId() {
    return this->mMatrixId;
}

GLuint GLRenderer::getProjectionMatrixAttrId() {
    return this->pMatrixId;
}

GLuint GLRenderer::getColorUniformId() {
    return this->colorUniformId;
}

GLuint GLRenderer::getModeUniformId() {
    return this->renderModeUniformId;
}
GLuint GLRenderer::getOffsetXUniformId()
{
    return this->offsetXUniformId;
}

GLuint GLRenderer::getOffsetYUniformId()
{
    return this->offsetYUniformId;
}

GLuint GLRenderer::getScaleXUniformId() {
    return this->scaleXUniformId;
}

GLuint GLRenderer::getScaleYUniformId() {
    return this->scaleYUniformId;
}

GLuint GLRenderer::getColorOverlayUniformId() {
    return this->colorOverlayUniformId;
}

GLuint GLRenderer::getBrightnessUniformId() {
    return this->brightnessUniformId;
}

GLuint GLRenderer::LoadTexture(string path) {
    glActiveTexture(GL_TEXTURE0);
    SDL_Surface* image = IMG_Load(path.c_str());
    if (image == NULL) {
        cerr << "IMG_Load: " << SDL_GetError() << endl;
        return -1;
    }
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    int Mode = GL_RGB;
    if (image->format->BytesPerPixel == 4) {
        Mode = GL_RGBA;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, Mode, image->w, image->h, 0, Mode, GL_UNSIGNED_BYTE, image->pixels);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    SDL_FreeSurface(image);

    return texture;
}

void GLRenderer::setViewMatrix(const glm::mat4& viewMatrix) {
    GLuint viewMatrixId = glGetUniformLocation(gProgramId, "viewMatrix");
    if (viewMatrixId != -1) {
        glUniformMatrix4fv(viewMatrixId, 1, GL_FALSE, glm::value_ptr(viewMatrix));
    }
}
void GLRenderer::updateCamera(const glm::vec3& playerPosition) {
    //camera->setPosition(glm::vec3(playerPosition.x + 1.0f, playerPosition.y + 1.0f, camera->getPosition().z));
    
    camera->followTarget();
    updateViewport();
    //if (isViewportEnabled) {
    //    
    //}
}

void GLRenderer::increaseZoomRatio(float newRatio) {
    zoomRatio += newRatio;
}
void GLRenderer::decreaseZoomRatio(float newRatio) {
    zoomRatio -= newRatio;
}

void GLRenderer::applyViewMatrix() {
    glm::vec3 a;
    if (camera->getTarget() == nullptr) {
        a = glm::vec3(0.0f, 0.0f, 0.0f);
    }
    else {
        a = camera->getTarget()->getTransform().getPosition();
    }
    
    glm::mat4 viewMatrix = glm::lookAt(camera->getPosition(), a, glm::vec3(0.0f, 1.0f, 0.0f));
    setViewMatrix(viewMatrix); 
}

void GLRenderer::drawCameraOutline() {
    glm::vec3 cameraPosition = camera->getPosition();

    float left = cameraPosition.x - (winWidth / 2.0f) * (1.0f / winHeight); 
    float right = cameraPosition.x + (winWidth / 2.0f) * (1.0f / winHeight);
    float bottom = cameraPosition.y - (winHeight / 2.0f) * (1.0f / winHeight);
    float top = cameraPosition.y + (winHeight / 2.0f) * (1.0f / winHeight);

    glUniform4f(colorUniformId, 1.0f, 0.0f, 0.0f, 1.0f); 

    glBegin(GL_LINE_LOOP);
    glVertex2f(left, top);
    glVertex2f(right, top);
    glVertex2f(right, bottom);
    glVertex2f(left, bottom);
    glEnd();
}

Camera* GLRenderer::getCamera() {
    return camera;
}

void GLRenderer::updateViewport() {
    //camera.setPosition(glm::vec3(camera.getPosition().x, camera.getPosition().y, -100.0f));
    glm::vec3 cameraPosition = camera->getPosition();
    //cameraPosition.y += 190;
    float aspectRatio = static_cast<float>(winWidth) / winHeight;
    float left = cameraPosition.x - zoomRatio * aspectRatio;
    float right = cameraPosition.x + zoomRatio * aspectRatio;
    float bottom = cameraPosition.y  - zoomRatio;
    float top = cameraPosition.y  + zoomRatio;

    if (isViewportEnabled) {
        setOrthoProjection(cameraPosition.x + -8, cameraPosition.x + 8, cameraPosition.y + -4.5, cameraPosition.y + 4.5);
    }
    else {
        setOrthoProjection(left, right, bottom, top);
    }
    
}
void GLRenderer::toggleViewport() {
    isViewportEnabled = !isViewportEnabled; 
    std::cout << "Viewport updating is now " << (isViewportEnabled ? "enabled" : "disabled") << std::endl;
}

void GLRenderer::setToggleViewport(bool value) {
    isViewportEnabled = value;
}

glm::vec3 GLRenderer::getCamPos() {
    return camera->getPosition();
}


bool GLRenderer::getIsViewportEnabled() const {
    return isViewportEnabled;
}
