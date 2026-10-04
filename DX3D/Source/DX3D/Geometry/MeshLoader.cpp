#include <DX3D/Geometry/MeshLoader.h>
#include <DX3D/Core/Logger.h>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <vector>

using namespace dx3d;

struct VertexKey
{
    i32 posIndex{ -1 };
    i32 texIndex{ -1 };
    i32 normIndex{ -1 };

    bool operator==(const VertexKey& o) const
    {
        return posIndex == o.posIndex && texIndex == o.texIndex && normIndex == o.normIndex;
    }
};

struct VertexKeyHash
{
    size_t operator()(const VertexKey& k) const
    {
        return (std::hash<i32>()(k.posIndex) ^ (std::hash<i32>()(k.texIndex) << 1)) ^ (std::hash<i32>()(k.normIndex) << 2);
    }
};

MeshPtr MeshLoader::LoadFromOBJ(const std::string& filePath)
{
    std::ifstream file(filePath);
    if (!file.is_open())
    {
        DX3DLogError("Failed to open OBJ file at path: " + filePath);
        return nullptr;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return LoadFromOBJString(buffer.str(), filePath);
}

MeshPtr MeshLoader::LoadFromOBJString(const std::string& objSource, const std::string& meshName)
{
    auto mesh = std::make_shared<Mesh>(meshName);

    std::vector<Vec3> positions;
    std::vector<Vec2> texCoords;
    std::vector<Vec3> normals;

    std::vector<Vertex> finalVertices;
    std::vector<u32> finalIndices;
    std::unordered_map<VertexKey, u32, VertexKeyHash> vertexCache;

    std::stringstream stream(objSource);
    std::string line;

    while (std::getline(stream, line))
    {
        if (line.empty() || line[0] == '#') continue;

        std::stringstream ss(line);
        std::string prefix;
        ss >> prefix;

        if (prefix == "v")
        {
            f32 x, y, z;
            ss >> x >> y >> z;
            positions.push_back({ x, y, z });
        }
        else if (prefix == "vt")
        {
            f32 u, v;
            ss >> u >> v;
            texCoords.push_back({ u, 1.0f - v });
        }
        else if (prefix == "vn")
        {
            f32 x, y, z;
            ss >> x >> y >> z;
            normals.push_back({ x, y, z });
        }
        else if (prefix == "f")
        {
            std::vector<VertexKey> faceKeys;
            std::string faceToken;

            while (ss >> faceToken)
            {
                VertexKey key;
                size_t firstSlash = faceToken.find('/');
                size_t secondSlash = faceToken.rfind('/');

                if (firstSlash == std::string::npos)
                {
                    key.posIndex = std::stoi(faceToken) - 1;
                }
                else if (firstSlash == secondSlash)
                {
                    key.posIndex = std::stoi(faceToken.substr(0, firstSlash)) - 1;
                    key.texIndex = std::stoi(faceToken.substr(firstSlash + 1)) - 1;
                }
                else
                {
                    key.posIndex = std::stoi(faceToken.substr(0, firstSlash)) - 1;
                    if (secondSlash > firstSlash + 1)
                    {
                        key.texIndex = std::stoi(faceToken.substr(firstSlash + 1, secondSlash - firstSlash - 1)) - 1;
                    }
                    key.normIndex = std::stoi(faceToken.substr(secondSlash + 1)) - 1;
                }
                faceKeys.push_back(key);
            }

            for (size_t i = 1; i + 1 < faceKeys.size(); ++i)
            {
                VertexKey triKeys[3] = { faceKeys[0], faceKeys[i], faceKeys[i + 1] };

                for (int k = 0; k < 3; ++k)
                {
                    auto it = vertexCache.find(triKeys[k]);
                    if (it != vertexCache.end())
                    {
                        finalIndices.push_back(it->second);
                    }
                    else
                    {
                        Vertex vert{};
                        if (triKeys[k].posIndex >= 0 && triKeys[k].posIndex < (i32)positions.size())
                            vert.position = positions[triKeys[k].posIndex];

                        if (triKeys[k].texIndex >= 0 && triKeys[k].texIndex < (i32)texCoords.size())
                            vert.texCoord = texCoords[triKeys[k].texIndex];

                        if (triKeys[k].normIndex >= 0 && triKeys[k].normIndex < (i32)normals.size())
                            vert.normal = normals[triKeys[k].normIndex];

                        u32 newIdx = static_cast<u32>(finalVertices.size());
                        finalVertices.push_back(vert);
                        vertexCache[triKeys[k]] = newIdx;
                        finalIndices.push_back(newIdx);
                    }
                }
            }
        }
    }

    mesh->setVertices(finalVertices);
    mesh->setIndices(finalIndices);

    if (normals.empty())
    {
        mesh->calculateNormals();
    }
    mesh->calculateTangents();

    DX3DLogInfo("Loaded OBJ '" + meshName + "' (" + std::to_string(finalVertices.size()) + " vertices, " + std::to_string(finalIndices.size() / 3) + " triangles).");
    return mesh;
}