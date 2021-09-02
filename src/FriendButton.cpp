#include "UnityEngine/MeshRenderer.hpp"
#include "UnityEngine/Time.hpp"
#include "UnityEngine/Color.hpp"
#include "UnityEngine/Collider.hpp"
#include "UnityEngine/PlayerPrefs.hpp"
#include "UnityEngine/GameObject.hpp"

#include "GlobalNamespace/GorillaTriggerColliderHandIndicator.hpp"
#include "GlobalNamespace/GorillaPlayerLineButton.hpp"
#include "GlobalNamespace/GorillaTagger.hpp"
#include "GlobalNamespace/VRRig.hpp"

#include "Photon/Realtime/Player.hpp"

#include "FriendButton.hpp"
#include "WebVerified.hpp"

#include "Logger.hpp"

DEFINE_TYPE(GorillaFriends, FriendButton);

UnityEngine::Color friendColour = UnityEngine::Color(0.8f, 0.5f, 0.9f, 1.0f);
UnityEngine::Color verifiedColour = UnityEngine::Color(0.5f, 1.0f, 0.5f, 1.0f);

static bool IsVerified(std::string userID)
{
    Log::INFO("Checking if player is verified");
    int listsize = GorillaFriends::WebVerified::verifiedUserIds.size();
    for(int i = 0; i < listsize; ++i)
    {
        if(GorillaFriends::WebVerified::verifiedUserIds[i] == userID) return true;
    }
    Log::INFO("Player was not verified");
    return false;
}

static bool IsFriend(const std::string& userID)
{
    return (UnityEngine::PlayerPrefs::GetInt(il2cpp_utils::newcsstr(userID + "_friend"), 0) != 0);
}

static bool IsInFriendList(const std::string& userID)
{
    int listsize = GorillaFriends::WebVerified::m_listCurrentSessionFriends.size();
    for(int i = 0; i < listsize; ++i)
    {
        if(GorillaFriends::WebVerified::m_listCurrentSessionFriends[i] == userID) return true;
    }
    return false;
}

namespace GorillaFriends{

    //initalizing static variables
    Il2CppString* FriendButton::isFriendStr = nullptr;
    Il2CppString* FriendButton::notFriendStr = nullptr;
    UnityEngine::Material* FriendButton::isFrMaterial = nullptr;
    UnityEngine::Material* FriendButton::notFrMaterial = nullptr;

    void FriendButton::Awake()
    {
        updateTime = UnityEngine::Time::get_time() + 0.5f;
        touchTime = 0.0f;

        amFriend = false;

        myText = nullptr;
        meshRenderer = nullptr;
        playerLineText = nullptr;
        playerRigText = nullptr;
    }

    void FriendButton::Start()
    {
        // custom types doesn't work with serialization, so grabbing everything in here
        GlobalNamespace::GorillaPlayerLineButton* muteBtnScript = get_gameObject()->GetComponent<GlobalNamespace::GorillaPlayerLineButton*>();
        if(muteBtnScript != nullptr){
            myText = muteBtnScript->myText; // our text component
            parentLine = muteBtnScript->parentLine; // line this is attached to

            UnityEngine::GameObject::Destroy(muteBtnScript); // don't need this anymore

            if(parentLine != nullptr){
                userID = to_utf8(csstrtostr(parentLine->linePlayer->get_UserId())); // save the usersID
                playerLineText = parentLine->playerName; // players name component on the player line
                playerRigText = parentLine->playerVRRig->playerText; // players name component on the players vrrig

                // if this is local player, disable button & script
                if(parentLine->linePlayer->IsLocal) get_gameObject()->SetActive(false);
            }
        }

        meshRenderer = get_gameObject()->GetComponent<UnityEngine::MeshRenderer*>();
        std::string myLoguserIdLog = "userID= " + userID;
        Log::D_INFO(myLoguserIdLog.c_str());

        // check if the usersID already exists in the friends list
        bool idExists = false;
        size_t listLength = GorillaFriends::WebVerified::m_listCurrentSessionFriends.size();
        for(size_t pos = 0; pos < listLength; pos++){
            if(GorillaFriends::WebVerified::m_listCurrentSessionFriends[pos] == userID) {
                idExists = true;
                amFriend = true;
                break;
            }
        }

        // if user currently isn't in friends list, add it
        if(!idExists && IsFriend(userID)){
             GorillaFriends::WebVerified::m_listCurrentSessionFriends.push_back(userID);
             amFriend = true;
        }
        UpdateColour();
    }

