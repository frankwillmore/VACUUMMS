/* libraries/vacuumms_cpp/scene.cc */

#include <cstdlib>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <filesystem>

#include <vacuumms/scene.hh>
#include <vacuumms/configuration.hh>
#include <vacuumms/cavity.hh>

std::string Scene::generateContainerSDL()
{
	std::stringstream out;

	// headers
	
    out << "#include \"colors.inc\"\n";

    out << "background {color " << background_color << "}\n";

    out << "camera {location <" << camera_location[0] 
        << "," << camera_location[1] 
        << "," << camera_location[2] 
        << "> look_at <" << camera_look_at[0]
        << "," << camera_look_at[1] 
        << "," << camera_look_at[2] 
        << "> right 1.0 angle 45}\n";

    // ambient light

	if (ambient_light) out << "global_settings { ambient_light rgb <" << ambient_light << "," << ambient_light << "," << ambient_light << "> }\n"; 

    // apply other light sources
    
    std::string light_color = "White";

    for (size_t i = 0; i < light_sources.size(); i++)
    {
        std::vector<vacuumms_float> source = light_sources[i];
		out << "light_source{<" << source[0] 
			<< "," << source[1] 
			<< "," << source[2] 
			<< "> color " << light_color << "}\n";
    }
    
    // box
    
    if (show_box)
    {
		out << "cylinder { <0,0,0>, <" << box_dimensions[0] 
			<< ",0,0>, 0.1 open texture { pigment { color " << box_color 
			<< " } }}\n";
		out << "cylinder { <0,0,0>, <0," << box_dimensions[1] 
			<< ",0>, 0.1 open texture { pigment { color " << box_color 
			<< " } }}\n";
		out << "cylinder { <0,0,0>, <0,0," << box_dimensions[2] 
			<< ">, 0.1 open texture { pigment { color " << box_color 
			<< " } }}\n";
		out << "cylinder { <" << box_dimensions[0] 
			<< "," << box_dimensions[1] << ",0>, <" 
			<< box_dimensions[0] << ",0,0>, 0.1 open texture { pigment { color " 
			<< box_color << " } }}\n";
		out << "cylinder { <" << box_dimensions[0] 
			<< "," << box_dimensions[1] << ",0>, <0," 
			<< box_dimensions[1] << ",0>, 0.1 open texture { pigment { color " 
			<< box_color << " } }}\n";
		out << "cylinder { <" << box_dimensions[0] << "," 
			<< box_dimensions[1] << ",0>, <" << box_dimensions[0] << "," 
			<< box_dimensions[1] << "," << box_dimensions[2] << ">, 0.1 open texture { pigment { color " 
			<< box_color << " } }}\n";
		out << "cylinder { <0," << box_dimensions[1] 
			<< "," << box_dimensions[2] << ">, <0," << box_dimensions[1] 
			<< ",0>, 0.1 open texture { pigment { color " << box_color 
			<< " } }}\n";
		out << "cylinder { <0," << box_dimensions[1] 
			<< "," << box_dimensions[2] << ">, <" << box_dimensions[0] << "," << box_dimensions[1] 
			<< "," << box_dimensions[2] << ">, 0.1 open texture { pigment { color " 
			<< box_color << " } }}\n";
		out << "cylinder { <0," << box_dimensions[1] << "," << box_dimensions[2] 
			<< ">, <0,0," << box_dimensions[2] << ">, 0.1 open texture { pigment { color " 
			<< box_color << " } }}\n";
		out << "cylinder { <" << box_dimensions[0] << ",0," << box_dimensions[2] 
			<< ">, <" << box_dimensions[0] << ",0,0>, 0.1 open texture { pigment { color " 
			<< box_color << " } }}\n";
		out << "cylinder { <" << box_dimensions[0] << ",0," << box_dimensions[2] 
			<< ">, <0,0," << box_dimensions[2] << ">, 0.1 open texture { pigment { color " 
			<< box_color << " } }}\n";
		out << "cylinder { <" << box_dimensions[0] << ",0," << box_dimensions[2] 
			<< ">, <" << box_dimensions[0] << "," << box_dimensions[1] << "," << box_dimensions[2] 
			<< ">, 0.1 open texture { pigment { color " 
			<< box_color << " } }}\n";

		out << "sphere{<0, 0, 0>, .1 texture{ pigment {color %s}}}\n";
		out << "sphere{<0, 0, " << box_dimensions[2] 
			<< ">, .1 texture{ pigment {color %s}}}\n";
		out << "sphere{<0, " << box_dimensions[1] 
			<< " ,0>, .1 texture{ pigment {color %s}}}\n";
		out << "sphere{<0, " << box_dimensions[1] 
			<< " ," << box_dimensions[2] << ">, .1 texture{ pigment {color %s}}}\n";
		out << "sphere{<" << box_dimensions[0] 
			<< ", 0, 0>, .1 texture{ pigment {color %s}}}\n";
		out << "sphere{<" << box_dimensions[0] << ", 0, " 
			<< box_dimensions[2] << ">, .1 texture{ pigment {color %s}}}\n";
		out << "sphere{<" << box_dimensions[0] << ", " << box_dimensions[1] 
			<< ", 0>, .1 texture{ pigment {color %s}}}\n";
		out << "sphere{<" << box_dimensions[0] << ", " << box_dimensions[1] << ", " 
			<< box_dimensions[2] << ">, .1 texture{ pigment {color %s}}}\n";

        // end of SDL comment
        
        out << "# end of sceneSDL\n\n";
	}

	return out.str();
}

