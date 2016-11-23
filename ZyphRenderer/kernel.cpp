#include "kernel.h"

#include "window.h"


Kernel::Kernel()
{
	m_window = new Window("softwareRenderer");
	m_device = new Device(m_window);
}

Kernel::~Kernel()
{
}

// Default settings to start the software renderer.
void Kernel::Run()
{
	Camera cam = Camera();
	Model* model = new Model();

	model->LoadModel("../Models/SoftwareRendererModels/cube.txt");

	cam.m_position = ZyphMaths::Vector3(0.0f, 0.0f, 10.0f);
	cam.m_target = ZyphMaths::Vector3::ZERO;

	bool result = true;

	m_device->SetFillMode(ZyphSettings::ZyphSettings::SOLID);
	//model->m_rotation = ZyphMaths::Vector3(105.0f, -35.0f, 0.0f); //rotation settings used for portfolio.! 
	while (result)
	{
		m_device->Clear(94, 0, 0, 0);
		model->m_rotation = ZyphMaths::Vector3(model->m_rotation.x + 0.01f, model->m_rotation.y + 0.01f, model->m_rotation.z);
	    (m_device->*m_device->Render)(cam, model, 1);
		m_window->UpdateWindow();
		m_device->Present();
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
