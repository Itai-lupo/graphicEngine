#include "window.h"
#include "logger.h"
#include <string>
namespace raftelGraphicEngine
{
    static std::vector<window> windows = std::vector<window>();
    raftelId windowManger::addWindow(const std::string& title, unsigned int width, unsigned int height)
    {
        window newWin(title, width, height);
        newWin.Window =  newWin.Init(newWin);
        windows.push_back(newWin);
        return windows.size() - 1;
    }

    void windowManger::onUpdate()
    {
        for(window win: windows)
        {
            win.onUpdate(win);
        }
    }

    windowPtr windowManger::raftelIdToWindowId(raftelId windowId)
    {
        return windows[windowId].Window;
    }

    unsigned int windowManger::getWidth(raftelId windowId){ return windows[windowId].Width; }
    unsigned int windowManger::getHeight(raftelId windowId){ return windows[windowId].Height; }
}