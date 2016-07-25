/*
	Darren Sweeney
	darrensweeney.net
	darrensweeneydev@gmail.com
*/

// Demos
#include "CubeMapDemo.h"
#include "ShadowMapping.h"
#include "HDR_Demo.h"
#include "StencilReflectionsDemo.h"
#include "InstancingDemo.h"
#include "DeferredRenderingDemo.h"
#include "ObjectOutlineDemo.h"
#include "SSAO_Demo.h"
#include "ParallaxMappingDemo.h"
#include "OmnidirectionalShadowDemo.h"
#include "ModelLoadingDemo.h"

// ImGui
#include "Imgui\imgui.h"
#include "imgui_impl_glfw_gl3.h"

// OpenGL and GLFW
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

// NOTE(Darren): Camera works here because it's included in other headers, is this a problem,
//					longer compile time? What is the standard?

/*
	NOTE:(Darren): From the GLFW documentation.
			Do not pass the window size to glViewport or other pixel-based OpenGL calls. 
			The window size is in screen coordinates, not pixels. Use the framebuffer size, which is in pixels, for pixel-based calls.
*/

// Camera movement for all the scene demos.
Camera camera(vector3(0.0f, 1.5f, 4.0f));
void SceneMovement();

// GLFW Callback functions.
static void error_callback(int error, const char* description);
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode);
void scroll_callback(GLFWwindow *window, double xOffset, double yOffset);
void mouse_callback(GLFWwindow *window, double xPos, double yPos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void window_size_callback(GLFWwindow* window, int width, int height);

/*
	-------------------------------------------------------------------------------
*/
	// TODO(Darren): Take this out.
	bool keys[1024];
	GLfloat lastX = 400, lastY = 300;
	bool activeCamera;
	bool windowResized;

	GLfloat deltaTime = 0.0f;
	GLfloat lastFrame = 0.0f;
/*
	-------------------------------------------------------------------------------
*/

// Take out white theme.
inline void SetupImGuiStyle(bool bStyleDark_, float alpha_)
{
	ImGuiStyle& style = ImGui::GetStyle();
	//ImGuiIO& io = ImGui::GetIO();
	//io.Fonts->AddFontFromFileTTF("Fonts/Roboto-Regular.ttf", 16.5f);

	// light style from Pac�me Danhiez (user itamago) https://github.com/ocornut/imgui/pull/511#issuecomment-175719267
	style.Alpha = 1.8f;
	style.FrameRounding = 3.0f;
	style.WindowRounding = 0.0f;
	style.ScrollbarSize = 12.0f;
	style.Colors[ImGuiCol_Text] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.94f, 0.94f, 0.94f, 0.94f);
	style.Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(1.00f, 1.00f, 1.00f, 0.94f);
	style.Colors[ImGuiCol_Border] = ImVec4(0.00f, 0.00f, 0.00f, 0.39f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(1.00f, 1.00f, 1.00f, 0.10f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(1.00f, 1.00f, 1.00f, 0.94f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.31f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 1.00f, 1.00f, 0.51f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.31f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.86f, 0.86f, 0.86f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.98f, 0.98f, 0.98f, 0.53f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.26f, 0.59f, 0.98f, 0.31f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.78f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	style.Colors[ImGuiCol_ComboBg] = ImVec4(0.86f, 0.86f, 0.86f, 0.99f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.24f, 0.52f, 0.88f, 1.00f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	style.Colors[ImGuiCol_Button] = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.06f, 0.53f, 0.98f, 1.00f);
	style.Colors[ImGuiCol_Header] = ImVec4(0.26f, 0.59f, 0.98f, 0.31f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	style.Colors[ImGuiCol_Column] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
	style.Colors[ImGuiCol_ColumnHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.78f);
	style.Colors[ImGuiCol_ColumnActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(1.00f, 1.00f, 1.00f, 0.50f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
	style.Colors[ImGuiCol_CloseButton] = ImVec4(0.59f, 0.59f, 0.59f, 0.50f);
	style.Colors[ImGuiCol_CloseButtonHovered] = ImVec4(0.98f, 0.39f, 0.36f, 1.00f);
	style.Colors[ImGuiCol_CloseButtonActive] = ImVec4(0.98f, 0.39f, 0.36f, 1.00f);
	style.Colors[ImGuiCol_PlotLines] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
	style.Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);

	if (bStyleDark_)
	{
		for (int i = 0; i <= ImGuiCol_COUNT; i++)
		{
			ImVec4& col = style.Colors[i];
			float H, S, V;
			ImGui::ColorConvertRGBtoHSV(col.x, col.y, col.z, H, S, V);

			if (S < 0.1f)
			{
				V = 1.0f - V;
			}
			ImGui::ColorConvertHSVtoRGB(H, S, V, col.x, col.y, col.z);
			if (col.w < 1.00f)
			{
				col.w *= alpha_;
			}
		}
	}
	else
	{
		for (int i = 0; i <= ImGuiCol_COUNT; i++)
		{
			ImVec4& col = style.Colors[i];
			if (col.w < 1.00f)
			{
				col.x *= alpha_;
				col.y *= alpha_;
				col.z *= alpha_;
				col.w *= alpha_;
			}
		}
	}
}

// NOTE(Darren): Just for testings.
bool switchDeltaTime;

int main(int, char**)
{
    // Setup window
    glfwSetErrorCallback(error_callback);
	if (!glfwInit())
	{
		printf("failed to initialize GLFW.\n");
		return -1;
	}

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

	// Get the desktop resolution.
	GLFWmonitor *monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* vidMode = glfwGetVideoMode(monitor);
	int screenWidth = vidMode->width;
	int screenHeight = vidMode->height;

	bool fullscreen = false;

    GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "OpenGL Tech Demo - Darren Sweeney",
		fullscreen ? monitor : NULL, NULL);

	// GLFW input callbacks.
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetWindowSizeCallback(window, window_size_callback);

	// Opitions
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetWindowPos(window, 0, 0);

    glfwMakeContextCurrent(window);
	if (gl3wInit()) 
	{
		printf("failed to initialize OpenGL\n");
		return -1;
	}

    // Setup ImGui binding
	// Set the install_callbacks to false as i am setting up the GLFW input callbacks myself above.
    ImGui_ImplGlfwGL3_Init(window, false);
	SetupImGuiStyle(true, 1.0f);
	bool ImGui_WindowOpened = true;

	// Demos
	ModelLoadingDemo *modelLoadingDemo = new ModelLoadingDemo();
	HDR_DEMO *hdrDemo = new HDR_DEMO();
	OmnidirectionalShadowDemo *omnidirectionalShadowDemo = new OmnidirectionalShadowDemo();
	InstancingDemo *instancingDemo = new InstancingDemo();
	StencilReflectionDemo *stencilReflectionDemo = new StencilReflectionDemo();
	ObjectOutlineDemo *objectOutlineDemo = new ObjectOutlineDemo();
	DeferredRenderingDemo *deferredRenderingDemo = new DeferredRenderingDemo();
	CubeMapDemo *cubeMapDemo = new CubeMapDemo();
	ShadowMapping *directionalShadowDemo = new ShadowMapping();		// TODO(Darren): Rename this.
	ParallaxMappingDemo *parallaxingDemo = new ParallaxMappingDemo();
	SSAO_Demo *ssao_Demo = new SSAO_Demo();

	enum Demos
	{
		cubeMap,					// ------------
		directionalShadow,			// *** DONE ***
		hdr,						// *** DONE ***
		stencilReflection,			// *** DONE ***
		instancing,					// *** DONE ***
		deferredRendering,			// *** DONE ***
		objectOutline,				// *** DONE ***
		ssao,						// ------------
		parallaxingMapping,			// *** DONE ***
		omnidirectionalShadow,		// *** DONE ***
		modelLoading				// *** DONE ***
	};

	Demos currentDemo = Demos::cubeMap;

	// TODO(Darren): Fill out demo info for each demo.
	const char* demoInfo = " ";

	// Scene opitions.
	bool wireframeMode = false;

	// Load all the shaders and models required for all the demos.
	// All resources deallocated on program exiting.
	ResourceManager::LoadAllDemoResources();

    // Main loop
    while (!glfwWindowShouldClose(window))
    {
		// Set frame time
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// TODO(Darren): Make use of delta time.
		camera.deltaTime = deltaTime;
		SceneMovement();
		camera.KeyboardMovement(keys, deltaTime);

		SetupImGuiStyle(true, 1.0f);

		glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glfwPollEvents();
        ImGui_ImplGlfwGL3_NewFrame();

#pragma region ImGui

		/*int texture_id = omnidirectionalShadowDemo.depthCubemap;

		ImGui::Begin("Cube Map - Framebuffer Test", &windowOpened, ImVec2(430, 250), 0.5f, ImGuiWindowFlags_NoSavedSettings);
		ImGui::SetWindowPos(ImVec2(screenWidth - 460, screenHeight - 300));
		ImVec2 uv0 = ImVec2(0, 1);
		ImVec2 uv1 = ImVec2(1, 0);
		ImGui::Image((ImTextureID)texture_id, ImVec2(400, 200), uv0, uv1);
		ImGui::End();*/

		ImGui::Begin("OpenGL Tech Demos", &ImGui_WindowOpened, ImVec2(0, 0), 0.5f, ImGuiWindowFlags_AlwaysUseWindowPadding | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoTitleBar);
		ImGui::SetWindowPos(ImVec2(10, 10));
		ImGui::SetWindowSize(ImVec2(255, screenHeight - 20));
		//ImGui::Text("Application average:\n\t %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

		//ImGui::Checkbox("Delta Time Mode", &switchDeltaTime);

		if (ImGui::CollapsingHeader("Demos", 0, true, true))
		{
			if (ImGui::TreeNode("Cube Mapping"))
			{
				bool clicked = ImGui::Button("Cube Mapping Demo");
				if(clicked)
					currentDemo = Demos::cubeMap;
				ImGui::TreePop();
			}
			if (ImGui::TreeNode("Instancing"))
			{
				bool clicked = ImGui::Button("Instancing Demo");
				if (clicked)
				{
					currentDemo = Demos::instancing;
					demoInfo = "Example showing rendering of 6,000 objects, 3,000 grass and 3,000 rock objects.";
				}
				ImGui::TreePop();
			}
			if (ImGui::TreeNode("Defered Rendering"))
			{
				bool clicked = ImGui::Button("Defered Rendering Demo");
				if (clicked)
				{
					currentDemo = Demos::deferredRendering;
					demoInfo = "Defered Rendering of 100 Light sources around 100 models..\n";
				}
				ImGui::Checkbox("Move Lights", &deferredRenderingDemo->moveLights);
				ImGui::Checkbox("Toggle Lights", &deferredRenderingDemo->renderLights);
				ImGui::TreePop();
			}
			if (ImGui::TreeNode("Model Loading"))
			{
				bool clicked = ImGui::Button("Model Loading Demo");
				if (clicked)
				{
					currentDemo = Demos::modelLoading;
					demoInfo = "I used Assimp libiary to load the model\nMore info here.\n";
				}
				ImGui::Checkbox("Display Normals", &modelLoadingDemo->showNormals);
				ImGui::Checkbox("Display Enviroment Map", &modelLoadingDemo->showCubemap);
				ImGui::TreePop();
			}
			if (ImGui::TreeNode("Stencil Buffer"))
			{
				if (ImGui::TreeNode("Stencil Reflections"))
				{
					bool clicked = ImGui::Button("Stencil Reflection Demo");
					if (clicked)
					{
						currentDemo = Demos::stencilReflection;
						demoInfo = "Shows stencil refleciton. By turning off the plane texutre you can see more clearly that the shadows are precerved when reflected.";
					}
					ImGui::Checkbox("Plane Texture Off", &stencilReflectionDemo->planeTextureOff);
					ImGui::TreePop();
				}

				if (ImGui::TreeNode("Stencil Outline"))
				{
					bool clicked = ImGui::Button("Stencil Outline Demo");
					if (clicked)
						currentDemo = Demos::objectOutline;
					ImGui::Checkbox("Render Lights", &objectOutlineDemo->renderLights);
					ImGui::TreePop();
				}

				ImGui::TreePop();
			}
			if (ImGui::TreeNode("Parralxing Mapping"))
			{
				bool clicked = ImGui::Button("Parralxing Mapping Demo");
				if (clicked)
				{
					currentDemo = Demos::parallaxingMapping;
					demoInfo = "Demostates steep parallax mapping along a walkable cobble floor.";
				}
				ImGui::SliderFloat("Depth", &parallaxingDemo->heightScale, 0.005f, 0.10f, "%.3f");
				ImGui::Checkbox("Enable Parallax", &parallaxingDemo->enableParallax);
				ImGui::Button("Pause Light");
				ImGui::TreePop();
			}
			if (ImGui::TreeNode("HDR Lighting"))
			{
				bool clicked = ImGui::Button("HDR Demo");
				if (clicked)
				{
					currentDemo = Demos::hdr;
					demoInfo = "";
				}
				ImGui::SliderFloat("Exposure", &hdrDemo->exposure, 0.1f, 5.0f, "%.3f");
				ImGui::Checkbox("Render Lights", &hdrDemo->renderLights);
				ImGui::TreePop();
			}
			if (ImGui::TreeNode("Shadow Maps"))
			{
				if (ImGui::TreeNode("Directional Shadow Maps"))
				{
					bool clicked = ImGui::Button("Directional Shadow Demo");
					if(clicked)
						currentDemo = Demos::directionalShadow;
					ImGui::Checkbox("Move Light Source", &directionalShadowDemo->moveLight);
					ImGui::Checkbox("Render Light", &directionalShadowDemo->renderLight);
					ImGui::TreePop();
				}

				if (ImGui::TreeNode("Omnidirectional Shadow Maps"))
				{
					bool clicked = ImGui::Button("Omnidirectional Shadow Demo");
					if (clicked)
						currentDemo = Demos::omnidirectionalShadow;
					ImGui::Checkbox("Move Light Source", &omnidirectionalShadowDemo->moveLight);
					ImGui::Checkbox("Render Light", &omnidirectionalShadowDemo->renderLight);
					ImGui::TreePop();
				}

				ImGui::TreePop();
			}
			if (ImGui::TreeNode("SSAO"))
			{
				bool clicked = ImGui::Button("SSAO Demo");
				if (clicked)
					currentDemo = Demos::ssao;
				ImGui::TreePop();
			}
		}
		if (ImGui::CollapsingHeader("Current Demo Info", 0, true, true))
		{
			ImGui::TextWrapped(demoInfo);
		}
		if (ImGui::CollapsingHeader("Application Info", 0, true, true))
		{
			char* version = (char*)glGetString(GL_VERSION);
			char* renderer = (char*)glGetString(GL_RENDERER);

			// TODO(Darren): Figure out how to add the two string together.
			ImGui::Text(version);
			ImGui::Text(renderer);
			ImGui::Text("Draw Calls: ");
			ImGui::Text("Post Processing Time: ");

			ImGui::Checkbox("FullScreen", &fullscreen);
		}
		if (ImGui::CollapsingHeader("About", 0, true, true))
		{
			ImGui::Text("OpenGL Tech Demo by Darren Sweeney\n\nWebsite: darrensweeney.net\nEmail: darrensweeneydev@gmail.com\nTwitter: @_DarrenSweeney");
		}

		if (ImGui::CollapsingHeader("Debug Controls", 0, true, true))
		{
			if (ImGui::TreeNode("Camera"))
			{
				ImGui::Checkbox("Fly Camera", &camera.flyCamera);

				// TODO(Darren): Figure out a way i can render labels above sliders.
				ImGui::SliderFloat("movement_speed", &camera.movementSpeed, 0.0f, 50.0f, "%.3f");
				ImGui::SliderFloat("camera_speed", &camera.cameraSpeed, 0.0f, 136.0f, "%.3f");

				// Camera Bobing
				ImGui::SliderFloat("camera_ampletude", &camera.ampletude, 0.0f, 1.0f, "%.36f");
				ImGui::SliderFloat("camera_frequincy", &camera.frequincy, 0.0f, 1.0f, "%.36f");
				ImGui::TreePop();
			}

			ImGui::Checkbox("Wireframe Mode", &wireframeMode);
		}
		ImGui::End();
#pragma endregion

		glfwGetFramebufferSize(window, &screenWidth, &screenHeight);
		glViewport(0, 0, screenWidth, screenHeight);

#pragma region DemoUpdateCalls
		switch (currentDemo)
		{
			case Demos::cubeMap:
			{
				cubeMapDemo->InitalizeScene(screenWidth, screenHeight);
				cubeMapDemo->UpdateScene(camera, screenWidth, screenHeight);

				break;
			}

			case Demos::directionalShadow:
			{
				directionalShadowDemo->InitalizeScene();
				directionalShadowDemo->UpdateScene(camera, screenWidth, screenHeight);

				break;
			}

			case Demos::hdr:
			{
				hdrDemo->InitalizeScene(screenWidth, screenHeight);
				hdrDemo->UpdateScene(camera, screenWidth, screenHeight, windowResized);

				break;
			}

			case Demos::stencilReflection:
			{
				stencilReflectionDemo->InitalizeScene();
				stencilReflectionDemo->Update(camera, screenWidth, screenHeight);

				break;
			}

			case Demos::instancing:
			{
				instancingDemo->InitalizeScene();
				instancingDemo->Update(camera, screenWidth, screenHeight);

				break;
			}

			case Demos::deferredRendering:
			{
				deferredRenderingDemo->InitalizeScene(screenWidth, screenHeight);
				deferredRenderingDemo->Update(camera, screenWidth, screenHeight, windowResized);

				break;
			}

			case Demos::objectOutline:
			{
				objectOutlineDemo->InitalizeScene();
				objectOutlineDemo->Update(camera, screenWidth, screenHeight);

				break;
			}

			case Demos::ssao:
			{
				ssao_Demo->InitalizeScene(screenWidth, screenHeight);
				ssao_Demo->Update(camera, screenWidth, screenHeight, windowResized);

				break;
			}

			case Demos::parallaxingMapping:
			{
				parallaxingDemo->Initalize(camera.position);
				parallaxingDemo->Update(camera, screenWidth, screenHeight);

				break;
			}

			case Demos::omnidirectionalShadow:
			{
				omnidirectionalShadowDemo->Initalize();
				omnidirectionalShadowDemo->Update(camera, screenWidth, screenHeight);

				break;
			}

			case Demos::modelLoading:
			{
				modelLoadingDemo->Initalize();
				modelLoadingDemo->Update(camera, screenWidth, screenHeight);

				break;
			}

		default:
			break;
		}
#pragma endregion


		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDisable(GL_STENCIL_TEST);
		// Render the GUI pannel.
		ImGui::Render();
		glEnable(GL_STENCIL_TEST);
		glPolygonMode(GL_FRONT_AND_BACK, wireframeMode ? GL_LINE : GL_FILL);

		windowResized = false;

        glfwSwapBuffers(window);
    }

    // Cleanup
    ImGui_ImplGlfwGL3_Shutdown();
	ResourceManager::Clear();
	delete modelLoadingDemo;
	delete hdrDemo;
	delete omnidirectionalShadowDemo;
	delete instancingDemo;
	delete stencilReflectionDemo;
	delete objectOutlineDemo;
	delete deferredRenderingDemo;
	delete cubeMapDemo;
	delete directionalShadowDemo;
	delete parallaxingDemo;
	delete ssao_Demo;
	glfwTerminate();

    return 0;
}

void SceneMovement()
{
	camera.KeyboardMovement(keys, switchDeltaTime ? ImGui::GetIO().DeltaTime : deltaTime);
}

#pragma region "GLFW callbacks"

static void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error %d: %s\n", error, description);
}

void window_size_callback(GLFWwindow* window, int width, int height)
{
	windowResized = true;
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			keys[key] = true;
		else if (action == GLFW_RELEASE)
			keys[key] = false;
	}
}

bool first_entered_window = true;
void mouse_callback(GLFWwindow *window, double xPos, double yPos)
{
	/*
		The sudden jump in the camera is caused by when the cursor first enters
		the window the mouse_callback funtion is called with a xPos and yPos
		equal to the location of the mouse when entered on screen.
		This is usually a position that is quite a distance away from
		the center of the screen resulting in large offsets.
	*/
	if (first_entered_window)
	{
		lastX = xPos;
		lastY = yPos;
		first_entered_window = false;
	}

	GLfloat xOffset = xPos - lastX;
	GLfloat yOffset = lastY - yPos;
	lastX = xPos;
	lastY = yPos;

	if(activeCamera)
		camera.MouseMovement(xOffset, yOffset);
}
void scroll_callback(GLFWwindow *window, double xOffset, double yOffset)
{
	camera.MouseScroll(yOffset);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
		activeCamera = true;
	else
		activeCamera = false;
}

#pragma endregion