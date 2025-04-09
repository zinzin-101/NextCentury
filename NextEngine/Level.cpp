#include "Level.h"
#include "CollisionHandler.h"
#include "MapLoader.h"
#include <fstream>

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
        keyBuffer[key] = LevelConstant::DEFAULT_BUFFER_DURATION;
        return;
    }

    if (keyBuffer[key] > 0.0f) {
        keyBuffer[key] -= dt;
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

    if (input.getMouseButtonDown(mouse)) {
        mouseBuffer[mouse] = LevelConstant::DEFAULT_BUFFER_DURATION;
        return;
    }

    if (mouseBuffer[mouse] > 0.0f) {
        mouseBuffer[mouse] -= dt;
    }
}

bool Level::isKeyInBuffer(SDL_Keycode key) const {
    return keyBuffer.at(key) > 0.0f;
}

bool Level::isMouseInBuffer(unsigned int mouse) const {
    return keyBuffer.at(mouse) > 0.0f;
}

void Level::clearKeyBuffer(SDL_Keycode key) {
    keyBuffer[key] = 0.0f;
}

void Level::clearMouseBuffer(unsigned int mouse) {
    mouseBuffer[mouse] = 0.0f;
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
    //if (player == nullptr) {
    //    player = new PlayerObject(playerInfo);
    //    objectsList.emplace_back(player);
    //}
    //else {
    //    player->setName(playerInfo.name);
    //    player->setHealth(playerInfo.health);
    //    //player->setMovementInfo(playerInfo.movementInfo);
    //}

    //player->setDrawCollider(true); // for debugging
    ////player->getTransform().setScale(glm::vec3(500.0f, 500.0f , 1.0f));
}

