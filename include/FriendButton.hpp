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
    //DECLARE_INSTANCE_FIELD_DEFAULT(GlobalNamespace::GorillaPlayerScoreboardLine*, parentLine, nullptr);
    DECLARE_INSTANCE_FIELD(GlobalNamespace::GorillaPlayerScoreboardLine*, parentLine);
    //DECLARE_INSTANCE_FIELD_DEFAULT(bool, isOn, false);
    DECLARE_INSTANCE_FIELD(bool, isOn);
    //DECLARE_STATIC_FIELD(Il2CppString*, offText);
    //DECLARE_STATIC_FIELD(Il2CppString*, onText);
    //DECLARE_INSTANCE_FIELD_DEFAULT(UnityEngine::UI::Text*, myText, nullptr);
    DECLARE_INSTANCE_FIELD(UnityEngine::UI::Text*, myText);
    //DECLARE_INSTANCE_FIELD_DEFAULT(float, touchTime, 0.0f);
    DECLARE_INSTANCE_FIELD(float, touchTime);
    //DECLARE_STATIC_FIELD(UnityEngine::Material*, offMaterial);
    //DECLARE_STATIC_FIELD(UnityEngine::Material*, onMaterial);
    //DECLARE_INSTANCE_FIELD_DEFAULT(UnityEngine::MeshRenderer*, meshRenderer, nullptr);
    DECLARE_INSTANCE_FIELD(UnityEngine::MeshRenderer*, meshRenderer);
    //DECLARE_INSTANCE_FIELD_DEFAULT(bool, initialized, false);
    DECLARE_INSTANCE_FIELD(bool, initialized);
    //DECLARE_INSTANCE_FIELD_DEFAULT(float, nextUpdate, 0.0f);
    DECLARE_INSTANCE_FIELD(float, nextUpdate);
    
    DECLARE_INSTANCE_METHOD(void, Start);
    DECLARE_INSTANCE_METHOD(void, Update);
    DECLARE_INSTANCE_METHOD(void, UpdateColor);
    DECLARE_INSTANCE_METHOD(void, OnTriggerEnter, UnityEngine::Collider* collider);

    public:
    static Il2CppString* offText;
    static Il2CppString* onText;
    static UnityEngine::Material* offMaterial;
    static UnityEngine::Material* onMaterial;
)