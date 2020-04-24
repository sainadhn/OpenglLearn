#include "TestClearColor.h"
#include "Renderer.h"
#include "imgui/imgui.h"

namespace Test {

	TestClearColor::TestClearColor() 
		:m_ClearColor{0.2f, 0.3f, 0.8f, 1}
	{
	
	}

	TestClearColor::~TestClearColor() 
	{
	
	}

	void TestClearColor::OnUpdate(float deltaTime) 
	{

	}
	void TestClearColor::OnRender() 
	{
		GLCall(glClearColor(m_ClearColor[0], m_ClearColor[1], m_ClearColor[2], m_ClearColor[3]));
		GLCall(glClear(GL_CLEAR_BUFFER));
	}
	void TestClearColor::OnImGuiRender() 
	{
		ImGui::ColorPicker4("Clear Color", m_ClearColor);
	}
}
