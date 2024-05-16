#ifndef DEV_TOOLS
#include <utils/checkML.h>
#endif // !DEV_TOOLS
#include <iostream>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <string>
#include <architecture/Game.h>
#include <sdlutils/SDLUtils.h>
#include <../rapidCSV/rapidcsv.h>



int main(int argc, char* argv[]) {
	/*rapidcsv::Document doc ("src/prueba.csv");
	std::vector<float> row = { 1,2,3,4,5,6,7,8 };
	doc.InsertRow<float>(0,row);*/
	try {
		Game::instance()->run();
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	}
	catch (std::string e) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "ERROR", e.c_str(), sdlutils().window());
	}
	catch(std::runtime_error e){
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "ERROR", e.what(), sdlutils().window());
	}
	//catch (std::exception e) {
	//	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "ERROR", e.what(), sdlutils().window());
	//}

	return 0;
}