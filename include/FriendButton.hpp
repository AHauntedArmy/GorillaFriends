#include <string>
#include <vector>

#include "WebVerified.hpp"

#include "custom-types/shared/macros.hpp"
#include "UnityEngine/MonoBehaviour.hpp"
#include "UnityEngine/UI/Text.hpp"
#include "UnityEngine/Material.hpp"
#include "UnityEngine/Collider.hpp"
#include "UnityEngine/MeshRenderer.hpp"
#include "GlobalNamespace/GorillaPlayerScoreboardLine.hpp"

DECLARE_CLASS_CODEGEN(GorillaFriends, FriendButton, UnityEngine::MonoBehaviour,
    DECLARE_INSTANCE_FIELD(GlobalNamespace::GorillaPlayerScoreboardLine*, parentLine);
    DECLARE_INSTANCE_FIELD(UnityEngine::UI::Text*, myText);
    DECLARE_INSTANCE_FIELD(UnityEngine::MeshRenderer*, meshRenderer);
    
    DECLARE_INSTANCE_METHOD(void, Awake);
    DECLARE_INSTANCE_METHOD(void, Start);
    DECLARE_INSTANCE_METHOD(void, Update);
    DECLARE_INSTANCE_METHOD(void, OnTriggerEnter, UnityEngine::Collider* collider);

    // c++ implementation, don't need these as c# types
    public:
    static Il2CppString* isFriendStr;
    static Il2CppString* notFriendStr;
    static UnityEngine::Material* isFrMaterial;
    static UnityEngine::Material* notFrMaterial;

    std::string userID;
    float updateTime;
    float touchTime;
    bool amFriend;

    UnityEngine::UI::Text* playerLineText;
    UnityEngine::UI::Text* playerRigText;

    void UpdateColour();
)