    void FriendButton::OnTriggerEnter(UnityEngine::Collider* collider)
    {
        if(touchTime > UnityEngine::Time::get_time()) return;
        touchTime = UnityEngine::Time::get_time() + 0.25;

        bool refAreNull = false;
        GlobalNamespace::GorillaTriggerColliderHandIndicator* component = collider->GetComponent<GlobalNamespace::GorillaTriggerColliderHandIndicator*>();
        GlobalNamespace::GorillaTagger* gtInstance = GlobalNamespace::GorillaTagger::get_Instance();
        if(component == nullptr) { Log::WARNING("FriendButton::OnTriggerEnter > component = nullptr"); refAreNull = true; }
        if(gtInstance == nullptr){ Log::WARNING("FriendButton::OnTriggerEnter > gorillataggerinstance = nullptr"); refAreNull = true; }
        if(refAreNull) return;

        // vibrate the controllers
        gtInstance->StartVibration(component->_get_isLeftHand(), gtInstance->_get_tapHapticStrength() / 2.0f, gtInstance->_get_tapHapticDuration());

        // if not already a friend, add them
        if(!IsFriend(userID)){
            GorillaFriends::WebVerified::m_listCurrentSessionFriends.push_back(userID);
            UnityEngine::PlayerPrefs::SetInt(il2cpp_utils::newcsstr(userID + "_friend"), 1);
            amFriend = true;
        }

        // otherwise, remove them from friends list
        else {
            // change taken from killmans pull request
            auto it = GorillaFriends::WebVerified::m_listCurrentSessionFriends.begin();
            auto end = GorillaFriends::WebVerified::m_listCurrentSessionFriends.end();
            while( it != end )
            {
                if(*it == userID)
                {
                    GorillaFriends::WebVerified::m_listCurrentSessionFriends.erase(it);
                    break;
                }
                ++it;
            }

            UnityEngine::PlayerPrefs::SetInt(il2cpp_utils::newcsstr(userID + "_friend"), 0);
            amFriend = false;
        }

        UpdateColour();
    }

    void FriendButton::UpdateColour()
    {
        Log::D_INFO("attempt to update colour");

        bool refAreNull = false;

        // if any of these happen to be null something is broken so do nothing
        if(myText == nullptr) { Log::WARNING("FriendButton::UpdateColour > myText = nullptr"); refAreNull = true; }
        if(meshRenderer == nullptr) { Log::WARNING("FriendButton::UpdateColour > meshRenderer = nullptr"); refAreNull = true; }
        if(playerLineText == nullptr) { Log::WARNING("FriendButton::UpdateColour > playerLineText = nullptr"); refAreNull = true; }
        if(playerRigText == nullptr) { Log::WARNING("FriendButton::UpdateColour > playerRigText = nullptr"); refAreNull = true; }
        if(refAreNull) return;

        // check state and set colours, purple for friend, white for not  friend
        if(amFriend){
            myText->set_text(isFriendStr);
            
            meshRenderer->set_material(isFrMaterial);
            playerLineText->set_color(friendColour);
            playerRigText->set_color(friendColour);

        }
        else {
            myText->set_text(notFriendStr);
            meshRenderer->set_material(notFrMaterial);

            // if user is verified make the names green instead of white
            if(IsVerified(userID)){
                playerLineText->set_color(verifiedColour);
                playerRigText->set_color(verifiedColour);
                return;
            }

            playerLineText->set_color(UnityEngine::Color::get_white());
            playerRigText->set_color(UnityEngine::Color::get_white());
        }
    }

    // for the other scoreboards to update there state, bad way to do it but i'm lazy, should really make ontriggerenter update relevant scoreboardlines
    void FriendButton::Update()
    {
        float time = UnityEngine::Time::get_time();
        if(updateTime > time) return;
        updateTime = time + 0.5f;

        //checks if state has changed
        if(amFriend != IsInFriendList(userID))
        {
            //update state
            amFriend = !amFriend;
            UpdateColour();
        }
    }
}