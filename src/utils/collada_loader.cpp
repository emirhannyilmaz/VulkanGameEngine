#include "collada_loader.hpp"

MeshData ColladaLoader::LoadMesh(const std::string& fileName) {
    MeshData meshData{};
    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> textureCoordinates;

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
            if (line.find("positions-array") != std::string::npos && line.find("accessor") == std::string::npos) {
                size_t offset = line.find(">") + 1;
                size_t length = line.find("</") - offset;
                std::string characters = line.substr(offset, length);
                std::array<std::string, 3> position{};
                int i = 0;
                for (auto character : characters) {
                    if (character == ' ') {
                        if (i == 2) {
                            positions.push_back(glm::vec3(std::stof(position[0]), std::stof(position[1]) * -1.0f, std::stof(position[2])));
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
                positions.push_back(glm::vec3(std::stof(position[0]), std::stof(position[1]) * -1.0f, std::stof(position[2])));
            }
            if (line.find("normals-array") != std::string::npos && line.find("accessor") == std::string::npos) {
                size_t offset = line.find(">") + 1;
                size_t length = line.find("</") - offset;
                std::string characters = line.substr(offset, length);
                std::array<std::string, 3> normal{};
                int i = 0;
                for (auto character : characters) {
                    if (character == ' ') {
                        if (i == 2) {
                            normals.push_back(glm::vec3(std::stof(normal[0]), std::stof(normal[1]) * -1.0f, std::stof(normal[2])));
                            i = 0;
                            normal[0] = "";
                            normal[1] = "";
                            normal[2] = "";
                            continue;
                        }
                        i++;
                        continue;
                    }

                    normal[i] += character;
                }
                normals.push_back(glm::vec3(std::stof(normal[0]), std::stof(normal[1]) * -1.0f, std::stof(normal[2])));
            }
            if (line.find("map-0-array") != std::string::npos && line.find("accessor") == std::string::npos) {
                size_t offset = line.find(">") + 1;
                size_t length = line.find("</") - offset;
                std::string characters = line.substr(offset, length);
                std::array<std::string, 2> textureCoordinate{};
                int i = 0;
                for (auto character : characters) {
                    if (character == ' ') {
                        if (i == 1) {
                            textureCoordinates.push_back(glm::vec2(std::stof(textureCoordinate[0]), 1.0f - std::stof(textureCoordinate[1])));
                            i = 0;
                            textureCoordinate[0] = "";
                            textureCoordinate[1] = "";
                            continue;
                        }
                        i++;
                        continue;
                    }

                    textureCoordinate[i] += character;
                }
                textureCoordinates.push_back(glm::vec2(std::stof(textureCoordinate[0]), 1.0f - std::stof(textureCoordinate[1])));
            }
            if (line.find("<p>") != std::string::npos) {
                std::unordered_map<Vertex, uint32_t> uniqueVertices{};
                size_t offset = line.find(">") + 1;
                size_t length = line.find("</") - offset;
                std::string characters = line.substr(offset, length);
                std::array<std::string, 4> values{};
                int i = 0;
                for (auto character : characters) {
                    if (character == ' ') {
                        if (i == 3) {
                            Vertex vertex{};
                            vertex.position = positions[std::stoi(values[0])];
                            vertex.normal = normals[std::stoi(values[1])];
                            vertex.textureCoordinates = textureCoordinates[std::stoi(values[2])];
                            if (uniqueVertices.count(vertex) == 0) {
                                uniqueVertices[vertex] = static_cast<uint32_t>(meshData.vertices.size());
                                meshData.vertices.push_back(vertex);
                            }
                            meshData.indices.push_back(uniqueVertices[vertex]);
                            i = 0;
                            values[0] = "";
                            values[1] = "";
                            values[2] = "";
                            values[3] = "";
                            continue;
                        }
                        i++;
                        continue;
                    }

                    values[i] += character;
                }
                Vertex vertex{};
                vertex.position = positions[std::stoi(values[0])];
                vertex.normal = normals[std::stoi(values[1])];
                vertex.textureCoordinates = textureCoordinates[std::stoi(values[2])];
                if (uniqueVertices.count(vertex) == 0) {
                    uniqueVertices[vertex] = static_cast<uint32_t>(meshData.vertices.size());
                    meshData.vertices.push_back(vertex);
                }
                meshData.indices.push_back(uniqueVertices[vertex]);
            }
        }
    }

    return meshData;
}
