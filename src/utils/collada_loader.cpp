#include "collada_loader.hpp"

MeshData ColladaLoader::LoadMesh(const std::string& fileName) {
    MeshData meshData{};
    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> textureCoordinates;

    std::ifstream file(fileName.c_str());
    std::string line;

    while (getline(file, line)) {
        if (line.find("positions-array") != std::string::npos && line.find("accessor") == std::string::npos) {
            size_t offset = line.find(">") + 1;
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
            positions.push_back(glm::vec3(std::stof(position[0]), std::stof(position[1]), std::stof(position[2])));
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
                        normals.push_back(glm::vec3(std::stof(normal[0]), std::stof(normal[1]), std::stof(normal[2])));
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
            normals.push_back(glm::vec3(std::stof(normal[0]), std::stof(normal[1]), std::stof(normal[2])));
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
            std::unordered_map<EntityVertex, uint32_t> uniqueVertices{};
            size_t offset = line.find(">") + 1;
            size_t length = line.find("</") - offset;
            std::string characters = line.substr(offset, length);
            std::array<std::string, 4> values{};
            int i = 0;
            for (auto character : characters) {
                if (character == ' ') {
                    if (i == 3) {
                        EntityVertex vertex{};
                        vertex.position = positions[std::stoi(values[0])];
                        vertex.normal = normals[std::stoi(values[1])];
                        vertex.textureCoordinates = textureCoordinates[std::stoi(values[2])];
                        if (uniqueVertices.count(vertex) == 0) {
                            uniqueVertices[vertex] = static_cast<uint32_t>(meshData.vertices.size());
                            meshData.vertices.push_back(vertex);
                        }
                        meshData.indices.insert(meshData.indices.begin(), uniqueVertices[vertex]);
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
            EntityVertex vertex{};
            vertex.position = positions[std::stoi(values[0])];
            vertex.normal = normals[std::stoi(values[1])];
            vertex.textureCoordinates = textureCoordinates[std::stoi(values[2])];
            if (uniqueVertices.count(vertex) == 0) {
                uniqueVertices[vertex] = static_cast<uint32_t>(meshData.vertices.size());
                meshData.vertices.push_back(vertex);
            }
            meshData.indices.insert(meshData.indices.begin(), uniqueVertices[vertex]);
        }
    }

    return meshData;
}

AnimatedMeshData ColladaLoader::LoadAnimatedMesh(const std::string& fileName) {
    AnimatedMeshData meshData{};
    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> textureCoordinates;
    std::vector<float> jointWeights;
    std::vector<int> jointsAffectedCount;
    std::unordered_map<int, std::unordered_map<glm::ivec3, glm::vec3>> jointsThatAffectVertex;

    std::ifstream file(fileName.c_str());
    std::string line;

    bool enteredVertexWeights = false;
    while (getline(file, line)) {
        if (line.find("weights-array") != std::string::npos && line.find("accessor") == std::string::npos) {
            size_t offset = line.find(">") + 1;
            size_t length = line.find("</") - offset;
            std::string characters = line.substr(offset, length);
            std::string weight;
            for (auto character : characters) {
                if (character == ' ') {
                    jointWeights.push_back(std::stof(weight));
                    weight = "";
                    continue;
                }

                weight += character;
            }
            jointWeights.push_back(std::stof(weight));
        }

        if (line.find("<vertex_weights") != std::string::npos) {
            enteredVertexWeights = true;
        }

        if (enteredVertexWeights) {
            if (line.find("<vcount>") != std::string::npos) {
                size_t offset = line.find(">") + 1;
                size_t length = line.find("</") - offset;
                std::string characters = line.substr(offset, length);
                for (auto character : characters) {
                    if (character == ' ') {
                        continue;
                    }

                    jointsAffectedCount.push_back(std::stoi(std::string(1, character)));
                }
            }

            if (line.find("<v>") != std::string::npos) {
                size_t offset = line.find(">") + 1;
                size_t length = line.find("</") - offset;
                std::string characters = line.substr(offset, length);
                std::vector<std::string> values;
                int stopPoint = jointsAffectedCount[0] * 2 - 1;
                int i = 0;
                int j = 0;
                values.push_back("");
                for (auto character : characters) {
                    if (character == ' ') {
                        if (i == stopPoint) {
                            std::vector<int> valuesInt;
                            for (std::string value : values) {
                                valuesInt.push_back(std::stoi(value));
                            }
                            std::unordered_map<glm::ivec3, glm::vec3> map{};
                            if (jointsAffectedCount[j] > 3) {
                                std::vector<float> weightsToLimit{};
                                for (int k = 0; k < jointsAffectedCount[j]; k++) {
                                    weightsToLimit.push_back(jointWeights[valuesInt[k * 2 + 1]]);
                                }
                                std::sort(weightsToLimit.begin(), weightsToLimit.end(), std::greater<float>());
                                glm::vec3 weights = glm::vec3(weightsToLimit[0], weightsToLimit[1], weightsToLimit[2]);
                                int firstWeightIndex = std::find(jointWeights.data(), jointWeights.data() + jointWeights.size(), weights.x) - jointWeights.data();
                                int secondWeightIndex = std::find(jointWeights.data(), jointWeights.data() + jointWeights.size(), weights.y) - jointWeights.data();
                                int thirdWeightIndex = std::find(jointWeights.data(), jointWeights.data() + jointWeights.size(), weights.z) - jointWeights.data();
                                int firstJointID = std::find(valuesInt.data(), valuesInt.data() + valuesInt.size(), firstWeightIndex) - valuesInt.data();
                                int secondJointID = std::find(valuesInt.data(), valuesInt.data() + valuesInt.size(), secondWeightIndex) - valuesInt.data();
                                int thirdJointID = std::find(valuesInt.data(), valuesInt.data() + valuesInt.size(), thirdWeightIndex) - valuesInt.data();
                                glm::ivec3 IDs = glm::ivec3(valuesInt[firstJointID - 1], valuesInt[secondJointID - 1], valuesInt[thirdJointID - 1]);
                                weights = glm::normalize(weights);
                                map[IDs] = weights;
                            } else {
                                glm::vec3 weights{};
                                glm::ivec3 IDs{};
                                for (int k = 0; k < jointsAffectedCount[j]; k++) {
                                    IDs[k] = valuesInt[k * 2];
                                    weights[k] = jointWeights[valuesInt[k * 2 + 1]];
                                }
                                map[IDs] = weights;
                            }
                            jointsThatAffectVertex[j] = map;
                            values.clear();
                            values.push_back("");
                            i = 0;
                            j++;
                            stopPoint = jointsAffectedCount[j] * 2 - 1;
                            continue;
                        } else {
                            i++;
                            values.push_back("");
                            continue;
                        }
                    }

                    values[i] += character;
                }
                std::vector<int> valuesInt;
                for (std::string value : values) {
                    valuesInt.push_back(std::stoi(value));
                }
                std::unordered_map<glm::ivec3, glm::vec3> map{};
                if (jointsAffectedCount[j] > 3) {
                    std::vector<float> weightsToLimit{};
                    for (int k = 0; k < jointsAffectedCount[j]; k++) {
                        weightsToLimit.push_back(jointWeights[valuesInt[k * 2 + 1]]);
                    }
                    std::sort(weightsToLimit.begin(), weightsToLimit.end(), std::greater<float>());
                    glm::vec3 weights = glm::vec3(weightsToLimit[0], weightsToLimit[1], weightsToLimit[2]);
                    int firstWeightIndex = std::find(jointWeights.data(), jointWeights.data() + jointWeights.size(), weights.x) - jointWeights.data();
                    int secondWeightIndex = std::find(jointWeights.data(), jointWeights.data() + jointWeights.size(), weights.y) - jointWeights.data();
                    int thirdWeightIndex = std::find(jointWeights.data(), jointWeights.data() + jointWeights.size(), weights.z) - jointWeights.data();
                    int firstJointID = std::find(valuesInt.data(), valuesInt.data() + valuesInt.size(), firstWeightIndex) - valuesInt.data();
                    int secondJointID = std::find(valuesInt.data(), valuesInt.data() + valuesInt.size(), secondWeightIndex) - valuesInt.data();
                    int thirdJointID = std::find(valuesInt.data(), valuesInt.data() + valuesInt.size(), thirdWeightIndex) - valuesInt.data();
                    glm::ivec3 IDs = glm::ivec3(valuesInt[firstJointID - 1], valuesInt[secondJointID - 1], valuesInt[thirdJointID - 1]);
                    weights = glm::normalize(weights);
                    map[IDs] = weights;
                } else {
                    glm::vec3 weights{};
                    glm::ivec3 IDs{};
                    for (int k = 0; k < jointsAffectedCount[j]; k++) {
                        IDs[k] = valuesInt[k * 2];
                        weights[k] = jointWeights[valuesInt[k * 2 + 1]];
                    }

                    if (jointsAffectedCount[j] == 1) {
                        IDs.y = 0;
                        weights.y = 0.0f;
                        IDs.z = 0;
                        weights.z = 0.0f;
                    } else if (jointsAffectedCount[j] == 2) {
                        IDs.z = 0;
                        weights.z = 0.0f;
                    }
                    map[IDs] = weights;
                }
                jointsThatAffectVertex[j] = map;
            }
        }
    }

    file.clear();
    file.seekg(0);

    while (getline(file, line)) {
        if (line.find("positions-array") != std::string::npos && line.find("accessor") == std::string::npos) {
            size_t offset = line.find(">") + 1;
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
            positions.push_back(glm::vec3(std::stof(position[0]), std::stof(position[1]), std::stof(position[2])));
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
                        normals.push_back(glm::vec3(std::stof(normal[0]), std::stof(normal[1]), std::stof(normal[2])));
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
            normals.push_back(glm::vec3(std::stof(normal[0]), std::stof(normal[1]), std::stof(normal[2])));
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
            std::unordered_map<AnimatedEntityVertex, uint32_t> uniqueVertices{};
            size_t offset = line.find(">") + 1;
            size_t length = line.find("</") - offset;
            std::string characters = line.substr(offset, length);
            std::array<std::string, 4> values{};
            int i = 0;
            for (auto character : characters) {
                if (character == ' ') {
                    if (i == 3) {
                        AnimatedEntityVertex vertex{};
                        vertex.position = positions[std::stoi(values[0])];
                        vertex.normal = normals[std::stoi(values[1])];
                        vertex.textureCoordinates = textureCoordinates[std::stoi(values[2])];
                        vertex.jointIDs = jointsThatAffectVertex[std::stoi(values[0])].begin()->first;
                        vertex.jointWeights = jointsThatAffectVertex[std::stoi(values[0])].begin()->second;
                        if (uniqueVertices.count(vertex) == 0) {
                            uniqueVertices[vertex] = static_cast<uint32_t>(meshData.vertices.size());
                            meshData.vertices.push_back(vertex);
                        }
                        meshData.indices.insert(meshData.indices.begin(), uniqueVertices[vertex]);
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
            AnimatedEntityVertex vertex{};
            vertex.position = positions[std::stoi(values[0])];
            vertex.normal = normals[std::stoi(values[1])];
            vertex.textureCoordinates = textureCoordinates[std::stoi(values[2])];
            vertex.jointIDs = jointsThatAffectVertex[std::stoi(values[0])].begin()->first;
            vertex.jointWeights = jointsThatAffectVertex[std::stoi(values[0])].begin()->second;
            if (uniqueVertices.count(vertex) == 0) {
                uniqueVertices[vertex] = static_cast<uint32_t>(meshData.vertices.size());
                meshData.vertices.push_back(vertex);
            }
            meshData.indices.insert(meshData.indices.begin(), uniqueVertices[vertex]);
        }
    }

    return meshData;
}

AnimationData ColladaLoader::LoadAnimation(const std::string& fileName) {
    AnimationData animationData{};
    std::vector<std::string> jointNames{};
    std::vector<std::string> jointHierarchy{};
    std::vector<Keyframe> keyframes{};
    std::vector<JointTransform> jointTransforms{};
    float animationLength;
    Joint* rootJoint = nullptr;
    bool insideArmature = false;

    std::ifstream file(fileName.c_str());
    std::string line;

    while (getline(file, line)) {
        if (keyframes.empty()) {
            if (line.find("input-array") != std::string::npos && line.find("accessor") == std::string::npos) {
                size_t offset = line.find(">") + 1;
                size_t length = line.find("</") - offset;
                std::string characters = line.substr(offset, length);
                std::vector<std::string> timestamps;
                timestamps.push_back("");
                int i = 0;

                for (auto character : characters) {
                    if (character == ' ') {
                        Keyframe keyframe{std::stof(timestamps[i])};
                        keyframes.push_back(keyframe);
                        i++;
                        timestamps.push_back("");
                        continue;
                    }

                    timestamps[i] += character;
                }
                Keyframe keyframe{std::stof(timestamps[i])};
                keyframes.push_back(keyframe);

                animationLength = std::stof(timestamps.back());
            }
        }

        if (line.find("output-array") != std::string::npos && line.find("accessor") == std::string::npos) {
            jointTransforms.clear();
            size_t offset = line.find(">") + 1;
            size_t length = line.find("</") - offset;
            std::string characters = line.substr(offset, length);
            std::vector<std::string> matrixRows;
            matrixRows.push_back("");
            int i = 0;

            for (auto character : characters) {
                if (character == ' ') {
                    i++;
                    if (i == 16) {
                        std::array<float, 16> matrixArray {
                            std::stof(matrixRows[0]), std::stof(matrixRows[1]), std::stof(matrixRows[2]), std::stof(matrixRows[3]),
                            std::stof(matrixRows[4]), std::stof(matrixRows[5]), std::stof(matrixRows[6]), std::stof(matrixRows[7]),
                            std::stof(matrixRows[8]), std::stof(matrixRows[9]), std::stof(matrixRows[10]), std::stof(matrixRows[11]),
                            std::stof(matrixRows[12]), std::stof(matrixRows[13]), std::stof(matrixRows[14]), std::stof(matrixRows[15])
                        };
                        glm::mat4 matrix = glm::make_mat4(matrixArray.data());
                        matrix = glm::transpose(matrix);
                        JointTransform jointTransform{glm::vec3(matrix[3][0], matrix[3][1], matrix[3][2]), glm::quat_cast(matrix)};
                        jointTransforms.push_back(jointTransform);
                        i = 0;
                        matrixRows.clear();
                    }
                    matrixRows.push_back("");
                    continue;
                }

                matrixRows[i] += character;
            }
            std::array<float, 16> matrixArray {
                std::stof(matrixRows[0]), std::stof(matrixRows[1]), std::stof(matrixRows[2]), std::stof(matrixRows[3]),
                std::stof(matrixRows[4]), std::stof(matrixRows[5]), std::stof(matrixRows[6]), std::stof(matrixRows[7]),
                std::stof(matrixRows[8]), std::stof(matrixRows[9]), std::stof(matrixRows[10]), std::stof(matrixRows[11]),
                std::stof(matrixRows[12]), std::stof(matrixRows[13]), std::stof(matrixRows[14]), std::stof(matrixRows[15])
            };
            glm::mat4 matrix = glm::make_mat4(matrixArray.data());
            matrix = glm::transpose(matrix);
            JointTransform jointTransform{glm::vec3(matrix[3][0], matrix[3][1], matrix[3][2]), glm::quat_cast(matrix)};
            jointTransforms.push_back(jointTransform);
        }

        if (line.find("<channel") != std::string::npos) {
            size_t offset = line.find("target=") + 8;
            size_t length = line.find("/") - offset;
            std::string jointName = line.substr(offset, length);
            
            for (int i = 0; i < keyframes.size(); i++) {
                keyframes[i].pose.insert({jointName, jointTransforms[i]});
            }
        }

        if (line.find("joints-array") != std::string::npos && line.find("accessor") == std::string::npos) {
            size_t offset = line.find(">") + 1;
            size_t length = line.find("</") - offset;
            std::string characters = line.substr(offset, length);
            jointNames.push_back("");
            int i = 0;

            for (auto character : characters) {
                if (character == ' ') {
                    i++;
                    jointNames.push_back("");
                    continue;
                }

                jointNames[i] += character;
            }
        }

        if (line.find("id=\"Armature\"") != std::string::npos) {
            insideArmature = true;
        }

        if (line.find("type=\"JOINT\"") != std::string::npos) {
            size_t offset = line.find("id") + 4;
            size_t length = line.find("\" name=\"") - offset;
            std::string jointName = line.substr(offset, length);
            jointHierarchy.push_back(jointName);
        }

        if (line.find("<matrix sid=\"transform\">") != std::string::npos) {
            size_t offset = line.find("\"transform\"") + 12;
            size_t length = line.find("</") - offset;
            std::string characters = line.substr(offset, length);
            std::array<std::string, 16> matrixRows;
            int i = 0;

            for (auto character : characters) {
                if (character == ' ') {
                    i++;
                    continue;
                }

                matrixRows[i] += character;
            }

            int index;
            for (int i = 0; i < jointNames.size(); i++) {
                if (jointNames[i] == jointHierarchy.back()) {
                    index = i;
                    break;
                }
            }

            std::array<float, 16> matrixArray {
                std::stof(matrixRows[0]), std::stof(matrixRows[1]), std::stof(matrixRows[2]), std::stof(matrixRows[3]),
                std::stof(matrixRows[4]), std::stof(matrixRows[5]), std::stof(matrixRows[6]), std::stof(matrixRows[7]),
                std::stof(matrixRows[8]), std::stof(matrixRows[9]), std::stof(matrixRows[10]), std::stof(matrixRows[11]),
                std::stof(matrixRows[12]), std::stof(matrixRows[13]), std::stof(matrixRows[14]), std::stof(matrixRows[15])
            };

            glm::mat4 matrix = glm::make_mat4(matrixArray.data());
            matrix = glm::transpose(matrix);

            Joint* joint = new Joint(index, jointHierarchy.back(), matrix);

            if (jointHierarchy.size() == 1) {
                rootJoint = joint;
            } else if(jointHierarchy.size() == 2) {
                rootJoint->children.push_back(joint);
            } else {
                AddJointToChildren(rootJoint, jointHierarchy[jointHierarchy.size() - 2], joint);
            }
        }

        if (line.find("</node>") != std::string::npos && insideArmature) {
            if (jointHierarchy.size() == 1) {
                insideArmature = false;
            } else {
                jointHierarchy.pop_back();
            }
        }
    }

    animationData.animation = Animation{animationLength, keyframes};
    animationData.rootJoint = rootJoint;
    animationData.jointCount = jointNames.size();

    return animationData;
}

void ColladaLoader::AddJointToChildren(Joint* rootJoint, std::string parentJointName, Joint* newJoint) {
    for (Joint* childJoint : rootJoint->children) {
        if (childJoint->name == parentJointName) {
            childJoint->children.push_back(newJoint);
            return;
        }

        AddJointToChildren(childJoint, parentJointName, newJoint);
    }
}
