// Dear ImGui: standalone example application for GLFW + OpenGL 3, using programmable pipeline
// (GLFW is a cross-platform general purpose library for handling windows, inputs, OpenGL/Vulkan/Metal graphics context creation, etc.)

// Learn about Dear ImGui:
// - FAQ                  https://dearimgui.com/faq
// - Getting Started      https://dearimgui.com/getting-started
// - Documentation        https://dearimgui.com/docs (same as your local docs/ folder).
// - Introduction, links and more at the top of imgui.cpp

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "imgui.h"
#include "implot.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>
#include <cmath>
/* ALL MY ADDED INCLUDES */
#include <string>
#include <algorithm>
#include <sstream>
#include <chrono>
#include <fstream>
#include <iostream>
#include <deque>
#include <mutex>
#include <thread>
#include <unordered_map>
#include <map>
#include <cstring>
#include <vector>
/* ALL MY ADDED INCLUDES */
#include <iostream>
#include <GL/glu.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define GL_SILENCE_DEPRECATION
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif

// [Win32] Our example includes a copy of glfw3.lib pre-compiled with VS2010 to maximize ease of testing and compatibility with old VS compilers.
// To link with VS2010-era libraries, VS2015+ requires linking with legacy_stdio_definitions.lib, which we do using this pragma.
// Your own project should not be affected, as you are likely to link with a newer binary of GLFW that is adequate for your version of Visual Studio.
#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

// This example can also compile and run with Emscripten! See 'Makefile.emscripten' for details.
#ifdef __EMSCRIPTEN__
#include "../libs/emscripten/emscripten_mainloop_stub.h"
#endif

/*-------------------------------------------*/
/* FIXME: Data and Constants*/
std::mutex dataLock; /* the lock for data sharing between the parser and GUI*/
const std::string PIPE_PATH = "log";
const int MAX_SIZE = 10;

struct Data {
    std::deque<double> timestamps;
    std::deque<double> voltages;
    std::deque<double> speeds;
    std::deque<double> temperatures;
} data;

// TODO: Try to set this up on a separate file?
void parser() {
    std::string line;
    std::ifstream ins(PIPE_PATH);
    if (!ins) {
        std::cout << "Pipe failed to open!" << std::endl;
        exit(EXIT_FAILURE);
    }
    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        // Read first line
        if (!std::getline(ins, line))
            continue;
            
        // Only process if the line contains "PUBLISH"
        if (line.find("PUBLISH") == std::string::npos)
            continue;
            
        // Extract topic between single quotes
        size_t firstQuote = line.find("'");
        if (firstQuote == std::string::npos)
            continue;
        size_t secondQuote = line.find("'", firstQuote + 1);
        if (secondQuote == std::string::npos)
            continue;
        std::string topic = line.substr(firstQuote + 1, secondQuote - firstQuote - 1);
            
        // Tokenize topic using '/' and ignore empty tokens
        std::stringstream ss(topic);
        std::string token;
        std::vector<std::string> tokens;
        while (std::getline(ss, token, '/')) {
            if (!token.empty())
                tokens.push_back(token);
        }
        if (tokens.size() < 2)
            continue;
            
        // The second token is our data_point
        std::string data_point = tokens[1];
        
        // Read second line and convert it to a double value
        std::string dataLine;
        if (!std::getline(ins, dataLine))
            continue;
        double value;
        try {
            value = std::stod(dataLine);
        } catch (...) {
            continue;
        }
        
        // Store the value based on data_point name
        std::lock_guard<std::mutex> lock(dataLock);
        if (data_point == "timestamp") {
            data.timestamps.push_back(value);
            if (data.timestamps.size() > MAX_SIZE)
                data.timestamps.pop_front();
        }
        else if (data_point == "temperature") {
            data.temperatures.push_back(value);
            if (data.temperatures.size() > MAX_SIZE)
                data.temperatures.pop_front();
        }
        else if (data_point == "voltage") {
            data.voltages.push_back(value);
            if (data.voltages.size() > MAX_SIZE)
                data.voltages.pop_front();
        }
        else if (data_point == "speed") {
            data.speeds.push_back(value);
            if (data.speeds.size() > MAX_SIZE)
                data.speeds.pop_front();
        }
        
        // Optional: print out the extracted values for debugging
        std::cerr << "data_point: " << data_point << ", value: " << value << std::endl;
    }
}


/*-------------------------------------------*/
static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

// For Loading Images
GLuint LoadTexture(const char* filePath)
{
    int width, height, channels;
    unsigned char* data = stbi_load(filePath, &width, &height, &channels, 4);
    if (!data) {
        std::cerr << "Failed to load texture: " << filePath << std::endl;
        return 0;
    }

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    stbi_image_free(data);
    return textureID;
}

