#include "game_level.h"

#include <fstream>
#include <sstream>


void GameLevel::Load(const char *file, unsigned int levelWidth, unsigned int levelHeight)
{
    // 清除上一关的数据
    this->Bricks.clear();
    // 加载关卡文件，从中读取砖块布局和种类设计
    unsigned int tileCode;
    GameLevel level;
    std::string line;
    std::ifstream fstream(file);
    // 就是把文件里的东西读成这个 二维数组
    std::vector<std::vector<unsigned int>> tileData;
    if (fstream)
    {
        while (std::getline(fstream, line)) 
        {
            std::istringstream sstream(line);
            std::vector<unsigned int> row;
            while (sstream >> tileCode) 
                row.push_back(tileCode);
            tileData.push_back(row);
        }
        if (tileData.size() > 0)
            this->init(tileData, levelWidth, levelHeight);
    }
}

void GameLevel::Draw(SpriteRenderer &renderer)
{
    for (BrickObject &tile : this->Bricks)
        if (!tile.Destroyed)
            tile.Draw(renderer);
}

bool GameLevel::IsCompleted()
{
    for (BrickObject &tile : this->Bricks)
        if (!tile.IsSolid && !tile.Destroyed)
            return false;
    return true;
}

void GameLevel::init(std::vector<std::vector<unsigned int>> tileData, unsigned int levelWidth, unsigned int levelHeight)
{
    // 计算每个砖块的长和宽
    unsigned int height = tileData.size();
    unsigned int width = tileData[0].size(); 
    float unit_width = levelWidth / static_cast<float>(width), unit_height = levelHeight / height; 
    // initialize level tiles based on tileData		
    for (unsigned int y = 0; y < height; ++y)
    {
        for (unsigned int x = 0; x < width; ++x)
        {
            // 砖块类型：1 代表不可摧毁，>1 代表可以摧毁，不同的数字拥有不同的颜色，0 代表该位置为空
            if (tileData[y][x] == 1) 
            {
                glm::vec2 pos(unit_width * x, unit_height * y);
                glm::vec2 size(unit_width, unit_height);
                BrickObject brick(pos, size, ResourceManager::GetTexture("block_solid"), glm::vec3(0.8f, 0.8f, 0.7f));
                brick.IsSolid = true;
                this->Bricks.push_back(brick);
            }
            else if (tileData[y][x] > 1)	// non-solid; now determine its color based on level data
            {
                glm::vec3 color = glm::vec3(1.0f); // original: white
                if (tileData[y][x] == 2)
                    color = glm::vec3(0.2f, 0.6f, 1.0f);
                else if (tileData[y][x] == 3)
                    color = glm::vec3(0.0f, 0.7f, 0.0f);
                else if (tileData[y][x] == 4)
                    color = glm::vec3(0.8f, 0.8f, 0.4f);
                else if (tileData[y][x] == 5)
                    color = glm::vec3(1.0f, 0.5f, 0.0f);

                glm::vec2 pos(unit_width * x, unit_height * y);
                glm::vec2 size(unit_width, unit_height);
                this->Bricks.push_back(BrickObject(pos, size, ResourceManager::GetTexture("block"), color));
            }
        }
    }
}