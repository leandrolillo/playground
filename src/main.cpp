#include <iostream>
#include <stdio.h>

#define GL_SILENCE_DEPRECATION
#include <OpenGLRunner.h>
#include <AudioRunner.h>

#include "demo/GeneralDemo.h"
#include "demo/PhysicsDemo.h"
#include "demo/TerrainDemo.h"
#include "demo/ObjDemo.h"
#include "demo/ColDetDemo.h"
#include "demo/RoadFighter.h"

#include "demo/PlaygroundTest.h"

/**
 * From https://stackoverflow.com/questions/865668/parsing-command-line-arguments-in-c
 */
class InputParser{
    public:
        InputParser (int &argc, char **argv){
            for (int i=1; i < argc; ++i)
                this->tokens.push_back(String(argv[i]));
        }
        /// @author iain
        const String& getCmdOption(const String &option) const{
            std::vector<String>::const_iterator itr;
            itr =  std::find(this->tokens.begin(), this->tokens.end(), option);
            if (itr != this->tokens.end() && ++itr != this->tokens.end()){
                return *itr;
            }
            static const String empty_string("");
            return empty_string;
        }

        const String &getArg(unsigned char position) {
//        	for(int index = 0; index < tokens.size(); index++) {
//        		printf("Token[%d]: [%s]\n", index, tokens[index].c_str());
//        	}

        	if(position < tokens.size()) {
        		return this->tokens[position];
        	}

        	static const String empty_string("");
            return empty_string;
        }

        /// @author iain
        bool cmdOptionExists(const String &option) const{
            return std::find(this->tokens.begin(), this->tokens.end(), option)
                   != this->tokens.end();
        }
    private:


        std::vector <String> tokens;

};


/**
 * Usage: playground [playground name]
 *	where [playground name] in [GeneralDemo, ObjDemo, CollisionDetection, Terrain, Physics, RoadFighter]
 * Note that tests are always ran
 */
int main(int argc, char** argv){

	String repository = Paths::add(Paths::getDirname(argv[0]), "../../media"); //assumes executable lies in playground/target folder
	String defaultPlaygroundName = "physics";


	//TODO: Move this to a list of playgrounds and pick the one to run based on its name. Also list available names from this list.
	InputParser input(argc, argv);
	if(input.cmdOptionExists("-h") || input.cmdOptionExists("--help")){
		printf("Usage: %s [playground name]\n", Paths::getBasename(argv[0]).c_str());
		printf("	where [playground name] in [GeneralDemo, ObjDemo, CollisionDetection, Terrain, Physics, RoadFighter]\n");
		printf("Note that tests are always ran\n");
	} else
	{
		String playgroundName = StringUtils::toLowercase(StringUtils::trim(input.getArg(0)));
		std::remove_if(playgroundName.begin(), playgroundName.end(), [](unsigned char ch) { return !std::isspace(ch); });
		if(playgroundName == "") {
			playgroundName = defaultPlaygroundName;
		}

	    TestsPlayground *playgroundtests = new TestsPlayground(repository);
	    playgroundtests->withName("Playground Tests")->run();
	    delete playgroundtests;


		printf("\n\nRunning playground [%s]\n", playgroundName.c_str());

	    if(playgroundName == "generaldemo") {
	    	Playground *playground = new PlaygroundGeneralDemo(repository);
			playground->withName("GeneralDemo")->run();
			delete playground;
	    } else if(playgroundName == "ObjDemo") {
	    	Playground *playground = new ObjDemoPlayground(repository);
			playground->withName("ObjDemo")->run();
			delete playground;
	    } else if(playgroundName == "collisiondetection") {
	    	Playground *playground = new CollisionDetectionPlayground(repository);
			playground->withName("Collision Detection Playground")->run();
			delete playground;
	    } else if(playgroundName == "terrain") {
	    	Playground *playground = new PlaygroundTerrainDemo(repository);
			playground->withName("Terrain Demo")->run();
			delete playground;
	    } else if(playgroundName == "physics") {
	    	Playground *playground = new PhysicsPlayground(repository);
			playground->withName("Physics")->run();
			delete playground;
	    } else if(playgroundName == "roadfighter") {
	    	Playground *playground = new RoadFighter(repository);
			playground->withName("Road Fighter")->run();
			delete playground;
	    } else {
	    	printf("Unknown playground [%s]\n", playgroundName.c_str());
	    }
	}

    printf("done\n");
    return 0;
}