std::string SceneComponent::getComponentSDL()
{
    // Return an unit orange bubble centered at origin as default. 
    // return std::string("sphere{<0.0, 0.0, 0.0>, 1.0 texture{ pigment {color Orange  transmit 0.700000  }  finish {phong 0.700000}  } }\n");
   
std::cout << "SceneComponent.getComponentSDL()" << std::endl;
    return std::string("// empty scene component \n");
}

//FTW
ConfigurationComponent::ConfigurationComponent()
{
std::cout << "creating empty ConfigurationComponent\n";
}

ConfigurationComponent::ConfigurationComponent(Configuration _configuration)
{
//FTW
std::cout << "Creating configuration component.\n";
    configuration = _configuration;
}

std::string CavityComponent::getComponentSDL()
{
	std::stringstream sdl;
std::cout << "called ConfigurationComponent.getComponentSDL()" << std::endl;
std::cout << "generating component SDL for " << configuration.getSize() << "records\n";

    sdl << "// CavityComponent SDL" << std::endl;

    return sdl.str();
}

std::string ConfigurationComponent::getComponentSDL()
{
	std::stringstream sdl;

//FTW
std::cout << "called ConfigurationComponent.getComponentSDL()" << std::endl;
std::cout << "generating component SDL for " << configuration.getSize() << "records\n";

    std::string texture = "texture { pigment {color " + color 
        + " transmit " + std::to_string(transmit)
        + "} finish {phong " + std::to_string(phong) + "}}";

    // iterate over configuration records
    for (int i=0; i < configuration.getSize(); i++)
    {
        ConfigurationRecord record = configuration.recordAt(i);
        sdl << "sphere{<" << record.x << ", " 
			<< record.y << ", " << record.z 
			<< ">, " << 0.5 * record.sigma 
			<< " " << texture << "}\n";
    }

    return sdl.str();
}
    
void SceneComponent::setTransmit(vacuumms_float _transmit)
{
    transmit = _transmit;
}

void SceneComponent::setPhong(vacuumms_float _phong)
{
    phong = _phong;
}

void SceneComponent::setColor(std::string _color)
{
    color = _color;
}

void SceneComponent::setBoxDims(std::vector<vacuumms_float> _dims)
{
    box_dims = _dims;
}

void SceneComponent::setClip(int _clip)
{
    clip = _clip;
}

//FTW
CavityComponent::CavityComponent()
{
std::cout << "creating empty CavityComponent\n";
}

CavityComponent::CavityComponent(CavityConfiguration _configuration)
{
    configuration = _configuration;
}

#ifdef BUILD_CUDA_COMPONENTS
FVIComponent::FVIComponent(FVIX fvix)
{
    fvix = _fvix;
}
#endif

void Scene::setBoxDimensions(std::vector<vacuumms_float> dims)
{
	box_dimensions = dims;
}

std::vector<vacuumms_float> Scene::getBoxDimensions()
{
	return box_dimensions;
}

SceneComponent* Scene::componentAt(int i)
{
	return components[i];
}

size_t Scene::deleteComponentAt(int i)
{
    // This erase also destructs the SceneComponent object
    components.erase(components.begin() + i);
    return components.size();
}

size_t Scene::getNumberOfComponents()
{
	return components.size();
}

