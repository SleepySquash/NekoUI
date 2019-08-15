//
//  Interest.cpp
//  NekoUI
//
//  Created by Никита Исаенко on 10/08/2019.
//  Copyright © 2019 Melancholy Hill. All rights reserved.
//

#include "Interest.hpp"

namespace NekoUI
{
    Interest::Interest(const std::string& name) : name(name), display(utf16(name)) { }
}