// Main code
int main(int, char**)
{
    /* FIXME: SET UP THE ENV TO RUN THE BROKER AND GUI */
    std::cerr << "Start of setup..." << std::endl;
    system((std::string("rm ") + PIPE_PATH).c_str());
    system((std::string("mkfifo ") + PIPE_PATH).c_str());
    system("sudo killall mosquitto_sub");
    system(std::string("(sudo mosquitto_sub -d -t \"#\" > " + PIPE_PATH + ")&").c_str());
    std::thread t1(parser);
    t1.detach();
    std::cerr << "End of setup..." << std::endl;
    /* ---------------------------------------------- */
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;

    // Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
    // GL ES 2.0 + GLSL 100
    const char* glsl_version = "#version 100";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
    // GL 3.2 + GLSL 150
    const char* glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

    // Create window with graphics context
    GLFWwindow* window = glfwCreateWindow(1280, 720, "Dear ImGui GLFW+OpenGL3 example", nullptr, nullptr);
    if (window == nullptr)
        return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return -1;
    }


    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImPlot::CreateContext();

    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
#ifdef __EMSCRIPTEN__
    ImGui_ImplGlfw_InstallEmscriptenCallbacks(window, "#canvas");
#endif
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return a nullptr. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype for higher quality font rendering.
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    // - Our Emscripten build process allows embedding fonts to be accessible at runtime from the "fonts/" folder. See Makefile.emscripten for details.
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != nullptr);


    // Our state
    bool show_demo_window = true;
    bool show_another_window = false;
    bool show_simple_data_window = true;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    // Main loop
#ifdef __EMSCRIPTEN__
    // For an Emscripten build we are disabling file-system access, so let's not attempt to do a fopen() of the imgui.ini file.
    // You may manually call LoadIniSettingsFromMemory() to load settings from your own storage.
    io.IniFilename = nullptr;
    EMSCRIPTEN_MAINLOOP_BEGIN
#else
    while (!glfwWindowShouldClose(window))
#endif
    {
        // Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
        glfwPollEvents();
        if (glfwGetWindowAttrib(window, GLFW_ICONIFIED) != 0)
        {
            ImGui_ImplGlfw_Sleep(10);
            continue;
        }

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);

        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
        {
            static float f = 0.0f;
            static int counter = 0;

            ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

            ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
            ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
            ImGui::Checkbox("Another Window", &show_another_window);

            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

            if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
                counter++;
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);

            ImGui::End();
        }

        // 3. Show another simple window.
        if (show_another_window)
        {
            ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
            ImGui::Text("Hello from another window!");
            if (ImGui::Button("Close Me"))
                show_another_window = false;
            ImGui::End();
        }

        {
            ImGui::Begin("Data", &show_simple_data_window);

            if (ImGui::BeginTable("table1", 3))
            {
                ImGui::TableNextColumn();
                ImGui::Text("Speed");
                ImGui::Text("20 mph");
                ImGui::TableNextColumn();
                ImGui::Text("Temperature");
                ImGui::Text("60 F");
                ImGui::TableNextColumn();
                ImGui::Text("Voltage");
                ImGui::Text("10 volts");
                ImGui::EndTable();
            }
            ImGui::End();
        }

        {
            ImGui::Begin("Implot", &show_simple_data_window);
            ImGui::Text("Here are the graphs!");

            static float xs1[1001], ys1[1001];
            for (int i = 0; i < 1001; ++i) {
                xs1[i] = i * 0.001f;
                ys1[i] = 0.5f + 0.5f * sinf(50 * (xs1[i] + (float)ImGui::GetTime() / 10));
            }

            static double xs2[20], ys2[20];
            for (int i = 0; i < 20; ++i) {
                xs2[i] = i * 1/19.0f;
                ys2[i] = xs2[i] * xs2[i];
            }
            if (ImPlot::BeginPlot("Speed")) {
                ImPlot::SetupAxes("x","y");
                ImPlot::PlotLine("f(x)", xs1, ys1, 1001);
                ImPlot::EndPlot();
            }
            if (ImPlot::BeginPlot("Temperature")) {
                ImPlot::SetupAxes("x","y");
                ImPlot::SetNextMarkerStyle(ImPlotMarker_Circle);
                ImPlot::PlotLine("g(x)", xs2, ys2, 20,ImPlotLineFlags_Segments);
                ImPlot::EndPlot();
            }
            if (ImPlot::BeginPlot("Voltage")) {
                ImPlot::SetupAxes("x","y");
                ImPlot::SetNextMarkerStyle(ImPlotMarker_Circle);
                ImPlot::PlotLine("g(x)", xs2, ys2, 20,ImPlotLineFlags_Segments);
                ImPlot::EndPlot();
            }

            ImGui::End();
        }
	/*
        {
            GLuint myImageTexture = LoadTexture("path/to/your/image.png");
            ImGui::Begin("Image Window");
            if (myImageTexture != 0) {
                ImGui::Text("Loaded image:");
                ImGui::Image((ImTextureID)(intptr_t)myImageTexture, ImVec2(256, 256));
            }
            ImGui::End();
        }
	*/

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }
#ifdef __EMSCRIPTEN__
    EMSCRIPTEN_MAINLOOP_END;
#endif

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImPlot::DestroyContext();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
