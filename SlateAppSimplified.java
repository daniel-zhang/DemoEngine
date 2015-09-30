class App {
    array<Window> mTopWindows;
    Renderer mRenderer;

    void step(){
        foreach(Window wnd : mTopWindows){
            tickWindow(wnd);
        }

        DrawBuffer db = mRenderer.getDrawBuffer();
        foreach(Window wnd : mTopWindows){
            drawWindowAndChildren(wnd, db);
        }

        mRenderer.draw(db);
    }

    void tickWindow(Window wnd) {
        wnd.prepass();
        wnd.tickWidgets(wnd.getDisplayArea(), getDeltaTime());

        foreach(childWnd : wnd.getChildWindows()){
            tickWindow(childWnd);
        }
    }

    void drawWindowAndChildren(Window wnd, DrawBuffer db){
        WindowElements we = db.addWndElements(wnd);
        wnd.prepass();
        wnd.onPaint(we);

        foreach(Window child : wnd.getChildWindows()){
            drawWindowAndChildren(child, db);
        }
    }
}

class Window{
    void prepass(){}

    void tickWidgets(DisplayArea area, float dt){

        this.tickMe(area, dt);

        ArrangedWigetList result;
        arrangeChildren(area, result);

        foreach(ArrangedWidget aw : result){
            aw.Widget.tickWidgets(aw.area, dt);
        }
    }

    array<Window> getChildWindows(){}

    void onPaint(WindowElements we){
        // somehow call this:
        child_widget.onPaint(we);
    }
}

class Renderer{
    DrawBuffer getDrawBuffer(){}

    void draw(DrawBuffer db){
        // just draw batches in the buffer
    }
}

class DrawBuffer{
    WindowElements addWndElements(Window wnd){

    }
}

