#include "./source/App.h"

int main(){

    App app;

    app.setUpOpenGl();
    app.createRenderer();

    app.run();

    return 0;
}