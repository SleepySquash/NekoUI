//
//  NotificationHolder.hpp
//  NekoUI
//
//  Created by Никита Исаенко on 25.01.2020.
//  Copyright © 2020 Melancholy Hill. All rights reserved.
//

#ifndef NotificationHolder_hpp
#define NotificationHolder_hpp

#include <iostream>

namespace ns
{
    struct NotificationHolder
    {
        std::wstring caption, text;
        
        NotificationHolder(const std::wstring& caption = L"", const std::wstring& text = L"");
    };
}

#endif /* NotificationHolder_hpp */
