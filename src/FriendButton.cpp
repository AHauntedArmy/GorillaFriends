#include "UnityEngine/MeshRenderer.hpp"
#include "UnityEngine/Time.hpp"
#include "UnityEngine/Color.hpp"
#include "UnityEngine/Collider.hpp"
#include "UnityEngine/PlayerPrefs.hpp"
#include "UnityEngine/GameObject.hpp"

#include "GlobalNamespace/GorillaTriggerColliderHandIndicator.hpp"
#include "GlobalNamespace/GorillaTagger.hpp"
#include "GlobalNamespace/VRRig.hpp"

#include "Photon/Realtime/Player.hpp"

#include "FriendButton.hpp"
#include "WebVerified.hpp"

#include "Logger.hpp"

DEFINE_TYPE(GorillaFriends, FriendButton);

UnityEngine::Color m_clrFriendColor = UnityEngine::Color(0.8f, 0.5f, 0.9f, 1.0f);
UnityEngine::Color m_clrVerifiedColor = UnityEngine::Color(0.5f, 1.0f, 0.5f, 1.0f);

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

static bool IsFriend(std::string userID)
{
    return (UnityEngine::PlayerPrefs::GetInt(il2cpp_utils::createcsstr(userID + "_friend"), 0) != 0);
}

static bool IsInFriendList(std::string userID)
{
    int listsize = GorillaFriends::WebVerified::m_listCurrentSessionFriends.size();
    for(int i = 0; i < listsize; ++i)
    {
        if(GorillaFriends::WebVerified::m_listCurrentSessionFriends[i] == userID) return true;
    }
    return false;
}

namespace GorillaFriends{
    Il2CppString* FriendButton::onText = nullptr;
    Il2CppString* FriendButton::offText = nullptr;
    UnityEngine::Material* FriendButton::onMaterial = nullptr;
    UnityEngine::Material* FriendButton::offMaterial = nullptr;

    void FriendButton::Start()
    {
        meshRenderer = get_gameObject()->GetComponent<UnityEngine::MeshRenderer*>();
        parentLine = GetComponentInParent<GlobalNamespace::GorillaPlayerScoreboardLine*>();
    }

