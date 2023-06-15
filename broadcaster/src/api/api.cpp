#include "api/api.h"


void API::Api::UpdateObjects(Graphics::Camera* Camera, std::array<Object*, 10> Objects, Graphics::Renderer* Renderer, Graphics::Texture* Texture)
{
    cpr::Response r = cpr::Get(cpr::Url{"http://api/scene/0"});
    json data = json::parse(r.text);

    json cameraPosition = data["Camera"][Camera->ID]["Position"];
    json cameraOrientation = data["Camera"][Camera->ID]["Orientation"];

    Camera->SetPositionOrientation(
        glm::vec3(cameraPosition["x"], cameraPosition["y"], cameraPosition["z"]),
        glm::vec3(cameraOrientation["x"], cameraOrientation["y"], cameraOrientation["z"])
    );

    for (size_t i = 0; i < data["Scene"].size(); i++)
    {
        json jsonObject = data["Scene"][i];
        glm::vec3 position = glm::vec3(jsonObject["Position"]["x"], jsonObject["Position"]["y"], jsonObject["Position"]["z"]);
        glm::vec3 orientation = glm::vec3(jsonObject["Orientation"]["x"], jsonObject["Orientation"]["y"], jsonObject["Orientation"]["z"]);

        if (Objects[i] == nullptr)
        {
            glm::vec3 halfSize = glm::vec3(jsonObject["HalfSize"]["x"], jsonObject["HalfSize"]["y"], jsonObject["HalfSize"]["z"]);
            Cube MyCube = Cube(position, orientation, halfSize);
            Objects[i] = &MyCube;
            Renderer->BindTexture(&MyCube, Texture);
        }
        else
        {
            Objects[i]->SetPositionOrientation(position, orientation);
        }
    }
}
