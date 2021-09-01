#pragma once
#include <string>
#include <vector>

#include "custom-types/shared/macros.hpp"
#include "custom-types/shared/coroutine.hpp"
#include "UnityEngine/MonoBehaviour.hpp"

DECLARE_CLASS_CODEGEN(GorillaFriends, WebVerified, UnityEngine::MonoBehaviour,
    DECLARE_INSTANCE_METHOD(void, Start);

    public:
        inline static std::vector<std::string> verifiedUserIds;
        inline static std::vector<std::string> m_listCurrentSessionFriends;
        custom_types::Helpers::Coroutine GetVerifiedModders();
)