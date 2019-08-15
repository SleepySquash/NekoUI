//
//  Shader.cpp
//  NekoUI
//
//  Created by Никита Исаенко on 14/08/2019.
//  Copyright © 2019 Melancholy Hill. All rights reserved.
//

#include "Shader.hpp"

namespace ns
{
    sf::Shader* shaderc::LoadShader(const std::string& name)
    {
        if (!sf::Shader::isAvailable()) return nullptr;
        
        auto it = shaders.find(name);
        if (it != shaders.end()) { ++(*it).second.usage; return (*it).second.shader; }
        if (base::FileExists(utf16(resourcePath()) + L"Data/Shaders/" + utf16(name) + L".shader"))
        {
            sf::Shader* shader = new sf::Shader();
            if (shader->loadFromFile(resourcePath() + "Data/Shaders/" + name + ".shader", sf::Shader::Fragment))
            {
                shaders[name] = ShaderCollectorObject(shader, 1);
                return shader;
            }
        }
        
        return nullptr;
    }
    void shaderc::DeleteShader(const std::string& name)
    {
        auto it = shaders.find(name);
        if (it != shaders.end()) { --(*it).second.usage; if ((*it).second.usage <= 0) { delete (*it).second.shader; shaders.erase(it); } }
    }
    
    std::unordered_map<std::string, ShaderCollectorObject> shaderc::shaders;
}
