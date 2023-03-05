#include <AnEngine.hpp>


class Sandbox : public AnEngine::Application {
    public:
	Sandbox() {}
	~Sandbox() {}

	void Run() {
		while (true);
	}
};

AnEngine::Application* AnEngine::CreateApplication() {
    return new Sandbox();
}
