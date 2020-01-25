//
//  GAME228.hpp
//  NekoUI
//
//  Created by Никита Исаенко on 25.01.2020.
//  Copyright © 2020 Melancholy Hill. All rights reserved.
//

#ifndef GAME228_hpp
#define GAME228_hpp

#include <iostream>

#include "../../minEH.hpp"
using namespace ns;
using namespace std;

#include "Vasya.hpp"

struct GAME228 : Component
{
    sf::RectangleShape shape; // Наш задний фон
    sf::Text livesText; // Текст, отображающий жизни и ВЫ ПАГИБЛИ
    
    int lives{ 3 }; // Жизни
    
    sf::Sprite sprite; // Спрайт персонажа
    float x{ 0 }, y{ 0 }; // Координаты персонажа (никак со спрайтом не связанные, мы в логике кода ЗАСТАВЛЯЕМ спрайт setPosition на эти координаты
    bool D{ false }, A{ false }, S{ false }, W{ false }; // Флажки, что нажата клавиша D, A, S или W
    
    Vasya vasya; // Вася (тортик)
    
    void Init() override
    {
        shape.setFillColor(sf::Color::White); // Цвет настроения(фона) - белый
        
        livesText.setFont(*fc::GetFont(L"Arial.ttf")); // Шрифт текста жизней = Arial.ttf
        livesText.setString(L"ЖЫЗНИ: " + to_wstring(lives)); // Строка по умолчанию - ЖЫЗНИ: 3
        livesText.setFillColor({0, 0, 0}); // Цвет текста = чёрный, т.к. цвет настроения(фона) - белый
        
        vasya.ASDAS(); // Вызываем функцию у Васи, которая внутри него загрузит там спрайт тортика
        
        sprite.setTexture(*ic::LoadTexture(L"Data/Neko/Chibi/body.png")); // Загружаем спрайт кошечки
        sprite.setOrigin(sprite.getLocalBounds().width/2, sprite.getLocalBounds().height/2); // Устанавливаем отсчётную точку системы координат кошечки, т.е. ту точку, относительно которой устанавливаем положение
        // !!! Обязательно юзаем getLOCALBounds при установке setOrigin, для остальных функций лучше юзать getGLOBALBounds
        
        cout << "asdasd" << endl;
    }
    void Destroy() override
    {
        ic::DeleteImage(L"Data/Neko/Chibi/body.png"); // Не забываем очистить загруженные текстуры
        // К слову, Вася тут не выгружает свои текстуры, а надо бы: ic::DeleteImage(L"Data/Items/Cake.png");
        cout << "dajkshdjkaw" << endl;
    }
    void Update(const sf::Time& elapsedTime) override
    {
        if (lives <= 0) return; // Если нет жизней, то код ниже не выполняется
        
        bool updatePosition{ D || A || W || S }; // Флаг о том, что нажата одна из клавиш: WASD
        if (D) x += 400 * elapsedTime.asSeconds(); // Если нажата D, то перемещаемся вправо со скоростью 400 пикселей в секунду
        if (A) x -= 400 * elapsedTime.asSeconds(); // Если нажата A, то перемещаемся влево со скоростью 400 пикселей в секунду
        if (W) y -= 400 * elapsedTime.asSeconds(); // Если нажата W, то перемещаемся вверх со скоростью 400 пикселей в секунду
        if (S) y += 400 * elapsedTime.asSeconds(); // Если нажата S, то перемещаемся вниз со скоростью 400 пикселей в секунду
        if (updatePosition) // Если флаг о том, что нажата одна из WASD клавиш, истинен
        {
            sprite.setPosition(x*gs::scale, y*gs::scale); // Обновляем положение спрайта, ТЕМ САМЫМ ЗАСТАВЛЯЯ спрайт соответствовать координатам
            // Обратите внимание, что я использую *gs::scale - это потому, что положение персонажа на экране ЗАВИСИТ от размеров экрана
            vasya.sprite.setPosition(x*gs::scale, y*gs::scale); // Обновляем положение тортика (можно сделать функцию внутри Васи, которая будет делать это же)
            if (x*gs::scale > gs::width/2) Jecka(); // Если мы дальше середины экрана, то Jecka()
        }
    }
    void PollEvent(sf::Event& event) override
    {
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Key::Space) // Если событие = НажатиеКлавиши И код клавиши = пробел
        {
            --lives; // Уменьшаем жизни любым способом: lives = lives - 1; lives -= 1; lives--; --lives; ...
            if (lives <= 0) livesText.setString(L"ВЫ ПАГИБЛИ"); // Если жизни = 0 или меньше, то ВЫ ПАГИБЛИ
            else livesText.setString(L"ЖЫЗНИ: " + to_wstring(lives)); // ... иначе ЖЫЗНИ: ЖЫЗНИ
            livesText.setPosition(gs::width/2 - livesText.getGlobalBounds().width/2, 0); // Обновляем положение текста с жизнями, т.к. надпись могла измениться и испортить выравнивание по середине!
        }
        
        // Флаги движения
        // (на самом деле лучше юзать один if (event.type == sf::Event::KeyPressed) { и в нём писать ЕСЛИ_КОД_РАВЕН_БЛАБЛА, ТО } и так же с Released
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Key::A) A = true;
        else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Key::D) D = true;
        else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Key::W) W = true;
        else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Key::S) S = true;
        else if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Key::A) A = false;
        else if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Key::D) D = false;
        else if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Key::W) W = false;
        else if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Key::S) S = false;
    }
    void Resize(unsigned int width, unsigned int height) override
    {
        shape.setSize({(float)width, (float)height}); // Задний фон должен быть размером окно
        
        livesText.setCharacterSize(50 * gs::scale); // Размер шрифта ЗАВИСИТ от размеров окна
        livesText.setPosition(width/2 - livesText.getGlobalBounds().width/2, 0); // Обновляем положение, равное середина экрана пополам минус половина ширины текста, чтоб текст был ровно посередине
        
        sprite.setScale(0.2*gs::scale, 0.2*gs::scale); // Обновляем размер спрайта персонажа
        sprite.setPosition(x*gs::scale, y*gs::scale); // ... и его положение, зависящее от размеров экрана, как сказано выше
        
        vasya.sprite.setScale(gs::scale, gs::scale); // и так же с Васей (тортиком)
        vasya.sprite.setPosition(x*gs::scale, y*gs::scale); // Можно это в функцию создать внутри Васи и туда запихнуть, так даже красивше будет
    }
    void Draw(sf::RenderWindow* window) override
    {
        // Рисуем ВСЁ
        window->draw(shape);
        window->draw(sprite);
        window->draw(livesText);
        
        vasya.Draw();
    }
    void Jecka()
    {
        // Обновляем текст и обязательно его положиние, т.к. текст-то изменился, а с ним изменилась и его ширина, т.е. сбилось выравнивание ровно посередине
        livesText.setString(L"Я БЛЯ БЕССМЕРТНЫЙ, Я ЖЕ ЖЕКА");
        livesText.setPosition(gs::width/2 - livesText.getGlobalBounds().width/2, 0);
    }
};

#endif /* GAME228_hpp */
