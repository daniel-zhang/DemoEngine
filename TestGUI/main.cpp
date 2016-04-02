#include "Engine/Engine.h"

#include "Engine/QText.h"
#include "Engine/QButton.h"
#include "Engine/QLinearLayout.h"

class UI_Test_1 : public UITestInterface
{
public:
    virtual void createUI(std::vector<QWindow*>& rootQWindows) override
    {
        ///////////////////////////////////////////////

        QLinearLayout* layout11 = new QLinearLayout(QLinearLayout::Vertical);
        layout11->setWidthLayout(ELayoutPropertyValue::wrap_content).setHeightLayout(match_parent);
        layout11->setMargin(30.f);
        layout11->setPadding(30.f);
        layout11->setBkgColor(Palette::Blue);
        {
            layout11->add(new QButton)
                .debugSetSize(Vector2D(120.f, 30.f))
                .setWidthLayout(ELayoutPropertyValue::wrap_content)
                .setHeightLayout(ELayoutPropertyValue::wrap_content)
                .setMargin(QMargin(5.f, 5.f, 5.f, 5.f));

            layout11->add(new QButton)
                .debugSetSize(Vector2D(120.f, 30.f))
                .setWidthLayout(ELayoutPropertyValue::wrap_content)
                .setHeightLayout(ELayoutPropertyValue::match_parent)
                .setMargin(QMargin(5.f, 5.f, 5.f, 5.f));

            layout11->add(new QButton)
                .debugSetSize(Vector2D(120.f, 30.f))
                .setWidthLayout(ELayoutPropertyValue::match_parent)
                .setHeightLayout(ELayoutPropertyValue::wrap_content)
                .setMargin(QMargin(5.f, 5.f, 5.f, 5.f));
        }

        QLinearLayout* layout12 = new QLinearLayout(QLinearLayout::Horizental);
        layout12->setWidthLayout(ELayoutPropertyValue::match_parent).setHeightLayout(ELayoutPropertyValue::match_parent);
        layout12->setBkgColor(Palette::Red);
        layout12->setMargin(5.f);
        layout12->setPadding(5.f);
        layout12->add(new QButton)
            .debugSetSize(Vector2D(120.f, 30.f))
            .setWidthLayout(ELayoutPropertyValue::wrap_content)
            .setHeightLayout(ELayoutPropertyValue::wrap_content)
            .setMargin(QMargin(5.f, 5.f, 5.f, 5.f));

        layout12->add(new QButton)
            .debugSetSize(Vector2D(120.f, 30.f))
            .setWidthLayout(ELayoutPropertyValue::wrap_content)
            .setHeightLayout(ELayoutPropertyValue::wrap_content)
            .setMargin(QMargin(5.f, 5.f, 5.f, 5.f));

        layout12->add(new QButton)
            .debugSetSize(Vector2D(120.f, 30.f))
            .setWidthLayout(ELayoutPropertyValue::wrap_content)
            .setHeightLayout(ELayoutPropertyValue::wrap_content)
            .setMargin(QMargin(5.f, 5.f, 5.f, 5.f));

        QLinearLayout* layout1 = new QLinearLayout(QLinearLayout::Horizental);
        layout1->setBkgColor(Palette::Green);
        layout1->setPadding(10.f);
        layout1->setMargin(10.f);
        layout1->add(layout11);
        layout1->add(layout12);

        QWindow* window1 = new QWindow(L"Window1", EQWindowType::AppWindow, Vector2D(200, 200), Vector2D( 800, 400), 1.f);
        window1->add(layout1);

        /*
        QText* text1 = new QText;
        text1->setText(L"Main Window");
        text1->setColor(Palette::QGray_Light);
        text1->setFontSize(10);
        window1->add(text1);

        uint32_t numBtn = 5;
        for (uint32_t i = 0; i < numBtn; ++i)
        {
            window1->add(new QButton).debugSetPosition(Vector2D(30.f, 50.f + i*40.f)).debugSetSize(Vector2D(120.f, 30.f));
        }
        */

        rootQWindows.push_back(window1);

        ///////////////////////////////////////////////
        /*
        QWindow* window2 = new QWindow(L"Window2", EQWindowType::AppWindow, Vector2D(300, 300), Vector2D(400, 200), 1.f);

        QButton* btn2 = new QButton;
        window2->add(btn2);

        QText* text2 = new QText;
        text2->setColor(Palette::QGray_Light);
        text2->setText(L"Secondary Window");
        text2->setFontSize(10);
        window2->add(text2);

        rootQWindows.push_back(window2);
        */
    }
};

std::string convColor(const std::string& name, int32_t r, int32_t g, int32_t b)
{
    //    XMGLOBALCONST Color Black(0.000000000f, 0.000000000f, 0.000000000f, 1.000000000f);

    return string_format(
        "XMGLOBALCONST Color %s(%ff, %ff, %ff, 1.000000000f);\n",
        name.c_str(), (float) r / 255, (float) g/255, (float) b/255, 1.f );
}


void debugColor()
{
    std::string code;
    code += convColor("QGray", 195, 196, 198);
    code += convColor("QGray_Light", 238, 238, 238);
    code += convColor("QYellow", 253, 205, 0);
    code += convColor("QBlue", 137, 160, 191);
    code += convColor("QBlue_Dark", 76, 99, 131);
    code += convColor("QDark", 35, 35, 35);

}

int WINAPI wWinMain(__in HINSTANCE hInstance, __in_opt HINSTANCE hPrevInstance, __in LPWSTR lpCmdLine, __in int nShowCmd)
{
    debugColor();

    UI_Test_1 uiTest1;

    Engine::getInstance()->initialize(&uiTest1);

    Engine::getInstance()->run();

    return 0;
}