/* vacuumms/scene.hh */

#pragma once

#include <vector>
//#include <typeinfo>

#include <vacuumms/types.h>

#include <vacuumms/configuration.hh>
#include <vacuumms/cavity.hh>

#ifdef BUILD_CUDA_COMPONENTS
    #include <vacuumms/fvi.hh>
#endif

#include <vacuumms/exports.hh>


class 
#ifdef PYBIND11_EXPORTS 
PYBIND11_EXPORT 
#endif
SceneComponent
{
    public:

        //FTW virtual std::string getComponentSDL() = 0;
        virtual std::string getComponentSDL();
        virtual ~SceneComponent() = default;

        void setTransmit(vacuumms_float);
        void setPhong(vacuumms_float);
        void setColor(std::string);
        void setBoxDims(std::vector<vacuumms_float>);
        void setClip(int);

virtual void dump();
        
    protected:

        vacuumms_float transmit;
        vacuumms_float phong;
        std::string color;
        std::vector<vacuumms_float> box_dims;
        int clip; // intersect with box
};

class 
#ifdef PYBIND11_EXPORTS 
PYBIND11_EXPORT 
#endif
ConfigurationComponent : public SceneComponent
{
    public:

        ConfigurationComponent(Configuration);
        ConfigurationComponent();
        std::string getComponentSDL();

void dump();

    protected:

        Configuration configuration;
    
};

class 
#ifdef PYBIND11_EXPORTS 
PYBIND11_EXPORT 
#endif
CavityComponent : public SceneComponent
{
    public:

        CavityComponent(CavityConfiguration);
        CavityComponent();
        std::string getComponentSDL();

void dump();

    protected:

        CavityConfiguration configuration;
    
};

#ifdef BUILD_CUDA_COMPONENTS

class 
#ifdef PYBIND11_EXPORTS 
PYBIND11_EXPORT 
#endif
FVIComponent : public SceneComponent
{
    public:

        FVIComponent(FVIX fvix);
        std::string getComponentSDL();

    protected:
  
        FVIX fvix;
};

#endif


class 
#ifdef PYBIND11_EXPORTS 
PYBIND11_EXPORT 
#endif
Scene
{
    public:

        // I/O
        int createSceneFile(const char* filename);  // POV file
        int renderScene(const char* filename);      // PNG file
        std::string generateContainerSDL();

        int addSceneComponent(SceneComponent&);
        int addCavityComponent(CavityComponent&);
        int addConfigurationComponent(ConfigurationComponent&);
#ifdef BUILD_CUDA_COMPONENTS
        int addFVIComponent(FVIComponent&);
#endif
        std::vector<SceneComponent*> components;
        void dumpComponents();

        SceneComponent* componentAt(int i);
        size_t deleteComponentAt(int i);
        size_t getNumberOfComponents();

        void setBoxDimensions(std::vector<vacuumms_float>);
        std::vector<vacuumms_float> getBoxDimensions();

        void setBackgroundColor(std::string);
        void setCameraLocation(std::vector<vacuumms_float>);
        void setCameraLookAt(std::vector<vacuumms_float>);
        size_t addLightSource(std::vector<vacuumms_float>, std::string color);
        size_t applyStandardLight();
        void applyAmbientLight();
        void setShowBox(int);
        void setBoxColor(std::string);
        ~Scene();

    private:

        int ambient_light = 0;
        int show_box = 0;

        std::vector<std::vector<vacuumms_float>> light_sources;
        
        std::vector<vacuumms_float> camera_location = {40, 40, 40};
        std::vector<vacuumms_float> camera_look_at = {0, 0, 0};
        std::vector<vacuumms_float> box_dimensions = {10, 10, 10};

        std::string light_color = "White";
        std::string box_color = "Yellow";
        std::string background_color = "Black";

#ifdef BUILD_PYBIND_BINDINGS
        pybind11::str __repr__();
#endif

};

