
#include "../window_handling/window_factory.h"

class Renderer {
public:
    Renderer(std::unique_ptr<IWindow> window);

    void initRender();

private:
    std::unique_ptr<IWindow> mRenderWindow;
};
