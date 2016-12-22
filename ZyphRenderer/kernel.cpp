#include "kernel.h"

#include "window.h"
#include <time.h> 

Kernel::Kernel()
{
	m_window = new Window("softwareRenderer");
	m_device = new Device(m_window);
	m_fpsCounter = new FpsCounter();
}

Kernel::~Kernel()
{
}

// Default settings to start the software renderer.
void Kernel::Run()
{
	Camera cam = Camera();
	Model* model = new Model();
	clock_t current_ticks, delta_ticks;
	clock_t fps = 0;
	model->LoadModel("../Models/SoftwareRendererModels/cube.txt");

	cam.m_position = ZyphMaths::Vector3(0.0f, 0.0f, 10.0f);
	cam.m_target = ZyphMaths::Vector3::ZERO;

	bool result = true;

	m_device->SetFillMode(ZyphSettings::ZyphSettings::LINE);
	model->m_rotation = ZyphMaths::Vector3(105.0f, -15.0f, 0.0f); //rotation settings used for portfolio.! 
	while (result)
	{
		current_ticks = clock();

		m_fpsCounter->Frame();
		m_device->Clear(94, 0, 0, 0);
		model->m_rotation = ZyphMaths::Vector3(model->m_rotation.x + 0.01f, model->m_rotation.y + 0.01f, model->m_rotation.z);
	    (m_device->*m_device->Render)(cam, model, 1);
		m_window->UpdateWindow();
		m_device->Present();

		delta_ticks = clock() - current_ticks; //the time, in ms, that took to render the scene
		if (delta_ticks > 0)
			fps = CLOCKS_PER_SEC / delta_ticks;
		OutputDebugString((std::to_string(fps).c_str()));
		OutputDebugString("\n");

	}
	delete m_window;
	delete model;
	delete m_device;
}

Device* Kernel::GetDevice()
{
	return m_device;
}

Window* Kernel::GetWindow()
{
	return m_window;
}