int Scene::addSceneComponent(SceneComponent* component)
{
std::cout<<"pushing SceneComponent " << std::endl;
    SceneComponent* heap = new SceneComponent(*component);
    components.push_back(heap);
	return components.size();
}

int Scene::addCavityComponent(CavityComponent* component)
{
std::cout<<"pushing CavityComponent " << std::endl;
    CavityComponent* heap = new CavityComponent(*component);
    components.push_back(heap);
	return components.size();
}

int Scene::addConfigurationComponent(ConfigurationComponent* component)
{
std::cout<<"pushing ConfigurationComponent " << std::endl;
    ConfigurationComponent* heap = new ConfigurationComponent(*component);
    components.push_back(heap);
	return components.size();
}

void Scene::setBackgroundColor(std::string color)
{
	background_color = color;
}

void Scene::setCameraLocation(std::vector<vacuumms_float> location)
{
	camera_location = location;
}

size_t Scene::addLightSource(std::vector<vacuumms_float> source, std::string color)
{
	light_sources.push_back(source);
    return light_sources.size();
}

void Scene::applyAmbientLight()
{
    ambient_light = 1;
}

size_t Scene::applyStandardLight()
{
	addLightSource({0,0,100}, "White");
	addLightSource({0,100,0}, "White");
	addLightSource({100,0,0}, "White");
	addLightSource({0,0,-100}, "White");
	addLightSource({0,-100,0}, "White");
	addLightSource({-100,0,0}, "White");
    return light_sources.size();
}

void Scene::setShowBox(int yn)
{
	show_box = yn;
}

void Scene::setBoxColor(std::string color)
{
	box_color = color;
}


// I/O
int Scene::createSceneFile(const char* filename)  // POV file
{
    std::ofstream scene_file(filename);
    if (scene_file.is_open()) // write it
    {
        // Container

        scene_file << generateContainerSDL();
        
        // Components

//std::cout << "components.size(): " << components.size() << std::endl;
        for (int i=0; i < components.size(); i++)
//        for (const auto* component : components)
//        for (auto* component : components)
        {
//std::cout << "dumping component #" << i << std::endl;
//            SceneComponent* component = components[i].get();
//std::cout << "FTW here " << i << std::endl;
//std::cout << "dumping component #" << (long)component << std::endl;
//            std::string sdl = component->getComponentSDL();
//std::cout << "got component sdl: " << sdl << std::endl;
   
//            scene_file << "// writing component " << component << std::endl;
//std::cout << "type: " << typeid(component).name() << std::endl;
//            scene_file << components[i]->getComponentSDL();

//std::cout << "dumping SDL: " << sdl << std::endl;
//std::cout << component.getComponentSDL();
//            scene_file << component.getComponentSDL();
//            scene_file << std::endl;
//std::cout << "component " << component << " has SDL: " << component->getComponentSDL() <<  "\n";
        }

        scene_file.close();

        return 0;
    }
    else
    {
        std::cout << "Could not write file " << filename << std::endl;
        return 1;
    }
}

int Scene::renderScene(const char* filename)      // PNG file
{
    std::string basename = std::filesystem::path(filename).stem().string();
    std::string pov_filename = basename + ".pov";

    createSceneFile(pov_filename.c_str());

    std::string command = "povray -W1920 -H1080 " + pov_filename;
    std::string rm_command = "rm -f " + pov_filename;

    // Render
    int result = std::system(command.c_str());
    if (result == 0) 
	{
        std::cout << "POV-Ray render completed successfully.\n";
    } 
	else 
	{
        std::cerr << "POV-Ray render failed with exit code: " << result << "\n";
    }

    // Clean up
    int rm_result = std::system(rm_command.c_str());
    if (rm_result == 0) 
	{
        std::cout << "POV-Ray temporary file deleted successfully.\n";
    } 
	else 
	{
        std::cerr << "POV-Ray temporary file could not be deleted, failed with exit code: " << rm_result << "\n";
    }

    return result;
}

Scene::~Scene()
{
    for (int i=0;i<components.size();i++) delete components[i];
}


// FTW testing stuff

void Scene::dumpComponents()
{
    for (int i=0; i<components.size(); i++)
    {
        components[i]->dump();
    }
}

void SceneComponent::dump()
{
    std::cout << "base Component\n";
}

void CavityComponent::dump()
{
    std::cout << "Cavity Component\n";
}

void ConfigurationComponent::dump()
{
    std::cout << "Configuration Component\n";
}
