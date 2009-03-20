// For conditions of distribution and use, see copyright notice in license.txt

#include "StableHeaders.h"
#include <Ogre.h>
#include "OgreRenderingModule.h"
#include <Poco/ClassLibrary.h>
#include "Foundation.h"
#include "ComponentRegistrarInterface.h"
#include "ServiceManager.h"
#include "EC_OgrePlaceable.h"
#include "EC_OgreMesh.h"

namespace OgreRenderer
{
    OgreRenderingModule::OgreRenderingModule() : ModuleInterface_Impl(type_static_), framework_(NULL)
    {
    }

    OgreRenderingModule::~OgreRenderingModule()
    {
    }

    // virtual
    void OgreRenderingModule::Load()
    {
        using namespace OgreRenderer;

        LogInfo("Module " + Name() + " loaded.");
        DECLARE_MODULE_EC(EC_OgrePlaceable);
        DECLARE_MODULE_EC(EC_OgreMesh);
    }

    // virtual
    void OgreRenderingModule::Unload()
    {
        LogInfo("Module " + Name() + " unloaded.");
    }

    // virtual
    void OgreRenderingModule::Initialize(Foundation::Framework *framework)
    {
        framework_  = framework;
        
        renderer_ = OgreRenderer::RendererPtr(new OgreRenderer::Renderer(framework));
        renderer_->Initialize();
        
        framework_->GetServiceManager()->RegisterService(Foundation::Service::ST_Renderer, renderer_.get());

        LogInfo("Module " + Name() + " initialized.");
    }

    // virtual
    void OgreRenderingModule::PostInitialize(Foundation::Framework *framework)
    {
        Console::CommandService *console = framework->GetService<Console::CommandService>
            (Foundation::Service::ST_ConsoleCommand);

        console->RegisterCommand("SetViewportColor", "Set viewport background color. Usage: SetViewportColor(r,g,b)", Console::Bind(this, &OgreRenderingModule::ConsoleSetViewportColor));

        //Foundation::SceneManagerServiceInterface *scene_manager = 
        //    framework_->GetService<Foundation::SceneManagerServiceInterface>(Foundation::Service::ST_SceneManager);
        //assert(scene_manager != NULL && "Failed to get SceneManager service");
        //
        //if (scene_manager->HasScene("World") == false)
        //    scene_manager->CreateScene("World");
        //Foundation::ScenePtr scene = scene_manager->GetScene("World");
        //
        //Foundation::EntityPtr entity = scene->CreateEntity();
        //Foundation::ComponentPtr placeable_ptr = framework_->GetComponentManager()->CreateComponent(EC_OgrePlaceable::Name());
        //Foundation::ComponentPtr mesh_ptr = framework_->GetComponentManager()->CreateComponent(EC_OgreMesh::Name());
        //entity->AddEntityComponent(placeable_ptr);
        //entity->AddEntityComponent(mesh_ptr);
        //
        //EC_OgrePlaceable* placeable = static_cast<EC_OgrePlaceable*>(placeable_ptr.get());
        //EC_OgreMesh* mesh = static_cast<EC_OgreMesh*>(mesh_ptr.get());
        //placeable->SetPosition(Core::Vector3df(0,0,-200));
        //mesh->SetPlaceable(placeable_ptr);
        //mesh->SetMesh("ogrehead.mesh");
        
    }

    // virtual 
    void OgreRenderingModule::Uninitialize(Foundation::Framework *framework)
    {        
        framework->GetServiceManager()->UnregisterService(renderer_.get());
    
        renderer_.reset();

        framework_ = NULL;
        
        LogInfo("Module " + Name() + " uninitialized.");
    }
    
    // virtual
    void OgreRenderingModule::Update()
    {
        renderer_->Update();
    }

    Console::CommandResult OgreRenderingModule::ConsoleSetViewportColor(const Core::StringVector &params)
    {
        
        if (params.size() != 3)
        {
            return Console::ResultFailure("Usage: SetViewportColor(r,g,b)");
        }

        Ogre::ColourValue color;
        try
        {
            color = Ogre::ColourValue(Core::ParseString<Core::Real>(params[0]), Core::ParseString<Core::Real>(params[1]), Core::ParseString<Core::Real>(params[2]));
        } catch (std::exception)
        {
            return Console::ResultInvalidParameters();
        }

        Ogre::Camera *camera = renderer_->GetCurrentCamera();
        if (camera == NULL)
            return Console::ResultFailure("No camera or viewport");

        camera->getViewport()->setBackgroundColour(color);
        return Console::ResultSuccess();
    }
}

using namespace OgreRenderer;

POCO_BEGIN_MANIFEST(Foundation::ModuleInterface)
   POCO_EXPORT_CLASS(OgreRenderingModule)
POCO_END_MANIFEST