#ifdef DEBUG_MODE_ON
void Level::drawImGui(std::list<DrawableObject*>& objectList) {
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Object List", NULL);

    static bool pauseGame = false;
    ImGui::Checkbox("Pause Game", &pauseGame);

    bool enableFreeViewPort = !GameEngine::getInstance()->getRenderer()->getIsViewportEnabled();
    ImGui::Checkbox("Free Viewport", &enableFreeViewPort);
    GameEngine::getInstance()->getRenderer()->setToggleViewport(!enableFreeViewPort);

    pauseGame ? GameEngine::getInstance()->getTime()->setTimeScale(0.0f) : GameEngine::getInstance()->getTime()->setTimeScale(1.0f);

    if (ImGui::Button("Reset Scene")) {
        GameEngine::getInstance()->getStateController()->gameStateNext = GameState::GS_RESTART;
    }
    ImGui::SameLine();
    if (ImGui::Button("Switch Scene")) {
        GameEngine::getInstance()->getStateController()->gameStateNext = (GameState)((GameEngine::getInstance()->getStateController()->gameStateCurr + 1) % 3);
    }

    static char stringBuffer[50] = "\0";
    static char nameBuffer[50] = "\0";
    static bool isSearching = false;
    int objectCounter = 0;

    ImGui::InputText("Text Field", stringBuffer, 50);

    static bool importOutline = false;
    ImGui::Checkbox("Draw Outline When Import", &importOutline);
    ImGui::SameLine();
    if (ImGui::Button("Import Data")) {
        importTransformData(objectList, std::string(stringBuffer), importOutline);
    }
    ImGui::SameLine();
    if (ImGui::Button("Export Data")) {
        exportTransformData(objectList, std::string(stringBuffer));
    }

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
            static float increment = 0.1f;
            ImGui::InputFloat("Custom Increment", &increment);
            if (ImGui::Button("-")) {
                inputNum -= increment;
            }
            ImGui::SameLine();
            if (ImGui::Button("+")) {
                inputNum += increment;
            }

            bool drawCollider = obj->getCanDrawCollider();
            ImGui::Checkbox("Draw Outline", &drawCollider);
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

                if (ImGui::Button("Reset Velocity")) {
                    obj->getPhysicsComponent()->setVelocity(glm::vec2());
                }
            }

            LightSource* light = dynamic_cast<LightSource*>(obj);
            if (light != NULL) {
                ImGui::SeparatorText("Light Source");
                ImGui::Text("Brightness: %.2f (0 - 1)", light->getBrightness());
                ImGui::SameLine();
                if (ImGui::Button("Set Brightness")) {
                    light->setBrightness(inputNum);
                }

                ImGui::Text("Max Distance: %.2f", light->getMaxDistance());
                ImGui::SameLine();
                if (ImGui::Button("Set Max Distance")) {
                    light->setMaxDistance(inputNum);
                }
            }

            EnemyObject* enemy = dynamic_cast<EnemyObject*>(obj);
            if (enemy != NULL) {
                ImGui::SeparatorText("Enemy Info");
                ImGui::Text("Health: %d", enemy->getHealth());
                ImGui::SameLine();
                if (ImGui::Button("Set Health")) {
                    enemy->setHealth(inputNum);
                }
                if (ImGui::Button("Set Player As Target")) {
                    PlayerObject* player = EnemyObject::findPlayer(objectList);
                    enemy->setTarget(player);
                }
            }

            PlayerObject* player = dynamic_cast<PlayerObject*>(obj);
            if (player != NULL) {
                ImGui::SeparatorText("Player Info");
                ImGui::Text("Health: %d", player->getHealth());
                ImGui::SameLine();
                if (ImGui::Button("Set Health")) {
                    player->setHealth(inputNum);
                }
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
    static float posX = 0.0f;
    static float posY = 0.0f;
    static float scaleX = 1.0f;
    static float scaleY = 1.0f;
    ImGui::Text("Position:");
    ImGui::InputFloat("X", &posX);
    ImGui::InputFloat("Y", &posY);
    ImGui::InputFloat("Scale X", &scaleX);
    ImGui::InputFloat("Scale Y", &scaleY);
    static bool drawOutline = false;
    ImGui::Checkbox("Draw Outline on spawn", &drawOutline);

    ImGui::SeparatorText("Collider Object");
    static float width = 1;
    static float height = 1;
    ImGui::InputFloat("Width", &width);
    ImGui::InputFloat("Height", &height);
    if (ImGui::Button("Spawn Collider Object")) {
        DrawableObject* obj = new ColliderObject("ColliderObject");
        obj->getTransform().setPosition(posX, posY);
        obj->getTransform().setScale(scaleX, scaleY);
        obj->setDrawCollider(drawOutline);
        obj->getColliderComponent()->setDimension(width, height);
        objectList.emplace_back(obj);
    }
    ImGui::SeparatorText("Lightsource");
    static float brightness = 1;
    static float maxDistance = 10;
    ImGui::InputFloat("Brightness", &brightness);
    ImGui::InputFloat("Max Distance", &maxDistance);

    if (ImGui::Button("Spawn LightSource")) {
        DrawableObject* light = new LightSource(brightness, maxDistance);
        light->getTransform().setPosition(posX, posY);
        light->getTransform().setScale(scaleX, scaleY);
        light->setName("LightSource");
        light->setDrawCollider(drawOutline);
        objectList.emplace_back(light);
    }
    ImGui::SeparatorText("Enemy");
    if (ImGui::Button("Spawn Zealot")) {
        Zealot* zealot = new Zealot(DefaultEnemyStat::ZEALOT_INFO);
        zealot->getTransform().setPosition(posX, posY);
        //zealot->getTransform().setScale(scaleX, scaleY);
        zealot->setDrawCollider(drawOutline);
        objectList.emplace_back(zealot);
        zealot->start(objectList);
    }
    if (ImGui::Button("Spawn BlightFlame")) {
        BlightFlame* bf = new BlightFlame(DefaultEnemyStat::BLIGHT_FLAME_INFO);
        bf->getTransform().setPosition(posX, posY);
        //bf->getTransform().setScale(scaleX, scaleY);
        bf->setDrawCollider(drawOutline);
        objectList.emplace_back(bf);
        bf->start(objectList);
    }
    ImGui::SeparatorText("Player");
    if (ImGui::Button("Spawn Player")) {
        PlayerObject* player = EnemyObject::findPlayer(objectList);
        if (player == nullptr) {
            player = new PlayerObject();
            player->getTransform().setPosition(posX, posY);
            player->getTransform().setScale(scaleX, scaleY);
            player->setDrawCollider(drawOutline);
            objectList.emplace_back(player);
            player->start(objectList);

            for (DrawableObject* obj : objectList) {
                ParallaxObject* pObj = dynamic_cast<ParallaxObject*>(obj);
                if (pObj != NULL) {
                    pObj->setPlayer(player);
                }
            }
        }
        else {
            std::cout << "Player already exists" << std::endl;
        }
    }
    ImGui::End();

    ImGui::Render();

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
#endif

void Level::exportTransformData(std::list<DrawableObject*>& objectsList, std::string fileName) {
    std::ofstream output("../Resource/output/" + fileName + ".txt");

    if (!output) {
        std::cout << "Failed to write file" << std::endl;
        return;
    }

    for (DrawableObject* obj : objectsList) {

        ColliderObject* colObj = dynamic_cast<ColliderObject*>(obj);
        LightSource* lightObj = dynamic_cast<LightSource*>(obj);
        EnemyObject* enemyObj = dynamic_cast<EnemyObject*>(obj);
        
        DamageCollider<EnemyObject>* dmgColEnemy = dynamic_cast<DamageCollider<EnemyObject>*>(obj);
        DamageCollider<PlayerObject>* dmgColPlayer = dynamic_cast<DamageCollider<PlayerObject>*>(obj);
        FlameDamage<PlayerObject>* flameCol = dynamic_cast<FlameDamage<PlayerObject>*>(obj);

        if (dmgColEnemy || dmgColPlayer || flameCol) {
            continue;
        }

        if (enemyObj != NULL) {
            appendEnemyData(output, enemyObj);
            continue;
        }

        if (colObj == NULL && lightObj == NULL) {
            continue;
        }

        output << ((colObj != NULL) ? "ColliderObject," : "LightSource,");
        
        output << obj->getName() << ",";
        Transform transform = obj->getTransform();
        glm::vec3 position = transform.getPosition();
        glm::vec3 scale = transform.getScale();
        output << position.x << "," << position.y << ",";
        output << transform.getRotationRad() << ",";
        output << scale.x << "," << scale.y;

        Collider* col = obj->getColliderComponent();

        output << ",";

        if (col != nullptr) {
            transform = col->getTransform();
            position = transform.getPosition();
            scale = transform.getScale();
            output << col->getWidth() << "," << col->getHeight() << ",";
            output << position.x << "," << position.y << ",";
            output << transform.getRotationRad() << ",";
            output << scale.x << "," << scale.y;
        }
        else if (lightObj != NULL) {
            output << lightObj->getBrightness() << ",";
            output << lightObj->getMaxDistance();
        }

        output << "\n";
    }

    output.close();
}

void Level::importTransformData(std::list<DrawableObject*>& objectsList, std::string fileName, bool drawOutline) {
    std::ifstream file("../Resource/output/" + fileName + ".txt");
    if (!file) {
        std::cout << "Failed to read file" << std::endl;
        return;
    }

    std::string line;
    while (!file.eof()) {
        if (!std::getline(file, line, ',')) {
            break;
        }

        std::string type = line;

        if (type == "Zealot" || type == "BlightFlame") {
            readEnemyData(file, type, objectsList, drawOutline);
            continue;
        }

        std::getline(file, line, ',');
        std::string name = line;

        std::getline(file, line, ',');
        float posX = std::stof(line);

        std::getline(file, line, ',');
        float posY = std::stof(line);

        std::getline(file, line, ',');
        float rotation = std::stof(line);

        std::getline(file, line, ',');
        float scaleX = std::stof(line);

        std::getline(file, line, ',');
        float scaleY = std::stof(line);

        DrawableObject* obj = nullptr;

        if (type == "ColliderObject") {
            std::getline(file, line, ',');
            float width = std::stof(line);

            std::getline(file, line, ',');
            float height = std::stof(line);

            std::getline(file, line, ',');
            float cPosX = std::stof(line);

            std::getline(file, line, ',');
            float cPosY = std::stof(line);

            std::getline(file, line, ',');
            float cRotation = std::stof(line);

            std::getline(file, line, ',');
            float cScaleX = std::stof(line);

            std::getline(file, line);
            float cScaleY = std::stof(line);

            obj = new ColliderObject();

            obj->getColliderComponent()->setDimension(width, height);
            obj->getColliderComponent()->getTransform().setPosition(cPosX, cPosY);
            obj->getColliderComponent()->getTransform().setRotation(cRotation);
            obj->getColliderComponent()->getTransform().setScale(cScaleX, cScaleY);
        }
        else if (type == "LightSource") {
            std::getline(file, line, ',');
            float brightness = std::stof(line);

            std::getline(file, line);
            float maxDistance = std::stof(line);

            obj = new LightSource(brightness, maxDistance);
        }
        else {
            std::cout << "Error reading file" << std::endl;
            break;
        }

        obj->setName(name);
        obj->getTransform().setPosition(posX, posY);
        obj->getTransform().setRotationRad(rotation);
        obj->getTransform().setScale(scaleX, scaleY);

        if (drawOutline) {
            obj->setDrawCollider(true);
        }

        objectsList.emplace_back(obj);
    }

    file.close();
}

void Level::appendEnemyData(std::ofstream& output, EnemyObject* enemy) {
    Zealot* zealot = dynamic_cast<Zealot*>(enemy);
    BlightFlame* bf = dynamic_cast<BlightFlame*>(enemy);

    if (zealot == NULL && bf == NULL) {
        std::cout << "Invalid Enemy Type" << std::endl;
        return;
    }

    if (zealot != NULL) {
        output << "Zealot,";
    }
    else if (bf != NULL) {
        output << "BlightFlame,";
    }

    output << enemy->getName() << ",";
    Transform transform = enemy->getTransform();
    glm::vec3 position = transform.getPosition();
    glm::vec3 scale = transform.getScale();
    output << position.x << "," << position.y << ",";
    output << transform.getRotationRad() << ",";
    output << scale.x << "," << scale.y;

    Collider* col = enemy->getColliderComponent();

    output << ",";

    transform = col->getTransform();
    position = transform.getPosition();
    scale = transform.getScale();
    output << col->getWidth() << "," << col->getHeight() << ",";
    output << position.x << "," << position.y << ",";
    output << transform.getRotationRad() << ",";
    output << scale.x << "," << scale.y;

    output << "\n";
}
void Level::readEnemyData(std::ifstream& file, std::string type, std::list<DrawableObject*>& objectsList, bool setDrawCollider) {
    std::string line;
    std::getline(file, line, ',');
    std::string name = line;

    std::getline(file, line, ',');
    float posX = std::stof(line);

    std::getline(file, line, ',');
    float posY = std::stof(line);

    std::getline(file, line, ',');
    float rotation = std::stof(line);

    std::getline(file, line, ',');
    float scaleX = std::stof(line);

    std::getline(file, line, ',');
    float scaleY = std::stof(line);

    DrawableObject* obj = nullptr;

    std::getline(file, line, ',');
    float width = std::stof(line);

    std::getline(file, line, ',');
    float height = std::stof(line);

    std::getline(file, line, ',');
    float cPosX = std::stof(line);

    std::getline(file, line, ',');
    float cPosY = std::stof(line);

    std::getline(file, line, ',');
    float cRotation = std::stof(line);

    std::getline(file, line, ',');
    float cScaleX = std::stof(line);

    std::getline(file, line);
    float cScaleY = std::stof(line);

    EnemyObject* enemy = nullptr;

    if (type == "Zealot") {
        enemy = new Zealot(DefaultEnemyStat::ZEALOT_INFO);
    }
    else if (type == "BlightFlame") {
        enemy = new BlightFlame(DefaultEnemyStat::BLIGHT_FLAME_INFO);
    }
    else {
        std::cout << "Invalid Type When Reading Enemy Data" << std::endl;
        return;
    }

    enemy->getTransform().setPosition(posX, posY);
    enemy->getTransform().setRotationRad(rotation);
    enemy->getTransform().setScale(scaleX, scaleY);

    enemy->getColliderComponent()->setDimension(width, height);
    enemy->getColliderComponent()->getTransform().setPosition(cPosX, cPosY);
    enemy->getColliderComponent()->getTransform().setRotation(cRotation);
    enemy->getColliderComponent()->getTransform().setScale(cScaleX, cScaleY);

    objectsList.emplace_back(enemy);
    enemy->start(objectsList);

    if (setDrawCollider) {
        enemy->setDrawCollider(true);
    }
}

void Level::addLoadingScreen() {
	blackLoadingScreen = new SimpleObject();
	blackLoadingScreen->setName("BlackLoadingScreen");
	blackLoadingScreen->getTransform().setPosition(0.0f, 0.0f);
	blackLoadingScreen->getTransform().setScale(2000.0f, 2000.0f);
	blackLoadingScreen->setDrawCollider(false);
	blackLoadingScreen->setColor(0.0f, 0.0f, 0.0f);
	objectsList.emplace_back(blackLoadingScreen);
}

void Level::removeLoadingScreen() {
	objectsList.remove(blackLoadingScreen);
	blackLoadingScreen = nullptr;
}

void Level::LoadContent() {

}