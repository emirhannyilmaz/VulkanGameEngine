#include "collada_loader.hpp"

MeshData ColladaLoader::LoadMesh(const std::string& fileName) {
    MeshData meshData{};

    std::ifstream file(fileName.c_str());
    std::string line;
    bool geometries = false;

    while (getline(file, line)) {
        if (line.find("<library_geometries>") != std::string::npos) {
            geometries = true;
            continue;
        }

        if (line.find("</library_geometries>") != std::string::npos) {
            geometries = false;
            continue;
        }

        if (geometries) {
            if (line.find("positions-array") != std::string::npos) {
                std::vector<glm::vec3> positions;
                size_t offset = line.find(">");
                size_t length = line.find("</") - offset;
                std::string characters = line.substr(offset, length);
                std::array<std::string, 3> position{};
                int i = 0;
                for (auto character : characters) {
                    if (character == ' ') {
                        if (i == 2) {
                            positions.push_back(glm::vec3(std::stof(position[0]), std::stof(position[1]), std::stof(position[2])));
                            i = 0;
                            position[0] = "";
                            position[1] = "";
                            position[2] = "";
                            continue;
                        }
                        i++;
                        continue;
                    }

                    position[i] += character;
                }
            }
        }
    }

    return meshData;
}
