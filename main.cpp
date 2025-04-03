// library
#include <SFML/Graphics.hpp>
#include <iostream>

// function - align text
void AlignText(sf::Text& text, float horizontal, float vertical)
{
    // bounding box
    sf::FloatRect bounds = text.getLocalBounds();

    // position of origin
    float x = bounds.position.x + bounds.size.x * horizontal;
    float y = text.getCharacterSize() * vertical;

    // set origin
    text.setOrigin({x, y});
}

// class - mouse
class Mouse
{
    public:

    // position
    sf::Vector2f position;

    // states
    bool isPressed = false;
    bool onPress   = false;
    bool onRelease = false;

    // function - update states
    void Update(sf::RenderWindow& window)
    {
        // get position
        position = sf::Vector2f(sf::Mouse::getPosition(window));

        // reset on press & release
        onPress   = false;
        onRelease = false;

        // when mouse is pressed
        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
        {
            // when mouse isn't pressed yet
            if (not isPressed)
                onPress = true;

            isPressed = true;
        }
        else
        {
            // when mouse is still pressed
            if (isPressed)
                onRelease = true;

            isPressed = false;
        }
    }
};

// class - button
class Button
{
    private:

    // shape & text
    sf::RectangleShape shape;
    sf::Text           text;
    
    public:

    // parameters
    sf::Vector2f size         = {200.f, 100.f};
    sf::Vector2f position     = {0.f, 0.f};
    std::string  string       = "Button";
    float        text_scale   = 0.5f;
    float        text_align_v = 0.5f;
    
    // colors
    sf::Color default_shape = sf::Color(0, 170, 255);
    sf::Color default_text  = sf::Color::White;
    sf::Color over_shape    = default_text;
    sf::Color over_text     = default_shape;
    sf::Color pressed_shape = sf::Color::Yellow;
    sf::Color pressed_text  = sf::Color::White;

    // states
    bool isOver          = false;
    bool isPressed       = false;
    bool isPressedInside = false;
    bool onPress         = false;
    bool onRelease       = false;

    // constructor
    Button(sf::Font& font) : text(font) {}

    // function - draw
    void Draw(sf::RenderWindow& window)
    {
        // shape
        shape.setSize(size);
        shape.setOrigin(shape.getGeometricCenter());
        shape.setPosition(position);
        
        // text
        text.setString(string);
        text.setCharacterSize(size.y * text_scale);
        text.setPosition(position);
        AlignText(text, 0.5f, text_align_v);

        // color
        if (isPressed)
        {
            shape.setFillColor(pressed_shape);
            text .setFillColor(pressed_text);
        }
        else if (isOver)
        {
            shape.setFillColor(over_shape);
            text .setFillColor(over_text);
        }
        else
        {
            shape.setFillColor(default_shape);
            text .setFillColor(default_text);
        }

        // draw
        window.draw(shape);
        window.draw(text);
    }

    // function - update states
    void Update(Mouse& mouse)
    {
        // reset on press & on release
        onPress   = false;
        onRelease = false;

        // when mouse is over
        if (shape.getGlobalBounds().contains(mouse.position))
        {
            // on mouse press
            if (mouse.onPress)
            {
                onPress = true;
                isPressedInside = true;
            }

            // on mouse release
            if (mouse.onRelease and isPressedInside)
                onRelease = true;

            // when mouse is pressed
            if (mouse.isPressed and isPressedInside)
                isPressed = true;

            isOver = true;
        }
        else
        {
            // reset is pressed
            isPressed = false;

            isOver = false;
        }

        // reset is pressed
        if (not mouse.isPressed)
        {
            isPressed       = false;
            isPressedInside = false;
        }
    }
};

// main program
int main()
{
    // mouse
    Mouse mouse;

    // load font
    sf::Font font("C:/Windows/Fonts/arial.ttf");
    float font_center = 0.64f;

    // count
    int count = 0;
    sf::Text text(font);

    // button - increase
    Button button_1(font);
    button_1.string       = "+";
    button_1.text_align_v = font_center;

    // button - decrease
    Button button_2(font);
    button_2.string       = "-";
    button_2.text_align_v = font_center;

    // button - reset
    Button button_3(font);
    button_3.string       = "Reset";
    button_3.text_align_v = font_center;

    // create window
    sf::RenderWindow window(sf::VideoMode({300, 400}), "Title");

    // while window is still open
    while (window.isOpen())
    {
        // handle events
        while (std::optional event = window.pollEvent())
        {
            // on close button release
            if (event->is<sf::Event::Closed>())
            {
                // close window
                window.close();
            }

            // when window is resized
            else if (event->is<sf::Event::Resized>())
            {
                // update view
                window.setView(sf::View(sf::FloatRect({0.f, 0.f}, sf::Vector2f(window.getSize()))));
            }
        }

        // size of window
        float window_w = window.getView().getSize().x;
        float window_h = window.getView().getSize().y;

        // update mouse
        mouse.Update(window);

        // update buttons
        button_1.Update(mouse);
        button_2.Update(mouse);
        button_3.Update(mouse);

        // on buttons release
        if (button_1.onRelease and (count < 99))  count++;
        if (button_2.onRelease and (count > 0) )  count--;
        if (button_3.onRelease)                   count = 0;

        // fill window with color
        window.clear(sf::Color(64, 64, 64));

        // draw count
        text.setCharacterSize(window_h/4);
        if (count < 10)
        {
            text.setString(std::to_string(count));
            text.setPosition({window_w/2, window_h/6});
            AlignText(text, 0.5f, font_center);
            window.draw(text);
        }
        else
        {
            float gap = text.getCharacterSize() * 0.3f;

            // 1st digit
            text.setString(std::to_string(count % 10));
            text.setPosition({window_w/2 + gap, window_h/6});
            AlignText(text, 0.5f, font_center);
            window.draw(text);

            // 2nd digit
            text.setString(std::to_string(count / 10));
            text.setPosition({window_w/2 - gap, window_h/6});
            AlignText(text, 0.5f, font_center);
            window.draw(text);
        }

        // draw button
        float space       = 10.f;
        button_1.size     = {window_w/2 - space, window_h/3 - space};
        button_2.size     = {window_w/2 - space, window_h/3 - space};
        button_3.size     = {window_w   - space, window_h/3 - space};
        button_1.position = {window_w/4  , window_h/2};
        button_2.position = {window_w*3/4, window_h/2};
        button_3.position = {window_w/2  , window_h*5/6};
        button_1.Draw(window);
        button_2.Draw(window);
        button_3.Draw(window);

        // display
        window.display();
    }

    // program end successfully
    return 0;
}