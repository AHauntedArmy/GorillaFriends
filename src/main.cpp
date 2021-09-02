#include <string>
#include <vector>

#include "main.hpp"
#include "WebVerified.hpp"
#include "FriendButton.hpp"

#include "custom-types/shared/register.hpp"
#include "custom-types/shared/coroutine.hpp"

#include "GorillaLocomotion/Player.hpp"
#include "GlobalNamespace/GorillaTagger.hpp"
#include "GlobalNamespace/GorillaScoreBoard.hpp"
#include "GlobalNamespace/GorillaPlayerLineButton.hpp"
#include "GlobalNamespace/GorillaPlayerScoreboardLine.hpp"
#include "GlobalNamespace/GorillaTriggerColliderHandIndicator.hpp"
#include "GlobalNamespace/VRRig.hpp"

#include "UnityEngine/Transform.hpp"
#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/Collider.hpp"
#include "UnityEngine/Vector3.hpp"
#include "UnityEngine/PlayerPrefs.hpp"
#include "UnityEngine/Color.hpp"
#include "UnityEngine/UI/Text.hpp"

#include "Photon/Realtime/Player.hpp"
#include "Photon/Pun/PhotonNetwork.hpp"

#include "Logger.hpp"

static ModInfo modInfo; // Stores the ID and version of our mod, and is sent to the modloader upon startup

// Loads the config from disk using our modInfo, then returns it for use
Configuration& getConfig() {
    static Configuration config(modInfo);
    config.Load();
    return config;
}

// Called at the early stages of game loading
extern "C" void setup(ModInfo& info) {
    info.id = ID;
    info.version = VERSION;
    modInfo = info;
	
    getConfig().Load(); // Load the config file
    Log::LoadLogger(modInfo);
    Log::INFO("Completed setup!");
}

GorillaFriends::WebVerified* webVerified;

MAKE_HOOK_MATCH(Player_start, &GorillaLocomotion::Player::Awake, void, GorillaLocomotion::Player* self)
{
    //GRAB LIST OF VERIFIED USER IDS
   Log::INFO("Making WebVerified Object");
    auto go = UnityEngine::GameObject::New_ctor(il2cpp_utils::newcsstr("WebVerified"));
    webVerified = go->AddComponent<GorillaFriends::WebVerified*>();
    webVerified->StartCoroutine(reinterpret_cast<System::Collections::IEnumerator*>(custom_types::Helpers::CoroutineHelper::New(webVerified->GetVerifiedModders())));
    Player_start(self);
}

UnityEngine::GameObject* m_pScoreboardFriendBtn = nullptr;
GorillaFriends::FriendButton* m_pFriendButtonController = nullptr;


MAKE_HOOK_MATCH(GSB_Awake, &GlobalNamespace::GorillaScoreBoard::Awake, void, GlobalNamespace::GorillaScoreBoard* self)
{
    if(m_pScoreboardFriendBtn != nullptr)
    {
		// Friend Button prefab has been created, skip our code
        GSB_Awake(self);
        return;
    }

    Log::INFO("GSB_AWAKE");
    auto transform = self->scoreBoardLinePrefab->get_transform();
    GlobalNamespace::GorillaPlayerScoreboardLine* parentLine = transform->get_gameObject()->GetComponent<GlobalNamespace::GorillaPlayerScoreboardLine*>();

    int transformNum = transform->get_childCount();
    for(int i = 0; i < transformNum; ++i)
    {
        Log::INFO("ooga");
        auto t = transform->GetChild(i);
        if(to_utf8(csstrtostr(t->get_name())) == "Mute Button")
        {
            Log::INFO("found mute button");

            m_pScoreboardFriendBtn = UnityEngine::GameObject::Instantiate(t->get_gameObject()); // clone our buttom from the mute button
            if(m_pScoreboardFriendBtn != nullptr)
            {
                t->set_localPosition(UnityEngine::Vector3(3.8f, 0.0f, 0.0f)); // move mut button sideways a tad

                m_pScoreboardFriendBtn->get_transform()->set_parent(self->scoreBoardLinePrefab->get_transform()); // set out parent to be the line we're on all positions and scale will be an offset to parents transform
                m_pScoreboardFriendBtn->get_transform()->set_name(il2cpp_utils::newcsstr("FriendButton"));
                m_pScoreboardFriendBtn->get_transform()->set_localPosition(UnityEngine::Vector3(17.5f, 0.0f, 0.0f));
                auto controller = m_pScoreboardFriendBtn->GetComponent<GlobalNamespace::GorillaPlayerLineButton*>();
                if(controller != nullptr)
                {
                    m_pFriendButtonController = m_pScoreboardFriendBtn->AddComponent<GorillaFriends::FriendButton*>();

                    //creating the materials
                    m_pFriendButtonController->notFrMaterial = UnityEngine::Material::New_ctor(controller->offMaterial);
                    m_pFriendButtonController->isFrMaterial = UnityEngine::Material::New_ctor(controller->onMaterial);
                    m_pFriendButtonController->isFrMaterial->set_color(UnityEngine::Color(0.8f, 0.5f, 0.9f, 1.0f));
                }
            }
			// We're added a Friend Button! Break our "for loop"
			break;
        }
    }

	// Call original function
    GSB_Awake(self);
}

MAKE_HOOK_MATCH(PN_Disconnect, &Photon::Pun::PhotonNetwork::Disconnect, void)
{
    Log::D_INFO("photonnetwork disconnect hook called, clearing friendslist");
    GorillaFriends::WebVerified::m_listCurrentSessionFriends.clear();
    PN_Disconnect();
}

// Called later on in the game loading - a good time to install function hooks
extern "C" void load() {
    il2cpp_functions::Init();
    
    Log::INFO("Installing hookeroos");

    Log::myLogger->info("hmm");

    Logger& logger = Log::GetLogger();
    INSTALL_HOOK(logger, Player_start);
    INSTALL_HOOK(logger, GSB_Awake);
    INSTALL_HOOK(logger, PN_Disconnect);

    Log::INFO("registering custom types");
    custom_types::Register::AutoRegister();

    // creating the button text string first
    GorillaFriends::FriendButton::isFriendStr = il2cpp_utils::newcsstr("FRIEND!");
    GorillaFriends::FriendButton::notFriendStr = il2cpp_utils::newcsstr("ADD\nFRIEND");
}