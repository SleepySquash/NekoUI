//
//  Shader.hpp
//  NekoUI
//
//  Created by Никита Исаенко on 14/08/2019.
//  Copyright © 2019 Melancholy Hill. All rights reserved.
//

#ifndef Shader_hpp
#define Shader_hpp

#include <iostream>
#include <unordered_map>

#include <SFML/Main.hpp>
#include <SFML/Graphics.hpp>

#include "../../Essentials/ResourcePath.hpp"
#include "../../Essentials/Base.hpp"

using ns::base::utf8;
using ns::base::utf16;
using std::cout;
using std::endl;

namespace ns
{
    struct ShaderCollectorObject
    {
        sf::Shader* shader{ nullptr };
        int usage{ 0 };
        
        ShaderCollectorObject(sf::Shader* shader = nullptr, int usage = 0) : shader(shader), usage(usage) { }
    };
    class ShaderCollector
    {
    private:
        ShaderCollector() { }
    public:
        static std::unordered_map<std::string, ShaderCollectorObject> shaders;
        
        static sf::Shader* LoadShader(const std::string& name);
        static void DeleteShader(const std::string& name);
    };
    
    typedef ShaderCollector shaderc;
}

#endif /* Shader_hpp */