    void FriendButton::Update()
    {
        Log::INFO("Checking whether to run or not");
        if(nextUpdate > UnityEngine::Time::get_time()) return;

        if(parentLine == nullptr){
            //Log::WARNING("parentLine is nullptr");
            return;
        }

        GlobalNamespace::VRRig* playersVRRig = parentLine->_get_playerVRRig();
        Photon::Realtime::Player* linesPlayer = parentLine->_get_linePlayer();

        if(playersVRRig == nullptr){
            Log::WARNING("parentLine->playerVRRig is nullptr");
            return;
        } 
        
        if(linesPlayer == nullptr){
            Log::WARNING("parentLine->linePlayer == nullptr");
            return;
        }

        nextUpdate = UnityEngine::Time::get_time() + 0.5f;

        std::string userID = to_utf8(csstrtostr(linesPlayer->get_UserId()));
        UnityEngine::UI::Text* playersName = parentLine->_get_playerName();

        if(!initialized)
        {
            Log::INFO("Wasn't initialized");
            initialized = true;

            //if(IsVerified(to_utf8(csstrtostr(parentLine->linePlayer->UserId))))
            if(IsVerified(userID))
            {
                Log::INFO("Was verified");
                //parentLine->playerName->set_color(m_clrVerifiedColor);
                playersName->set_color(m_clrVerifiedColor);
                //parentLine->playerVRRig->playerText->set_color(m_clrVerifiedColor);
                playersVRRig->_get_playerText()->set_color(m_clrVerifiedColor);
            }

            //if(parentLine->linePlayer->IsLocal) get_gameObject()->SetActive(false);
            if(linesPlayer->_get_IsLocal()) get_gameObject()->SetActive(false);
            else
            {
                Log::INFO("Checking if player is a freind");
                //if(IsFriend(to_utf8(csstrtostr(parentLine->linePlayer->UserId))))
                if(IsFriend(userID))
                {
                    Log::INFO("Was friend");
                    //if(!IsInFriendList(to_utf8(csstrtostr(parentLine->linePlayer->UserId)))) GorillaFriends::WebVerified::m_listCurrentSessionFriends.push_back(to_utf8(csstrtostr(parentLine->linePlayer->UserId)));
                    if(!IsInFriendList(userID)) WebVerified::m_listCurrentSessionFriends.push_back(userID);
                    //parentLine->playerName->set_color(m_clrFriendColor);
                    playersName->set_color(m_clrFriendColor);
                    isOn = true;
                    //myText->set_text(onText);
                    UpdateColor();
                }
                else
                {
                    Log::INFO("Wasn't friend");
                }
            }
            return;
        }

        //if(!parentLine->linePlayer->IsLocal && isOn != IsInFriendList(to_utf8(csstrtostr(parentLine->linePlayer->UserId))))
        if(linesPlayer->_get_IsLocal() && isOn != IsInFriendList(userID))
        {
            isOn = !isOn;
            UpdateColor();
            if(!isOn)
            {
                //if(IsVerified(to_utf8(csstrtostr(parentLine->linePlayer->UserId))))
                if(IsVerified(userID))
                {
                    //parentLine->playerName->set_color(m_clrVerifiedColor);
                    playersName->set_color(m_clrVerifiedColor);
                    //parentLine->playerVRRig->playerText->set_color(m_clrVerifiedColor);
                    playersVRRig->_get_playerText()->set_color(m_clrVerifiedColor);
                }
                else
                {
                    //parentLine->playerName->set_color(UnityEngine::Color::get_white());
                    playersName->set_color(UnityEngine::Color::get_white());
                    //parentLine->playerVRRig->playerText->set_color(UnityEngine::Color::get_white());
                    playersVRRig->_get_playerText()->set_color(UnityEngine::Color::get_white());
                }
                //myText->set_text(offText);
            }
            else
            {
                //parentLine->playerName->set_color(m_clrFriendColor);
                playersName->set_color(m_clrFriendColor);
                //parentLine->playerVRRig->playerText->set_color(m_clrFriendColor);
                playersVRRig->_get_playerText()->set_color(m_clrFriendColor);
                //myText->set_text(onText);
            }
        }
    }

