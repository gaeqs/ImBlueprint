#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <imgui.h>

#include <GLFW/glfw3.h>
#include <stdio.h>
#include <imblueprint/imblueprint.h>

#ifdef __APPLE__
    #define GL_SILENCE_DEPRECATION
#endif

template<typename T>
class ValueNode : public ImBlueprint::Node
{
    T _value;

  public:
    explicit ValueNode(T initial) :
        Node("Value"),
        _value(initial)
    {
        defineOutput<T>("value", _value);
    }

    void renderBody() override
    {
        ImGui::SetNextItemWidth(100);
        if constexpr (std::is_same_v<T, int>) {
            if (ImGui::InputInt("Value", &_value)) {
                sendOutput("value", _value);
            }
        } else if constexpr (std::is_same_v<T, float>) {
            if (ImGui::InputFloat("Value", &_value)) {
                sendOutput("value", _value);
            }
        }
    }
};

template<typename T>
class ValueSum : public ImBlueprint::Node
{
    T _result;

  public:
    ValueSum() :
        Node("Sum"),
        _result(0)
    {
        defineInput<T>("first", false);
        defineInput<T>("second", true);
        defineOutput<T>("result", _result);
    }

    void renderBody() override
    {
        if constexpr (std::is_same_v<T, int>) {
            ImGui::Text("Current result: %d", _result);
        } else if constexpr (std::is_same_v<T, float>) {
            ImGui::Text("Current result: %f", _result);
        }
    }

    void onInputChange(const std::string& name, const std::any& value) override
    {
        T result = getInput<T>("first").value_or(0);
        auto second = getMultipleInputs<T>("second").value_or(std::vector<T>());

        for (T it : second) {
            result += it;
        }

        if (result != _result) {
            _result = result;
            sendOutput("result", _result);
        }
    }
};

template<typename From, typename To>
class ValueCast : public ImBlueprint::Node

{
  public:
    ValueCast() :
        Node("Cast")
    {
        defineInput<From>("from", false);
        defineOutput<To>("to");
    }

    void onInputChange(const std::string& name, const std::any& value) override
    {
        auto from = getInput<From>("from");
        if (!from.has_value()) {
            sendOutput("to", std::any());
            return;
        }

        To to = static_cast<To>(from.value());
        sendOutput("to", to);
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
    editor.addNode<ValueNode<int>>(5);
    editor.addNode<ValueNode<int>>(10);
    editor.addNode<ValueNode<int>>(15);

    editor.addNode<ValueNode<float>>(5);
    editor.addNode<ValueNode<float>>(10);
    editor.addNode<ValueNode<float>>(15);

    editor.addNode<ValueSum<int>>();
    editor.addNode<ValueSum<int>>();
    editor.addNode<ValueSum<int>>();
    editor.addNode<ValueSum<int>>();

    editor.addNode<ValueSum<float>>();
    editor.addNode<ValueSum<float>>();
    editor.addNode<ValueSum<float>>();
    editor.addNode<ValueSum<float>>();

    editor.addNode<ValueCast<int, float>>();
    editor.addNode<ValueCast<int, float>>();

    editor.addNode<ValueCast<float, int>>();
    editor.addNode<ValueCast<float, int>>();

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
