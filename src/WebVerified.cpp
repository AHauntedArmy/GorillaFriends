#include <string>
#include <vector>
#include <sstream>

#include "WebVerified.hpp"
#include "System/Collections/IEnumerator.hpp"
#include "custom-types/shared/coroutine.hpp"

#include "UnityEngine/Networking/UnityWebRequest.hpp"
#include "UnityEngine/Networking/DownloadHandler.hpp"

#include "System/Text/Encoding.hpp"

#include "Logger.hpp"

DEFINE_TYPE(GorillaFriends, WebVerified);

namespace GorillaFriends{

    void WebVerified::Start()
    {
        Log::INFO("Started coroutine...");
        this->StartCoroutine(reinterpret_cast<System::Collections::IEnumerator*>(custom_types::Helpers::CoroutineHelper::New(GetVerifiedModders())));
    }

    custom_types::Helpers::Coroutine GorillaFriends::WebVerified::GetVerifiedModders()
    {
        auto webRequest = UnityEngine::Networking::UnityWebRequest::Get(il2cpp_utils::newcsstr("https://github.com/RusJJ/GorillaFriends/raw/main/gorillas.verified"));
        co_yield reinterpret_cast<System::Collections::IEnumerator*>(webRequest->SendWebRequest());
        auto data = webRequest->get_downloadHandler()->get_data();
        auto responsecs = System::Text::Encoding::get_Default()->GetString(data);
        auto response = to_utf8(csstrtostr(responsecs));
        Log::INFO(response.c_str());

        std::istringstream instring(response);
        std::string line;
        while(std::getline(instring, line))
        {
            verifiedUserIds.push_back(line);
        }

        Log::INFO("Ended coroutine.");
        co_return;
    }
}