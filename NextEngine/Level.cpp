#include "Level.h"
#include "CollisionHandler.h"
#include "MapLoader.h"

#ifdef DEBUG_MODE_ON
#include <fstream>
bool Level::enableCameraMove = false;
#endif

void Level::levelLoad() {
    SquareMeshVbo* square = new SquareMeshVbo();
    square->loadData();
    GameEngine::getInstance()->addMesh(SquareMeshVbo::MESH_NAME, square);

    SquareBorderMesh* border = new SquareBorderMesh();
    border->loadData();
    GameEngine::getInstance()->addMesh(SquareBorderMesh::MESH_NAME, border);
}

void Level::levelInit() {
    /// Will be implemented in inherited level when used ///
}


void Level::levelUpdate() {
    /// Will be implemented in inherited level when used ///
}

void Level::levelDraw() {
    GameEngine::getInstance()->render(objectsList);
}

void Level::levelFree() {
    for (DrawableObject* obj : objectsList) {
        delete obj;
    }
    objectsList.clear();
}

void Level::levelUnload() {
    GameEngine::getInstance()->clearMesh();
    //cout << "Unload Level" << endl;
}

void Level::handleKey(char key) {
    /// Will be implemented in inherited level when used ///
}

void Level::handleKey(InputManager& input) {
    /// Will be implemented in inherited level when used ///
}

void Level::processHeldKey(InputManager& input, SDL_Keycode key) {
    float dt = GameEngine::getInstance()->getTime()->getDeltaTime();

    if (input.getButtonDown(key)) {
        keyHeldDuration[key] = 0.0f;
        return;
    }

    if (input.getButton(key)) {
        keyHeldDuration[key] += dt;
        return;
    }
}

void Level::processKeyBuffer(InputManager& input, SDL_Keycode key) {
    float dt = GameEngine::getInstance()->getTime()->getDeltaTime();
    
    if (input.getButtonDown(key)) {
        keyBuffer[key]++;
        return;
    }
}

void Level::processHeldMouse(InputManager& input, unsigned int mouse) {
    float dt = GameEngine::getInstance()->getTime()->getDeltaTime();

    if (input.getMouseButtonDown(mouse)) {
        mouseHeldDuration[mouse] = 0.0f;
        return;
    }

    if (input.getMouseButton(mouse)) {
        mouseHeldDuration[mouse] += dt;
        return;
    }
}

void Level::processMouseBuffer(InputManager& input, unsigned int mouse) {
    float dt = GameEngine::getInstance()->getTime()->getDeltaTime();

    if (input.getButtonDown(mouse)) {
        mouseBuffer[mouse]++;
        return;
    }
}

void Level::clearKeyBuffer(SDL_Keycode key) {
    keyBuffer[key] = 0;
}

void Level::handleMouse(int type, int x, int y) {
    /// Will be implemented in inherited level when used ///
}

void Level::handleAnalogStick(int type, float amount) {

    /// Usage ///
    /*
    if (type == 0) { // x axis
        player->getTransform().translate(glm::vec3(0.3 * amount, 0, 0));
    }
    else if (type == 1) { // y axis
        player->getTransform().translate(glm::vec3(0, -0.3 * amount, 0));
    }
    */

    /// Will be implemented in inherited level when used ///
}

void Level::startObjects(list<DrawableObject*>& objectsList) {
    for (DrawableObject* obj : objectsList) {
        obj->start(objectsList);
    }
}

void Level::updateObjects(list<DrawableObject*>& objectsList) {
    for (DrawableObject* obj : objectsList) {
        if (obj->getIsActive()) {
            obj->update(objectsList);
        }
    }

    handleObjectCollision(objectsList);

    for (std::list<DrawableObject*>::iterator itr = objectsList.begin(); itr != objectsList.end(); itr++) {
        DrawableObject* obj = *itr;
        if (obj->getMarkedForDelete()) {
            delete obj;
            itr = objectsList.erase(itr);
            if (itr != objectsList.begin()) itr--;
            if (itr == objectsList.end()) break;
        }
    }
}

