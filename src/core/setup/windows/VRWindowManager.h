#ifndef VRWINDOWMANAGER_H_INCLUDED
#define VRWINDOWMANAGER_H_INCLUDED

#include <OpenSG/OSGRenderAction.h>
#include "core/setup/VRSetupFwd.h"

namespace xmlpp{ class Element; }
namespace Gtk{ class Window; }

OSG_BEGIN_NAMESPACE;
using namespace std;

class VRWindow;

class VRWindowManager {
    private:
        map<string, VRWindowPtr> windows;
        VRGtkWindowPtr editorWindow;
        RenderActionRefPtr ract;
        bool rendering_paused = false;

        bool checkWin(string name);

    public:
        VRWindowManager();
        ~VRWindowManager();

        map<string, VRWindowPtr> getWindows();
        VRWindowPtr getWindow(string name);

        void initGlut();

        VRWindowPtr addGlutWindow  (string name);
        VRWindowPtr addGtkWindow   (string name, string glarea = "glarea");
        VRWindowPtr addMultiWindow (string name);
        void removeWindow   (string name);

        void setWindowView(string name, VRViewPtr view);
        void addWindowServer(string name, string server);
        void changeWindowName(string& name, string new_name);

        void getWindowSize(string name, int& w, int& h);
        void resizeWindow(string name, int w, int h);

        void pauseRendering(bool b);
        RenderActionRefPtr getRenderAction();
        void updateWindows();

        VRGtkWindowPtr getEditorWindow();

        void save(xmlpp::Element* node);
        void load(xmlpp::Element* node);
};

OSG_END_NAMESPACE;

#endif // VRWINDOWMANAGER_H_INCLUDED
