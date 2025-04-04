#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <imgui.h>

#include <GLFW/glfw3.h>
#include <stdio.h>
#include <imblueprint/imblueprint.h>

#ifdef __APPLE__
    #define GL_SILENCE_DEPRECATION
#endif

class TestNode : public ImBlueprint::Node
{
  public:
    TestNode() :
        Node("Test node")
    {
        defineInput<int>("Patata");
        defineOutput<int>("Pototo");
    }
};

class ValueNode : public ImBlueprint::Node
{
    int _value;

  public:
    explicit ValueNode(int initial) :
        Node("Value"),
        _value(initial)
    {
        defineInput<int>("test1");
        defineInput<int>("test2");
        defineInput<int>("test3");
        defineInput<int>("test4");
        defineInput<int>("test1");
        defineOutput<int>("value", _value);
    }

    void renderBody() override
    {
        ImGui::SetNextItemWidth(100);
        if (ImGui::InputInt("Value", &_value)) {
            sendOutput("value", _value);
        }
    }
};

class ValueSum : public ImBlueprint::Node
{
    int _result;

  public:
    ValueSum() :
        Node("Sum"),
        _result(0)
    {
        defineInput<int>("first");
        defineInput<int>("second");
        defineOutput<int>("result", _result);
    }

    void renderBody() override
    {
        ImGui::Text("Current result: %d", _result);
    }

    void onInputChange(const std::string& name, const std::any& value) override
    {
        auto out = getInput<int>("first").value_or(0) + getInput<int>("second").value_or(0);
        if (out != _result) {
            _result = out;
            sendOutput("result", _result);
        }
    }
};

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "Error GLFW %d: %s\n", error, description);
}

int main(int, char**)
{
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit()) {
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(1280, 720, "Imgui", nullptr, nullptr);
    if (window == nullptr) {
        return 1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    ImBlueprint::Editor editor;
    editor.addNode<ValueNode>(5);
    editor.addNode<ValueNode>(10);
    editor.addNode<ValueNode>(15);
    editor.addNode<ValueSum>();
    editor.addNode<ValueSum>();
    editor.addNode<ValueSum>();
    editor.addNode<ValueSum>();

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
        if (ImGui::Begin("Scene editor")) {
            editor.render();
        }
        ImGui::End();

        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