    void FriendButton::OnTriggerEnter(UnityEngine::Collider* collider)
    {
        if(touchTime > UnityEngine::Time::get_time()) return;
        touchTime = UnityEngine::Time::get_time() + 0.25;
        GlobalNamespace::GorillaTriggerColliderHandIndicator* component = collider->GetComponent<GlobalNamespace::GorillaTriggerColliderHandIndicator*>();
        if(component == nullptr) return;

        isOn = !isOn;
        UpdateColor();
        //GlobalNamespace::GorillaTagger::get_Instance()->StartVibration(component->isLeftHand, GlobalNamespace::GorillaTagger::get_Instance()->tapHapticStrength / 2.0f, GlobalNamespace::GorillaTagger::get_Instance()->tapHapticDuration);
        GlobalNamespace::GorillaTagger* gtInstance = GlobalNamespace::GorillaTagger::get_Instance();
        if(gtInstance == nullptr){
            Log::WARNING("OnTriggerEnter > gorillataggerinstance = nullptr");
            return;
        }
        gtInstance->StartVibration(component->_get_isLeftHand(), gtInstance->_get_tapHapticStrength() / 2.0f, gtInstance->_get_tapHapticDuration());

        if(parentLine == nullptr){
            Log::WARNING("OnTriggerEnter > parentLine = nullptr");
            return;
        }

        Photon::Realtime::Player* linesPlayer = parentLine->_get_linePlayer();
        UnityEngine::UI::Text* playersName = parentLine->_get_playerName();

        if(linesPlayer == nullptr){
            Log::WARNING("OnTriggerEnter > parentlines player = nullptr");
            return;
        }

        if(playersName == nullptr){
            Log::WARNING("OnTriggerEnter > parentlines playername = nullptr");
            return;
        }

        std::string userID = to_utf8(csstrtostr(linesPlayer->get_UserId()));

        if(isOn)
        {
            //GorillaFriends::WebVerified::m_listCurrentSessionFriends.push_back(to_utf8(csstrtostr(parentLine->linePlayer->UserId)));
            WebVerified::m_listCurrentSessionFriends.push_back(userID);
            //UnityEngine::PlayerPrefs::SetInt(il2cpp_utils::createcsstr(to_utf8(csstrtostr(parentLine->linePlayer->UserId)) + "_friend"), 1);
            UnityEngine::PlayerPrefs::SetInt(il2cpp_utils::createcsstr(userID + "_friend"), 1);
            //parentLine->playerName->set_color(m_clrFriendColor);
            Log::INFO("adding friend > set player name color");
            playersName->set_color(m_clrFriendColor);
            //myText->set_text(onText);
            return;
        }

        // change taken from killmans pull request
        auto it = GorillaFriends::WebVerified::m_listCurrentSessionFriends.begin();
        auto end = GorillaFriends::WebVerified::m_listCurrentSessionFriends.end();
        while( it != end )
        {
            //if(*it == to_utf8(csstrtostr(parentLine->linePlayer->UserId)))
            if(*it == userID)
            {
                GorillaFriends::WebVerified::m_listCurrentSessionFriends.erase(it);
                break;
            }
            ++it;
        }

        /*
        int listsize = GorillaFriends::WebVerified::m_listCurrentSessionFriends.size();
        for(int i = 0; i < listsize; ++i)
        {
            if(GorillaFriends::WebVerified::m_listCurrentSessionFriends[i] == to_utf8(csstrtostr(parentLine->linePlayer->UserId)))
            {
                GorillaFriends::WebVerified::m_listCurrentSessionFriends.erase(GorillaFriends::WebVerified::m_listCurrentSessionFriends.begin() + i - 1);
            }
        }
        */

        GlobalNamespace::VRRig* playersVRRig = parentLine->_get_playerVRRig();
        if(playersVRRig == nullptr){
           Log::WARNING("OnTriggerEnter > parentlines playerVRRig = nullptr");
           return;
       }

        //UnityEngine::PlayerPrefs::SetInt(il2cpp_utils::createcsstr(to_utf8(csstrtostr(parentLine->linePlayer->UserId)) + "_friend"), 0);
        UnityEngine::PlayerPrefs::SetInt(il2cpp_utils::createcsstr(userID + "_friend"), 0);
        //myText->set_text(offText);

        //if(IsVerified(to_utf8(csstrtostr(parentLine->linePlayer->UserId))))
        if(IsVerified(userID))
        {
            //parentLine->playerName->set_color(m_clrVerifiedColor);
            Log::INFO("player is verified > set playername colour");
            playersName->set_color(m_clrVerifiedColor);
            //parentLine->playerVRRig->playerText->set_color(m_clrVerifiedColor);
            Log::INFO("player is verified > set player vrrig name colour");
            playersVRRig->_get_playerText()->set_color(m_clrVerifiedColor);
        }
        else
        {
            //parentLine->playerName->set_color(UnityEngine::Color::get_white());
            Log::INFO("player is verified > set playername colour");
            playersName->set_color(UnityEngine::Color::get_white());
            //parentLine->playerVRRig->playerText->set_color(UnityEngine::Color::get_white());
            Log::INFO("player is not verified > set player vrrig name colour");
            playersVRRig->_get_playerText()->set_color(UnityEngine::Color::get_white());
        }
    }

    void FriendButton::UpdateColor()
    {
        if(isOn)
        {
            if(meshRenderer != nullptr) meshRenderer->set_material(onMaterial);
            if(myText == nullptr){
                Log::WARNING("myText is nullptr??");
                return;
            }   
            myText->set_text(onText);
        }
        else
        {
            if(meshRenderer != nullptr) meshRenderer->set_material(offMaterial);
            if(myText == nullptr){
                Log::WARNING("myText is nullptr??");
                return;
            }   
            myText->set_text(offText);
        }
    }
}