void Level::initPlayer(PlayerObject*& player, PlayerInfo playerInfo) {
    if (player == nullptr) {
        player = new PlayerObject(playerInfo);
        objectsList.emplace_back(player);
    }
    else {
        player->setName(playerInfo.name);
        player->setHealth(playerInfo.health);
        //player->setMovementInfo(playerInfo.movementInfo);
    }

    player->setDrawCollider(true); // for debugging
    //player->getTransform().setScale(glm::vec3(500.0f, 500.0f , 1.0f));
}

#ifdef DEBUG_MODE_ON
void Level::drawImGui(std::list<DrawableObject*>& objectList) {
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Object List", NULL, ImGuiWindowFlags_NoCollapse);

    static bool pauseGame = false;
    ImGui::Checkbox("Pause Game", &pauseGame);
    ImGui::Checkbox("Enable Debug Camera Movement", &enableCameraMove);

    pauseGame ? GameEngine::getInstance()->getTime()->setTimeScale(0.0f) : GameEngine::getInstance()->getTime()->setTimeScale(1.0f);

    if (ImGui::Button("Reset Scene")) {
        GameEngine::getInstance()->getStateController()->gameStateNext = GameState::GS_RESTART;
    }
    ImGui::SameLine();
    if (ImGui::Button("Switch Scene")) {
        GameEngine::getInstance()->getStateController()->gameStateNext = (GameState)((GameEngine::getInstance()->getStateController()->gameStateCurr + 1) % 3);
    }

    if (ImGui::Button("Export Transform Data")) {
        exportTransformData(objectList);
    }

    static char stringBuffer[50] = "\0";
    static char nameBuffer[50] = "\0";
    static bool isSearching = false;
    int objectCounter = 0;

    ImGui::InputText("Search Text Field", stringBuffer, 50);

    static std::string searchString;

    if (ImGui::Button("Search name")) {
        searchString = std::string(stringBuffer);
        isSearching = true;
    }

    ImGui::SameLine();

    if (ImGui::Button("clear search")) {
        isSearching = false;
    }

    for (DrawableObject* obj : objectList) {
        objectCounter++;

        if (isSearching) {
            std::string name = obj->getName();

            std::transform(name.begin(), name.end(), name.begin(),
                [](unsigned char c) { return std::tolower(c); });

            std::transform(searchString.begin(), searchString.end(), searchString.begin(),
                [](unsigned char c) { return std::tolower(c); });

            if (name != searchString) {

                continue;
            }
        }

        ImGui::PushID(objectCounter);

        if (ImGui::CollapsingHeader(obj->getName().c_str())) {
            ImGui::InputText("", nameBuffer, 50);
            ImGui::SameLine();
            if (ImGui::Button("Change name")) {
                obj->setName(std::string(nameBuffer));
                stringBuffer[0] = '\0';
            }

            bool isActive = obj->getIsActive();
            ImGui::Checkbox("Is Active", &isActive);
            obj->setActive(isActive);

            ImGui::SeparatorText("");

            static float inputNum;
            ImGui::InputFloat("Float Input Field", &inputNum);
            if (ImGui::Button("<<<")) {
                inputNum -= 10.0f;
            }
            ImGui::SameLine();
            if (ImGui::Button("<<")) {
                inputNum -= 5.0f;
            }
            ImGui::SameLine();
            if (ImGui::Button("<")) {
                inputNum -= 1.0f;
            }
            ImGui::SameLine();
            if (ImGui::Button(">")) {
                inputNum += 1.0f;
            }
            ImGui::SameLine();
            if (ImGui::Button(">>")) {
                inputNum += 5.0f;
            }
            ImGui::SameLine();
            if (ImGui::Button(">>>")) {
                inputNum += 10.0f;
            }

            bool drawCollider = obj->getCanDrawCollider();
            ImGui::Checkbox("Draw Collider", &drawCollider);
            obj->setDrawCollider(drawCollider);

            ImGui::SeparatorText("Transform");

            Transform transform = obj->getTransform();
            glm::vec3 position = transform.getPosition();
            float rotationRad = transform.getRotationRad();
            float rotationDeg = transform.getRotationDeg();
            glm::vec3 scale = transform.getScale();
            ImGui::Text("Position: x: %.2f, y: %.2f", position.x, position.y);
            ImGui::SameLine();
            if (ImGui::Button("Set X position")) {
                position.x = inputNum;
                obj->getTransform().setPosition(position);
            }
            ImGui::SameLine();
            if (ImGui::Button("Set Y position")) {
                position.y = inputNum;
                obj->getTransform().setPosition(position);
            }

            ImGui::Text("Rotation: %.2f rad / %.2f degree", rotationRad, rotationDeg);
            ImGui::SameLine();
            if (ImGui::Button("Set Rotation Rad")) {
                rotationRad = inputNum;
                obj->getTransform().setRotationRad(rotationRad);
            }
            ImGui::SameLine();
            if (ImGui::Button("Set Rotation Degree")) {
                rotationDeg = inputNum;
                obj->getTransform().setRotationDeg(rotationDeg);
            }

            ImGui::Text("Scale: x: %.2f, y: %.2f", scale.x, scale.y);
            ImGui::SameLine();
            if (ImGui::Button("Set Scale X")) {
                scale.x = inputNum;
                obj->getTransform().setScale(scale);
            }
            ImGui::SameLine();
            if (ImGui::Button("Set Scale Y")) {
                scale.y = inputNum;
                obj->getTransform().setScale(scale);
            }

            ImGui::PopID();
            ImGui::PushID(-objectCounter);

            if (obj->getColliderComponent() != nullptr) {
                ImGui::SeparatorText("Collider");

                bool isCollisionEnabled = obj->getColliderComponent()->isEnable();
                ImGui::Checkbox("Enable Collision", &isCollisionEnabled);
                obj->getColliderComponent()->setEnableCollision(isCollisionEnabled);

                bool isTrigger = obj->getColliderComponent()->isTrigger();
                ImGui::Checkbox("Is Trigger", &isTrigger);
                obj->getColliderComponent()->setTrigger(isTrigger);

                ImGui::Text("Dimension: Width: %.2f, Height: %.2f", obj->getColliderComponent()->getWidth(), obj->getColliderComponent()->getHeight());
                ImGui::SameLine();
                if (ImGui::Button("Set Width")) {
                    obj->getColliderComponent()->setWidth(inputNum);
                }
                ImGui::SameLine();
                if (ImGui::Button("Set Height")) {
                    obj->getColliderComponent()->setHeight(inputNum);
                }

                Transform colTransform = obj->getColliderComponent()->getTransform();
                glm::vec3 pos = colTransform.getPosition();
                float rotRad = colTransform.getRotationRad();
                float rotDeg = colTransform.getRotationDeg();
                glm::vec3 colScale = colTransform.getScale();
                ImGui::Text("Collider Offset");
                ImGui::Text("Position: x: %.2f, y: %.2f", pos.x, pos.y);
                ImGui::SameLine();
                if (ImGui::Button("Set Offset X")) {
                    pos.x = inputNum;
                    obj->getColliderComponent()->getTransform().setPosition(pos);
                }
                ImGui::SameLine();
                if (ImGui::Button("Set Offset Y")) {
                    pos.y = inputNum;
                    obj->getColliderComponent()->getTransform().setPosition(pos);
                }

                ImGui::Text("Rotation: %.2f rad / %.2f degree", rotRad, rotDeg);
                ImGui::SameLine();
                if (ImGui::Button("Set Rotation Rad")) {
                    rotRad = inputNum;
                    obj->getColliderComponent()->getTransform().setRotationRad(rotRad);
                }
                ImGui::SameLine();
                if (ImGui::Button("Set Rotation Deg")) {
                    rotDeg = inputNum;
                    obj->getColliderComponent()->getTransform().setRotationRad(rotDeg);
                }


                ImGui::Text("Scale: x: %.2f, y: %.2f", colScale.x, colScale.y);
                ImGui::SameLine();
                if (ImGui::Button("Set Scale X")) {
                    colScale.x = inputNum;
                    obj->getColliderComponent()->getTransform().setScale(colScale);
                }
                ImGui::SameLine();
                if (ImGui::Button("Set Scale Y")) {
                    colScale.y = inputNum;
                    obj->getColliderComponent()->getTransform().setScale(colScale);
                }
            }

            if (obj->getPhysicsComponent() != nullptr) {
                ImGui::SeparatorText("Physics");
                bool isPhysicsEnabled = obj->getPhysicsComponent()->isPhysicsEnable();
                ImGui::Checkbox("Enable Physics", &isPhysicsEnabled);
                obj->getPhysicsComponent()->setEnablePhysics(isPhysicsEnabled);

                bool isGravityEnabled = obj->getPhysicsComponent()->isGravityEnable();
                ImGui::Checkbox("Enable Gravity", &isGravityEnabled);
                obj->getPhysicsComponent()->setEnableGravity(isGravityEnabled);
            }
            ImGui::SeparatorText("");
            static unsigned int deletePressed = 0;
            if (deletePressed == 0) {
                if (ImGui::Button("Delete Object")) {
                    deletePressed++;
                }
            }
            else {
                if (ImGui::Button("Sure?")) {
                    DrawableObject::destroyObject(obj);
                    deletePressed = 0;
                }
                ImGui::SameLine();
                if (ImGui::Button("No")) {
                    deletePressed = 0;
                }
            }
        }

        ImGui::SeparatorText("");
        ImGui::PopID();
    }

    ImGui::End();

    ImGui::Begin("Object Spawner");
    static float posX;
    static float posY;
    ImGui::Text("Position:");
    ImGui::InputFloat("X", &posX);
    ImGui::InputFloat("Y", &posY);

    ImGui::SeparatorText("Collider Object");
    if (ImGui::Button("Spawn Collider Object")) {
        DrawableObject* obj = new ColliderObject("ColliderObject");
        obj->getTransform().setPosition(posX, posY);
        objectList.emplace_back(obj);
    }
    ImGui::SeparatorText("Lightsource");
    static float brightness;
    static float maxDistance;
    ImGui::InputFloat("Brightness", &brightness);
    ImGui::InputFloat("Max Distance", &maxDistance);

    if (ImGui::Button("Spawn LightSource")) {
        DrawableObject* light = new LightSource(brightness, maxDistance);
        light->getTransform().setPosition(posX, posY);
        light->setName("LightSource");
        objectList.emplace_back(light);
    }
    ImGui::End();

    ImGui::Render();

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Level::exportTransformData(std::list<DrawableObject*>& objectsList) {
    std::ofstream output("../Resource/output/TransformData.txt");

    if (!output) {
        return;
    }

    for (DrawableObject* obj : objectsList) {
        output << obj->getName() << ", ";
        Transform transform = obj->getTransform();
        glm::vec3 position = transform.getPosition();
        glm::vec3 scale = transform.getScale();
        output << "Position: " << "x: " << position.x << ", y: " << position.y << ", ";
        output << "Rotation: " << transform.getRotationDeg() << " deg" << ", ";
        output << "Scale: " << "x: " << scale.x << ", y: " << scale.y;

        Collider* col = obj->getColliderComponent();

        if (col != nullptr) {
            output << ", ";

            transform = col->getTransform();
            position = transform.getPosition();
            scale = transform.getScale();
            output << "Collider Width: " << col->getWidth() << ", Collider Height: " << col->getHeight() << ", ";
            output << "Collider Position: " << "x: " << position.x << ", y: " << position.y << ", ";
            output << "Collider Rotation: " << transform.getRotationDeg() << " deg" << ", ";
            output << "Collider Scale: " << "x: " << scale.x << ", y: " << scale.y;
        }

        output << "\n";
    }

    output.close();
}

#endif